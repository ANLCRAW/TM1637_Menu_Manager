#include <tm1637menuManager.h>

// Define TM1637 pins
#define CLK 26
#define DIO 25

// Define button pins
#define ENTER 0x01
#define MENU  0x100
#define UP    0x02
#define DOWN  0x200

// display
char screenData[5];  // Stores all the Chars of Strings that is displays on the screen
#define debounceDelay 200

byte printValue;
#define MAX_PRINTVALUE 99

// Initialize TM1637Display
TM1637 tm1637(CLK, DIO);
TM16xxButtons buttons(&tm1637);       // TM16xx button 
TM16xxDisplay display(&tm1637, 4);    // TM16xx object, 4 digits


// Create menu-items
    MenuObject mainMenu("MAIN");
    MenuObject main0("nn 0");
        MenuObject sub01("Sub0");
    MenuObject main1("nn 1");
        MenuObject sub10("Sub0", 2);
        MenuObject sub11("Sub1", 2);
          MenuObject subsub0("SS 0", 4);
          MenuObject subsub1("SS 1", 4);
            MenuObject valueMenu("");


MenuManager menuManager(&tm1637, &mainMenu);

void setScreenData(const char* string, int dotPos = 0, int printValue = 0){
  sprintf(screenData, string, printValue);
  display.setDisplayToString(screenData, dotPos); // Dot is off
}

void setup() {
  // Set Brightness to 3 -> 1-7
  tm1637.setupDisplay(true, 3);

  mainMenu.addSubMenu(&main0);
      main0.addSubMenu(&sub01);
  mainMenu.addSubMenu(&main1);
      main1.addSubMenu(&sub10);
      main1.addSubMenu(&sub11);
        sub11.addSubMenu(&subsub0);
        sub11.addSubMenu(&subsub1);
          subsub1.addSubMenu(&valueMenu);
      
  // Initialiseer en toon het root-menu
  menuManager.showCurrentMenu();
}

void loop() {

  uint32_t dwButtons=buttons.tick(); 
  // Voorbeeldknopnavigatie
  switch(dwButtons){
    case MENU:
      menuManager.enterMenu();  // **Correct gebruik van object menuManager**
      delay(debounceDelay); // Debounce delay
    break;
    case ENTER:
      menuManager.exitMenu();  // **Correct gebruik van object menuManager**
      delay(debounceDelay);
    break;
    case UP:
      menuManager.nextItem();  // **Correct gebruik van object menuManager**
      delay(debounceDelay);
    break;
    case DOWN:
      menuManager.previousItem();  // **Correct gebruik van object menuManager**
      delay(debounceDelay);
    break;
  }

  // Check the menu
  MenuObject* currentMenu = menuManager.getCurrentMenu();

  if (currentMenu == &valueMenu) {
    setScreenData("u=%2u", 0, printValue);
    menuManager.showText(screenData); 
    switch(dwButtons){
    case UP:
      printValue = (printValue % MAX_PRINTVALUE) + 1; // 1 tot 7 en 7 tot 1
      break;
    case DOWN:
      printValue = (printValue + MAX_PRINTVALUE-2) % MAX_PRINTVALUE + 1; // 1 tot 7 en 7 tot 1
      break;
    }
    }else {
      // More here
    }

}

