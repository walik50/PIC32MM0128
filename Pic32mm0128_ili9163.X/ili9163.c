/************************************************************************
	ili9163.c
    ILI9163 128x160 LCD library
    3.3 V
    for PIC32MM
***********************************************************************/

#include "mcc_generated_files/mcc.h"
#include "delay.h"
#include <string.h>
#include "ili9163.h"
#include "font5x8.h"
#include "font8x8.h"
#include "font8x12.h"



/*symulacja SPI*/
void writeSD(uint8_t byteOut)
{ 
  uint8_t bitcnt ;
  SCE_ON;
  DisplayCLK = 0 ;           
  delayUs(1);
  for (bitcnt = 8 ; bitcnt > 0 ; bitcnt--)
  {
    DisplaySDI = (byteOut&0x80) ? 1 : 0 ;  // transfer current bit 7 of byteOut to DisplaySDI pin
    delayUs(1);
    DisplayCLK = 1 ; 
    delayUs(1);
    DisplayCLK = 0 ;
    delayUs(1);
    byteOut = byteOut << 1 ;   // logical shift left: get next bit to b7
  }
  DisplayCLK = 0 ;
  delayUs(1);
  SCE_OFF;
  
}

/********************************************
 * 
 * Low-level LCD driving functions
 * 
 **********************************************/

void lcdWriteCommand_bis(uint8_t cmdOut)
{ // Write a command to Display
   DC_ON; 		/*DC on = command set A0 low, select Command mode DisplayD_C = 0 for this byte*/
   writeSD(cmdOut) ;  // Clock out command bits
   DC_OFF;
   }

void lcdWriteParameter_bis(uint8_t data)
{
	writeSD(data) ;  
}

void lcdWriteData_bis(uint8_t dataByte1, uint8_t dataByte2)
{
	writeSD(dataByte1);
	writeSD(dataByte2);
}

// Reset the LCD hardware
void lcdReset(void)
{
	//Chipselect ON
	SCE_ON;	
	// Reset pin is active low (0 = reset, 1 = ready)
	RESET_ON;
	delayMs(50);
	RESET_OFF;
	delayMs(150);
	SCE_OFF;
}

