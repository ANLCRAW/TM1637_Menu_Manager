#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <TM1637.h>  // We gebruiken de "TM16xxLEDs and Buttons" bibliotheek 
#include <TM16xxDisplay.h>
#include <TM16xxButtons.h> 
#include "tm1637menuObject.h"

class MenuManager {
private:
    TM1637* display;
    MenuObject* currentMenu;
    MenuObject* menuStack[MAX_SUBMENUS];
    int menuStackPointer;
    int currentIndex;
    int currentMainMenuObject;
    int currentSubMenuObject;

public:
    MenuManager(TM1637* display, MenuObject* rootMenu);

    MenuObject* getCurrentMenu();

    void enterMenu();
    void exitMenu();
    void nextItem();
    void previousItem();
    void showCurrentMenu();
    void showText(const char* text, int dot = 0); //  no visable dot 
};

#endif
