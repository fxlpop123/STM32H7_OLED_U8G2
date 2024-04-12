/*
*********************************************************************************************************
*
*	ģ������ : OLED��ʾ������ģ��
*	�ļ����� : bsp_oled.c
*	��    �� : V1.1
*	˵    �� : OLED ��(128x64)�ײ�������������оƬ�ͺ�Ϊ SSD1306.  �����������ʺ���STM32-V5������.
*				OLEDģ�����FSMC�����ϡ�
*	�޸ļ�¼ :
*		�汾��  ����        ����    ˵��
*		V1.0    2013-02-01 armfly  ��ʽ����
*		V1.1    2015-10-14 armfly  ���24����32�����ֺ�ASCII֧��
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "fonts.h"

/* ����2������ѡ 1��; ��ʾ��ʾ���� */
//#define DIR_NORMAL			/* ���б�ʾ������ʾ���� */
#define DIR_180					/* ���б�ʾ��ת180�� */


/* 12864 OLED���Դ澵��ռ��1K�ֽ�. ��8�У�ÿ��128���� */
static uint8_t s_ucGRAM[8][128];

/* Ϊ�˱���ˢ����Ļ��̫ǿ������ˢ����־��
0 ��ʾ��ʾ����ֻ��д����������д����1 ��ʾֱ��д����ͬʱд�������� */
static uint8_t s_ucUpdateEn = 1;

static void OLED_WriteCmd(uint8_t _ucCmd);
static void OLED_WriteData(uint8_t _ucData);
static void OLED_BufToPanel(void);

/*
*********************************************************************************************************
*	�� �� ��: OLED_InitHard
*	����˵��: ��ʼ��OLED��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_InitHard(void)
{
	OLED_ConfigGPIO();

	SSD_CS_1();
	SSD_RS_1();

	/* �ϵ��ӳ� */
	SSD_RST_0();
	bsp_DelayMS(50);
	SSD_RST_1();

	 /* ģ�鳧���ṩ��ʼ������ */
	OLED_WriteCmd(0xAE);	/* �ر�OLED�����ʾ(����) */
	OLED_WriteCmd(0x00);	/* �����е�ַ��4bit */
	OLED_WriteCmd(0x10);	/* �����е�ַ��4bit */
	OLED_WriteCmd(0x40);	/* ������ʼ�е�ַ����5bit 0-63���� Ӳ�����*/

	OLED_WriteCmd(0x81);	/* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
	OLED_WriteCmd(0xFF);	/* ���öԱȶȲ���,ȱʡCF */

