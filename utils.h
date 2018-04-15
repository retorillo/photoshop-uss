#pragma once

#include <regex>
#include <sstream>
using namespace std;

#include <windows.h>
#pragma comment(lib, "kernel32.lib")

#include "tfunctions.h"
#include "disposer.h"

#define HAS_FLAG(x, f) ((x & f) == f)
#define LONGPATHPREFIX TEXT("\\\\?\\")

tstring combine(tstring a, tstring b);
tstring replace(tstring input, tregex r, function<tstring(tstring)> callback);
tstring env(tstring input);
bool is_directory(tstring path);
