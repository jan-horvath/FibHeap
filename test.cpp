#include "FibHeap.hpp"
#include "catch.hpp"
#include <iostream>

#define CATCH_CONFIG_MAIN

struct Copyable {
  Copyable() : value(0) {}
  Copyable(int v) : value(v) {}
  Copyable(const Copyable &o) : value(o.value) {}
  Copyable &operator=(const Copyable &o) {
    value = o.value;
    return *this;
  }
  Copyable(Copyable &&) = delete;
  Copyable &operator=(Copyable &&) = delete;

  bool operator>(const Copyable &o) const { return this->value > o.value; }
  bool operator<(const Copyable &o) const { return this->value < o.value; }
  bool operator>=(const Copyable &o) const { return this->value >= o.value; }
  bool operator<=(const Copyable &o) const { return this->value <= o.value; }
  bool operator==(const Copyable &o) const { return value == o.value; }
  bool operator!=(const Copyable &o) const { return !((*this) == o); }

  int getValue() const { return value; }

private:
  int value;
};

struct Movable {
  Movable() : value(0) {}
  Movable(int v) : value(v) {}
  Movable(Movable &&o) noexcept : value(o.value) { o.value = 0; }
  Movable &operator=(Movable &&o) noexcept {
    value = o.value;
    o.value = 0;
    return *this;
  }
  Movable(const Movable &) = delete;
  Movable &operator=(const Movable &) = delete;

  bool operator>(const Movable &o) const { return value > o.value; }
  bool operator<(const Movable &o) const { return value < o.value; }
  bool operator>=(const Movable &o) const { return value >= o.value; }
  bool operator<=(const Movable &o) const { return value <= o.value; }
  bool operator==(const Movable &o) const { return value == o.value; }
  bool operator!=(const Movable &o) const { return !((*this) == o); }

  int getValue() const { return value; }

private:
  int value;
};

struct X {
  static std::vector<std::uintptr_t> addresses;

  X() : value(0) {
    X::addresses.push_back(reinterpret_cast<std::uintptr_t>(this)); // NOLINT
    // std::cout << "X constructed with value " << value << "\n";
  }

  X(const X &other) : value(other.value) {
    X::addresses.push_back(reinterpret_cast<std::uintptr_t>(this)); // NOLINT
    // std::cout << "X copy constructed with value " << value << "\n";
  }
  X(X &&other) noexcept : value(other.value) {
    X::addresses.push_back(reinterpret_cast<std::uintptr_t>(this)); // NOLINT
    // std::cout << "X move constructed with value " << value << "\n";
  }

  X &operator=(const X &other) {
    this->value = other.value;
    return *this;
  }

  X(int i) : value(i) {
    X::addresses.push_back(reinterpret_cast<std::uintptr_t>(this)); // NOLINT
    // std::cout << "X constructed with value " << value << "\n";
  }
  ~X() {
    // std::cout << "X destroyed with value " << value << "\n";
    REQUIRE(std::find(addresses.begin(), addresses.end(),
                      reinterpret_cast<std::uintptr_t>(this)) !=
            addresses.end()); // NOLINT
    addresses.erase(std::remove(addresses.begin(), addresses.end(),
                                reinterpret_cast<std::uintptr_t>(this)),
                    addresses.end()); // NOLINT
  }

  bool operator<(const X &other) const { return this->value > other.value; }
  bool operator>(const X &other) const { return this->value < other.value; }
  bool operator==(const X &other) const { return this->value == other.value; }
  bool operator<=(const X &other) const { return this->value <= other.value; }
  bool operator>=(const X &other) const { return this->value >= other.value; }

  int value;
};

std::vector<std::uintptr_t> X::addresses;

struct cmpX {
  bool operator()(const X &first, const X &second) const {
    return first < second;
  }
};

template <typename Value>
bool CheckHeap(FibHeap<Value> &heap, std::vector<Value> &values) {
  std::sort(values.begin(), values.end(), std::greater<Value>()); // descending

  for (auto &value : values) {
    if (value != heap.top())
      return false;
    heap.extract_top();
  }
  return heap.empty();
}

