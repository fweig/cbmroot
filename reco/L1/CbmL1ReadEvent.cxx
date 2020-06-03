/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov, I. Rostovtseva (2016)
 *  
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  Read Event information to L1
 *
 *====================================================================
 */

#include "L1AlgoInputData.h"
#include "CbmL1.h"
#include "L1Algo/L1Algo.h"
#include "CbmKF.h"
#include "CbmMatch.h"
#include "CbmStsAddress.h"
#include "CbmStsSetup.h"

#include "CbmMatch.h"
#include "CbmMuchPixelHit.h"
#include "CbmTrdHit.h"
#include "CbmTofHit.h"
#include "CbmMuchGeoScheme.h"
#include "CbmMuchPoint.h"
#include "CbmTrdPoint.h"
#include "CbmTofPoint.h"

//#include "CbmMvdHitMatch.h"


#include "TDatabasePDG.h"
#include "TRandom.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::find;

//#define MVDIDEALHITS
//#define STSIDEALHITS


static bool compareZ(const int &a, const int &b )
{
//        return (CbmL1::Instance()->vMCPoints[a].z < CbmL1::Instance()->vMCPoints[b].z);
  const CbmL1* l1 = CbmL1::Instance();
  return l1->Get_Z_vMCPoint(a) < l1->Get_Z_vMCPoint(b);
}




struct TmpHit{ // used for sort Hits before writing in the normal arrays
  int iStripF, iStripB;   // indices of real-strips, sts-strips (one got from detector. They consist from parts with differen positions, so will be divided before using)
  int indStripF, indStripB; // indices of L1-strips, indices in TmpStrip arrays
  int iStation;
  int ExtIndex;           // index of hit in the TClonesArray array ( negative for MVD )
  bool isStrip;
  double u_front, u_back; // positions of strips
  double x, y;            // position of hit
  double dx, dy, dxy;
  double du, dv;
  int iMC;                // index of MCPoint in the vMCPoints array
  double time, t_er, z;
  int Det;
  int id;
  int track;
  static bool Compare( const TmpHit &a, const TmpHit &b ){
    return ( a.iStation <  b.iStation ) ||
         ( ( a.iStation == b.iStation ) && ( a.y < b.y ) );
  }
};

struct TmpStrip{
  fscal u;
  fscal time;
  int iStation;
  int iStrip;
  bool isStrip;
  int effIndex; // used for unefficiency
};

  /// Repack data from Clones Arrays to L1 arrays
