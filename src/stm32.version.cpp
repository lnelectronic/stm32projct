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
#define N5110_DIN       PA7
#define N5110_CLK       PA5


//rotary encoder pin    
#define encoderPinA     PB9 // CLK
#define encoderPinB     PB8 // DT
#define encoderButton   PB7 // SW

//Nokia 5110-related libraries
#include "math.h"
#include "N5110_SPI.h"
#if USESPI==1
#include <SPI.h>
#endif
N5110_SPI lcd(N5110_RST, N5110_CS, N5110_DC, N5110_DIN, N5110_CLK); // RST,CS,DC,DIN,CLK

//*** funtion menu */
#define MAIN_MENU 0
#define SUB_MENU 1

//int menuMode = MAIN_MENU; // 0: Main Menu, 1: Submenu
int mainMenuIndex = 0;
int subMenuIndex = 0;

//*** end funtion menu */

// -------------------------

#include <Arduino.h>
#include "c64enh_font.h"
//extern const uint8_t c64enh[];
//#include "times_dig_16x24_font.h"
//#include "term9x14_font.h"
#include "tiny3x7_font.h"
#include "small4x7_font.h"
#include "small5x7_font.h"
#include "small5x7bold_font.h"
#include "small5x6_font.h"

volatile int encoderSet = 0;
volatile int encoderPos = 0;
static volatile float numberOfTurns = 130;
static volatile int maxmainspeed = 1600;
static volatile float wireDia = 0.35;

// declared in main program
void editWireDiameter();
void showConfig();


void buttonInt() {}
void readEncoderInt()
{
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > 5) { // 5ms debounce
        int stateA = digitalRead(encoderPinA);
        int stateB = digitalRead(encoderPinB);

        if (stateA == LOW) {
            if (stateB != stateA) 
            {
              encoderPos++;
              encoderSet++;
            }
            else {
              encoderPos--;
              encoderSet--;
            }
        }
    }
    lastInterruptTime = interruptTime;
    Serial.print("Encoder Updated: ");
    Serial.println(encoderPos);
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

// --------------------------------------------------------------------------

char buf[25], buf2[15];

int numT = 0;
int curT = 0;
float bufT[300];
int graphStart = 0;

const char *menuTxt[] = {"Coil Run ", "config", "show config", "Help", "Reboot"};
const char *menuTxt1[] = {"Turn22 ", "VCC2", "Clock", "Backlight", "Contrast", "EEPROM dump", "Graph", "Help", "Reboot"};
//const char *menuTxt2[] = {"menu1 ", "menu2", "menu3", "menu4", "menu5", "menu6 ", "menu7", "menu8", "menu9"};
// Update the menu text to make it more descriptive
const char *menuTxt2[] = {"Turns", "Bobbin width", "Bobbin diameter", "wire diameter", "Exit"};
int numMenus = 0;
int menuLine;
int menuStart;
int numScrLines = 6;
int menuMode = -1; // -1 -> menu of options, 0..n -> option
int oldPos = 0;

// submenu variables
int subMenuLine = 0;
int subMenuStart = 0;
int subMenuNumLines = 6;
int subMenuMode = -1; // -1 -> menu of options, 0..n -> option
int subNumMenus = 0;


void setup()
{
  Serial.begin(115200);
  
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
  subNumMenus = sizeof(menuTxt2) / sizeof(char*);
  /* pinMode(N5110_BACKLIGHT, OUTPUT);
  analogWrite(N5110_BACKLIGHT, 0); // 0=max */ 
  menuMode = MAIN_MENU;
}


void reboot()
{
  if (encoderPos >= 1 * 2) encoderPos = 1 * 2;
  int st = encoderPos / 2;
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 1, "Reboot?");
  lcd.setInvert(st ? 0 : 1);
  lcd.printStr(10, 3, " NO ");
  lcd.setInvert(st ? 1 : 0);
  lcd.printStr(43, 3, " YES ");
  lcd.setInvert(0);
  if (readButton() <= 0) return;
  //menuMode = -1;
  menuMode = MAIN_MENU;
  lcd.clrScr();
  if (st > 0) { // yes
    lcd.printStr(ALIGN_CENTER, 2, "Rebooting ..."); delay(500);
    lcd.clrScr();
    

  }
  encoderPos = oldPos;
}


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
   // menuMode = -1;
   menuMode = MAIN_MENU;
    lcd.clrScr();
    encoderPos = oldPos;
    encoderSet = 0;
  }
}

void formatMenu(const char* src, char* dest, int len) {
  strncpy(dest, src, len);
  dest[len - 1] = '\0'; // Ensure null-termination
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

void showHelp()
{
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 0, "Help");
  lcd.setFont(Small4x7PL);
  lcd.setCR(1);
  lcd.printStr(0, 1, "Use encoder to select menu item. Press button to exit.");
  lcd.setCR(0);
}

