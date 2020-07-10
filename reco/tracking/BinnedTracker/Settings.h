/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Settings.h
 * Author: tablyaz
 *
 * Created on October 4, 2017, 12:57 PM
 */

#ifndef CBM_BINNED_SETTINGS_H
#define CBM_BINNED_SETTINGS_H

#include "CbmDefs.h"
#include "CbmSetup.h"
#include "FairParGenericSet.h"
#include "FairParamList.h"
#include "TArrayD.h"
#include <list>
#include <tuple>

enum EPrimaryParticleId { ppiNone, ppiJpsi };

class CbmBinnedSettings : public FairParGenericSet {
public:
  static CbmBinnedSettings* Instance() {
    static CbmBinnedSettings* theInstance = 0;

    if (0 == theInstance) theInstance = new CbmBinnedSettings;

    return theInstance;
  }

public:
  CbmBinnedSettings()
    : FairParGenericSet("CbmBinnedSettings",
                        "Binned tracker reconstruction parameters",
                        "Default")
    , fIsConfiguring(false)
    , fIsOnlyPrimary(true)
    , fNofStsStations(0)
    , fNofMuchStations(0)
    , fNofTrdStations(0) /*, fStationErrors()*/
    , fNofStations(0)
    , fXScats()
    , fYScats()
    , fPrimaryParticleIds() {
    std::fill_n(fUseModules, int(ECbmModuleId::kLastModule), true);
    fPrimaryParticleIds.push_back(ppiNone);
  }

  CbmBinnedSettings(const CbmBinnedSettings&) = delete;
  CbmBinnedSettings& operator=(const CbmBinnedSettings&) = delete;

  void clear() {
    status = kFALSE;
    resetInputVersions();
  }

  // Overridden virtual methods
  void putParams(FairParamList* l) {
    if (0 == l) return;

    l->add("OnlyPrimary", fIsOnlyPrimary);

    TArrayC detFlags(ToIntegralType(ECbmModuleId::kLastModule));

    for (int i = 0; i < ToIntegralType(ECbmModuleId::kLastModule); ++i)
      detFlags[i] = fUseModules[i];

    l->add("UseModules", detFlags);
    l->add("NofStsStations", fNofStsStations);
    l->add("NofMuchStations", fNofMuchStations);
    l->add("NofTrdStations", fNofTrdStations);

    /*Int_t nofStations = fStationErrors.size();
        l->add("NofStations", nofStations);
        TArrayD stationErrorArray(3 * nofStations);
        
        for (int i = 0; i < nofStations; ++i)
        {
            stationErrorArray[3 * i] = std::get<0> (fStationErrors[i]);
            stationErrorArray[3 * i + 1] = std::get<1> (fStationErrors[i]);
            stationErrorArray[3 * i + 2] = std::get<2> (fStationErrors[i]);
        }
        
        l->add("StationErrors", stationErrorArray);*/

    l->add("NofStations", fNofStations);
    TArrayD stationXScatArray(fNofStations);
    TArrayD stationYScatArray(fNofStations);

    for (int i = 0; i < fNofStations; ++i) {
      stationXScatArray[i] = fXScats[i];
      stationYScatArray[i] = fYScats[i];
    }

    l->add("StationXScats", stationXScatArray);
    l->add("StationYScats", stationYScatArray);
  }

  Bool_t getParams(FairParamList* l) {
    if (0 == l) return kFALSE;

    Int_t op = 0;

    if (!l->fill("OnlyPrimary", &op)) return kFALSE;

    fIsOnlyPrimary = op != 0;

    TArrayC detFlags(ToIntegralType(ECbmModuleId::kLastModule));

    if (!l->fill("UseModules", &detFlags)) return kFALSE;

    for (int i = 0; i < ToIntegralType(ECbmModuleId::kLastModule); ++i)
      fUseModules[i] = detFlags[i];

    if (!l->fill("NofStsStations", &fNofStsStations)) return kFALSE;

    if (!l->fill("NofMuchStations", &fNofMuchStations)) return kFALSE;

    if (!l->fill("NofTrdStations", &fNofTrdStations)) return kFALSE;

    if (!l->fill("NofStations", &fNofStations)) return kFALSE;

    /*TArrayD stationErrorArray(3 * nofStations);
        
        if (!l->fill("StationErrors", &stationErrorArray))
            return kFALSE;
        
        fStationErrors.resize(nofStations);
        
        for (int i = 0; i < nofStations; ++i)
        {
            std::get<0> (fStationErrors[i]) = stationErrorArray[3 * i];
            std::get<1> (fStationErrors[i]) = stationErrorArray[3 * i + 1];
            std::get<2> (fStationErrors[i]) = stationErrorArray[3 * i + 2];
        }*/

    TArrayD stationXScats(fNofStations);

    if (!l->fill("StationXScats", &stationXScats)) return kFALSE;

    TArrayD stationYScats(fNofStations);

    if (!l->fill("StationYScats", &stationYScats)) return kFALSE;

    for (int i = 0; i < fNofStations; ++i) {
      fXScats[i] = stationXScats[i];
      fYScats[i] = stationYScats[i];
    }

    return kTRUE;
  }
  // ~Overridded virtual methods

