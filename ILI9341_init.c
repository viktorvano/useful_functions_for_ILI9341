#define RESET_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)
#define RESET_LOW HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#define CS_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)
#define CS_LOW HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)
#define DC_DATA HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET)
#define DC_COMMAND HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET)

#define LCD_W 240
#define LCD_H 320

void LCD_WRITE_DATA(uint8_t data)
{
	DC_DATA;
	//CS_LOW;
	//HAL_SPI_Transmit(&hspi3, &data, 1, 1);
	//SPI_send_8bit(data);
	HAL_SPI_Transmit_IT(&hspi3, &data, 1);
	//HAL_Delay(1);
	//CS_HIGH;
}

void LCD_WRITE_DATA_16(uint8_t dataH, uint8_t dataL)
{
	DC_DATA;
	//CS_LOW;
	//HAL_SPI_Transmit(&hspi3, &dataH, 1, 1);
	//HAL_SPI_Transmit(&hspi3, &dataL, 1, 1);
	HAL_SPI_Transmit_IT(&hspi3, &dataH, 1);
	HAL_SPI_Transmit_IT(&hspi3, &dataL, 1);
	//SPI_send_8bit(dataH);
	//SPI_send_8bit(dataL);
	//CS_HIGH;
}

void LCD_WRITE_COMMAND(uint8_t data)
{
	DC_COMMAND;
	//CS_LOW;
	//HAL_SPI_Transmit(&hspi3, &data, 1, 1);
	HAL_SPI_Transmit_IT(&hspi3, &data, 1);
	//HAL_Delay(1);
	//SPI_send_8bit(data);
	//CS_HIGH;
}

void LCD_READ_COMMAND(uint8_t cmd, uint8_t* data)
{
	//CS_LOW;
	DC_COMMAND;

	HAL_SPI_Transmit(&hspi3, &cmd, 1, 1);
	DC_DATA;
	HAL_SPI_Receive(&hspi3, data, 4, 5);

	//HAL_SPI_Transmit_IT(&hspi3, &data, 1);
	//HAL_Delay(1);
	//CS_HIGH;
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)  // CONVERTED TO CCS
{
	LCD_WRITE_COMMAND(0x2a);
	LCD_WRITE_DATA(x1>>8);
	LCD_WRITE_DATA(x1);
	LCD_WRITE_DATA(x2>>8);
	LCD_WRITE_DATA(x2);

	LCD_WRITE_COMMAND(0x2b);
	LCD_WRITE_DATA(y1>>8);
	LCD_WRITE_DATA(y1);
	LCD_WRITE_DATA(y2>>8);
	LCD_WRITE_DATA(y2);

	LCD_WRITE_COMMAND(0x2C);
}

void LCD_Clear(uint16_t Color) // CONVERTED TO CCS
{
	uint8_t VH,VL;
	uint16_t i,j;
	//int x=1,y=1;
	VH=Color>>8;
	VL=Color;
	Address_set(0,0,LCD_W,LCD_H);

    for(i=0;i<LCD_H;i++)
	 {
	  for (j=0;j<LCD_W;j++) // LCD_H
	   	{
		  LCD_WRITE_DATA_16(VH, VL);
	    }
	  }
}

