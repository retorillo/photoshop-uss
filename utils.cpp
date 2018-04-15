#include "utils.h"

// TODO: This function should be replaced by PathCchAppend
// Note that PathCombine is under MAX_PATH constraint
tstring combine(tstring a, tstring b) {
  tostringstream o;
  o << a;
  if (!regex_match(a, tregex(TEXT("(\\|/)$"))))
    o << "\\";
  o << b;
  return o.str();
}

tstring replace(tstring input, tregex r, function<tstring(tstring)> callback) {
  tostringstream o;
  const TCHAR * input_cstr = input.c_str();
  int l = tstrlen(input_cstr) * sizeof(TCHAR);
  tregex_token_iterator begin(input_cstr, input_cstr + l, r);
  tregex_token_iterator end;
  const TCHAR* last = input_cstr;
  const auto handle_skipped = [&last, &o](const TCHAR* mfirst) -> void {
     if (mfirst < last) return;
     int skiplen = mfirst - last;
     TCHAR* skippart = new TCHAR[skiplen + 1];
     memcpy(skippart, last, skiplen * sizeof(TCHAR));
     skippart[skiplen] = '\0';
     o << skippart;
     delete[] skippart;
  };
  for_each(begin, end, [&handle_skipped, &o, &callback, &last](tsub_match m) -> void {
    handle_skipped(m.first);
    o << callback(m.str());
    last = m.second;
  });
  handle_skipped(input_cstr + l);
  return o.str();
}

tstring env(tstring input) {
  tstring unwrap = regex_replace(input, tregex(TEXT("^%|%$")), TEXT(""));
  DWORD len = GetEnvironmentVariable(unwrap.c_str(), NULL, 0);
  if (!len && GetLastError() == ERROR_ENVVAR_NOT_FOUND)
    return input;
  disposer dispo;
  TCHAR* buf = new TCHAR[len + 1]();
  dispo.push([buf]() -> void { delete[] buf; });
  DWORD r = GetEnvironmentVariable(unwrap.c_str(), buf, len + 1);
  if (!r)
    throw runtime_error("failed to expand environment variable.");
  return tstring(buf);
}

bool is_directory(tstring path) {
  WIN32_FILE_ATTRIBUTE_DATA info;
  // Starting in Windows 10, version 1607, for the unicode version of this function (GetFileAttributesExW),
  // you can opt-in to remove the MAX_PATH character limitation without prepending "\\?\".
  // https://msdn.microsoft.com/en-us/library/windows/desktop/aa364946(v=vs.85).aspx
  if (!GetFileAttributesEx((LONGPATHPREFIX + path).c_str(), GET_FILEEX_INFO_LEVELS::GetFileExInfoStandard, &info))
    return false;
  return HAS_FLAG(info.dwFileAttributes, FILE_ATTRIBUTE_DIRECTORY);
}
