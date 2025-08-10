
# TM1637 Menu Manager

**A flexible hierarchical menu system for Arduino or ESP32 projects with TM1637 4-digit displays.**

This library provides an easy way to build and navigate multi-level menus on a TM1637-based 7-segment display.  
It is ideal for projects where you need a structured menu system but only have limited display space.

---

## Features
- **Hierarchical menu navigation**  
  Supports main menus, submenus, and deeper nesting.
- **TM1637 display integration**  
  Optimized to work with [TM16xxLEDs and Buttons](https://github.com/maxint-rd/TM16xx) library.
  Optimized to work with [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) library
- **Menu navigation functions**  
  - `enterMenu()` → Go to selected submenu  
  - `exitMenu()` → Return to previous menu  
  - `nextItem()` / `previousItem()` → Cycle through menu items  
  - `jumpToMenu()` → Jump directly to any menu object
- **Dot indicator control** for visual feedback.
- **Stack-based menu tracking** for fast back/forward navigation.
- **Null-safe & crash-resistant** with pointer and bounds checking.
- Debug logging with optional `DEBUG` and `DEBUG_EX` flags.

---

## Installation
1. Download or clone this repository into your Arduino `libraries` folder.
2. Install the required dependency:  
   - [TM16xxLEDs and Buttons library](https://github.com/maxint-rd/TM16xx)
   - [Adafruit GFX Library library](https://github.com/adafruit/Adafruit-GFX-Library)
3. Restart the Arduino IDE.

---

| Method                               | Description                       |
| ------------------------------------ | --------------------------------- |
| `enterMenu()`                        | Enter currently selected submenu  |
| `exitMenu()`                         | Go back to previous menu          |
| `nextItem()`                         | Select next menu item             |
| `previousItem()`                     | Select previous menu item         |
| `jumpToMenu(MenuObject* target)`     | Jump directly to target menu      |
| `showCurrentMenu(bool dmxDot=false)` | Display current menu/submenu name |
| `darkScreen(bool dmxDot=false)`      | Blank the display                 |
| `getCurrentMenu()`                   | Get current `MenuObject*`         |
| `getCurrentMenuIsMain()`             | Check if currently in main menu   |
| `getCurrentMenuIsSub()`              | Check if in a submenu             |
| `getCurrentMenuIsSubSub()`           | Check if in a sub-submenu         |

---

## Notes
* The library uses a stack to track navigation depth.
* All menu names are shown on the TM1637 display (first 4 chars).
* MenuObject can hold up to MAX_SUBMENUS items (default: 16).
* Debug logging can be enabled by setting DEBUG or DEBUG_EX in tm1637menuManager.h.
 
