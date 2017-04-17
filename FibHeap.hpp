//
// Created by Jan Horvath on 16-Apr-17.
//

#ifndef FIBHEAP_FIBHEAP_HPP
#define FIBHEAP_FIBHEAP_HPP

template <typename Value> //treba pridat druhy parameter sablony
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

		Node(const Value &value);

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
	FibHeap(); //: m_minimum(nullptr), m_number(0) {}

	//Returns minimum value of Fibonacci Heap
	const Value* minimum() const;

	//inserts new value into Fibonacci Heap
	//returns handler for this value
	Handler insert(const Value& value);

	//unites current heap with another one
	//the other heap is invalidated
	//current heap will contain all values
	void uniteWith(FibHeap& other);

	//extracts minimum value
	//this value is removed from the heap and new one is selected
	//this function also calls the consolidate function
	Value* extract_min();

	//deletes value pointed to by handler
	//handler is supplied by the insert function
	//returns true if deletion was successful
	bool delete_value(Handler &handler);


	//decreases value of a key, pointed to by handler
	//may cascade cut the heap
	bool decrease_key(const Handler &handler);

private:
	//modifies the heap so that it does not contain two trees with the same degree
	//ensures the amortized logatimic deletion and extract-min time
	void consolidate();

	Value* m_minimum;
	unsigned m_number;
};



#endif //FIBHEAP_FIBHEAP_HPP
