/* Copyright (C) 2010-2021 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer] */

#ifndef L1AlgoTBB_h
#define L1AlgoTBB_h


#include "L1Algo.h"

#ifdef TBB

#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"

namespace tbb
{

  class ParalleledDup {
    // input
    int isec;
    L1Station* vStations;
    int NStations;
    Tindex* HitsStartIndex;
    Tindex* HitsStopIndex;
    vector<unsigned char>& vSFlag;
    vector<unsigned char>& vSFlagB;
    std::vector<L1HitPoint>& vHits;
    double Pick_r;
    double Pick_m;
    double MaxInvMom;
    fvec targX;
    fvec targY;
    fvec targZ;
    L1FieldValue& targB;
    L1XYMeasurementInfo& TargetXYInfo;

    Tindex* n_g1;
    unsigned* portionStopIndex;
    L1TrackPar* T_g1;
    L1FieldRegion* fld_g1;
    L1HitIndex_t* hitsl_g1;
    // 	fvec *x_minusV_g1; fvec *x_plusV_g1; fvec *y_minusV_g1; fvec *y_plusV_g1;
    // output
    L1HitIndex_t* Duplets_start;
    L1HitIndex_t* Duplets_hits;

    Tindex* n_g2;
    L1HitIndex_t* i1_g2;
    // 	L1TrackPar *T_g2;
    // 	L1FieldRegion *fld_g2;
    // 	fvec *u_front_g2; fvec *u_back_g2;
    // 	L1HitIndex_t *hitsl_g2;
    L1HitIndex_t* hitsm_g2;
    // 	fvec *x_minusV_g2; fvec *x_plusV_g2; fvec *y_minusV_g2; fvec *y_plusV_g2;

  public:
    void operator()(const blocked_range<size_t>& r) const;

    ParalleledDup(
      // input
      int isec_, L1Station* vStations_, int NStations_, Tindex* HitsStartIndex_, Tindex* HitsStopIndex_,
      vector<unsigned char>& vSFlag_, vector<unsigned char>& vSFlagB_, std::vector<L1HitPoint>& vHits_, double Pick_r_,
      double Pick_m_, double MaxInvMom_, fvec targX_, fvec targY_, fvec targZ_, L1FieldValue& targB_,
      L1XYMeasurementInfo& TargetXYInfo_,

      Tindex* n_g1_, unsigned* portionStopIndex_, L1TrackPar* T_g1_, L1FieldRegion* fld_g1_, L1HitIndex_t* hitsl_g1_,
      // 								fvec *x_minusV_g1_, fvec *x_plusV_g1_, fvec *y_minusV_g1_, fvec *y_plusV_g1_,
      // output
      L1HitIndex_t* Duplets_start_, L1HitIndex_t* Duplets_hits_,

      Tindex* n_g2_, L1HitIndex_t* i1_g2_,
      // 								L1TrackPar *T_g2_,
      // 								L1FieldRegion *fld_g2_,
      // 								fvec *u_front_g2_, fvec *u_back_g2_,
      // 								L1HitIndex_t *hitsl_g2_,
      L1HitIndex_t* hitsm_g2_
      // 								fvec *x_minusV_g2_, fvec *x_plusV_g2_, fvec *y_minusV_g2_, fvec *y_plusV_g2_
      )
      : isec(isec_)
      , vStations(vStations_)
      , NStations(NStations_)
      , HitsStartIndex(HitsStartIndex_)
      , HitsStopIndex(HitsStopIndex_)
      , vSFlag(vSFlag_)
      , vSFlagB(vSFlagB_)
      , vHits(vHits_)
      , Pick_r(Pick_r_)
      , Pick_m(Pick_m_)
      , MaxInvMom(MaxInvMom_)
      , targX(targX_)
      , targY(targY_)
      , targZ(targZ_)
      , targB(targB_)
      , TargetXYInfo(TargetXYInfo_)
      ,

      n_g1(n_g1_)
      , portionStopIndex(portionStopIndex_)
      , T_g1(T_g1_)
      , fld_g1(fld_g1_)
      , hitsl_g1(hitsl_g1_)
      ,
      // 											x_minusV_g1(x_minusV_g1_), x_plusV_g1(x_plusV_g1_), y_minusV_g1(y_minusV_g1_), y_plusV_g1(y_plusV_g1_),
      // output
      Duplets_start(Duplets_start_)
      , Duplets_hits(Duplets_hits_)
      ,

      n_g2(n_g2_)
      , i1_g2(i1_g2_)
      ,
      // 											T_g2(T_g2_),
      // 											fld_g2(fld_g2_),
      // 											u_front_g2(u_front_g2_), u_back_g2(u_back_g2_),
      // 											hitsl_g2(hitsl_g2_),
      hitsm_g2(hitsm_g2_)
      // 											x_minusV_g2(x_minusV_g2_), x_plusV_g2(x_plusV_g2_), y_minusV_g2(y_minusV_g2_), y_plusV_g2(y_plusV_g2_)
      {};

    ~ParalleledDup() {};
  };

