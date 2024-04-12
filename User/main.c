/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : OLED显示实现。
*              实验目的：
*                1. 学习OLED的驱动和界面设置。
*              实验内容：
*                每个都测试了四种情况
*                1. 摇杆上下键 ： 调节对比度。
*                2. 摇杆左右键 ： 切换显示界面。
*                3. 摇杆OK键   ： 切换显示方向（显示内容翻转180度)。
*              注意事项：
*                1. OLED采用的FMC接口，8bit方式。
*                2. 本实验推荐使用串口软件SecureCRT查看打印信息，波特率115200，数据位8，奇偶校验位无，停止位1。
*                3. 务必将编辑器的缩进参数和TAB设置为4来阅读本文件，要不代码显示不整齐。
*
*	修改记录 :
*		版本号   日期         作者        说明
*		V1.0    2019-07-15   Eric2013     1. CMSIS软包版本 V5.4.0
*                                         2. HAL库版本 V1.3.0
*
*	Copyright (C), 2018-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/	
#include "bsp.h"			 /* 底层硬件驱动 */
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
	u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_sw_spi, u8g2_gpio_and_delay); // 初始化 u8g2 结构体
	u8g2_InitDisplay(u8g2);			// 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
	u8g2_SetPowerSave(u8g2, 0); 	// 唤醒显示器
	u8g2_SetContrast(u8g2, 100);	// 设置对比度
	u8g2_ClearBuffer(u8g2);			// 清除显示缓存
}

/*官方提供的Logo绘制demo*/
void draw(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1); 				/*字体模式选择*/
    u8g2_SetFontDirection(u8g2, 0); 		/*字体方向选择*/
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*字库选择*/
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
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	u8g2_t u8g2;

	/*
		ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
		配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM
	*/

	bsp_Init();			/* 硬件初始化 */
	u8g2_init(&u8g2);   /* u8g2初始化*/

	while (1)
	{
		bsp_Idle();		/* 这个函数在bsp.c文件。用户可以修改这个函数实现CPU休眠和喂狗 */
		
		u8g2_FirstPage(&u8g2);
		do
		{
			draw(&u8g2);
		} while (u8g2_NextPage(&u8g2)); 
	}
}




/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
