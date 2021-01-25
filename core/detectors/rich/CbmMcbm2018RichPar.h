#ifndef CbmMcbm2018RichPar_H
#define CbmMcbm2018RichPar_H

#include "FairParGenericSet.h"  // mother class

// STD
#include <map>

// ROOT
#include <TArrayD.h>
#include <TArrayI.h>

class CbmMcbm2018RichPar : public FairParGenericSet {
public:
  CbmMcbm2018RichPar(const char* name    = "CbmMcbm2018RichPar",
                     const char* title   = "RICH unpacker parameters",
                     const char* context = "Default");

  virtual ~CbmMcbm2018RichPar();

  virtual void putParams(FairParamList*);

  virtual Bool_t getParams(FairParamList*);

public:
  Int_t GetNaddresses(void) const { return fTRBaddresses.GetSize(); }

  Int_t GetAddressIdx(Int_t addr) const;

  Int_t GetAddress(Int_t ind) const;

  /**
	 * First argument is TDC ID (i.e. 0x7210)
	 * TODO: test!
	 */
  Double_t GetToTshift(Int_t tdc, Int_t ch) const {
    Int_t tdcIdx = this->GetAddressIdx(tdc);
    return this->GetToTshift2(tdcIdx, ch);
  }

  /**
	 * First argument is TDC index (i.e. 0,1,2,...)
	 * TODO: test!
	 */
  Double_t GetToTshift2(Int_t tdcIdx, Int_t ch) const {
    return fToTshiftMap.at(tdcIdx * 33 + ch);
  }

private:  // Stored in the par file
  /**
	 * List of TRB addresses
	 * required for unpacking (at least)
	 */
  TArrayI fTRBaddresses;

  /**
	 * Array of shifts added to the calculated ToTs
	 * for each channel. The array is organized as follows:
	 * For each entry in the fTRBaddresses there is a sub-array
	 * of 33 elements corresponding to the 33 channels of the
	 * TDC. These sub-arrays are listed one after another in the
	 * order the TDCs appear in the fTRBaddresses array.
	 */
  TArrayD fToTshifts;

private:  // Recalculated
  /**
	 * key - TRB address (0x7211), value - index in the array
	 */
  std::map<Int_t, Int_t> fTRBaddrMap;  //!

  /**
	 * key - unique channel ID, value - ToT shift
	 */
  std::map<Int_t, Double_t> fToTshiftMap;  //!

  ClassDef(CbmMcbm2018RichPar, 1);
};

#endif  // CbmMcbm2018RichPar_H
