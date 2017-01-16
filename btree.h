/**
 * The btree is a linked structure which operates much like
 * a binary search _tree, save the fact that multiple client
 * elements are stored in a single node.  Whereas a single element
 * would partition the _tree into two ordered subtrees, a node
 * that stores m client elements partition the _tree
 * into m + 1 sorted subtrees.
 */

#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <cstddef>
#include <utility>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

#include "btree_iterator.h"

template<typename TREE> class btree;
template<typename TREE> ostream &operator<<(ostream&, const btree<TREE>&);
template<typename TREE>
class btree {
public:

    friend class btree_iterator<TREE>;
    friend class const_btree_iterator<TREE>;
    typedef btree_iterator<TREE> iterator;
    typedef const_btree_iterator<TREE> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    iterator begin() const;
    iterator end() const;
    reverse_iterator rbegin() const;
    reverse_iterator rend() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;


    btree(size_t elems_max_nodes = 3);

    btree(const btree<TREE>& my_btree_);

    btree(btree<TREE> && my_btree_);

    btree<TREE>& operator=(const btree<TREE>& right_obj);

    btree<TREE>& operator=(btree<TREE> && right_obj);

    friend std::ostream& operator<<<TREE>(std::ostream& _ost, const btree<TREE>& _tree);

    iterator find(const TREE& elem);

    const_iterator find(const TREE& elem) const;

    std::pair<iterator, bool> insert(const TREE& elem);

    ~btree();

private:

    struct Node {
        Node( const TREE& elem ,size_t max_element_ = 3, Node *parent_ = nullptr) :

                __elem_(new TREE[max_element_]),

                __parent_(parent_),

                __descendants_(new Node*[max_element_ + 1]()),

                __occupied_size_(1),

                __max_size_(max_element_) { __elem_[0] = elem; };

        Node(size_t max_element_ = 3, Node *parent_ = nullptr) :

                __elem_(new TREE[max_element_]),

                __parent_(parent_),

                __descendants_(new Node*[max_element_ + 1]()),

                __occupied_size_(0),

                __max_size_(max_element_) {	};

        ~Node() {

            for (size_t n = 0; n < __max_size_ + 1; ++n) {

                if(__descendants_[n] != nullptr)

                    delete __descendants_[n];

            }

            delete [] __elem_;

            delete [] __descendants_;

        };

        Node& operator=(const Node& right_obj);

        inline bool	  node_is_empty() const { return __occupied_size_ == __max_size_; };
        inline size_t size() 	const { return __occupied_size_; };


        TREE      *__elem_;
        Node   *__parent_;
        Node   **__descendants_;
        size_t __occupied_size_;  // The current element sum in the Node.
        size_t __max_size_;       // The maximum number of elements that can be

    };

    Node *root_, *head_, *tail_;
    size_t __elems_max_nodes;  	// The maximum number of elements that can be

    size_t ___SizeOfTree_;    	// The size (node number) of the b-_tree.
};


template<typename TREE>
btree<TREE>::btree(size_t elems_max_nodes) :
        root_(nullptr), head_(nullptr), tail_(nullptr), __elems_max_nodes(elems_max_nodes),
        ___SizeOfTree_(0) {}

template<typename TREE>
btree<TREE>::btree(const btree<TREE>& my_btree_) :
        root_(nullptr), head_(nullptr), tail_(nullptr), __elems_max_nodes(
        my_btree_.__elems_max_nodes), ___SizeOfTree_(0) {
    queue<typename btree<TREE>::Node*> _node_queue;
    _node_queue.push(my_btree_.root_);
    while (_node_queue.size() != 0) {
        typename btree<TREE>::Node* _temp = _node_queue.front();
        _node_queue.pop();
        for (size_t n = 0; n < _temp->__occupied_size_; ++n) {
            insert(_temp->__elem_[n]);
        }
        if (_temp->node_is_empty()) {
            for (size_t n = 0; n <= _temp->__occupied_size_; ++n) {
                if (_temp->__descendants_[n] != nullptr)
                    _node_queue.push(_temp->__descendants_[n]);
            }
        }
    }
}


template<typename TREE>
btree<TREE>::btree(btree<TREE> && my_btree_) :
        root_(my_btree_.root_), head_(my_btree_.head_), tail_(my_btree_.tail_), __elems_max_nodes(
        my_btree_.__elems_max_nodes), ___SizeOfTree_(my_btree_.___SizeOfTree_) {
}

