/* Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: David Emschermann [committer] */

//
//   TRD pad layout for geometry v17o
//
// automatically generated by Create_TRD_Geometry_v17o_1e.C
// created 20170613
//

#ifndef CBMTRDPADS_H
#define CBMTRDPADS_H

Int_t fst1_sect_count = 3;
// array of pad geometries in the TRD (trd1mod[1-8])
// 8 modules  // 3 sectors  // 4 values
Float_t fst1_pad_type[8][3][4] =

  // module type 1
  // number of pads:  80 x 32 = 2560
  // pad size sector 1:  0.68 cm x  1.75 cm =  1.18 cm2
  // pad size sector 0:  0.68 cm x  1.50 cm =  1.01 cm2
  {{{54.0, 6.00, 54.0 / 80, 1.50}, {54.0, 42.00, 54.0 / 80, 1.75}, {54.0, 6.00, 54.0 / 80, 1.50}},

   // module type 2
   // number of pads:  80 x 16 = 1280
   // pad size sector 1:  0.68 cm x  3.50 cm =  2.36 cm2
   // pad size sector 0:  0.68 cm x  3.25 cm =  2.19 cm2
   {{54.0, 13.00, 54.0 / 80, 3.25}, {54.0, 28.00, 54.0 / 80, 3.50}, {54.0, 13.00, 54.0 / 80, 3.25}},

   // module type 3
   // number of pads:  80 x  8 =  640
   // pad size sector 1:  0.68 cm x  6.75 cm =  4.56 cm2
   // pad size sector 0:  0.68 cm x  6.75 cm =  4.56 cm2
   {{54.0, 13.50, 54.0 / 80, 6.75}, {54.0, 27.00, 54.0 / 80, 6.75}, {54.0, 13.50, 54.0 / 80, 6.75}},

   // module type 4
   // number of pads:  80 x  8 =  640
   // pad size sector 1:  0.68 cm x  6.75 cm =  4.56 cm2
   // pad size sector 0:  0.68 cm x  6.75 cm =  4.56 cm2
   {{54.0, 13.50, 54.0 / 80, 6.75}, {54.0, 27.00, 54.0 / 80, 6.75}, {54.0, 13.50, 54.0 / 80, 6.75}},

   //---

   // module type 5
   // number of pads: 128 x 12 = 1536
   // pad size sector 1:  0.68 cm x  7.25 cm =  4.93 cm2
   // pad size sector 0:  0.68 cm x  7.25 cm =  4.93 cm2
   {{87.0, 29.00, 87.0 / 128, 7.25}, {87.0, 29.00, 87.0 / 128, 7.25}, {87.0, 29.00, 87.0 / 128, 7.25}},

   // module type 6
   // number of pads: 128 x 12 = 1536
   // pad size sector 1:  0.68 cm x  7.25 cm =  4.93 cm2
   // pad size sector 0:  0.68 cm x  7.25 cm =  4.93 cm2
   {{87.0, 29.00, 87.0 / 128, 7.25}, {87.0, 29.00, 87.0 / 128, 7.25}, {87.0, 29.00, 87.0 / 128, 7.25}},

   // module type 7
   // number of pads: 128 x  4 =  512
   // pad size sector 1:  0.68 cm x 21.75 cm = 14.78 cm2
   // pad size sector 0:  0.68 cm x 21.75 cm = 14.78 cm2
   {{87.0, 21.75, 87.0 / 128, 21.75}, {87.0, 43.50, 87.0 / 128, 21.75}, {87.0, 21.75, 87.0 / 128, 21.75}},

   // module type 8
   // number of pads: 128 x  4 =  512
   // pad size sector 1:  0.68 cm x 21.75 cm = 14.78 cm2
   // pad size sector 0:  0.68 cm x 21.75 cm = 14.78 cm2
   {{87.0, 21.75, 87.0 / 128, 21.75}, {87.0, 43.50, 87.0 / 128, 21.75}, {87.0, 21.75, 87.0 / 128, 21.75}}};

#endif
