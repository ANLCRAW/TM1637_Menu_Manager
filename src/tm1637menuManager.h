#ifndef MENUMANAGER_H
#define MENUMANAGER_H


#define DEBUG 0
#define DEBUG_EX 0

#if DEBUG
#define D_begin(...)       Serial.begin(__VA_ARGS__)
#define D_printf(...)      Serial.printf(__VA_ARGS__)
#define D_print(...)       Serial.print(__VA_ARGS__)
#define D_println(...)     Serial.println(__VA_ARGS__)
#else
#define D_begin(...)      
#define D_printf(...) 
#define D_print(...)
#define D_println(...)
#endif

#if DEBUG_EX
#define DEX_begin(...)       Serial.begin(__VA_ARGS__)
#define DEX_printf(...)      Serial.printf(__VA_ARGS__)
#define DEX_print(...)       Serial.print(__VA_ARGS__)
#define DEX_println(...)     Serial.println(__VA_ARGS__)
#else
#define DEX_begin(...)  
#define DEX_printf(...)
#define DEX_print(...)
#define DEX_println(...)
#endif

#include <TM1637.h>  // We gebruiken de "TM16xxLEDs and Buttons" bibliotheek 
#include <TM16xxDisplay.h>
#include <TM16xxButtons.h> 
#include "tm1637menuObject.h"

class MenuManager {
private:
    TM1637* display; // Pointer to the TM1637 display object
    MenuObject* currentMenu; // Pointer to the current menu object
    MenuObject* menuStack[MAX_SUBMENUS]; // Stack to hold menu objects for navigation
    bool findMenuPath(MenuObject* root, MenuObject* target, int depth, int &newStackPointer, int &mainMenuIndex);
    int menuStackPointer; // Pointer to the current position in the menu stack
    int currentIndex; // Index of the current submenu in the current menu
    int currentMenuObject[MAX_SUBMENUS]; // Store current index for each menu level
    void printDebug();

public:
    MenuManager(TM1637* display, MenuObject* rootMenu);

    ~MenuManager();

    MenuObject* getCurrentMenu();

    void enterMenu();
    void exitMenu();
    void nextItem();
    void previousItem();
    void showCurrentMenu(bool dmxDot = false);
    void showCurrentScriptMenu(bool dmxDot = false);
    void darkScreen(bool dmxDot = false);
    void showText(const char* text, int dot = 0); //  no visable dot 
    bool getCurrentMenuIsMain();
    bool getTargetMenuIsMain(MenuObject* targetMenu);
    bool getCurrentMenuIsSub();
    bool getCurrentMenuIsSubSub();
    int getCurrentIndex();
    void jumpToMenu(MenuObject* targetMenu);
};

#endif
