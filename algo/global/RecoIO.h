/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_GLOBAL_RECOIO_H
#define CBM_ALGO_GLOBAL_RECOIO_H

#include <optional>
#include <vector>

// Forward declarations
class CbmStsDigi;
namespace cbm::algo::sts
{
  struct Cluster;
  struct Hit;
  using Digi = CbmStsDigi;
}  // namespace cbm::algo::sts

namespace cbm::algo
{

  /**
   * @brief Class to hold the input and output of the reconstruction
   *
   * This class is used to hold the input and output of the reconstruction.
   * All values are stored as optional vectors. If a value is empty the corresponding reconstruction step was skipped.
   * If the vector is empty instead, the reconstruction step was performed but no results were found.
   *
   * Implementation note: RecoIO constructor and destructor are defined in RecoIO.cxx to allow forward declarations of all used classes.
   * Keeps this header lean and avoids introducing unnecessary dependencies.
   */
  class RecoIO {
  public:
    template<typename T>
    using MaybeVector = std::optional<std::vector<T>>;

    RecoIO();
    ~RecoIO();

    MaybeVector<sts::Digi>& StsDigis() { return fStsDigis; }
    const MaybeVector<sts::Digi>& StsDigis() const { return fStsDigis; }

    MaybeVector<sts::Cluster>& StsClusters() { return fStsClusters; }
    const MaybeVector<sts::Cluster>& StsClusters() const { return fStsClusters; }

    MaybeVector<sts::Hit>& StsHits() { return fStsHits; }
    const MaybeVector<sts::Hit>& StsHits() const { return fStsHits; }

  private:
    MaybeVector<sts::Digi> fStsDigis;
    MaybeVector<sts::Cluster> fStsClusters;
    MaybeVector<sts::Hit> fStsHits;
  };

}  // namespace cbm::algo

#endif
