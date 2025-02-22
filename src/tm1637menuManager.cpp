#include "tm1637menuManager.h"

MenuManager::MenuManager(TM1637* display, MenuObject* rootMenu) {
    this->display = display;
    this->currentMenu = rootMenu;
    this->menuStackPointer = 0;
    this->currentIndex = 0;

}

// Destructor for MenuManager
MenuManager::~MenuManager() {
    // Delete the dynamically allocated root menu and all its submenus
    delete currentMenu;
}

MenuObject* MenuManager::getCurrentMenu() {
    // Check if the current menu has submenus and return the first one if available
    if (currentMenu && currentMenu->getSubMenuCount() > 0) {
        return currentMenu->getSubMenu(currentIndex);  // Return the first submenu
    }
    return currentMenu;  // If no submenus, return the current menu itself
}

bool MenuManager::getCurrentMenuIsMain() {
    bool isMain =  menuStackPointer == 0 ? true : false; //in mainmMenu
    return isMain;
}

bool MenuManager::getTargetMenuIsMain(MenuObject* targetMenu) {

    // **Find the top-level menu**
    MenuObject* root = (menuStackPointer > 0) ? menuStack[0] : currentMenu;
    for (int i = 0; i < root->getSubMenuCount(); i++) {
        // **If target menu found**
        if (root == targetMenu) {
            D_println("Already in a main menu, you can ONLY jump to subMenu's , ABORTING JUMP!");
            return true;
        } else{
            return false;
        }
    }
    // If loop finishes without finding the target menu, return false
    return false;
}

bool MenuManager::getCurrentMenuIsSub() {
    bool isSub = menuStackPointer > 0 ? true : false; // in sub menu
    return isSub;
}

bool MenuManager::getCurrentMenuIsSubSub() {
    bool isSubSub = menuStackPointer > 1 ? true : false; // in sub sub menu
    return isSubSub;
}

void MenuManager::jumpToMenu(MenuObject* targetMenu) {
    if (!targetMenu) return;

     // Check if the current menu is a main menu
    if (getTargetMenuIsMain(targetMenu)) {
        return;  // Abort the jump if we're in a main menu
    }

      D_println("Jumping to menu...");

      int newStackPointer = 0;
      int mainMenuIndex = 0;

      // **Find the top-level menu**
      MenuObject* rootMenu = (menuStackPointer > 0) ? menuStack[0] : currentMenu;

      // **Find the path to the target menu**
      if (!findMenuPath(rootMenu, targetMenu, 0, newStackPointer, mainMenuIndex)) {
          D_println("Target menu not found in tree!");
          return;
      }

      // **Ensure stack pointer is within bounds**
      if (newStackPointer >= MAX_SUBMENUS) {
          D_println("ERROR: Stack overflow detected, ABORTING!");
          return;
      }

      // **Update menu navigation variables**
      menuStackPointer = newStackPointer - 1;
      currentMenu = targetMenu;  // This should update correctly
      currentIndex = mainMenuIndex;  // Ensure correct submenu index
      

      D_println("Jump successful!");
      D_printf("  New Stack Pointer: %u\n", menuStackPointer);
      D_printf("  Main Menu Index: %u\n", mainMenuIndex);
      D_printf("  Current Menu: %u\n", currentMenu ? currentMenu->getName() : "NULL");

      showCurrentMenu();
}

int MenuManager::getCurrentIndex(){
    return currentMenuObject[menuStackPointer];
    //return currentIndex;
}

bool MenuManager::findMenuPath(MenuObject* root, MenuObject* target, int depth, int &newStackPointer, int &mainMenuIndex) {
    if (!root || depth >= MAX_SUBMENUS) return false;

    for (int i = 0; i < root->getSubMenuCount(); i++) {
        MenuObject* subMenu = root->getSubMenu(i);
        DEX_printf("Checking submenu: %u\n", subMenu->getName());

        // **If target menu found, update stack**
        if (subMenu == target) {
            D_println("Target menu found!");
            newStackPointer = depth + 1;
            mainMenuIndex = i;
            DEX_printf("mainMenuIndex: %u\n", mainMenuIndex);
            menuStack[depth] = root;  // Store parent in stack

            // **STOP SEARCH if target is a main menu**
            if (depth == 0) {
                D_println("Target is a MAIN MENU. Stopping search.");
                newStackPointer = 0; // is still a mainMenu
                return true;
            }

            return true;
        } else{
          currentMenuObject[0] = i;
        }

        // **Recursively search deeper**
        if (findMenuPath(subMenu, target, depth + 1, newStackPointer, mainMenuIndex)) {
            menuStack[depth] = root;  // Store parent in stack
            return true;
        }
    }
    return false;
}



