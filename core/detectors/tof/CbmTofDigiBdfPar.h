/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau, Florian Uhlig [committer] */

/** @file CbmTofDigiBdfPar.h
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 01.08.2013
 **/

/** @class CbmTofDigiBdfPar
 ** @brief Parameters class for the CBM ToF digitizer using beam data distributions
 ** @brief as hit/cluster models
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @version 1.0
 **/

#ifndef CBMTOFDIGIBDFPAR_H
#define CBMTOFDIGIBDFPAR_H 1

#include <CbmTofAddress.h>  // for accessors
#include <CbmTofHit.h>      // for accessors

#include <FairParGenericSet.h>  // for FairParGenericSet

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t
#include <TArrayD.h>     // for TArrayD
#include <TArrayI.h>     // for TArrayI
#include <TString.h>     // for TString

#include <map>     // for map
#include <vector>  // for vector

class FairParamList;
class TH1;

class CbmTofDigiBdfPar : public FairParGenericSet {
public:
  CbmTofDigiBdfPar(const char* name    = "CbmTofDigiBdfPar",
                   const char* title   = "BDF Digitization parameters for the TOF detector",
                   const char* context = "TestDefaultContext");
  ~CbmTofDigiBdfPar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  void printParams();
  Bool_t LoadBeamtimeHistos();

  void SetInputFile(TString FileName) { fsBeamInputFile = FileName; }
  void SetCalibFile(TString FileName) { fsBeamCalibFile = FileName; }

  // Fee properties
  Double_t GetFeeGainSigma() const { return fdFeeGainSigma; };
  Double_t GetFeeThreshold() const { return fdFeeTotThr; };
  Double_t GetFeeTimeRes() const { return fdTimeResElec; };
  Double_t GetStartTimeRes() const { return fdTimeResStart; };
  Double_t GetDeadtime() const { return fdDeadtime; };


  // Geometry variables
  Double_t GetSignalSpeed() const { return fdSignalPropSpeed; };
  Int_t GetNbSmTypes() const { return fiNbSmTypes; };
  Int_t GetNbSm(Int_t iSmType) const;
  Int_t GetNbRpc(Int_t iSmType) const;
  Int_t GetNbGaps(Int_t iSmType, Int_t iRpc) const;
  Double_t GetGapSize(Int_t iSmType, Int_t iRpc) const;
  Double_t GetSigVel(Int_t iSmType, Int_t iSm, Int_t iRpc) const;
  void SetSigVel(Int_t iSmType, Int_t iSm, Int_t iRpc, Double_t dvel);
  Int_t GetNbChan(Int_t iSmType, Int_t iRpc) const;
  Int_t GetChanType(Int_t iSmType, Int_t iRpc) const;
  Int_t GetChanOrient(Int_t iSmType, Int_t iRpc) const;
  Int_t GetNbDet() const;
  Int_t GetDetUId(Int_t iDet);
  Int_t GetDetInd(Int_t iAddr);
  Int_t GetTrackingStation(Int_t iSmType, Int_t iSm, Int_t iRpc) const;
  Int_t GetTrackingStation(CbmTofHit* pHit) const
  {
    return GetTrackingStation(CbmTofAddress::GetSmType(pHit->GetAddress()), CbmTofAddress::GetSmId(pHit->GetAddress()),
                              CbmTofAddress::GetRpcId(pHit->GetAddress()));
  };
  Int_t GetNbTrackingStations() const { return fiNbTrackingStations; };

  // Beamtime variables
  TString GetInputFileName() const { return fsBeamInputFile; };
  TString GetCalibFileName() const { return fsBeamCalibFile; };
  Int_t GetClusterRadiusModel() const { return fiClusterRadiusModel; };
  Int_t GetTypeInputMap(Int_t iSmType) const;
  Double_t GetEfficiency(Int_t iSmType) const;
  Double_t GetGapEfficiency(Int_t iSmType, Int_t iRpc) const;
  Double_t GetResolution(Int_t iSmType) const;
  Double_t GetSystemResolution(Int_t iSmType) const;
  TH1* GetClustSizeHist(Int_t iSmType) const;
  TH1* GetClustTotHist(Int_t iSmType) const;
  Double_t GetLandauMpv(Int_t iSmType) const;
  Double_t GetLandauSigma(Int_t iSmType) const;

