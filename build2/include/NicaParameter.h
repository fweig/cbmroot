/*
 * NicaParameter.h
 *
 *  Created on: 21-06-2013
 *      Author: Daniel Wielanek
 */

#ifndef NICANAMEDVALUE_H_
#define NICANAMEDVALUE_H_

#include <TString.h>
#include "NicaPack.h"

/**
 * abstract for storing parameters, upgraded TParameter known from root
 */

class TBrowser;

class NicaParameter : public NicaPack {
 protected:
  /**
   * merging option
   */
  enum class ENicaParameterMerge {
    kPlus,      //!< kPlus
    kMultiply,  //!< kMultiply
    kEqual,     //!< kEqual
    kMaximum,   //!< kMaximum
    kMinimum,   //!< kMinimum
    kFirst,     //!< kFirst
    kLast,      //!< kLast
    kAverage    //!< kAverage
  };
  /**
   * mergin mode for this parameter
   */
  ENicaParameterMerge fMergeMode;

 public:
  /**
   * default constructor shouldn't be used
   */
  NicaParameter();
  /**
   *  main constructor
   * @param name name of parameter
   * @param merge_mode mode of merging
   */
  NicaParameter(TString name, Char_t merge_mode = '=');
  /**
   * set merge mode
   * @param mergemode one of following modes '+' for adding '=' if merged nodes
   * must have similar values, other describe how value to this node (X) will be
   * set according to value from added node (Y)
   * '*' X=X*Y , 'f' X=X, 'l' X=Y, M X = max(X,Y), m X = min(X,Y), 'a' X =
   * (X+Y)/2
   */
  void SetMergeMode(Char_t mergemode);
  /**
   *
   * @param object other object the same class
   * @return true if both objects have the same name
   */
  Bool_t CheckName(const TObject *object);
  /**
   * used to merge similar objects
   * @param collection
   */
  Long64_t Merge(TCollection *collection);
  /**
   * used in TBrowser
   * @param b
   */
  void Browse(TBrowser *b);
  /**
   * extract this into HTML by return formated value of parameter
   * @param counter - not used
   * @param dir - not used
   * @return formated value
   */
  virtual TString HTMLExtract(Int_t /*counter*/ = 0,
                              TString /*dir*/ = " ") const {
    return "NULL";
  };
  virtual ~NicaParameter();
  ClassDef(NicaParameter, 1)
};
/**
 * parameter to store Int_t
 */
class NicaParameterInt : public NicaParameter {
  Int_t fValue;

 public:
  NicaParameterInt();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterInt(TString name, Int_t value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(Int_t value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline Int_t GetValue() const { return fValue; };
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterInt();
  ClassDef(NicaParameterInt, 1)
};
/**
 * parameter to store Double_t
 */
class NicaParameterDouble : public NicaParameter {
  Double_t fValue;

 public:
  NicaParameterDouble();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterDouble(TString name, Double_t value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(Double_t value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline Double_t GetValue() const { return fValue; };
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterDouble();
  ClassDef(NicaParameterDouble, 1)
};
/**
 * parameter to store Float_t
 */
class NicaParameterFloat : public NicaParameter {
  Float_t fValue;

 public:
  NicaParameterFloat();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterFloat(TString name, Float_t value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(Float_t value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline Float_t GetValue() const { return fValue; };
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterFloat();
  ClassDef(NicaParameterFloat, 1)
};
/**
 * parameter to store TString
 */
class NicaParameterString : public NicaParameter {
  TString fValue;

 public:
  NicaParameterString();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterString(TString name, TString value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(TString value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline TString GetValue() const { return fValue; };
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterString();
  ClassDef(NicaParameterString, 1)
};
/**
 * parameter to store UInt_t
 */
class NicaParameterUInt : public NicaParameter {
  UInt_t fValue;

 public:
  NicaParameterUInt();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterUInt(TString name, UInt_t value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(UInt_t value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline UInt_t GetValue() const { return fValue; };
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterUInt();
  ClassDef(NicaParameterUInt, 1)
};
/**
 * parameter to store ULong_t
 */
class NicaParameterULong : public NicaParameter {
  ULong_t fValue;

 public:
  NicaParameterULong();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterULong(TString name, ULong_t value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(ULong_t value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline ULong_t GetValue() const { return fValue; };
  TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterULong();
  ClassDef(NicaParameterULong, 1)
};
/**
 * parameter to store ULong64_t
 */
class NicaParameterULong64 : public NicaParameter {
  ULong64_t fValue;

 public:
  NicaParameterULong64();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterULong64(TString name, ULong64_t value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(ULong64_t value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline ULong64_t GetValue() const { return fValue; };
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterULong64();
  ClassDef(NicaParameterULong64, 1)
};
/**
 * parameter to store Bool_t
 */
class NicaParameterBool : public NicaParameter {
  Bool_t fValue;

 public:
  NicaParameterBool();
  /**
   *  main constructor
   * @param name name of parameter
   * @param value value of parameter
   * @param merge_mode mode of merging
   */
  NicaParameterBool(TString name, Bool_t value, Char_t merge_mode = '=');
  ;
  void Add(const NicaPack *val);
  /**
   * set value
   * @param value new value
   */
  inline void SetValue(Bool_t value) { fValue = value; };
  /**
   *
   * @return value of parameter
   */
  inline Bool_t GetValue() const { return fValue; };
  virtual TString HTMLExtract(Int_t counter = 0, TString dir = " ") const;
  virtual ~NicaParameterBool();
  ClassDef(NicaParameterBool, 1)
};

#endif /* NICANAMEDVALUE_H_ */
