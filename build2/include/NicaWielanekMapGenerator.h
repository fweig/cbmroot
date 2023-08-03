/*
 * NicaWielanekMapGenerator.h
 *
 *  Created on: 28 sty 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAWIELANEKMAPGENERATOR_H_
#define NICAWIELANEKMAPGENERATOR_H_
#include <TString.h>

#include "NicaFemtoWeightGenerator.h"

class TH2D;

/**
 * alternative map generator this code generate maps that are not compatible
 * with NicaCorrFitKisiel! this code works only with PRF frame
 */
class NicaWielanekMapGenerator : public TObject {
  Double_t fRMin, fRMax;
  Double_t fKMin, fKMax;
  Int_t fRBins, fKBins, fPairsNo;
  Double_t *fWeightArray;  //[fPairsNo]
  Double_t *fAngleArray;   //[fPairsNo]
  NicaFemtoWeightGenerator *fWeight;
  TH2D *fMap;
  Double_t fM1, fM2;
  Double_t GeneratePairs(NicaFemtoPair *pair, Double_t R, Double_t K);

 public:
  /**
   * default constructor
   */
  NicaWielanekMapGenerator();
  /**
   * add weight to calculation algorithm
   * @param calc weight that will be used to calculate maps
   */
  void SetWeight(const NicaFemtoWeightGenerator &gen) {
    fWeight = gen.MakeCopy();
  }
  /**
   * set R* axis in map
   * @param bins number of bins
   * @param min minimal value of R (typicaly -max)
   * @param max maximal value of R
   */
  void SetRAxis(Int_t bins, Double_t min, Double_t max);
  /**
   * set K* axis map
   * @param bins bins in map
   * @param min minimal value of K* (typically should be zero)
   * @param max maximal value of K*
   */
  void SetKAxis(Int_t bins, Double_t min, Double_t max);
  /**
   * initialize mapper
   */
  void Init();
  /**
   * create map
   * @param pairs_per_bin number of pairs in single bin
   */
  void Exec(Int_t pairs_per_bin);
  /**
   *
   * @param filename name of file with map
   */
  void SaveMap(TString filename);
  virtual ~NicaWielanekMapGenerator();
  ClassDef(NicaWielanekMapGenerator, 1)
};

#endif /* NICAWIELANEKMAPGENERATOR_H_ */
