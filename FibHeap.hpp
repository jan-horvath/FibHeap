#ifndef FIBHEAP_FIBHEAP_HPP
#define FIBHEAP_FIBHEAP_HPP

#include <algorithm>
#include <cstdio>
#include <functional>
#include <cmath>
/**
 * default function for compare makes maximal Fibonacci Heap
 */
template <typename Value, typename Compare = std::less<Value>>
class FibHeap {
 public:
    class Handler;

  /**
   * class for definitions of Nodes in the fibonacci heap
   * every node has
   * 		left sibling, right sibling, parent and child - pointers to
   * Nodes
   * (nullptr if does not have)
   * 		m_mark - indicates if at least one child has been taken
   * 		m_degree - number of children
   * 		m_key - value hold in the Node
   */
  class Node {
    Node* m_left;
    Node* m_right;
    Node* m_parent;
    Node* m_child;

    bool m_mark;
    unsigned m_degree;
    Value m_key;

    FibHeap::Handler *m_handler;

    Node()
        : m_left(nullptr),
          m_right(nullptr),
          m_parent(nullptr),
          m_child(nullptr),
          m_mark(false),
          m_degree(0),
          m_key(0),
		  m_handler(nullptr){};
    Node(const Value& val)
        : m_left(nullptr),
          m_right(nullptr),
          m_parent(nullptr),
          m_child(nullptr),
          m_mark(false),
          m_degree(0),
          m_key(std::move(val)),
		  m_handler(nullptr){};
    Node(Value&& val)
        : m_left(nullptr),
          m_right(nullptr),
          m_parent(nullptr),
          m_child(nullptr),
          m_mark(false),
          m_degree(0),
          m_key(std::move(val)),
		  m_handler(nullptr){};
    Node(const Node& n)
        : m_left(nullptr),
          m_right(nullptr),
          m_parent(nullptr),
          m_child(nullptr),
          m_mark(n.m_mark),
          m_degree(n.m_degree),
          m_key(n.m_key),
		  m_handler(nullptr){};
    Node& operator=(const Node& n) {
      Node tmp(n);
      swap(tmp);
      return *this;
    }

    /**
     * swaps two Nodes
     * @param other Node to swap with
     */
    void swap(Node& other) {
      std::swap(other.m_left, m_left);
      std::swap(other.m_right, m_right);
      std::swap(other.m_parent, m_parent);
      std::swap(other.m_child, m_child);
      std::swap(other.m_key, m_key);
      std::swap(other.m_degree, m_degree);
      std::swap(other.m_mark, m_mark);
		std::swap(other.m_handler, m_handler);
    }

    ~Node() = default;

    friend class FibHeap;
  };

  /**
   * class for handling the pointer to a certain Node
   * is returned in insert to store an inserted Node
   * every handler has
   * 		m_node - pointer to a Node
   * 		m_exists - indicates if stored Node exists
   */
  class Handler {
    Node* m_node;
    bool m_exists;

    Handler() = delete;
    Handler(const Handler&) = delete;
    Handler& operator=(const Handler&) = delete;

    Handler(Node* node) : m_node(node), m_exists(true) {}

   public:
    Handler(Handler&& h) noexcept : m_node(h.m_node), m_exists(h.m_exists) {
		m_node->m_handler = this;
      h.m_node = nullptr;
      h.m_exists = false;
    }
    Handler& operator=(Handler&& h) noexcept {
      m_node = h.m_node;
      m_exists = h.m_exists;
		m_node->m_handler = this;
      h.m_node = nullptr;
      h.m_exists = false;
      return *this;
    }

    /**
     *
     * @return value of the stored Node
     */
    const Value& value() const { return m_node->m_key; }

    ~Handler() = default;

    friend class FibHeap;
  };

  /**
   * creates empty Fibonacci heap
   * @return empty Fibonacci heap
   */
  FibHeap() : m_top(nullptr), m_number(0), m_size(0) {}

  /**
   * copy constructs Fibonacci heap (deep copy)
   * @param other heap to copy from
   * @return copied heap
   */
  FibHeap(const FibHeap& other) : m_top(nullptr) {
    if (other.m_top) {
      m_top = new Node(other.top());
      copyRec(*other.m_top, *m_top, other.m_top, m_top);
    }

    m_number = other.m_number;
    m_size = other.m_size;
  }

