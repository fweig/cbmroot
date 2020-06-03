// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                             CbmROCUnpack                          -----
// -----                    Created 10.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMROCUNPACK_H
#define CBMROCUNPACK_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Bool_t, ULong_t, kTRUE
#include <TObject.h>     // for TObject
#include <boost/any.hpp>
#include <string>
#include <vector>
#include <FairRootManager.h>

namespace roc { class Message; }

class CbmROCUnpack : public TObject
{
 public:
  
  CbmROCUnpack();
  virtual ~CbmROCUnpack();
    
  virtual Bool_t Init() = 0;
  virtual Bool_t DoUnpack(roc::Message* Message, ULong_t hitTime) = 0;
  virtual void FillOutput(boost::any digi) = 0;
  virtual void Reset() = 0;
  virtual void Finish() = 0;

  void SetPersistence(Bool_t persistence = kTRUE) 
  { fPersistence = persistence; }

  /** @brief Register the output arrays
   **
   ** An array for the digis will be created and
   ** registered as output to the ROOT tree. The current implementation
   ** uses std::vector as container.
   ** The pointer to the registered array is passed as return value
   **/
  template <class Digi>
    std::vector<Digi>* RegisterOutput(std::string branchName) 
  {
    // --- Get FairRootManager instance
    FairRootManager* ioman = FairRootManager::Instance();
    assert ( ioman );

    // --- Branch for digis
    std::vector<Digi>* fDigis = new std::vector<Digi>();
    ioman->RegisterAny(branchName.c_str(), fDigis,
		       fPersistence);
    return fDigis;
  }

  
 protected:

  Bool_t  fPersistence;

  ClassDef(CbmROCUnpack, 0)
};

#endif
