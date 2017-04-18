#include "catch.hpp"
#include "FibHeap.hpp"
#include <iostream>

#define CATCH_CONFIG_MAIN

TEST_CASE( "Default constructor test" ) {
	FibHeap<int> testHeap;

	REQUIRE(testHeap.empty());
	REQUIRE(testHeap.size() == 0);
}
/*
TEST_CASE( "Copy constructor test, empty heap" ) {
	FibHeap<int> emptyHeap;
	FibHeap<int> copiedHeap(emptyHeap);

	REQUIRE(copiedHeap.empty());
	REQUIRE(copiedHeap.size() == 0);

	REQUIRE(copiedHeap.empty() == emptyHeap.empty());
	REQUIRE(copiedHeap.size() == emptyHeap.size());
}

TEST_CASE( "Move constructor test, empty heap" ) {
	FibHeap<int> emptyHeap;
	FibHeap<int> movedHeap(std::move(emptyHeap));

	REQUIRE(movedHeap.empty());
	REQUIRE(movedHeap.size() == 0);

	REQUIRE(movedHeap.empty() == emptyHeap.empty());
	REQUIRE(movedHeap.size() == emptyHeap.size());
}

TEST_CASE( "Copy assignment, empty heap" ) {
	FibHeap<int> emptyHeap;
	FibHeap<int> copiedHeap = emptyHeap;

	REQUIRE(copiedHeap.empty());
	REQUIRE(copiedHeap.size() == 0);

	REQUIRE(copiedHeap.empty() == emptyHeap.empty());
	REQUIRE(copiedHeap.size() == emptyHeap.size());
}

TEST_CASE( "Move assignment, empty heap" ) {
	FibHeap<int> emptyHeap;
	FibHeap<int> movedHeap = emptyHeap; //nie std::move?

	REQUIRE(movedHeap.empty());
	REQUIRE(movedHeap.size() == 0);

	REQUIRE(movedHeap.empty() == emptyHeap.empty());
	REQUIRE(movedHeap.size() == emptyHeap.size());
}*/

TEST_CASE( "Simple insert test" ) {
	FibHeap<int> testHeap;

	testHeap.insert(5);
	REQUIRE(!testHeap.empty());
	REQUIRE(testHeap.size() == 1);
	REQUIRE(testHeap.top() == 5);

	int i = 10;
	testHeap.insert(i);
	REQUIRE(!testHeap.empty());
	REQUIRE(testHeap.size() == 2);
	REQUIRE(testHeap.top() == 10);

	testHeap.insert(8);
	REQUIRE(!testHeap.empty());
	REQUIRE(testHeap.size() == 3);
	REQUIRE(testHeap.top() == 10);

}
/*
TEST_CASE( "Simple range constructor" ) {
	std::vector<int> emptyVector;
	std::vector<int> nEmptyVector = {1,3,5,7,9,11,13, -13};

	FibHeap<int> emptyHeap(emptyVector.begin(), emptyVector.end());
	REQUIRE(!emptyHeap.top());
	REQUIRE(emptyHeap.empty());
	REQUIRE(emptyHeap.size() == 0);

	FibHeap<int> nEmptyHeap(nEmptyVector.begin(), nEmptyVector.end());
	REQUIRE(!nEmptyHeap.empty());
	REQUIRE(nEmptyHeap.size() == 8);
	REQUIRE(nEmptyHeap.top() == 13);
}*/

/*
 * TODO
 * copy ctor test Extended
 * move ctor test Extended
 * copy assignment operator test Extended
 * move assignment operator test Extended
 * range ctor test Extended
 * initList ctor test (Basic/Extended)
 *
 * unitewith test (Basic/Extended)
 * extract_top test (Basic/Extended)
 * delete_value test (Basic/Extended)
 * decrease_key test (Basic/Extended)
 * swap test (Basic/Extended)
 * */


