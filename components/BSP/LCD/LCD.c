#include "LCD.h"
#include "LCD_Font.h"
#include "string.h"

static spi_device_handle_t spi_dev;  // 设备句柄
#define MySPI_SendByte(Byte)  hardware_spi_write_byte(spi_dev, Byte)
/**
  * 函    数：SPI起始
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Start(void)
{
	CS(0);				//拉低SS，开始时序
}

/**
  * 函    数：SPI终止
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Stop(void)
{
	CS(1);				//拉高SS，终止时序
}

void MYSPI_Send16b(uint16_t Data)
{
	MySPI_Start();

	DC(1);
	
	MySPI_SendByte(Data>>8);
	MySPI_SendByte(Data);
	
	MySPI_Stop();
}

void WriteComm(uint8_t Byte)
{
	MySPI_Start();
	
	DC(0);
	MySPI_SendByte(Byte);
	
	MySPI_Stop();
}
void WriteData(uint8_t Byte)
{
	MySPI_Start();
	
	DC(1);
	MySPI_SendByte(Byte);
	
	MySPI_Stop();
}


void LCD_SendBuffer(uint8_t *data, int len)
{
    MySPI_Start();

    DC(1);

    hardware_spi_write_buffer(spi_dev,data,len);

    MySPI_Stop();
}


void LCD_IC_Init()
{
	WriteComm(0x11); //Sleep out
	Delay_ms(120); //Delay 120ms
	//-------------------------ST7735S Frame Rate-----------------------------------------//
	
	WriteComm(0x36);
	WriteData(0x00);
	
	WriteComm(0x3A);
	WriteData(0x05);
	
	WriteComm(0xB1);
	WriteData(0x05);
	WriteData(0x3C);
	WriteData(0x3C);
	
	WriteComm(0xB2);
	WriteData(0x05);
	WriteData(0x3C);
	WriteData(0x3C);
	
	WriteComm(0xB3);
	WriteData(0x05);
	WriteData(0x3C);
	WriteData(0x3C);
	WriteData(0x05);
	WriteData(0x3C);
	WriteData(0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	WriteComm(0xB4); //Dot inversion
	WriteData(0x03);
	//------------------------------------ST7735S Power Sequence-----------------------------------------//
	WriteComm(0xC0);
	WriteData(0x2E);
	WriteData(0x06);
	WriteData(0x04);
	
	WriteComm(0xC1);
	WriteData(0xC0);
	WriteData(0xC2);

	WriteComm(0xC2);
	WriteData(0x0D);
	WriteData(0x0D);
	
	WriteComm(0xC3);
	WriteData(0x8D);
	WriteData(0xEE);
	
	WriteComm(0xC4);
	WriteData(0x8D);
	WriteData(0xEE);
	//---------------------End ST7735S Power Sequence-------------------------------------//
	WriteComm(0xC5); 	   //VCOM
	WriteData(0x0E);
	
	WriteComm(0x36); 
	WriteData(0x08);
	
	//----------------------ST7735S Gamma Sequence-----------------------------------------//
	WriteComm(0xE0);
	WriteData(0x1B);
	WriteData(0x21);
	WriteData(0x10);
	WriteData(0x15);
	WriteData(0x2B);
	WriteData(0x25);
	WriteData(0x1F);
	WriteData(0x23);
	WriteData(0x22);
	WriteData(0x22);
	WriteData(0x2B);
	WriteData(0x37);
	WriteData(0x00);
	WriteData(0x15);
	WriteData(0x02);
	WriteData(0x3F);
	
	WriteComm(0xE1);
	WriteData(0x1A);
	WriteData(0x20);
	WriteData(0x0F);
	WriteData(0x15);
	WriteData(0x2A);
	WriteData(0x25);
	WriteData(0x1E);
	WriteData(0x23);
	WriteData(0x23);
	WriteData(0x22);
	WriteData(0x2B);
	WriteData(0x37);
	WriteData(0x00);
	WriteData(0x15);
	WriteData(0x02);
	WriteData(0x3F);
	//---------------------End ST7735S Gamma Sequence-----------------------------------------//

	WriteComm(0x2C); 

	WriteComm(0x29); //Display on
	
}

void LCD_Init()
{

	hardware_spi_init(spi_id);

	spi_device_interface_config_t devcfg = 
	{
        .clock_speed_hz = 30 * 1000 * 1000,  // 15 MHz
        .mode = 0,                            // SPI mode 0
        .spics_io_num = -1,          		// CS 引脚
        .queue_size = 7,
    };
    spi_bus_add_device(spi_id, &devcfg, &spi_dev);  // 获取设备句柄

	gpio_config_t gpio_conf=
	{
		.intr_type=GPIO_INTR_DISABLE,
		.mode=GPIO_MODE_OUTPUT,
		.pin_bit_mask=(1ULL<<DC_PIN)|(1ULL<<CS_PIN)|(1ULL<<RES_PIN),
		.pull_down_en=GPIO_PULLDOWN_DISABLE,
		.pull_up_en=GPIO_PULLUP_DISABLE
	};
	gpio_config(&gpio_conf);

	CS(1);											//SS默认高电平
	
	RES(0);											//LCD复位
	Delay_ms(200);
	RES(1);
	Delay_ms(200);

	LCD_IC_Init();
}

void LCD_BlockWrite(uint16_t X_start,uint16_t Y_start,uint16_t X_end,uint16_t Y_end) //
{

	 WriteComm(0x2a);
	
	 MYSPI_Send16b(X_start);
	 MYSPI_Send16b(X_end);
	 
	 WriteComm(0x2b);
	
	 MYSPI_Send16b(Y_start);
	 MYSPI_Send16b(Y_end);

	 WriteComm(0x2c); 	
	
}

void LCD_SetCursor(uint16_t x,uint16_t y)
{
	LCD_BlockWrite(x,y,x,y);
}

void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetCursor(x,y);
	
	MySPI_Start();
	
	MYSPI_Send16b(color);
	
	MySPI_Stop();
}

void LCD_FullScreen(uint16_t X_start,uint16_t Y_start,uint16_t X_end,uint16_t Y_end,uint16_t color)
{
	LCD_BlockWrite(X_start,Y_start,X_end,Y_end);
	
	WriteComm(0x2c);
	
	for(int i=0;i<X_end-X_start;i++)
	{
		for(int j=0;j<Y_end-Y_start;j++)
		{
			MYSPI_Send16b(color);
		}
	}
	
}

void LCD_Clear(uint16_t color)
{
	LCD_BlockWrite(0,0,line-1,column-1);
	
	WriteComm(0x2c);
	
	for(int i=0;i<line;i++)
	{
		for(int j=0;j<column;j++)
		{
			MYSPI_Send16b(color);
		}
	}

}

/*
功能：显示图片
参数：图片左上角坐标，图长，图宽 （注意长宽不是坐标，128*160的图就填128 160，而非127，159 ）
      这里仅利用长宽计算图像像素点的个数，而非定位。哪怕少1，图像也会显示出错。

解释：定位区域后，然后将若干像素点输出即可，会自动换行。
      一个像素点占16位，需要2个unsigned chara数组的位置，输出像素点时，对连续2个元素进行位运算合成
      【注意】图像取模时，可勾选 低位或高位在前,但LCD屏幕要求高位在前，所以需要根据原始数据的顺序修改picH=p[2*i];
		picL=p[2*i+1]; 	
*/
void LCD_ShowImage_LSB(uint16_t x, uint16_t y, uint16_t length, uint16_t width, const uint8_t *p)
{
	LCD_BlockWrite(x,y,x+length-1,y+width-1);//定位区域
	
	uint8_t picH,picL;  
	
	for(int i=0;i<length*width;i++)
	{  
		//length*width表示像素点的个数，数组下标表示存下这些颜色的点需要多少8位元素
   	    picL=p[2*i];
		picH=p[2*i+1]; 						//2个元素记录一个颜色点
		
		WriteData(picH);
		WriteData(picL);
		
		//-------16位发送---------------------------------------------------------//
		//MYSPI_Send16b(picH<<8|picL);      //图像取模时若选则：低位在前（默认一般为此）
		//TFT_Send16Bit(picL<<8|picH);     //图像取模时若选则：高位在前
	}
	
	LCD_BlockWrite(0,0,127,127);//恢复窗口
	
}
void LCD_ShowImage_MSB(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t *p)
{

    LCD_BlockWrite(x,y,x+length-1,y+width-1);

    LCD_SendBuffer((uint8_t *)p,length*width*2);

}