#ifdef DIR_NORMAL
	OLED_WriteCmd(0xA0);	/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
	OLED_WriteCmd(0xC0);	/* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
#endif

#ifdef DIR_180
	OLED_WriteCmd(0xA1);	/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
	OLED_WriteCmd(0xC8);	/* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
#endif

	OLED_WriteCmd(0xA6);	/* A6 : ����������ʾģʽ; A7 : ����Ϊ����ģʽ */

	OLED_WriteCmd(0xA8);	/* ����COM·�� */
	OLED_WriteCmd(0x3F);	/* 1 ->��63+1��· */

	OLED_WriteCmd(0xD3);	/* ������ʾƫ�ƣ�˫�ֽ����*/
	OLED_WriteCmd(0x00);	/* ��ƫ�� */

	OLED_WriteCmd(0xD5);	/* ������ʾʱ�ӷ�Ƶϵ��/��Ƶ�� */
	OLED_WriteCmd(0xB0);	/* ���÷�Ƶϵ��,��4bit�Ƿ�Ƶϵ������4bit����Ƶ�� */

	OLED_WriteCmd(0xD9);	/* ����Ԥ������� */
	OLED_WriteCmd(0x22);	/* [3:0],PHASE 1; [7:4],PHASE 2; */

	OLED_WriteCmd(0xDA);	/* ����COM��Ӳ�����߷�ʽ */
	OLED_WriteCmd(0x12);

	OLED_WriteCmd(0xDB);	/* ���� vcomh ��ѹ���� */
	OLED_WriteCmd(0x3C);	/* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */

	OLED_WriteCmd(0x8D);	/* ���ó��ã����¸�������ʹ�ã� */
	OLED_WriteCmd(0x14);	/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
	OLED_WriteCmd(0xAF);	/* ��OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispOn
*	����˵��: ����ʾ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispOn(void)
{
	OLED_WriteCmd(0x8D);	/* ���ó��ã����¸�������ʹ�ã� */
	OLED_WriteCmd(0x14);	/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
	OLED_WriteCmd(0xAF);	/* ��OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispOff
*	����˵��: �ر���ʾ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispOff(void)
{
	OLED_WriteCmd(0x8D);	/* ���ó��ã����¸�������ʹ�ã�*/
	OLED_WriteCmd(0x10);	/* 0x14 ʹ�ܳ��ã�0x10 �ǹر� */
	OLED_WriteCmd(0xAE);	/* ��OLED��� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_SetDir
*	����˵��: ������ʾ����
*	��    ��: _ucDir = 0 ��ʾ��������1��ʾ��ת180��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_SetDir(uint8_t _ucDir)
{
	if (_ucDir == 0)
	{
       	OLED_WriteCmd(0xA0);	/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
		OLED_WriteCmd(0xC0);	/* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
	}
	else
	{
		OLED_WriteCmd(0xA1);	/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
		OLED_WriteCmd(0xC8);	/* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_SetContrast
*	����˵��: ���öԱȶ�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_SetContrast(uint8_t ucValue)
{
	OLED_WriteCmd(0x81);	/* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
	OLED_WriteCmd(ucValue);	/* ���öԱȶȲ���,ȱʡCF */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_StartDraw
