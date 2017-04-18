//
// Created by Jan Horvath on 16-Apr-17.
//

#ifndef FIBHEAP_FIBHEAP_HPP
#define FIBHEAP_FIBHEAP_HPP

#include <cstdio>
#include <functional>

template <typename Value, typename Compare = std::greater<Value>> //default function for compare makes minimal Fibonacci Heap
class FibHeap {
public:
	class Node {
		Node* m_left;
		Node* m_right;
		Node* m_parent;
		Node* m_child;

		bool m_mark;
		unsigned m_degree;
		Value m_key;

		Node(const Value &);

		friend class FibHeap;
	};

	class Handler {
		Node* m_node;
		bool m_exists;

		Handler() = delete;
		Handler(Node* node);
		Handler(const Handler&) = delete;
		Handler operator=(const Handler&) = delete;

		friend class FibHeap;
	};

	//creates empty Fibonacci Heap
	FibHeap(); //: m_top(nullptr), m_number(0) {}

	//copy constructs Fibonacci Heap (deep copy)
	FibHeap(const FibHeap&);

	//move constructs Fibonacci Heap
	FibHeap(FibHeap&&);

	//copy assignment operator
	FibHeap operator=(const FibHeap&);

	//move assignment operator
	FibHeap operator=(FibHeap &&);

	//constructs Fibonacci Heap from range
	template <typename It>
	FibHeap(It begin, It end);

	//constructs Fibonacci Heap from initializer list
	FibHeap(std::initializer_list<Value>);

	//Returns top value of Fibonacci Heap
	const Value* top() const;

	//returns true if heap is empty
	bool empty() const;

	//returns size of the heap
	size_t size() const;

	//inserts new value into Fibonacci Heap
	//returns handler for this value
	Handler insert(const Value&);

	//unites current heap with another one
	//the other heap is invalidated
	//current heap will contain all values
	void uniteWith(FibHeap&);

	//extracts top value
	//this value is removed from the heap and new one is selected
	//this function also calls the consolidate function
	Value* extract_top();

	//deletes value pointed to by handler
	//handler is supplied by the insert function
	//returns true if deletion was successful
	bool delete_value(Handler &);

	//decreases value of a key, pointed to by handler
	//may cascade cut the heap
	bool decrease_key(const Handler &);

	//swaps two different Fibonacci Heaps
	void swap(FibHeap &);

private:
	//modifies the heap so that it does not contain two trees with the same degree
	//ensures the amortized logatimic deletion and extract-top time
	void consolidate();

	Value* m_top;
	unsigned m_number;
};



#endif //FIBHEAP_FIBHEAP_HPP
