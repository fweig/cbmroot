/* Copyright (C) 2013-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Florian Uhlig */

/**
 * \file CbmMatch.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 *
 * Base data class for storing RECO-to-MC matching information.
 **/

#ifndef CBMMATCH_H_
#define CBMMATCH_H_

#include "CbmLink.h"  // for CbmLink

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>
#include <memory>  // for unique_ptr
#include <string>  // for string
#include <vector>  // for vector

class CbmMatch : public TObject {
public:
  /**
    * \brief Default constructor.
    */
  CbmMatch();

  /**
    * \brief Destructor.
    */
  virtual ~CbmMatch();

  /* Accessors */
  const CbmLink& GetLink(int32_t i) const { return fLinks.at(i); }
  const std::vector<CbmLink>& GetLinks() const { return fLinks; }
  const CbmLink& GetMatchedLink() const { return fLinks.at(fMatchedIndex); }
  int32_t GetNofLinks() const { return fLinks.size(); }
  double GetTotalWeight() const { return fTotalWeight; }

  /* Modifiers */
  void AddLinks(const CbmMatch& match);
  void AddLink(const CbmLink& newLink);
  void AddLink(double weight, int32_t index, int32_t entry = -1, int32_t file = -1);
  void ClearLinks();

  /**
    * \brief Return string representation of the object.
    * \return String representation of the object.
    **/
  virtual std::string ToString() const;

protected:
  std::vector<CbmLink> fLinks;  // List of links to MC
  double fTotalWeight;          // Sum of all reference weights
  int32_t fMatchedIndex;        // Index of the matched reference in fReferences array

  ClassDef(CbmMatch, 1);
};

typedef std::unique_ptr<CbmMatch> up_CbmMatch;

#endif /* CBMMATCH_H_ */
