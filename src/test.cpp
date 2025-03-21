
// define USESPI in LCD driver header for HW SPI version
/* #include "N5110_SPI.h"
#if USESPI==1
#include <SPI.h>
#endif
N5110_SPI lcd(N5110_RST, N5110_CS, N5110_DC); // RST,CS,DC */


//Defining the pins for the Nokia 5110 LCD
//Reset and DC
#define N5110_RST       PA1
#define N5110_DC        PA4
//SPI
#define N5110_CS        PA0
#define N5110_DIN        PA7
#define N5110_CLK        PA5


//rotary encoder pin    
#define encoderPinA     PB9
#define encoderPinB     PB8
#define encoderButton   PB7

//Nokia 5110-related libraries
#include "math.h"
#include "N5110_SPI.h"
#if USESPI==1
#include <SPI.h>
#endif
N5110_SPI lcd(N5110_RST, N5110_CS, N5110_DC, N5110_DIN, N5110_CLK); // RST,CS,DC,DIN,CLK

// -------------------------

#include "c64enh_font.h"

//extern const uint8_t c64enh[];
//#include "times_dig_16x24_font.h"
//#include "term9x14_font.h"
#include "tiny3x7_font.h"
#include "small4x7_font.h"
#include "small5x7_font.h"
#include "small5x7bold_font.h"
#include "small5x6_font.h"


volatile int encoderPos = 0;
volatile float numberOfTurns = 130;

char str_temp[7]; //for printing floats

void buttonInt() {}

void readEncoderInt()
{
  (digitalRead(encoderPinA) == digitalRead(encoderPinB)) ? encoderPos++ : encoderPos--;
}

int readButton()
{
  static int lastState = HIGH;
  int val = 0, state = digitalRead(encoderButton);
  if (state == LOW && lastState == HIGH) val = 1;
  lastState = state;
  return val;
}

void initEncoder()
{
  encoderPos = 0;
  pinMode(encoderPinA,   INPUT_PULLUP);
  pinMode(encoderPinB,   INPUT_PULLUP);
  pinMode(encoderButton, INPUT_PULLUP);
  attachInterrupt(encoderPinA, readEncoderInt, CHANGE);
  attachInterrupt(encoderButton, buttonInt, CHANGE);
}

// -------------------------
// simple pixel based drawing functions
#define PIXEL_OFF 0
#define PIXEL_ON  1
#define PIXEL_XOR 2

byte scr[84 * 4]; // frame buffer
byte scrWd = 84;
byte scrHt = 4;

void clrBuf()
{
  for (int i = 0; i < scrWd * scrHt; i++) scr[i] = 0;
}

void drawPixel(int16_t x, int16_t y, uint16_t c)
{
  if ((x < 0) || (x >= scrWd) || (y < 0) || (y >= scrHt * 8)) return;
  switch (c) {
    case PIXEL_OFF: scr[x + (y / 8)*scrWd] &= ~(1 << (y & 7)); break;
    case PIXEL_ON:  scr[x + (y / 8)*scrWd] |=  (1 << (y & 7)); break;
    case PIXEL_XOR: scr[x + (y / 8)*scrWd] ^=  (1 << (y & 7)); break;
  }
}

void drawLineV(int16_t x, int16_t y0, int16_t y1, uint16_t c)
{
  if (y1 > y0) for (int y = y0; y <= y1; y++) drawPixel(x, y, c);
  else         for (int y = y1; y <= y0; y++) drawPixel(x, y, c);
}
// muiinputversatilerotaryencoder
// --------------------------------------------------------------------------

char buf[25], buf2[25];
int numT = 0;
int curT = 0;
float bufT[300];
int graphStart = 0;

//const char *menuTxt[] = {"LN", "Help"," sin"};
//const char *menuTxt[] = {"MCU Temp", "Acceleration", "Clock", "Backlight", "Contrast", "EEPROM dump", "Graph", "Help", "Reboot"};
const char *menuTxt[] = {"Number of Turn", "Acceleration", "wireDiameter" "coilPitch", "bobbinWidth", "bobbinDiameter", "mainShaftStepper", "feederShaftStepper", "winding", "autoWinding"};
const char *menuTxt2[] = {"Turn","wireDiameter","coilPitch", "bobbinWidth", "bobbinDiameter", "autoWinding"};
int numMenus = 0;
int menuLine;
int menuStart;
int numScrLines = 6;// 6 lines of text on screen
int menuMode = -1; // -1 -> menu of options, 0..n -> option
int oldPos = 0;