void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t size, char ch, uint16_t color, uint16_t bgcolor)
{
    uint8_t i, j, byte;
    uint8_t *pFont;
    uint16_t charWidth, charHeight;
    
    // 根据字体大小选择字模和设置字符尺寸
    switch(size)
    {
        case 12:
            pFont = (uint8_t*)asc2_1206[ch - ' '];
            charWidth = 6;  // 12号字体宽度为6像素
            charHeight = 12; // 12号字体高度为12像素
            break;
        case 16:
            pFont = (uint8_t*)asc2_1608[ch - ' '];
            charWidth = 8;  // 16号字体宽度为8像素
            charHeight = 16; // 16号字体高度为16像素
            break;
        case 24:
            pFont = (uint8_t*)asc2_2412[ch - ' '];
            charWidth = 12; // 24号字体宽度为12像素
            charHeight = 24; // 24号字体高度为24像素
            break;
        default:
            return; // 不支持的字体大小
    }
    
    // 设置字符显示区域
    LCD_BlockWrite(x, y, x + charWidth - 1, y + charHeight - 1);
    
    // 遍历字符的每一行
    for(i = 0; i < charHeight; i++)
    {
        // 获取当前行的字模数据
        byte = pFont[i];
        
        // 遍历当前行的每一个像素
        for(j = 0; j < charWidth; j++)
        {
            // 检查当前位是否为1（前景色）
            if(byte&(0x80>>j))
            {
                MYSPI_Send16b(color); // 前景色
            }
            else
            {
                MYSPI_Send16b(bgcolor); // 背景色
            }
        }
    }
}

