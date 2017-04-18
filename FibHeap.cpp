//
// Created by Jan Horvath on 16-Apr-17.
//

#include "FibHeap.hpp"

FibHeap<value_type, compare>::Node::Node(const value_type &val) : m_left(nullptr), m_right(nullptr), m_parent(nullptr), m_child(nullptr)
		, m_mark(false), m_degree(0), m_key(val) {}

/*
FibHeap<value_type>::Handler::Handler(FibHeap::Node *node) : m_node(node), m_exists(true /* ? ) {}


FibHeap<value_type, compare>::FibHeap() : m_top(nullptr), m_number(0) {}

FibHeap<value_type, compare>::FibHeap(const FibHeap&);

FibHeap<value_type, compare>::FibHeap(FibHeap&&);

FibHeap FibHeap<value_type, compare>::operator=(const FibHeap&);

FibHeap FibHeap<value_type, compare>::operator=(FibHeap &&);

template <typename It>
FibHeap<value_type, compare>::FibHeap(It begin, It end);

FibHeap<value_type, compare>::FibHeap(std::initializer_list<value_type>);

const value_type& FibHeap<value_type, compare>::top() const{
	return m_top->m_key;
}

bool FibHeap<value_type, compare>::empty() const{
	return size() == 0;
}

size_t FibHeap<value_type, compare>::size() const{
	return m_size;
}

Handler FibHeap<value_type, compare>::insert(const value_type &val){
	return insert(std::move(val));
}

Handler FibHeap<value_type, compare>::insert(value_type &&val){
	Node *x(val);

	if(!m_top){
		m_top = x;
	}else{
		m_top->m_left->m_right = x;
		m_top->m_left = x;

		if(x->m_key < m_top->m_key){
			m_top = x;
		}
	}
	m_number++;
}

void FibHeap<value_type, compare>::uniteWith(FibHeap&);

value_type* FibHeap<value_type, compare>::extract_top();

bool FibHeap<value_type, compare>::delete_value(Handler &);

bool FibHeap<value_type, compare>::decrease_key(const Handler &);

void FibHeap<value_type, compare>::swap(FibHeap &);
*/