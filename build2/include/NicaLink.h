/*
 * NicaLink.h
 *
 *  Created on: 22 lut 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICALINK_H_
#define NICALINK_H_
#include <TObject.h>

#include "NicaArray.h"
/**
 * class for linking objects, contains id that point to other object
 *used especially in @see NicaTrack to provide information about track
 *dependencies
 */
class NicaLink : public TObject {
  Int_t fActiveSize;
  Int_t fSize;
  Int_t* fLinks;  // [fActiveSize]
public:
  /**
   * i - link from
   * @param i link size
   */
  NicaLink(Int_t i = 1);
  NicaLink(const NicaLink& link);
  /**
   * operator
   * @param link
   * @return
   */
  NicaLink operator=(const NicaLink& link);
  /**
   * set id of linked object and clean "to" array, if you don't want to clean
   * "to" array @see OverwriteFrom
   * @param i id of linked object
   */
  void SetNextLink(Int_t i);
  /**
   * set link to object at position val
   * @param index object number
   * @param val link value
   */
  void SetLink(Int_t index, Int_t val) { fLinks[index] = val; }
  void ExpandFast(Int_t size);
  void SetLinkAndExpand(Int_t index, Int_t val);
  /**
   * get link to index-th object connected with this object, this
   * method doesn't check link size!
   * @param index
   * @return
   */
  inline Int_t GetLink(Int_t index) const { return fLinks[index]; };
  /**
   *
   * @return number of objects conneted with this object
   */
  inline Int_t GetLinkSize() const { return fActiveSize; };
  virtual void Clear(Option_t* opt = "");
  void ClearLinks();
  virtual ~NicaLink();
  ClassDef(NicaLink, 1)
};

#endif /* NICALINK_H_ */
