#include <conio.h>
#include <iostream>
#include <fstream>
#include <exception>

#include "utils.h"

#define PATH_HEAD TEXT("%APPDATA%\\Adobe\\Adobe Photoshop CC {VER}\\Adobe Photoshop CC {VER} Settings")
#define PATH_LEAF TEXT("PSUserConfig.txt")
#define VER_PAT TEXT("\\{VER\\}")
#define USESYSTEMSTYLUS_1 TEXT("UseSystemStylus 1")
#define USESYSTEMSTYLUS_0 TEXT("UseSystemStylus 0")
#define VER_BEG 2014
#define VER_END 2018

enum TERMCOLORS {
  GRAY = 8,
  BLUE = 9,
  GREEN = 10,
  CYAN = 11,
  RED = 12,
  MAGENTA = 13,
  YELLOW = 14,
  WHITE = 15,
};
enum CONFIG_STATUS {
  FILE_NOT_FOUND,
  CONFIG_NOT_SET,
  CONFIG_0,
  CONFIG_1,
  CONFIG_INVALID,
  CONFIG_AMBIGUOUS,
};

int main();
CONFIG_STATUS get_config(tstring path);
void set_config(tstring path, bool value);
