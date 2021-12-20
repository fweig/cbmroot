/* Copyright (C) 2016-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/************************************************************************************************************
 * @file L1Parameters.h
 * @brief Parameter container for the L1Algo library
 * @since 19.12.2021
 *
 ***********************************************************************************************************/

#ifndef L1Parameters_h
#define L1Parameters_h 1

class L1Parameters {
public:
  ///-----------------------------------------------------------------------------------------------------///
  ///   Array sizes                                                                                       ///
  ///-----------------------------------------------------------------------------------------------------///
  // Amount of elements in cx, cy and cz coefficient arrays of L1FieldSlice
  static constexpr int kN_FS_COEFFS {21}; // TODO: May be we should return the order of polynomial?

};

#endif // L1Parameters_h