  /**
   * move constructs Fibonacci heap
   * @param other heap to move from
   * @return moved heap
   */
  FibHeap(FibHeap&& other) noexcept : m_top(nullptr), m_number(0), m_size(0) {
    *this = std::move(other);
  }

  /**
   * copy assignment operator
   * @param other heap to copy assign from
   * @return copy assigned heap
   */
  FibHeap& operator=(const FibHeap& other) {
    FibHeap tmp(other);
    swap(tmp);
    return *this;
  }

  /**
   * move assignment operator
   * @param other heap to move assign from
   * @return move assigned heap
   */
  FibHeap& operator=(FibHeap&& other) noexcept {
    if (m_top)
      deleteFibHeap(m_top, m_top);

    m_top = other.m_top;
    other.m_top = nullptr;

    m_number = other.m_number;
    other.m_number = 0;

    m_size = other.m_size;
    other.m_size = 0;
    return *this;
  }

  ~FibHeap() {
    if (m_top)
      deleteFibHeap(m_top, m_top);
  }

  /**
   * constructs Fibonacci heap from range
   * @param begin begin of the range
   * @param end end of the range
   * @return constructed heap
   */
  template <typename It>
  FibHeap(It begin, It end) : m_top(nullptr), m_number(0), m_size(0) {
    for (It i = begin; i != end; i++)
      insert(*i);
  }

  /**
   * constructs Fibonacci heap from initializer list
   * @param list list to constract heap from
   * @return constructed heap
   */
  FibHeap(std::initializer_list<Value> list)
      : m_top(nullptr), m_number(0), m_size(0) {
    for (Value v : list)
      insert(v);
  }

  /**
   * returns top value of Fibonacci heap
   * can only be called if the heap is not empty
   * may throw exceptions
   * @return value of the top Node
   */
  const Value& top() const {
    if (!m_top)
      throw std::runtime_error("Dereferencing nullptr(top)!");
    return m_top->m_key;
  }

  /**
   *
   * @return true if heap is empty
   */
  bool empty() const { return size() == 0; }

  /**
   *
   * @return size of the heap
   */
  size_t size() const { return m_size; }

  /**
   * inserts new value into Fibonacci heap
   * function calls implementation of insert_help
   * returns Handler for this value
   * @param val value to insert
   * @return Handler to inserted Node
   */
  template <typename T = Value>
  Handler insert(T&& val) {
    Node* n = insert_help(std::forward<T>(val), std::is_lvalue_reference<T>());

    if (empty()) {
      m_top = n;
      m_top->m_right = m_top;
      m_top->m_left = m_top;
    } else {
      n->m_left = m_top->m_left;
      n->m_right = m_top;

      m_top->m_left->m_right = n;
      m_top->m_left = n;

      if (compare(m_top->m_key, n->m_key)) {
        m_top = n;
      }
    }

    m_size++;
    m_number++;
    return Handler(n);
  }

  /**
   * unites current heap with another one
   * the other heap is invalidated
   * current heap will contain all values
   * any Handlers created by the other heap have to be valid (for the current
   * heap)
   * @param other heap to unite current with
   */
  void uniteWith(FibHeap& other) {
    if (other.empty() || this->m_top == other.m_top) {
      return;
    }

    if (empty()) {
      m_top = other.m_top;
      m_size = other.m_size;
      m_number = other.m_number;

      other.m_top = nullptr;
      other.m_size = 0;
      other.m_number = 0;

      return;
    }

    Node* left = m_top->m_left;
    Node* otherRight = other.m_top->m_right;

    left->m_right = otherRight;
    otherRight->m_left = left;

    m_top->m_left = other.m_top;
    other.m_top->m_right = m_top;

    if (compare(m_top->m_key, other.m_top->m_key)) {
      m_top = other.m_top;
    }

    m_size += other.m_size;
    m_number += other.m_number;

    other.m_top = nullptr;
    other.m_size = 0;
    other.m_number = 0;
  }