// Initialise the display with the require screen orientation
void lcdInitialise(uint8_t orientation)
{   
	// Hardware reset the LCD
	lcdReset();
	
    lcdWriteCommand_bis(EXIT_SLEEP_MODE);
    delayMs(50); // Wait for the screen to wake up
    
    lcdWriteCommand_bis(SET_PIXEL_FORMAT);
    lcdWriteParameter_bis(0x05); // 16 bits per pixel
   
    lcdWriteCommand_bis(SET_GAMMA_CURVE);
    lcdWriteParameter_bis(0x04); // Select gamma curve 3
	
    lcdWriteCommand_bis(GAM_R_SEL);
    lcdWriteParameter_bis(0x01); // Gamma adjustment enabled
    
    lcdWriteCommand_bis(POSITIVE_GAMMA_CORRECT);
    lcdWriteParameter_bis(0x3f); // 1st Parameter
    lcdWriteParameter_bis(0x25); // 2nd Parameter
    lcdWriteParameter_bis(0x1c); // 3rd Parameter
    lcdWriteParameter_bis(0x1e); // 4th Parameter
    lcdWriteParameter_bis(0x20); // 5th Parameter
    lcdWriteParameter_bis(0x12); // 6th Parameter
    lcdWriteParameter_bis(0x2a); // 7th Parameter
    lcdWriteParameter_bis(0x90); // 8th Parameter
    lcdWriteParameter_bis(0x24); // 9th Parameter
    lcdWriteParameter_bis(0x11); // 10th Parameter
    lcdWriteParameter_bis(0x00); // 11th Parameter
    lcdWriteParameter_bis(0x00); // 12th Parameter
    lcdWriteParameter_bis(0x00); // 13th Parameter
    lcdWriteParameter_bis(0x00); // 14th Parameter
    lcdWriteParameter_bis(0x00); // 15th Parameter
     
    lcdWriteCommand_bis(NEGATIVE_GAMMA_CORRECT);
    lcdWriteParameter_bis(0x20); // 1st Parameter
    lcdWriteParameter_bis(0x20); // 2nd Parameter
    lcdWriteParameter_bis(0x20); // 3rd Parameter
    lcdWriteParameter_bis(0x20); // 4th Parameter
    lcdWriteParameter_bis(0x05); // 5th Parameter
    lcdWriteParameter_bis(0x00); // 6th Parameter
    lcdWriteParameter_bis(0x15); // 7th Parameter
    lcdWriteParameter_bis(0xa7); // 8th Parameter
    lcdWriteParameter_bis(0x3d); // 9th Parameter
    lcdWriteParameter_bis(0x18); // 10th Parameter
    lcdWriteParameter_bis(0x25); // 11th Parameter
    lcdWriteParameter_bis(0x2a); // 12th Parameter
    lcdWriteParameter_bis(0x2b); // 13th Parameter
    lcdWriteParameter_bis(0x2b); // 14th Parameter
    lcdWriteParameter_bis(0x3a); // 15th Parameter
    
    lcdWriteCommand_bis(FRAME_RATE_CONTROL1);
    lcdWriteParameter_bis(0x0E); // DIVA = 14
    lcdWriteParameter_bis(0x14); // VPA = 20
    
    lcdWriteCommand_bis(DISPLAY_INVERSION);
    lcdWriteParameter_bis(0x07); // NLA = 1, NLB = 1, NLC = 1 (all on Frame Inversion)
   
    lcdWriteCommand_bis(POWER_CONTROL1);
    lcdWriteParameter_bis(0x1F); // VRH = 31:  GVDD = 3,00V
          
    lcdWriteCommand_bis(POWER_CONTROL2);
    lcdWriteParameter_bis(0x00); // BT = 0: AVDD = 2xVCI1, VCL = -1xVCI1, VGH = 4xVCI1, VGL = -3xVCI1

    lcdWriteCommand_bis(VCOM_CONTROL1);
    lcdWriteParameter_bis(0x24); // VMH = 36: VCOMH voltage = 3.4
    lcdWriteParameter_bis(0x64); // VML = 100: VCOML voltage = 0
	
    lcdWriteCommand_bis(VCOM_OFFSET_CONTROL);
    lcdWriteParameter_bis(0x40); // nVM = 0, VMF = 64: VCOMH output = VMH, VCOML output = VML	
    
    lcdWriteCommand_bis(SET_COLUMN_ADDRESS);
    lcdWriteParameter_bis(0x00); // XSH
    lcdWriteParameter_bis(0x00); // XSL
    lcdWriteParameter_bis(0x00); // XEH
    lcdWriteParameter_bis(0x7f); // XEL (128 pixels x)
   
    lcdWriteCommand_bis(SET_PAGE_ADDRESS);
    lcdWriteParameter_bis(0x00);
    lcdWriteParameter_bis(0x00);
    lcdWriteParameter_bis(0x00);
    lcdWriteParameter_bis(0x9f); // 160 pixels y
	
	// Select display orientation
    lcdWriteCommand_bis(SET_ADDRESS_MODE);
	lcdWriteParameter_bis(orientation);

	// Set the display to on
    lcdWriteCommand_bis(SET_DISPLAY_ON);
    //lcdWriteCommand(WRITE_MEMORY_START);
   
}

/*LCD graphics functions*/

void lcdClearDisplay(uint16_t colour)
{
	uint16_t pixel;
  
	/*set the column address to 0-127*/
	lcdWriteCommand_bis(SET_COLUMN_ADDRESS);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x7f); //7f = 128px

	/*Set the page address to 0-159*/
	lcdWriteCommand_bis(SET_PAGE_ADDRESS);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x9f); //9f = 160px
  
	/*Plot the pixels*/
	lcdWriteCommand_bis(WRITE_MEMORY_START);
	for(pixel = 0; pixel < 20481; pixel++) lcdWriteData_bis(colour >> 8, colour);
}

void lcdPlot(uint8_t x, uint8_t y, uint16_t colour)
{
	/*Horizontal Address Start Position*/
	lcdWriteCommand_bis(SET_COLUMN_ADDRESS);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(x);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x7f); //7f = 128px
  
	/*Vertical Address end Position*/
	lcdWriteCommand_bis(SET_PAGE_ADDRESS);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(y);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x9f); //9f = 160px

	/*Plot the point*/
	lcdWriteCommand_bis(WRITE_MEMORY_START);
	lcdWriteData_bis(colour >> 8, colour);
}

// Draw a line from x0, y0 to x1, y1
// Note:	This is a version of Bresenham's line drawing algorithm
//			It only draws lines from left to right!
/* |-----X------>
 * |
 * Y
 * |
 * V
 * 
 **/
void lcdLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	int16_t dy = y1 - y0;
	int16_t dx = x1 - x0;
	int16_t stepx, stepy;

	if (dy < 0)
	{
		dy = -dy; stepy = -1; 
	}
	else stepy = 1; 

 	if (dx < 0)
	{
		dx = -dx; stepx = -1; 
	}
	else stepx = 1; 

	dy <<= 1; 							// dy is now 2*dy
	dx <<= 1; 							// dx is now 2*dx
 
	lcdPlot(x0, y0, colour);

	if (dx > dy) {
		int fraction = dy - (dx >> 1);	// same as 2*dy - dx
		while (x0 != x1)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx; 		// same as fraction -= 2*dx
			}

   			x0 += stepx;
   			fraction += dy; 				// same as fraction -= 2*dy
   			lcdPlot(x0, y0, colour);
		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}

			y0 += stepy;
			fraction += dx;
			lcdPlot(x0, y0, colour);
		}
	}
}