  bool IsConfiguring() const { return fIsConfiguring; }
  void SetConfiguring(bool v) { fIsConfiguring = v; }
  bool IsOnlyPrimary() const { return fIsOnlyPrimary; }
  void SetOnlyPrimary(bool v) { fIsOnlyPrimary = v; }
  bool Use(ECbmModuleId m) const {
    return fUseModules[ToIntegralType(m)] && CbmSetup::Instance()->IsActive(m);
  }
  void SetUse(Int_t m, bool v) { fUseModules[m] = v; }
  void SetUse(bool v) {
    std::fill_n(fUseModules, int(ECbmModuleId::kLastModule), v);
  }
  void SetUse(bool useModules[ToIntegralType(ECbmModuleId::kLastModule)]) {
    std::copy(useModules,
              useModules + ToIntegralType(ECbmModuleId::kLastModule),
              fUseModules);
  }
  Int_t GetNofStsStations() const { return fNofStsStations; }
  void SetNofStsStations(Int_t v) { fNofStsStations = v; }
  Int_t GetNofMuchStations() const { return fNofMuchStations; }
  void SetNofMuchStations(Int_t v) { fNofMuchStations = v; }
  Int_t GetNofTrdStations() const { return fNofTrdStations; }
  void SetNofTrdStations(Int_t v) { fNofTrdStations = v; }
  Int_t GetNofStations() const { return fNofStations; }
  void SetNofStations(Int_t v) { fNofStations = v; }
  /*void AddStationErrors(Double_t xErr, Double_t yErr, Double_t tErr) { fStationErrors.push_back(std::make_tuple(xErr, yErr, tErr)); }
    Int_t GetNofStations() const { return fStationErrors.size(); }
    Double_t GetXError(int stationNumber) const { return std::get<0> (fStationErrors[stationNumber]); }
    Double_t GetYError(int stationNumber) const { return std::get<1> (fStationErrors[stationNumber]); }
    Double_t GetTError(int stationNumber) const { return std::get<2> (fStationErrors[stationNumber]); }*/

  void AddStationScats(Double_t x, Double_t y) {
    fXScats.push_back(x);
    fYScats.push_back(y);
    ++fNofStations;
  }

  Double_t GetXScat(size_t stationNumber) const {
    return stationNumber < fXScats.size() ? fXScats[stationNumber] : 0;
  }
  Double_t GetYScat(size_t stationNumber) const {
    return stationNumber < fYScats.size() ? fYScats[stationNumber] : 0;
  }

  void ResetScats() {
    fXScats.clear();
    fYScats.clear();
  }

  const std::list<EPrimaryParticleId> GetPrimaryParticles() const {
    return fPrimaryParticleIds;
  }

  void SetPrimaryParticle(EPrimaryParticleId v) {
    fPrimaryParticleIds.clear();
    fPrimaryParticleIds.push_back(v);
  }

  void AddPrimaryParticle(EPrimaryParticleId v) {
    fPrimaryParticleIds.push_back(v);
  }

  void SetPrimaryParticles(const std::list<EPrimaryParticleId> v) {
    fPrimaryParticleIds = v;
  }
  void AddPrimaryParticles(const std::list<EPrimaryParticleId> v) {
    fPrimaryParticleIds.insert(fPrimaryParticleIds.end(), v.begin(), v.end());
  }

private:
  bool fIsConfiguring;
  bool fIsOnlyPrimary;
  bool fUseModules[ToIntegralType(ECbmModuleId::kLastModule)];
  Int_t fNofStsStations;
  Int_t fNofMuchStations;
  Int_t fNofTrdStations;
  //std::vector<std::tuple<Double_t, Double_t, Double_t> > fStationErrors;
  Int_t fNofStations;
  std::vector<Double_t> fXScats;
  std::vector<Double_t> fYScats;
  std::list<EPrimaryParticleId> fPrimaryParticleIds;

  ClassDef(CbmBinnedSettings, 8)
};

#endif /* CBM_BINNED_SETTINGS_H */
