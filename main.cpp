#include <cstdlib>
#include <functional>
#include <iostream>
#include <utility>

#include "src/bst.hpp"

int main(){
  bst<int, int> mytree;
  auto pair = std::make_pair(50,3);

  auto result = mytree.insert(pair);
  std::cout << std::get<1>(result) << std::endl;

  for(int i = 0; i < 10; ++i) {
    int key = rand() % 100;
    pair = std::make_pair(key,3);
    result = mytree.insert(pair);
    auto it = std::get<0>(result);
  }

  /* Print the tree */
  auto begin = mytree.begin();
  auto end = mytree.end();
  std::cout << "Next: " << std::get<0>(*begin) << std::endl;

  for (auto i : mytree) {
    std::cout << "Node: " << std::get<0>(i) << std::endl;
  }
  return 0;
}