void LCD_ILI9341_Init(void)
{
//	uint8_t data[5];

	CS_LOW;
	//************* Reset LCD Driver ****************//
	//Hardware reset
	RESET_HIGH;
	HAL_Delay(5);
	RESET_LOW;
	HAL_Delay(15);
	RESET_HIGH;
	HAL_Delay(15);
	//Done Reset
	//Software Reset
	LCD_WRITE_COMMAND(0x01);
	HAL_Delay(15);
	//Done Reset

	//LCD_READ_COMMAND(0x04);

	LCD_WRITE_COMMAND(0xEF);
	LCD_WRITE_DATA(0x03);
	LCD_WRITE_DATA(0x80);
	LCD_WRITE_DATA(0x02);

	LCD_WRITE_COMMAND(0xCF);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0XC1);
	LCD_WRITE_DATA(0X30);

	  LCD_WRITE_COMMAND(0xED);
	  LCD_WRITE_DATA(0x64);
	  LCD_WRITE_DATA(0x03);
	  LCD_WRITE_DATA(0X12);
	  LCD_WRITE_DATA(0X81);

	  LCD_WRITE_COMMAND(0xE8);
	  LCD_WRITE_DATA(0x85);
	  LCD_WRITE_DATA(0x00);
	  LCD_WRITE_DATA(0x78);

	  LCD_WRITE_COMMAND(0xCB);
	  LCD_WRITE_DATA(0x39);
	  LCD_WRITE_DATA(0x2C);
	  LCD_WRITE_DATA(0x00);
	  LCD_WRITE_DATA(0x34);
	  LCD_WRITE_DATA(0x02);

	  LCD_WRITE_COMMAND(0xF7);
	  LCD_WRITE_DATA(0x20);

	  LCD_WRITE_COMMAND(0xEA);
	  LCD_WRITE_DATA(0x00);
	  LCD_WRITE_DATA(0x00);

	  LCD_WRITE_COMMAND(ILI9341_PWCTR1);    //Power control
	  LCD_WRITE_DATA(0x23);   //VRH[5:0]

	  LCD_WRITE_COMMAND(ILI9341_PWCTR2);    //Power control
	  LCD_WRITE_DATA(0x10);   //SAP[2:0];BT[3:0]

	  LCD_WRITE_COMMAND(ILI9341_VMCTR1);    //VCM control
	  LCD_WRITE_DATA(0x3e); //
	  LCD_WRITE_DATA(0x28);

	  LCD_WRITE_COMMAND(ILI9341_VMCTR2);    //VCM control2
	  LCD_WRITE_DATA(0x86);  //--

	  LCD_WRITE_COMMAND(ILI9341_MADCTL);    // Memory Access Control
	  LCD_WRITE_DATA(0x48);

	  LCD_WRITE_COMMAND(ILI9341_PIXFMT);
	  LCD_WRITE_DATA(0x55);

	  LCD_WRITE_COMMAND(ILI9341_FRMCTR1);
	  LCD_WRITE_DATA(0x00);
	  LCD_WRITE_DATA(0x18);

	  LCD_WRITE_COMMAND(ILI9341_DFUNCTR);    // Display Function Control
	  LCD_WRITE_DATA(0x08);
	  LCD_WRITE_DATA(0x82);
	  LCD_WRITE_DATA(0x27);

	  LCD_WRITE_COMMAND(0xF2);    // 3Gamma Function Disable
	  LCD_WRITE_DATA(0x00);

	  LCD_WRITE_COMMAND(ILI9341_GAMMASET);    //Gamma curve selected
	  LCD_WRITE_DATA(0x01);

	  LCD_WRITE_COMMAND(ILI9341_GMCTRP1);    //Set Gamma
	  LCD_WRITE_DATA(0x0F);
	  LCD_WRITE_DATA(0x31);
	  LCD_WRITE_DATA(0x2B);
	  LCD_WRITE_DATA(0x0C);
	  LCD_WRITE_DATA(0x0E);
	  LCD_WRITE_DATA(0x08);
	  LCD_WRITE_DATA(0x4E);
	  LCD_WRITE_DATA(0xF1);
	  LCD_WRITE_DATA(0x37);
	  LCD_WRITE_DATA(0x07);
	  LCD_WRITE_DATA(0x10);
	  LCD_WRITE_DATA(0x03);
	  LCD_WRITE_DATA(0x0E);
	  LCD_WRITE_DATA(0x09);
	  LCD_WRITE_DATA(0x00);

	  LCD_WRITE_COMMAND(ILI9341_GMCTRN1);    //Set Gamma
	  LCD_WRITE_DATA(0x00);
	  LCD_WRITE_DATA(0x0E);
	  LCD_WRITE_DATA(0x14);
	  LCD_WRITE_DATA(0x03);
	  LCD_WRITE_DATA(0x11);
	  LCD_WRITE_DATA(0x07);
	  LCD_WRITE_DATA(0x31);
	  LCD_WRITE_DATA(0xC1);
	  LCD_WRITE_DATA(0x48);
	  LCD_WRITE_DATA(0x08);
	  LCD_WRITE_DATA(0x0F);
	  LCD_WRITE_DATA(0x0C);
	  LCD_WRITE_DATA(0x31);
	  LCD_WRITE_DATA(0x36);
	  LCD_WRITE_DATA(0x0F);

	  LCD_WRITE_COMMAND(ILI9341_SLPOUT);    //Exit Sleep
	  HAL_Delay(120);
	  LCD_Clear(WHITE);

	  LCD_WRITE_COMMAND(ILI9341_DISPON);    //Display on
}