void CbmL1::ReadEvent(L1AlgoInputData* fData_, CbmEvent* event)
{
  if (fVerbose >= 10) cout << "ReadEvent: start." << endl;

    // clear arrays for next event
  vMCPoints.clear();
  vMCPoints_in_Time_Slice.clear();
  vMCTracks.clear();
  vStsHits.clear();
  vRTracks.clear();
  vHitMCRef.clear();
  vHitStore.clear();
  dFEI2vMCPoints.clear();
  dFEI2vMCTracks.clear();
  if (fVerbose >= 10) cout << "ReadEvent: clear is done." << endl;
  
  vector<TmpHit> tmpHits;
  vector<TmpStrip> tmpStrips;
  vector<TmpStrip> tmpStripsB;

    // -- produce Sts hits from space points --

  for(int i = 0; i < NStation; i++){

    fData_->StsHitsStartIndex[i] = static_cast<THitI>(-1);
    fData_->StsHitsStopIndex[i]  = 0;
  }

  //Int_t nMvdPoints = 0;
    nMvdPoints = 0;
    // get MVD hits
  Int_t nMvdHits = 0;
  Int_t nMuchHits = 0;
  Int_t nTrdHits = 0;
  Int_t nTofHits = 0;
    // get STS hits
  int nStsHits = 0;
  
  int nStsPoints =0;
  int nTrdPoints =0;
  nMuchPoints =0;
  int nTofPoints =0;
  
  vector <CbmLink*> ToFPointsMatch;

  if (fPerformance)
  {
    Fill_vMCTracks();
    
    for( DFSET::iterator set_it = vFileEvent.begin(); set_it!=vFileEvent.end(); ++set_it){
      Int_t iFile = set_it->first;
      Int_t iEvent = set_it->second;

     
      if(fMvdPoints && listMvdPts)
      {
        Int_t nMvdPointsInEvent =  fMvdPoints->Size(iFile, iEvent);
        for(Int_t iMC = 0; iMC < nMvdPointsInEvent; iMC++){
          CbmL1MCPoint MC;
          
          MC.event = iEvent;
  
          if( ! ReadMCPoint( &MC, iMC, iFile, iEvent, 1 ) ){
            MC.iStation = -1;
            L1Station *sta = algo->vStations;
            for(Int_t iSt=0; iSt < NStsStations; iSt++)
              MC.iStation = (MC.z > sta[iSt].z[0] - 1) ? iSt : MC.iStation;

            Double_t dtrck =dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            if (trk_it==dFEI2vMCTracks.end()) continue;
            Int_t IND_Track = trk_it->second;
            vMCTracks[IND_Track].Points.push_back(vMCPoints.size()); 

            MC.ID = trk_it->second;
            
  //           vMCTracks[MC.ID].Points.push_back(vMCPoints.size()); 
            
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            
            dFEI2vMCPoints.insert( DFEI2I::value_type(dFEI(iFile,iEvent,iMC), vMCPoints.size()-1) );
            nMvdPoints++;
          }
        }
      }
        
        Int_t nMC =  fStsPoints->Size(iFile, iEvent);
        
        for(Int_t iMC = 0; iMC < nMC; iMC++){
          CbmL1MCPoint MC;
          
          MC.event = iEvent;
  
          if( ! ReadMCPoint( &MC, iMC, iFile, iEvent, 0 ) ){
            MC.iStation = -1;
            L1Station *sta = algo->vStations + NMvdStations;
            for(Int_t iSt=0; iSt < NStsStations; iSt++)
              MC.iStation = (MC.z > sta[iSt].z[0] - 2.5) ? (NMvdStations + iSt) : MC.iStation;

            Double_t dtrck =dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            if (trk_it==dFEI2vMCTracks.end()) continue;
            Int_t IND_Track = trk_it->second;
            vMCTracks[IND_Track].Points.push_back(vMCPoints.size()); 

            MC.ID = trk_it->second;
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            
            dFEI2vMCPoints.insert( DFEI2I::value_type(dFEI(iFile,iEvent,iMC+nMvdPoints), vMCPoints.size()-1) );
            nStsPoints++;

          }
        }       
        
        if(fMuchPoints){
        
        for(Int_t iMC = 0; iMC < fMuchPoints->Size(iFile, iEvent); iMC++){
          CbmL1MCPoint MC;
          
          MC.event = iEvent;
  
          if( ! ReadMCPoint( &MC, iMC, iFile, iEvent, 2 ) ){
            MC.iStation = -1;
            L1Station *sta = algo->vStations + NMvdStations + NStsStations;
            for(Int_t iSt=0; iSt < NMuchStations; iSt++)
              MC.iStation = (MC.z > sta[iSt].z[0] - 2.5) ? (NMvdStations+ NStsStations + iSt) : MC.iStation;

            Double_t dtrck =dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            if (trk_it==dFEI2vMCTracks.end()) continue;
            Int_t IND_Track = trk_it->second;

            vMCTracks[IND_Track].Points.push_back(vMCPoints.size()); 

            MC.ID = trk_it->second;
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            
            dFEI2vMCPoints.insert( DFEI2I::value_type(dFEI(iFile,iEvent,iMC+nMvdPoints+nStsPoints), vMCPoints.size()-1) );
            nMuchPoints++;
            

          }
        } 
        } 
        
        
        if(fTrdPoints)
        for(Int_t iMC = 0; iMC < fTrdPoints->Size(iFile, iEvent); iMC++){
          CbmL1MCPoint MC;
          
          MC.event = iEvent;

  
          if( ! ReadMCPoint( &MC, iMC, iFile, iEvent, 3 ) ){
            
            MC.iStation = -1;
            L1Station *sta = algo->vStations + NMvdStations + NStsStations+NMuchStations;
            for(Int_t iSt=0; iSt < NTrdStations; iSt++)
              MC.iStation = (MC.z > sta[iSt].z[0] - 4.0) ? (NMvdStations+ NStsStations+NMuchStations + iSt) : MC.iStation;


            Double_t dtrck =dFEI(iFile, iEvent, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            if (trk_it==dFEI2vMCTracks.end()) continue;
            Int_t IND_Track = trk_it->second;
            
            vMCTracks[IND_Track].Points.push_back(vMCPoints.size()); 

            MC.ID = trk_it->second;
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            
            dFEI2vMCPoints.insert( DFEI2I::value_type(dFEI(iFile,iEvent,iMC+nMvdPoints+nStsPoints+nMuchPoints), vMCPoints.size()-1) );
            nTrdPoints++;
            

          }
        } 

        ToFPointsMatch.resize(0);
  
        if(fTofPoints) {
          for (int j=0; j <fTofHits->GetEntries(); j++ ){

          CbmTofHit *mh = L1_DYNAMIC_CAST<CbmTofHit*>( fTofHits->At(j) );
        
          CbmMatch *matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>( fTofHitDigiMatches->At(j) );
        
          if( matchHitMatch->GetNofLinks() > 0 ) {

            CbmLink *link = (CbmLink*) &matchHitMatch->GetLink(0);
            CbmTofPoint* pt = (CbmTofPoint*) fTofPoints->Get(link->GetFile(),link->GetEntry(),link->GetIndex());

            for(int iLink=1; iLink<matchHitMatch -> GetNofLinks(); iLink++)
            {
          
             CbmLink *link1 = (CbmLink*) &(matchHitMatch->GetLink(iLink));
          
             CbmTofPoint* pt_cur = (CbmTofPoint*) fTofPoints->Get(link1->GetFile(),link1->GetEntry(),link1->GetIndex());
          
              TVector3 pos_cur, pos_old, pos_hit;

          
              pt_cur->Position(pos_cur);
              pt->Position(pos_old);
              mh->Position(pos_hit);
          
              if ( fabs(pos_cur.Z() - pos_hit.Z()) < fabs(pos_old.Z() - pos_hit.Z()) ) {pt = pt_cur; link = link1; }         
         }  

         ToFPointsMatch.push_back(link);
    } // for j
  } // if listTrdHits
  

        for(UInt_t iMC = 0; iMC < ToFPointsMatch.size(); iMC++){
          CbmL1MCPoint MC;
          
          MC.event = iEvent;
          
          int eventNr = ToFPointsMatch[iMC]->GetEntry();
          
          if (eventNr != iEvent) continue;
  
          if( ! ReadMCPoint( &MC, ToFPointsMatch[iMC]->GetIndex(), ToFPointsMatch[iMC]->GetFile(),ToFPointsMatch[iMC]->GetEntry(),  4 ) ){
            
            MC.iStation = NMvdStations + NStsStations + NMuchStations + NTrdStations;

            Double_t dtrck =dFEI(iFile, eventNr, MC.ID);
            DFEI2I::iterator trk_it = dFEI2vMCTracks.find(dtrck);
            if (trk_it==dFEI2vMCTracks.end()) continue;
            
            Int_t IND_Track = trk_it->second;
            
            vMCTracks[IND_Track].Points.push_back(vMCPoints.size()); 

            MC.ID = trk_it->second;
            
            vMCPoints.push_back(MC);
            vMCPoints_in_Time_Slice.push_back(0);
            
            dFEI2vMCPoints.insert( DFEI2I::value_type(dFEI(iFile,eventNr,ToFPointsMatch[iMC]->GetIndex()+nMvdPoints+nStsPoints+nMuchPoints+nTrdPoints), vMCPoints.size()-1) );
            nTofPoints++;
          }
        }
      }
        
    } //time_slice

    for (unsigned int iTr = 0; iTr < vMCTracks.size(); iTr++) 
    {

      sort( vMCTracks[iTr].Points.begin(), vMCTracks[iTr].Points.end(), compareZ );

      if ( vMCTracks[iTr].mother_ID >= 0)
      {
          Double_t dtrck =dFEI(vMCTracks[iTr].iFile, vMCTracks[iTr].iEvent, vMCTracks[iTr].mother_ID);
          DFEI2I::iterator map_it = dFEI2vMCTracks.find(dtrck);
        if (map_it==dFEI2vMCTracks.end()) vMCTracks[iTr].mother_ID = -2;
          else 
            vMCTracks[iTr].mother_ID = map_it->second;
          
          }
    } //iTr
    if (fVerbose >= 10) cout << "Points in vMCTracks are sorted." << endl;

  } //fPerformance

  
      // get MVD hits

  if( listMvdHits ){

//     if(listMvdPts)
//     {
//       isUsedMvdPoint.resize(nMC);
//       for(int iMc=0; iMc<nMC; iMc++) isUsedMvdPoint[iMc]=0;
//     }

    for (int j=0; j <listMvdHits->GetEntries(); j++ ){
      TmpHit th;
      {
        CbmMvdHit *mh = L1_DYNAMIC_CAST<CbmMvdHit*>( listMvdHits->At(j) );
        th.ExtIndex = -(1+j);
        th.iStation = mh->GetStationNr() ;
     //   th.iSector  = 0;
        th.isStrip  = 0;
        th.iStripF = j;
        th.iStripB = -1;
        if( th.iStripF<0 ) continue;
        if( th.iStripF>=0 && th.iStripB>=0 ) th.isStrip  = 1;
        if( th.iStripB <0 ) th.iStripB = th.iStripF;
  
        TVector3 pos, err;
        mh->Position(pos);
        mh->PositionError(err);
        
        th.dx = mh->GetDx();
        th.dy = mh->GetDy();
        
        th.du = mh->GetDx();
        th.dv = mh->GetDy();
        th.dxy = mh->GetDxy();

        th.x = pos.X();
        th.y = pos.Y();
  
        L1Station &st = algo->vStations[th.iStation];
        th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
        th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];
      }
      th.Det = 0;
      th.iMC=-1;
      int iMC = -1;
//   int iMCTr = -1;
//   if( listMvdHitMatches ){
//     CbmMvdHitMatch *match = (CbmMvdHitMatch*) listMvdHitMatches->At(j);
//     if( match){
//       iMC = match->GetPointId();
//       iMCTr = match->GetTrackId();
//     }
//   }
      if (fPerformance)
      {
      if( listMvdHitMatches ){
        CbmMatch *mvdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>( listMvdHitMatches->At(j) );

        
        if( mvdHitMatch->GetNofLinks() > 0 )
          if( mvdHitMatch->GetLink(0).GetIndex() < nMvdPoints )
          {
            iMC = mvdHitMatch->GetLink(0).GetIndex();
            th.iMC = iMC;
        //    th.track = iMC;
          }
      }
//       if( listMvdPts && iMC>=0 ){ // TODO1: don't need this with FairLinks
//         CbmL1MCPoint MC;
//         if( ! ReadMCPoint( &MC, iMC, 1 ) ){
//           MC.iStation = th.iStation;
//       //    isUsedMvdPoint[iMC] = 1;
// 
// //       MC.ID = iMCTr; // because atch->GetPointId() == 0 !!! and ReadMCPoint don't work
// //       MC.z = th.iStation; // for sort in right order
//       
//           vMCPoints.push_back( MC );
//           nMvdPoints++;
//           vMCPoints_in_Time_Slice.push_back(0);
//           th.iMC = vMCPoints.size()-1;
//         }
//         Int_t iFile =-1;
//         Int_t iEvent=-1;
//         
//         
//                if ( !fTimesliceMode )
//         {
//           iFile  = vFileEvent.begin()->first;
//           iEvent = vFileEvent.begin()->second;
//         } 
//         
//       Double_t dtrck =dFEI(iFile, iEvent, iMC);
//       DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
//       if (trk_it==dFEI2vMCPoints.end()) continue;
//       iMC = trk_it->second;  
//         
//       } // if listStsPts
      
      
      }
  //if(  h.MC_Point >=0 ) // DEBUG !!!!
      {
        tmpHits.push_back(th);
        nMvdHits++;
      }
    } // for j
  } // if listMvdHits
  if (fVerbose >= 10) cout << "ReadEvent: mvd hits are gotten." << endl;


  if( listStsHits )
  {
    Int_t nEnt = 0;
    if ( fTimesliceMode ) nEnt = listStsHits->GetEntries();
    else nEnt = (event ? event->GetNofData(ECbmDataType::kStsHit) : listStsHits->GetEntries());

    for(Int_t j = 0; j < nEnt; j++ )
    {
      Int_t hitIndex = 0;
      if ( fTimesliceMode ) hitIndex = j;
      else hitIndex = (event ? event->GetIndex(ECbmDataType::kStsHit, j) : j);

      CbmStsHit *sh = L1_DYNAMIC_CAST<CbmStsHit*>( listStsHits->At(hitIndex) );
      
      TmpHit th;
      {
        CbmStsHit *mh = L1_DYNAMIC_CAST<CbmStsHit*>( listStsHits->At(hitIndex) );
        th.ExtIndex = hitIndex;
        th.Det = 1;
        th.iStation = NMvdStations + CbmStsSetup::Instance()->GetStationNumber(mh->GetAddress());//mh->GetStationNr() - 1;
        th.isStrip  = 0;
        th.iStripF = 0;//mh->GetFrontDigiId();
        th.iStripB = 0;//mh->GetBackDigiId();

        //Get time
        th.time =  mh->GetTime();
        th.t_er =  mh->GetTimeError();    
        

        th.iStripF += nMvdHits;
        th.iStripB += nMvdHits;

        TVector3 pos, err;
        mh->Position(pos);
        mh->PositionError(err);

        th.x = pos.X();
        th.y = pos.Y();
        
        th.dx = mh->GetDx();
        th.dy = mh->GetDy();
        th.dxy = mh->GetDxy();
        
        th.du = mh->GetDu();
        th.dv = mh->GetDv();

        L1Station &st = algo->vStations[th.iStation];
        th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
        th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y* st.backInfo.sin_phi[0];
      }
      th.iMC = -1;

      Int_t iMC = -1;

      if (fPerformance)
      {
        if(listStsClusterMatch){
          const CbmMatch* frontClusterMatch = static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetFrontClusterId()));
          const CbmMatch* backClusterMatch  = static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetBackClusterId()));
          CbmMatch stsHitMatch;

          for(Int_t iFrontLink = 0; iFrontLink<frontClusterMatch->GetNofLinks(); iFrontLink++){
            const CbmLink& frontLink = frontClusterMatch->GetLink(iFrontLink);

            for(Int_t iBackLink = 0; iBackLink<backClusterMatch->GetNofLinks(); iBackLink++){
              const CbmLink& backLink = backClusterMatch->GetLink(iBackLink);
              if (frontLink == backLink){
                stsHitMatch.AddLink(frontLink);
                stsHitMatch.AddLink(backLink);
              }
            }
          }

          if( stsHitMatch.GetNofLinks()>0 ){
            Float_t bestWeight = 0.f;
            for(Int_t iLink=0; iLink < stsHitMatch.GetNofLinks(); iLink++){
              if( stsHitMatch.GetLink(iLink).GetWeight() > bestWeight){
                bestWeight = stsHitMatch.GetLink(iLink).GetWeight();
                Int_t iFile  = stsHitMatch.GetLink(iLink).GetFile();
                Int_t iEvent = stsHitMatch.GetLink(iLink).GetEntry();
//                 if(!fTimesliceMode) //TODO Fix the event number in links
//                   iEvent+=1;
                Int_t iIndex = stsHitMatch.GetLink(iLink).GetIndex() + nMvdPoints;
                Double_t dtrck =dFEI(iFile, iEvent, iIndex);
                DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
                if (trk_it==dFEI2vMCPoints.end()) continue;
                iMC = trk_it->second;
              }
            }
          }
        }        
        else
          iMC = sh->GetRefId(); // TODO1: don't need this with FairLinks
      } //fPerformance

      if(iMC > -1)
      { th.iMC = iMC;
       // th.track = iMC;
        
      }

      tmpHits.push_back(th);
      nStsHits++;
    
    } // for j
  } // if listStsHits
  
      if( fMuchPixelHits ){

    Int_t nEnt  = fMuchPixelHits->GetEntries();


    for (int j=0; j <nEnt; j++ ){
      TmpHit th;
      {
        
        CbmMuchPixelHit* mh = static_cast<CbmMuchPixelHit*>(fMuchPixelHits->At(j) );
        
        th.ExtIndex = j;
        th.Det = 2;

        
        Int_t stationNumber = CbmMuchGeoScheme::GetStationIndex(mh->GetAddress());
        Int_t layerNumber = CbmMuchGeoScheme::GetLayerIndex(mh->GetAddress());
        
        int DetId = stationNumber*3 + layerNumber;
        
        
        th.iStation = DetId + NMvdStations+NStsStations ;
                //Get time
        th.time =  mh->GetTime()-17;
        th.t_er =  mh->GetTimeError();  


     //   th.iSector  = 0;
        th.isStrip  = 0;
        th.iStripF = j;
        th.iStripB = -1;
        if( th.iStripF<0 ) continue;
        if( th.iStripF>=0 && th.iStripB>=0 ) th.isStrip  = 1;
        if( th.iStripB <0 ) th.iStripB = th.iStripF;
        
        th.iStripF += nMvdHits+nStsHits;
        th.iStripB += nMvdHits+nStsHits;

        th.x = mh->GetX();
        th.y = mh->GetY();
        th.z = mh->GetZ();
        
        th.dx = mh->GetDx();
        th.dy = mh->GetDy();
        th.dxy = 0;
        
        th.du = mh->GetDx();
        th.dv = mh->GetDy();

  
        L1Station &st = algo->vStations[th.iStation];
        th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
        th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];
      }
      th.iMC=-1;
      int iMC = -1;


       if (fPerformance)
      {
      if( listMuchHitMatches ){
        CbmMatch *matchHitMatch = L1_DYNAMIC_CAST<CbmMatch*>( listMuchHitMatches->At(j) );


        for(Int_t iLink=0; iLink < matchHitMatch->GetNofLinks(); iLink++){
          if( matchHitMatch->GetLink(iLink).GetIndex() < nMuchPoints )
          {
            iMC = matchHitMatch->GetLink(iLink).GetIndex();
            Int_t iIndex = iMC+nMvdPoints+nStsPoints;
            
            Int_t iFile  = matchHitMatch->GetLink(0).GetFile();
            Int_t iEvent = matchHitMatch->GetLink(0).GetEntry();
            
            Double_t dtrck =dFEI(iFile, iEvent, iIndex);
            DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
            if (trk_it==dFEI2vMCPoints.end()) continue;
            th.iMC = trk_it->second;
//             th.track = vMCPoints[th.iMC].ID;
//              th.qp = vMCPoints[iMC].p;  
  //    if(matchHitMatch -> GetNofLinks() == 0) continue;
//      Float_t bestWeight = 0.f;
//      Float_t totalWeight = 0.f;
 //     int iMCPoint = -1;
   //   CbmLink link;

//    CbmMuchPoint* pt = (CbmMuchPoint*) fMuchPoints->Get(matchHitMatch->GetLink(0).GetFile(),matchHitMatch->GetLink(0).GetEntry(),matchHitMatch->GetLink(0).GetIndex());
   // double mom = sqrt(pt->GetPxOut()*pt->GetPxOut()+pt->GetPyOut()*pt->GetPyOut()+pt->GetPzOut()*pt->GetPzOut());
//     th.p = mom;
//     th.q = pt->GetTrackID();//(L1_DYNAMIC_CAST<CbmMCTrack*>(fMCTracks->Get(link.GetFile(),link.GetEntry(),  pt->GetTrackID()) ))->GetCharge();
/*    
    static float dx_= th.dx;
     static float dy_= th.dy;
      static float dt_= th.t_er;
            
        th.x = pt->GetX( th.z ) + gRandom->Gaus(0,th.dx);

        th.y = pt->GetY(th.z)+ gRandom->Gaus(0,th.dy);
        th.time =  pt->GetTime()+ gRandom->Gaus(0,th.t_er);
        
                L1Station &st = algo->vStations[th.iStation];
        th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
        th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];*/
          }  
          }
      }
      }

        tmpHits.push_back(th);
        nMuchHits++;
      
    } // for j
  } // if listMvdHits
  
    if( listTrdHits )
  {

    for (int j=0; j <listTrdHits->GetEntries(); j++ ){
      TmpHit th;
      
        CbmTrdHit *mh = L1_DYNAMIC_CAST<CbmTrdHit*>( listTrdHits->At(j) );
        th.ExtIndex = j;
        th.Det = 3;
         /*  num variable not used
        int num =mh->GetPlaneId();
        
#ifdef mCBM        
        
        if ((mh->GetPlaneId())==0) num = 0;
        if ((mh->GetPlaneId())==1) num = 1;
        if ((mh->GetPlaneId())==2) num = 2;
        if ((mh->GetPlaneId())==3) num = 3;
        if ((mh->GetPlaneId())==4) num = 4;        
#endif   
//        if (num == 1) continue;
//        if (num == 3) continue;
        */
        
   //    for (int k=0; k <TrdHitsOnStation[num+1].size(); k++ ){
         
        th.iStation = NMvdStations + mh->GetPlaneId() + NStsStations+NMuchStations;
        
        
        th.time =  mh->GetTime();
        th.t_er = 5;// mh->GetTimeError();  
        
     //   th.iSector  = 0;
        th.isStrip  = 0;
        th.iStripF = j;
        th.iStripB = -1;//TrdHitsOnStationIndex[num+1][k];
        
        th.iStripF += nMvdHits+nStsHits+nMuchHits;
        th.iStripB += nMvdHits+nStsHits+nMuchHits;
  
        TVector3 pos, err;
        mh->Position(pos);
        mh->PositionError(err);

        th.x = pos.X();
        th.y = pos.Y();
        
        
        th.dx = fabs(mh->GetDx());
        th.dy = fabs(mh->GetDy());
        th.dxy = 0;
        
        th.du = fabs(mh->GetDx());
        th.dv = fabs(mh->GetDy());
        
//         CbmTrdHit mh1 = TrdHitsOnStation[num+1][k];
//         
//         TVector3 pos1, err1;
//         mh1.Position(pos1);
//         mh1.PositionError(err1);
//         
//         if (th.dx>th.dy){
//           
//         th.x = pos1.X();
//         
//         
//         th.dx = mh1.GetDx();
// 
//         
//         th.du = mh1.GetDx();
//           
//         }
//         
//         if (th.dy>th.dx){
// 
//         th.y = pos1.Y();
// 
//         th.dy = mh1.GetDy();
// 
//         th.dv = mh1.GetDy();  
//         }
        
        L1Station &st = algo->vStations[th.iStation];
        th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
        th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];
      
      th.iMC=-1;
      int iMC = -1;
    
            if (fPerformance)
      {
        
      if( fTrdHitMatches ){
        
        CbmMatch *trdHitMatch = L1_DYNAMIC_CAST<CbmMatch*>( fTrdHitMatches->At(j) );
        
      //  CbmMatch *trdHitMatch1 = L1_DYNAMIC_CAST<CbmMatch*>( fTrdHitMatches->At(TrdHitsOnStationIndex[num+1][k]) );

        
        if( trdHitMatch->GetNofLinks() > 0 )
          if( trdHitMatch->GetLink(0).GetIndex() < nTrdPoints )
          {
            iMC = trdHitMatch->GetLink(0).GetIndex();
            th.iMC = iMC+nMvdPoints+nStsPoints+nMuchPoints;
      //      th.track = vMCPoints[th.iMC].ID;
            
            CbmTrdPoint* pt = (CbmTrdPoint*) fTrdPoints->Get(trdHitMatch->GetLink(0).GetFile(),trdHitMatch->GetLink(0).GetEntry(),trdHitMatch->GetLink(0).GetIndex());
  
//            float min = 0.1;
            
//             if (min>th.dx) min = th.dx;
//             if (min>th.dy) min = th.dy;
            
           // if (num==3)
            {
            
//         th.x = pt->GetXIn();//+ gRandom->Gaus(0,min);
//         th.y = pt->GetYIn();//+ gRandom->Gaus(0,min);
         th.time = pt->GetTime();//+ gRandom->Gaus(0,th.t_er);
//         
//         th.dx = min;
//         th.dy = min;
//         
//         th.du = min;
//         th.dv = min;
//         
//         L1Station &st = algo->vStations[th.iStation];
//         th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
//         th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];
            }
          }
          
          
//          if( trdHitMatch1->GetNofLinks() > 0 )
//           if( trdHitMatch1->GetLink(0).GetIndex() < nTrdPoints )
//           {
//             
//             CbmTrdPoint* pt1 = (CbmTrdPoint*) fTrdPoints->Get(trdHitMatch1->GetLink(0).GetFile(),trdHitMatch1->GetLink(0).GetEntry(),trdHitMatch1->GetLink(0).GetIndex());
//             
//            if (mh->GetDx()>mh->GetDy()){
//           
//             th.dx = mh1.GetDx();
//             th.du = mh1.GetDx();
//             th.x = pt1->GetXOut()+ gRandom->Gaus(0,th.dx); 
//             }
//         
//            if (mh->GetDy()>mh->GetDx()){
// 
//             th.dy = mh1.GetDy();
//             th.dv = mh1.GetDy();
//             th.y = pt1->GetYOut()+ gRandom->Gaus(0,th.dy);  
//            }
//             
//             
//          L1Station &st = algo->vStations[th.iStation];
//         th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
//         th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];
// 
//           }
      
        }

      }
        tmpHits.push_back(th);
        nTrdHits++;
        
      //}
    } // for j
  } // if listTrdHits
  
    if( fTofHits )
  {

    for (int j=0; j <fTofHits->GetEntries(); j++ ){
      TmpHit th;
      
        CbmTofHit *mh = L1_DYNAMIC_CAST<CbmTofHit*>( fTofHits->At(j) );
        th.ExtIndex = j;
        th.Det = 4;
        th.iStation = NMvdStations + mh->GetPlaneId() + NStsStations+NMuchStations+NTrdStations;
        
        th.time =  mh->GetTime();

        th.t_er =  mh->GetTimeError();  
        
        th.dx = mh->GetDx();
        th.dy = mh->GetDy();
        th.dxy = 0;
        
        th.du = mh->GetDx();
        th.dv = mh->GetDy();

     //   th.iSector  = 0;
        th.isStrip  = 0;
        th.iStripF = j;
        th.iStripB = -1;
        if( th.iStripF<0 ) continue;
        if( th.iStripF>=0 && th.iStripB>=0 ) th.isStrip  = 1;
        if( th.iStripB <0 ) th.iStripB = th.iStripF;
        
        th.iStripF += nMvdHits+nStsHits+nMuchHits+nTrdHits;
        th.iStripB += nMvdHits+nStsHits+nMuchHits+nTrdHits;
  
        TVector3 pos, err;
        mh->Position(pos);
        mh->PositionError(err);

        th.x = pos.X();
        th.y = pos.Y();
  
        L1Station &st = algo->vStations[th.iStation];
        th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
        th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];
        
        
      
      th.iMC=-1;
//      int iMC = -1;
      
      
      
      if (fPerformance)
      {
        
     //   if (ToFPointsMatch[j]!=0) 

      //  th.iMC = j+nMvdPoints+nStsPoints+nTrdPoints+nMuchPoints;
        
        Int_t iFile  = ToFPointsMatch[j]->GetFile();
        Int_t iEvent = ToFPointsMatch[j]->GetEntry();

        Int_t iIndex = ToFPointsMatch[j]->GetIndex() + nMvdPoints+nStsPoints+nMuchPoints+nTrdPoints;
        
        Double_t dtrck =dFEI(iFile, iEvent, iIndex);
        DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
        if (trk_it==dFEI2vMCPoints.end()) continue;
        th.iMC = trk_it->second;
//         th.track = vMCPoints[th.iMC].ID;
//         th.qp = vMCPoints[th.iMC].p;
        
//        CbmTofPoint* pt = (CbmTofPoint*) fTofPoints->Get(ToFPointsMatch[j]->GetFile(),ToFPointsMatch[j]->GetEntry(),ToFPointsMatch[j]->GetIndex());
   
        
//                 th.x = pt->GetX() + gRandom->Gaus(0,th.dx);
// 
//         th.y = pt->GetY()+ gRandom->Gaus(0,th.dy);
//         th.time =  pt->GetTime()+ gRandom->Gaus(0,th.t_er);
//         
//                 L1Station &st = algo->vStations[th.iStation];
//         th.u_front = th.x*st.frontInfo.cos_phi[0] + th.y*st.frontInfo.sin_phi[0];
//         th.u_back  = th.x* st.backInfo.cos_phi[0] + th.y*st.backInfo.sin_phi[0];


      }
      

      
        tmpHits.push_back(th);
        nTofHits++;
      
    } // for j
  } // if listTrdHits
  
  if (fVerbose >= 10) cout << "ReadEvent: sts hits are gotten." << endl;

    // sort hits
  int nHits = nMvdHits + nStsHits + nMuchHits+nTrdHits+nTofHits;

  sort( tmpHits.begin(), tmpHits.end(), TmpHit::Compare );

    // -- create strips --
  int NStrips = 0, NStripsB = 0;
  for ( int ih = 0; ih<nHits; ih++ ){
    TmpHit &th = tmpHits[ih];
    
      // try to find the respective front and back strip from the already created
    th.indStripF = -1;
    th.indStripB = -1;
    for( int is = 0; is<NStrips; is++ ){
      TmpStrip &s = tmpStrips[is];
      if( s.iStation!=th.iStation ) continue;
      if( s.iStrip!=th.iStripF ) continue;
      if( fabs(s.time - th.time)>20 ) continue;
      if( fabs(s.u - th.u_front)>1.e-4 ) continue;
      th.indStripF = is;
    }
    for( int is = 0; is<NStripsB; is++ ){
      TmpStrip &s = tmpStripsB[is];
      if( s.iStation!=th.iStation ) continue;
      if( s.iStrip!=th.iStripB ) continue;
      if( fabs(s.time - th.time)>20 ) continue;
      if( fabs(s.u - th.u_back)>1.e-4 ) continue;
      th.indStripB = is;
    }
      // create new strips
    if( th.indStripF<0 ){
      TmpStrip tmp;
      tmp.iStation = th.iStation;
      tmp.iStrip = th.iStripF;
      tmp.u = th.u_front;
      tmp.time = th.time;
      tmp.isStrip = th.isStrip;
      tmpStrips.push_back(tmp);
      th.indStripF = NStrips++;
    }
    if( th.indStripB<0 ){
      TmpStrip tmp1;
      tmp1.iStation = th.iStation;
      tmp1.iStrip = th.iStripB;
      tmp1.isStrip = th.isStrip;
      tmp1.u = th.u_back;
      tmp1.time = th.time;
      tmpStripsB.push_back(tmp1);
      th.indStripB = NStripsB++;
    }
  } // ih
  
    // take into account unefficiency and save strips in L1Algo
  Int_t NEffStrips = 0, NEffStripsB = 0;
  for( int i=0; i<NStrips; i++ ){
    TmpStrip &ts = tmpStrips[i];
      //     if( ts.effIndex == -1 ){
    ts.effIndex = NEffStrips++;
    char flag = ts.iStation*4;

    fData_->vStsStrips.push_back(ts.u);
    fData_->vSFlag.push_back(flag);
  }
  for( int i=0; i<NStripsB; i++ ){
    TmpStrip &ts = tmpStripsB[i];
      //     if( ts.effIndex == -1 ){
    ts.effIndex = NEffStripsB++;
    char flag = ts.iStation*4;

    fData_->vStsStripsB.push_back(ts.u);
    fData_->vSFlagB.push_back(flag);
  }


  if (fVerbose >= 10) cout << "ReadEvent: strips are read." << endl;
  
    // -- save hits --
  int nEffHits = 0;
  vector<float> vStsZPos_temp; // temp array for unsorted z positions of detectors segments
  for( int i=0; i<nHits; i++ )
  {
    TmpHit &th = tmpHits[i];
 
    CbmL1HitStore s;
    s.Det = th.Det;
    s.ExtIndex = th.ExtIndex;
    s.iStation = th.iStation;
    s.x        = th.x;
    s.y        = th.y;
    s.dx        = th.dx;
    s.dy        = th.dy;
    s.dxy        = th.dxy;
    s.time = th.time; 

    if( th.indStripF <0 || th.indStripF >= NStrips ) continue;
    if( th.indStripB <0 || th.indStripB >= NStripsB ) continue;

    TmpStrip &tsF = tmpStrips [th.indStripF];
    TmpStrip &tsB = tmpStripsB[th.indStripB];

    L1StsHit h;
    h.f = tsF.effIndex;
    h.b = tsB.effIndex;

    h.t_reco = th.time; 
    h.t_er = th.t_er; 
  //  h.track = th.track;
    h.dx   = th.dx;
    h.dy   = th.dy;
    h.du   = th.du;
    h.dv   = th.dv;
    h.dxy   = th.dxy;
//     h.p = th.p;
//     h.q = th.q;
    h.ista = th.iStation;

    // find and save z positions
    float z_tmp = -111.;
    int ist = th.iStation;
        if (ist < NMvdStations){
#ifdef MVDIDEALHITS
      CbmMvdPoint* point = L1_DYNAMIC_CAST<CbmMvdPoint*>(listMvdPts->At(- s.ExtIndex - 1));
      z_tmp = 0.5 * ( point->GetZOut() + point->GetZ() );
#else
      CbmMvdHit *mh_m = L1_DYNAMIC_CAST<CbmMvdHit*>( listMvdHits->At(- s.ExtIndex - 1));
      z_tmp = mh_m->GetZ();
#endif
    }
    
    if ((ist >= NMvdStations)&&(ist < (NStsStations+NMvdStations)))
    {
#ifdef STSIDEALHITS
      CbmStsPoint* point = L1_DYNAMIC_CAST<CbmStsPoint*>(listStsPts->At(s.ExtIndex));
      z_tmp = 0.5 * ( point->GetZOut() + point->GetZIn() );
#else
      CbmStsHit *mh_m = L1_DYNAMIC_CAST<CbmStsHit*>( listStsHits->At(s.ExtIndex));
      z_tmp = mh_m->GetZ();
#endif
    }
    
    if ((ist >= NStsStations+NMvdStations)&&(ist < (NStsStations+NMvdStations+NMuchStations))) {
//#ifdef STSIDEALHITS
    //  CbmStsPoint* point = L1_DYNAMIC_CAST<CbmStsPoint*>(listStsPts->At(s.ExtIndex));
   //   z_tmp = 0.5 * ( point->GetZOut() + point->GetZIn() );
//#else
      CbmMuchPixelHit* mh = static_cast<CbmMuchPixelHit*>(fMuchPixelHits->At(s.ExtIndex) );
      z_tmp = mh->GetZ();
//#endif
    }
    
        if ((ist >= NStsStations+NMvdStations+NMuchStations)&&(ist < (NStsStations+NMvdStations+NMuchStations+NTrdStations))) {
//#ifdef STSIDEALHITS
    //  CbmStsPoint* point = L1_DYNAMIC_CAST<CbmStsPoint*>(listStsPts->At(s.ExtIndex));
   //   z_tmp = 0.5 * ( point->GetZOut() + point->GetZIn() );
//#else
      CbmTrdHit* mh = static_cast<CbmTrdHit*>(listTrdHits->At( s.ExtIndex) );
      z_tmp = mh->GetZ();
//#endif
    }
    
        if ((ist >= NStsStations+NMvdStations+NMuchStations+NTrdStations+NTOFStation)&&(ist < (NStsStations+NMvdStations+NMuchStations+NTrdStations))) {
//#ifdef STSIDEALHITS
    //  CbmStsPoint* point = L1_DYNAMIC_CAST<CbmStsPoint*>(listStsPts->At(s.ExtIndex));
   //   z_tmp = 0.5 * ( point->GetZOut() + point->GetZIn() );
//#else
      CbmTofHit* mh = static_cast<CbmTofHit*>(fTofHits->At( s.ExtIndex ) );
      z_tmp = mh->GetZ();
//#endif
    }

    unsigned int k;
    for (k = 0; k < vStsZPos_temp.size(); k++){
      if (vStsZPos_temp[k] == z_tmp){
        h.iz = k;
        break;
      }
    }
    if (k == vStsZPos_temp.size()){
      h.iz = vStsZPos_temp.size();
      vStsZPos_temp.push_back(z_tmp);
    }
    
    // save hit
    vStsHits.push_back( CbmL1StsHit(fData->vStsHits.size(), th.ExtIndex, th.Det ) );
        
    vStsHits[vStsHits.size()-1].x = th.x;
    vStsHits[vStsHits.size()-1].y = th.y;
    vStsHits[vStsHits.size()-1].t = th.time;
        


    fData_->vStsHits.push_back(h);
    
    int sta = th.iStation;

    if (fData_->StsHitsStartIndex[sta] == static_cast<THitI>(-1)) fData_->StsHitsStartIndex[sta] = nEffHits;
    nEffHits++;

    fData_->StsHitsStopIndex[sta] = nEffHits;
    
    vHitStore.push_back(s);
    vHitMCRef.push_back(th.iMC);

  }
  

  for(int i = 0; i < NStation; i++){

     if (fData_->StsHitsStartIndex[i] == static_cast<THitI>(-1)) fData_->StsHitsStartIndex[i] = fData_->StsHitsStopIndex[i];
  }

  if (fVerbose >= 10) cout << "ReadEvent: mvd and sts are saved." << endl;

  
    // sort z-pos
  if (vStsZPos_temp.size() != 0){
    vector<float> vStsZPos_temp2;
    vStsZPos_temp2.clear();
    vStsZPos_temp2.push_back(vStsZPos_temp[0]);
    vector<int> newToOldIndex;
    newToOldIndex.clear();
    newToOldIndex.push_back(0);

    for (unsigned int k = 1; k < vStsZPos_temp.size(); k++){
      vector<float>::iterator itpos = vStsZPos_temp2.begin()+1;
      vector<int>::iterator iti = newToOldIndex.begin()+1;
      for (; itpos < vStsZPos_temp2.end(); itpos++, iti++){
        if (vStsZPos_temp[k] < *itpos){
          vStsZPos_temp2.insert(itpos,vStsZPos_temp[k]);
          newToOldIndex.insert(iti,k);
          break;
        }
      }
      if (itpos == vStsZPos_temp2.end()){
        vStsZPos_temp2.push_back(vStsZPos_temp[k]);
        newToOldIndex.push_back(k);
      }
    } // k


    if (fVerbose >= 10) cout << "ReadEvent: z-pos are sorted." << endl;
    
    for (unsigned int k = 0; k < vStsZPos_temp2.size(); k++)
      fData_->vStsZPos.push_back(vStsZPos_temp2[k]);

    int size_nto_tmp = newToOldIndex.size();
    vector<int> oldToNewIndex;
    oldToNewIndex.clear();
    oldToNewIndex.resize(size_nto_tmp);
    for (int k = 0; k < size_nto_tmp; k++)
      oldToNewIndex[newToOldIndex[k]] = k;
    
    int size_hs_tmp = vHitStore.size();
    for (int k = 0; k < size_hs_tmp; k++)
      fData_->vStsHits[k].iz = oldToNewIndex[fData_->vStsHits[k].iz];
  }

  if (fVerbose >= 10) cout << "ReadEvent: z-pos are saved." << endl;

  algo->SetData(
    fData_->GetStsHits(),
    fData_->GetStsStrips(),
    fData_->GetStsStripsB(),
    fData_->GetStsZPos(),
    fData_->GetSFlag(),
    fData_->GetSFlagB(),
    fData_->GetStsHitsStartIndex(),
    fData_->GetStsHitsStopIndex()
    );

  if (fPerformance){

    if (fVerbose >= 10) cout << "HitMatch is done." << endl;
    if (fVerbose >= 10) cout << "MCPoints and MCTracks are saved." << endl;
  }

  if (fVerbose >= 10) cout << "ReadEvent is done." << endl;


} // void CbmL1::ReadEvent()


