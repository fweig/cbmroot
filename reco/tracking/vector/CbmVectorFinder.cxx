/** @file CbmVectorFinder.cxx
  * @copyright Copyright (C) 2017-2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Alexander Zinchenko [committer] **/

/** CbmVectorFinder.cxx
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2017
 **/
#include "CbmVectorFinder.h"

#include "CbmDefs.h"
#include "CbmMuchFindVectors.h"
#include "CbmMuchFindVectorsGem.h"
#include "CbmMuchMergeVectors.h"
#include "CbmMuchToTrdVectors.h"
#include "CbmSetup.h"
#include "CbmTrdFindVectors.h"
#include "CbmTrdToTofVector.h"
//#include "CbmMuchToTofVectors.h"

#include <iostream>

using std::cout;
using std::endl;
//using std::map;

// -----   Default constructor   -------------------------------------------
CbmVectorFinder::CbmVectorFinder() : FairTask("VectorFinder") {}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
CbmVectorFinder::~CbmVectorFinder() {}
// -------------------------------------------------------------------------

// -----   Public method Init (abstract in base class)  --------------------
InitStatus CbmVectorFinder::Init()
{
  //cout << " !!!!!!!!! AZ " << CbmSetup::Instance()->IsActive(kTrd) << endl;
  //cout << " !!!!!!!!! AZ " << CbmSetup::Instance()->IsActive(kRich) << endl;
  if (CbmSetup::Instance()->IsActive(kTrd)) {
    FairTask* trdVecf = new CbmTrdFindVectors();
    Add(trdVecf);
    if (CbmSetup::Instance()->IsActive(kTof)) {
      FairTask* trd2tof = new CbmTrdToTofVector();
      Add(trd2tof);
    }
  }

  if (CbmSetup::Instance()->IsActive(kMuch)) {
    FairTask* muchVecf = new CbmMuchFindVectors();
    Add(muchVecf);
    FairTask* muchVecGem = new CbmMuchFindVectorsGem();
    Add(muchVecGem);
    FairTask* muchMerge = new CbmMuchMergeVectors();
    Add(muchMerge);
    if (CbmSetup::Instance()->IsActive(kTrd)) {
      // MUCH-to-TRD merging
      FairTask* muchTrd = new CbmMuchToTrdVectors();
      Add(muchTrd);
    }
    /*
    if (CbmSetup::Instance()->IsActive(kTof)) {
      // MUCH-to-TOF merging
      FairTask *muchTof = new CbmMuchToTofVectors();
      Add(muchTof);
    }
    */
  }
}
// -------------------------------------------------------------------------

// -----  SetParContainers -------------------------------------------------
void CbmVectorFinder::SetParContainers() {}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void CbmVectorFinder::Exec(Option_t* opt) {}
// -------------------------------------------------------------------------

// -----   Public method Finish   ------------------------------------------
void CbmVectorFinder::Finish() {}
// -------------------------------------------------------------------------

ClassImp(CbmVectorFinder);
