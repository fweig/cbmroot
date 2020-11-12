/*
 * NicaCbmTrack.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMATRECOTRACK_H_
#define NICACBMATRECOTRACK_H_

#include "AnalysisTree/Detector.hpp"
#include "AnalysisTree/Particle.hpp"
#include "CbmGlobalTrack.h"
#include "CbmHelix.h"
#include "NicaCbmATEventInterface.h"
#include "NicaExpTrack.h"
#include "NicaToFTrack.h"

class CbmHelix;
class NicaCbmATTrackInterface;
class NicaCbmATTrack : public NicaExpTrack {
  NicaToFTrack* fTofTrack;
  CbmHelix* fHelix;
  Float_t fChi2Vertex;

public:
  NicaCbmATTrack();
  NicaCbmATTrack(const NicaCbmATTrack& other);
  NicaCbmATTrack& operator=(const NicaCbmATTrack& other);
  NicaToFTrack* GetTofTrack() { return fTofTrack; };
  CbmHelix* GetHelix() const { return fHelix; };
  Double_t GetVertexChi2() const { return fChi2Vertex; };
  void SetVertexChi2(Double_t v) { fChi2Vertex = v; };
  virtual void CopyData(NicaTrack* other);
  virtual TObject* GetDetTrack(const UInt_t detID) const;
  virtual ~NicaCbmATTrack();
  ClassDef(NicaCbmATTrack, 1)
};

#endif /* NICACBMATRECOTRACK_H_ */
