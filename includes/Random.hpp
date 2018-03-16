//
// Created by mateus558 on 25/08/17.
//

#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

/*! Random namespace
   \file Random.hpp
   \author Mateus Coutinho Marim
*/

#include <random>
#include <functional>

namespace Random {
  std::mt19937 m_gen;
  unsigned int m_seed;

  auto init(unsigned int seed = 666) {
      m_seed = (seed == 666)?std::random_device {} (): seed;
      m_gen.seed(m_seed);

      return m_seed;
  }

  int intInRange(int low, int high) {
      std::uniform_int_distribution<int> dist(low, high);

      return dist(m_gen);
  }

  float floatInRange(float low, float high) {
      std::uniform_real_distribution<float> dist(low, high);

      return dist(m_gen);
  }

  auto getSeed(){
      return m_seed;
  }

}


#endif //DONKEY_KONG_TRAB_RANDOM_H
