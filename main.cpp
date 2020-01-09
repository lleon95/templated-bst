#include <cstdlib>
#include <functional>
#include <iostream>
#include <utility>

#include "src/bst.hpp"

int main(){
  bst<int, int> mytree;
  auto pair = std::make_pair(2,3);

  auto result = mytree.insert(pair);
  std::cout << std::get<1>(result) << std::endl;

  for(int i = 0; i < 10; ++i) {
    int key = rand() % 100;
    pair = std::make_pair(key,3);
    result = mytree.insert(pair);
  }
  return 0;
}