//add submenus
int subnumMenus = 0;
int submenuLine;
int submenuStart;
int subnumScrLines = 6;// 6 lines of text on screen
int submenuMode = -1; // -1 -> menu of options, 0..n -> option
int suboldPos = 0;

int blinkTimer = 100; //timer for blinking the LED on the microcontroller
bool blinkStatus = true;


void setup()
{
  //Serial.begin(115200);

//Starting up the Nokia 5110 LCD
lcd.init(); //initialize LCD  
delay(300);   
lcd.clrScr(); //clear the whole display
delay(300);   
lcd.setContrast(57); 
lcd.setBias(4); 
lcd.displayMode(PCD8544_DISPLAYNORMAL);
delay(300);   

  initEncoder();
  numMenus = sizeof(menuTxt) / sizeof(char*);
  /* pinMode(N5110_BACKLIGHT, OUTPUT);
  analogWrite(N5110_BACKLIGHT, 0); // 0=max */
}


void drawBatt(int x, int y, int wd, int perc)
{
  int w = wd * perc / 100;
  lcd.fillWin(x, y, 1 + w, 1, B01111111);
  x += w + 1;
  w = wd - w;
  if (w > 0) {
    lcd.fillWin(x, y, w, 1, B01000001);
    x += w;
  }
  lcd.fillWin(x++, y, 1, 1, B01111111);
  lcd.fillWin(x++, y, 1, 1, B00011100);
  lcd.fillWin(x++, y, 1, 1, B00011100);
}

void drawBattBig(int x, int y, int wd, int perc)
{
  int w = wd * perc / 100;
  lcd.fillWin(x, y + 0, 1, 1, B11111110);
  lcd.fillWin(x, y + 1, 1, 1, B01111111); x++;
  lcd.fillWin(x, y + 0, 1, 1, B00000010);
  lcd.fillWin(x, y + 1, 1, 1, B01000000); x++;
  lcd.fillWin(x, y + 0, w, 1, B11111010);
  lcd.fillWin(x, y + 1, w, 1, B01011111); x += w;
  w = wd - w;
  if (w > 0) {
    lcd.fillWin(x, y + 0, w, 1, B00000010);
    lcd.fillWin(x, y + 1, w, 1, B01000000); x += w;
  }
  lcd.fillWin(x, y + 0, 1, 1, B00000010);
  lcd.fillWin(x, y + 1, 1, 1, B01000000); x++;
  lcd.fillWin(x, y + 0, 1, 1, B11111110);
  lcd.fillWin(x, y + 1, 1, 1, B01111111); x++;
  lcd.fillWin(x, y + 0, 1, 1, B11100000);
  lcd.fillWin(x, y + 1, 1, 1, B00000111); x++;
  lcd.fillWin(x, y + 0, 1, 1, B11100000);
  lcd.fillWin(x, y + 1, 1, 1, B00000111);
}

// ---------------------------------------
int t = 0;
unsigned long tm;

void drawSin()
{
  if (encoderPos < 0) encoderPos = 0;
  if (encoderPos > 30) encoderPos = 30;
  float mult = encoderPos - 15;
  int x, y, yold;
  scrWd = 84;
  scrHt = 4;
  tm = millis();
  clrBuf();
  for (x = 0; x < 84; x++) {
    y = 16 + mult * (sin((x + t / 4.0) / 5.0) * cos(x / 22.0));
    if (x == 0 || y == yold)
      drawPixel(x, y, 1);
    else
      drawLineV(x, y, yold, 1);
    yold = y;
  }
  lcd.drawBuf(scr, 0, 1, scrWd, 4);
  lcd.setFont(Small5x7PLBold);
  lcd.setDigitMinWd(4);
  snprintf(buf, 99, "  Mult: %2d  ", (int)mult);
  lcd.printStr(ALIGN_CENTER, 0, buf);
  t += 4;
  while (millis() - tm < 80);
}

// ---------------------------------------
int x;
long vcc_mV = 3892;
float temp = 21.3, mint = 99, maxt = -99;
int temp1 = 21;
int temp10 = 3;

