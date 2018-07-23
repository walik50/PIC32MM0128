

#include "mcc_generated_files/mcc.h"

#include "xc.h" /* wykrywa rodzaj procka i includuje odpowiedni plik naglowkowy "pic32mm0256gpm048.h"*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> /*deklaracje uint8_t itp*/
#include "delay.h"
#include "ili9163.h"
#include "bitmap.h"
/*
                         Main application
 */
int main(void)
{
   // initialize the device
    SYSTEM_Initialize();
    RESET_OFF; /*Reset hgh*/
    lcdInitialise(LCD_ORIENTATION3);
        
    // When using interrupts, you need to set the Global Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalDisable();
    lcdClearDisplay(BLACK);
    //lcdPutS("PIC32MM", lcdTextX(1), lcdTextY(2), YELLOW, BLACK);
	//lcdFilledRectangle(20, 20, 50, 50, RED);
	//lcdLine(20,50,100,50,LIME);
	//lcdPutCh('A',5,5,LIME,BLACK);
//    lcdBitmap(bitmap1,128,30,0,0,BLACK,LIME);
//    lcdBitmap(bitmap3,128,30,0,31,YELLOW,RED);
//    lcdBitmap(bitmap2,128,30,0,62,LIGHT_GOLDENROD_YELLOW,BLUE);
//    lcdBitmap(bitmap4,128,30,0,93,RED,PINK);
//    lcdBitmap(bitmap5,128,30,0,124,WHITE,BLACK);
    
    //drawChar(5, 5, '1', ORANGE, BLACK,5);
    drawString("PIC", 0, 50, ORANGE, BLACK, 5);
    
    while (1) {

    }
}
/**
 End of File
*/