/*Draw a rectangle between x0, y0 and x1, y1*/
void lcdRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	lcdLine(x0, y0, x0, y1, colour);
	lcdLine(x0, y1, x1, y1, colour);
	lcdLine(x1, y0, x1, y1, colour);
	lcdLine(x0, y0, x1, y0, colour);
}

/* Draw a filled rectangle
   Note:	y1 must be greater than y0  and x1 must be greater than x0
 			for this to work*/
void lcdFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
	uint16_t pixels;
			
	/* To speed up plotting we define a x window with the width of the 
	   rectangle and then just output the required number of bytes to
	   fill down to the end point*/
	
	lcdWriteCommand_bis(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(x0);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(x1); 
  
	lcdWriteCommand_bis(SET_PAGE_ADDRESS); // Vertical Address end Position
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(y0);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x9f); //9f = 160px
		
	lcdWriteCommand_bis(WRITE_MEMORY_START);
	
	for (pixels = 0; pixels < ((x1 - x0) * (y1 - y0)); pixels++)
		lcdWriteData_bis(colour >> 8, colour);;
}

/* Draw a circle
 Note:	This is another version of Bresenham's line drawing algorithm.
			There's plenty of documentation on the web if you are curious
			how this works.*/
void lcdCircle(int16_t xCentre, int16_t yCentre, int16_t radius, uint16_t colour)
{
	int16_t x = 0, y = radius;
	int16_t d = 3 - (2 * radius);
 
    while(x <= y)
	{
		lcdPlot(xCentre + x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre + x, colour);
		lcdPlot(xCentre - x, yCentre + y, colour);
		lcdPlot(xCentre + y, yCentre - x, colour);
		lcdPlot(xCentre - x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre - x, colour);
		lcdPlot(xCentre + x, yCentre - y, colour);
		lcdPlot(xCentre - y, yCentre + x, colour);

		if (d < 0) d += (4 * x) + 6;
		else
		{
			d += (4 * (x - y)) + 10;
			y -= 1;
		}

		x++;
	}
}


// Plot a Bitmap at the specified x, y co-ordinates (top left hand corner of Bitmap)
/*
 * 31.8.2017 grn
 * 
 * Bitmap is drawn from left to right and from top to bottom 
 *
 * Convert bitmap with: http://www.riuson.com/lcd-image-converter
 *  
 */
