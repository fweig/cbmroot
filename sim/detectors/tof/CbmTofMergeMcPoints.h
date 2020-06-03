/**
 * \file CbmTofMergeMcPoints.h
 * \brief FairTask for merging TOF MC Points into more realistic TOF MC points.
 * \author Pierre-Alain Loizeau <p.-a.loizeau@gsi.de>
 * \date 2016
 */

#ifndef CBMTOFMERGEMCPOINTS_H_
#define CBMTOFMERGEMCPOINTS_H_

#include "FairTask.h"
class TClonesArray;

#include <map>
#include <vector>

class CbmTofGeoHandler;
class CbmTofDetectorId;

class CbmTofMergeMcPoints : public FairTask
{
   public:
      /**
      * \brief Constructor.
      */
      CbmTofMergeMcPoints();

      /**
      * \brief Destructor.
      */
      virtual ~CbmTofMergeMcPoints();

      /**
      * \brief Derived from FairTask.
      */
      virtual InitStatus Init();

      /**
      * \brief Derived from FairTask.
      */
      virtual void Exec(
       Option_t* opt);

      /**
      * \brief Derived from FairTask.
      */
      virtual void Finish();

   private:
      /**
      * \brief Read and create data branches.
      */
      void ReadAndCreateDataBranches();
       
      void MergeRealisticTofPoints(
          const TClonesArray* tracks,
          const TClonesArray* points,
          TClonesArray* realisticPoints,
          TClonesArray* pointsMatches);
          
      CbmTofGeoHandler * fGeoHandler;
      CbmTofDetectorId * fTofId;
      
      TClonesArray     * fMcTracksColl;  // MC tracks
      TClonesArray     * fTofPointsColl; // CbmTofPoint array
      
      std::map< Int_t, std::vector< Int_t > > fTofPntTrkMap; // map of TrkId + vector of PntId
      TClonesArray     * fRealTofPoints; // CbmTofPoint array for realistic TOF MC: 1 per (Trk, det) pair
      TClonesArray     * fTofRealPntMatches; 	// CbmMatch array for MC TOF Pnt -> Realistic MC TOF Point

      CbmTofMergeMcPoints(const CbmTofMergeMcPoints&);
      CbmTofMergeMcPoints& operator=(const CbmTofMergeMcPoints&);

   ClassDef(CbmTofMergeMcPoints, 1);
};

#endif /* CBMTOFMERGEMCPOINTS_H_ */
