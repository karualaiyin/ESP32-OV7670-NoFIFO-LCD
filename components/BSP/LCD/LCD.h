#ifndef __LCD_h
#define __LCD_h

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "hardware_spi.h"

#define spi_id  1
//BL  3.3V
#define DC_PIN GPIO_NUM_4
#define CS_PIN GPIO_NUM_16
#define RES_PIN GPIO_NUM_32

#define RES(x)  gpio_set_level(RES_PIN,x)
#define	DC(x)  gpio_set_level(DC_PIN,x)
#define	CS(x)  gpio_set_level(CS_PIN,x)
#define Delay_ms(x)  vTaskDelay(pdMS_TO_TICKS(x))

//记得更改初始化代码里的GPIO	

#define line    128 	    //显示列数
#define column  128			//显示行数

//颜色
#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define BLUE2 		0x1c9f
#define PINK    	0xd8a7
#define ORANGE  	0xfa20
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  	0xFFE0
#define CYAN   		0x07ff
#define PURPLE 	 	0xf81f
#define PURPLE2 	0xdb92
#define PURPLE3 	0x8811
#define GRAY0   	0xEF7D
#define GRAY1   	0x8410
#define GRAY2   	0x4208


void LCD_IC_Init();
void LCD_Init();
void LCD_BlockWrite(uint16_t X_start,uint16_t Y_start,uint16_t X_end,uint16_t Y_end);
void LCD_SetCursor(uint16_t x,uint16_t y);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_FullScreen(uint16_t X_start,uint16_t Y_start,uint16_t X_end,uint16_t Y_end,uint16_t color);
void LCD_Clear(uint16_t color);
void LCD_ShowImage_LSB(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t *p);
void LCD_ShowImage_MSB(uint16_t x, uint16_t y,uint16_t length,uint16_t width,const uint8_t *p);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t size, char ch, uint16_t color, uint16_t bgcolor);
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t size,char *p,uint16_t color, uint16_t bgcolor);
uint32_t LCD_Pow(uint32_t X, uint32_t Y);
void LCD_ShowNum(uint8_t x,uint8_t y,uint16_t width,uint16_t height,uint16_t size,uint16_t color, uint16_t bgcolor,int32_t Number,uint8_t Length);


void LCD_SendBuffer(uint8_t *data, int len);
void LCD_ShowImage_Cropped(const uint8_t *src, int src_w, int src_h,
                           int x, int y, int dst_w, int dst_h,
                           int lcd_x, int lcd_y);
void LCD_ShowImage_CroppedCenter(const uint8_t *src, int src_w, int src_h,
                                 int dst_w, int dst_h,
                                 int lcd_x, int lcd_y);
#endif