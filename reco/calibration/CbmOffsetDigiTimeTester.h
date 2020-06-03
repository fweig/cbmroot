// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                    CbmOffsetDigiTimeTester                        -----
// -----               Created 18.02.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMOFFSETDIGITIMETESTER_H
#define CBMOFFSETDIGITIMETESTER_H

/// CbmRoot (+externals) headers
#include "CbmOffsetDigiTime.h"
#include "CbmStsDigi.h"
#include "CbmTofDigi.h"
#include "CbmMuchBeamTimeDigi.h"

/// FairRoot headers

/// Fairsoft (Root, Boost, ...) headers

/// C/C++ headers


class CbmOffsetDigiTimeSts : public CbmOffsetDigiTime< CbmStsDigi >
{
   public:
      CbmOffsetDigiTimeSts( TString sDigiBranchName, TString sDigiCalBranchName = "", TString sDigiCalBranchDescr = "" ) :
         CbmOffsetDigiTime< CbmStsDigi >( sDigiBranchName, sDigiCalBranchName, sDigiCalBranchDescr )
         {};

      ~CbmOffsetDigiTimeSts(){};

   ClassDef( CbmOffsetDigiTimeSts, 0 );
};

class CbmOffsetDigiTimeTof : public CbmOffsetDigiTime< CbmTofDigi >
{
   public:

      CbmOffsetDigiTimeTof( TString sDigiBranchName, TString sDigiCalBranchName = "", TString sDigiCalBranchDescr = "" ) :
         CbmOffsetDigiTime< CbmTofDigi >( sDigiBranchName, sDigiCalBranchName, sDigiCalBranchDescr )
         {};

      ~CbmOffsetDigiTimeTof(){};

   ClassDef( CbmOffsetDigiTimeTof, 0 );
};

class CbmOffsetDigiTimeMuch : public CbmOffsetDigiTime< CbmMuchBeamTimeDigi >
{
   public:

      CbmOffsetDigiTimeMuch( TString sDigiBranchName, TString sDigiCalBranchName = "", TString sDigiCalBranchDescr = "" ) :
         CbmOffsetDigiTime< CbmMuchBeamTimeDigi >( sDigiBranchName, sDigiCalBranchName, sDigiCalBranchDescr )
         {};

      ~CbmOffsetDigiTimeMuch(){};

   ClassDef( CbmOffsetDigiTimeMuch, 0 );
};

class CbmOffsetDigiTimeTester
{
   public:

      CbmOffsetDigiTimeTester();
      ~CbmOffsetDigiTimeTester();

};

#endif // CBMOFFSETDIGITIMETESTER_H
