// ------------------------------------------------------------------
// -----               CbmTofClusterizersDef                    -----
// -----              Created 30/04/2015 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef CBMTOFCLUSTERIZERSDEF_H_
#define CBMTOFCLUSTERIZERSDEF_H_

/*
 * From CbmTofSimpleClusterizer.cxx, duplicates are commented out as
 * this class from trunk is older than the TesBeamClusterizer from Tofgroup
 */
//const Int_t DetMask = 4194303;
//const Int_t nbClWalkBinX=20;
//const Int_t nbClWalkBinY=41;  // choose odd number to have central bin symmetric around 0
//const Double_t WalkNHmin=100; // minimal number of hits in bin for walk correction
//Double_t TOTMax=5.E4;
//Double_t TOTMin=2.E4;
//const Double_t TTotMean=2.E4;

//const Int_t nbClDelTofBinX=50;
//const Int_t nbClDelTofBinY=49;
//const Double_t DelTofMax=5000.;

//const Int_t nbCldXdYBinX=49;
//const Int_t nbCldXdYBinY=49;
//const Double_t dXdYMax=10.;

const Int_t iNTrg = 1;

//const Double_t Zref = 200.;   // distance of projection plane to target

/*
 * From CbmTofTestBeamClusterizer.cxx
 */
const Double_t dDoubleMax = 1.E300;
/*
const Int_t DetMask       = 0x003FFFFF; // geo v14a
const Int_t ModMask       = 0x0000FFFF; // geo v14a
*/
const Int_t DetMask = 0x001FFFFF;  // geo v21a
const Int_t ModMask = 0x0000FFFF;  // geo v21a

/*
Int_t iNevtBuild=0; // Should be class member!
Int_t iMsgCnt=100;  // Should be class member!
*/
const Int_t nbClWalkBinX = 50;  // was 100 (11.10.2018)
const Int_t nbClWalkBinY =
  41;  // choose odd number to have central bin symmetric around 0
const Double_t WalkNHmin =
  10.;  // minimal number of hits in bin for walk correction
const Int_t iNWalkSmooth = 1;  // Smoothing level for walk histos
/*
Double_t TOTMax=5.E4; // Should be class member!
Double_t TOTMin=0.;           //2.E4; // Should be class member!
Double_t TTotMean=5.E3; //2.E4; // Should be class member!

Double_t dMaxTimeDist=0.;     // Should be class member!
Double_t dMaxSpaceDist = 0.;  // Should be class member!
*/

const Int_t nbClDelTofBinX = 60;
const Int_t nbClDelTofBinY = 59;
//const Double_t DelTofMax=60000.;

const Int_t nbCldXdYBinX = 49;
const Int_t nbCldXdYBinY = 49;
const Double_t dXdYMax   = 10.;

const Int_t iNSel = 2;  //4;

const Double_t Zref = 200.;  // distance of projection plane to target

const Double_t MaxNbEvent = 1500000;
//Double_t dEvent=0; // Should be class member!

#endif  // CBMTOFCLUSTERIZERSDEF_H_
