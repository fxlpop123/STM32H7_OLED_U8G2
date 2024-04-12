/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : LCD_tft_lcd.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _BSP_OLED_H
#define _BSP_OLED_H


#if 1
	#include "bsp_tft_lcd.h"	/* 仅使用了其中的 FONT_T 结构体 */
#else
	typedef struct
	{
		uint16_t FontCode;	/* 字体代码 0 表示16点阵 */
		uint16_t FrontColor;/* 字体颜色 */
		uint16_t BackColor;	/* 文字背景颜色，透明 */
		uint16_t Space;		/* 文字间距，单位 = 像素 */
	}FONT_T;
#endif

/*SPI 模式接线定义 (只需要6根杜邦线连接)  本例子采用软件模拟SPI时序*/
#define OLED_SPI_RST_PORT               GPIOA
#define OLED_SPI_RST_PIN                GPIO_PIN_15
#define OLED_SPI_RST_CLK_ENABLE()       do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define OLED_SPI_CS_PORT                GPIOB
#define OLED_SPI_CS_PIN                 GPIO_PIN_7
#define OLED_SPI_CS_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define OLED_SPI_RS_PORT                GPIOB
#define OLED_SPI_RS_PIN                 GPIO_PIN_4
#define OLED_SPI_RS_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define OLED_SPI_SCLK_PORT              GPIOC
#define OLED_SPI_SCLK_PIN               GPIO_PIN_6
#define OLED_SPI_SCLK_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

#define OLED_SPI_SDIN_PORT              GPIOC
#define OLED_SPI_SDIN_PIN               GPIO_PIN_7
#define OLED_SPI_SDIN_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

/* 定义IO = 1和 0的代码 （不用更改） */
#define SSD_RST_1() 					OLED_SPI_RST_PORT->BSRRL = OLED_SPI_RST_PIN
#define SSD_RST_0() 					OLED_SPI_RST_PORT->BSRRH = OLED_SPI_RST_PIN

#define SSD_CS_1() 						OLED_SPI_CS_PORT->BSRRL = OLED_SPI_CS_PIN
#define SSD_CS_0() 						OLED_SPI_CS_PORT->BSRRH = OLED_SPI_CS_PIN

#define SSD_RS_1() 						OLED_SPI_RS_PORT->BSRRL = OLED_SPI_RS_PIN
#define SSD_RS_0() 						OLED_SPI_RS_PORT->BSRRH = OLED_SPI_RS_PIN

#define SSD_SCK_1() 					OLED_SPI_SCLK_PORT->BSRRL = OLED_SPI_SCLK_PIN
#define SSD_SCK_0() 					OLED_SPI_SCLK_PORT->BSRRH = OLED_SPI_SCLK_PIN

#define SSD_SDIN_1() 					OLED_SPI_SDIN_PORT->BSRRL = OLED_SPI_SDIN_PIN
#define SSD_SDIN_0() 					OLED_SPI_SDIN_PORT->BSRRH = OLED_SPI_SDIN_PIN	
	
	
/* 可供外部模块调用的函数 */
void OLED_ConfigGPIO(void);
void OLED_InitHard(void);
void OLED_DispOn(void);
void OLED_DispOff(void);
void OLED_SetDir(uint8_t _ucDir);
void OLED_SetContrast(uint8_t ucValue);
void OLED_StartDraw(void);
void OLED_EndDraw(void);
void OLED_ClrScr(uint8_t _ucMode);
void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor);
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY);
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor);
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor);
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor);
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor);
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr);

#endif


