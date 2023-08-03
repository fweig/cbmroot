/*
 * NicaFemtoFastCut.h
 *
 *  Created on: 25-07-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTOFASTCUT_H_
#define NICAFEMTOFASTCUT_H_

#include <TObject.h>


class NicaFemtoPair;
class NicaPackage;

/**
 * abstract class for returnin fast cuts
 */
class NicaFemtoFastCut : public TObject {
 protected:
  /**
   * number of bins
   */
  Int_t fBins;
  /**
   * lower edges of  bins
   */
  Double_t *fMin;  //[fBins]
  /**
   * upper edges of bins
   */
  Double_t *fMax;  //[fBins]
  /**
   * lower edges of bins used for comparisons
   */
  Double_t *fMinTrue;  //[fBins]
  /**
   * upper edges of bins used for comparisons
   */
  Double_t *fMaxTrue;  //[fBins]
  /**
   * pointer to processed pair
   */
  NicaFemtoPair *fPair;

 public:
  /**
   * default constructor
   */
  NicaFemtoFastCut();
  /**
   * default copy constructor
   * @param copy
   */
  NicaFemtoFastCut(const NicaFemtoFastCut &copy);
  /**
   * make copy of this object
   * @return
   */
  virtual NicaFemtoFastCut *MakeCopy() const = 0;
  /**
   * checking if bins don't overlap
   */
  void Allocate();
  /**
   *
   * @param i bin
   * @return lower edge value
   */
  Double_t GetMin(Int_t i) const { return fMin[i]; };
  /**
   *
   * @param i bin number
   * @return upper egde value
   */
  Double_t GetMax(Int_t i) const { return fMax[i]; };
  /**
   * initaliza this class
   * @param pair pointer to processed femto pair (this pointer cannot be changed
   * during run !
   */
  virtual void Init(NicaFemtoPair *pair);
  /**
   *
   * @return number of bins
   */
  virtual Int_t GetNBins() const { return fBins; };
  /**
   * return bin number for pair
   * @return bin number for pair
   */
  virtual Int_t GetBin() const = 0;
  /**
   *
   * @return bin number for rotated pair
   */
  virtual Int_t GetBinRotated() const = 0;
  /**
   *
   * @return bin number for hemisphere pair
   */
  virtual Int_t GetBinHemisphere() const = 0;
  /**
   *
   * @return bin number for mixed pair
   */
  virtual Int_t GetBinMixed() const { return GetBin(); };
  /**
   *
   * @return bin number for perfect pair
   */
  virtual Int_t GetBinPerfect() const { return GetBin(); };
  /**
   *
   * @return bin number for charged pair
   */
  virtual Int_t GetBinCharged() const { return GetBin(); };
  /**
   * add new bin
   * @param min lower endge of new bin
   * @param max upper edge of new bin
   * @param opt not used
   */
  virtual void AddCutBin(Double_t min, Double_t max, Option_t *opt = " ");
  /**
   * create package
   * @return package with report
   */
  virtual NicaPackage *Report() const;
  virtual ~NicaFemtoFastCut();
  ClassDef(NicaFemtoFastCut, 1)
};
/**
 * class for pass al pairs of track
 */
class NicaFemtoFastCutVirtual : public NicaFemtoFastCut {
 public:
  NicaFemtoFastCutVirtual() { AddCutBin(-1, 1); };
  /**
   * copy ctor
   * @param fast
   */
  NicaFemtoFastCutVirtual(const NicaFemtoFastCutVirtual &fast);
  Int_t GetBin() const { return 0; };
  Int_t GetBinRotated() const { return 0; };
  Int_t GetBinHemisphere() const { return 0; };
  virtual NicaFemtoFastCut *MakeCopy() const {
    return new NicaFemtoFastCutVirtual(*this);
  }
  virtual ~NicaFemtoFastCutVirtual(){};
  ClassDef(NicaFemtoFastCutVirtual, 1)
};
/**
 * basic class for calculating kt bins
 */
class NicaFemtoFastCutKt : public NicaFemtoFastCut {
 public:
  NicaFemtoFastCutKt();
  /**
   * copy ctor
   * @param fast
   */
  NicaFemtoFastCutKt(const NicaFemtoFastCutKt &fast);
  Int_t GetBin() const;
  Int_t GetBinRotated() const;
  Int_t GetBinHemisphere() const;
  virtual NicaFemtoFastCut *MakeCopy() const {
    return new NicaFemtoFastCutKt(*this);
  }
  virtual ~NicaFemtoFastCutKt();
  ClassDef(NicaFemtoFastCutKt, 1)
};
/**
 * basic class for calculating put sum bins
 */
class NicaFemtoFastCutPtSum : public NicaFemtoFastCut {
 public:
  NicaFemtoFastCutPtSum();
  /**
   * copy ctor
   * @param fast
   */
  NicaFemtoFastCutPtSum(const NicaFemtoFastCutPtSum &fast);
  Int_t GetBin() const;
  Int_t GetBinRotated() const;
  Int_t GetBinHemisphere() const;
  virtual NicaFemtoFastCut *MakeCopy() const {
    return new NicaFemtoFastCutPtSum(*this);
  }
  virtual ~NicaFemtoFastCutPtSum();
  ClassDef(NicaFemtoFastCutPtSum, 1)
};
/**
 * basic class for calculating phi bins
 */
class NicaFemtoFastCutPhi : public NicaFemtoFastCut {
  Double_t fEventPhi;
  Double_t fGlobalMin;
  Double_t fPhiCorrection;
  Double_t RoundPhi(Double_t val) const;
  void TryRoundRange(Int_t i);

 public:
  NicaFemtoFastCutPhi();
  /**
   * copy ctor
   * @param fast
   */
  NicaFemtoFastCutPhi(const NicaFemtoFastCutPhi &fast);
  Int_t GetBin() const;
  Int_t GetBinRotated() const;
  Int_t GetBinHemisphere() const;
  void Init(NicaFemtoPair *pair);
  /**
   * set reaction plane phi
   * @param phi event reaction plane
   */
  void SetEventPhi(const Double_t phi);
  virtual NicaFemtoFastCut *MakeCopy() const {
    return new NicaFemtoFastCutPhi(*this);
  }
  virtual ~NicaFemtoFastCutPhi();
  virtual NicaPackage *Report() const;
  ClassDef(NicaFemtoFastCutPhi, 1)
};
/**
 * basic class for calculating eta bins
 */

class NicaFemtoFastCutEta : public NicaFemtoFastCut {
 public:
  NicaFemtoFastCutEta();
  /**
   * copy ctor
   * @param fast
   */
  NicaFemtoFastCutEta(const NicaFemtoFastCutEta &fast);
  Int_t GetBin() const;
  Int_t GetBinRotated() const;
  Int_t GetBinHemisphere() const;
  virtual NicaFemtoFastCut *MakeCopy() const {
    return new NicaFemtoFastCutEta(*this);
  }
  virtual ~NicaFemtoFastCutEta();
  ClassDef(NicaFemtoFastCutEta, 1)
};

#endif /* NICAFEMTOFASTCUTS_H_ */
