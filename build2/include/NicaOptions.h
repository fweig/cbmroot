/*
 * NicaOptions.h
 *
 *  Created on: 21-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAOPTIONS_H_
#define NICAOPTIONS_H_

#include <TClonesArray.h>
#include <TObject.h>

#include "NicaCout.h"
// array that recognize labels
/** base class for hold options in some arrays **/
class NicaOptionArray : public TObject {
  Int_t fNoLabels;
  TClonesArray **fParameters;  //[fNoLabels]
  TClonesArray *fLabels;
  TString GetLabel(Int_t i) const;

 public:
  NicaOptionArray();
  /** register label
   * @param label - label name
   */
  void RegisterLabel(TString label);
  /** add option to array
   * @param option - option to add
   */
  Bool_t Add(TString option);
  /** get label for give option e.g for data:format - return data
   * @param label - label for given option
   * @param no - number of option with given label
   */
  TString GetByLabel(TString label, Int_t no = 0) const;
  /** get label by number e.g if registered labes "A" and "B" GetByLabelNo("B")
   * return 1
   * @param label - label
   * */
  Int_t GetByLabelNo(TString label) const;
  /** default destructor */
  void Print(Option_t * /*opt*/ = "") const;
  virtual ~NicaOptionArray();
  ClassDef(NicaOptionArray, 1)
};
/**
 * convert ooption to int
 */
class NicaOptionConverter : public TObject {
  Int_t fSize;
  TString *fNames;  //[fSize]
  Int_t *fValues;   //[fSize]
 public:
  NicaOptionConverter();
  /**
   * convert option to int
   * @param val integer for option
   * @param option option
   * @return true if object found
   */
  Bool_t GetOptionInt(Int_t &val, TString option) const;
  /**
   * find string for given int
   * @param val string
   * @param value integer
   * @return true if found
   */
  Bool_t GetString(TString &val, Int_t value) const;
  /**
   * register option
   * @param option option
   */
  void RegisterOption(TString option);
  /**
   * register option with given int
   * @param option option
   * @param no number
   */
  void RegisterOption(TString option, Int_t no);
  virtual ~NicaOptionConverter();
  ClassDef(NicaOptionConverter, 1)
};

#endif /* NICAOPTIONS_H_ */
