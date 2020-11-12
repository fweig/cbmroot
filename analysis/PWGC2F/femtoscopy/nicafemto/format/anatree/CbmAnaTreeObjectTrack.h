/*
 * CbmAnaTreeObjectTrack.h
 *
 *  Created on: 14 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMROOT_ANALYSIS_PWGC2F_FEMTOSCOPY_NICAFEMTO_FORMAT_ANATREE_CBMANATREEOBJECTTRACK_H_
#define CBMROOT_ANALYSIS_PWGC2F_FEMTOSCOPY_NICAFEMTO_FORMAT_ANATREE_CBMANATREEOBJECTTRACK_H_

#include "AnalysisTree/Detector.hpp"
#include "AnalysisTree/Hit.hpp"
#include "AnalysisTree/Particle.hpp"
#include <TObject.h>

class CbmAnaTreeObjectTrack : public TObject {
  AnalysisTree::Particle* fTreeParticle;
  AnalysisTree::Track* fTreeTrack;
  AnalysisTree::Hit* fTreeHit;

public:
  CbmAnaTreeObjectTrack();
  AnalysisTree::Particle* GetTreeParticle() const { return fTreeParticle; };
  AnalysisTree::Track* GetTreeTrack() const { return fTreeTrack; };
  AnalysisTree::Hit* GetTreeHit() const { return fTreeHit; };
  void SetTreeParticle(AnalysisTree::Particle* p) { fTreeParticle = p; };
  void SetTreeTrack(AnalysisTree::Track* t) { fTreeTrack = t; };
  void SetTreeTof(AnalysisTree::Hit* h) { fTreeHit = t; };
  virtual ~CbmAnaTreeObjectTrack();
  CbmAnaTreeObjectTrack(const CbmAnaTreeObjectTrack& other);
  CbmAnaTreeObjectTrack& operator=(const CbmAnaTreeObjectTrack& other);
};

#endif /* CBMROOT_ANALYSIS_PWGC2F_FEMTOSCOPY_NICAFEMTO_FORMAT_ANATREE_CBMANATREEOBJECTTRACK_H_ */
