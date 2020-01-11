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
#include <vector>

template <typename KT, typename VT, typename CMP = std::less<KT>>
class bst{
  using pair_t = std::pair<const KT, VT>;
  /**
   * @brief Operator member
   */
  CMP op();
  
  /**
   * @brief Node struct
   * @details This defines the members or leaves of the binary tree
   */
  struct node{
    pair_t pair;

    node * parent = nullptr;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;

    node() noexcept = default;
    ~node() noexcept = default;

    explicit node(const pair_t &p) : pair{p}, left_child{nullptr}, right_child{nullptr} {
    };
    explicit node(pair_t &&p) : pair{std::move(p)}, left_child{nullptr}, right_child{nullptr} {
    };
    node& operator=(const pair_t &p) {*this.pair = p; return *this;}
    
    explicit node(node * p, node * new_parent) 
      : pair{p->pair}, parent{new_parent}, left_child{nullptr}, right_child{nullptr} 
    {
      if (p->left_child)
      left_child = std::make_unique<node>(p->left_child.get(), this);
      if (p->right_child)
      right_child = std::make_unique<node>(p->right_child.get(), this);
    }
  };

public:
  /**
   * @brief Pointer the root node
   */
  std::unique_ptr<node> root;

  /**
   * @brief Clear
   */
  void clear(){
    root.reset();
  }

  /**
   * @brief Auxiliar function to walk into the tree
   * @details This method will get the lower leaf from the current node
   * This will be used to get the next element, when executing this method
   * from the right child of the current node.
   */
  static node * get_lower(node * leaf) {
    if(!leaf) return nullptr;

    if(leaf->left_child) {
      return get_lower(leaf->left_child.get());
    }
    return leaf;
  }

  bst() noexcept = default;
  bst(bst&& l) noexcept = default;
  
  bst(const bst& l) {
    root = std::make_unique<node>(l.root.get(), nullptr);
  }

  bst& operator=(bst&& l) noexcept = default;
  
  bst& operator=(const bst& l) {
    this->clear();
    root = std::make_unique<node>(l.root.get(), nullptr);
  }

  /**
   * @brief Iterator class
   * @details The iterator value is a std::pair, because it acts as a whole.
   */
  template <typename O>
  class __iterator;

  using iterator = __iterator< pair_t >;
  using const_iterator = __iterator< const pair_t >;

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
   * @brief Lookup 
   */
  auto look_up (const KT key, std::unique_ptr<node> * bin, node * parent = nullptr) {
    if (!(*bin)) {
      return std::make_pair(bin, parent);
    } else {
      auto bin_key = std::get<0>(bin->get()->pair);
      if (op(key, bin_key)) {
        return look_up(key, &(bin->get()->left_child), bin->get());
      } else if (key == bin_key) {
        return std::make_pair(bin, parent);
      } else {
        return look_up(key, &(bin->get()->right_child), bin->get());
      }
    }
  }

  /**
   * @brief Find
  */
  iterator find(const KT& key) {
    auto bin = std::get<0>(look_up(key, &root));
    if (!bin->get()) {
      return end();
    } 
    return iterator{bin->get()};
  }
  const_iterator find(const KT& key) const {
    auto bin = std::get<0>(look_up(key, &root));
    if (!bin->get()) {
      return end();
    } 
    return const_iterator{bin->get()};
  }

  /**
   * @brief Insertion Method
   * @details This inserts a new node in case it does not exist. In case it 
   * exist, it won't overwrite the value, but the iterator is returned
   */
  std::pair<iterator, bool> insert(const pair_t& x) {
    /* Get the key */
    
    auto target_k = std::get<0>(x);
    auto elem_lookup = look_up(target_k, &root);

    auto bin = std::get<0>(elem_lookup);
    auto parent = std::get<1>(elem_lookup);
    
    if (!bin->get()) {
      *(bin) = std::make_unique<node>(x);
      bin->get()->parent = parent;
      return std::make_pair(iterator{bin->get()}, true);
    } else {
      return std::make_pair(iterator{bin->get()}, false);
    } 
  }
  std::pair<iterator, bool> insert(pair_t&& x) {
    /* Get the key */
    auto target_k = std::get<0>(x);
    auto elem_lookup = look_up(target_k, &root);

    auto bin = std::get<0>(elem_lookup);
    auto parent = std::get<1>(elem_lookup);
    
    if (!bin->get()) {
      *(bin) = std::make_unique<node>(std::move(x));
      bin->get()->parent = parent;
      return std::make_pair(iterator{bin->get()}, true);
    } else {
      return std::make_pair(iterator{bin->get()}, false);
    } 
  }

