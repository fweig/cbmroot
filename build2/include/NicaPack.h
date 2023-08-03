/*
 * NicaPack.h
 *
 *  Created on: 21-10-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAPACK_H_
#define NICAPACK_H_

#include <TNamed.h>
#include <TString.h>


/**
 * Class for objects that can be extracted by HTML extractor. In contrast to
 * NicaPackage NicaPack usually store object that should be treated as single
 * object e.g. parameter. In some rare cases it migth contain more objects if
 * they are connecte. Example of such object are NicaDIvidedHisto-based classes
 * * that store two histograms (numerator and denominator).
 */
class NicaPack : public TNamed {
 public:
  /**
   * default constructor
   */
  NicaPack();
  /**
   * not used
   * @return extraction type
   */
  virtual TString GetExtractType() const;
  /**
   * exttact this object into HTML
   * @param no (used to numbering directories to avoid name conflicts)
   * @param dir directory where object should be extracted
   * @return value or Url to html with report
   */
  virtual TString HTMLExtract(Int_t /*no*/, TString /*dir*/ = "") const;
  /**
   * add object of the same class to this
   * @param pack added object
   */
  virtual void Add(const NicaPack *pack);
  /**
   * used to merge similar objects
   * @param collection
   */
  virtual Long64_t Merge(TCollection *collection);
  virtual ~NicaPack();
  ClassDef(NicaPack, 1)
};

#endif /* NICAPACK_H_ */
