#include "tm1637menuObject.h"

MenuObject::MenuObject(String name, int dot) {
    this->name = name;
    this->subMenuCount = 0;
    this->dot = dot;
}

void MenuObject::addSubMenu(MenuObject* submenu) {
    if (subMenuCount < MAX_SUBMENUS) {
        subMenus[subMenuCount] = submenu;
        subMenuCount++;
    }
}

String MenuObject::getName() {
    return name;
}

int MenuObject::getDot() {
    return dot;
}

MenuObject* MenuObject::getSubMenu(int index) {
    if (index >= 0 && index < subMenuCount) {
        return subMenus[index];
    }
    return nullptr;
}

int MenuObject::getSubMenuCount() {
    return subMenuCount;
}
