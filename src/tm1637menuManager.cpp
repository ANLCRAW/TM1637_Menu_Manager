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
            D_println("Already in a main menu, you can ONLY jump to subMenu's , aborting jump.");
            return true;
        } else{
            return false;
        }
    }
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
          D_println("ERROR: Stack overflow detected, aborting!");
          return;
      }

      // **Update menu navigation variables**
      menuStackPointer = newStackPointer - 1;
      currentMenu = targetMenu;  // This should update correctly
      currentIndex = mainMenuIndex;  // Ensure correct submenu index
      

      D_println("Jump successful:");
      D_print("  New Stack Pointer: "); D_println(menuStackPointer);
      D_print("  Main Menu Index: "); D_println(mainMenuIndex);
      D_print("  Current Menu: "); D_println(currentMenu ? currentMenu->getName() : "NULL");

      showCurrentMenu();
}



bool MenuManager::findMenuPath(MenuObject* root, MenuObject* target, int depth, int &newStackPointer, int &mainMenuIndex) {
    if (!root || depth >= MAX_SUBMENUS) return false;

    for (int i = 0; i < root->getSubMenuCount(); i++) {
        MenuObject* subMenu = root->getSubMenu(i);
        DEX_print("Checking submenu: "); DEX_println(subMenu->getName());

        // **If target menu found, update stack**
        if (subMenu == target) {
            D_println("Target menu found!");
            newStackPointer = depth + 1;
            mainMenuIndex = i;
            DEX_print("mainMenuIndex: "); DEX_println(mainMenuIndex);
            menuStack[depth] = root;  // Store parent in stack

            // **STOP SEARCH if target is a main menu**
            if (depth == 0) {
                D_println("Target is a MAIN MENU. Stopping search.");
                newStackPointer = 0; // is still a mainMenu
                return true;
            }

            return true;
        } else{
          currentMainMenuObject = i;
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
    
    DEX_print("currentMainMenuObject: "); DEX_println(currentMainMenuObject);
    DEX_print("submenucount: "); DEX_println(currentMenu->getSubMenuCount());
    DEX_print("menuStackPointer: "); DEX_println(menuStackPointer);
    DEX_print("currentIndex: "); DEX_println(currentIndex);
    DEX_print("currenMenu NAME: ");//DEX_println(currentMenu ? currentMenu->getName() : "NULL");
    if (currentMenu->getSubMenuCount() > 0 && currentIndex < currentMenu->getSubMenuCount()) {
        DEX_println(currentMenu ? currentMenu->getName() : "NULL");
    } else {
        DEX_println("No valid submenu!");
    }
    
    showCurrentMenu();    
}

void MenuManager::exitMenu() {
    if (menuStackPointer == 0){ // in mainMenu
      currentMainMenuObject = currentIndex; //go back to the mainMenu that last was selected
    }
    if (menuStackPointer > 0) { // still in a subMenu
      currentMenu = menuStack[--menuStackPointer];
      if (menuStackPointer == 0){ // in the mainMenu
        currentIndex = currentMainMenuObject; // get the recently use menuNR
        currentMainMenuObject = 0; // reset currentMainMenuObject to 0
      }
    }else if (currentIndex > 0 ){ // still in the last stored mainmenu
      currentIndex = 0; // reset currentIndex to 0 -> go to the first mainMenu
    }//else { // go te the default menu 
     // currentIndex = 0; // set mainMenu NR
     // int x = 1; // set subMenu NR
     // for (int i = 0; i < x; i++){
     //   menuStack[menuStackPointer++] = currentMenu; // apply it
     // }
     // currentMenu = currentMenu->getSubMenu(currentIndex); // apply it
    //} 
    
    
    DEX_print("currentMainMenuObject: "); DEX_println(currentMainMenuObject);
    DEX_print("submenucount: "); DEX_println(currentMenu->getSubMenuCount());
    DEX_print("menuStackPointer: "); DEX_println(menuStackPointer);
    DEX_print("currentIndex: "); DEX_println(currentIndex);
    DEX_print("currenMenu NAME: ");DEX_println(currentMenu ? currentMenu->getName() : "NULL");
    
    showCurrentMenu();
}

void MenuManager::nextItem() {
    currentIndex = (currentIndex + 1) % currentMenu->getSubMenuCount();
    if (menuStackPointer == 0){ // in mainMenu
      currentMainMenuObject = currentIndex; //go back to the mainMenu that last was selected
    }
    
    DEX_print("currentMainMenuObject: "); DEX_println(currentMainMenuObject);
    DEX_print("submenucount: "); DEX_println(currentMenu->getSubMenuCount());
    DEX_print("menuStackPointer: "); DEX_println(menuStackPointer);
    DEX_print("currentIndex: "); DEX_println(currentIndex);
    DEX_print("currenMenu NAME: ");DEX_println(currentMenu ? currentMenu->getName() : "NULL");
    
    showCurrentMenu();
}

void MenuManager::previousItem() {
    currentIndex = (currentIndex - 1 + currentMenu->getSubMenuCount()) % currentMenu->getSubMenuCount();
    if (menuStackPointer == 0){ // in mainMenu
      currentMainMenuObject = currentIndex; //go back to the mainMenu that last was selected
    }
    
    DEX_print("currentMainMenuObject: "); DEX_println(currentMainMenuObject);
    DEX_print("submenucount: "); DEX_println(currentMenu->getSubMenuCount());
    DEX_print("menuStackPointer: "); DEX_println(menuStackPointer);
    DEX_print("currentIndex: "); DEX_println(currentIndex);
    DEX_print("currenMenu NAME: ");DEX_println(currentMenu ? currentMenu->getName() : "NULL");
    
    showCurrentMenu();
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
