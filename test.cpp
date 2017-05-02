#include "catch.hpp"
#include "FibHeap.hpp"
#include <iostream>

#define CATCH_CONFIG_MAIN

struct Copyable {

	Copyable() : value(0) {}
	Copyable(int v) : value(v) {}
	Copyable(const Copyable& o) : value(o.value) {}
	Copyable &operator= (const Copyable& o) {
		value = o.value;
		return *this;
	}
	Copyable(Copyable &&) = delete;
	Copyable &operator=(Copyable &&) = delete;

	bool operator> (const Copyable &o) const {
		return this->value > o.value;
	}
	bool operator< (const Copyable &o) const {
		return this->value < o.value;
	}
	bool operator>= (const Copyable &o) const {
		return this->value >= o.value;
	}
	bool operator<= (const Copyable &o) const {
		return this->value <= o.value;
	}
	bool operator== (const Copyable &o) const {
		return value == o.value;
	}
	bool operator!= (const Copyable &o) const {
		return !((*this)==o);
	}

private:
	int value;
};

struct Movable {
	Movable() : value(0) {}
	Movable(int v) : value(v) {}
	Movable(Movable&& o) : value(o.value) {
		o.value = 0;
	}
	Movable &operator= (Movable&& o) {
		value = o.value;
		o.value = 0;
		return *this;
	}
	Movable(const Movable&) = delete;
	Movable &operator=(const Movable&) = delete;

	bool operator> (const Movable &o) const {
		return value > o.value;
	}
	bool operator< (const Movable &o) const {
		return value < o.value;
	}
	bool operator>= (const Movable &o) const {
		return value >= o.value;
	}
	bool operator<= (const Movable &o) const {
		return value <= o.value;
	}
	bool operator== (const Movable &o) const {
		return value == o.value;
	}
	bool operator!= (const Movable &o) const  {
		return !((*this)==o);
	}
private:
	int value;
};
/*
struct CopyableMovable {
	CopyableMovable() : value(0) {}
	CopyableMovable(int v) : value(v) {}
	CopyableMovable(CopyableMovable&& o) : value(o.value) {
		o.value = 0;
	}
	CopyableMovable operator= (CopyableMovable&& o) {
		value = o.value;
		o.value = 0;
	}
	CopyableMovable(const CopyableMovable& o) : value(o.value) {}
	CopyableMovable operator=(const CopyableMovable& o) {
		value = o.value;
	}

	bool operator> (const CopyableMovable &o) const {
		return this->value > o.value;
	}
	bool operator< (const CopyableMovable &o) const {
		return this->value < o.value;
	}
	bool operator>= (const CopyableMovable &o) const {
		return this->value >= o.value;
	}
	bool operator<= (const CopyableMovable &o) const {
		return this->value <= o.value;
	}
	bool operator== (const CopyableMovable &o) const {
		return value == o.value;
	}
	bool operator!= (const CopyableMovable &o) const {
		return !((*this)==o);
	}
private:
	int value;
};
 */

template <typename Value>
bool CheckHeap(FibHeap<Value>& heap, std::vector<Value>& values) {
	std::sort(values.begin(), values.end(), std::greater<Value>()); //descending

	for (auto &value : values) {
		if (value != heap.top())
			return false;
		heap.extract_top();
	}
	return heap.empty();
}

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
	FibHeap<int> movedHeap = std::move(emptyHeap);

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
	testHeap.insert(5);

	REQUIRE(!testHeap.empty());
	REQUIRE(testHeap.size() == 3);
	REQUIRE(testHeap.top() == 42);

	testHeap.extract_top();
	REQUIRE(testHeap.size() == 2);
	REQUIRE(testHeap.top() == 13);

	testHeap.extract_top();
	REQUIRE(!testHeap.empty());
	REQUIRE(testHeap.size() == 1);
	REQUIRE(testHeap.top() == 5);

	testHeap.extract_top();
	REQUIRE(testHeap.empty());
	REQUIRE(testHeap.size() == 0);
}