void setTurnNumber()
{
    snprintf(buf, 6, " %d ", (int)numberOfTurns);
    lcd.printStr(ALIGN_CENTER, 2, buf);
    static int lastEncoderPos = 0;
    if (encoderSet != lastEncoderPos) {
        numberOfTurns += (encoderSet - lastEncoderPos);  // Adjust based on encoder movement
        lastEncoderPos = encoderSet;  // Update last position
    }

    // static int lastEncoderPos = encoderSet;  // Keep track of last encoder position persistently
    // int change = encoderSet - lastEncoderPos;  // Calculate change in encoder position
    // if (change != 0) {
    //     numberOfTurns += change;  // Update numberOfTurns
    //     numberOfTurns = constrain(numberOfTurns, 0, 9999);  // Ensure within bounds
    //     lastEncoderPos = encoderSet;  // Store the last position
    // }

    snprintf(buf, 6, " %d ", (int)numberOfTurns);
    lcd.printStr(ALIGN_CENTER, 5, buf);
    // Keep numberOfTurns within range (0 to 9999)
    numberOfTurns = constrain(numberOfTurns, 0, 9999);

    // Format and display the value on LCD
    snprintf(buf, 6, " %d ", (int)numberOfTurns);
    lcd.setFont(Small5x7PL);
    lcd.setCharMinWd(5);
    lcd.setDigitMinWd(5);
    lcd.printStr(ALIGN_CENTER, 1, buf);
    lcd.printStr(ALIGN_LEFT, 1, "000");
    lcd.printStr(ALIGN_RIGHT, 1, "255");
    
    
    Serial.print("EncoderSet: ");
    Serial.println(encoderSet);
    Serial.print("EncoderPos: ");
    Serial.println(encoderPos);
    Serial.print("NumberOfTurns: ");
    Serial.println(numberOfTurns);

}

void displayMenu(const char* menuTxt[], int numMenus, int menuLine, int menuStart, int numScrLines) {
    for (int i = 0; i < numScrLines; i++) {
        if (i + menuStart < numMenus) {
            lcd.setInvert(i + menuStart == menuLine ? 1 : 0);
            formatMenu(menuTxt[i + menuStart], buf, sizeof(buf));
            lcd.printStr(ALIGN_LEFT, i, buf);
        }
    }
}

void enterSubMenu(int mainMenuItem) {
  // Save current main menu position
  mainMenuIndex = mainMenuItem;
  
  // Switch to submenu mode
  menuMode = SUB_MENU;
  
  // Reset encoder position for submenu navigation
  oldPos = encoderPos;
  encoderPos = 0;
  
  // Clear the screen for submenu display
  lcd.clrScr();
  
  // Reset submenu position to top
  subMenuLine = 0;
  subMenuStart = 0;
}

void exitSubMenu() {
  // Return to main menu
  menuMode = MAIN_MENU;
  
  // Restore encoder position for main menu
  encoderPos = mainMenuIndex * 2;
  
  // Clear screen
  lcd.clrScr();
}

void editNumberOfTurns() {
  lcd.clrScr();
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 0, "Edit Turns");
  
  // Store initial value to allow cancelling
  float initialValue = numberOfTurns;
  int editMode = 1; // 1: editing, 0: confirmation
  
  // Reset encoder for editing
  int startEncoderPos = encoderSet;
  
  while (1) {
    // Clear the value display area
    lcd.setFont(Small5x7PL);
    lcd.clrScr(); // Clear the value area
    
    if (editMode) {
      // Editing mode
      // Update numberOfTurns based on encoder movement
      int encoderDelta = encoderSet - startEncoderPos;
      numberOfTurns = initialValue + encoderDelta;
      
      // Constrain value between 0 and 9999
      numberOfTurns = constrain(numberOfTurns, 0, 9999);
      
      // Display current value and range
      snprintf(buf, 12, "Value: %d", (int)numberOfTurns);
      lcd.printStr(ALIGN_CENTER, 2, buf);
      lcd.printStr(ALIGN_LEFT, 4, "Min: 0");
      lcd.printStr(ALIGN_RIGHT, 4, "Max: 9999");
      
      // Display instructions
      lcd.printStr(ALIGN_CENTER, 5, "Click to confirm");
    } else {
      // Confirmation mode
      lcd.printStr(ALIGN_CENTER, 2, "Save changes?");
      lcd.setInvert(encoderPos % 2 == 0 ? 1 : 0);
      lcd.printStr(15, 4, " Yes ");
      lcd.setInvert(encoderPos % 2 == 0 ? 0 : 1);
      lcd.printStr(45, 4, " No ");
      lcd.setInvert(0);
    }
    
    // Handle button press
    if (readButton() > 0) {
      if (editMode) {
        // Switch to confirmation mode
        editMode = 0;
      } else {
        // Save or cancel based on selection
        if (encoderPos % 2 == 0) {
          // Yes - save value and exit
          Serial.print("Saved numberOfTurns: ");
          Serial.println(numberOfTurns);
          return;
        } else {
          // No - revert to initial value and exit
          numberOfTurns = initialValue;
          Serial.println("Edit cancelled");
          return;
        }
      }
    }
    
    // Small delay to avoid flickering
    delay(50);
  }
}

