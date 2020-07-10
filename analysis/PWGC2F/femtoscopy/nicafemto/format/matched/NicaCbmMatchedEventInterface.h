/*
 * NicaCbmMatchedEventInterface.h
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMMATCHEDEVENTINTERFACE_H_
#define NICACBMMATCHEDEVENTINTERFACE_H_

#include "NicaCbmEventInterface.h"
#include "NicaCbmMatchedTrackInterface.h"
class NicaCbmEvent;
class NicaCbmMatchedEvent;
class NicaCbmMatchedEventInterface : public NicaCbmEventInterface {
  friend class NicaCbmMatchedEvent;

protected:
  NicaTrackClones* fStsMatches;
  NicaTrackClones* fTofMatches;
  NicaTrackClones* fTrdMatches;
  NicaTrackClones* fMuchMatches;
  NicaTrackClones* fRichMatches;
  virtual void ConnectToTree();
  void Register(Bool_t write);

public:
  NicaCbmMatchedEventInterface(NicaCbmSetup mode = kSis100Hadron);
  TClonesArray* GetStsMatches() const { return fStsMatches->GetArray(); };
  TClonesArray* GetTofMatches() const { return fTofMatches->GetArray(); };
  TClonesArray* GetTrdMatches() const { return fTrdMatches->GetArray(); };
  TClonesArray* GetRichMatches() const { return fRichMatches->GetArray(); };
  TClonesArray* GetMuchMatches() const { return fMuchMatches->GetArray(); };
  virtual void CopyData(NicaEventInterface* s);
  virtual void Compress(Int_t* map, Int_t map_size);
  virtual void
  CopyAndCompress(NicaEventInterface* s, Int_t* map, Int_t map_size);
  virtual void FillTrackInterface(NicaTrackInterface* track, Int_t index);
  virtual NicaTrackInterface* GetTrackInterface() const {
    return new NicaCbmMatchedTrackInterface();
  }
  virtual ~NicaCbmMatchedEventInterface();
  ClassDef(NicaCbmMatchedEventInterface, 1)
};

#endif /* NICA_CBM_FORMAT_MATCHED_NICACBMMATCHEDEVENTINTERFACE_H_ */
