#include <cstdlib>
#include <functional>
#include <iostream>
#include <utility>

#include "src/bst.hpp"

int main(){
  bst<int, int> mytree;
  auto pair = std::make_pair(50,3);

  auto result = mytree.insert(std::make_pair(50,3));
  std::cout << std::get<1>(result) << std::endl;

  for(int i = 0; i < 10; ++i) {
    int key = rand() % 100;
    pair = std::make_pair(key,3);
    result = mytree.insert(pair);
  }

  /* Print the tree */
  for (auto i : mytree) {
    std::cout << "Node: " << std::get<0>(i) << std::endl;
  }
  

  /* Look for a key */
  auto elem = mytree.find(16);
  auto end = mytree.end();
  if (elem == end) {
    std::cout << "Holder: empty" << std::endl;
  } else {
    std::cout << "Holder: " << std::get<0>(*elem) << "," << std::get<1>(*elem) << std::endl;
  }

  /* Test the subscriptor */
  mytree[15] = 5;
  std::cout << mytree[1] << std::endl;

  /* Test the emplace */
  mytree.emplace(100,2);
  std::cout << "Ostream \n" << mytree << std::endl;

  /* Clear */
  mytree.clear();
  /* Print the tree */
  for (auto i : mytree) {
    std::cout << "Node: " << std::get<0>(i) << std::endl;
  }

  return 0;
}