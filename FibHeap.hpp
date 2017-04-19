//
// Created by Jan Horvath on 16-Apr-17.
//

#ifndef FIBHEAP_FIBHEAP_HPP
#define FIBHEAP_FIBHEAP_HPP

#include <algorithm>
#include <cstdio>
#include <functional>

//default function for compare makes maximal Fibonacci Heap
template <typename Value, typename Compare = std::less<Value>>
class FibHeap {
public:
	using value_type = Value;
	using compare = Compare;
	class Node {
		Node* m_left;
		Node* m_right;
		Node* m_parent;
		Node* m_child;

		bool m_mark;
		unsigned m_degree;
		value_type m_key;

		Node(const value_type &val) : m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_child(nullptr)
		, m_mark(false), m_degree(0), m_key(val) {};
		Node(const Node &n) : m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_child(nullptr)
				, m_mark(n.m_mark), m_degree(n.m_degree), m_key(n.m_key) {};
		Node &operator=(const Node &n) {
			Node tmp(n);
			swap(tmp);
			return *this;
		}

		void swap(Node &other) {
			std::swap(other.m_left, m_left);
			std::swap(other.m_right, m_right);
			std::swap(other.m_parent, m_parent);
			std::swap(other.m_child, m_child);
			std::swap(other.m_key, m_key);
			std::swap(other.m_degree, m_degree);
			std::swap(other.m_mark, m_mark);
		}

		friend class FibHeap;
	};

	class Handler {
		Node* m_node;
		bool m_exists;

		Handler() = delete;
		Handler(const Handler&) = delete;
		Handler &operator=(const Handler&) = delete;

		Handler(Node* node) : m_node(node), m_exists(true) {}

	public:
		Handler(Handler &&h) : m_node(h.m_node), m_exists(h.m_exists) {
			h.m_node = nullptr;
			h.m_exists = false;
		}
		Handler &operator=(Handler &&h) {
			m_node = h.m_node;
			m_exists = h.m_exists;
			h.m_node = nullptr;
			h.m_exists = false;
			return *this;
		}

		friend class FibHeap;
	};

	//creates empty Fibonacci Heap
	FibHeap(): m_top(nullptr), m_number(0) {}

	//copy constructs Fibonacci Heap (deep copy)
	FibHeap(const FibHeap &other) {
		m_top = new Node(other.top());
		copyRec(*other.m_top, *m_top, other.m_top, m_top);
		m_number = other.m_number;
		m_size = other.m_size;
	}

	//move constructs Fibonacci Heap
	FibHeap(FibHeap &&other) {
		*this = std::move(other);
	}

	//copy assignment operator
	FibHeap &operator=(const FibHeap &other) {

		FibHeap tmp(other);
		swap(tmp);
		return *this;
	}

	//move assignment operator
	FibHeap &operator=(FibHeap &&other) {
		m_top = other.m_top;
		other.m_top = nullptr;

		m_number = other.m_number;
		other.m_number = 0;

		m_size = other.m_size;
		other.m_size = 0;
		return *this;
	}

	//constructs Fibonacci Heap from range
	template <typename It>
	FibHeap(It begin, It end) {
		for(It i = begin; i != end; i++)
			insert(*i);
	}

	//constructs Fibonacci Heap from initializer list
	FibHeap(std::initializer_list<value_type> list) {
		for(value_type v : list)
			insert(v);
	}

	//Returns top value of Fibonacci Heap
	//Can only be called if the heap is not empty
	//May throw exceptions
	const value_type& top() const {
		return m_top->m_key;
	}

	//returns true if heap is empty
	bool empty() const {
		return size() == 0;
	}

	//returns size of the heap
	size_t size() const {
		return m_size;
	}

	//inserts new value into Fibonacci Heap
	//returns handler for this value
	Handler&& insert(const value_type &val) {
		return insert(std::move(val));
	}

	//moves new value into Fibonacci Heap
	Handler&& insert(value_type &&val) {
		Node *n = new Node(val);

		if(empty()){
			m_top = n;
			m_top->m_right = m_top;
			m_top->m_left = m_top;
		}else{
			n->m_left = m_top->m_left;
			n->m_right = m_top;

			m_top->m_left->m_right = n;
			m_top->m_left = n;

			consolidate();
		}

		return Handler(n);
	}

	//unites current heap with another one
	//the other heap is invalidated
	//current heap will contain all values
	//any Handlers created by the other heap have to be valid (for the current heap)
	void uniteWith(FibHeap&);

	//extracts top value
	//this value is removed from the heap and new one is selected
	//this function also calls the consolidate function
	value_type* extract_top();

	//deletes value pointed to by handler
	//handler is supplied by the insert function
	//returns true if deletion was successful
	//may throw exceptions
	bool delete_value(Handler &);

	//increase value of a key, pointed to by handler
	//may cascade cut the heap
	//here, increase means changing the value so that Compare(current_value, new_value) returns true
	//may throw exceptions (for non-existing value and for non-satisfying new_value)
	bool increase_key(const Handler &, const Value& );

	//swaps two different Fibonacci Heaps
	void swap(FibHeap &);

private:
	//modifies the heap so that it does not contain two trees with the same degree
	//ensures the amortized logatimic deletion and extract-top time
	void consolidate();

	/**
	 * creating and copying every node to new heap
	 *
	 * @param from node we are copiyng from
	 * @param to node we are copying to
	 * @param initial node (start) from linked list of nodes from original heap
	 * @param copyInitial node (start) from linked list of nodes from new heap
	 */
	void copyRec(const Node &from, Node &to, const Node *initial, Node *copyInitial) {
		to.m_key = from.m_key;

		if(from.m_child){
			to.m_child = new Node(*from.m_child);
			*to.m_child->m_parent = to;

			copyRec(*from.m_child, *to.m_child, from.m_child, to.m_child);
		}else{
			to.m_child = nullptr;
		}

		if(from.m_right == initial){
			to.m_right = copyInitial;
			*to.m_right->m_left = to;
		}else{
			to.m_right = new Node(*from.m_right);
			*to.m_right->m_left = to;

			copyRec(*from.m_right, *to.m_right, initial, copyInitial);
		}

		to.m_parent = copyInitial->m_parent;
		to.m_degree = from.m_degree;
		to.m_mark = from.m_mark;
	}

	Node* m_top;
	unsigned m_number;
	size_t m_size;
};


#endif //FIBHEAP_FIBHEAP_HPP
