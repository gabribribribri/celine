#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

import celine;

TEST_CASE("testing the funniest number") {
  CHECK(TestNotBroken::the_funniest_number() == 67);
}
