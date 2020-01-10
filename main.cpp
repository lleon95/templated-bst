#include <cstdlib>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <utility>


#include "src/bst.hpp"
#include "src/time.hpp"

int main(){
  bst<int, int> mytree;
  auto pair = std::make_pair(50,3);

  INIT_PROFILER(bst_profiler);

  START_PROFILE(dummy, bst_profiler, 10)
    usleep(1000);
  END_PROFILE(dummy)

  std::cout << bst_profiler << std::endl;

  auto result = mytree.insert(std::make_pair(50,3));
  std::cout << std::get<1>(result) << std::endl;

  for(int i = 0; i < 20; ++i) {
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

  /* Testing move and copy semantics */
  bst<int, int> mytree_2cpassign = mytree;
  bst<int, int> mytree_2cpconst{mytree};
  bst<int, int> mytree_2cpconstmv{std::move(mytree_2cpconst)};
  bst<int, int> mytree_2mvassign = std::move(mytree_2cpassign);

  /* Clear */
  mytree.clear();
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
  elem = mytree.find(59);
  end = mytree.end();
  if (elem == end) {
    std::cout << "Holder: empty" << std::endl;
  } else {
    std::cout << "Holder: " << std::get<0>(*elem) << "," << std::get<1>(*elem) << std::endl;
  }

  /* Test others */
  std::cout << "Ostream mytree \n" << mytree << std::endl;
  std::cout << "Ostream mytree_2cpassign \n" << mytree_2mvassign << std::endl;
  std::cout << "Ostream mytree_2cpconst \n" << mytree_2cpconstmv << std::endl;
  mytree_2mvassign.balance();
  std::cout << "Ostream mytree_2cpassign \n" << mytree_2mvassign << std::endl;
  return 0;
}