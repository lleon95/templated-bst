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

template <typename KT, typename VT, typename CMP = std::less<KT>> class bst {
  using pair_t = std::pair<const KT, VT>;
  /**
   * @brief Operator member
   */
  CMP op = CMP{};

  /**
   * @brief Node struct
   * @details This defines the members or leaves of the binary tree
   */
  struct node {
    pair_t pair;

    node *parent = nullptr;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;

    node() noexcept = default;
    ~node() noexcept = default;

    explicit node(const pair_t &p) noexcept
        : pair{p}, left_child{nullptr}, right_child{nullptr} {};
    explicit node(pair_t &&p) noexcept
        : pair{std::move(p)}, left_child{nullptr}, right_child{nullptr} {};
    node &operator=(const pair_t &p) noexcept {
      *this.pair = p;
      return *this;
    }

    explicit node(node *p, node *new_parent)
        : pair{p->pair}, parent{new_parent}, left_child{nullptr}, right_child{
                                                                      nullptr} {
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
  void clear() { root.reset(); }

  /**
   * @brief Auxiliar function to walk into the tree
   * @details This method will get the lower leaf from the current node
   * This will be used to get the next element, when executing this method
   * from the right child of the current node.
   */
  static node *get_lower(node *leaf) noexcept {
    if (!leaf)
      return nullptr;

    if (leaf->left_child) {
      return get_lower(leaf->left_child.get());
    }
    return leaf;
  }

  bst() noexcept = default;
  bst(bst &&l) noexcept = default;

  bst(const bst &l) { root = std::make_unique<node>(l.root.get(), nullptr); }

  bst &operator=(bst &&l) noexcept = default;

  bst &operator=(const bst &l) {
    this->clear();
    root = std::make_unique<node>(l.root.get(), nullptr);
  }

  /**
   * @brief Iterator class
   * @details The iterator value is a std::pair, because it acts as a whole.
   */
  template <typename O> class __iterator;

  using iterator = __iterator<pair_t>;
  using const_iterator = __iterator<const pair_t>;

  /**
   * @brief Iterator methods
   * @details The first element (left-most) will be the first, whereas the
   * last one is the right-most (one-past the right-most)
   */
  iterator begin() noexcept { return iterator{get_lower(root.get())}; }
  iterator end() noexcept { return iterator{nullptr}; }

  /* From a range for loop */
  const_iterator begin() const noexcept {
    return const_iterator{get_lower(root.get())};
  }
  const_iterator end() const noexcept { return const_iterator{nullptr}; }

  /* Elevated user */
  const_iterator cbegin() const noexcept {
    return const_iterator{get_lower(root.get())};
  }
  const_iterator cend() const noexcept { return const_iterator{nullptr}; }

  /**
   * @brief Lookup
   */
  auto look_up(const KT key, std::unique_ptr<node> *bin,
               node *parent = nullptr) {
    if (!(*bin)) {
      return std::make_pair(bin, parent);
    } else {
      auto bin_key = std::get<0>(bin->get()->pair);
      if (op(key, bin_key)) {
        return look_up(key, &(bin->get()->left_child), bin->get());
      } else if (!op(key, bin_key) && !op(bin_key, key)) {
        return std::make_pair(bin, parent);
      } else {
        return look_up(key, &(bin->get()->right_child), bin->get());
      }
    }
  }

  /**
   * @brief Find
   */
  iterator find(const KT &key) {
    auto bin = std::get<0>(look_up(key, &root));
    if (!bin->get()) {
      return end();
    }
    return iterator{bin->get()};
  }
  const_iterator find(const KT &key) const {
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
  std::pair<iterator, bool> insert(const pair_t &x) {
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
  std::pair<iterator, bool> insert(pair_t &&x) {
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
  friend std::ostream &operator<<(std::ostream &os, const bst &x) {
    os << "BST: { ";
    for (auto i : x) {
      os << std::get<0>(i) << "," << std::get<1>(i) << " ";
    }
    os << "} ";
    return os;
  }

  /**
   * @brief Subscription operator
   */
  VT &operator[](const KT &x) {
    /* Use insert with an default value */
    auto element = std::make_pair(x, VT{});
    auto insert_result = insert(element);

    /* Return the reference */
    auto it = std::get<0>(insert_result);
    return std::get<1>(*it);
  }
  VT &operator[](KT &&x) {
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
  template <class... Types>
  std::pair<iterator, bool> emplace(Types &&... args) {
    return insert(pair_t{std::forward<Types>(args)...});
  }

  /**
   * @brief Balance auxiliar
   */
  void fill(std::vector<pair_t> &buffer, const long long int &begin,
            const long long int &end) {

    insert(buffer[(end + begin) / 2]);

    if (begin >= end)
      return;

    fill(buffer, begin, ((end + begin) / 2) - 1);
    fill(buffer, ((end + begin) / 2) + 1, end);
  }

  /**
   * @brief Balance
   * @details A very naive implementation
   */
  void balance() {
    if (!root)
      return;

    std::vector<pair_t> buffer;
    /* Load into a buffer */
    for (auto i : *(this)) {
      buffer.push_back(i);
    }

    /* Start reordering */
    clear();
    fill(buffer, 0, buffer.size() - 1);
  }
};

/* Iterator class */
template <typename KT, typename VT, typename CMP>
template <typename O>
class bst<KT, VT, CMP>::__iterator {

  using node = typename bst<KT, VT, CMP>::node;
  node *current, *past;

public:
  explicit __iterator(node *x) noexcept : current{x}, past{nullptr} {}

  using value_type = O;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;
  using reference = value_type &;
  using pointer = value_type *;

  reference operator*() const noexcept { return current->pair; }
  pointer operator->() const noexcept { return &(*(*this)); }

  /* Pre-increment */
  __iterator &operator++() noexcept {
    if (current) {
      if (current->right_child) {
        current = bst<KT, VT, CMP>::get_lower(current->right_child.get());
      } else {
        /* Climb to the main parent */
        node *parent = current->parent;
        if (parent) {
          node *right_child = parent->right_child.get();
          while (current == right_child) {
            current = current->parent;
            parent = current->parent;
            if (parent)
              right_child = parent->right_child.get();
          }
        }
        current = current->parent;
      }
    }
    return *this;
  }

  /* Post-increment */
  __iterator operator++(int) noexcept {
    __iterator tmp{current};
    ++(*this);
    return tmp;
  }

  friend bool operator==(const __iterator &a, const __iterator &b) {
    return a.current == b.current;
  }
  friend bool operator!=(const __iterator &a, const __iterator &b) {
    return !(a == b);
  }
};
