/*
 * NicaCorrFitTempMaps.h
 *
 *  Created on: 14 lut 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_NICACORRFITHDFUNC_H_
#define NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_NICACORRFITHDFUNC_H_

#include "NicaArray.h"
#include "NicaSplines.h"
#include <NicaDividedHisto.h>
#include <TObject.h>

/**
 * store denominator of CF in "hd mode" (with extrapolated bins inside)
 */

class NicaCorrFitHDFunc : public TObject {
protected:
  /**
   * dummy val - used to avoid recalculation of bins
   */
  const Int_t fDummyVal;
  const Int_t fDim;
  NicaArray_1<Int_t> fHDToBin;
  Double_t* fParams;  //[fDim]
  Double_t* fMins;    //[fDim]
  Double_t* fSteps;   //[fDim]
  /**
   * recalculate array with conversion from bin to hd bin
   * @param newSize
   */
  void RecalcHDBin(Int_t newSize);
  /**
   * calculates the total number of non-zero bins
   * @param h
   * @return
   */
  Int_t CalcBinNonZero(TH1* h) const;
  /**
   *
   * @param bin
   * @return HD bin number for given bin
   */
  inline Int_t GetBinHD(Int_t bin) const { return bin * 2 - 1; };

public:
  NicaCorrFitHDFunc(Int_t dim = 1);
  /**
   *
   * @param mask
   * @return number of bins
   */
  virtual void SetMask(TH1* mask, TH1* denominator, Bool_t hd) = 0;
  /**
   * conver "standard bin number" to "HD bin number"
   * @param hd_bin
   * @return
   */
  inline Int_t HDBinToBin(Int_t hd_bin) const { return fHDToBin.Get(hd_bin); };
  /**
   * return half of the bin width of original function
   * @param no axis
   * @return
   */
  inline Double_t GetStep(Int_t no) const { return fSteps[no]; };
  /**
   * return minimal value of the histogram with CF
   * @param no
   * @return
   */
  inline Double_t GetMin(Int_t no) const { return fMins[no]; };
  virtual ~NicaCorrFitHDFunc();
  ClassDef(NicaCorrFitHDFunc, 1)
};

/**
 * representation of 1D CF
 * this holds
 * a) map in HD mode
 * b) map f sum
 * NOTE - there are "active bins" as arguments of CF's
 */
class NicaCorrFitHDFunc1D : public NicaCorrFitHDFunc {
  NicaArray_1<Double_t> fDenominatorHD;
  NicaArray_1<Double_t> fDenominatorSum;
  NicaArray_1<Double_t> fMapHD;

  NicaArray_1<Int_t> fBins;
  NicaArray_1<Int_t> fBinsHD;
  void CalculateBinsArrays(TH1* mask, Bool_t hd);

public:
  NicaCorrFitHDFunc1D();
  /**
   *
   * @return total number of "active bins"
   */
  inline Int_t GetNbins() const { return fBins.GetSize(); };
  /**
   * convert "active bin number to bin nubmer
   * @param no
   * @return
   */
  inline Int_t GetBin(Int_t no) const { return fBins.Get(no); };
  /**
   *
   * @param mask fitting mask
   * @param denominator denomiator of CF
   * @param hd if HD then calculate also values of denominator on the edges of bins
   */
  void SetMask(TH1* mask, TH1* denominator, Bool_t hd);
  /**
   * return value of the CF at given bin, note: only active bins have this value calculated
   * @param bin
   * @param extrapolated
   * @return
   */
  Double_t GetBinCFVal(Int_t bin, Bool_t extrapolated) const;
  /**
   * calculate X-value for given "hd bin"
   * @param hdBin
   * @return
   */
  inline Double_t EvalHD(Double_t hdBin) const { return fMins[0] + fSteps[0] * hdBin; }
  inline NicaArray_1<Int_t>& GetBinsHD() { return fBinsHD; };
  inline NicaArray_1<Double_t>& GetCFMapHD() { return fMapHD; }
  inline NicaArray_1<Double_t>& GetDenominatorHD() { return fDenominatorHD; };
  inline NicaArray_1<Double_t>& GetDenominatorSum() { return fDenominatorSum; };
  virtual ~NicaCorrFitHDFunc1D();
  ClassDef(NicaCorrFitHDFunc1D, 1)
};

class NicaCorrFitHDFunc3D : public NicaCorrFitHDFunc {
  NicaArray_3<Double_t> fDenominatorHD;
  NicaArray_3<Double_t> fDenominatorSum;
  NicaArray_3<Double_t> fMapHD;
  NicaArray_1<Int_t> fBinsX;
  NicaArray_1<Int_t> fBinsY;
  NicaArray_1<Int_t> fBinsZ;
  NicaArray_1<Int_t> fBinsHDX;
  NicaArray_1<Int_t> fBinsHDY;
  NicaArray_1<Int_t> fBinsHDZ;
  void CalculateBinsArrays(TH1* mask, Bool_t hd);

public:
  NicaCorrFitHDFunc3D();
  void SetMask(TH1* mask, TH1* denominator, Bool_t hd);
  /**
   *
   * @param binX
   * @param binY
   * @param binZ
   * @param extrapolated
   * @return value of CF for given bins
   */
  Double_t GetBinCFVal(Int_t binX, Int_t binY, Int_t binZ, Bool_t extrapolated) const;
  /**
   * converts active bin number into bin number
   * @param i
   * @return
   */
  inline Int_t GetXBin(Int_t i) const { return fBinsX.Get(i); };
  /**
   * converts active bin number into bin number
   * @param i
   * @return
   */
  inline Int_t GetYBin(Int_t i) const { return fBinsY.Get(i); };
  /**
   * converts active bin number into bin number
   * @param i
   * @return
   */
  inline Int_t GetZBin(Int_t i) const { return fBinsZ.Get(i); };
  inline NicaArray_1<Int_t>& GetBinsHDX() { return fBinsHDX; };
  inline NicaArray_1<Int_t>& GetBinsHDY() { return fBinsHDY; };
  inline NicaArray_1<Int_t>& GetBinsHDZ() { return fBinsHDZ; };
  inline NicaArray_3<Double_t>& CFMapHD() { return fMapHD; }
  inline NicaArray_3<Double_t>& GetDenominatorHD() { return fDenominatorHD; };
  inline NicaArray_3<Double_t>& GetDenominatorSum() { return fDenominatorSum; };
  inline Int_t GetNBins() const { return fBinsX.GetSize(); };
  inline Double_t EvalHDX(Double_t hdBin) const { return fMins[0] + fSteps[0] * hdBin; }
  inline Double_t EvalHDY(Double_t hdBin) const { return fMins[1] + fSteps[1] * hdBin; }
  inline Double_t EvalHDZ(Double_t hdBin) const { return fMins[2] + fSteps[2] * hdBin; }
  virtual ~NicaCorrFitHDFunc3D() {};
  ClassDef(NicaCorrFitHDFunc3D, 1)
};

#endif /* NICAFEMTO_ANALYSIS_FEMTO_CORRFIT_NICACORRFITHDFUNC_H_ */
