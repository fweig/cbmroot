// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmROCUnpackHodo                         -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACKHODO_H
#define CBMROCUNPACKHODO_H

#include <Rtypes.h>        // for ClassDef
#include <RtypesCore.h>    // for Bool_t, ULong_t
#include "CbmROCUnpack.h"  // for CbmROCUnpack
#include <boost/any.hpp>
#include <vector>
#include "CbmFiberHodoDigi.h"

#include "CbmHistManager.h" // for RootCling

class CbmDaqMap;
class CbmSourceLmdNew;
class CbmTbDaqBuffer;

namespace roc { class Message; }

class CbmROCUnpackHodo : public CbmROCUnpack
{
 public:
  
  CbmROCUnpackHodo();
  virtual ~CbmROCUnpackHodo();
    
  virtual Bool_t Init();
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime);
  virtual void FillOutput(boost::any);
  virtual void Reset();
  virtual void Finish();

 private:

  CbmTbDaqBuffer* fBuffer;        ///< Digi buffer instance
  CbmDaqMap* fDaqMap;             ///< Mapping from electronics to detectors
  CbmSourceLmdNew* fSource;
  std::vector<CbmFiberHodoDigi>* fHodoDigis;         ///< Output array of CbmHodoDigi
  std::vector<CbmFiberHodoDigi>* fHodoBaselineDigis; ///< Output array for baseline calib.
  CbmHistManager* fHM;  ///< Histogram manager

  void CreateHistograms();

  CbmROCUnpackHodo(const CbmROCUnpackHodo&);
  CbmROCUnpackHodo& operator=(const CbmROCUnpackHodo&);
  
  ClassDef(CbmROCUnpackHodo, 2)
};

#endif