  /**
   * extracts top value
   * this value is removed from the heap and new one is selected
   * this function also calls the consolidate function
   */
  void extract_top() {
	  if(m_top->m_handler){
		  m_top->m_handler->m_exists = false;
	  }

    if (size() == 1) {
      delete m_top;
      m_top = nullptr;
      m_size = 0;
      m_number = 0;
      return;
    }

    Node* left = m_top->m_left;
    Node* right = m_top->m_right;
    Node* child = m_top->m_child;

    if (child) {
      if (left != m_top) {
        child->m_left = left;
        left->m_right = child;
      }

      for (unsigned i = 0; i < m_top->m_degree - 1; i++) {
        child->m_parent = nullptr;
        child = child->m_right;
      }

      child->m_parent = nullptr;

      if (right != m_top) {
        child->m_right = right;
        right->m_left = child;
      }

      m_number = m_number + (m_top->m_degree - 1);
    } else {
      left->m_right = right;
      right->m_left = left;
      m_number--;
    }

    m_size--;

    delete m_top;
    if (child)
      m_top = child;
    else
      m_top = left;

    consolidate();
  }

  /**
   * deletes value pointed to by Handler
   * Handler is supplied by the insert function
   * may throw exceptions
   * @param h Handler to Node to delete
   */
  void delete_value(Handler& h) {
    if (!h.m_exists || !h.m_node)
      throw std::invalid_argument(
          "Handler does not exist or does not have a pointer to a Node!");

    Node* node = h.m_node;
    Node* parent = node->m_parent;

    if (parent) {
      cutBranch(h.m_node, parent);
      cascadingCutBranch(parent);
    }
    m_top = node;

    extract_top();

    h.m_exists = false;
  }

  /**
   * increase value of a key, pointed to by Handler
   * may cascade cuts the heap
   * here, increase means changing the value so that Compare(old_value,
   * new_value) returns true
   * may throw exceptions (for non-existing value and for non-satisfying
   * new_value)
   * @param h Handler to Node to change key
   * @param new_value value to change Node's value to
   */
  void increase_key(const Handler& h, const Value& new_value) {
    if (!h.m_exists || !h.m_node)
      throw std::invalid_argument(
          "Handler does not exist or does not have a pointer to a Node!");

    Value* curr_value = &h.m_node->m_key;
    if (!compare(*curr_value, new_value))
      throw std::invalid_argument("Wrong new value in increase_key!");

    *curr_value = new_value;
    Node* parent = h.m_node->m_parent;

    if (parent && !compare(*curr_value, parent->m_key)) {
      cutBranch(h.m_node, parent);
      cascadingCutBranch(parent);
    }

    if (!compare(h.m_node->m_key, m_top->m_key)) {
      m_top = h.m_node;
    }
  }

  /**
   * swaps two different Fibonacci heaps
   * @param heap heap to swap with
   */
  void swap(FibHeap& heap) {
    std::swap(m_top, heap.m_top);
    std::swap(m_number, heap.m_number);
    std::swap(m_size, heap.m_size);
  }

 private:
  /**
   * cuts the current branch and puts it in the list of tops
   * @param current Node to cut
   * @param parent parent of the @current Node
   */
  void cutBranch(Node* current, Node* parent) {
    /*if(!parent)
            return;*/
    Node* child = parent->m_child;
    bool checkChild = false;
    if (current == child)
      checkChild = true;
    while (child != current) {
      child = child->m_right;
    }
    // child == current
    if (parent->m_degree > 1) {
      child->m_right->m_left = child->m_left;
      child->m_left->m_right = child->m_right;
      if (checkChild)
        parent->m_child = child->m_right;
    } else {
      parent->m_child = nullptr;
    }

    parent->m_degree--;

    child->m_left = m_top->m_left;
    child->m_right = m_top;
    m_top->m_left->m_right = child;
    m_top->m_left = child;

    child->m_parent = nullptr;
    child->m_mark = false;

    m_number++;
  }

  /**
   * cuts branches until heap satisfy the requirements of Fibonacci heap
   * @param node Node to cut from
   */
  void cascadingCutBranch(Node* node) {
    Node* parent = node->m_parent;

    if (!parent)
      return;

    while (node->m_mark) {
      if (!parent)
        break;
      cutBranch(node, parent);
      node = parent;
      parent = parent->m_parent;
    }
    node->m_mark = true;
  }

  /**
   * computes max degree of all heap parts
   * @return max degree for heaps
   */
  int maxDegree() {
    using namespace std;
    return static_cast<int>(
               ceil(log(static_cast<double>(m_size)) /
                    log(static_cast<double>(1 + sqrt(static_cast<double>(5))) /
                        2))) + 1;
  }

