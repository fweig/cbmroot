/* Copyright (C) 2005-2016 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Dmytro Kresan, Norbert Herrmann [committer] */

//  ----------------------------------------------------------------------
//  -----                         CbmKFTofHit                        -----
//  -----               Created 12/12/2005  by D.Kresan              -----
//  ----------------------------------------------------------------------

#include "CbmKFTofHit.h"

#include "CbmKF.h"
#include "CbmKFMaterial.h"
#include "CbmTofHit.h"

#include "TMath.h"
#include "TVector3.h"

#include <iostream>

using std::cout;
using std::endl;


//  ------------------------ Create from CbmTofHit -----------------------
void CbmKFTofHit::Create(CbmTofHit* hit)
{
  // Get pointer to the Kalman filter
  CbmKF* KF = CbmKF::Instance();
  if (NULL == KF) {
    cout << "-E- CbmKFTofHit::Create : "
         << "Kalman filter is not instantiated." << endl;
    return;
  }
  // Get index of material
  MaterialIndex = KF->GetMaterialIndex(hit->GetPlaneId());
  // Store pointer to the material
  wall = (MaterialIndex >= 0) ? (CbmKFWall*) KF->vMaterial[MaterialIndex] : 0;

  // Get position and position error
  TVector3 pos, err;
  hit->Position(pos);
  hit->PositionError(err);
  // Store values
  /*    FitPoint.x = pos.X();
    FitPoint.y = pos.Y();
    FitPoint.z = pos.Z();
    FitPoint.V[0] = 1e-8 * err.X() * err.X();
    FitPoint.V[1] = 0;
    FitPoint.V[2] = 1e-8 * err.Y() * err.Y();*/
  Double_t u      = pos.X();
  Double_t phi    = 0.;
  Double_t sigma2 = err.X() * err.X();
  if (err.Y() < err.X()) {
    u      = pos.Y();
    phi    = TMath::Pi() / 2.;
    sigma2 = err.Y() * err.Y();
  }
  FitPoint.Set(pos.Z(), u, phi, sigma2);
}
//  ----------------------------------------------------------------------


//  ------------------------------ Filter --------------------------------
Int_t CbmKFTofHit::Filter(CbmKFTrackInterface& track, Bool_t /*downstream*/, Double_t& QP0)
{
  Bool_t err = 0;
  /*
    Double_t zfst, zlst, zend;
    if( downstream ) {
        zfst = wall->ZReference - wall->ZThickness/4.;
        zlst = wall->ZReference + wall->ZThickness/4.;
        zend = wall->ZReference + wall->ZThickness/2.;
    }
    else {
        zfst = wall->ZReference + wall->ZThickness/4.;
        zlst = wall->ZReference - wall->ZThickness/4.;
        zend = wall->ZReference - wall->ZThickness/2.;
    }
    Double_t zthick = wall->ZThickness/2.;
*/
  //    wall->Pass( zfst, zthick, track, downstream, QP0 );
  //    KF->Propagate( track, FitPoint.z, QP0 );

  err = err || track.Propagate(FitPoint.z, QP0);
  err = err || FitPoint.Filter(track);
  //  cout<<"  Filter z "<<FitPoint.z<<", QP0 "<<QP0<<endl;
  //    wall->Pass( zlst, zthick, track, downstream, QP0 );
  //    KF->Propagate( track, zend, QP0 );
  return err;
}
//  ----------------------------------------------------------------------


ClassImp(CbmKFTofHit);