void showHelp()
{
  //lcd.setFont(c64enh);
  lcd.setFont(Small5x7PL);//Small5x7PL);
  lcd.printStr(ALIGN_CENTER, 0, "Help");
  lcd.setFont(Small5x7PL);
  lcd.setCR(1);
  lcd.printStr(0, 1, "Use encoder to select menu item. Press button to exit.");
  lcd.setCR(0);
}
void showHelp2()
{
  //lcd.setFont(c64enh);
  lcd.setFont(Small5x7PL);//Small5x7PL);
  lcd.printStr(ALIGN_CENTER, 0, "Help2");
  lcd.setFont(Small5x7PL);
  lcd.setCR(1);
  lcd.printStr(0, 1, "Test Help 2");
  lcd.setCR(0);
}



void setContrast()
{
  if (encoderPos > 63) encoderPos = 63;
  snprintf(buf, 6, "%02X", encoderPos * 2);
  lcd.setFont(Small5x7PL);
  lcd.setCharMinWd(5);
  lcd.setDigitMinWd(5);
  lcd.printStr(28, 1, buf);
  lcd.printStr(ALIGN_LEFT, 1, "00");
  lcd.printStr(58, 1, "7F");
  lcd.fillWin(0, 2, encoderPos, 1, 0xfe);
  if (encoderPos < 84) lcd.fillWin(encoderPos, 2, 84 - encoderPos, 1, 0);
  lcd.setContrast(encoderPos * 2);
}


void setNumberOfTurn()
{
  /* if (encoderPos > 84) encoderPos = 84;
  snprintf(buf, 6, " %d ", encoderPos * 255 / 84);
  lcd.setFont(Small5x7PL);
  lcd.setCharMinWd(5);
  lcd.setDigitMinWd(5);
  lcd.printStr(ALIGN_CENTER, 1, buf);
  lcd.printStr(ALIGN_LEFT, 1, "000");
  lcd.printStr(ALIGN_RIGHT, 1, "255");
  lcd.fillWin(0, 2, encoderPos, 1, 0xfc);
  if (encoderPos < 84) lcd.fillWin(encoderPos, 2, 84 - encoderPos, 1, 0);
  analogWrite(N5110_BACKLIGHT, 255 - encoderPos * 3); */


      lcd.setFont(c64enh);
      //lcd.printStr(ALIGN_CENTER, 0, emptyStr);
      lcd.printStr(ALIGN_CENTER, 0, "Turns");
      //lcd.setFont(c64enh);
      lcd.setFont(Small5x7PL);
      //dtostrf(numberOfTurns, 4, 0, buf); //dtostrf is used/necessary for floats
      //snprintf(buf, 8, "%s", encoderPos);  
      snprintf(buf, sizeof(buf), "%d", encoderPos);
      lcd.printStr(ALIGN_CENTER, 2, buf);    
      

      lcd.setFont(c64enh);
      
      lcd.printStr(ALIGN_CENTER, 4, buf);
      lcd.printStr(ALIGN_CENTER, 5, buf);
}
void (*avr_reset)(void) = 0;


void setMenu(int m)
{
  menuMode = m;
  lcd.clrScr();
  oldPos = encoderPos;
  encoderPos = 0;
}

void endMenu()
{
  if (readButton() > 0) {
    menuMode = -1;
    lcd.clrScr();
    encoderPos = oldPos;
  }
}

void formatMenu(const char *in, char *out, int num)
{
  int j = strlen(in);
  out[0] = ' ';
  strncpy(out + 1, in, j++);
  for (; j < num; j++) out[j] = ' ';
  out[j] = 0;
}

void drawMenuSlider()
{
  int n = (8 * numScrLines - 2 - 5 - 2) * menuLine / (numMenus - 1);
  scrWd = 3;
  scrHt = numScrLines;
  clrBuf();
  drawLineV(1, 0, numScrLines*8-1, 1);
  drawLineV(0, n+2, n+2+4, 1);
  drawLineV(2, n+2, n+2+4, 1);
  lcd.drawBuf(scr, 81, 0, scrWd, scrHt);
}

//new function for submenu slider

void setSubMenu(int m)
{
  submenuMode = m;
  lcd.clrScr();
  suboldPos = encoderPos;
  encoderPos = 0;
}

void endsubMenu()
{
  if (readButton() > 0) {
    submenuMode = -1;
    lcd.clrScr();
    encoderPos = suboldPos;
  }
}

void drawSubMenuSlider()
{
  int n = (8 * submenuLine - 2 - 5 - 2) * submenuLine / (subnumMenus - 1);
  scrWd = 3;
  scrHt = subnumScrLines;
  clrBuf();
  drawLineV(1, 0, subnumScrLines*8-1, 1);
  drawLineV(0, n+2, n+2+4, 1);
  drawLineV(2, n+2, n+2+4, 1);
  lcd.drawBuf(scr, 81, 0, scrWd, scrHt);
}
///******* */

