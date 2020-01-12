#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <utility>

#include "src/bst.hpp"
#include "src/time.hpp"

/* Switches */
/* #define ENABLE_VERBOSE */ /* Enable this when you want to see the results */
#define ENABLE_PROFILE

typedef bst<int, int> bst_t;
unsigned int number_elements = 100;
unsigned int number_iters = 100;
unsigned int number_iter_semantics = 100;

template <typename T> void benchmark();

int main(int argc, char **argv) {
  if (argc >= 2) {
    std::istringstream incoming{argv[1]};
    incoming >> number_elements;
  }
  if (argc >= 3) {
    std::istringstream incoming{argv[2]};
    incoming >> number_iters;
  }
  if (argc >= 4) {
    std::istringstream incoming{argv[3]};
    incoming >> number_iter_semantics;
  }
#ifdef ENABLE_PROFILE

  std::cout << "*** Analysing BST ***" << std::endl;
  benchmark<bst_t>();
#ifndef ENABLE_VERBOSE
  std::cout << "*** Analysing std::map ***" << std::endl;
  benchmark<std::map<int, int>>();
#endif

#else
  bst_t mytree;
  /* Create a tree */
  std::cout << "Creating tree: ";
  for (size_t i{0}; i < number_elements; ++i) {
    int key = rand() % (number_elements * 100);
    auto pair = std::make_pair(key, key);
    std::cout << key << " ";
    mytree.insert(pair);
  }
  std::cout << std::endl;
  /* Print */
  std::cout << mytree << std::endl;
  bst_t mytree2 = mytree;
  /* Print */
  mytree2.balance();
  std::cout << mytree2 << std::endl;

  INIT_PROFILER(bst_profiler);
  START_PROFILE(last, bst_profiler, number_elements)
  usleep(1000000);
  END_PROFILE(last)
  std::cout << bst_profiler << std::endl;

#endif
  return 0;
}

template <typename T> void benchmark() {
  /*
    Perform benchmark of the Custom BST
  */
  INIT_PROFILER(bst_profiler);
  T mytree, mytree1, mytree2;

  /* Insertion */
  std::cout << "-- Insertion: copy" << std::endl;
  START_PROFILE(insertion_copy, bst_profiler, number_elements)
  int key = rand() % (number_elements * 100);
  auto pair = std::make_pair(key, key);

#ifdef ENABLE_VERBOSE
  auto result = mytree1.insert(pair);
  std::cout << "Inserting -> " << key << " Result: " << std::get<1>(result)
            << std::endl;
#else
  mytree1.insert(pair);
#endif
  END_PROFILE(insertion_copy)

  std::cout << "-- Insertion: move" << std::endl;
  START_PROFILE(insertion_move, bst_profiler, number_elements)
  int key = rand() % (number_elements * 100);
#ifdef ENABLE_VERBOSE
  auto result = mytree2.insert(std::make_pair(key, key));
  std::cout << "Inserting -> " << key << " Result: " << std::get<1>(result)
            << std::endl;
#else
  mytree2.insert(std::make_pair(key, key));
#endif
  END_PROFILE(insertion_move)

  std::cout << "-- Emplace" << std::endl;
  START_PROFILE(emplace, bst_profiler, number_elements)
  int key = rand() % (number_elements * 100);
#ifdef ENABLE_VERBOSE
  auto result = mytree.emplace(key, key);
  std::cout << "Emplacing -> " << key << " Result: " << std::get<1>(result)
            << std::endl;
#else
  mytree.emplace(key, key);
#endif
  END_PROFILE(emplace)

  std::cout << "-- Clear" << std::endl;
  START_PROFILE(clear, bst_profiler, number_iters)
  mytree2.clear();
  END_PROFILE(clear)

  std::cout << "-- Find unbalanced" << std::endl;
  START_PROFILE(find_unbalanced, bst_profiler, number_iters)
  int key = rand() % (number_elements * 100);

#ifdef ENABLE_VERBOSE
  auto it = mytree1.find(key);
  std::cout << "Finding -> " << key << " Result: " << (it == mytree.end())
            << std::endl;
#else
  mytree1.find(key);
#endif
  END_PROFILE(find_unbalanced)

  if constexpr (std::is_same<T, bst_t>::value) {
    std::cout << "-- Find balanced" << std::endl;
    mytree1.balance();
    START_PROFILE(find_balanced, bst_profiler, number_iters)
    int key = rand() % (number_elements * 100);

#ifdef ENABLE_VERBOSE
    auto it = mytree1.find(key);
    std::cout << "Finding -> " << key << " Result: " << (it == mytree1.end())
              << std::endl;
#else
    mytree1.find(key);
#endif
    END_PROFILE(find_balanced)
  }

  std::cout << "-- Subscription: retrieve" << std::endl;
  START_PROFILE(suscription_retrieve, bst_profiler, number_iters)
  int key = rand() % (number_elements * 100);
#ifdef ENABLE_VERBOSE
  auto result = mytree[key];
  std::cout << "Retrieving -> " << key << " Result: " << result << std::endl;
#else
  mytree1[key];
#endif
  END_PROFILE(suscription_retrieve)

  std::cout << "-- Subscription: setting" << std::endl;
  START_PROFILE(subscription_setting, bst_profiler, number_iters)
  int key = rand() % (number_elements * 100);
  mytree1[key] = key;
#ifdef ENABLE_VERBOSE
  std::cout << "Setting -> " << key << " Result: " << key << std::endl;
#endif
  END_PROFILE(subscription_setting)

  std::cout << "-- Copy semantics: assignment" << std::endl;

  START_PROFILE(copy_assignment, bst_profiler, number_iter_semantics)
  T tree_test = ((i % 2 == 0) ? mytree : mytree1);
#ifdef ENABLE_VERBOSE
  std::cout << "Copying -> Tree " << (i % 2) << "\n"
            << ((i % 2 == 0) ? mytree : mytree1) << std::endl;
  std::cout << "Copied: " << tree_test << std::endl;
#endif
  END_PROFILE(copy_assignment)

  std::cout << "-- Copy semantics: construction" << std::endl;
  START_PROFILE(copy_construction, bst_profiler, number_iter_semantics)
  if (i % 2 == 0)
    T tree_test{mytree};
  if (i % 2 != 0)
    T tree_test{mytree1};

#ifdef ENABLE_VERBOSE
  std::cout << "Copying -> Tree " << (i % 2) << "\n"
            << ((i % 2 == 0) ? mytree : mytree1) << std::endl;
#endif
  END_PROFILE(copy_construction)

  std::cout << "-- Move semantics: assignment" << std::endl;
  GET_PROFILE_INSTANCE(move_assignment, bst_profiler)
  for (unsigned int i = 0; i < number_iter_semantics; i++) {
#ifdef ENABLE_VERBOSE
    std::cout << "Creating new tree ... " << std::endl;
#endif
    T test;
    for (unsigned int i = 0; i < number_elements; ++i) {
      int key = rand() % (number_elements * 100);
      test.insert(std::make_pair(key, key));
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
  for (unsigned int i = 0; i < number_iter_semantics; i++) {
#ifdef ENABLE_VERBOSE
    std::cout << "Creating new tree ... " << std::endl;
#endif
    T test;
    for (unsigned i = 0; i < number_elements; ++i) {
      int key = rand() % (number_elements * 100);
      test.insert(std::make_pair(key, key));
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