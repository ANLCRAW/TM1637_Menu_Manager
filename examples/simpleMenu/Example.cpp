#include <TM1637.h>
#include <tm1637menuManager.h>
#include <tm1637menuObject.h>

// Create TM1637 display
TM1637 display(26, 25); // CLK, DIO

// Create menu objects
MenuObject mainMenu("MAIN");
MenuObject settingsMenu("SET");
MenuObject aboutMenu("INFO");

// Create submenus
MenuObject brightnessMenu("BRI");
MenuObject volumeMenu("VOL");

// Create MenuManager
MenuManager menu(&display, &mainMenu);

void setup() {

  // Build menu structure
  mainMenu.addSubMenu(&settingsMenu);
  mainMenu.addSubMenu(&aboutMenu);
  settingsMenu.addSubMenu(&brightnessMenu);
  settingsMenu.addSubMenu(&volumeMenu);
  // Initialize display
  display.begin();
  display.setupDisplay(true, 7);
  menu.showCurrentMenu();
}

void loop() {
  // Example navigation:
  menu.nextItem();   // Move to next item
  menu.enterMenu();  // Enter submenu
}