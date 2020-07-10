/** CbmMuchFindVectorsGem.h
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **
 ** Task class for vector finding in MUCH GEMs.
 ** Input: TClonesArray of CbmMuchHit
 ** Output: TClonesArray of CbmMuchTrack
 **
 **/

#ifndef CBMMUCHFINDVECTORSGEM_H_
#define CBMMUCHFINDVECTORSGEM_H_ 1

#include "CbmMuchGeoScheme.h"
#include "FairTask.h"
#include "FairTrackParam.h"
#include <TDecompLU.h>
#include <map>
#include <set>
#include <vector>

class CbmMuchPixelHit;
class CbmMuchTrack;
class CbmMCDataArray;
class TClonesArray;

class CbmMuchFindVectorsGem : public FairTask {
public:
  /** Default constructor **/
  CbmMuchFindVectorsGem();

  /** Destructor **/
  virtual ~CbmMuchFindVectorsGem();

  /** Initialisation **/
  virtual InitStatus Init();

  /** Task execution **/
  virtual void Exec(Option_t* opt);

  /** Finish at the end **/
  virtual void Finish();

  /** SetParContainers **/
  virtual void SetParContainers();

  /** Accessors **/
  Int_t GetNofTracks() { return fNofTracks; };
  Int_t GetNofStat() const { return fgkStat; }

  // Return info about absorbers
  Int_t GetAbsorbers(Double_t zabs[9][2], Double_t* x0abs);

private:
  // Some constants
  //static const Int_t fgkStat = 2;               // Number of stations - 2 GEMs
  static const Int_t fgkStat   = 4;  // Number of stations - 4 GEMs
  static const Int_t fgkPlanes = 6;  // Number of GEM planes per station

private:
  CbmMuchGeoScheme* fGeoScheme;  // Geometry scheme
  TClonesArray* fTrackArray;     // Output array of CbmMuchVectors
  TClonesArray*
    fVecPool;           // Transient array of CbmMuchVectors (instead of heap)
  Int_t fNofTracks;     // Number of tracks created
  TClonesArray* fHits;  // Input array of CbmMuchHit
  TClonesArray* fClusters;     // Input array of CbmMuchCluster
  TClonesArray* fDigiMatches;  // Input array of CbmMatch
  TClonesArray* fTrdVectors;   // Input array of CbmMuchTrack
  CbmMCDataArray* fPoints;     // Input array of CbmMuchPoint
  Int_t fStatFirst;            // First straw station No.
  std::multimap<Int_t, Int_t>
    fHitPl[fgkStat][fgkPlanes];  // hit indices on planes vs tube No
  std::multimap<Double_t, Int_t>
    fHitX[fgkStat][fgkPlanes];  // hit indices on planes vs X-coord.
  std::vector<CbmMuchTrack*> fVectors[fgkStat];  // track vectors for stations
  //std::vector<CbmMuchTrack*> fVectorsHigh[fgkStat]; // track vectors for stations (high resolution)
  Double_t fXy[fgkPlanes][5];   // hit float data
  Double_t fXyi[fgkPlanes][3];  // hit int data
  Double_t fDz[fgkPlanes];  // geometrical constants (Z-distances from layer 0)
  std::map<Int_t, TDecompLU*>
    fLus;                 // system matrices (for different hit layer patterns)
  Double_t fErrX[9];      // hit measurement error in X
  Double_t fErrY[9];      // hit measurement error in Y
  Double_t fCutChi2[9];   // Chi2-cuts for each station
  Double_t fZ0[fgkStat];  // Z-positions of the first layers
  Double_t fRmin[fgkStat];  // inner radii of stations
  Double_t fRmax[fgkStat];  // outer radii of stations
  Int_t fNsect[fgkStat];    // number of sectors per layer
  //Double_t fDtubes[fgkStat][fgkPlanes];         // max. tube difference between views
  std::map<Int_t, TMatrixDSym*>
    fMatr;  // system matrices (for different hit layer patterns)
  std::multimap<Int_t, CbmMuchTrack*>
    fSecVec[fgkStat];     // to arrange vectors according to sectors
  Double_t fZabs0[9][2];  // Z-positions of absorber faces
  Double_t fX0abs[9];     // radiation lengths of the absorbers

  //std::vector<pair<Int_t,Int_t> > fHit2d[fgkStat][fgkPlanes/2]; // Indx1,Indx2 of doublet hits

  void ComputeMatrix();
  void GetHits();
  Bool_t SelectHitId(const CbmMuchPixelHit* hit);
  Int_t GetDowns(Int_t ista,
                 std::vector<std::pair<Double_t, Double_t>>& vecDowns);
  void MakeVectors(Int_t ista);
  Int_t GetTrdVectors(std::vector<std::pair<Double_t, Double_t>>& vecDowns);
  void ProcessPlane(Int_t ista, Int_t lay2, Int_t patt, Int_t flag);
  void AddVector(Int_t ista, Int_t patt, Double_t chi2, Double_t* pars);
  void SetTrackId(CbmMuchTrack* vec);
  void FindLine(Int_t patt, Double_t* pars);
  Double_t FindChi2(Int_t ista, Int_t patt, Double_t* pars);
  void CheckParams(Int_t ista);
  void RemoveClones(Int_t ista);
  void StoreVectors();
  Int_t CountBits(Int_t x);
  void Refit(Int_t patt, Double_t& chi2, Double_t* pars, TMatrixDSym& cov);
  void MatchVectors();

  CbmMuchFindVectorsGem(const CbmMuchFindVectorsGem&);
  CbmMuchFindVectorsGem& operator=(const CbmMuchFindVectorsGem&);

  ClassDef(CbmMuchFindVectorsGem, 0)
};

#endif
