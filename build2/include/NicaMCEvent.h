/*
 * NicaMCEvent.h
 *
 *  Created on: 1 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAMCEVENT_H_
#define NICAMCEVENT_H_

#include "NicaEvent.h"
/**
 * class for MC events
 */
class NicaMCEvent : public NicaEvent {
protected:
  /**
   * impact parameter
   */
  Double_t fB;
  virtual void ShallowCopyEvent(NicaEvent* event);
  virtual void ShallowCopyTracks(NicaEvent* event);
  NicaMCEvent(TString track_classname, TString v0_class = "NicaV0Track");

public:
  NicaMCEvent();
  NicaMCEvent(const NicaMCEvent& other);
  virtual void Update();
  inline Double_t GetImpactParameter() const { return fB; };
  virtual ENicaFormatType GetFormatType() const { return ENicaFormatType::kSim; };
  inline NicaMCEvent* GetMC() const { return const_cast<NicaMCEvent*>(this); };
  virtual Float_t GetFieldVal(Int_t fieldID) const;
  virtual TString GetFieldName(Int_t fieldID) const;
  virtual ~NicaMCEvent();
  ClassDef(NicaMCEvent, 1)
};

#endif /* NICAMCEVENT_H_ */
