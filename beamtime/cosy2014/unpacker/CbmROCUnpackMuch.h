// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          CbmROCUnpackMuch                         -----
// -----                    Created 14.12.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACKMUCH_H
#define CBMROCUNPACKMUCH_H

#include "roc/Message.h"

#include "CbmROCUnpack.h"
#include <boost/any.hpp>
#include <vector>
#include "CbmMuchBeamTimeDigi.h"

namespace roc { class Message; }

class CbmTbDaqBuffer;
class CbmDaqMap;
class CbmSourceLmdNew;

class CbmROCUnpackMuch : public CbmROCUnpack
{
 public:
  
  CbmROCUnpackMuch();
  virtual ~CbmROCUnpackMuch();
    
  virtual Bool_t Init();
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime);
  virtual void FillOutput(boost::any);
  virtual void Reset();
  virtual void Finish() {;}

 private:

  CbmTbDaqBuffer* fBuffer;        ///< Digi buffer instance
  CbmDaqMap* fDaqMap;             ///< Mapping from electronics to detectors
  CbmSourceLmdNew* fSource;
  std::vector<CbmMuchBeamTimeDigi>* fMuchDigis;         ///< Output array of CbmMuchDigi
  std::vector<CbmMuchBeamTimeDigi>* fMuchBaselineDigis; ///< Output array for baseline calib.

  CbmROCUnpackMuch(const CbmROCUnpackMuch&);
  CbmROCUnpackMuch& operator=(const CbmROCUnpackMuch&);

  ClassDef(CbmROCUnpackMuch, 2)
};

#endif
