/**
 *  Templated BST
 *  Luis Leon V.
 */

/**
 * @file bst.hpp
 */
#pragma once
#include <memory>
#include <utility>

template <typename KT, typename VT, typename CMP>
class bst{
  
  /**
   * @brief Node struct
   * @details This defines the members or leaves of the binary tree
   */
  struct node{
    std::pair<const KT, VT> pair;

    std::unique_ptr<node> parent;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;
  };
  
  /**
   * @brief Pointer the root node
   */
  std::unique_ptr<node> root;

  /**
   * @brief Auxiliar function to walk into the tree
   * @details This method will get the lower leave from the current node
   * This will be used to get the next element, when executing this method
   * from the right child of the current node.
   */
  node * const get_lower(node * const leave) {
    if(leave->left_child) {
      return get_lower(leave->left_child);
    }
    return leave;
  }

  /**
   * @brief Iterator class
   * @details The iterator value is a std::pair, because it acts as a whole.
   */
  template <typename O>
  class __iterator;

  using iterator = __iterator< std::pair<const KT, VT> >;
  using const_iterator = __iterator< const std::pair<const KT, VT> >;

  /**
   * @brief Iterator methods
   * @details The first element (left-most) will be the first, whereas the 
   * last one is the right-most (one-past the right-most) 
   */
  iterator begin() noexcept {return iterator{get_lower(root.get());}
  iterator end() {return iterator{nullptr};}
  
  /* From a range for loop */
  const_iterator begin() const {return const_iterator{get_lower(root.get());}
  const_iterator end() const {return const_iterator{nullptr};}
  
  /* Elevated user */
  const_iterator cbegin() const {return const_iterator{get_lower(root.get());}
  const_iterator cend() const {return const_iterator{nullptr};}

};


/* Iterator class */
template <typename KT, typename VT, typename CMP>
template <typename O>
class bst<KT, VT, CMP>::__iterator{

  using node = typename List<KT, VT, CMP>::node;
  node * current;
  
public:
  explicit __iterator(node * x) noexcept : current{x} {}

  using value_type = O;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using reference = value_type&;
  using pointer = value_type*;

  reference operator*() const noexcept { return current->pair; }
  pointer operator->() const noexcept { return &(*(*this)); }
  
  /* Pre-increment */
  __iterator& operator++() noexcept 
  { 
    current = get_lower(current->right_child.get()); 
    return *this;
  }

  /* Post-increment */
  __iterator operator++(int) noexcept 
  { 
    __iterator tmp {current};
    ++(*this);
    return tmp;
  }

  friend
  bool operator==(const __iterator& a, const __iterator& b)
  {
    return !(a == b);
  }
};


