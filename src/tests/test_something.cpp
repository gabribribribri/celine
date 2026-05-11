#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

import celine;

TEST_CASE("testing something") {
  CHECK(LinearApp::something() == 42);
}