*	����˵��: ��ʼ��ͼ���Ժ��ͼ����ֻ��д������������д����Դ�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_StartDraw(void)
{
	s_ucUpdateEn = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_EndDraw
*	����˵��: ������ͼ��������������ˢ�µ�����Դ档 OLED_StartDraw() �� OLED_EndDraw() ����ɶ�ʹ��
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_EndDraw(void)
{
	s_ucUpdateEn = 1;
	OLED_BufToPanel();
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ClrScr
*	����˵��: ����
*	��    ��:  _ucMode : 0 ��ʾȫ�ڣ� 0xFF��ʾȫ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_ClrScr(uint8_t _ucMode)
{
	uint8_t i,j;

	for (i = 0 ; i < 8; i++)
	{
		for (j = 0 ; j < 128; j++)
		{
			s_ucGRAM[i][j] = _ucMode;
		}
	}

	if (s_ucUpdateEn == 1)
	{
		OLED_BufToPanel();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_BufToPanel
*	����˵��: ���������еĵ�������д�����
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void OLED_BufToPanel(void)
{
	uint8_t i,j;

	for (i = 0 ; i< 8; i++)
	{
		OLED_WriteCmd (0xB0 + i);	/* ����ҳ��ַ��0~7�� */
		OLED_WriteCmd (0x00);		/* �����е�ַ�ĵ͵�ַ */
		OLED_WriteCmd (0x10);		/* �����е�ַ�ĸߵ�ַ */

		for (j = 0 ; j < 128; j++)
		{
			OLED_WriteData(s_ucGRAM[i][j]);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DispStr
*	����˵��: ����Ļָ�����꣨���Ͻ�Ϊ0��0����ʾһ���ַ���
*	��    ��:
*		_usX : X���꣬����12864������ΧΪ��0 - 127��
*		_usY : Y���꣬����12864������ΧΪ��0 - 63��
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address = 0;
	uint8_t buf[32 * 32 / 8];	/* ���֧��32������ */
	uint16_t m;
	uint8_t width;
	uint8_t font_width,font_height, font_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	uint8_t line_bytes;
	uint8_t asc_bytes;
	uint8_t hz_bytes;
	uint8_t fGT = 0;

#ifdef USE_SMALL_FONT		
	const uint8_t *pHzDot = g_Hz12;
#else	
	uint32_t AddrHZK;
#endif	

	if (_tFont->FontCode == FC_ST_12)
	{
		font_height = 12;
		font_width = 12;
		font_bytes = 24;
		hz_bytes = 2;
		asc_bytes = 1;
		fGT = 0;
		pAscDot = g_Ascii12;
	#ifdef USE_SMALL_FONT		
		pHzDot = g_Hz12;
	#else		
		AddrHZK = HZK12_ADDR;
	#endif		
	}
	else if (_tFont->FontCode == FC_ST_16)
	{
		/* ȱʡ��16���� */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		hz_bytes = 2;
		asc_bytes = 1;
		fGT = 0;
		pAscDot = g_Ascii16;
	#ifdef USE_SMALL_FONT		
		pHzDot = g_Hz16;
	#else
		AddrHZK = HZK16_ADDR;
	#endif
	}
	else if (_tFont->FontCode == FC_ST_24)
	{
		font_height = 24;
		font_width = 24;
		font_bytes = 72;
		hz_bytes = 3;
		asc_bytes = 2;
		fGT = 1;
		pAscDot = g_Ascii24;
	#ifdef USE_SMALL_FONT		
		pHzDot = g_Hz24;
	#else
		AddrHZK = HZK24_ADDR;
	#endif
	}
	else	// if (_tFont->FontCode == FC_ST_32)
	{
		font_height = 32;
		font_width = 32;
		font_bytes = 128;
		hz_bytes = 4;
		asc_bytes = 2;
		fGT = 1;
		pAscDot = g_Ascii32;
	#ifdef USE_SMALL_FONT		
		pHzDot = g_Hz32;
	#else
		AddrHZK = HZK32_ADDR;
	#endif
	}

	/* ��ʼѭ�������ַ� */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
		if (code1 < 0x80)
		{
			/* ��ascii�ַ������Ƶ�buf */
			if (fGT == 1)	/* ����ASCII���������� */
			{
				m = 0;
				while(1)
				{
					address = m * (asc_bytes * font_height + 1);
					m++;
					if (code1 == pAscDot[address + 0])
					{
						address += 1;
						memcpy(buf, &pAscDot[address], asc_bytes * font_height);
						break;
					}
					else if ((pAscDot[address + 0] == 0xFF) && (pAscDot[address + 1] == 0xFF))
					{
						/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
					
				}
			}
			else	/* ȫASCII�����ֱ�Ӽ����ַ */
			{
				memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
			}
			width = font_width / 2;
			line_bytes = asc_bytes;
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}

			/* ����16�����ֵ����ַ
				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
				;
			*/
			#ifdef USE_SMALL_FONT
				m = 0;
				while(1)
				{
					address = m * (font_bytes + 2);
					m++;
					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
					{
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
					}
					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
					{
						/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* ��ȫ�ֿ� */
				/* �˴���Ҫ�����ֿ��ļ����λ�ý����޸� */
				if (code1 >=0xA1 && code1 <= 0xA9 && code2 >=0xA1)
				{
					address = ((code1 - 0xA1) * 94 + (code2 - 0xA1)) * font_bytes + AddrHZK;
				}
				else if (code1 >=0xB0 && code1 <= 0xF7 && code2 >=0xA1)
				{
					address = ((code1 - 0xB0) * 94 + (code2 - 0xA1) + 846) * font_bytes + AddrHZK;
				}
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
				line_bytes = hz_bytes;
		}

		y = _usY;
		/* ��ʼˢLCD */
		for (m = 0; m < font_height; m++)	/* �ַ��߶� */
		{
			x = _usX;
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((buf[m * line_bytes + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					OLED_PutPixel(x, y, _tFont->FrontColor);	/* ����������ɫΪ����ɫ */
				}
				else
				{
					OLED_PutPixel(x, y, _tFont->BackColor);	/* ����������ɫΪ���ֱ���ɫ */
				}

				x++;
			}
			y++;
		}

		if (_tFont->Space > 0)
		{
			/* ������ֵ�ɫ��_tFont->usBackColor�������ּ����ڵ���Ŀ�ȣ���ô��Ҫ������֮�����(��ʱδʵ��) */
		}
		_usX += width + _tFont->Space;	/* �е�ַ���� */
		_ptr++;			/* ָ����һ���ַ� */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_PutPixel
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_ucColor  ��������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;

	const uint8_t aOrTab[8]  = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
	const uint8_t aAndTab[8] = {0xFE, 0xFD, 0xFB, 0xF7,0xEF,0xDF,0xBF,0x7F};

	ucPageAddr = _usY / 8;
	ucColAddr = _usX;

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (_ucColor == 0)
	{
		ucValue &= aAndTab[_usY % 8];
	}
	else
	{
		ucValue |= aOrTab[_usY % 8];
	}
	s_ucGRAM[ucPageAddr][ucColAddr] = ucValue;

	if (s_ucUpdateEn == 1)
	{
		OLED_WriteCmd (0xB0 + ucPageAddr);					/* ����ҳ��ַ��0~7�� */
		OLED_WriteCmd (0x00 + (ucColAddr & 0x0F));			/* �����е�ַ�ĵ͵�ַ */
		OLED_WriteCmd (0x10 + ((ucColAddr >> 4) & 0x0F));	/* �����е�ַ�ĸߵ�ַ */
		OLED_WriteData(ucValue);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_GetPixel
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*	�� �� ֵ: ��ɫֵ (0, 1)
*********************************************************************************************************
*/
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint8_t ucValue;
	uint8_t ucPageAddr;
	uint8_t ucColAddr;

	ucPageAddr = _usY / 8;
	ucColAddr = _usX;

	ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
	if (ucValue & (_usY % 8))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawLine
*	����˵��: ���� Bresenham �㷨����2��仭һ��ֱ�ߡ�
*	��    ��:
*			_usX1, _usY1 ����ʼ������
*			_usX2, _usY2 ����ֹ��Y����
*			_ucColor     ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* ���� Bresenham �㷨����2��仭һ��ֱ�� */

	OLED_PutPixel(_usX1 , _usY1 , _ucColor);

	/* ��������غϣ���������Ķ�����*/
	if ( _usX1 == _usX2 && _usY1 == _usY2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 > _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 > _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if ( dx < dy )   /*���dyΪ�Ƴ������򽻻��ݺ����ꡣ*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* ȷ������1���Ǽ�1 */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* ѭ������ */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			OLED_PutPixel ( y , x , _ucColor) ;
		}
		else
		{
			OLED_PutPixel ( x , y , _ucColor) ;
		}
		x += tx ;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawPoints
*	����˵��: ���� Bresenham �㷨������һ��㣬������Щ�����������������ڲ�����ʾ��
*	��    ��:
*			x, y     ����������
*			_ucColor ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		OLED_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _ucColor);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawRect
*	����˵��: ���ƾ��Ρ�
*	��    ��:
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
	/*
	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	OLED_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _ucColor);	/* �� */
	OLED_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _ucColor);	/* �� */

	OLED_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _ucColor);	/* �� */
	OLED_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _ucColor);	/* �� */
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawCircle
*	����˵��: ����һ��Բ���ʿ�Ϊ1������
*	��    ��:
*			_usX,_usY  ��Բ�ĵ�����
*			_usRadius  ��Բ�İ뾶
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* ��ǰ X ֵ */
	uint32_t  CurY;		/* ��ǰ Y ֵ */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		OLED_PutPixel(_usX + CurX, _usY + CurY, _ucColor);
		OLED_PutPixel(_usX + CurX, _usY - CurY, _ucColor);
		OLED_PutPixel(_usX - CurX, _usY + CurY, _ucColor);
		OLED_PutPixel(_usX - CurX, _usY - CurY, _ucColor);
		OLED_PutPixel(_usX + CurY, _usY + CurX, _ucColor);
		OLED_PutPixel(_usX + CurY, _usY - CurX, _ucColor);
		OLED_PutPixel(_usX - CurY, _usY + CurX, _ucColor);
		OLED_PutPixel(_usX - CurY, _usY - CurX, _ucColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_DrawBMP
*	����˵��: ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
*	��    ��:
*			_usX, _usY : ͼƬ������
*			_usHeight  ��ͼƬ�߶�
*			_usWidth   ��ͼƬ���
*			_ptr       ����ɫͼƬ����ָ�룬ÿ������ռ��1���ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr)
{
	uint16_t x, y;

	for (x = 0; x < _usWidth; x++)
	{
		for (y = 0; y < _usHeight; y++)
		{
			OLED_PutPixel(_usX + x, _usY + y, *_ptr);
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_ConfigGPIO
*	����˵��: ����OLED���ƿ��ߣ�����Ϊ8λ80XX���߿���ģʽ��SPIģʽ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_ConfigGPIO(void)
{
	GPIO_InitTypeDef gpio_init_struct;

	OLED_SPI_RST_CLK_ENABLE();
    OLED_SPI_CS_CLK_ENABLE();
    OLED_SPI_RS_CLK_ENABLE();
    OLED_SPI_SCLK_CLK_ENABLE();
    OLED_SPI_SDIN_CLK_ENABLE();
	
    
    gpio_init_struct.Pin = OLED_SPI_RST_PIN;                /* RST���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;        /* ���� */
    HAL_GPIO_Init(OLED_SPI_RST_PORT, &gpio_init_struct);    /* RST����ģʽ���� */

    gpio_init_struct.Pin = OLED_SPI_CS_PIN;                 /* CS���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;        /* ���� */
    HAL_GPIO_Init(OLED_SPI_CS_PORT, &gpio_init_struct);     /* CS����ģʽ���� */

    gpio_init_struct.Pin = OLED_SPI_RS_PIN;                 /* RS���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;        /* ���� */
    HAL_GPIO_Init(OLED_SPI_RS_PORT, &gpio_init_struct);     /* RS����ģʽ���� */

    gpio_init_struct.Pin = OLED_SPI_SCLK_PIN;               /* SCLK���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;        /* ���� */
    HAL_GPIO_Init(OLED_SPI_SCLK_PORT, &gpio_init_struct);   /* SCLK����ģʽ���� */

    gpio_init_struct.Pin = OLED_SPI_SDIN_PIN;               /* SDIN����ģʽ���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;        /* ���� */
    HAL_GPIO_Init(OLED_SPI_SDIN_PORT, &gpio_init_struct);   /* SDIN����ģʽ���� */

	//SSD_SCK_1();
	//SSD_SDIN_1();
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_WriteCmd
*	����˵��: ��SSD1306����һ�ֽ�����
*	��    ��:  ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void OLED_WriteCmd(uint8_t _ucCmd)
{
	uint8_t i;

	SSD_CS_0();

	SSD_RS_0();	/* 0 ��ʾ���洫�͵������� 1��ʾ���洫�͵����� */


	for (i = 0; i < 8; i++)
	{
		SSD_SCK_0();
		SSD_SCK_0();
		if (_ucCmd & 0x80)
		{
			SSD_SDIN_1();
		}
		else
		{
			SSD_SDIN_0();
		}
		_ucCmd <<= 1;
		SSD_SCK_1();
		SSD_SCK_1();
	}

	SSD_CS_1();
	SSD_RS_1();
}

/*
*********************************************************************************************************
*	�� �� ��: OLED_WriteData
*	����˵��: ��SSD1306����һ�ֽ�����
*	��    ��:  ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void OLED_WriteData(uint8_t _ucData)
{
	uint8_t i;

	SSD_CS_0();

	SSD_RS_1();	/* 0 ��ʾ���洫�͵������� 1��ʾ���洫�͵����� */

	for (i = 0; i < 8; i++)
	{
		SSD_SCK_0();
		SSD_SCK_0();
		if (_ucData & 0x80)
		{
			SSD_SDIN_1();
		}
		else
		{
			SSD_SDIN_0();
		}
		_ucData <<= 1;
		SSD_SCK_1();
		SSD_SCK_1();
	}

	SSD_CS_1();
	SSD_RS_1();
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