TEST_CASE( "Simple range constructor" ) {
	std::vector<int> emptyVector;
	std::vector<int> nEmptyVector = {1,3,5,7,9,11,13, -13};

	FibHeap<int> emptyHeap(emptyVector.begin(), emptyVector.end());
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

TEST_CASE( "Simple increase key test" ) {
	FibHeap<int> testHeap;
	testHeap.insert(9);
	testHeap.insert(6);
	auto h3 = testHeap.insert(3);

	REQUIRE(testHeap.top() == 9);
	testHeap.increase_key(h3, 7);
	testHeap.extract_top();
	REQUIRE(testHeap.top() == 7);
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

// #################################################### EXTENDED TESTS ###################################### //

TEST_CASE( "Copy constructor test" ) {
	FibHeap<int> testHeap{0,5,10,15,20, 35};
	auto H25 = testHeap.insert(25);
	auto H30 = testHeap.insert(30);

	FibHeap<int> copyHeap(testHeap);

	SECTION("Equality check") {
		for (int i = 7; i >=0; --i) {
			REQUIRE(copyHeap.top() == testHeap.top());
			REQUIRE(copyHeap.size() == i+1);
			REQUIRE(testHeap.size() == i+1);
			copyHeap.extract_top();
			testHeap.extract_top();
		}
	}

	SECTION("Deleting value") {
		testHeap.delete_value(H25);
		testHeap.delete_value(H30);
		REQUIRE(copyHeap.top() == testHeap.top());
		REQUIRE(copyHeap.size() == 8);
		copyHeap.extract_top();
		REQUIRE(copyHeap.top() == 30);
	}

	SECTION("Increasing value") {
		testHeap.increase_key(H25, 125);
		testHeap.increase_key(H30, 130);
		REQUIRE(testHeap.size() == copyHeap.size());
		testHeap.extract_top();
		REQUIRE(copyHeap.top() == 35);
		copyHeap.extract_top();
		REQUIRE(copyHeap.top() == 30);
	}
}

TEST_CASE( "Move constructor test" ) {
	FibHeap<int> movedFrom;
	movedFrom.insert(20);
	movedFrom.insert(15);
	auto H10 = movedFrom.insert(10);
	FibHeap<int> movedTo(std::move(movedFrom));

	SECTION("Emptiness and correctness of move") {
		REQUIRE(movedFrom.empty());
		REQUIRE(movedTo.top() == 20);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 15);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 10);
		movedTo.extract_top();
		REQUIRE(movedTo.empty());
	}

	SECTION("Handler validity 1") {
		REQUIRE(movedTo.size() == 3);
		movedTo.increase_key(H10, 17);
		REQUIRE(movedTo.top() == 20);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 17);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 15);
		movedTo.extract_top();
		REQUIRE(movedTo.empty());
	}

	SECTION("Handler validity 1") {
		REQUIRE(movedTo.size() == 3);
		movedTo.delete_value(H10);
		REQUIRE(movedTo.size() == 2);
		REQUIRE(movedTo.top() == 20);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 15);
		movedTo.extract_top();
		REQUIRE(movedTo.empty());
	}
}

TEST_CASE( "Copy assignment operator test" ) {
	FibHeap<int> testHeap{0,5,10,15,20, 35};
	auto H25 = testHeap.insert(25);
	auto H30 = testHeap.insert(30);

	FibHeap<int> copyHeap{99,1001,-365};
	copyHeap = testHeap;
	REQUIRE(copyHeap.size() == 8);

	SECTION("Equality check") {
		for (int i = 7; i >=0; --i) {
			REQUIRE(copyHeap.top() == testHeap.top());
			REQUIRE(copyHeap.size() == i+1);
			REQUIRE(testHeap.size() == i+1);
			copyHeap.extract_top();
			testHeap.extract_top();
		}
	}

	SECTION("Deleting value") {
		testHeap.delete_value(H25);
		testHeap.delete_value(H30);
		REQUIRE(copyHeap.top() == testHeap.top());
		REQUIRE(copyHeap.size() == 8);
		copyHeap.extract_top();
		REQUIRE(copyHeap.top() == 30);
	}

	SECTION("Increasing value") {
		testHeap.increase_key(H25, 125);
		testHeap.increase_key(H30, 130);
		REQUIRE(testHeap.size() == copyHeap.size());
		testHeap.extract_top();
		REQUIRE(copyHeap.top() == 35);
		copyHeap.extract_top();
		REQUIRE(copyHeap.top() == 30);
	}
}

TEST_CASE( "Move assignment operator test" ) {
	FibHeap<int> movedFrom;
	movedFrom.insert(20);
	movedFrom.insert(15);
	auto H10 = movedFrom.insert(10);
	FibHeap<int> movedTo{88,-44,9,15,18,22};
	movedTo = std::move(movedFrom);



	SECTION("Emptiness and correctness of move") {
		REQUIRE(movedFrom.empty());
		REQUIRE(movedTo.top() == 20);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 15);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 10);
		movedTo.extract_top();
		REQUIRE(movedTo.empty());
	}

	SECTION("Handler validity 1") {
		REQUIRE(movedTo.size() == 3);
		movedTo.increase_key(H10, 17);
		REQUIRE(movedTo.top() == 20);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 17);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 15);
		movedTo.extract_top();
		REQUIRE(movedTo.empty());
	}

	SECTION("Handler validity 1") {
		REQUIRE(movedTo.size() == 3);
		movedTo.delete_value(H10);
		REQUIRE(movedTo.size() == 2);
		REQUIRE(movedTo.top() == 20);
		movedTo.extract_top();
		REQUIRE(movedTo.top() == 15);
		movedTo.extract_top();
		REQUIRE(movedTo.empty());
	}
}