void CbmL1::Fill_vMCTracks()
{
  PrimVtx.MC_ID = 999;
  {
    CbmL1Vtx Vtxcurr;
    int nvtracks=0, nvtrackscurr=0;

    vMCTracks.clear();
  
    for( DFSET::iterator set_it = vFileEvent.begin(); set_it!=vFileEvent.end(); ++set_it)
    {
      Int_t iFile = set_it->first;
      Int_t iEvent = set_it->second;
      
      
      Int_t nMCTrack =fMCTracks->Size(iFile, iEvent);


      for(Int_t iMCTrack= 0; iMCTrack < nMCTrack; iMCTrack++)
      {
        CbmMCTrack *MCTrack =  L1_DYNAMIC_CAST<CbmMCTrack*>(fMCTracks->Get(iFile,iEvent,iMCTrack) );  
        if (!MCTrack) continue;

        int mother_ID = MCTrack->GetMotherId();

        
        if (mother_ID<0&&mother_ID!=-2) mother_ID= -iEvent-1;
        TVector3 vr;
        TLorentzVector vp;
        MCTrack->GetStartVertex(vr);
        MCTrack->Get4Momentum(vp);

        Int_t pdg = MCTrack->GetPdgCode();
        Double_t q=1, mass = 0.;
        if ( pdg < 9999999 && ( (TParticlePDG *)TDatabasePDG::Instance()->GetParticle(pdg) ))
        {
          q = TDatabasePDG::Instance()->GetParticle(pdg)->Charge()/3.0;
          mass = TDatabasePDG::Instance()->GetParticle(pdg)->Mass(); 
        }
        else    q = 0;
        Int_t IND_Track = vMCTracks.size(); //or iMCTrack?
        CbmL1MCTrack T(mass, q, vr, vp, IND_Track, mother_ID, pdg);  
//        CbmL1MCTrack T(mass, q, vr, vp, iMCTrack, mother_ID, pdg);
        T.time = MCTrack->GetStartT();
        T.iFile = iFile;
        T.iEvent = iEvent;

        vMCTracks.push_back( T );
     //    Double_t dtrck =dFEI(iFile,iEvent,iMCTrack);
         dFEI2vMCTracks.insert( DFEI2I::value_type(dFEI(iFile,iEvent,iMCTrack), vMCTracks.size()-1) );

        if ( T.mother_ID <0 ){ // vertex track
          if (  PrimVtx.MC_ID == 999 || fabs(T.z-Vtxcurr.MC_z)>1.e-7 ){// new vertex
            if( nvtrackscurr > nvtracks ){
              PrimVtx = Vtxcurr;
              nvtracks = nvtrackscurr;
            }
            Vtxcurr.MC_x  = T.x;
            Vtxcurr.MC_y  = T.y;
            Vtxcurr.MC_z  = T.z;
            Vtxcurr.MC_ID = T.mother_ID;
            nvtrackscurr = 1;
          }
          else nvtrackscurr++;
        }
      }//iTracks
    } //Links
    if( nvtrackscurr > nvtracks ) PrimVtx = Vtxcurr;
  } //PrimVtx

  if ( fVerbose && PrimVtx.MC_ID == 999 ) cout<<"No primary vertex !!!"<<endl;
} //Fill_vMCTracks
 
