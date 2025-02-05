#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#define DEBUG 1
#define DEBUG_EX 0

#if DEBUG
#define D_print(...)       Serial.print(__VA_ARGS__)
#define D_println(...)     Serial.println(__VA_ARGS__)
#else
#define D_print(...)
#define D_println(...)
#endif

#if DEBUG_EX
#define DEX_print(...)       Serial.print(__VA_ARGS__)
#define DEX_println(...)     Serial.println(__VA_ARGS__)
#else
#define DEX_print(...)
#define DEX_println(...)
#endif

#include <TM1637.h>  // We gebruiken de "TM16xxLEDs and Buttons" bibliotheek 
#include <TM16xxDisplay.h>
#include <TM16xxButtons.h> 
#include "tm1637menuObject.h"

class MenuManager {
private:
    TM1637* display;
    MenuObject* currentMenu;
    MenuObject* menuStack[MAX_SUBMENUS];
    bool findMenuPath(MenuObject* root, MenuObject* target, int depth, int &newStackPointer, int &mainMenuIndex);
    int menuStackPointer;
    int currentIndex;
    int currentMainMenuObject;
    int currentSubMenuObject;

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
    void showText(const char* text, int dot = 0); //  no visable dot 
    bool getCurrentMenuIsMain();
    bool getTargetMenuIsMain(MenuObject* targetMenu);
    bool getCurrentMenuIsSub();
    bool getCurrentMenuIsSubSub();
    void jumpToMenu(MenuObject* targetMenu);
    
};

#endif
