#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <utility>

//#define ENABLE_VERBOSE

#include "src/bst.hpp"
#include "src/time.hpp"

typedef bst<int, int>  bst_t;

template <typename T>
void benchmark();

int main(){
  std::cout << "*** Analysing BST ***" << std::endl;
  benchmark< bst_t >();
  std::cout << "*** Analysing std::map ***" << std::endl;
  benchmark< std::map<int, int> >();
  return 0;
}
/*
void benchmark()
{
  INIT_PROFILER(map_profiler);
  std::map<int, int> mytree, mytree1, mytree2;

  std::cout << "-- Insertion: copy" << std::endl;
  START_PROFILE(insertion_copy, map_profiler, 10)
  int key = rand() % 100;
  auto pair = std::make_pair(key,key);
  
#ifdef ENABLE_VERBOSE
  auto result = mytree1.insert(pair);
  std::cout << "Inserting -> " << key << " Result: "
  << std::get<1>(result) << std::endl;
#else
  mytree1.insert(pair);
#endif
  END_PROFILE(insertion_copy)

  std::cout << "*** std::map ***" <<  map_profiler << std::endl;
}
*/
template <typename T>
void benchmark()
{
  /*
    Perform benchmark of the Custom BST
  */
  INIT_PROFILER(bst_profiler);
  T mytree, mytree1, mytree2;

  /* Insertion */
  std::cout << "-- Insertion: copy" << std::endl;
  START_PROFILE(insertion_copy, bst_profiler, 10)
  int key = rand() % 100;
  auto pair = std::make_pair(key,key);
  
#ifdef ENABLE_VERBOSE
  auto result = mytree1.insert(pair);
  std::cout << "Inserting -> " << key << " Result: "
  << std::get<1>(result) << std::endl;
#else
  mytree1.insert(pair);
#endif
  END_PROFILE(insertion_copy)

  std::cout << "-- Insertion: move" << std::endl;
  START_PROFILE(insertion_move, bst_profiler, 10)
  int key = rand() % 100;
#ifdef ENABLE_VERBOSE
  auto result = mytree2.insert(std::make_pair(key,key));
  std::cout << "Inserting -> " << key << " Result: "
  << std::get<1>(result) << std::endl;
#else
  mytree2.insert(std::make_pair(key,key));
#endif
  END_PROFILE(insertion_move)

  std::cout << "-- Emplace" << std::endl;
  START_PROFILE(emplace, bst_profiler, 10)
  int key = rand() % 100;
#ifdef ENABLE_VERBOSE
  auto result = mytree.emplace(key,key);
  std::cout << "Emplacing -> " << key << " Result: "
  << std::get<1>(result) << std::endl;
#else
  mytree.emplace(key,key);
#endif
  END_PROFILE(emplace)
  
  std::cout << "-- Clear" << std::endl;
  START_PROFILE(clear, bst_profiler, 10)
  mytree2.clear();
  END_PROFILE(clear)

  std::cout << "-- Find unbalanced" << std::endl;
  START_PROFILE(find_unbalanced, bst_profiler, 10)
  int key = rand() % 100;
  
#ifdef ENABLE_VERBOSE
  auto it = mytree1.find(key);
  std::cout << "Finding -> " << key << " Result: "
  << (it == mytree.end()) << std::endl;
#else
  mytree1.find(key);
#endif
  END_PROFILE(find_unbalanced)

  if constexpr(std::is_same_v<T, bst_t>){
    std::cout << "-- Find balanced" << std::endl;
    mytree1.balance();
    START_PROFILE(find_balanced, bst_profiler, 10)
    int key = rand() % 100;
  
#ifdef ENABLE_VERBOSE
    auto it = mytree1.find(key);
    std::cout << "Finding -> " << key << " Result: "
    << (it == mytree1.end()) << std::endl;
#else
    mytree1.find(key);
#endif
    END_PROFILE(find_balanced)
  }

  std::cout << "-- Subscription: retrieve" << std::endl;
  START_PROFILE(suscription_retrieve, bst_profiler, 10)
  int key = rand() % 100;
#ifdef ENABLE_VERBOSE
  auto result = mytree[key];
  std::cout << "Retrieving -> " << key << " Result: "
  << result << std::endl;
#else
  mytree[key];
#endif
  END_PROFILE(suscription_retrieve)

  std::cout << "-- Subscription: setting" << std::endl;
  START_PROFILE(subscription_setting, bst_profiler, 10)
  int key = rand() % 100;
  mytree[key] = key;
#ifdef ENABLE_VERBOSE
  std::cout << "Setting -> " << key << " Result: "
  << key << std::endl;
#endif
  END_PROFILE(subscription_setting)

  const int iter_semantics = 10;

  std::cout << "-- Copy semantics: assignment" << std::endl;

  START_PROFILE(copy_assignmet, bst_profiler, iter_semantics)
  T tree_test = ((i % 2 == 0) ? mytree : mytree1);
#ifdef ENABLE_VERBOSE
  std::cout << "Copying -> Tree " << (i % 2) << "\n" <<
  ((i % 2 == 0) ? mytree : mytree1) <<std::endl;
  std::cout << "Copied: " << tree_test << std::endl;
#endif
  END_PROFILE(copy_assignmet)

  std::cout << "-- Copy semantics: construction" << std::endl;
  START_PROFILE(copy_construction, bst_profiler, iter_semantics)
  if (i % 2 == 0) T tree_test{mytree};
  if (i % 2 != 0) T tree_test{mytree1};
  
#ifdef ENABLE_VERBOSE
  std::cout << "Copying -> Tree " << (i % 2) << "\n"
  << ((i % 2 == 0) ? mytree : mytree1) << std::endl;
#endif
  END_PROFILE(copy_construction)

  std::cout << "-- Move semantics: assignment" << std::endl;
  GET_PROFILE_INSTANCE(move_assignment, bst_profiler)
  for(auto i = 0; i < 20; i++){
#ifdef ENABLE_VERBOSE
    std::cout << "Creating new tree ... " << std::endl;
#endif
    T test;
    for(int i = 0; i < 20; ++i) {
      int key = rand() % 100;
      test.insert(std::make_pair(key,key));
    }
#ifdef ENABLE_VERBOSE
    std::cout << "Moving tree ... " << std::endl;
    std::cout << "Original: " << test << std::endl;
#endif
    move_assignment->reset();
    mytree2 = std::move(test);
    move_assignment->tick();
#ifdef ENABLE_VERBOSE
    std::cout << "Moved: " << mytree2 << std::endl;
#endif
  }

  std::cout << "-- Move semantics: construction" << std::endl;
  GET_PROFILE_INSTANCE(move_construction, bst_profiler)
  for(auto i = 0; i < 20; i++){
#ifdef ENABLE_VERBOSE
    std::cout << "Creating new tree ... " << std::endl;
#endif
    T test;
    for(int i = 0; i < 20; ++i) {
      int key = rand() % 100;
      test.insert(std::make_pair(key,key));
    }
#ifdef ENABLE_VERBOSE
    std::cout << "Moving tree ... " << std::endl;
    std::cout << "Original: " << test << std::endl;
#endif
    move_construction->reset();
    T test_mv{std::move(test)};
    move_construction->tick();
#ifdef ENABLE_VERBOSE
    std::cout << "Moved: " << test_mv << std::endl;
#endif
  }
  
  std::cout << "*** Results ***\n" << bst_profiler << std::endl;
}