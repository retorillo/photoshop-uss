#include "photoshop-uss.h"

int main() {
  try {
    tstring dir_template = replace(tstring(PATH_HEAD), tregex(TEXT("%[^%]+%")),
      [](tstring m) -> tstring { return env(m); });
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(console, TERMCOLORS::GREEN);
    tcout << TEXT("  **********************************************") << endl
          << TEXT("  *   Photoshop UseSystemStylus Configurator   *") << endl
          << TEXT("  *       (C) 2018 Retorillo / MIT License     *") << endl
          << TEXT("  **********************************************") << endl;

  SCREEN_START:
    vector<tstring> touchable_files;
    SetConsoleTextAttribute(console, TERMCOLORS::WHITE);
    tcout << endl << TEXT("  The following conditions are detected:") << endl << endl;
    for (int ver = VER_BEG; ver <= VER_END; ver++) {
      tstring dir = regex_replace(dir_template, tregex(VER_PAT), to_tstring(ver));
      bool found = is_directory(dir);
      tstring status;
      int color_status;
      if (found) {
        tstring fullname = combine(dir, PATH_LEAF);
        touchable_files.push_back(fullname);
        switch(get_config(fullname)) {
          case CONFIG_STATUS::CONFIG_1:
            status = TEXT("UseSystemStylus 1");
            color_status = TERMCOLORS::GREEN;
            break;
          case CONFIG_STATUS::CONFIG_0:
            status = TEXT("UseSystemStylus 0");
            color_status = TERMCOLORS::YELLOW;
            break;
          case CONFIG_STATUS::CONFIG_INVALID:
            status = TEXT("UseSystemStylus is invalid format");
            color_status = TERMCOLORS::RED;
            break;
          case CONFIG_STATUS::CONFIG_AMBIGUOUS:
            status = TEXT("UseSystemStylus is specified multiple times");
            color_status = TERMCOLORS::RED;
            break;
          case CONFIG_STATUS::CONFIG_NOT_SET:
            status = TEXT("UseSystemStylus is not set");
            color_status = TERMCOLORS::GRAY;
            break;
          case CONFIG_STATUS::FILE_NOT_FOUND:
            status = TEXT("PSUserConfig.txt is not found");
            color_status = TERMCOLORS::GRAY;
            break;
          default: status = TEXT("?");
        }
      }
      else {
        status = TEXT("Directory Not Found");
        color_status = 8;
      }
      SetConsoleTextAttribute(console, color_status);
      tcout << TEXT("  Photoshop CC ") << ver << TEXT(" : ") << status << endl;
      SetConsoleTextAttribute(console, 15);
    }
  SCREEN_RETYPE:
    tcout << endl
          << TEXT("  [0] Configure to UseSystemStylus 0") << endl
          << TEXT("  [1] Configure to UseSystemStylus 1") << endl
          << TEXT("  [4] Erase PSUserConfig.txt") << endl
          << TEXT("  [8] Restart configurator") << endl
          << TEXT("  [9] Quit") << endl
          << TEXT("  Enter action code [0-9]: ");
    auto action = tgetch() - TEXT('0');
    tcout << action << endl << endl;
    switch(action) {
      case 0: // fallthrough
      case 1:
        for (tstring f : touchable_files)
          set_config(f, action);
        SetConsoleTextAttribute(console, TERMCOLORS::CYAN);
        tcout << TEXT("     +++++++++++ COMPLETED ++++++++++") << endl
              << TEXT("     +       RESTART PHOTOSHOP      +") << endl
              << TEXT("     +     TO APPLY THIS CHANGES    +") << endl
              << TEXT("     ++++++++++++++++++++++++++++++++") << endl;
        SetConsoleTextAttribute(console, TERMCOLORS::WHITE);
        goto SCREEN_START;
      case 4:
        SetConsoleTextAttribute(console, TERMCOLORS::RED);
        tcout << TEXT("     @@@@@@@@@@@ WARNING @@@@@@@@@@@@") << endl
              << TEXT("     @ THIS ACTION CANNOT BE UNDONE @") << endl
              << TEXT("     @  SOME SETTINGS MAY BE LOST!  @") << endl
              << TEXT("     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@") << endl
              << TEXT("       PROCEED THIS ACTION? (Y/N): ");
        SetConsoleTextAttribute(console, TERMCOLORS::WHITE);
        action = tgetch();
        tcout << static_cast<TCHAR>(action) << endl;
        if (action == TEXT('y')) {
          for (tstring f : touchable_files)
            DeleteFile(f.c_str());
        }
        goto SCREEN_START;
      case 8:
        goto SCREEN_START;
      case 9:
        return 0; // quit
      default:
        goto SCREEN_RETYPE;
    }
  }
  catch(const runtime_error& e) {
    cout << e.what() << endl;
    return -1;
  }
}

CONFIG_STATUS get_config(tstring path) {
  tifstream f(path);
  if (!f.is_open())
    return CONFIG_STATUS::FILE_NOT_FOUND;
  tostringstream fbuf;
  fbuf << f.rdbuf();
  wstring fread = fbuf.str();
  int detect = 0;
  replace(fread, tregex(TEXT("^\\s*UseSystemStylus.*$")), [&detect](tstring s) -> tstring {
    if (detect) {
      detect = CONFIG_STATUS::CONFIG_AMBIGUOUS;
      return s;
    }
    tmatch m;
    if (!regex_match(s, m, tregex(TEXT("^(\\s*)UseSystemStylus\\s+(0|1)\\s*$"))))
      detect = CONFIG_STATUS::CONFIG_NOT_SET;
    if (m.size() == 3) {
      if (m[1].length() > 0) // Photochop CC cannot trim leading white-space (tested in 2017)
        detect = CONFIG_STATUS::CONFIG_INVALID;
      else
        detect = m[2] == TEXT("1") ? CONFIG_STATUS::CONFIG_1 : CONFIG_STATUS::CONFIG_0;
    }
    else
      detect = CONFIG_STATUS::CONFIG_INVALID;
    return s;
  });
  return static_cast<CONFIG_STATUS>(detect);
}

void set_config(tstring path, bool value) {
  tifstream f(path);
  tstring fread;
  if (f.is_open()) {
    tostringstream fbuf;
    fbuf << f.rdbuf();
    fread = fbuf.str();
  }
  else
    fread = TEXT("");
  bool set = false;
  fread = replace(fread, tregex(TEXT("^\\s*UseSystemStylus.*$")), [&set, value](tstring str) -> tstring {
    if (set) return TEXT("");
    set = true;
    return value ? USESYSTEMSTYLUS_1 : USESYSTEMSTYLUS_0;
  });
  tofstream o(path);
  if (!o.is_open())
    throw runtime_error("Failed to open config file to write.");
  if (!set)
    o << (value ? USESYSTEMSTYLUS_1 : USESYSTEMSTYLUS_0) << fread << endl;
  else
    o << fread;
}