  // Digi type to use
  Bool_t UseExpandedDigi() const { return fbUseExpDigi; };
  // Switch to use only primary tracks
  Bool_t UseOnlyPrimaries() const { return fbUseOnlyPrim; };
  // Switch to use only primary tracks
  Bool_t UseOneGapPerTrk() const { return fbOneGapTrack; };

  // Cluster Model to use
  Int_t GetClusterModel() const { return fiClusterModel; };

  // Simple clusterizer parameters
  Bool_t ClustUseTrackId() const { return fbMulUseTrackId; };
  Double_t GetMaxTimeDist() const;
  Double_t GetMaxDistAlongCh() const { return fdMaxSpaceDistClust; };

  // Cleaning junk
  void ClearHistos();

private:
  // Fit the cluster size distribution and use it to extract
  // the values of the parameter to use for the Landau Cluster
  // size distribution. If failing use default values from parameter
  // file.
  Bool_t GetLandauParFromBeamDataFit();

  // Digi type to use
  Bool_t fbUseExpDigi;
  // Switch to use only primary tracks
  Bool_t fbUseOnlyPrim;
  // Switch method to try avoiding counting all gaps/TofPoint from same track
  // with different time/charge
  Bool_t fbOneGapTrack;

  // Cluster Model to use
  Int_t fiClusterModel;

  // Fee properties
  Double_t fdFeeGainSigma;  // -> 0 makes gain 1 for all channels
  Double_t fdFeeTotThr;     //
  Double_t fdTimeResElec;   //
  Double_t fdTimeResStart;  //
  Double_t fdDeadtime;      //

  // Geometry variables, text to be generated in the CreateGeometry macros
  Double_t fdSignalPropSpeed;         //  -> in parameter?
  Int_t fiNbSmTypes;                  //
  Int_t fiNbTrackingStations;         //
  TArrayI fiNbSm;                     // [fiNbSmTypes]
  TArrayI fiNbRpc;                    // [fiNbSmTypes]
  std::vector<TArrayI> fiNbGaps;      // [fiNbSmTypes][fiNbRpc]
  std::vector<TArrayD> fdGapSize;     // [fiNbSmTypes][fiNbRpc]
  std::vector<TArrayD> fdSigVel;      // [fiNbSmTypes][fiNbSm*fiNbRpc]  // Signal velocity
  std::vector<TArrayI> fiTrkStation;  // [fiNbSmTypes][fiNbSm*fiNbRpc]  // Signal velocity
  std::vector<TArrayI> fiNbCh;        // [fiNbSmTypes][fiNbRpc]
  std::vector<TArrayI> fiChType;      // [fiNbSmTypes][fiNbRpc]
  std::vector<TArrayI>
    fiChOrientation;  // [fiNbSmTypes][fiNbRpc] -> in parameter? Possibility to Readout from geometry angles?
  TArrayI fiDetUId;   // [NbDet]
  std::map<Int_t, Int_t> fMapDetInd;  // [NbDet]

  // Beamtime variables
  TString fsBeamInputFile;
  TString fsBeamCalibFile;
  Int_t
    fiClusterRadiusModel;  // 0 = fixed value, 1 = Landau distrib. with fixed param, 2 = Landau distrib. with bdf params
  TArrayI fiSmTypeInpMapp;
  TArrayD fdEfficiency;                   // [fiNbSmTypes] -> in parameter?
  std::vector<TArrayD> fdGapsEfficiency;  // [fiNbSmTypes][fiNbRpc]
  TArrayD fdTimeResolution;               // [fiNbSmTypes] -> in parameter?
  std::vector<TH1*> fh1ClusterSize;       // [fiNbSmTypes] -> in parameter?
  std::vector<TH1*> fh1ClusterTot;        // [fiNbSmTypes] -> in parameter?
  TArrayD fdLandauMpv;                    // [fiNbSmTypes] -> in parameter?
  TArrayD fdLandauSigma;                  // [fiNbSmTypes] -> in parameter?

  // Simple clusterizer parameters
  Bool_t fbMulUseTrackId;
  Double_t fdMaxTimeDistClust;
  Double_t fdMaxSpaceDistClust;

  CbmTofDigiBdfPar(const CbmTofDigiBdfPar&);
  CbmTofDigiBdfPar& operator=(const CbmTofDigiBdfPar&);

  ClassDef(CbmTofDigiBdfPar, 1)
};

#endif  // CBMTOFDIGIBDFPAR_H