void lcdBitmap(const unsigned char *data, uint8_t width, uint8_t height, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
	uint8_t row, column, line;
	uint16_t pic;
	
	/* To speed up plotting we define a x window of 6 pixels and then
	 write out one row at a time.  This means the LCD will correctly
	 update the memory pointer saving us a good few bytes*/
	
	lcdWriteCommand_bis(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(x);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(x+(width-1));
  
	
	/*Plot the bitmap data*/
	for(line = 0; line < height/8; line++) //Write horizontal line
	{
		lcdWriteCommand_bis(SET_PAGE_ADDRESS); // Vertical Address end Position
		lcdWriteParameter_bis(0x00);
		lcdWriteParameter_bis(y);				
		lcdWriteParameter_bis(0x00);
		lcdWriteParameter_bis(0x9f); //9f = 160px
		
		lcdWriteCommand_bis(WRITE_MEMORY_START);
				
		for (row = 0; row < 8; row++)		//Write bit by bit
		{
			pic=0+(line*width);
			for (column = 0; column < width; column++)	
			{
			
				if (data[pic] & (1 << row))
					lcdWriteData_bis(fgColour >> 8, fgColour);
				else lcdWriteData_bis(bgColour >> 8, bgColour);
				pic++;
			}
		}
	y += 8;	//increase vertical Address and Position	
	
	}//end of for line
		
}//end of lcdBitmap

/*LCD text manipulation functions*/

/* Plot a character at the specified x, y co-ordinates (top left hand corner of character)*/
void lcdPutCh(unsigned char character, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
	uint8_t row, column;
	
	// To speed up plotting we define a x window of 6 pixels and then
	// write out one row at a time.  This means the LCD will correctly
	// update the memory pointer saving us a good few bytes
	
	lcdWriteCommand_bis(SET_COLUMN_ADDRESS); // Horizontal Address Start Position
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(x);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(x+5);
  
	lcdWriteCommand_bis(SET_PAGE_ADDRESS); // Vertical Address end Position
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(y);
	lcdWriteParameter_bis(0x00);
	lcdWriteParameter_bis(0x9f); //9f = 160px
		
	lcdWriteCommand_bis(WRITE_MEMORY_START);
	
	/*Plot the font data*/
	for (row = 0; row < 8; row++)
	{
		for (column = 0; column < 6; column++)
		{
			if ((font6x8[character][column]) & (1 << row))
            
				 lcdWriteData_bis(fgColour >> 8, fgColour);
            else lcdWriteData_bis(bgColour >> 8, bgColour);
        }  
                 
		
	}
}

/*Plot a string of characters to the LCD*/
void lcdPutS(const char *string, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
	uint8_t origin = x;
    uint8_t characterNumber;

	for (characterNumber = 0; characterNumber < strlen(string); characterNumber++)
	{
		// Check if we are out of bounds and move to 
		// the next line if we are
		if (x > 121)
		{
			x = origin;
			y += 8;
		}

		// If we move past the bottom of the screen just exit
		if (y > 152) break; //120 oryginal for 128x128

		// Plot the current character
//		lcdPutCh(string[characterNumber], x, y, fgColour, bgColour);
		x += 6;
	}
}

/**************************************************
 * 
 * LCD special functions / translation from ST7735
 * 
 **************************************************/
uint8_t width = 128;
uint8_t height = 160;
int8_t colstart = 0, rowstart = 0;
int16_t wrap;

void pushColour(uint16_t colour) 
{
  DC_OFF;//SET_HIGH(DXL);
  lcdWriteParameter_bis(colour >> 8); //spiWrite(colour >> 8);
  lcdWriteParameter_bis(colour); //spiWrite(colour);
}

void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) 
{
  lcdWriteCommand_bis(SET_COLUMN_ADDRESS); //writeCommand(ST7735_CASET); 
  lcdWriteParameter_bis(0x00); //writeData(0x00);
  lcdWriteParameter_bis(x0+colstart); //writeData(x0+colstart);   
  lcdWriteParameter_bis(0x00); //writeData(0x00);
  lcdWriteParameter_bis(x1+colstart); //writeData(x1+colstart);  

  lcdWriteCommand_bis(SET_PAGE_ADDRESS); //writeCommand(ST7735_RASET); 
  lcdWriteParameter_bis(0x00); //writeData(0x00);
  lcdWriteParameter_bis(y0+rowstart); //writeData(y0+rowstart);  
  lcdWriteParameter_bis(0x00); //writeData(0x00);
  lcdWriteParameter_bis(y1+rowstart); //writeData(y1+rowstart); 

  lcdWriteCommand_bis(WRITE_MEMORY_START); //writeCommand(ST7735_RAMWR);
}

void drawPixel(int16_t x, int16_t y, uint16_t colour) 
{
  if((x < 0) ||(x >= width) || (y < 0) || (y >= height)) return;

  setAddrWindow(x,y,x+1,y+1);
  pushColour(colour);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour) 
{
  if((x >= width) || (y >= height)) return;
  if((x + w - 1) >= width)  {
    w = width  - x;
  }
  if((y + h - 1) >= height) {
    h = height - y;
  }
  uint8_t hi = colour >> 8, lo = colour;
  
  setAddrWindow(x, y, x+w-1, y+h-1);
  DC_OFF; //SET_HIGH(DXL);

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      lcdWriteParameter_bis(hi); //spiWrite(hi);
      lcdWriteParameter_bis(lo); //spiWrite(lo);
    }
  } 
}

void drawChar(int16_t x, int16_t y, unsigned char character, uint16_t colour, uint16_t bg, uint8_t size) {

  if((x >= width)              || 
     (y >= height)             || 
     ((x + 6 * size - 1) < 0)  || 
     ((y + 8 * size - 1) < 0)) 
    return;
  
  int8_t i;
  for (i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 6)  //oryg 5
      line = 0x0;
    else 
      line = font6x8[character][i];//*(font1+(character*5)+i);
    
    int8_t j;
    for (j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, colour);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, colour);
        } 
      } else if (bg != colour) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}

void drawString(char *c, int16_t x, int16_t y, uint16_t colour, uint16_t bg, uint8_t size) {
  
  while(*c) {
    if (*c == '\n') {
      y += size*8;
      x  = 0;
    } else if (*c == '\r') {
      // Skip
    } else {
      drawChar(x, y, *c, colour, bg, size);
      x += size*6;
      if (wrap && (x > (width - size*6))) {
        y += size*8;
        x = 0;
      }
    }
    c++;
  }
}