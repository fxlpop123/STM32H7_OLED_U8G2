/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_key.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

/* ����Ӧ�ó���Ĺ��������������� */
#define JOY_DOWN_U			KEY_1_DOWN		/* �� */
#define JOY_UP_U			KEY_1_UP
#define JOY_LONG_DOWN_U		KEY_1_LONG_DOWN
#define JOY_LONG_UP_U       KEY_1_LONG_UP
#define JOY_AUTO_U          KEY_1_AUTO_UP
#define JOY_DB_U            KEY_1_DB_UP

#define JOY_DOWN_D			KEY_2_DOWN		/* �� */
#define JOY_UP_D			KEY_2_UP
#define JOY_LONG_DOWN_D		KEY_2_LONG_DOWN
#define JOY_LONG_UP_D       KEY_2_LONG_UP
#define JOY_AUTO_D          KEY_2_AUTO_UP
#define JOY_DB_D            KEY_2_DB_UP

#define JOY_DOWN_L			KEY_3_DOWN		/* �� */
#define JOY_UP_L			KEY_3_UP
#define JOY_LONG_DOWN_L		KEY_3_LONG_DOWN
#define JOY_LONG_UP_L       KEY_3_LONG_UP
#define JOY_AUTO_L          KEY_3_AUTO_UP
#define JOY_DB_L            KEY_3_DB_UP

#define JOY_DOWN_R			KEY_4_DOWN		/* �� */
#define JOY_UP_R			KEY_4_UP
#define JOY_LONG_DOWN_R		KEY_4_LONG_DOWN
#define JOY_LONG_UP_R       KEY_4_LONG_UP
#define JOY_AUTO_R          KEY_4_AUTO_UP
#define JOY_DB_R            KEY_4_DB_UP

/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_JOY_U = 0,
	KID_JOY_D,
	KID_JOY_L,
	KID_JOY_R,
}KEY_ID_E;

/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* ��λ10ms�� ����1�룬��Ϊ�����¼� */

#define KEY_DB_CLICK_TIME       40      /* ˫�����ʱ������λms */
/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
	
    uint16_t DelayCount;    /* �ӳټ�����������˫����� */
    uint8_t ClickCount;     /* �������� */
    int32_t LastTime;       /* �ϴΰ���ʱ�� */
}KEY_T;

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
#define KEY_MSG_STEP    6   /* ������Ϣ���벽��(ÿ�����м����¼����룬˳�����У� */
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				/* 1������ */
    KEY_1_LONG_DOWN,    	/* 1������ */
    KEY_1_LONG_UP,      	/* 1��������ĵ��� */  
    KEY_1_AUTO_UP,      	/* 1���������Զ����� */
    KEY_1_DB_UP,        	/* 1��˫�� */ 

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,				/* 2������ */
    KEY_2_LONG_DOWN,    	/* 2������ */
    KEY_2_LONG_UP,      	/* 2��������ĵ��� */  
    KEY_2_AUTO_UP,      	/* 2���������Զ����� */
    KEY_2_DB_UP,        	/* 2��˫�� */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,				/* 3������ */
    KEY_3_LONG_DOWN,    	/* 3������ */
    KEY_3_LONG_UP,      	/* 3��������ĵ��� */  
    KEY_3_AUTO_UP,      	/* 3���������Զ����� */
    KEY_3_DB_UP,        	/* 3��˫�� */

	KEY_4_DOWN,				/* 4������ */
	KEY_4_UP,				/* 4������ */
    KEY_4_LONG_DOWN,    	/* 4������ */
    KEY_4_LONG_UP,      	/* 4��������ĵ��� */  
    KEY_4_AUTO_UP,      	/* 4���������Զ����� */
    KEY_4_DB_UP,        	/* 4��˫�� */
}KEY_ENUM;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	10
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitKey(void);
void bsp_KeyScan10ms(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
