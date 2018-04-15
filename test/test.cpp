#include "test.h"

TEST_CLASS(Class1) {
public:
  TEST_METHOD(replace_singleline) {
    tstring input = TEXT("foo%bar%%baz%qux%quux%corge");
    vector<tstring> found;
    tstring output = replace(input, tregex(TEXT("%([^%]+)%")), [&found](const tstring& m) -> tstring {
      found.push_back(m); return TEXT("_");
    });
    Assert::AreEqual(static_cast<int>(found.size()), 3);
    Assert::AreEqual(found[0].c_str(), TEXT("%bar%"));
    Assert::AreEqual(found[1].c_str(), TEXT("%baz%"));
    Assert::AreEqual(found[2].c_str(), TEXT("%quux%"));
    Assert::AreEqual(output.c_str(), TEXT("foo__qux_corge"));
  }
  TEST_METHOD(replace_multiline) {
    tstring input = TEXT("foo:BAR\nBAR:qux\n\nBAR:corge");
    vector<tstring> found;
    tstring output = replace(input, tregex(TEXT("^BAR")), [&found](const tstring& m) -> tstring {
      found.push_back(m); return TEXT("QUX");
    });
    Assert::AreEqual(static_cast<int>(found.size()), 2);
    Assert::AreEqual(found[0].c_str(), TEXT("BAR"));
    Assert::AreEqual(found[1].c_str(), TEXT("BAR"));
    Assert::AreEqual(output.c_str(), TEXT("foo:BAR\nQUX:qux\n\nQUX:corge"));
  }
};
