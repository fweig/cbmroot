/*
 * NicaCut.h
 *
 *  Created on: 20-06-2013
 *      Author: Daniel Wielanek
 */
/** Basic class to hold all kind of cuts
 **/

#ifndef _NICACUT_H_
#define _NICACUT_H_

#include <TNamed.h>
#include <TString.h>

#include "NicaStd.h"
#include "NicaSubCut.h"

class NicaPackage;

class NicaEventComplexCut;
class NicaEventImaginaryCut;
class NicaEventRealCut;

class NicaTrackComplexCut;
class NicaTrackImaginaryCut;
class NicaTrackRealCut;

class NicaTwoTrackComplexCut;
class NicaTwoTrackImaginaryCut;
class NicaTwoTrackRealCut;

/**
 * basic abstract class for all cuts
 */
class NicaCut : public TNamed {
  friend class NicaEventComplexCut;
  friend class NicaEventImaginaryCut;
  friend class NicaEventRealCut;

  friend class NicaTrackComplexCut;
  friend class NicaTrackImaginaryCut;
  friend class NicaTrackRealCut;

  friend class NicaTwoTrackComplexCut;
  friend class NicaTwoTrackImaginaryCut;
  friend class NicaTwoTrackRealCut;

private:
  /**
   *  number of objects that passed thought cut
   *   */
  ULong64_t fPassed;
  /**
   *  number of objects that didn't passed thought cut
   *   */
  ULong64_t fFailed;
  /**
   * total number of object that has been checked by this cut
   */
  ULong64_t fTotal;
  /**
   * NicaSubCut object used for storing and checking values
   */
  NicaSubCut fSubCut;

protected:
  /**
   * number of parameter checked by cut
   */
  const Int_t fCutSize;
  /**
   * return true if cut has been initialized
   */
  Bool_t fInit;
  /**
   * not used
   */
  Int_t fIsCloned;
  /**
   * state of this cut 0 -if object didn't pass, 1 if passed
   */
  Int_t fState;
  /**
   * collection number of this cut
   */
  Int_t fCollectionID;
  /**
   * ration of uddating this cut
   */
  const ENicaCutUpdate fUpdateRatio;
  /**
   * weight of cut (not used)
   */
  Double_t fWeight;
  /**
   * group of cut (used for description of axes)
   */
  const TString fGroupFlag;
  /**
   *
   * @return true if objcet passed
   */
  Bool_t Validate();
  /**
   * similar to validate but instead passing obj that is between min and max,
   * pass object that is not between those two values
   * @return true if objcet passed
   */
  Bool_t AntiValidate();
  /**
   *
   * @return true if abs values passed
   */
  Bool_t ValidateAbs();
  /**
   * update value of cut without validating
   * @param state new state (true if object is ok, false otherwise)
   * @return state value
   */
  Bool_t ForcedUpdate(Bool_t state);
  /**
   * check if this used format is supported by this cut
   * @param format required format
   * @param format_id format id (from Init())
   * @return true if format with given format_id is the same as required
   */
  Bool_t FormatEquals(TString format, Int_t format_id, ENicaFormatDepth depth = ENicaFormatDepth::kAll) const;
  /**
   * check if this used format is supported by this cut
   * @param format required format
   * @param format_id format id (from Init())
   * @return true if format with given format_id inherints from "format"
   */
  Bool_t FormatInhertis(TString format, Int_t format_id, ENicaFormatDepth depth = ENicaFormatDepth::kAll) const;
  /**
   *
   * @param par parameter number
   * @return true if parameter valu is in limits
   */
  Bool_t InLimits(Int_t par) const;
  /**
   *
   * @param par parameter number
   * @return true if prameter is outside limits
   */
  Bool_t OutLimits(Int_t par) const { return !InLimits(par); };

public:
  /** default constructor
   **@param size - number of values in cut
   */
  NicaCut(const Int_t size = 1, ENicaCutUpdate update = ENicaCutUpdate::kNoUpdate, TString groupFlag = "");
  /**
   * copy constructor
   * @param cut
   */
  NicaCut(const NicaCut& cut);
  /**
   * default assignement operator
   * @param other
   * @return
   */
  NicaCut& operator=(const NicaCut& other);
  /**
   * not used
   */
  void Finish() {};
  /**
   * reset counters
   */
  void Reset();
  /**
   *  @see SetMinMax, call SetMinMax(val,val)
   * @param val value of cut
   * @param i parameter number
   */
  void SetMinAndMax(Double_t val, Int_t i = 0);
  /**
   * set mim and maximum values of parameters
   * @param min minimum value of paramter
   * @param max maximum value of parameter
   * @param i parameter number
   */
  void SetMinMax(Double_t min, Double_t max, Int_t i = 0);
  /**
   *
   * @return flag group
   */
  TString GetGroupFlag() const { return fGroupFlag; };
  /**
   *
   * @return update ratio
   */
  ENicaCutUpdate GetUpdateRatio() const { return fUpdateRatio; };
  /**
   * return copies of this cut
   * @param no number of copies
   * @param increment_collections if true then collection number are incremented
   * by 1
   * @return array of clones
   */
  NicaCut** MultiClone(Int_t no, Bool_t increment_collections = kFALSE);
  /**
   * set mimal value of cut for given parameter
   * @param min mimimal value
   * @param i parameter number
   */
  inline void SetMinimum(Double_t min, Int_t i = 0) { fSubCut.SetMin(min, i); };
  /**
   * set maximal value of cut for given parameter
   * @param max maimum value
   * @param i parameter number
   */
  inline void SetMaximum(Double_t max, Int_t i = 0) { fSubCut.SetMax(max, i); };
  /**
   * set value of parameter (later used for veryficatio
   * @param val value of parameter
   * @param i parameter number
   */
  inline void SetValue(Double_t val, Int_t i = 0) { fSubCut.SetValue(val, i); };
  /**
   * set unit name
   * @param name unit name
   * @param i parameter no
   */
  inline void SetUnitName(TString name, Int_t i = 0) { fSubCut.SetUnitName(name, i); };
  /**
   * set collection number for this cut, collection number define in witch cut
   * collection this cut will be stored
   * @param i collection number
   */
  inline void SetCollectionID(Int_t i) { fCollectionID = i; };
  /**
   *
   * @return number of checked parameters
   */
  inline Int_t GetCutSize() const { return fCutSize; };
  /**
   *
   * @return collection number
   */
  inline Int_t GetCollectionID() const { return fCollectionID; };
  /**
   *
   * @return true if last object pass throught this cut, false otherwise
   */
  inline Bool_t HasPassed() const { return fPassed; };
  /**
   *
   * @return true if this cut is cloned (not used)
   */
  inline Bool_t IsCloned() const { return fIsCloned; };
  /**
   *
   * @param i parameter number
   * @return mimal value of parameter
   */
  inline Double_t GetMin(Int_t i = 0) const { return fSubCut.GetMin(i); };
  /**
   *
   * @param i parameter number
   * @return maximum value of parameter
   */
  inline Double_t GetMax(Int_t i = 0) const { return fSubCut.GetMax(i); };
  /**
   *
   * @param i parameter value
   * @return last checked value of parameter
   */
  inline Double_t GetValue(Int_t i = 0) const { return fSubCut.GetValue(i); };
  /**
   *
   * @return weight of checked object (not used)
   */
  inline Double_t GetWeight() { return fWeight; };
  /**
   *
   * @return number of objects that pass thought this cut
   */
  inline ULong64_t GetPassed() const { return fPassed; };
  /**
   *
   * @return total number of checked objects
   */
  inline ULong64_t GetTotal() const { return fTotal; };
  /**
   *
   * @return number of objects that didn't pass thought this cut
   */
  inline ULong64_t GetFailed() const { return fFailed; };
  /**
   *
   * @param i parameter no
   * @return unit name
   */
  inline TString GetUnit(Int_t i) const { return fSubCut.GetUnit(i); };
  /**
   * copy of this cut, can be implemented by CutDef macro (if cut don't have
   * dynamically allocated objects or have copy ctor
   * @return copy of this cut
   */
  virtual NicaCut* MakeCopy() const { return (NicaCut*) this->Clone(); };
  /**
   *
   * @return report about this cut
   */
  virtual NicaPackage* Report() const;
  /**
   * print information about this cut
   */
  virtual void Print(Option_t* option = "") const;
  /**
   * create many cuts with acceptations of this_cut/n. For example if we have
   * "AB" cut with A:<-10;10> and B<0;5> AB->Split(2,0) create two cuts:
   * (A<-10;0> B<0;5>) and (A<0;10> B<0;5>) whereas AB->Split(2,1) creates
   * (A<-10;10> B<0,2.5>) and (A<-10;10> B<2.5;5>);
   * @param n number of cuts
   * @param i paramter of cut that will be used for splitiing
   * @param shift shift parameter - used if you want to avoid "overlapping of
   * cuts. This shifts boundary of cuts. If shift is positive then lower
   * boundary is increased, if shift is negative then  upper boundary is
   * decreased. For example if cuts is <0,10> Split(2,0,-1) makes cuts <0.4>
   * <5,10>) Split(2,0,0.5) makes <0,5> <5.5,10>
   * @return
   */
  TObjArray* Split(Int_t n, Int_t i = 0, Double_t shift = 0) const;
  /**
   * initialize object
   * @param format_id id of format used in current task
   */
  virtual Bool_t Init(Int_t /*format_id*/ = 0) {
    fInit = kTRUE;
    return kTRUE;
  };
  /**
   * @param opt additional option, if you add this cut directly then not used,
   * however if you create pure real /imaginary cut then you can get obtain
   * complex cut name by set option "re" or "im"
   * @return name of cut (usally Classname)
   */
  virtual TString CutName(Option_t* opt = "") const;
  virtual ~NicaCut() {};
  ClassDef(NicaCut, 1)
};

#endif /* NICACUT_H_ */
