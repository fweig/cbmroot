/* Copyright (C) 2017-2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexander Zinchenko [committer] */

/** CbmVectorFinder.h
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2017
 **
 ** Steering task class for vector finding in CBM.
 ** Input: 
 ** Output:
 **
 **/

#ifndef CBMVECTORFINDER_H_
#define CBMVECTORFINDER_H_ 1

//#include "CbmMuchGeoScheme.h"
#include "FairTask.h"
//#include "FairTrackParam.h"
//#include <TDecompLU.h>
//#include <map>
//#include <set>
//#include <vector>

//class CbmMuchPixelHit;
//class CbmMuchTrack;
//class TClonesArray;

class CbmVectorFinder : public FairTask {
public:
  /** Default constructor **/
  CbmVectorFinder();

  /** Destructor **/
  virtual ~CbmVectorFinder();

  /** Initialisation **/
  virtual InitStatus Init();

  /** Task execution **/
  virtual void Exec(Option_t* opt);

  /** Finish at the end **/
  virtual void Finish();

  /** SetParContainers **/
  virtual void SetParContainers();

  /** Accessors **/
  //Int_t GetNofTracks()           { return fNofTracks; };
  //Int_t GetNofStat() const       { return fgkStat; }

private:
  // Some constants
  //static const Int_t fgkStat = 4;               // Number of stations - 4 GEMs
  //static const Int_t fgkPlanes = 6;             // Number of GEM planes per station

private:
  /*
  CbmMuchGeoScheme* fGeoScheme;                 // Geometry scheme
  TClonesArray* fTrackArray;                    // Output array of CbmMuchVectors
  Int_t fNofTracks;                             // Number of tracks created
  TClonesArray* fHits;                          // Input array of CbmMuchHit
  TClonesArray* fClusters;                      // Input array of CbmMuchCluster
  TClonesArray* fPoints;                        // Input array of CbmMuchHit
  TClonesArray* fDigiMatches;                   // Input array of CbmMuchHit
  Int_t fStatFirst;                             // First straw station No.
  std::multimap<Int_t,Int_t> fHitPl[fgkStat][fgkPlanes]; // hit indices on planes vs tube No
  std::multimap<Double_t,Int_t> fHitX[fgkStat][fgkPlanes]; // hit indices on planes vs X-coord.
  std::vector<CbmMuchTrack*> fVectors[fgkStat]; // track vectors for stations
  //std::vector<CbmMuchTrack*> fVectorsHigh[fgkStat]; // track vectors for stations (high resolution)
  Double_t fXy[fgkPlanes][5];                   // hit float data
  Double_t fXyi[fgkPlanes][3];                  // hit int data
  Double_t fDz[fgkPlanes];                      // geometrical constants (Z-distances from layer 0)
  std::map<Int_t,TDecompLU*> fLus;              // system matrices (for different hit layer patterns)
  Double_t fErrX[9];                            // hit measurement error in X
  Double_t fErrY[9];                            // hit measurement error in Y
  Double_t fCutChi2[9];                         // Chi2-cuts for each station
  Double_t fZ0[fgkStat];                        // Z-positions of the first layers
  Double_t fRmin[fgkStat];                      // inner radii of stations
  Double_t fRmax[fgkStat];                      // outer radii of stations
  Int_t fNsect[fgkStat];                        // number of sectors per layer
  //Double_t fDtubes[fgkStat][fgkPlanes];         // max. tube difference between views
  std::map<Int_t,TMatrixDSym*> fMatr;           // system matrices (for different hit layer patterns)
  std::multimap<Int_t,CbmMuchTrack*> fSecVec[fgkStat]; // to arrange vectors according to sectors
  Double_t fZabs0[9][2];                        // Z-positions of absorber faces
  Double_t fX0abs[9];                           // radiation lengths of the absorbers

  //std::vector<pair<Int_t,Int_t> > fHit2d[fgkStat][fgkPlanes/2]; // Indx1,Indx2 of doublet hits

  void ComputeMatrix();
  void GetHits();
  Bool_t SelectHitId(const CbmMuchPixelHit *hit);
  Int_t GetDowns(Int_t ista, std::vector<std::pair<Double_t,Double_t> >& vecDowns);
  void MakeVectors();
  //void ProcessDouble(Int_t ista, Int_t lay2, Int_t patt, Int_t flag, Int_t tube0, Int_t segment0);
  void ProcessPlane(Int_t ista, Int_t lay2, Int_t patt, Int_t flag);
  void AddVector(Int_t ista, Int_t patt, Double_t chi2, Double_t *pars); 
  void SetTrackId(CbmMuchTrack *vec);
  void FindLine(Int_t patt, Double_t *pars);
  Double_t FindChi2(Int_t ista, Int_t patt, Double_t *pars);
  void CheckParams();
  //void HighRes();
  //void ProcessSingleHigh(Int_t ista, Int_t plane, Int_t patt, Int_t flag, Int_t nok, Double_t uu[fgkPlanes][2]); 
  void RemoveClones();
  void StoreVectors();
  Int_t CountBits(Int_t x); 
  void Refit(Int_t patt, Double_t &chi2, Double_t *pars, TMatrixDSym &cov);
  void MatchVectors();
  */

  CbmVectorFinder(const CbmVectorFinder&);
  CbmVectorFinder& operator=(const CbmVectorFinder&);

  ClassDef(CbmVectorFinder, 0)
};

#endif
