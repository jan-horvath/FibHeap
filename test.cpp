#include "catch.hpp"
#include <iostream>

#define CATCH_CONFIG_MAIN

TEST_CASE( "testing testing" ) {
  int i = 1;
  i = i++;
  REQUIRE(i == 1);
}

