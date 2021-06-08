/* Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Egor Ovcharenko [committer] */

#ifndef CbmMcbm2018UnpackerUtilRich_H
#define CbmMcbm2018UnpackerUtilRich_H

// STD
#include <string>

// ROOT
#include <Rtypes.h>

class CbmMcbmRichEdge {
public:
  CbmMcbmRichEdge(Int_t subsubevid, Int_t ch, Double_t time) : fSubSubEventID(subsubevid), fChannel(ch), fTime(time) {}
  ~CbmMcbmRichEdge() {}
  Int_t fSubSubEventID;
  Int_t fChannel;
  Double_t fTime;
};

namespace mRichSupport
{

  std::string GetBinaryRepresentation(size_t const size, uint8_t const* const ptr);

  /**
	 * size in bytes
	 */
  std::string GetHexRepresentation(size_t const size, uint8_t const* const ptr);

  std::string GetWordHexRepr(uint8_t const* const ptr);

  std::string GetWordHexReprInv(uint8_t const* const ptr);

  void SwapBytes(size_t const size, uint8_t const* ptr);

  void PrintRaw(size_t const size, uint8_t const* const ptr);

};  // namespace mRichSupport

#endif  // CbmMcbm2018UnpackerUtilRich_H