void backToMainMenu() {
  // Switch back to main menu mode
  menuMode = MAIN_MENU;
  
  // Restore encoder position to last main menu position
  encoderPos = mainMenuIndex * 2;
  
  // Clear screen to prepare for main menu display
  lcd.clrScr();
  
  Serial.println("Returned to main menu");
}

void menuConfig() {
  lcd.setFont(Small5x6PL);
  lcd.setCharMinWd(5);
  lcd.setDigitMinWd(5);

  if (encoderPos < 0) encoderPos = 0;

  subMenuLine = encoderPos / 2;
  if (subMenuLine >= subNumMenus) {
    subMenuLine = subNumMenus - 1;
    encoderPos = subMenuLine * 2;
  }
  if (subMenuLine >= subMenuStart + subMenuNumLines) subMenuStart = subMenuLine - subMenuNumLines + 1;
  if (subMenuLine < subMenuStart) subMenuStart = subMenuLine;

  // Display the submenu
  displayMenu(menuTxt2, subNumMenus, subMenuLine, subMenuStart, subMenuNumLines);
  drawMenuSlider();

  // Handle submenu item selection
  if (readButton() > 0) {
    switch (subMenuLine) {
      case 0:
        // Call the edit function for numberOfTurns
        editNumberOfTurns();
        lcd.clrScr(); // Clear screen after editing
        break;
      case 1:
        // Example of different functionality
        showHelp();
        lcd.clrScr();
        break;
      case 2:
        // Add function for submenu item 2
        setTurnNumber(); // Your existing function
        lcd.clrScr();
        break;
      case 3:
        // Wire diameter option
        editWireDiameter(); // Ensure this function is implemented correctly
        lcd.clrScr();
        break;
      case 4: // Exit option
        // Go back to main menu
        backToMainMenu();
        return; // Exit submenu handling
      default:
        // Default action for unimplemented items
        lcd.clrScr();
        lcd.setFont(c64enh);
        lcd.printStr(ALIGN_CENTER, 2, "Not implemented");
        delay(1000);
        lcd.clrScr();
        break;
    }
  }
}

bool checkLongPress() {
  // Check for long press (button held for >1 second)
  static unsigned long pressStartTime = 0;
  static bool buttonWasPressed = false;
  
  int buttonState = digitalRead(encoderButton);
  
  if (buttonState == LOW && !buttonWasPressed) {
    // Button just pressed
    pressStartTime = millis();
    buttonWasPressed = true;
    return false;
  } 
  else if (buttonState == LOW && buttonWasPressed) {
    // Button still pressed
    if (millis() - pressStartTime > 1000) { // 1 second threshold
      // Long press detected
      buttonWasPressed = false;
      return true;
    }
    return false;
  }
  else if (buttonState == HIGH && buttonWasPressed) {
    // Button released before long press
    buttonWasPressed = false;
  }
  
  return false;
}

void handleMenu() {
  lcd.setFont(Small5x6PL);
  lcd.setCharMinWd(5);
  lcd.setDigitMinWd(5);
  
  if (encoderPos < 0) encoderPos = 0;
  
  // Main menu handling
  if (menuMode == MAIN_MENU) {
    menuLine = encoderPos / 2;
    if (menuLine >= numMenus) {
      menuLine = numMenus - 1;
      encoderPos = menuLine * 2;
    }
    if (menuLine >= menuStart + numScrLines) menuStart = menuLine - numScrLines + 1;
    if (menuLine < menuStart) menuStart = menuLine;
    
    displayMenu(menuTxt, numMenus, menuLine, menuStart, numScrLines);
    drawMenuSlider();
    
    if (readButton() > 0) {
      // Store the current main menu position
      mainMenuIndex = menuLine;
      
      // Check which menu item was selected
      switch (menuLine) {
        case 0: // "Coil Run"
          menuMode = SUB_MENU;
          oldPos = encoderPos;
          encoderPos = 0;
          lcd.clrScr();
          break;
        case 1: // "config"
          menuMode = SUB_MENU;
          oldPos = encoderPos;
          encoderPos = 0;
          lcd.clrScr();
          break;
        case 2: // "show config"
          // Show existing configuration
          showConfig();
          break;
        case 3: // "Help"
          setMenu(menuLine);
          break;
        case 4: // "Reboot"
          reboot();
          break;
        default:
          setMenu(menuLine);
          break;
      }
    }
  } 
  // Submenu handling
  else if (menuMode == SUB_MENU) {
    menuConfig();
  }
  // Handle specific menu options (for legacy code compatibility)
  else if (menuMode >= 0) {
    if (menuMode == 1) { showHelp(); endMenu(); } else
    if (menuMode == 2) { showHelp(); endMenu(); } else
    if (menuMode == 3) { showHelp(); endMenu(); } else
    if (menuMode == 4) { reboot(); endMenu(); } else
    { menuMode = MAIN_MENU; lcd.clrScr(); }
  }
}

