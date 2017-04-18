#include "catch.hpp"
#include "FibHeap.hpp"
#include <iostream>

#define CATCH_CONFIG_MAIN

TEST_CASE( "Basic default constructor test" ) {
	FibHeap<int> testHeap;

	REQUIRE(!testHeap.top());
	REQUIRE(testHeap.empty());
	REQUIRE(testHeap.size() == 0);
}

TEST_CASE( "Basic copy constructor test, empty heap" ) {
	FibHeap<int> emptyHeap;
	FibHeap<int> copiedHeap(emptyHeap);

	REQUIRE(!copiedHeap.top());
	REQUIRE(copiedHeap.empty());
	REQUIRE(copiedHeap.size() == 0);

	REQUIRE(copiedHeap.top() == emptyHeap.top());
	REQUIRE(copiedHeap.empty() == emptyHeap.empty());
	REQUIRE(copiedHeap.size() == emptyHeap.size());
}





