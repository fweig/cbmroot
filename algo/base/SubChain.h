/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_SUBCHAIN_H
#define CBM_ALGO_BASE_SUBCHAIN_H

#include <gsl/pointers>

#include "ChainContext.h"

namespace cbm::algo
{
  class SubChain {

  public:
    void SetContext(ChainContext* ctx) { fContext = ctx; }

    const Options& Opts() const { return gsl::make_not_null(fContext)->opts; }
    const RecoParams& Params() const { return gsl::make_not_null(fContext)->recoParams; }

  private:
    ChainContext* fContext = nullptr;
  };
}  // namespace cbm::algo

#endif
