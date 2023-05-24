/* Copyright (C) 2023 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergei Zharko [committer] */

/// @file    CaAlgoRandom.h
/// @brief   Random generator utility class for CA tracking
/// @since   16.05.2023
/// @authors Sergei Zharko <s.zharko@gsi.de>, Sergey Gorbunov <se.gorbunov@gsi.de>

#ifndef CaAlgoRandom_h
#define CaAlgoRandom_h 1

#include <cstdint>
#include <random>
#include <type_traits>

namespace ca::algo
{
  /// @brief A class, providing ROOT-free access to randomly generated variables
  class Random {
    using GeneratorType_t = std::mt19937_64;

  public:
    /// @brief  Default constructor
    ///
    /// Sets seed equal to 1
    Random();

    /// @brief  Constructor
    /// @param  seed  A random seed
    ///
    /// If 0 is selected for the random seed, the generator is seeded with std::random_device
    Random(int seed);

    /// @brief  Destructor
    ~Random() = default;

    /// @brief  Copy constructor
    Random(const Random& other) = delete;

    /// @brief  Move constructor
    Random(Random&& other) = delete;

    /// @brief  Copy assignment operator
    Random& operator=(const Random&) = delete;

    /// @brief  Move assignment operator
    Random& operator=(Random&&) = delete;

    /// @brief  Gets seed of the random generator
    int GetSeed() const { return fSeed; }

    /// @brief  Sets seed to the random generator
    /// @param  seed  A random seed
    ///
    /// If 0 is selected for the random seed, the generator is seeded with std::random_device
    void SetSeed(int seed);

    /// @brief  Returns a normally distributed random value, limited within a selected sigma range
    /// @tparam T        Type of floating point
    /// @param  mean     Mean of the distribution
    /// @param  sigma    Sigma of the distribution
    /// @param  nSigmas  Half-width of the generated numbers domain, expressed in number of sigmas
    template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
    T BoundedGaus(const T& mean, const T& sigma, const T& nSigmas) const;

  private:
    mutable GeneratorType_t fGenerator;  ///< Random number generator
    unsigned int fSeed = 1;              ///< Random number seed
  };
}  // namespace ca::algo


// *********************************************************
// **     Template and inline function implementation     **
// *********************************************************

// ---------------------------------------------------------------------------------------------------------------------
//
template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool>>
T ca::algo::Random::BoundedGaus(const T& mean, const T& sigma, const T& nSigmas) const
{
  assert(sigma > 0 && std::isfinite(sigma));
  assert(nSigmas > 0 && std::isfinite(nSigmas));
  std::normal_distribution rndGaus {mean, sigma};
  double res = 0;
  do {
    res = rndGaus(fGenerator);
  } while (std::fabs(res - mean) > nSigmas * sigma);
  return res;
}


#endif  // CaAlgoRandom_h