TEST_CASE( "Insert test" ) {
	SECTION("Copyable") {
		std::vector<Copyable> copyables/*{32,32,16,8,4,4,4,2,1}*/;
		//copyables.push_back(32);
		std::vector<int> vector{32};

		for(int i : vector) {
			Copyable x(i);
			copyables.push_back(x);
		}

		FibHeap<Copyable> testHeap;

		for (unsigned i = 0; i < copyables.size(); ++i) {
			testHeap.insert(copyables.at(i));
			REQUIRE(copyables.at(i) != 0);
		}

		for (auto &value : copyables) {
			REQUIRE(value == testHeap.top());
			testHeap.extract_top();
		}
		REQUIRE(testHeap.empty());
	}
	SECTION("Movable") {
		std::vector<Movable> movables;
		movables.push_back(Movable(8));
		movables.push_back(Movable(16));
		movables.push_back(Movable(32));
		movables.push_back(Movable(2));
		movables.push_back(Movable(4));
		movables.push_back(Movable(16));
		movables.push_back(Movable(32));
		movables.push_back(Movable(1));
		movables.push_back(Movable(1));

		FibHeap<Movable> testHeap;

		for (unsigned i = 0; i < movables.size(); ++i) {
			testHeap.insert(std::move(movables.at(i)));
			REQUIRE(movables.at(i) == 0);
		}
	}
}

TEST_CASE( "Heap union test" ) {
	SECTION( "Union - empty heap(s)" ) {
		FibHeap<int> emptyHeap1;
		FibHeap<int> emptyHeap2;
		FibHeap<int> testHeap{2,10,8,4,6};
		std::vector<int> vector{2,10,8,4,6};

		SECTION("Empty-empty union") {
			emptyHeap1.uniteWith(emptyHeap2);
			REQUIRE(emptyHeap1.size() == 0);
			REQUIRE(emptyHeap2.size() == 0);
		}
		SECTION("Empty-nonempty union") {
			emptyHeap1.uniteWith(testHeap);
			REQUIRE(emptyHeap1.size() == 5);
			REQUIRE(testHeap.size() == 0);
			REQUIRE(CheckHeap(emptyHeap1,vector));
		}
		SECTION("Nonempty-empty union") {
			testHeap.uniteWith(emptyHeap2);
			REQUIRE(emptyHeap2.size() == 0);
			REQUIRE(testHeap.size() == 5);
			REQUIRE(CheckHeap(testHeap, vector));
		}
	}

	SECTION("Nonempty heaps union") {
		FibHeap<int> testHeap4{16,4,36,40,28,8,32,12,24,20};
		FibHeap<int> testHeap2{10,12,6,2,18,4,20,2,16,14};
		std::vector<int> vector{10,12,6,2,18,4,20,2,16,14};
		std::vector<int> unitedVector{10,12,6,2,18,4,20,2,16,14,
		                              16,4,36,40,28,8,32,12,24,20};

		SECTION("Self-union") {
			testHeap2.uniteWith(testHeap2);
			REQUIRE(testHeap2.size() == 10);
			REQUIRE(CheckHeap(testHeap2, vector));
		}
		SECTION("Duplicate values") {
			testHeap2.uniteWith(testHeap4);
			REQUIRE(testHeap4.size() == 0);
			REQUIRE(testHeap2.size() == 20);
			REQUIRE(CheckHeap(testHeap2, unitedVector));
		}
	}

	SECTION("Handlers check") {
		FibHeap<int> testHeap1{50,100};
		auto H75 = testHeap1.insert(75);
		auto H120 = testHeap1.insert(120);
		FibHeap<int> testHeap2{89,99};
		auto H109 = testHeap2.insert(109);

		SECTION("1 <- 2 with delete value") {
			testHeap1.uniteWith(testHeap2);
			REQUIRE(testHeap2.size() == 0);
			REQUIRE(testHeap1.size() == 7);
			REQUIRE(testHeap1.top() == 120);
			testHeap1.extract_top();
			REQUIRE(testHeap1.top() == 109);
			testHeap1.delete_value(H109);
			REQUIRE(testHeap1.top() == 100);

			std::vector<int> vector{50,100,75,89,99};
			REQUIRE(CheckHeap(testHeap1, vector));
		}

		SECTION("2 <- 1 with increase value") {
			testHeap2.uniteWith(testHeap1);
			REQUIRE(testHeap1.size() == 0);
			REQUIRE(testHeap2.size() == 7);
			REQUIRE(testHeap2.top() == 120);
			testHeap2.increase_key(H75, 500);
			REQUIRE(testHeap2.size() == 7);
			REQUIRE(testHeap2.top() == 500);

			std::vector<int> vector{50,100,500,120,89,99,109};
			REQUIRE(CheckHeap(testHeap2, vector));
		}
	}
}

TEST_CASE( "Delete value and increase key test" ) {
	FibHeap<int> testHeap{20,30,40};
	auto H50 = testHeap.insert(50);
	auto H10 = testHeap.insert(10);

    for (unsigned i = 6; i < 11; ++i) {
        testHeap.insert(10*i);
    }
    for (unsigned i = 6; i < 11; ++i) {
        testHeap.extract_top();
    }

	testHeap.delete_value(H50);
	REQUIRE(testHeap.top() == 40);
	REQUIRE_THROWS(testHeap.delete_value(H50));

	testHeap.increase_key(H10, 100);
	REQUIRE(testHeap.top() == 100);
	REQUIRE_THROWS(testHeap.increase_key(H50, 120));
	REQUIRE_THROWS(testHeap.increase_key(H10, 0));
}


