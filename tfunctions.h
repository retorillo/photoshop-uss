#pragma once

#ifdef UNICODE
  #define tcout wcout
  #define tgetch _getch
  #define tcin wcin
  #define tifstream wifstream
  #define tofstream wofstream
  #define tstring wstring
  #define tstrlen wcslen
  #define tregex wregex
  #define tmatch wsmatch
  #define to_tstring to_wstring
  #define tostringstream wostringstream
  #define tsub_match wcsub_match
  #define tregex_token_iterator wcregex_token_iterator
#else
  #define tcout cout
  #define tgetch _getwch
  #define tcin cin
  #define tifstream ifstream
  #define tofstream ofstream
  #define tstring string
  #define tstrlen strlen
  #define tregex regex
  #define tmatch smatch
  #define to_tstring to_string
  #define tostringstream ostringstream
  #define tsub_match csub_match
  #define tregex_token_iterator cregex_token_iterator
#endif