void editWireDiameter() {
  lcd.clrScr();
  lcd.setFont(c64enh);
  lcd.printStr(ALIGN_CENTER, 0, "Edit Wire Dia");

  // Store initial value to allow canceling
  int initialValue = wireDia;
  int editMode = 1; // 1: editing, 0: confirmation

  // Reset encoder for editing
  int startEncoderPos = encoderSet;

  while (1) {
    // Clear the value display area
    lcd.setFont(Small5x7PL);
    lcd.clrScr(); // Clear the value area

    if (editMode) {
      // Editing mode
      // Update wireDia based on encoder movement
      int encoderDelta = encoderSet - startEncoderPos;
      wireDia = initialValue + encoderDelta;

      // Constrain value between 0 and 100 (example range)
      wireDia = constrain(wireDia, 0, 100);

      // Display current value and range
      snprintf(buf, 12, "Value: %d", wireDia);
      lcd.printStr(ALIGN_CENTER, 2, buf);
      lcd.printStr(ALIGN_LEFT, 4, "Min: 0");
      lcd.printStr(ALIGN_RIGHT, 4, "Max: 100");

      // Display instructions
      lcd.printStr(ALIGN_CENTER, 5, "Click to confirm");
    } else {
      // Confirmation mode
      lcd.printStr(ALIGN_CENTER, 2, "Save changes?");
      lcd.setInvert(encoderPos % 2 == 0 ? 1 : 0);
      lcd.printStr(15, 4, " Yes ");
      lcd.setInvert(encoderPos % 2 == 0 ? 0 : 1);
      lcd.printStr(45, 4, " No ");
      lcd.setInvert(0);
    }

    // Handle button press
    if (readButton() > 0) {
      if (editMode) {
        // Switch to confirmation mode
        editMode = 0;
      } else {
        // Save or cancel based on selection
        if (encoderPos % 2 == 0) {
          // Yes - save value and exit
          Serial.print("Saved wireDia: ");
          Serial.println(wireDia);
          return;
        } else {
          // No - revert to initial value and exit
          wireDia = initialValue;
          Serial.println("Edit canceled");
          return;
        }
      }
    }

    // Small delay to avoid flickering
    delay(50);
  }
}

void showConfig() {
    lcd.clrScr();
    lcd.setFont(c64enh);
    lcd.printStr(ALIGN_CENTER, 0, "Configuration");
    
    lcd.setFont(Small5x7PL);
    
    // Show Number of Turns with proper formatting
    char turns_buf[20];
    dtostrf(numberOfTurns, 4, 0, turns_buf);
    char display_buf[20];
    snprintf(display_buf, sizeof(display_buf), "Turns: %s", turns_buf);
    lcd.printStr(ALIGN_LEFT, 2, display_buf);
    
    // Show Maximum Speed
    snprintf(display_buf, sizeof(display_buf), "Speed: %d", maxmainspeed);
    lcd.printStr(ALIGN_LEFT, 3, display_buf);
    
    // Show Wire Diameter with proper decimal places
    char wire_buf[20];
    dtostrf(wireDia, 4, 2, wire_buf); // Convert float to string with 2 decimal places
    snprintf(display_buf, sizeof(display_buf), "Wire: %smm", wire_buf);
    lcd.printStr(ALIGN_LEFT, 4, display_buf);
    
    //lcd.drawRect(0, 0, 83, 47); // Add border
    lcd.printStr(ALIGN_CENTER, 5, "Press to exit");
    
    while (!readButton()) {
        delay(50);
    }
}

void loop() {
  // Check for long press to return to main menu from anywhere
  if (checkLongPress()) {
    backToMainMenu();
  }
  
  handleMenu();
}