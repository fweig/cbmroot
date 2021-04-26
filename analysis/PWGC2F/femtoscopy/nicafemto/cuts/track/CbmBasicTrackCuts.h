/*
 * CbmBasicTrackCut.h
 *
 *  Created on: 22 wrz 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMBASICTRACKCUT_H_
#define CBMBASICTRACKCUT_H_

#include <Rtypes.h>
#include <RtypesCore.h>

#include "NicaCutsAndMonitors.h"

class NicaTrackPCut;

class CbmTofCut;
class NicaTrackChargeCut;
class CbmNHitsCut;
class NicaTrackChi2Cut;
class NicaTrackPtCut;
class NicaTrackEtaCut;
class NicaTrackDCACut;
/**collection of basic cuts and cut monitors for track in AnaTree format **/

class CbmBasicTrackCuts : public NicaCutsAndMonitors {

  NicaTrackChargeCut* GetChargeCut() const;
  CbmNHitsCut* GetNHitsCut() const;
  NicaTrackChi2Cut* GetChi2Cut() const;
  NicaTrackPCut* GetPCut() const;
  NicaTrackPtCut* GetPtCut() const;
  NicaTrackEtaCut* GetEtaCut() const;
  NicaTrackDCACut* GetDCACut() const;
  TVector3 fMonitorsSettingsKin[2];
  TVector3 fMonitorSettingsTof[2];
  TVector3 fMonitorSettingsNHits;
  TVector3 fMonitorSettingsChi2;
  TVector3 fMonitorSettingsDCA[2];

public:
  CbmBasicTrackCuts();
  /**
     * enable monitors must be called **after** setting axes of histograms
     */
  void CreateBasicMonitors();
  void SetCharge(Int_t charge);
  void SetNHits(Int_t min, Int_t max);
  void SetPt(Double_t min, Double_t max);
  void SetEta(Double_t min, Double_t max);
  void AcceptOnlyWithTofHit(Bool_t val);
  void SetM2(Double_t min, Double_t max);
  void SetDCAXY(Double_t min, Double_t max);
  void SetDCAZ(Double_t min, Double_t max);
  void SetTofMonitorPAxis(Int_t nbins, Double_t min, Double_t max);
  void SetTofMonitorM2Axis(Int_t nbins, Double_t min, Double_t max);
  void SetPtEtaMonitorPtAxis(Int_t nbins, Double_t min, Double_t max);
  void SetPtEtaMonitorEtaAxis(Int_t nbins, Double_t min, Double_t max);
  void SetNHitsMonitorAxis(Int_t nbins, Double_t min, Double_t max);
  void SetChi2MonitorAxis(Int_t nbins, Double_t min, Double_t max);
  void SetDCAMonitorZAxis(Int_t nbins, Double_t min, Double_t max);
  void SetDCAMonitorXYAxis(Int_t nbins, Double_t min, Double_t max);
  CbmTofCut* GetTofCut() const;
  virtual ~CbmBasicTrackCuts();
  ClassDef(CbmBasicTrackCuts, 1)
};

#endif /* CBMBASICTRACKCUT_H_ */
