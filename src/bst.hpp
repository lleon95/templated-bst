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

    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;  
  };
  
  /**
   * @brief Pointer the root node
   */
  std::unique_ptr<node> root;
};


