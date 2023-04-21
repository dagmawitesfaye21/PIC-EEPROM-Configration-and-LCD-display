#include <xc.h>
#include "nar.h"
   
#define _XTAL_FREQ 8000000

#define lcd PORTD
#define com PORTC

#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

float byte;
void read(const unsigned char address){
    EEADR=address;
    EECON1bits.EEPGD=1;
    EECON1bits.RD=1;
    byte=EEDATA;}
    
void writeData(unsigned char data,const unsigned char address){
    EEADR=address;
    EEDATA=data;
    
    EECON1bits.EEPGD=0;
    EECON1bits.WREN=1; 
    EECON2=0x55;
    EECON2=0xAA;
    EECON1bits.WR=1;
    
}

void lcd_lat(){
    EN=1;
    __delay_ms(10);
    EN=0;
}

void lcd_cmd(unsigned char command){
    com = command;
    RS=0;
    RW=0;
    lcd_lat();
}

void lcd_data(unsigned char data){
    lcd = data;
    RS=1;
    RW=0;
    lcd_lat();
}


void lcd_string(unsigned char *strng)
{
    while(*strng != 0)              
        lcd_data(*strng++);          //Sends data to LCD and then increment
}


void lcd_int(){
    lcd_cmd(0x38);              //8-bit mode, 2 lines, 5x7 dots matrix
    lcd_cmd(0x0E);              //Cursor Blinking
    lcd_cmd(0x01);              //Clear LCD
    lcd_cmd(0x06);              //Increment Cursor
    lcd_cmd(0x80); 
}


void main() {
    ANSELD = 0x00;
    TRISD = 0x00;
    ANSELC &= 0xF8;
    TRISC &= 0xF8;
    
    lcd_int();
    lcd_cmd(0x80);
    lcd_string("ROM value: ");
    lcd_cmd(0xC0);
  
  while(1){
    writeData(9,0xC0);
    
    read(0xC0);
    
    lcd_data(byte+(0x30));
  }
          
}
