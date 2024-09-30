#include <tm1637menuManager.h>

// Definieer pinnen voor TM1637
#define CLK A1
#define DIO A2

// Definieer knop pinnen
#define ENTER 0x01
#define MENU  0x100
#define UP    0x02
#define DOWN  0x200

// DMX
#define DMX_MAX 512

// display
char screenData[5];                             // Stores all the Chars of Strings that is displays on the screen

// Initialiseer TM1637Display
TM1637 tm1637(CLK, DIO);
TM16xxButtons buttons(&tm1637);       // TM16xx button 
TM16xxDisplay display(&tm1637, 4);    // TM16xx object, 4 digits

int dmxAddress = 1; // Default 1-512

// Maak menu-items aan
//MenuObject rootMenu("ROOT");
    MenuObject mainMenu("MAIN");
    MenuObject addressMenu("Addr");
        MenuObject changeAddressMenu("");
    MenuObject channelMenu("CHAN");
        MenuObject mode1("CH 3", 4);
        MenuObject mode2("CH 5", 4);
        MenuObject mode3("CH 8", 4);
        MenuObject mode4("CH48", 4);
        MenuObject mode5("CH96", 4);
    MenuObject staticMenu("STAT");
        MenuObject colorSetMenu("COLO");
            MenuObject colorR("R___", 8);
            MenuObject colorG("G___", 8);
            MenuObject colorB("b___", 8);
        MenuObject presetColorMenu("PRES");
    MenuObject autoMenu("AUTO");
    MenuObject soundMenu("SOUN");
    MenuObject sensorMenu("SENS");
    MenuObject speedMenu("SPEE");
    MenuObject dimMenu("dInn");
    MenuObject irMenu("IR  ");
    MenuObject lostMenu("LOST");
    MenuObject displayMenu("dISP");
        MenuObject brightnessMenu("brtnS"); // brightness
            MenuObject changeBrightnessMenu("");
        MenuObject flipMenu("FLIP"); // flip
    MenuObject slaveMenu("SLAu");
    MenuObject infoMenu("INFO");
    MenuObject adminMenu("ROOT");



// *** Globaal MenuManager object ***
MenuManager menuManager(&tm1637, &mainMenu);

void setScreenData(const char* string, int dotPos = 0, int printValue = 0){
  sprintf(screenData, string, printValue);
  display.setDisplayToString(screenData, dotPos); // Dot is off
}

void setup() {

    // Zet helderheid
    tm1637.setupDisplay(true, 3);

    mainMenu.addSubMenu(&addressMenu);
        addressMenu.addSubMenu(&changeAddressMenu);
    mainMenu.addSubMenu(&channelMenu);
        channelMenu.addSubMenu(&mode1);
        channelMenu.addSubMenu(&mode2);
        channelMenu.addSubMenu(&mode3);
        channelMenu.addSubMenu(&mode4);
        channelMenu.addSubMenu(&mode5);
        
    // Initialiseer en toon het root-menu
    menuManager.showCurrentMenu();
}

void loop() {

  uint32_t dwButtons=buttons.tick(); 
    // Voorbeeldknopnavigatie
    switch(dwButtons){
      case MENU:
        menuManager.enterMenu();  // **Correct gebruik van object menuManager**
        delay(200); // Debounce delay
      break;
      case ENTER:
        menuManager.exitMenu();  // **Correct gebruik van object menuManager**
        delay(200);
      break;
      case UP:
        menuManager.nextItem();  // **Correct gebruik van object menuManager**
        delay(200);
      break;
      case DOWN:
        menuManager.previousItem();  // **Correct gebruik van object menuManager**
        delay(200);
      break;
    }

  // Controleer het huidige menu
    MenuObject* currentMenu = menuManager.getCurrentMenu();

    if (currentMenu == &addressMenu) {
      sprintf(screenData, "A%0003u", dmxAddress);
      menuManager.showText(screenData, 8); 
      switch(dwButtons){
      case UP:
        dmxAddress = (dmxAddress % DMX_MAX) + 1; // 1 tot 7 en 7 tot 1
        break;
      case DOWN:
        dmxAddress = (dmxAddress + DMX_MAX-2) % DMX_MAX + 1; // 1 tot 7 en 7 tot 1
        break;
      }
    }else {
      // More here
    }

}