void test1()
{
  
  //lcd.setFont(Small5x7PL);
  lcd.setFont(Small5x7PL);
  lcd.setCharMinWd(5);
  lcd.setDigitMinWd(5);
  if (encoderPos < 0) encoderPos = 0;
  if (submenuMode == -1) {

    //blink the onboard LED on the MCU every 1 s -> it indicates that the loop() works - you can remove it
    /* if(millis() - blinkTimer > 1000)
    {
        digitalWrite(PC13, blinkStatus);
        blinkStatus = !blinkStatus;
        blinkTimer = millis(); 
    }*/
    submenuLine = encoderPos / 2;
    if (submenuLine >= subnumMenus) {
      submenuLine = subnumMenus - 1;
      encoderPos = submenuLine * 2;
    }
    if (submenuLine >= submenuStart + subnumScrLines) submenuStart = submenuLine - subnumScrLines + 1;
    if (submenuLine < submenuStart) submenuStart = submenuLine;
    for (int i = 0; i < subnumScrLines; i++) {
      if (i + submenuStart < subnumMenus) {
        lcd.setInvert(i + submenuStart == submenuLine ? 1 : 0);
        formatMenu(menuTxt2[i + submenuStart], buf2, 13);
        lcd.printStr(ALIGN_LEFT, i, buf2);
      }
    }
    drawSubMenuSlider();
    if (readButton()) {
      setSubMenu(submenuLine);
      if (submenuLine == 3) encoderPos = 84; // setBacklight
      if (submenuLine == 4) encoderPos = 0x30 / 2; // setContrast
    }
  } else 
  if (submenuMode == 0) { showHelp2(); endsubMenu(); } else
  if (submenuMode == 1) { drawSin(); endsubMenu(); } else 
  if (submenuMode == 2) { setContrast(); endsubMenu(); } else
  if (submenuMode == 3) { showHelp2(); endsubMenu(); } else
  if (submenuMode == 4) { showHelp2(); endsubMenu(); } else
  if (submenuMode == 5) { showHelp2(); endsubMenu(); } else  
  {  submenuMode = -1; lcd.clrScr(); }
}

void handleMenu()
{
  //lcd.setFont(Small5x7PL); // set font
  lcd.setFont(Small5x7PL);
  lcd.setCharMinWd(5);
  lcd.setDigitMinWd(5);
  if (encoderPos < 0) encoderPos = 0;
  if (menuMode == -1) {
    menuLine = encoderPos / 2;
    if (menuLine >= numMenus) {
      menuLine = numMenus - 1;
      encoderPos = menuLine * 2;
    }
    if (menuLine >= menuStart + numScrLines) menuStart = menuLine - numScrLines + 1;
    if (menuLine < menuStart) menuStart = menuLine;
    for (int i = 0; i < numScrLines; i++) {
      if (i + menuStart < numMenus) {
        lcd.setInvert(i + menuStart == menuLine ? 1 : 0);
        formatMenu(menuTxt[i + menuStart], buf, 13);
        lcd.printStr(ALIGN_LEFT, i, buf);
      }
    }
    drawMenuSlider();
    if (readButton()) {
      setMenu(menuLine);
      if (menuLine == 3) encoderPos = 84; // setBacklight
      if (menuLine == 4) encoderPos = 0x30 / 2; // setContrast
    }
  } else 
  /* if (menuMode == 0) { showHelp(); endMenu(); } else
  if (menuMode == 1) { showHelp(); endMenu(); } else
  if (menuMode == 2) { setContrast(); endMenu(); } else
  if (menuMode == 3) { drawSin(); endMenu(); } else
  if (menuMode == 4) { showHelp(); endMenu(); } else */


  if (menuMode == 0) { test1(); endMenu(); } else
  if (menuMode == 1) { setNumberOfTurn(); endMenu(); } else 
  if (menuMode == 2) { drawSin(); endMenu(); } else
  if (menuMode == 3) { drawSin(); endMenu(); } else 
  if (menuMode == 4) { setContrast(); endMenu(); } else
  if (menuMode == 5) { drawSin(); endMenu(); } else
  if (menuMode == 6) { drawSin(); endMenu(); } else
  if (menuMode == 7) { showHelp(); endMenu(); } else
  if (menuMode == 8) { showHelp(); endMenu(); } else 
  {  menuMode = -1; lcd.clrScr(); }
}

void loop()
{
  handleMenu();
}