#include "tm1637menuManager.h"

MenuManager::MenuManager(TM1637* display, MenuObject* rootMenu) {
    this->display = display;
    this->currentMenu = rootMenu;
    this->menuStackPointer = 0;
    this->currentIndex = 0;

    // Initialize stored indexes to 0
    for (int i = 0; i < 6; i++) {
        currentMenuObject[i] = 0;
        menuStack[i] = nullptr;
    }
}

MenuManager::~MenuManager() {
    // Only delete if MenuManager owns the menu objects
    // If menus are created outside, do NOT delete them here
    // delete currentMenu; // <- Removed to prevent deleting external/static objects
}

MenuObject* MenuManager::getCurrentMenu() {
    if (currentMenu && currentMenu->getSubMenuCount() > 0) {
        MenuObject* sub = currentMenu->getSubMenu(currentIndex);
        return sub ? sub : currentMenu; 
    }
    return currentMenu;
}

bool MenuManager::getCurrentMenuIsMain() {
    return (menuStackPointer == 0);
}

bool MenuManager::getTargetMenuIsMain(MenuObject* targetMenu) {
    if (!targetMenu) return false;

    MenuObject* root = (menuStackPointer > 0) ? menuStack[0] : currentMenu;
    if (!root) return false;

    // Check all submenus at root
    for (int i = 0; i < root->getSubMenuCount(); i++) {
        if (root->getSubMenu(i) == targetMenu) {
            D_println("Target is a main menu item.");
            return true;
        }
    }
    return false;
}

bool MenuManager::getCurrentMenuIsSub() {
    return (menuStackPointer > 0);
}

bool MenuManager::getCurrentMenuIsSubSub() {
    return (menuStackPointer > 1);
}

void MenuManager::jumpToMenu(MenuObject* targetMenu) {
    if (!targetMenu) return;

    if (getTargetMenuIsMain(targetMenu)) return;

    D_println("Jumping to menu...");

    int newStackPointer = 0;
    int mainMenuIndex = 0;

    MenuObject* rootMenu = (menuStackPointer > 0) ? menuStack[0] : currentMenu;
    if (!rootMenu) return;

    if (!findMenuPath(rootMenu, targetMenu, 0, newStackPointer, mainMenuIndex)) {
        D_println("Target menu not found in tree!");
        return;
    }

    if (newStackPointer >= MAX_SUBMENUS) {
        D_println("ERROR: Stack overflow detected, ABORTING!");
        return;
    }

    menuStackPointer = newStackPointer - 1;
    currentMenu = targetMenu;
    currentIndex = mainMenuIndex;

    D_println("Jump successful!");
    D_printf("  New Stack Pointer: %d\n", menuStackPointer);
    D_printf("  Main Menu Index: %d\n", mainMenuIndex);
    D_printf("  Current Menu: %s\n", currentMenu ? currentMenu->getName().c_str() : "NULL");

    showCurrentMenu();
}

int MenuManager::getCurrentIndex() {
    return currentMenuObject[menuStackPointer];
}

bool MenuManager::findMenuPath(MenuObject* root, MenuObject* target, int depth, int &newStackPointer, int &mainMenuIndex) {
    if (!root || depth >= MAX_SUBMENUS) return false;

    for (int i = 0; i < root->getSubMenuCount(); i++) {
        MenuObject* subMenu = root->getSubMenu(i);
        if (!subMenu) continue;

        DEX_printf("Checking submenu: %s\n", subMenu->getName().c_str());

        if (subMenu == target) {
            D_println("Target menu found!");
            newStackPointer = depth + 1;
            mainMenuIndex = i;
            menuStack[depth] = root;

            if (depth == 0) {
                D_println("Target is a MAIN MENU. Stopping search.");
                newStackPointer = 0;
            }
            return true;
        } else {
            currentMenuObject[0] = i;
        }

        if (findMenuPath(subMenu, target, depth + 1, newStackPointer, mainMenuIndex)) {
            menuStack[depth] = root;
            return true;
        }
    }
    return false;
}

void MenuManager::enterMenu() {
    if (!currentMenu) return;

    if (currentMenu->getSubMenuCount() > 0) {
        menuStack[menuStackPointer++] = currentMenu;
        MenuObject* next = currentMenu->getSubMenu(currentIndex);

        if (next) {
            currentMenu = next;
        }

        currentIndex = currentMenuObject[menuStackPointer];

        if (!currentMenu || currentMenu->getSubMenuCount() == 0) {
            currentMenu = menuStack[--menuStackPointer];
            currentIndex = currentMenuObject[menuStackPointer];
        }
    }

    printDebug();
    showCurrentMenu();
}

void MenuManager::exitMenu() {
    currentMenuObject[menuStackPointer] = currentIndex;

    if (menuStackPointer > 0) {
        currentMenu = menuStack[--menuStackPointer];
        currentIndex = currentMenuObject[menuStackPointer];
    } else if (currentIndex > 0) {
        currentIndex = 0;
        currentMenuObject[menuStackPointer] = currentIndex;
    }

    printDebug();
    showCurrentMenu();
}

void MenuManager::nextItem() {
    if (!currentMenu) return;
    int count = currentMenu->getSubMenuCount();
    if (count == 0) return;

    currentIndex = (currentIndex + 1) % count;
    currentMenuObject[menuStackPointer] = currentIndex;

    printDebug();
    showCurrentMenu();
}

void MenuManager::previousItem() {
    if (!currentMenu) return;
    int count = currentMenu->getSubMenuCount();
    if (count == 0) return;

    currentIndex = (currentIndex - 1 + count) % count;
    currentMenuObject[menuStackPointer] = currentIndex;

    printDebug();
    showCurrentMenu();
}

void MenuManager::printDebug() {
    DEX_printf("currentMenuObject[%d]: %d %s\n", menuStackPointer, currentMenuObject[menuStackPointer], menuStackPointer == 0 ? "-> MAIN" : "");
    DEX_printf("submenucount: %d\n", currentMenu ? currentMenu->getSubMenuCount() : -1);
    DEX_printf("menuStackPointer: %d\n", menuStackPointer);
    DEX_printf("currentIndex: %d\n", currentIndex);
    DEX_printf("currenMenu NAME: %s\n", currentMenu ? currentMenu->getName().c_str() : "NULL");
}

void MenuManager::showCurrentMenu(bool dmxDot) {
    if (!currentMenu) return;

    if (currentMenu->getSubMenuCount() > 0) {
        MenuObject* sub = currentMenu->getSubMenu(currentIndex);
        if (!sub) return;
        showText(sub->getName().c_str(), sub->getDot() + dmxDot);
    } else {
        showText(currentMenu->getName().c_str(), currentMenu->getDot() + dmxDot);
    }
}

void MenuManager::darkScreen(bool dmxDot) {
    showText("    ", dmxDot);
}

void MenuManager::showCurrentScriptMenu(bool dmxDot) {
    if (!currentMenu) return;
    showText(currentMenu->getName().c_str(), currentMenu->getDot() + dmxDot);
}

void MenuManager::showText(const char* text, int dot) {
    if (!display) return;
    display->setDisplayToString(text, dot);
}
