// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                              CbmTSUnpack                          -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACK_H
#define CBMTSUNPACK_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Bool_t, kTRUE
#include <stddef.h>      // for size_t
#include <TObject.h>     // for TObject
#include <boost/any.hpp>
#include <FairRootManager.h> // FairRootManager

namespace fles { class Timeslice; }

class CbmTSUnpack : public TObject
{
 public:
  
  CbmTSUnpack();
  virtual ~CbmTSUnpack();
    
  virtual Bool_t Init() = 0;

  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component) = 0;

  virtual void Reset() = 0;

  virtual void Finish() = 0;

  virtual void SetParContainers() = 0;

  virtual Bool_t InitContainers() {return kTRUE;}

  virtual Bool_t ReInitContainers() {return kTRUE;}

  virtual void FillOutput(boost::any) = 0;

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
		       kTRUE);
    return fDigis;
//		       Isoutputbranchpersistent(branchName));

  }
  // --------------------------------------------------------------------------

  
  ClassDef(CbmTSUnpack, 0)
};

#endif
