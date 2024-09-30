#include "tm1637menuManager.h"

MenuManager::MenuManager(TM1637* display, MenuObject* rootMenu) {
    this->display = display;
    this->currentMenu = rootMenu;
    this->menuStackPointer = 0;
    this->currentIndex = 0;
}

MenuObject* MenuManager::getCurrentMenu() {
    return currentMenu;
}

void MenuManager::enterMenu() {
    if (menuStackPointer == 0){ // in mainMenu
      currentMainMenuObject = currentIndex; //go back to the mainMenu that last was selected
    }
    if (currentMenu->getSubMenuCount() > 0) {
      menuStack[menuStackPointer++] = currentMenu;
      currentMenu = currentMenu->getSubMenu(currentIndex);
      currentSubMenuObject = currentIndex; // holds currentindex temperaly
      currentIndex = 0;

      // check it again | overflow protection
      if(currentMenu->getSubMenuCount() == 0){ // prevent extra menuStackPointer that not exists
        currentMenu = menuStack[--menuStackPointer]; // noting there -> go back
        currentIndex = currentSubMenuObject;
      }
      
    }
    /*// DEBUG
    display->setDisplayToDecNumber(currentMenu->getSubMenuCount(),_BV(4));
    delay(500);
    display->setDisplayToDecNumber(menuStackPointer, _BV(4));
    delay(500);
    display->setDisplayToDecNumber(currentMenu, _BV(4));
    delay(500);
    */
    showCurrentMenu();    
}

void MenuManager::exitMenu() {
    if (menuStackPointer > 0) { // still in a subMenu
      currentMenu = menuStack[--menuStackPointer];
      if (menuStackPointer == 0){ // in the mainMenu
        currentIndex = currentMainMenuObject; // get the recently use menuNR
        currentMainMenuObject = 0; // reset currentMainMenuObject to 0
      }
    }else if (currentIndex > 0 ){ // still in the last stored mainmenu
      currentIndex = 0; // reset currentIndex to 0 -> go to the first mainMenu
    }else { // go te the default menu 
      currentIndex = 0; // set mainMenu NR
      menuStackPointer = 1; // set subMenu NR
      menuStack[menuStackPointer] = currentMenu; // apply it
      currentMenu = currentMenu->getSubMenu(currentIndex); // apply it
    } 
    showCurrentMenu();
}

void MenuManager::nextItem() {
    currentIndex = (currentIndex + 1) % currentMenu->getSubMenuCount();
    showCurrentMenu();
}

void MenuManager::previousItem() {
    currentIndex = (currentIndex - 1 + currentMenu->getSubMenuCount()) % currentMenu->getSubMenuCount();
    showCurrentMenu();
}

void MenuManager::showCurrentMenu(bool dmxDot) {
    if (currentMenu->getSubMenuCount() > 0) {
        showText(currentMenu->getSubMenu(currentIndex)->getName().c_str(), currentMenu->getSubMenu(currentIndex)->getDot()+dmxDot);
    } else {
        showText(currentMenu->getName().c_str(), currentMenu->getSubMenu(currentIndex)->getDot()+dmxDot);
    }
}

void MenuManager::showText(const char* text, int dot) {
    display->setDisplayToString(text, dot);  // Gebruik de juiste methode om tekst te tonen op het 7-segment display
}