  /**
   * modifies the heap so that it does not contain two trees with the same
   * degree
   * ensures the amortized logatimic deletion and extract-top time
   */
  void consolidate() {
    std::vector<Node*> trees(maxDegree(), nullptr);
    Node* current = m_top;

    for (unsigned i = 0; i < m_number; i++) {
      unsigned degree = current->m_degree;
      Node* current_parent = current;

      while ((trees[degree] != nullptr)) {
        Node* son = trees[degree];
        Node* parent = current_parent;

        if (compare(parent->m_key, son->m_key)) {
          Node* tmp = son;
          son = parent;
          parent = tmp;
        }

        if (current == son)
          current = current->m_left;

        son->m_right->m_left = son->m_left;
        son->m_left->m_right = son->m_right;

        if (!parent->m_child) {
          parent->m_child = son;
          son->m_right = son;
          son->m_left = son;

        } else {
          son->m_left = parent->m_child->m_left;
          son->m_right = parent->m_child;
          parent->m_child->m_left->m_right = son;
          parent->m_child->m_left = son;
        }
        son->m_parent = parent;

        parent->m_degree++;
        trees[degree] = nullptr;
        degree++;
        current_parent = parent;
      }

      trees[degree] = current_parent;
      current = current->m_right;
    }

    m_number = 0;
    for (Node* n : trees) {
      if (n) {
        if (!m_top || compare(m_top->m_key, n->m_key) || m_top->m_parent) {
          m_top = n;
        }
        m_number++;
      }
    }
  }

  /**
   * creating and copying every node to new heap
   *
   * @param from node we are copiyng from
   * @param to node we are copying to
   * @param initial node (start) from linked list of nodes from original heap
   * @param copyInitial node (start) from linked list of nodes from new heap
   */
  void copyRec(const Node& from,
               Node& to,
               const Node* initial,
               Node* copyInitial) {
    to.m_key = from.m_key;

    if (from.m_child) {
      to.m_child = new Node(*from.m_child);
      to.m_child->m_parent = &to;

      copyRec(*from.m_child, *to.m_child, from.m_child, to.m_child);
    } else {
      to.m_child = nullptr;
    }

    if (from.m_right == initial) {
      to.m_right = copyInitial;
      to.m_right->m_left = &to;
    } else {
      to.m_right = new Node(*from.m_right);
      to.m_right->m_left = &to;

      copyRec(*from.m_right, *to.m_right, initial, copyInitial);
    }

    to.m_parent = copyInitial->m_parent;
    to.m_degree = from.m_degree;
    to.m_mark = from.m_mark;
  }

  /**
   * compares two values with function if the heap
   * @param a first value
   * @param b second value
   * @return true/false according to Compare function
   */
  bool compare(const Value& a, const Value& b) const {
    return cmpFunction(a, b);
  }

  bool compare(Value& a, Value& b) { return cmpFunction(a, b); }

  /**
   * implementation of insert for rvalue values
   * @param t value to insert
   * @return created Node from the value @t
   */
  template <typename T = Value>
  Node* insert_help(T&& t, std::false_type) {
    auto n = new Node(std::move(t));
    return n;
  }

  /**
   * implementation of insert for lvalue values
   * @param t value to insert
   * @return created Node from the value @t
   */
  template <typename T = Value>
  Node* insert_help(const T& t, std::true_type) {
    auto n = new Node(t);
    return n;
  }

  void deleteFibHeap(Node* top, Node* current) {
    if (current->m_child) {
      deleteFibHeap(current->m_child, current->m_child);
    }
    if (current->m_right != top) {
      deleteFibHeap(top, current->m_right);
    }
    if (current == top) {
      if (current->m_parent)
        current->m_parent->m_child = nullptr;
      delete current;
      return;
    }
    top->m_left = current->m_left;
    current->m_left->m_right = top;
    delete current;
  }

  static Compare cmpFunction;
  Node* m_top;
  unsigned m_number;
  size_t m_size;
};

template <typename Value, typename Compare>
Compare FibHeap<Value, Compare>::cmpFunction = Compare();

#endif  // FIBHEAP_FIBHEAP_HPP