TEST_CASE("Default constructor test") { // NOLINT
  FibHeap<int> testHeap;

  REQUIRE(testHeap.empty());
  REQUIRE(testHeap.size() == 0); // NOLINT
}

TEST_CASE("Copy constructor test, empty heap") { // NOLINT
  FibHeap<int> emptyHeap;
  FibHeap<int> copiedHeap(emptyHeap); // NOLINT

  REQUIRE(copiedHeap.empty());
  REQUIRE(copiedHeap.size() == 0); // NOLINT

  REQUIRE(copiedHeap.empty() == emptyHeap.empty());
  REQUIRE(copiedHeap.size() == emptyHeap.size());
}

TEST_CASE("Move constructor test, empty heap") { // NOLINT
  FibHeap<int> emptyHeap;
  FibHeap<int> movedHeap(std::move(emptyHeap));

  REQUIRE(movedHeap.empty());
  REQUIRE(movedHeap.size() == 0); // NOLINT

  REQUIRE(movedHeap.empty() == emptyHeap.empty());
  REQUIRE(movedHeap.size() == emptyHeap.size());
}

TEST_CASE("Copy assignment, empty heap") { // NOLINT
  FibHeap<int> emptyHeap;
  FibHeap<int> copiedHeap = emptyHeap; // NOLINT

  REQUIRE(copiedHeap.empty());
  REQUIRE(copiedHeap.size() == 0); // NOLINT

  REQUIRE(copiedHeap.empty() == emptyHeap.empty());
  REQUIRE(copiedHeap.size() == emptyHeap.size());
}

TEST_CASE("Move assignment, empty heap") { // NOLINT
  FibHeap<int> emptyHeap;
  FibHeap<int> movedHeap = std::move(emptyHeap);

  REQUIRE(movedHeap.empty());
  REQUIRE(movedHeap.size() == 0); // NOLINT

  REQUIRE(movedHeap.empty() == emptyHeap.empty());
  REQUIRE(movedHeap.size() == emptyHeap.size());
}