void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t size,char *p,uint16_t color, uint16_t bgcolor)
{ 
	
	uint16_t num=strlen(p);
	uint16_t x0=x,y0=y;
	
	for(int i=0;i<num;i++)
	{
		if((p[i]<='~')&&(p[i]>=' '))
		{
			if(x-x0+size/2>width)
			{
				x=x0;
				y=y+size;
			}
			if(y-y0+size>height)
			{
				break;
			}
			LCD_ShowChar(x,y,size,p[i],color,bgcolor);
			x=x+size/2;
		}
	}
}

uint32_t LCD_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result=1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void LCD_ShowNum(uint8_t x,uint8_t y,uint16_t width,uint16_t height,uint16_t size,uint16_t color, uint16_t bgcolor,int32_t Number,uint8_t Length)
{
	
	if(Number>0||Number==0)
	{
		char Data[Length];
		for (uint8_t i=0;i<Length;i++)							
		{
			Data[i]=Number/LCD_Pow(10,Length-i-1)%10+'0';
		}
		
		LCD_ShowString(x,y,width,height,size,Data,color,bgcolor);
	}
	else
	{
		Number=-Number;
		
		char Data[Length+1];
		
		Data[0]='-';
		
		for (uint8_t i=0;i<Length;i++)							
		{
			Data[i+1]=Number/LCD_Pow(10,Length-i-1)%10+'0';
		}
		
		LCD_ShowString(x,y,width,height,size,Data,color,bgcolor);
	}
}

/**
 * @brief 零拷贝：直接从源图像裁剪并显示到 LCD，无需中间 buffer
 * @param src     源图像 RGB565 数据
 * @param src_w   源图像宽度
 * @param src_h   源图像高度
 * @param x, y    裁剪起始坐标（源图像中）
 * @param dst_w   裁剪宽度
 * @param dst_h   裁剪高度
 * @param lcd_x   LCD 显示起始 X
 * @param lcd_y   LCD 显示起始 Y
 */
void LCD_ShowImage_Cropped(const uint8_t *src, int src_w, int src_h,
                           int x, int y, int dst_w, int dst_h,
                           int lcd_x, int lcd_y)
{
    if (!src || x < 0 || y < 0 || dst_w <= 0 || dst_h <= 0) return;
    if (x + dst_w > src_w || y + dst_h > src_h) return;

    // 设置 LCD 窗口，内部已发送 0x2c 准备写显存
    LCD_BlockWrite(lcd_x, lcd_y, lcd_x + dst_w - 1, lcd_y + dst_h - 1);

    int src_stride = src_w * 2;      // 源图像每行字节数
    int x_offset = x * 2;            // 起始列的字节偏移
    int bytes_per_row = dst_w * 2;   // 每行要发送的字节数

    MySPI_Start();
    DC(1);  // 数据模式

    for (int row = 0; row < dst_h; row++) 
	{
        const uint8_t *row_ptr = src + (y + row) * src_stride + x_offset;
        hardware_spi_write_buffer(spi_dev, (uint8_t *)row_ptr, bytes_per_row);
    }

    MySPI_Stop();
}

/**
 * @brief 中心裁剪并直接显示到 LCD（零拷贝）
 */
void LCD_ShowImage_CroppedCenter(const uint8_t *src, int src_w, int src_h,
                                 int dst_w, int dst_h,
                                 int lcd_x, int lcd_y)
{
    if (!src || dst_w <= 0 || dst_h <= 0) return;
    if (dst_w > src_w || dst_h > src_h) return;

    int x = (src_w - dst_w) / 2;
    int y = (src_h - dst_h) / 2;

    LCD_ShowImage_Cropped(src, src_w, src_h, x, y, dst_w, dst_h, lcd_x, lcd_y);
}








