/** @file cbmrootlibs.C
  * @copyright Copyright (C) 2008-2012 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer] **/

/**
 * \file cbmrootlibs.C
 * \brief Macro loads CBMROOT libraries.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 */

void cbmrootlibs()
{
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libCbmBase");
  gSystem->Load("libCbmData");
  gSystem->Load("libCbmGenerators");
  gSystem->Load("libField");
  gSystem->Load("libGen");
  gSystem->Load("libPassive");
  gSystem->Load("libTrkBase");
  gSystem->Load("libGeane");

  gSystem->Load("libSts");
  gSystem->Load("libTrd");
  gSystem->Load("libEcal");

  gSystem->Load("libKF");
  gSystem->Load("libL1");
  gSystem->Load("libMvd");

  gSystem->Load("libRich");
  gSystem->Load("libMuch");

  gSystem->Load("libTof");

  gSystem->Load("libGlobal");
  //   gSystem->Load("libcudalittrack");
  gSystem->Load("libLittrack");
  gSystem->Load("libLittrackparallel");
  gSystem->Load("libMinuit2");  // Needed for field fitter
  gSystem->Load("libClustering");
}
