#ifndef BTREE_ITERATOR_H
#define BTREE_ITERATOR_H

#include <iterator>
using namespace std;

template<typename TREE> class btree;
template<typename TREE> class btree_iterator;
template<typename TREE> class const_btree_iterator;

template<typename TREE> class btree_iterator {
public:
    typedef ptrdiff_t                  	difference_type;
    typedef bidirectional_iterator_tag	iterator_category;
    typedef TREE 						   	value_type;
    typedef TREE* 							pointer;
    typedef TREE& 							reference;
    friend class const_btree_iterator<TREE>;

    reference       operator *() const;
    pointer         operator->() const { return &(operator*()); }
    btree_iterator& operator++();
    btree_iterator& operator--();
    btree_iterator  operator++(int);
    btree_iterator  operator--(int);
    btree_iterator& operator =(const btree_iterator& temp_btree);
    bool			operator==(const btree_iterator& temp_btree) const;
    bool            operator==(const const_btree_iterator<TREE>& temp_btree) const;
    bool			operator!=(const btree_iterator& temp_btree) const
    { return !operator==(temp_btree); }
    bool            operator!=(const const_btree_iterator<TREE>& temp_btree) const
    { return !operator==(temp_btree); }
    btree_iterator(typename btree<TREE>::Node *pointee = nullptr, size_t idx = 0,
                   const btree<TREE> *btree = nullptr) :
            pointee_(pointee), _index(idx), btree_(btree) {}
private:

    void _up_ascend(const TREE& temp_elem);
    void _down_ascend();
    void _up_descend(const TREE& temp_elem);
    void _down_descend();

    typename btree<TREE>::Node *pointee_;
    size_t _index;
    const btree<TREE> *btree_;
};

template<typename TREE> class const_btree_iterator {
public:
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag	iterator_category;
    typedef TREE value_type;
    typedef const TREE* pointer;
    typedef const TREE& reference;
    friend class btree_iterator<TREE>;

    reference operator*()  const;
    pointer operator->() const { return &(operator*()); }
    const_btree_iterator& operator++();
    const_btree_iterator& operator--();
    const_btree_iterator  operator++(int);
    const_btree_iterator  operator--(int);
    const_btree_iterator& operator =(const const_btree_iterator& temp_btree);
    bool				  operator==(const const_btree_iterator& temp_btree) const;
    bool            	  operator==(const btree_iterator<TREE>& temp_btree) const;
    bool				  operator!=(const const_btree_iterator& temp_btree) const	{ return !operator==(temp_btree); }
    bool            	  operator!=(const btree_iterator<TREE>& temp_btree) const		{ return !operator==(temp_btree); }
    const_btree_iterator(typename btree<TREE>::Node *pointee = nullptr, size_t idx = 0,
                         const btree<TREE> *btree = NULL) :
            pointee_(pointee), _index(idx), btree_(btree) {}
    const_btree_iterator(const btree_iterator<TREE>& rhs) :
            pointee_(rhs.pointee_), _index(rhs._index), btree_(rhs.btree_) {}
private:
    /**
     * helper functions
     */
    void _up_ascend(const TREE& temp_elem);
    void _down_ascend();
    void _up_descend(const TREE& temp_elem);
    void _down_descend();

    /**
     * private parameters
     */
    typename btree<TREE>::Node *pointee_;
    size_t _index;
    const btree<TREE> *btree_;
};

#include "btree_iterator.tem"
//#include "btree_iterator.tem.cpp"
#endif