TEST_CASE("Simple insert test") { // NOLINT
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

TEST_CASE("Simple extract_top test") { // NOLINT
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
  REQUIRE(testHeap.size() == 0); // NOLINT
}

TEST_CASE("Simple range constructor") { // NOLINT
  std::vector<int> emptyVector;
  std::vector<int> nEmptyVector = {1, 3, 5, 7, 9, 11, 13, -13};

  FibHeap<int> emptyHeap(emptyVector.begin(), emptyVector.end());
  REQUIRE(emptyHeap.empty());
  REQUIRE(emptyHeap.size() == 0); // NOLINT

  FibHeap<int> nEmptyHeap(nEmptyVector.begin(), nEmptyVector.end());
  REQUIRE(!nEmptyHeap.empty());
  REQUIRE(nEmptyHeap.size() == 8);
  REQUIRE(nEmptyHeap.top() == 13);

  for (int i = 6; i >= 0; --i) {
    REQUIRE(!nEmptyHeap.empty());
    REQUIRE(nEmptyHeap.size() == i + 2);
    REQUIRE(nEmptyHeap.top() == 2 * i + 1);
    nEmptyHeap.extract_top();
  }

  REQUIRE(nEmptyHeap.top() == -13);
  REQUIRE(nEmptyHeap.size() == 1);
}

TEST_CASE("Simple initializer list constructor test") { // NOLINT
  FibHeap<int> testHeap{1, 3, 9, 5, 0, 8};
  REQUIRE(testHeap.top() == 9);
  REQUIRE(testHeap.size() == 6);
  testHeap.extract_top();
  REQUIRE(testHeap.top() == 8);
}

TEST_CASE("Simple heap union test") { // NOLINT
  FibHeap<int> testHeap1{3, 2, 1, 10, 20};
  FibHeap<int> testHeap2{6, 5, 4, 15, 25};

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

TEST_CASE("Simple delete value test") { // NOLINT
  FibHeap<int> testHeap;
  testHeap.insert(3);
  testHeap.insert(9);
  auto h6 = testHeap.insert(6);

  REQUIRE(testHeap.top() == 9);
  testHeap.delete_value(h6);
  testHeap.extract_top();
  REQUIRE(testHeap.top() == 3);
}

TEST_CASE("Simple increase key test") { // NOLINT
  FibHeap<int> testHeap;
  testHeap.insert(9);
  testHeap.insert(6);
  auto h3 = testHeap.insert(3);

  REQUIRE(testHeap.top() == 9);
  testHeap.increase_key(h3, 7);
  testHeap.extract_top();
  REQUIRE(testHeap.top() == 7);
}

TEST_CASE("Simple swap test") { // NOLINT
  FibHeap<int> testHeap1{10, 20, 30};
  FibHeap<int> testHeap2{8, 9, 10, 15, 20, 25};
  testHeap1.swap(testHeap2);

  REQUIRE(testHeap2.size() == 3);
  REQUIRE(testHeap1.size() == 6);
  REQUIRE(testHeap1.top() == 25);
  REQUIRE(testHeap2.top() == 30);
}

// #################################################### EXTENDED TESTS
// ###################################### //

TEST_CASE("Copy constructor test") { // NOLINT
  FibHeap<int> testHeap{0, 5, 10, 15, 20, 35};
  auto H25 = testHeap.insert(25);
  auto H30 = testHeap.insert(30);

  FibHeap<int> copyHeap(testHeap);

  SECTION("Equality check") {
    for (int i = 7; i >= 0; --i) {
      REQUIRE(copyHeap.top() == testHeap.top());
      REQUIRE(copyHeap.size() == i + 1);
      REQUIRE(testHeap.size() == i + 1);
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

TEST_CASE("Move constructor test") { // NOLINT
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

TEST_CASE("Copy assignment operator test") { // NOLINT
  FibHeap<int> testHeap{0, 5, 10, 15, 20, 35};
  auto H25 = testHeap.insert(25);
  auto H30 = testHeap.insert(30);

  FibHeap<int> copyHeap{99, 1001, -365};
  copyHeap = testHeap;
  REQUIRE(copyHeap.size() == 8);

  SECTION("Equality check") {
    for (int i = 7; i >= 0; --i) {
      REQUIRE(copyHeap.top() == testHeap.top());
      REQUIRE(copyHeap.size() == i + 1);
      REQUIRE(testHeap.size() == i + 1);
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

TEST_CASE("Move assignment operator test") { // NOLINT
  FibHeap<int> movedFrom;
  movedFrom.insert(20);
  movedFrom.insert(15);
  auto H10 = movedFrom.insert(10);
  FibHeap<int> movedTo{88, -44, 9, 15, 18, 22};
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

TEST_CASE("Insert test") { // NOLINT
  SECTION("Copyable") {
    std::vector<Copyable> copyables, copyables2;
    std::vector<int> vector{50, 20, 25, 10, 30, 5, 35, 15, 45, 40};
    std::vector<int> vector2{50, 45, 40, 35, 30, 25, 20, 15, 10, 5};

    for (int i : vector) {
      Copyable x(i);
      copyables.push_back(x);
    }

    for (int i : vector2) {
      Copyable x(i);
      copyables2.push_back(x);
    }

    FibHeap<Copyable> testHeap;

    for (const auto &c : copyables) {
      testHeap.insert(c);
      REQUIRE(c.getValue() != 0);
    }

    for (auto &value : copyables2) {
      REQUIRE(value.getValue() == testHeap.top().getValue());
      testHeap.extract_top();
    }
    REQUIRE(testHeap.empty());
  }
  SECTION("Movable") {
    std::vector<Movable> movables;
    movables.emplace_back(Movable(8));
    movables.emplace_back(Movable(16));
    movables.emplace_back(Movable(32));
    movables.emplace_back(Movable(2));
    movables.emplace_back(Movable(4));
    movables.emplace_back(Movable(16));
    movables.emplace_back(Movable(32));
    movables.emplace_back(Movable(1));
    movables.emplace_back(Movable(1));

    FibHeap<Movable> testHeap;

    for (auto &m : movables) {
      testHeap.insert(std::move(m));
      REQUIRE(m.getValue() == 0);
    }
  }
}

TEST_CASE("Heap union test") { // NOLINT
  SECTION("Union - empty heap(s)") {
    FibHeap<int> emptyHeap1;
    FibHeap<int> emptyHeap2;
    FibHeap<int> testHeap{2, 10, 8, 4, 6};
    std::vector<int> vector{2, 10, 8, 4, 6};

    SECTION("Empty-empty union") {
      emptyHeap1.uniteWith(emptyHeap2);
      REQUIRE(emptyHeap1.empty());
      REQUIRE(emptyHeap2.empty());
    }
    SECTION("Empty-nonempty union") {
      emptyHeap1.uniteWith(testHeap);
      REQUIRE(emptyHeap1.size() == 5);
      REQUIRE(testHeap.empty());
      REQUIRE(CheckHeap(emptyHeap1, vector));
    }
    SECTION("Nonempty-empty union") {
      testHeap.uniteWith(emptyHeap2);
      REQUIRE(emptyHeap2.empty());
      REQUIRE(testHeap.size() == 5);
      REQUIRE(CheckHeap(testHeap, vector));
    }
  }

  SECTION("Nonempty heaps union") {
    FibHeap<int> testHeap4{16, 4, 36, 40, 28, 8, 32, 12, 24, 20};
    FibHeap<int> testHeap2{10, 12, 6, 2, 18, 4, 20, 2, 16, 14};
    std::vector<int> vector{10, 12, 6, 2, 18, 4, 20, 2, 16, 14};
    std::vector<int> unitedVector{10, 12, 6,  2,  18, 4, 20, 2,  16, 14,
                                  16, 4,  36, 40, 28, 8, 32, 12, 24, 20};

    SECTION("Self-union") {
      testHeap2.uniteWith(testHeap2);
      REQUIRE(testHeap2.size() == 10);
      REQUIRE(CheckHeap(testHeap2, vector));
    }
    SECTION("Duplicate values") {
      testHeap2.uniteWith(testHeap4);
      REQUIRE(testHeap4.empty());
      REQUIRE(testHeap2.size() == 20);
      REQUIRE(CheckHeap(testHeap2, unitedVector));
    }
  }

  SECTION("Handlers check") {
    FibHeap<int> testHeap1{50, 100};
    auto H75 = testHeap1.insert(75);
    testHeap1.insert(120);
    FibHeap<int> testHeap2{89, 99};
    auto H109 = testHeap2.insert(109);

    SECTION("1 <- 2 with delete value") {
      testHeap1.uniteWith(testHeap2);
      REQUIRE(testHeap2.empty());
      REQUIRE(testHeap1.size() == 7);
      REQUIRE(testHeap1.top() == 120);
      testHeap1.extract_top();
      REQUIRE(testHeap1.top() == 109);
      testHeap1.delete_value(H109);
      REQUIRE(testHeap1.top() == 100);

      std::vector<int> vector{50, 100, 75, 89, 99};
      REQUIRE(CheckHeap(testHeap1, vector));
    }

    SECTION("2 <- 1 with increase value") {
      testHeap2.uniteWith(testHeap1);
      REQUIRE(testHeap1.empty());
      REQUIRE(testHeap2.size() == 7);
      REQUIRE(testHeap2.top() == 120);
      testHeap2.increase_key(H75, 500);
      REQUIRE(testHeap2.size() == 7);
      REQUIRE(testHeap2.top() == 500);

      std::vector<int> vector{50, 100, 500, 120, 89, 99, 109};
      REQUIRE(CheckHeap(testHeap2, vector));
    }
  }
}

TEST_CASE("Delete value and increase key test") { // NOLINT
  FibHeap<int> testHeap{20, 30, 40};
  auto H50 = testHeap.insert(50);
  auto H10 = testHeap.insert(10);

  for (unsigned i = 6; i < 11; ++i) {
    testHeap.insert(10 * i);
  }
  for (unsigned i = 6; i < 11; ++i) {
    testHeap.extract_top();
  }

  testHeap.delete_value(H50);
  REQUIRE(testHeap.top() == 40);
  REQUIRE_THROWS(testHeap.delete_value(H50)); // still reachable 16 bytes

  testHeap.increase_key(H10, 100);
  REQUIRE(testHeap.top() == 100);
  REQUIRE_THROWS(testHeap.increase_key(H50, 120));
  REQUIRE_THROWS(testHeap.increase_key(H10, 0));
}

TEST_CASE("Delete value test") { // NOLINT
  FibHeap<int> testHeap{20, 30, 40};
  auto H50 = testHeap.insert(50);
  auto H10 = testHeap.insert(10);
  for (unsigned i = 6; i <= 10; ++i) {
    testHeap.insert(10 * i);
  }

  testHeap.delete_value(H10);
  testHeap.delete_value(H50);
  std::vector<int> vec = {20, 30, 40, 60, 70, 80, 90, 100};
  REQUIRE(CheckHeap(testHeap, vec));
}

TEST_CASE("Work with handlers (delete value)") { // NOLINT
  std::vector<FibHeap<int, std::greater<int>>::Handler> handlers;
  FibHeap<int, std::greater<int>> fibHeap;
  const size_t HEAP_SIZE = 6;
  const size_t AFTER_DEL = HEAP_SIZE / 2;

  for (unsigned i = 0; i < HEAP_SIZE; ++i) {
    handlers.push_back(fibHeap.insert(10 * i));
  }
  REQUIRE(fibHeap.size() == HEAP_SIZE);
  REQUIRE(fibHeap.top() == 0);

  for (unsigned i = 0; i < HEAP_SIZE; i += 2) {
    auto handler = std::move(handlers.at(i));
    fibHeap.delete_value(handler);
  }

  REQUIRE(fibHeap.size() == AFTER_DEL);
  REQUIRE(fibHeap.top() == 10);
  for (unsigned i = 0; i < AFTER_DEL; ++i) {
    REQUIRE(!fibHeap.empty());
    REQUIRE(fibHeap.top() == 20 * i + 10);
    fibHeap.extract_top();
  }
  REQUIRE(fibHeap.empty());
}

TEST_CASE("Work with handlers (decrease key)") { // NOLINT
  std::vector<FibHeap<int, std::greater<int>>::Handler> handlers;
  FibHeap<int, std::greater<int>> fibHeap;
  const int HEAP_SIZE = 10000;

  for (unsigned i = 1; i <= HEAP_SIZE; ++i) {
    handlers.push_back(fibHeap.insert(10 * i));
  }
  REQUIRE(fibHeap.size() == HEAP_SIZE);
  REQUIRE(fibHeap.top() == 10);

  for (unsigned j = 0; j < 10; ++j) {
    for (const auto &handler : handlers) {
      fibHeap.increase_key(handler, handler.value() - 1);
    }
  }

  REQUIRE(fibHeap.size() == HEAP_SIZE);
  REQUIRE(fibHeap.top() == 0);
  for (unsigned i = 0; i < HEAP_SIZE; ++i) {
    REQUIRE(!fibHeap.empty());
    REQUIRE(fibHeap.top() == 10 * i);
    fibHeap.extract_top();
  }
  REQUIRE(fibHeap.empty());
}

TEST_CASE("Test") { // NOLINT
  std::vector<int> vector = {1, 3, 5, 7, 9, 11, 13, -13};
  FibHeap<X, cmpX> testHeap1{10, 20, 30, 40, 50, 45, 35, 25, 15, 5};
  FibHeap<X, cmpX> testHeap3 = testHeap1;
  FibHeap<X, cmpX> testHeap4;
  FibHeap<X, cmpX> testHeap6(vector.begin(), vector.end());
  for (unsigned i = 0; i < 10; ++i) {
    testHeap4.insert(-4 * i);
  }
  FibHeap<X, cmpX> testHeap5(testHeap4);
  testHeap4 = std::move(testHeap1);
  REQUIRE(testHeap4.top().value == 5);
  REQUIRE(testHeap1.empty());
  REQUIRE(testHeap3.size() == 10);
  testHeap3.uniteWith(testHeap6);
  REQUIRE(testHeap6.empty());
  testHeap3.extract_top();
  REQUIRE(testHeap3.top().value == 1);
  auto H100 = testHeap3.insert(X(100));
  testHeap3.increase_key(H100, X(99));
  testHeap3.delete_value(H100);
  testHeap1.swap(testHeap3);
}
