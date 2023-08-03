/*
 * NicaFemto1DCFAnaMapMC.h
 *
 *  Created on: 2 kwi 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO1DCFANAMAPMC_H_
#define NICAFEMTO1DCFANAMAPMC_H_

#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>

#include "NicaArray.h"
#include "NicaFemtoConst.h"
#include "NicaFemtoFreezoutGenerator.h"
#include "NicaFemtoWeightGenerator.h"

class NicaDividedHisto2D;
class NicaFemto1DCF;

/**
 * generate 2-dim CF map for 1D CF,
 */


class NicaFemto1DCFAnaMapMC : public TObject {
  Double_t fKStarMin, fKStarMax;
  Double_t fRMinEff, fRStep;
  Double_t fRmin;
  Double_t fRmax;
  Double_t fM1, fM2;
  Int_t fKStarBins;
  ENicaFemtoKinematics fKinematics;

protected:
  Int_t fRbins;
  NicaFemtoPair* fPair;
  NicaFemtoFreezoutGenerator* fGenerator;
  NicaFemtoWeightGenerator* fWeight;
  NicaDividedHisto2D* fMap;
  NicaArray_1<Double_t> fRadiiBins;

public:
  NicaFemto1DCFAnaMapMC();
  /**
   * set correlation function used for maps
   * @param cf
   */
  virtual void SetCorrFctn(NicaFemto1DCF* cf);
  /**
   * set radii range in generated map
   * @param r_bins number of r bins
   * @param r_min minimal radius
   * @param r_max maximal radius
   */
  void SetRadiusRange(Int_t r_bins, Double_t r_min, Double_t r_max);
  void SetFreezoutGenerator(const NicaFemtoFreezoutGenerator& freez) { fGenerator = freez.MakeCopy(); }
  void SetWeight(const NicaFemtoWeightGenerator& gen) { fWeight = gen.MakeCopy(); }
  virtual Bool_t Init();
  /**
   * create map
   * @param pairs_per_bin number of pairs in single bin
   */
  virtual void Exec(Int_t pairs_per_bin);
  /**
   *
   * @param filename name of file with map
   */
  void SaveMap(TString filename);
  NicaDividedHisto2D* GetMap() const { return fMap; };
  virtual ~NicaFemto1DCFAnaMapMC();
  ClassDef(NicaFemto1DCFAnaMapMC, 1)
};

#endif /* NICAFEMTO1DCFANAMAPMC_H_ */
