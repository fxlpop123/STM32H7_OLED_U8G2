/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : OLED��ʾʵ�֡�
*              ʵ��Ŀ�ģ�
*                1. ѧϰOLED�������ͽ������á�
*              ʵ�����ݣ�
*                ÿ�����������������
*                1. ҡ�����¼� �� ���ڶԱȶȡ�
*                2. ҡ�����Ҽ� �� �л���ʾ���档
*                3. ҡ��OK��   �� �л���ʾ������ʾ���ݷ�ת180��)��
*              ע�����
*                1. OLED���õ�FMC�ӿڣ�8bit��ʽ��
*                2. ��ʵ���Ƽ�ʹ�ô������SecureCRT�鿴��ӡ��Ϣ��������115200������λ8����żУ��λ�ޣ�ֹͣλ1��
*                3. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��   ����         ����        ˵��
*		V1.0    2019-07-15   Eric2013     1. CMSIS����汾 V5.4.0
*                                         2. HAL��汾 V1.3.0
*
*	Copyright (C), 2018-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/	
#include "bsp.h"			 /* �ײ�Ӳ������ */
#include "u8g2.h"

uint8_t u8g2_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
  switch(msg)
  {
    //Initialize SPI peripheral
	case U8X8_MSG_GPIO_AND_DELAY_INIT:
      OLED_ConfigGPIO();
      break;
	//Function which implements a delay, arg_int contains the amount of ms
	case U8X8_MSG_DELAY_MILLI:
      bsp_DelayMS(arg_int);
      break;
	//Function which delays 10us
    case U8X8_MSG_DELAY_10MICRO:
	  bsp_DelayUS(arg_int);
	  break;
	//Function which delays 100ns
    case U8X8_MSG_DELAY_100NANO:
      __NOP();;
      break;
    //Function to define the logic level of the clockline
	case U8X8_MSG_GPIO_SPI_CLOCK:
      if(arg_int) 
	  	SSD_SCK_1();
	  else 
	  	SSD_SCK_0();
      break;
    //Function to define the logic level of the data line to the display
	case U8X8_MSG_GPIO_SPI_DATA:
  	  if(arg_int) 
	  	SSD_SDIN_1();
      else 
	  	SSD_SDIN_0();
	  break;
    // Function to define the logic level of the CS line
	case U8X8_MSG_GPIO_CS:
	  if(arg_int) 
	  	SSD_CS_1();
	  else 
	  	SSD_CS_0();
	  break;
	//Function to define the logic level of the Data/ Command line
	case U8X8_MSG_GPIO_DC:
	  if(arg_int) 
	  	SSD_RS_1();
	  else 
	  	SSD_RS_0();
	  break;
	//Function to define the logic level of the RESET line
	case U8X8_MSG_GPIO_RESET:
	  if(arg_int) 
	  	SSD_RST_1();
	  else 
	  	SSD_RST_0();
	  break;
	default:
	  return 0; //A message was received which is not implemented, return 0 to indicate an error
	}
	return 1; // command processed successfully.
}

void u8g2_init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay); // ��ʼ�� u8g2 �ṹ��
	u8g2_InitDisplay(u8g2);			// ������ѡ��оƬ���г�ʼ����������ʼ����ɺ���ʾ�����ڹر�״̬
	u8g2_SetPowerSave(u8g2, 0); 	// ������ʾ��
	u8g2_SetContrast(u8g2, 100);	// ���öԱȶ�
	u8g2_ClearBuffer(u8g2);			// �����ʾ����
}

/*�ٷ��ṩ��Logo����demo*/
void draw(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1); 				/*����ģʽѡ��*/
    u8g2_SetFontDirection(u8g2, 0); 		/*���巽��ѡ��*/
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*�ֿ�ѡ��*/
    u8g2_DrawStr(u8g2, 0, 20, "U");

    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21, 8, "8");

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51, 30, "g");
    u8g2_DrawStr(u8g2, 67, 30, "\xb2");

    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);

    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1, 54, "github.com/olikraus/u8g2");
}

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	u8g2_t u8g2;

	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
		����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM
	*/

	bsp_Init();			/* Ӳ����ʼ�� */
	u8g2_init(&u8g2);   /* u8g2��ʼ��*/

	while (1)
	{
		bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */
		
		u8g2_FirstPage(&u8g2);
		do
		{
			draw(&u8g2);
		} while (u8g2_NextPage(&u8g2)); 
	}
}




/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
