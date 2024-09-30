#ifndef MENUOBJECT_H
#define MENUOBJECT_H

#include <Arduino.h>

#define MAX_SUBMENUS 14 // Maximaal aantal sub-menu's per menu

class MenuObject {
public:
    String name;
    int dot;
    MenuObject* subMenus[MAX_SUBMENUS];
    int subMenuCount;

    MenuObject(String name, int dot = 0); // _BV(4) = no visable dot (0-3)

    void addSubMenu(MenuObject* submenu);
    String getName();
    int getDot();
    MenuObject* getSubMenu(int index);
    int getSubMenuCount();
};

#endif