  /**
   * @brief Put-to operator
   * @details 
   */
  friend std::ostream & operator<<(std::ostream& os, const bst& x){
    os << "BST: { ";
    for(auto i : x) {
      os << std::get<0>(i) << "," << std::get<1>(i) << " ";
    }
    os << "} ";
    return os;
  }

  /**
   * @brief Subscription operator
   * @details
   */
  VT& operator[](const KT& x) {
    /* Use insert with an default value */
    auto element = std::make_pair(x, VT{});
    auto insert_result = insert(element);

    /* Return the reference */
    auto it = std::get<0>(insert_result);
    return std::get<1>(*it);
  }
  VT& operator[](KT&& x) {
    /* Use insert with an default value */
    auto element = std::make_pair(x, VT{});
    auto insert_result = insert(element);

    /* Return the reference */
    auto it = std::get<0>(insert_result);
    return std::get<1>(*it);
  }

  /**
   * @brief Emplace
   */
  template< class ... Types >
  std::pair<iterator, bool> emplace(Types&&... args){
    return insert(pair_t{std::forward<Types>(args)...});
  }

  /**
   * @brief Balance
   */
  void balance(){
    std::vector<pair_t> buffer{};
    /* Load into a buffer */
    for(auto i : *(this)) {
      buffer.push_back(i);
    }
    
    /* Abort if no elements */
    if(buffer.size() == 0) return;
    
    /* Start reordering */
    clear();    
    
    size_t half = buffer.size()/2;
    size_t quarter = half/2;

    insert(buffer.at(half));
    insert(buffer.at(half + quarter));
    insert(buffer.at(half - quarter));
    for (size_t i{1}; i <= quarter; ++i) {
      size_t leftm_left_idx = quarter - i;
      size_t leftm_right_idx = quarter + i;
      size_t rightm_left_idx = half + quarter - i;
      size_t rightm_right_idx = half + quarter + i;
      
      if(leftm_right_idx != half)
        insert(buffer.at(leftm_right_idx));
      if(rightm_right_idx != buffer.size())
        insert(buffer.at(rightm_right_idx));

      insert(buffer.at(leftm_left_idx));
      insert(buffer.at(rightm_left_idx));
    }
  }

};


/* Iterator class */
template <typename KT, typename VT, typename CMP>
template <typename O>
class bst<KT, VT, CMP>::__iterator{

  using node = typename bst<KT, VT, CMP>::node;
  node * current, * past;

  
public:
  explicit __iterator(node * x) noexcept : current{x}, past{nullptr} {}

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
    node * parent = current->parent;
    /* Root case */
    if (!current) goto RETURN_ITER;
    /* It is a leaf */
    if (!current->left_child.get() && !current->right_child.get()) {
      /* Root case */
      if (!parent) {
        current = nullptr;
        past = current;
        goto RETURN_ITER;
      }
      /* is it a left leaf - just go to parent */
      if (parent->left_child.get() == current) {
        past = current;
        current = current->parent;
      }
      /* is it a right leaf - go until not being a right leaf */ 
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
      if (past == current->left_child.get() || !current->left_child.get()) {
        /* Go right */
        if (current->right_child.get()) {
          past = current;
          current = bst<KT, VT, CMP>::get_lower(current->right_child.get());
        } else {
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
        past = nullptr;
        current = bst<KT, VT, CMP>::get_lower(current);
      }
    }
RETURN_ITER:
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
