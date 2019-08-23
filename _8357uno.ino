
#define LCD_RD   A0
#define LCD_WR   A1     
#define LCD_RS   A2        
#define LCD_CS   A3       
#define LCD_REST A4
// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF
void Lcd_Writ_Bus(unsigned char VH)
{
 PORTD = (PORTD & B00000011) | ((VH) & B11111100); 
 PORTB = (PORTB & B11111100) | ((VH) & B00000011); 
 *(portOutputRegister(digitalPinToPort(LCD_WR))) &=  ~digitalPinToBitMask(LCD_WR);
 *(portOutputRegister(digitalPinToPort(LCD_WR)))|=  digitalPinToBitMask(LCD_WR);

}


void Lcd_Write_Com(unsigned char VH)  
{   
  *(portOutputRegister(digitalPinToPort(LCD_RS))) &=  ~digitalPinToBitMask(LCD_RS);//LCD_RS=0;
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Data(unsigned char VH)
{
  *(portOutputRegister(digitalPinToPort(LCD_RS)))|=  digitalPinToBitMask(LCD_RS);//LCD_RS=1;
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Com_Data(unsigned char com,unsigned char dat)
{
  Lcd_Write_Com(com);
Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	Lcd_Write_Com_Data(0x2a,x1>>8);
	Lcd_Write_Com_Data(0x2a,x1);
	Lcd_Write_Com_Data(0x2a,x2>>8);
	Lcd_Write_Com_Data(0x2a,x2);
	Lcd_Write_Com_Data(0x2b,y1>>8);
	Lcd_Write_Com_Data(0x2b,y1);
	Lcd_Write_Com_Data(0x2b,y2>>8);
	Lcd_Write_Com_Data(0x2b,y2);
	Lcd_Write_Com(0x2c); 							 
}

void Lcd_Init(void)
{
  digitalWrite(LCD_REST,HIGH);
  delay(5); 
  digitalWrite(LCD_REST,LOW);
  delay(15);
  digitalWrite(LCD_REST,HIGH);
  delay(15);

  digitalWrite(LCD_CS,HIGH);
  digitalWrite(LCD_WR,HIGH);
  digitalWrite(LCD_CS,LOW);  //CS

Lcd_Write_Com(0x11); //Sleep Out
delay(150);
Lcd_Write_Com(0xB4); //Set RM, DM
Lcd_Write_Data(0x00); //MPU Mode
Lcd_Write_Com(0xC0); //Set PANEL
Lcd_Write_Data(0x14);
Lcd_Write_Data(0x3B);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x02);
Lcd_Write_Data(0x11);
Lcd_Write_Com(0xC8); //Set Gamma
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x36);
Lcd_Write_Data(0x13);
Lcd_Write_Data(0x21);
Lcd_Write_Data(0x09);
Lcd_Write_Data(0x0C);
Lcd_Write_Data(0x46);
Lcd_Write_Data(0x14);
Lcd_Write_Data(0x77);
Lcd_Write_Data(0x12);
Lcd_Write_Data(0x06);
Lcd_Write_Data(0x12);
Lcd_Write_Com(0xD0); //Set Power
Lcd_Write_Data(0x45); //DDVDH
Lcd_Write_Data(0x41);
Lcd_Write_Data(0x02); //VREG1
Lcd_Write_Com(0xD1); //Set VCOM
Lcd_Write_Data(0x44); //VCOMH
Lcd_Write_Data(0x0F); //VCOML
Lcd_Write_Com(0xE9); //Set Panel
Lcd_Write_Data(0x01);
Lcd_Write_Com(0xEE); //Set EQ
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x00);
Lcd_Write_Com(0xED); //Set DIR TIM
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0xA2);
Lcd_Write_Data(0xA2);
Lcd_Write_Data(0xA2);
Lcd_Write_Data(0xA2);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0x00);
Lcd_Write_Data(0xAE);
Lcd_Write_Data(0xAE);
Lcd_Write_Data(0x01);
Lcd_Write_Data(0xA2);
Lcd_Write_Data(0x00);
Lcd_Write_Com(0x29); //Display On
delay(5);
}

void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
  digitalWrite(LCD_CS,LOW);
  l=l+x;
  Address_set(x,y,l,y);
  j=l*2;
  for(i=1;i<=j;i++)
  {
   Lcd_Write_Data(c);
  }
  digitalWrite(LCD_CS,HIGH);   
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
{	
  unsigned int i,j;
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
  digitalWrite(LCD_CS,LOW);
  l=l+y;
  Address_set(x,y,x,l);
  j=l*2;
  for(i=1;i<=j;i++)
  { 
    Lcd_Write_Data(c);
  }
  digitalWrite(LCD_CS,HIGH);   
}

void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  unsigned int i;
  for(i=0;i<h;i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}
int RGB(int r,int g,int b)
{return r << 16 | g << 8 | b;
}
void LCD_Clear(unsigned int j)                   
{	
  unsigned int i,m;
 Address_set(0,0,480,320);
  Lcd_Write_Com(0x02c); //write_memory_start
  digitalWrite(LCD_RS,HIGH);
  digitalWrite(LCD_CS,LOW);


  for(i=0;i<480;i++)
    for(m=0;m<320;m++)
    {
    //  Lcd_Write_Data(j>>8);
      Lcd_Write_Data(j);

    }
  digitalWrite(LCD_CS,HIGH);   
}

void setup()
{
  for(int p=2;p<10;p++)
  {
    pinMode(p,OUTPUT);
  }
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, HIGH);
  Lcd_Init();
  LCD_Clear(RED);
    LCD_Clear(GREEN);
      LCD_Clear(BLUE);
  
}

void loop()
{  

  for(int i=0;i<1000;i++)
  {
    Rect(random(300),random(300),random(300),random(300),random(65535)); // rectangle at x, y, with, hight, color
  }
  
//  LCD_Clear(0xf800);
}