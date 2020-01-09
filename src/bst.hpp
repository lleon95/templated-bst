/**
 *  Templated BST
 *  Luis Leon V.
 */

/**
 * @file bst.hpp
 */
#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <unistd.h>

template <typename KT, typename VT, typename CMP = std::less<KT>>
class bst{
  
  /**
   * @brief Node struct
   * @details This defines the members or leaves of the binary tree
   */
  struct node{
    std::pair<const KT, VT> pair;

    node * parent = nullptr;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;

    node() noexcept = default;

    node(const std::pair<const KT, VT> &p) : pair{p}, left_child{nullptr}, right_child{nullptr} {
      std::cout << "Constructor: " << std::get<0>(p) << std::endl;
    };
    node& operator=(const std::pair<const KT, VT> &p) {*this.pair = p; return *this;}
  };

public:
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
  static node * get_lower(node * leave) {
    if(!leave) return nullptr;

    if(leave->left_child) {
      return get_lower(leave->left_child.get());
    }
    return leave;
  }

  bst() noexcept = default;
  bst(bst&& l) noexcept = default;
  bst& operator=(bst&& l) noexcept = default;

  bst(const bst& l);
  bst& operator=(const bst& l);

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
  iterator begin() noexcept {return iterator{get_lower(root.get())};}
  iterator end() {return iterator{nullptr};}
  
  /* From a range for loop */
  const_iterator begin() const {return const_iterator{get_lower(root.get())};}
  const_iterator end() const {return const_iterator{nullptr};}
  
  /* Elevated user */
  const_iterator cbegin() const {return const_iterator{get_lower(root.get())};}
  const_iterator cend() const {return const_iterator{nullptr};}
 
  /**
   * @brief Insertion Method
   * @details This inserts a new node in case it does not exist. In case it 
   * exist, it won't overwrite the value, but the iterator is returned
   */
  std::pair<iterator, bool> insert(const std::pair<KT, VT>& x) {
    /* Get the key */
    auto target_k = std::get<0>(x);
    std::cout << "Key - target: " << target_k << std::endl;
    /* Get the node bin */
    if (!root) {
      root = std::make_unique<node>(x);
      return std::make_pair(iterator{root.get()}, true);
    }

    auto node_bin = root.get();

    while(true) {
      auto node_key = std::get<0>(node_bin->pair);
      std::cout << "Key - node: " << node_key << std::endl;
      /* Case less */
      if(target_k < node_key) {
        if (!node_bin->left_child) {
          node_bin->left_child = std::make_unique<node>(x);
          node_bin->left_child.get()->parent = node_bin;
          return std::make_pair(iterator{node_bin->left_child.get()}, true);
        } else {
          node_bin = node_bin->left_child.get();
        }
      } else if(node_key == target_k) {
        return std::make_pair(iterator{node_bin}, false);
      } else {
        if (!node_bin->right_child) {
          node_bin->right_child = std::make_unique<node>(x);
          node_bin->right_child.get()->parent = node_bin;
          return std::make_pair(iterator{node_bin->right_child.get()}, true);
        } else {
          node_bin = node_bin->right_child.get();
        }
      }
    }
  }
};


/* Iterator class */
template <typename KT, typename VT, typename CMP>
template <typename O>
class bst<KT, VT, CMP>::__iterator{

  using node = typename bst<KT, VT, CMP>::node;
  node * current, * past = nullptr;

  
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
    usleep(1000000);
    /* It is a leave */
    if (!current->left_child.get() && !current->right_child.get()) {
      node * parent = current->parent;
      /* is it a left leave - just go to parent */
      if (parent->left_child.get() == current) {
        past = current;
        current = current->parent;
      }
      /* is it a right leave - go until not being a right leave */ 
      else {
        while(parent->right_child.get() == current) {
          current = parent;
          parent = current->parent;
          if (parent == nullptr) {
            /* End of tree */
            break;
          }
        }
        if(parent != nullptr) {
          past = current;
          current = parent;
        } else {
          current = nullptr;
        }
      } 
    } else {
      if (past == current->left_child.get()) {
        /* Go right */
        past = current;
        current = bst<KT, VT, CMP>::get_lower(current->right_child.get());
      } else {
        past = nullptr;
        current = bst<KT, VT, CMP>::get_lower(current);
      }
    }
    return *this;
  }

  /* Post-increment */
  __iterator operator++(int) noexcept 
  { 
    __iterator tmp {current};
    ++(*this);
    return tmp;
  }

  friend bool operator==(const __iterator& a, const __iterator& b) {
    return a.current == b.current;
  }
  friend bool operator!=(const __iterator& a, const __iterator& b) {
    return !(a == b);
  }

};
