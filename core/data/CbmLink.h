/* Copyright (C) 2013-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Volker Friese */

/**
 * \file CbmLink.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 *
 * Base data class for storing MC information link.
 **/

#ifndef CBMLINK_H_
#define CBMLINK_H_

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>
#include <string>  // for string

class CbmLink : public TObject {
public:
  /**
    * \brief Constructor.
    */
  CbmLink();

  /**
    * \brief Standard constructor.
    */
  CbmLink(float weight, int32_t index, int32_t entry = -1, int32_t file = -1);

  /**
    * \brief Destructor.
    */
  virtual ~CbmLink();

  /* Modifiers */
  int32_t GetFile() const { return fFile; }
  int32_t GetEntry() const { return fEntry; }
  int32_t GetIndex() const { return fIndex; }
  float GetWeight() const { return fWeight; }

  /* Accessors */
  void SetFile(int32_t file) { fFile = file; }
  void SetEntry(int32_t entry) { fEntry = entry; }
  void SetIndex(int32_t index) { fIndex = index; }
  void SetWeight(float weight) { fWeight = weight; }

  void AddWeight(float weight) { fWeight += weight; }

  /**
    * \brief Return string representation of the object.
    * \return String representation of the object.
    **/
  virtual std::string ToString() const;

  friend bool operator==(const CbmLink& lhs, const CbmLink& rhs)
  {
    return (lhs.GetFile() == rhs.GetFile() && lhs.GetEntry() == rhs.GetEntry() && lhs.GetIndex() == rhs.GetIndex());
  }

  friend Bool_t operator!=(const CbmLink& lhs, const CbmLink& rhs) { return !(lhs == rhs); }

  /** Comparison operators by //Dr.Sys **/
  friend bool operator<(const CbmLink& l, const CbmLink& r)
  {
    if (l.GetFile() == r.GetFile()) {
      if (l.GetEntry() == r.GetEntry()) return l.GetIndex() < r.GetIndex();
      return l.GetEntry() < r.GetEntry();
    }
    return l.GetFile() < r.GetFile();
  }

  friend bool operator>(const CbmLink& l, const CbmLink& r)
  {
    if (l.GetFile() == r.GetFile()) {
      if (l.GetEntry() == r.GetEntry()) return l.GetIndex() > r.GetIndex();
      return l.GetEntry() > r.GetEntry();
    }
    return l.GetFile() > r.GetFile();
  }


private:
  int32_t fFile;   // File ID
  int32_t fEntry;  // Entry number
  int32_t fIndex;  // Index in array
  float fWeight;   // Weight

  ClassDef(CbmLink, 1)
};

#endif /* CBMLINK_H_ */