template<typename TREE>
btree<TREE>::~btree() {
    delete root_;
}


template<typename TREE> btree<TREE>&
btree<TREE>::operator=(const btree<TREE>& right_obj) {
    if (this != &right_obj) {
        delete root_;
        root_ = nullptr;
        head_ = nullptr;
        tail_ = nullptr;

        queue<typename btree<TREE>::Node*> _node_queue;
        _node_queue.push(right_obj.root_);
        while (_node_queue.size() != 0) {
            typename btree<TREE>::Node* _temp = _node_queue.front();
            _node_queue.pop();
            for (size_t n = 0; n < _temp->__occupied_size_; ++n) {
                insert(_temp->__elem_[n]);
            }
            if (_temp->node_is_empty()) {
                for (size_t n = 0; n <= _temp->__occupied_size_; ++n) {
                    if (_temp->__descendants_[n] != nullptr)
                        _node_queue.push(_temp->__descendants_[n]);
                }
            }
        }
        __elems_max_nodes = right_obj.__elems_max_nodes;
        ___SizeOfTree_ = right_obj.___SizeOfTree_;
    }
    return *this;
}

template<typename TREE> btree<TREE>&
btree<TREE>::operator=(btree<TREE> && right_obj) {
    if (this != &right_obj) {
        delete root_;
        root_ = nullptr;
        head_ = nullptr;
        tail_ = nullptr;
        __elems_max_nodes = right_obj.__elems_max_nodes;
        ___SizeOfTree_ = right_obj.___SizeOfTree_;
        root_ = right_obj.root_;

        right_obj.root_ = new Node(__elems_max_nodes);
        right_obj.head_ = nullptr;
        right_obj.tail_ = nullptr;
    }
    return *this;
}

template<typename TREE> typename btree<TREE>::Node&
btree<TREE>::Node::operator =(const btree<TREE>::Node& right_obj) {
    if (this != &right_obj) {
        delete this;
        __max_size_ = right_obj.__max_size_;
        __occupied_size_ = right_obj.__occupied_size_;
        __elem_ = new TREE[__max_size_];
        __parent_ = nullptr;
        for (size_t n = 0; n < __max_size_; ++n) {
            if (right_obj.__descendants_[n] != nullptr) {
                Node _temp(*right_obj.__descendants_[n]);
                __descendants_[n] = &_temp;
            }
        }
    }
    return *this;
}

template<typename TREE> typename btree<TREE>::iterator
btree<TREE>::begin() const {

    return iterator(head_, 0, this);
}

template<typename TREE> typename btree<TREE>::iterator
btree<TREE>::end() const {
    return iterator(NULL, 0, this);
}

template<typename TREE> typename btree<TREE>::reverse_iterator
btree<TREE>::rbegin() const {
    return reverse_iterator(end());
}

template<typename TREE> typename btree<TREE>::reverse_iterator
btree<TREE>::rend() const {
    return reverse_iterator(begin());
}

template<typename TREE> typename btree<TREE>::const_iterator
btree<TREE>::cbegin() const {
    return const_iterator(begin());
}

template<typename TREE> typename btree<TREE>::const_iterator
btree<TREE>::cend() const {
    return const_iterator(end());
}

template<typename TREE> typename btree<TREE>::const_reverse_iterator
btree<TREE>::crbegin() const {
    return const_reverse_iterator(end());
}

template<typename TREE> typename btree<TREE>::const_reverse_iterator
btree<TREE>::crend() const {
    return const_reverse_iterator(begin());
}

template<typename TREE> std::ostream&
operator<<(std::ostream& _ost, const btree<TREE>& _tree) {
    vector<typename btree<TREE>::Node *> _currLevel;
    vector<typename btree<TREE>::Node *> _nextLevel;

    _currLevel.push_back(_tree.root_);
    while (!_currLevel.empty()) {
        typename vector<typename btree<TREE>::Node *>::const_iterator iter;
        for (iter = _currLevel.begin(); iter != _currLevel.end(); ++iter) {
            typename btree<TREE>::Node * _node = *iter;
            size_t n;
            for (n = 0; n < _node->__occupied_size_; ++n) {
                _ost << _node->__elem_[n] << " ";
                if (_node->__descendants_[n] != nullptr)
                    _nextLevel.push_back(_node->__descendants_[n]);
            }
            // descendant always have one more
            if (_node->__descendants_[n] != nullptr)
                _nextLevel.push_back(_node->__descendants_[n]);
        }
        _currLevel = _nextLevel;
        _nextLevel.clear();
    }
    return _ost;
}