  void ParalleledDup::operator()(const blocked_range<size_t>& r) const
  {

    for (Tindex istal = r.begin(); istal < (Tindex) r.end(); ++istal) {
      // 		cout<< " ParalleledDup::operator(). Station: " << istal << endl;
      DupletsStaPort(  // input
        isec, istal, vHits, targX, targY, targZ, targB, TargetXYInfo,

        n_g1, portionStopIndex, T_g1, fld_g1, hitsl_g1,
        // 											x_minusV_g1, x_plusV_g1, y_minusV_g1, y_plusV_g1,
        // output
        Duplets_start, Duplets_hits,

        n_g2, i1_g2,
        // 											T_g2,
        // 											fld_g2,
        // 											u_front_g2, u_back_g2,
        // 											hitsl_g2,
        hitsm_g2
        // 											x_minusV_g2, x_plusV_g2, y_minusV_g2, y_plusV_g2
      );
    }
  }


  class ParalleledTrip {
    // input
    int isec;
    L1Station* vStations;
    int NStations;
    Tindex* HitsStartIndex;
    Tindex* HitsStopIndex;
    std::vector<L1HitPoint>& svHits;
    double Pick_r;
    double TRACK_CHI2_CUT;
    double MaxInvMom;

    Tindex* n_g1;
    L1TrackPar* T_g1;
    L1FieldRegion* fld_g1;
    L1HitIndex_t* hitsl_g1;

    Tindex* n_g2;
    unsigned* portionStopIndex;
    L1HitIndex_t* i1_g2;
    /*	L1FieldRegion *fld_g2;
	fvec *u_front_g2; fvec *u_back_g2;
	L1TrackPar *T_g2;*/
    // 	L1HitIndex_t *hitsl_g2;
    L1HitIndex_t* hitsm_g2;
    // 	fvec *x_minusV_g2; fvec *x_plusV_g2; fvec *y_minusV_g2; fvec *y_plusV_g2;
    // output
    L1HitIndex_t* Duplets_start;
    L1HitIndex_t* Duplets_hits;
    std::vector<L1Triplet>* vTriplets_part;
    unsigned* TripStartIndexH;
    unsigned* TripStopIndexH;

  public:
    void operator()(const blocked_range<size_t>& r) const;

    ParalleledTrip(
      // input
      int isec_, L1Station* vStations_, int NStations_, Tindex* HitsStartIndex_, Tindex* HitsStopIndex_,
      std::vector<L1HitPoint>& vHits_, double Pick_r_, double TRACK_CHI2_CUT_, double MaxInvMom_,

      Tindex* n_g1_, L1TrackPar* T_g1_, L1FieldRegion* fld_g1_, L1HitIndex_t* hitsl_g1_,

      Tindex* n_g2_, unsigned* portionStopIndex_, L1HitIndex_t* i1_g2_,
      /*L1FieldRegion *fld_g2_,
								fvec *u_front_g2_, fvec *u_back_g2_,
								L1TrackPar *T_g2_,*/
      // 								L1HitIndex_t *hitsl_g2_,
      L1HitIndex_t* hitsm_g2_,
      // 								fvec *x_minusV_g2_, fvec *x_plusV_g2_, fvec *y_minusV_g2_, fvec *y_plusV_g2_,
      // output
      L1HitIndex_t* Duplets_start_, L1HitIndex_t* Duplets_hits_, std::vector<L1Triplet>* vTriplets_part_,
      unsigned* TripStartIndexH_,
      unsigned* TripStopIndexH_)
      :  // input
      isec(isec_)
      , vStations(vStations_)
      , NStations(NStations_)
      , HitsStartIndex(HitsStartIndex_)
      , HitsStopIndex(HitsStopIndex_)
      , svHits(vHits_)
      , Pick_r(Pick_r_)
      , TRACK_CHI2_CUT(TRACK_CHI2_CUT_)
      , MaxInvMom(MaxInvMom_)
      ,

      n_g1(n_g1_)
      , T_g1(T_g1_)
      , fld_g1(fld_g1_)
      , hitsl_g1(hitsl_g1_)
      ,

      n_g2(n_g2_)
      , portionStopIndex(portionStopIndex_)
      , i1_g2(i1_g2_)
      ,
      /*											fld_g2(fld_g2_),
											u_front_g2(u_front_g2_), u_back_g2(u_back_g2_),
											T_g2(T_g2_),*/
      // 											hitsl_g2(hitsl_g2_),
      hitsm_g2(hitsm_g2_)
      ,
      // 											x_minusV_g2(x_minusV_g2_), x_plusV_g2(x_plusV_g2_), y_minusV_g2(y_minusV_g2_), y_plusV_g2(y_plusV_g2_),
      // output
      Duplets_start(Duplets_start_)
      , Duplets_hits(Duplets_hits_)
      , vTriplets_part(vTriplets_part_)
      , TripStartIndexH(TripStartIndexH_)
      , TripStopIndexH(TripStopIndexH_) {};

    ~ParalleledTrip() {};
  };

  void ParalleledTrip::operator()(const blocked_range<size_t>& r) const
  {

    for (Tindex istal = r.begin(); istal < (Tindex) r.end(); ++istal) {
      // 		cout<< " ParalleledTrip::operator(). Station: " << istal << endl;
      TripletsStaPort(  // input
        isec, istal, svHits, n_g1, T_g1, fld_g1, hitsl_g1,

        n_g2, portionStopIndex, i1_g2,
        /*											fld_g2,
											u_front_g2, u_back_g2,
											T_g2,*/
        // 											hitsl_g2,
        hitsm_g2,
        // 											x_minusV_g2, x_plusV_g2, y_minusV_g2, y_plusV_g2,
        // output
        Duplets_start, Duplets_hits, vTriplets_part, TripStartIndexH, TripStopIndexH);
    }
  }
}  //namespace tbb

#endif  // TBB

#endif
