#include <cstdlib>
#include <functional>
#include <iostream>
#include <unistd.h>
#include <utility>

#define ENABLE_VERBOSE

#include "src/bst.hpp"
#include "src/time.hpp"

int main(){
  INIT_PROFILER(bst_profiler);

  /* Performing operations with custom_made bst */
  bst<int, int> mytree, mytree1, mytree2;

  /* Insertion */
  std::cout << "-- Insertion: copy" << std::endl;
  START_PROFILE(insertion_copy, bst_profiler, 10)
  int key = rand() % 100;
  auto pair = std::make_pair(key,key);
  auto result = mytree1.insert(pair);
#ifdef ENABLE_VERBOSE
  std::cout << "Inserting -> " << key << " Result: "
  << std::get<1>(result) << std::endl;
#endif
  END_PROFILE(insertion_copy)

  std::cout << "-- Insertion: move" << std::endl;
  START_PROFILE(insertion_move, bst_profiler, 10)
  int key = rand() % 100;
  auto result = mytree2.insert(std::make_pair(key,key));
#ifdef ENABLE_VERBOSE
  std::cout << "Inserting -> " << key << " Result: "
  << std::get<1>(result) << std::endl;
#endif
  END_PROFILE(insertion_move)

  std::cout << "-- Emplace" << std::endl;
  START_PROFILE(emplace, bst_profiler, 10)
  int key = rand() % 100;
  auto result = mytree.emplace(key,key);
#ifdef ENABLE_VERBOSE
  std::cout << "Emplacing -> " << key << " Result: "
  << std::get<1>(result) << std::endl;
#endif
  END_PROFILE(emplace)
  
  std::cout << "-- Clear" << std::endl;
  START_PROFILE(clear, bst_profiler, 10)
  mytree2.clear();
  END_PROFILE(clear)

  std::cout << "-- Find" << std::endl;
  START_PROFILE(find, bst_profiler, 10)
  int key = rand() % 100;
  auto it = mytree1.find(key);
#ifdef ENABLE_VERBOSE
  std::cout << "Finding -> " << key << " Result: "
  << (it == mytree.end()) << std::endl;
#endif
  END_PROFILE(find)

  std::cout << "-- Balance" << std::endl;
  GET_PROFILE_INSTANCE(balancing, bst_profiler)
  for(auto i = 0; i < 20; i++){
#ifdef ENABLE_VERBOSE
    std::cout << "Creating new tree ... " << std::endl;
#endif
    for(int i = 0; i < 20; ++i) {
      int key = rand() % 100;
      mytree2.insert(std::make_pair(key,key));
    }
#ifdef ENABLE_VERBOSE
    std::cout << "Balancing tree ... " << std::endl;
#endif
    balancing->reset();
    mytree2.balance();
    balancing->tick();
  }

  std::cout << "-- Subscription: retrieve" << std::endl;
  START_PROFILE(suscription_retrieve, bst_profiler, 10)
  int key = rand() % 100;
  auto result = mytree[key];
#ifdef ENABLE_VERBOSE
  std::cout << "Retrieving -> " << key << " Result: "
  << result << std::endl;
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
/*
  std::cout << "-- Copy semantics: assignment" << std::endl;
  
  std::vector<bst<int,int>> trees;

  START_PROFILE(copy_assignmet, bst_profiler, iter_semantics)
  trees[i] = ((i % 2 == 0) ? mytree : mytree1);
#ifdef ENABLE_VERBOSE
  std::cout << "Copying -> Tree " << (i % 2) << "\n" <<
  ((i % 2 == 0) ? mytree : mytree1) <<std::endl;
  std::cout << "Copied: " << trees[i] << std::endl;
#endif
  END_PROFILE(copy_assignmet)
*/
  std::cout << "-- Copy semantics: construction" << std::endl;
  START_PROFILE(copy_construction, bst_profiler, iter_semantics)
  if (i % 2 == 0) bst<int,int> tree_test{mytree};
  if (i % 2 != 0) bst<int,int> tree_test{mytree1};
  
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
    bst<int,int> test;
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
    bst<int,int> test;
    for(int i = 0; i < 20; ++i) {
      int key = rand() % 100;
      test.insert(std::make_pair(key,key));
    }
#ifdef ENABLE_VERBOSE
    std::cout << "Moving tree ... " << std::endl;
    std::cout << "Original: " << test << std::endl;
#endif
    move_construction->reset();
    bst<int,int> test_mv{std::move(test)};
    move_construction->tick();
#ifdef ENABLE_VERBOSE
    std::cout << "Moved: " << test_mv << std::endl;
#endif
  }
  
  std::cout << bst_profiler << std::endl;
  return 0;
}