/*
 * NicaGeneratorParamersArray.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAGENERATOPARAMETERSARRAY_H_
#define NICAGENERATOPARAMETERSARRAY_H_

#include <TObject.h>
#include <TString.h>


/**
 * basic class for stroing parameters transfered from NicaGeneratorRead to
 * NicaGeneratorWrite
 */
class NicaGeneratorParametersArray : public TObject {
  Int_t fIntSize;
  Int_t fDoubleSize;
  Int_t fStringSize;
  Int_t fLongSize;
  Int_t fCounter[4];
  Int_t *fIntArray;        //[fIntSize]
  Double_t *fDoubleArray;  //[fDoubleSize]
  TString *fStringArray;   //[fStringSize]
  ULong64_t *fLongArray;   //[fLongSize]
  TString *fIntNames;      //[fIntSize]
  TString *fIntLabels;     //[fStringSize]
  TString *fDoubleNames;   //[fDoubleSize]
  TString *fDoubleLabels;  //[fDoubleSize]
  TString *fStringNames;   //[fStringSize]
  TString *fStringLabels;  //[fStringSize]
  TString *fLongLabels;    //[fLongSize]
  TString *fLongNames;     //[fLongSize]
  void Resize(Char_t type, Int_t new_size);

 public:
  /**
   * default ct-r
   * @param size_i number of integers
   * @param size_d number of doubles
   * @param size_s number of strings
   * @param size_l number of long64
   */
  NicaGeneratorParametersArray(Int_t size_i = 0, Int_t size_d = 0,
                               Int_t size_s = 0, Int_t size_l = 0);
  /**
   *
   * @param pos parameter position
   * @return double parameter
   */
  inline Double_t GetDouble(Int_t pos) const { return fDoubleArray[pos]; };
  /**
   *
   * @param pos parameter position
   * @return int parameter
   */
  inline Int_t GetInt(Int_t pos) const { return fIntArray[pos]; };
  /**
   *
   * @param pos parameter position
   * @return string parameter
   */
  inline TString GetString(Int_t pos) const { return fStringArray[pos]; };
  /**
   *
   * @param pos postion
   * @return long value
   */
  inline ULong64_t GetLong(Int_t pos) const { return fLongArray[pos]; };
  /**
   *
   * @param pattern parameter name
   * @return index
   */
  Int_t GetIndex(TString pattern);
  /**
   *
   * @param pos parameter position
   * @param type parameter type i-int, d-double, s-string, u - ulong64
   * @return parameter name
   */
  TString GetParameterName(Int_t pos, Char_t type) const;
  /**
   *
   * @param pattern
   * @return parameter description
   */
  TString GetParameterDescription(TString pattern) const;
  /**
   *
   * @param pattern parameter name
   * @param desc parameter description
   * @param type  type of parameters i-int, d- double, s - string
   */
  void SetParameter(TString pattern, TString desc, Char_t type);
  /**
   * set double parameter
   * @param val value
   * @param pos position
   */
  inline void SetDouble(Double_t val, Int_t pos) { fDoubleArray[pos] = val; };
  /**
   * set int parameter
   * @param val value
   * @param pos position
   */
  inline void SetInt(Int_t val, Int_t pos) { fIntArray[pos] = val; };
  /**
   * set string parameter
   * @param val value
   * @param pos paraterm index
   */
  inline void SetString(Int_t val, Int_t pos) { fStringArray[pos] = val; };
  /**
   * set long value
   * @param val value
   * @param pos poistion
   */
  inline void SetLong(ULong64_t val, Int_t pos) { fLongArray[pos] = val; };
  /**
   * get nubmer of parameters
   * @param type  type of parameters i-int, d- double, s - string
   * @return size of array
   */
  Int_t GetSize(Char_t type) const;
  virtual ~NicaGeneratorParametersArray();
  ClassDef(NicaGeneratorParametersArray, 1)
};

#endif /* NICAGENERATOPARAMETERSARRAY_H_ */
