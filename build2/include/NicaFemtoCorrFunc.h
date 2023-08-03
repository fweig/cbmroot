/*
 * NicaCorrFuncCollection.h
 *
 *  Created on: 4 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACORRFUNCCOLLECTION_H_
#define NICACORRFUNCCOLLECTION_H_


#include <Rtypes.h>
#include <RtypesCore.h>
#include <TString.h>
#include <initializer_list>
#include <iostream>

#include "NicaDividedHisto.h"
#include "NicaHtmlFile.h"
#include "NicaArray.h"
#include "NicaObjectMatrix.h"
#include "NicaParameter.h"

class NicaFemtoPair;

/**
 * basic class for storing correlation function (binned)
 */
class NicaFemtoCorrFunc : public NicaPack {
  NicaParameterInt fEventCol;
  NicaParameterInt fPairCol;

 protected:
  NicaObjectMatrix_1 *fArray;
  /**
   * deafult c-tor
   * @param h correlaction function to duplication
   * @param size number of correlation function bins (total)
   */
  NicaFemtoCorrFunc(const NicaDividedHisto1D *h, Int_t size = 0);

 public:
  NicaFemtoCorrFunc();
  NicaFemtoCorrFunc(const NicaDividedHisto1D &h, Int_t size = 0);
  NicaFemtoCorrFunc(const NicaFemtoCorrFunc &other);
  NicaFemtoCorrFunc &operator=(const NicaFemtoCorrFunc &other);
  /**
   * additional check of sizes, ranges
   * @return
   */
  virtual Bool_t Check() = 0;
  /**
   * add those objects
   * @param pack
   */
  virtual void Add(const NicaPack *pack);
  virtual void Browse(TBrowser *b);
  /**
   * fill numerator
   * @param pair
   */
  virtual void FillNum(NicaFemtoPair *pair) = 0;
  virtual void FillDenPerfect(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenRotated(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenMixed(NicaFemtoPair *pair) = 0;
  virtual void FillDenHemisphere(NicaFemtoPair *pair) { FillDenMixed(pair); };
  virtual void FillDenCharged(NicaFemtoPair *pair) { FillDenMixed(pair); };
  /**
   * set event collection ID
   * @param no
   */
  void SetEventCollID(Int_t no) { fEventCol.SetValue(no); };
  /**
   * set pair collection ID
   * @param no
   */
  void SetPairCollID(Int_t no) { fPairCol.SetValue(no); };
  /**
   *
   * @return event collection ID
   */
  Int_t GetEventCollID() const { return fEventCol.GetValue(); };
  /**
   *
   * @return pair collection ID
   */
  Int_t GetPairCollID() const { return fPairCol.GetValue(); };
  /**
   *
   * @return total number of subfunction
   */
  Int_t GetEntries() const { return fArray->GetSize(); };
  /**
   * return correlation function at given position
   * @param i
   * @return
   */
  NicaDividedHisto1D *GetCF(Int_t i) const {
    return (NicaDividedHisto1D *)fArray->At(i);
  };
  virtual ~NicaFemtoCorrFunc();
  ClassDef(NicaFemtoCorrFunc, 1)
};

class NicaFemtoCorrFunc1D : public NicaFemtoCorrFunc {
 protected:
  const TString fLabel;
  NicaArray_1<Float_t> fRange;
  void SetBins(const std::initializer_list<double> &array);
  virtual void AddHTMLCode(NicaHtmlFile & /*file*/) const {};
  /**
   *
   * @param h cf histogram to clone
   * @param label binning labes (e.g. kT)
   * @param size numer of CF's
   */
  NicaFemtoCorrFunc1D(const NicaDividedHisto1D *h, TString label = "",
                      Int_t size = 0);

 public:
  NicaFemtoCorrFunc1D() : NicaFemtoCorrFunc(){};
  NicaFemtoCorrFunc1D(const NicaDividedHisto1D &h, TString label = "",
                      Int_t size = 0);
  virtual Bool_t Check();
  virtual TString HTMLExtract(Int_t no, TString dir = "") const;
  TString GetLabel() const { return fLabel; };
  const NicaArray_1<Float_t> GetRange() const { return fRange; };
  virtual ~NicaFemtoCorrFunc1D();
  ClassDef(NicaFemtoCorrFunc1D, 1)
};

class NicaFemtoCorrFunc2D : public NicaFemtoCorrFunc {
 protected:
  const TString fLabelX, fLabelY;
  NicaArray_1<Float_t> fRangeX;
  NicaArray_1<Float_t> fRangeY;
  virtual void AddHTMLCode(std::ofstream & /*file*/) const {};
  NicaFemtoCorrFunc2D(const NicaDividedHisto1D *h, TString labelX = "",
                      TString labelY = "", Int_t sizeX = 0, Int_t sizeY = 0);

 public:
  NicaFemtoCorrFunc2D() : NicaFemtoCorrFunc(){};
  NicaFemtoCorrFunc2D(const NicaDividedHisto1D &h, TString labelX = "",
                      TString labelY = "", Int_t sizeX = 0, Int_t sizeY = 0);
  virtual Bool_t Check();
  virtual TString HTMLExtract(Int_t no, TString dir = "") const;
  TString GetXlabel() const { return fLabelX; };
  TString GetYlabel() const { return fLabelY; };
  Int_t GetXsize() const { return fRangeX.GetSize() - 1; };
  Int_t GetYsize() const { return fRangeY.GetSize() - 1; };
  const NicaArray_1<Float_t> GetRangeX() const { return fRangeX; };
  const NicaArray_1<Float_t> GetRangeY() const { return fRangeY; };
  NicaDividedHisto1D *GetCF(Int_t i, Int_t j) const;
  virtual ~NicaFemtoCorrFunc2D();
  ClassDef(NicaFemtoCorrFunc2D, 1)
};

#endif /* NICAFEMTO_ANALYSIS_FEMTO_BASE_NICACORRFUNCCOLLECTION_H_ */
