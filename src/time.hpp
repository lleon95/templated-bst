/**
 *  Templated BST - profiling instrumentation
 *  Luis Leon V.
 */

/**
 * @file time.hpp
 */
#pragma once
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define str(s) #s

typedef std::chrono::high_resolution_clock::time_point time_point_t;
#define INIT_PROFILER(NAME)                                   \
    profiler (NAME){};
#define GET_PROFILE_INSTANCE(NAME, PROFILER)  \
    profile_node * NAME = (PROFILER).create(str(NAME));
#define START_PROFILE(NAME, PROFILER, ITER)                   \
    profile_node * NAME = (PROFILER).create(str(NAME));          \
    for(size_t i{0}; i < (ITER); ++i) {     
#define END_PROFILE(NAME)                                     \
      (NAME)->tick();                                          \
    }

/* Profiler instance */
class profile_node {
  std::string name;
  double average;
  size_t counter;
  time_point_t tlast;

public:
  profile_node(std::string name_) : name{name_}, average{0}, counter{0},
  tlast{std::chrono::high_resolution_clock::now()} {}
  void tick() {
    time_point_t tnow = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = 
      std::chrono::duration_cast<std::chrono::duration<double>>(tnow - tlast);
    average = (average * counter + time_span.count());
    ++counter;
    average /= counter;
    tlast = tnow;
  }
  void reset(){
    tlast = std::chrono::high_resolution_clock::now();
  }
  friend
  std::ostream& operator<<(std::ostream& os, const profile_node& pn){
    os << "-- " << pn.name << " --" << " (AVG: " << pn.average << ", IT:" 
    << pn.counter << ")";
    return os;
  }
};

class profiler{
  std::vector<std::unique_ptr<profile_node>> profilers;
public:
  /* Default */
  profiler() {};
  
  /* Create a new instance */
  profile_node * create(std::string name){
    profilers.push_back(std::make_unique<profile_node>(name));
    return profilers[profilers.size()-1].get();
  }
  /* Print instance */
  friend
  std::ostream& operator<<(std::ostream& os, const profiler& p){
    os << "Printing profile results: \n";
    for(size_t i{0}; i < p.profilers.size(); ++i){
      os << *(p.profilers[i].get()) << "\n";
    }
    return os;
  }
};



