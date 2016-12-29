#include<avr/io.h>
#include<util/delay.h>

#define rs PD0
#define rw PD1
#define en PD2

void lcd_init(void);   //Function for LCD initilization
void lcd_cmd(int);     //Function for sending command
void dis_cmd(int);     //Function for sending command in 4-bit mode
void lcd_data(int);    //Function for sending data
void dis_data(int);    //Function for sending data in 4-bit mode

void lcd_string(char *str);   //Function for displaying any string
void lcd_number(int);         //Function for displaying any number

unsigned int adc_read(unsigned);   //Displaying the values of ADC on LCD


int main()
{
DDRA=0x00;  //connect DTMF to PA0-3
DDRD=0xff;  //connect LCD
DDRB=0x0f;  //motor output PB0-3
lcd_init();

dis_cmd(0x84);
lcd_string("Dual Tone");
dis_cmd(0xc0);
lcd_string("Multiple Frequen");_delay_ms(1000);


while(1)
{
int a;
a=adc_read(0);    //**it reads 4-bits PA0-3**
a=(PINA&0x0f);  //PORTB=a;
dis_cmd(0xc2);
lcd_number(a);_delay_ms(500);

if (a==2) {PORTB=0b00000101;dis_cmd(0x01); _delay_ms(50); dis_cmd(0xc5);lcd_string("FORWARD"); _delay_ms(50);}      //forward
if (a==4) {PORTB=0b00000100;dis_cmd(0x01); _delay_ms(50); dis_cmd(0xc5);lcd_string("LEFT");    _delay_ms(50);}      //left
if (a==6) {PORTB=0b00000001;dis_cmd(0x01); _delay_ms(50); dis_cmd(0xc5);lcd_string("RIGHT");   _delay_ms(50);}      //right
if (a==8) {PORTB=0b00001010;dis_cmd(0x01); _delay_ms(50); dis_cmd(0xc5);lcd_string("BACKWARD");_delay_ms(50);}      //backward
if (a==5) {PORTB=0b00000000;dis_cmd(0x01); _delay_ms(50); dis_cmd(0xc5);lcd_string("STOP");    _delay_ms(50);}      //stop
}


return 0;
}

unsigned int adc_read(unsigned chn)
{
ADMUX=chn|0x40;
ADCSRA=0xd7;
while((ADCSRA&0x10)==0);
ADCSRA|=0x10;
return ADC;
}


void lcd_string(char *str)
{

  for(int i=0;str[i]!='\0';i++)
  {
  dis_data(str[i]);
  }
}

void lcd_number(int abc)
{
 dis_cmd(0x04);
 unsigned int d=0; 
   d=abc%10;
   dis_data(d+48);
   abc=abc/10;
   d=abc%10;
   dis_data(d+48);
}


void lcd_init()
{
dis_cmd(0x02);
dis_cmd(0x28);
dis_cmd(0x0C);
dis_cmd(0x06);
}

void lcd_cmd(int x)
{
PORTD=x;
PORTD&=~(1<<rs);
PORTD&=~(1<<rw);
PORTD|=(1<<en);
_delay_ms(1);
PORTD&=~(1<<en);
}

void dis_cmd(int x)
{
int x1;
x1=x&0xf0;
lcd_cmd(x1);
x1=(x<<4)&0xf0;
lcd_cmd(x1);
}

void lcd_data(int x)
{
PORTD=x;
PORTD|=(1<<rs);
PORTD&=~(1<<rw);
PORTD|=(1<<en);
_delay_ms(1);
PORTD&=~(1<<en);
}

void dis_data(int x)
{
int x1;
x1=x&0xf0;
lcd_data(x1);
x1=(x<<4)&0xf0;
lcd_data(x1);
}