void MenuManager::enterMenu() {
    
    if (currentMenu->getSubMenuCount() > 0) { // has submenu's
      menuStack[menuStackPointer++] = currentMenu; // add one sub layer
      
      currentMenu = currentMenu->getSubMenu(currentIndex); // set current menu
      currentIndex = currentMenuObject[menuStackPointer]; // get the previous stored index

      // check it again | overflow protection
      if(currentMenu->getSubMenuCount() == 0){ // prevent extra menuStackPointer that not exists
        currentMenu = menuStack[--menuStackPointer]; // noting there -> go back
        currentIndex = currentMenuObject[menuStackPointer]; // get the previous stored index
      }
      
    }
    
    printDebug();
    showCurrentMenu();    
}

void MenuManager::exitMenu() {
    currentMenuObject[menuStackPointer] = currentIndex; //save index before leaving

    if (menuStackPointer > 0) { // still in a subMenu
      currentMenu = menuStack[--menuStackPointer];
      currentIndex = currentMenuObject[menuStackPointer]; // get the recently use menuNR
      
    }else if (currentIndex > 0 ){ // still in the last stored mainmenu
      currentIndex = 0; // reset currentIndex to 0 -> go to the first mainMenu
      currentMenuObject[menuStackPointer] = currentIndex;
    }//else { // go te the default menu 
    
    
    printDebug();
    showCurrentMenu();
}

void MenuManager::nextItem() {
    currentIndex = (currentIndex + 1) % currentMenu->getSubMenuCount(); // add +1 to index
    currentMenuObject[menuStackPointer] = currentIndex; // save the new index
    
    printDebug();
    showCurrentMenu();
}

void MenuManager::previousItem() {
    currentIndex = (currentIndex - 1 + currentMenu->getSubMenuCount()) % currentMenu->getSubMenuCount(); // add -1 to index
    currentMenuObject[menuStackPointer] = currentIndex; // save the new index
    
    printDebug();
    showCurrentMenu();
}

void MenuManager::printDebug(){
    DEX_printf("currentMenuObject[%u]: %u %s\n", menuStackPointer, currentMenuObject[menuStackPointer], menuStackPointer == 0 ? "-> MAIN" : "");
    DEX_printf("submenucount: %u\n", currentMenu->getSubMenuCount());
    DEX_printf("menuStackPointer: %u\n", menuStackPointer);
    DEX_printf("currentIndex: %u\n", currentIndex);
    DEX_printf("currenMenu NAME: %u\n", currentMenu ? currentMenu->getName() : "NULL");
}

void MenuManager::showCurrentMenu(bool dmxDot) {
  // Check if currentMenu has submenus
  if (currentMenu->getSubMenuCount() > 0) {
      // Show the submenu name if there are submenus
      showText(currentMenu->getSubMenu(currentIndex)->getName().c_str(), 
              currentMenu->getSubMenu(currentIndex)->getDot() + dmxDot);
  } else {
      // If there are no submenus, show the main menu name
      showText(currentMenu->getName().c_str(), currentMenu->getDot() + dmxDot);
  }
}

void MenuManager::darkScreen(bool dmxDot){
    showText("    ", dmxDot);
}

void MenuManager::showCurrentScriptMenu(bool dmxDot){
    showText(currentMenu->getName().c_str(), currentMenu->getDot()+dmxDot);
}

void MenuManager::showText(const char* text, int dot) {
    display->setDisplayToString(text, dot);  // Gebruik de juiste methode om tekst te tonen op het 7-segment display
}