template<typename TREE> typename btree<TREE>::iterator
btree<TREE>::find(const TREE& elem) {
    Node *_temp_ = root_;
    while(true) {
        size_t n;
        // go through all elements from root to tail
        for (n = 0; n < _temp_->__occupied_size_; ++n) {
            if (_temp_->__elem_[n] == elem) {
                // find the elem, return an iterator
                return iterator(_temp_, n, this);
            } else if (_temp_->__elem_[n] > elem) {
                // elem is not in current Node, go to descendants
                // for the elem.
                if (_temp_->__descendants_[n] == nullptr) {
                    return end();
                } else {
                    _temp_ = _temp_->__descendants_[n];
                    break;
                }
            }
        }
        // handling boundaries cases
        if (n == _temp_->__occupied_size_) {
            if (_temp_->__descendants_[n] == nullptr) {
                return end();
            } else {
                _temp_ = _temp_->__descendants_[n];
            }
        }
    }
}

template<typename TREE> typename btree<TREE>::const_iterator
btree<TREE>::find(const TREE& elem) const {
    Node *_temp_ = root_;
    while(true) {
        size_t n;
        // go through all elements from root to tail
        for (n = 0; n < _temp_->__occupied_size_; ++n) {
            if (_temp_->__elem_[n] == elem) {
                // find the elem, return an iterator
                return const_iterator(_temp_, n, this);
            } else if (_temp_->__elem_[n] > elem) {
                // elem is not in current Node, go to descendants
                // for the elem.
                if (_temp_->__descendants_[n] == nullptr) {
                    return cend();
                } else {
                    _temp_ = _temp_->__descendants_[n];
                    break;
                }
            }
        }
        // handling boundaries cases
        if (n == _temp_->__occupied_size_) {
            if (_temp_->__descendants_[n] == nullptr) {
                return cend();
            } else {
                _temp_ = _temp_->__descendants_[n];
            }
        }
    }
}

template<typename TREE> std::pair<typename btree<TREE>::iterator, bool>
btree<TREE>::insert(const TREE& elem) {
    Node **pointer_ = &root_;
    Node *parent = NULL;
    bool new_head = false;
    bool new_tail = false;
    while (true) {
        Node *_temp_ = *pointer_;
        if (_temp_ == nullptr) {
            // No element in the btree currently, create a new Node elem
            *pointer_ = new Node(elem, __elems_max_nodes, parent);
            if (*pointer_ == root_) {
                head_ = root_;
                tail_ = root_;
            } else if (new_head == true) {
                head_ = *pointer_;
                new_head = false;
            } else if (new_tail == true) {
                tail_ = *pointer_;
                new_tail = false;
            }
            ++___SizeOfTree_;
            return make_pair(iterator(_temp_, 0, this), true);
        } else {
            // search current node to see if elem exists
            size_t n;
            for (n = 0; n < _temp_->__occupied_size_; ++n) {
                if (_temp_->__elem_[n] == elem) {
                    // elem already exists in current node, should return false.
                    return make_pair(iterator(_temp_, n, this), false);
                } else if (_temp_->__elem_[n] > elem) {
                    if (_temp_->node_is_empty()) {
                        pointer_ = &(_temp_->__descendants_[n]);
                        break;
                    } else {
                        // move existing element
                        for (size_t j = _temp_->__occupied_size_; j > n && j > 0; --j) {
                            _temp_->__elem_[j] = _temp_->__elem_[j-1];
                        }
                        _temp_->__elem_[n] = elem;
                        break;
                    }
                }
            }

            // handling boundaries cases
            if (_temp_->node_is_empty()) {
//				std::cout << n << std::endl;
                if (n == 0) {
                    if (_temp_ == head_)
                        new_head = true;
                } else if (n == __elems_max_nodes) {
                    pointer_ = &(_temp_->__descendants_[n]);
                    if (_temp_ == tail_)
                        new_tail = true;
                }
                parent = _temp_;
            } else {
                // elem is larger than all exist elements, add to the end
                if (_temp_->__occupied_size_ == n)
                    _temp_->__elem_[n] = elem;
                ++(_temp_->__occupied_size_);
                ++___SizeOfTree_;
                return make_pair(iterator(_temp_, n, this), true);
            }

        }

    }

}
//#include "btree.tem.cpp"

#endif
