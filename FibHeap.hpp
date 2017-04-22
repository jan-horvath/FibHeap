//
// Created by Jan Horvath on 16-Apr-17.
//

#ifndef FIBHEAP_FIBHEAP_HPP
#define FIBHEAP_FIBHEAP_HPP

#include <algorithm>
#include <cstdio>
#include <functional>
#include <cmath>

//default function for compare makes maximal Fibonacci Heap
template <typename Value, typename Compare = std::less<Value>>
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

		Node(const Value &val) : m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_child(nullptr)
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
	FibHeap(): m_top(nullptr), m_number(0), m_size(0) {}

	//copy constructs Fibonacci Heap (deep copy)
	FibHeap(const FibHeap &other) : m_top(nullptr) {
		if(other.m_top) {
			m_top = new Node(other.top());
			copyRec(*other.m_top, *m_top, other.m_top, m_top);
		}

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
	FibHeap(It begin, It end) : m_top(nullptr), m_number(0), m_size(0) {
		for(It i = begin; i != end; i++)
			insert(*i);
	}

	//constructs Fibonacci Heap from initializer list
	FibHeap(std::initializer_list<Value> list) : m_top(nullptr), m_number(0), m_size(0) {
		for(Value v : list)
			insert(v);
	}

	//Returns top value of Fibonacci Heap
	//Can only be called if the heap is not empty
	//May throw exceptions
	const Value& top() const {
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
	Handler&& insert(const Value &val) {
		return insert(static_cast<Value&&>(static_cast<Value>(val)));
	}

	//moves new value into Fibonacci Heap
	Handler&& insert(Value &&val) {
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

			if(compare(m_top->m_key, n->m_key)){
				m_top = n;
			}
		}

		m_size++;
		m_number++;
		return Handler(n);
	}

	//unites current heap with another one
	//the other heap is invalidated
	//current heap will contain all values
	//any Handlers created by the other heap have to be valid (for the current heap)
	void uniteWith(FibHeap &other){
		if(other.empty() || this->m_top == other.m_top){
			return;
		}

		if(empty()){
			m_top = other.m_top;
			m_size = other.m_size;
			m_number = other.m_number;

			other.m_top = nullptr;
			other.m_size = 0;
			other.m_number = 0;

			return;
		}

		Node *left = m_top->m_left;
		Node *otherRight = other.m_top->m_right;

		left->m_right = otherRight;
		otherRight->m_left = left;

		m_top->m_left = other.m_top;
		other.m_top->m_right = m_top;

		if(compare(m_top->m_key, other.m_top->m_key)){
			m_top = other.m_top;
		}

		m_size += other.m_size;
		m_number += other.m_number;

		other.m_top = nullptr;
		other.m_size = 0;
		other.m_number = 0;
	}

	//extracts top value
	//this value is removed from the heap and new one is selected
	//this function also calls the consolidate function
	void extract_top(){
		if(size() == 1){
			//delete m_top;
			m_top = nullptr;
			m_size = 0;
			m_number = 0;
			return;
		}

		Node *left = m_top->m_left;
		Node *right = m_top->m_right;
		Node *child = m_top->m_child;

		if(child) {
			if(left != m_top) {
				child->m_left = left;
				left->m_right = child;
			}

			for (int i = 0; i < m_top->m_degree - 1; i++) {
				child->m_parent = nullptr;
				child = child->m_right;
			}

			child->m_parent = nullptr;

			if(right != m_top) {
				child->m_right = right;
				right->m_left = child;
			}

			m_number = m_number + (m_top->m_degree - 1);
		}else{
			left->m_right = right;
			right->m_left = left;
			m_number--;
		}

		m_size--;

		delete m_top;
		if(child)
			m_top = child;
		else
			m_top = left;

		consolidate();
	}

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
	int maxDegree(){
		using namespace std;
		return static_cast<int>(ceil(log(static_cast<double>(m_size))/log(static_cast<double>(1 + sqrt(static_cast<double>(5)))/2))) + 1;
	}

	//modifies the heap so that it does not contain two trees with the same degree
	//ensures the amortized logatimic deletion and extract-top time
	void consolidate(){
		std::vector<Node*> trees(maxDegree(), nullptr);
		Node *current = m_top;

		for(int i = 0; i < m_number; i++){
			unsigned degree = current->m_degree;

			while(trees[degree] != nullptr){
				Node *other = trees[degree];

				if(compare(current->m_key, other->m_key)){
					Node *tmp = other;
					other = current;
					current = tmp;
				}
				//current is parent
				//other is son

				other->m_right->m_left = other->m_left;
				other->m_left->m_right = other->m_right;

				if(!current->m_child){
					current->m_child = other;
					other->m_right = other;
					other->m_left = other;

				}else{
					other->m_left = current->m_child->m_left;
					other->m_right = current->m_child;
					current->m_child->m_left->m_right = other;
					current->m_child->m_left = other;

				}
				other->m_parent = current;

				current->m_degree++;
				trees[degree] = nullptr;
				degree++;
			}

			trees[degree] = current;
			current = current->m_right;
		}

		m_number = 0;
		for(Node *n : trees){
			if(n) {
				if (!m_top || compare(m_top->m_key, n->m_key)) {
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

	bool compare(const Value &a, const Value &b) const {
		Compare cmp = Compare();
		return cmp(a,b);
	}

	Node* m_top;
	unsigned m_number;
	size_t m_size;
};


#endif //FIBHEAP_FIBHEAP_HPP
