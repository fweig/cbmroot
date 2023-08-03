/*
 * NicaSubCut.h
 *
 *  Created on: 10 gru 2016
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICASUBCUT_H_
#define NICASUBCUT_H_

#include <TObject.h>
#include <TString.h>


class TH1;

/**
 * small class for storing cut parameters
 */
class NicaSubCut : public TObject {
  Int_t fSize;
  Double_t *fMin;      //[fSize]
  Double_t *fMax;      //[fSize]
  Double_t *fValue;    //[fSize]
  TString *fUnitName;  //[fSize]
 public:
  /**
   * constructor
   * @param size default sub-cut size
   */
  NicaSubCut(Int_t size = 1);
  /**
   * set name of  cut unit on given parameter
   * @param name unit name
   * @param i parameter numer
   */
  void SetUnitName(TString name, Int_t i = 0) { fUnitName[i] = name; };
  /**
   * set minimum of given sub-cut parameter
   * @param val minimum  (axis)
   * @param i parameter no
   */
  void SetMin(Double_t val, Int_t i = 0);
  /**
   * set maximum  of given sub-cut parameter (axis)
   * @param val maximum
   * @param i parameter no
   */
  void SetMax(Double_t val, Int_t i = 0);
  /**
   * check values
   * @return return true if  values for all axes are between fMin and fMax
   */
  Bool_t Validate();
  /**
   * check values
   * @return return true if  absolute values for all axes are between fMin and
   * fMax
   */
  Bool_t ValidateAbs();
  /**
   *
   * @param i axis (parameter no)
   * @return unit name
   */
  TString GetUnit(Int_t i) const { return fUnitName[i]; };
  /**
   * operator =
   * @param other
   * @return
   */
  NicaSubCut &operator=(const NicaSubCut &other);
  /**
   *
   * @return number of paramters in this subcut
   */
  inline Int_t GetSize() const { return fSize; };
  /**
   * set value of given parameter
   * @param val value
   * @param i parameter no
   */
  inline void SetValue(Double_t val, Int_t i = 0) { fValue[i] = val; };
  /**
   * return lower edge of cut for given parameter
   * @param i parameter no
   * @return lower edge value
   */
  inline Double_t GetMin(Int_t i) const { return fMin[i]; };
  /**
   * return upper edge of cut for given parameter
   * @param i parameter no
   * @return upper edge value
   */
  inline Double_t GetMax(Int_t i) const { return fMax[i]; };
  /**
   * return latest value put by SetValue with given parameter number
   * @param i parameter no
   * @return value
   */
  inline Double_t GetValue(Int_t i) const { return fValue[i]; };
  /**
   * copy constructor
   * @param other
   */
  NicaSubCut(const NicaSubCut &other);
  virtual ~NicaSubCut();
  ClassDef(NicaSubCut, 1)
};

/**
 * Similar helper like NicaSubCut but instead of parameters - histogram of
 * acceptance is used
 */
class NicaSubCutHisto : public TObject {
  Int_t fSize;
  Double_t *fValue;    //[fSize]
  TString *fUnitName;  //[fSize]
  TH1 *fAcceptanceHistogram;

 public:
  /**
   * constructor
   * @param size default sub-cut size
   */
  NicaSubCutHisto(Int_t size = 3);
  /**
   * set name of  cut unit on given parameter
   * @param name unit name
   * @param i parameter numer
   */
  void SetUnitName(TString name, Int_t i = 0) { fUnitName[i] = name; };
  /**
   *
   * @return number of checked values
   */
  Int_t GetSize() const { return fSize; };
  /**
   * set acceptance histogram
   * @param h histogram with acceptance, shoud have the same numerf of dimesions
   * like number fSize, values should be between 0 and 1 and describe probablity
   * of accepting/rejecting object
   * @return false if histogram cannot be used here
   */
  Bool_t SetAcceptanceHistogram(TH1 *h);
  /**
   * check values
   * @return return true if  values for all axes are between fMin and fMax
   */
  Bool_t Validate();
  /**
   * check values
   * @return return true if  absolute values for all axes are between fMin and
   * fMax
   */
  Bool_t ValidateAbs();
  /**
   *
   * @param i axis (parameter no)
   * @return unit name
   */
  TString GetUnit(Int_t i) const { return fUnitName[i]; };
  /**
   * operator =
   * @param other
   * @return
   */
  NicaSubCutHisto &operator=(const NicaSubCutHisto &other);
  /**
   * set value of given parameter
   * @param val value
   * @param i parameter no
   */
  inline void SetValue(Double_t val, Int_t i = 0) { fValue[i] = val; };
  /**
   * return latest value put by SetValue with given parameter number
   * @param i parameter no
   * @return value
   */
  inline Double_t GetValue(Int_t i) const { return fValue[i]; };
  /**
   *
   * @return accpetance histogram
   */
  TH1 *GetHisto() const { return fAcceptanceHistogram; };
  /**
   * check if everything is ok
   * @return tru if this is ok
   */
  Bool_t Init();
  /**
   * copy constructor
   * @param other
   */
  NicaSubCutHisto(const NicaSubCutHisto &other);
  virtual ~NicaSubCutHisto();
  ClassDef(NicaSubCutHisto, 1)
};

#endif /* NICASUBCUT_H_ */
