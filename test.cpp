#include "catch.hpp"
#include "FibHeap.hpp"
#include <iostream>

#define CATCH_CONFIG_MAIN

TEST_CASE( "Default constructor test" ) {
	FibHeap<int> testHeap;

	REQUIRE(testHeap.empty());
	REQUIRE(testHeap.size() == 0);
}

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
}

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

TEST_CASE( "Simple extract_top test" ) {
	FibHeap<int> testHeap;

	testHeap.insert(13);
	testHeap.insert(42);

	REQUIRE(!testHeap.empty());
	REQUIRE(testHeap.size() == 2);
	REQUIRE(testHeap.top() == 42);

	testHeap.extract_top();
	REQUIRE(!testHeap.empty());
	REQUIRE(testHeap.size() == 1);
	REQUIRE(testHeap.top() == 13);

	testHeap.extract_top();
	REQUIRE(testHeap.empty());
	REQUIRE(testHeap.size() == 0);
}

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

	for (int i = 6; i >= 0; --i) {
		REQUIRE(!nEmptyHeap.empty());
		REQUIRE(nEmptyHeap.size() == i+2);
		REQUIRE(nEmptyHeap.top() == 2*i+1);
		nEmptyHeap.extract_top();
	}

	REQUIRE(nEmptyHeap.top() == -13);
	REQUIRE(nEmptyHeap.size() == 1);
}

TEST_CASE( "Simple initializer list constructor test" ) {
	FibHeap<int> testHeap{1,3,9,5,0,8};
	REQUIRE(testHeap.top() == 9);
	REQUIRE(testHeap.size() == 6);
	testHeap.extract_top();
	REQUIRE(testHeap.top() == 8);
}

TEST_CASE( "Simple heap union test" ) {
	FibHeap<int> testHeap1{3,2,1,10,20};
	FibHeap<int> testHeap2{6,5,4,15,25};

	testHeap1.uniteWith(testHeap2);
	REQUIRE(testHeap1.top() == 25);
	testHeap1.extract_top();
	REQUIRE(testHeap1.top() == 20);
	testHeap1.extract_top();
	REQUIRE(testHeap1.top() == 15);
	testHeap1.extract_top();
	REQUIRE(testHeap1.top() == 10);
	REQUIRE(testHeap1.size() == 7);
	REQUIRE(testHeap2.empty());
}

TEST_CASE( "Simple delete value test" ) {
	FibHeap<int> testHeap;
	testHeap.insert(3);
	testHeap.insert(9);
	auto h6 = testHeap.insert(6);

	REQUIRE(testHeap.top() == 9);
	testHeap.delete_value(h6);
	testHeap.extract_top();
	REQUIRE(testHeap.top() == 3);
}

TEST_CASE( "Simple decrease key test" ) {
	FibHeap<int> testHeap;
	testHeap.insert(3);
	testHeap.insert(9);
	auto h6 = testHeap.insert(6);

	REQUIRE(testHeap.top() == 9);
	testHeap.decrease_key(h6, 1);
	testHeap.extract_top();
	REQUIRE(testHeap.top() == 3);
}

TEST_CASE( "Simple swap test" ) {
	FibHeap<int> testHeap1{10,20,30};
	FibHeap<int> testHeap2{8,9,10,15,20,25};
	testHeap1.swap(testHeap2);

	REQUIRE(testHeap2.size() == 3);
	REQUIRE(testHeap1.size() == 6);
	REQUIRE(testHeap1.top() == 25);
	REQUIRE(testHeap2.top() == 30);
}

/**
 * TODO
 * copy ctor test Extended - check deep copy and handlers
 * move ctor test Extended - check handler validity, check moved heap for emptiness
 * copy assignment operator test Extended - same as copy ctor
 * move assignment operator test Extended - same as move ctor
 * range ctor test Extended
 * initList ctor test (Extended)
 *
 * insert test Extended
 * unitewith test (Extended) - duplicate values, empty heap union, valid handlers
 * extract_top test (Extended)
 * delete_value test (Extended) - check handler exception
 * decrease_key test (Extended) - check exception
 * swap test (Extended)
 * */


