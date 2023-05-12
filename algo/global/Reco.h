/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_GLOBAL_RECO_H
#define CBM_ALGO_GLOBAL_RECO_H

#include "SubChain.h"

namespace fles
{
  class Timeslice;
}  // namespace fles

namespace cbm::algo
{
  class Options;

  class Reco : SubChain {
  public:
    Reco();
    ~Reco();

    Reco(const Reco&) = delete;
    Reco& operator=(const Reco&) = delete;
    Reco(Reco&&)                 = delete;
    Reco& operator=(Reco&&) = delete;

    void Init(const Options&);
    void Run();
    void Finalize();

  private:
    bool fInitialized = false;
    ChainContext fContext;
  };
}  // namespace cbm::algo

#endif