bool CbmL1::ReadMCPoint( CbmL1MCPoint *MC, int iPoint, int file, int event, int MVD ) 
{
  TVector3 xyzI,PI, xyzO,PO;
  Int_t mcID=-1;
  Double_t time = 0.f;
  if( MVD==1){
   CbmMvdPoint *pt = L1_DYNAMIC_CAST<CbmMvdPoint*> ( fMvdPoints->Get(file,event,iPoint) ); // file, event, object
    //CbmMvdPoint *pt = L1_DYNAMIC_CAST<CbmMvdPoint*> (Point);
    
    if ( !pt ) return 1;
    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();
  }
  if( MVD==0)
  {  
    CbmStsPoint *pt = L1_DYNAMIC_CAST<CbmStsPoint*>(fStsPoints->Get(file,event,iPoint) ); // file, event, object   
    if ( !pt ) return 1;
//     if ( fTimesliceMode )
//     {
//       Double_t StartTime = fTimeSlice->GetStartTime();
//       Double_t EndTime = fTimeSlice->GetEndTime();
//       Double_t Time_MC_point =  pt->GetTime() + fEventList->GetEventTime(event, file);
//       if (Time_MC_point < StartTime ) 
//         return 1;
// 
//       if (Time_MC_point > EndTime ) 
//         return 1; 
//     } //if ( fTimesliceMode )

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();
  }
  
  
  if( MVD==2)
  {  
    CbmMuchPoint *pt = L1_DYNAMIC_CAST<CbmMuchPoint*>(fMuchPoints->Get(file,event,iPoint) ); // file, event, object   
    if ( !pt ) return 1;
    if ( fTimesliceMode )
    {
      Double_t StartTime = fTimeSlice->GetStartTime();
      Double_t EndTime = fTimeSlice->GetEndTime();
      Double_t Time_MC_point =  pt->GetTime() + fEventList->GetEventTime(event, file);
      if (Time_MC_point < StartTime ) 
        return 1;

      if (Time_MC_point > EndTime ) 
        return 1; 
    } //if ( fTimesliceMode )

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->Momentum(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();
  }
  
    
      if( MVD==3)
  {  
    CbmTrdPoint *pt = L1_DYNAMIC_CAST<CbmTrdPoint*>(fTrdPoints->Get(file,event,iPoint) ); // file, event, object   

    if ( !pt ) return 1;
    if ( fTimesliceMode )
    {
      //Double_t StartTime = fTimeSlice->GetStartTime();  // not used
      //Double_t EndTime = fTimeSlice->GetEndTime();      // not used
      //Double_t Time_MC_point =  pt->GetTime() + fEventList->GetEventTime(event, file); // not used
//       if (Time_MC_point < StartTime ) 
//         return 1;
// 
//       if (Time_MC_point > EndTime ) 
//         return 1; 
    } //if ( fTimesliceMode )

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->PositionOut(xyzO);
    pt->MomentumOut(PO);
    mcID = pt->GetTrackID();
    
    time = pt->GetTime();

  }
  
   if( MVD==4)
  {  
    CbmTofPoint *pt = L1_DYNAMIC_CAST<CbmTofPoint*>(fTofPoints->Get(file,event,iPoint) ); // file, event, object   
    if ( !pt ) return 1;
    if ( fTimesliceMode )
    {
      Double_t StartTime = fTimeSlice->GetStartTime();
      Double_t EndTime = fTimeSlice->GetEndTime();
      Double_t Time_MC_point =  pt->GetTime() + fEventList->GetEventTime(event, file);
      if (Time_MC_point < StartTime ) 
        return 1;

      if (Time_MC_point > EndTime ) 
        return 1; 
    } //if ( fTimesliceMode )

    pt->Position(xyzI);
    pt->Momentum(PI);
    pt->Position(xyzO);
    pt->Momentum(PO);
    mcID = pt->GetTrackID();
    time = pt->GetTime();

  }

  TVector3 xyz = .5*(xyzI + xyzO );
  TVector3 P = .5*(PI + PO );
  MC->x  = xyz.X();
  MC->y  = xyz.Y();
  MC->z  = xyz.Z();
  MC->px = P.X();
  MC->py = P.Y();
  MC->pz = P.Z();
  MC->xIn  = xyzI.X();
  MC->yIn  = xyzI.Y();
  MC->zIn  = xyzI.Z();
  MC->pxIn = PI.X();
  MC->pyIn = PI.Y();
  MC->pzIn = PI.Z();
  MC->xOut  = xyzO.X();
  MC->yOut  = xyzO.Y();
  MC->zOut  = xyzO.Z();
  MC->pxOut = PO.X();
  MC->pyOut = PO.Y();
  MC->pzOut = PO.Z();
  MC->p = sqrt( fabs( MC->px*MC->px + MC->py*MC->py + MC->pz*MC->pz ) );
  MC->ID = mcID;
  MC->pointId = iPoint;
  MC->file = file;
  MC->event = event; 
  
  MC->time = time;
  
  if(MC->ID < 0)
    return 1;
  CbmMCTrack *MCTrack =  L1_DYNAMIC_CAST<CbmMCTrack*>(fMCTracks->Get(file,event,  MC->ID) );
  if ( !MCTrack ) return 1;
  MC->pdg  = MCTrack->GetPdgCode();
  MC->mother_ID = MCTrack->GetMotherId();

  TParticlePDG* particlePDG = TDatabasePDG::Instance()->GetParticle(MC->pdg);
  if(particlePDG)
  {
    MC->q = particlePDG->Charge()/3.0;
    MC->mass = particlePDG->Mass();
  }

  return 0;
}

bool CbmL1::ReadMCPoint( CbmL1MCPoint* /*MC*/, int /*iPoint*/, int /*MVD*/ )
{
  return 0;
}

/// Procedure for match hits and MCPoints.
/// Read information about correspondence between hits and mcpoints and fill CbmL1MCPoint::hitIds and CbmL1StsHit::mcPointIds arrays
/// should be called after fill of algo
void CbmL1::HitMatch()
{    
  const int NHits = vStsHits.size();
  for (int iH = 0; iH < NHits; iH++)
  {
    CbmL1StsHit& hit = vStsHits[iH];
    
   if (  hit.Det == 1){
    CbmStsHit *sh = L1_DYNAMIC_CAST<CbmStsHit*>( listStsHits->At(vStsHits[iH].extIndex) );

    int iP = -1;  
     
    vector <int> iEvent1;

     
    if(listStsClusterMatch)
    {

      const CbmMatch* frontClusterMatch = static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetFrontClusterId()));
      const CbmMatch* backClusterMatch  = static_cast<const CbmMatch*>(listStsClusterMatch->At(sh->GetBackClusterId()));
      CbmMatch stsHitMatch;

      Float_t Sum_of_front = 0;
      Float_t Sum_of_back = 0;


      for(Int_t iFrontLink = 0; iFrontLink<frontClusterMatch->GetNofLinks(); iFrontLink++)
      {
        const CbmLink& frontLink = frontClusterMatch->GetLink(iFrontLink);
        Sum_of_front = Sum_of_front + frontLink.GetWeight();
      }

      for(Int_t iBackLink = 0; iBackLink<backClusterMatch->GetNofLinks(); iBackLink++){
        const CbmLink& backLink = backClusterMatch->GetLink(iBackLink);
        Sum_of_back = Sum_of_back + backLink.GetWeight();
      }

      for(Int_t iFrontLink = 0; iFrontLink<frontClusterMatch->GetNofLinks(); iFrontLink++)
      {
        const CbmLink& frontLink = frontClusterMatch->GetLink(iFrontLink);

       // Float_t Fraction_front = frontLink.GetWeight()/Sum_of_front;

        for(Int_t iBackLink = 0; iBackLink<backClusterMatch->GetNofLinks(); iBackLink++){
          const CbmLink& backLink = backClusterMatch->GetLink(iBackLink);

       // Float_t  Fraction_back = backLink.GetWeight()/Sum_of_back;

          if (frontLink == backLink){
            stsHitMatch.AddLink(frontLink);
            stsHitMatch.AddLink(backLink);
          }
        }
      }        

      Float_t bestWeight = 0.f;
      Float_t totalWeight = 0.f;
      for(Int_t iLink=0; iLink < stsHitMatch.GetNofLinks(); iLink++)
      {
        Int_t iFile  = stsHitMatch.GetLink(iLink).GetFile();
        Int_t iEvent = stsHitMatch.GetLink(iLink).GetEntry();
        Int_t iIndex = stsHitMatch.GetLink(iLink).GetIndex();
        
        iEvent1.push_back(iEvent);

    
        if ( !fTimesliceMode )
        {
          iFile  = vFileEvent.begin()->first;
          iEvent = vFileEvent.begin()->second;
        }
        int nMvdPoints_ = 0;
        if(listMvdPts)
          nMvdPoints_ = listMvdPts->GetEntriesFast();
        Double_t dtrck =dFEI(iFile, iEvent, iIndex+nMvdPoints_);
        DFEI2I::iterator trk_it = dFEI2vMCPoints.find(dtrck);
        
        if (trk_it==dFEI2vMCPoints.end()) continue;
        
        totalWeight += stsHitMatch.GetLink(iLink).GetWeight();
        if( stsHitMatch.GetLink(iLink).GetWeight() > bestWeight)
        {
          bestWeight = stsHitMatch.GetLink(iLink).GetWeight();
          iP = trk_it->second;
        }
      }
    } //mach cluster


    if ( iP >= 0 )
    {
      
      for(unsigned int i=0; i < iEvent1.size(); i++)
      {
        hit.event = iEvent1[i];
      }

      hit.event=vMCPoints[iP].event;
      hit.mcPointIds.push_back( iP );
      vMCPoints[iP].hitIds.push_back(iH);
    }  
    else 
    {
      hit.event = -1; 
      int idPoint = vHitMCRef[iH];
      if (idPoint >= 0){
        hit.mcPointIds.push_back( idPoint );
        vMCPoints[idPoint].hitIds.push_back(iH);
      }
    } // if no clusters 
  }

  
  if (  hit.Det == 0){ // if no use Links or this is mvd hit

      int iP = vHitMCRef[iH];
      if (iP >= 0){
        hit.mcPointIds.push_back( iP );
        vMCPoints[iP].hitIds.push_back(iH);
      }
  }
  
     if (  hit.Det == 2){ // if no use Links or this is mvd hit

      int iP = vHitMCRef[iH];

      if (iP >= 0){
        hit.mcPointIds.push_back( iP );
        vMCPoints[iP].hitIds.push_back(iH);
      }
    }
    
     if (  hit.Det == 3){ // if no use Links or this is mvd hit


      int iP = vHitMCRef[iH];
      if (iP >= 0){
        hit.mcPointIds.push_back( iP );
        vMCPoints[iP].hitIds.push_back(iH);
      }
    }
    
         if (  hit.Det == 4){ // if no use Links or this is mvd hit


      int iP = vHitMCRef[iH];

      if (iP >= 0){
        hit.mcPointIds.push_back( iP );
        vMCPoints[iP].hitIds.push_back(iH);
      }
    }
  
  

  } // for hits
}


