/* Copyright (C) 2010-2014 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#ifndef Ca_CaSimdVc_H
#define Ca_CaSimdVc_H

#include "Vc/Vc"

namespace cbm::algo::ca
{

  typedef Vc::float_v fvec;
  typedef fvec::EntryType fscal;
  typedef Vc::float_m fmask;

#define _fvecalignment __attribute__((aligned(Vc::VectorAlignment)))

}  // namespace cbm::algo::ca

#endif
