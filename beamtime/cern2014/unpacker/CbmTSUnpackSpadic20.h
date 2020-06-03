// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                           CbmTSUnpackSpadic                       -----
// -----                    Created 07.11.2014 by F. Uhlig                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CBMTSUNPACKSPADIC20_H
#define CBMTSUNPACKSPADIC20_H

#include "Timeslice.hpp"
#include "Message.hpp"


#include "CbmTSUnpack.h"
#include "CbmBeamDefaults.h"
#include <map>

#include "TClonesArray.h"

#include "CbmHistManager.h"


#include <vector>
#include <array>

class CbmTSUnpackSpadic20 : public CbmTSUnpack
{
 protected:
  CbmHistManager* fHm;
  
 public:
  
  CbmTSUnpackSpadic20();
  virtual ~CbmTSUnpackSpadic20();
    
  virtual Bool_t Init();

  void CreateHistograms();
  virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
  void print_message(const spadic::Message& m);

  virtual void Reset();

  virtual void Finish();

  void SetParContainers() {;}

  // protected:
  //  virtual void Register();

  /**
   * A structure to store a hit message with some of its information.
   * Used for reconstruction of multihits.
   * Multimessage hits (b_multiFlag = true) are flagged with StopType 10.
   */
  struct hit_object{
    //attributes
    Int_t triggerType;                 /**< TriggerType of the hit.*/
    Int_t stopType;                    /**< StopType of the hit.*/
    Int_t nrSamples;                   /**< Number of samples of the hit.*/
    Bool_t b_complete;                 /**< = true, if the hit was corrected and is complete for Digi creation.*/
    Bool_t b_multiFlag;                /**< = true, if the hit is a corrected multimessage. (For QA)*/
    Int_t time;                        /**< Timestamp of the hit.*/
    ULong_t fullTime;                  /**< Fulltime of the hit.*/
    std::array<Short_t,32> samples;               /**< Samples of the hit.*/
    
  };

  /**
   * Handling of a hit identified as a successor of a multihit.
   * @param link Link of the successor hit. (E-Link of SPADIC)
   * @param addr Address of the successor hit. (Address of AFCK)
   * @param channel Channel of the successor hit. (Channel on SPADIC)
   * @param currHit Hit object of the successor hit containing its data.
   * @return Edited successor hit object. 
   * */
  hit_object correct_successor(Int_t link, Int_t addr, Int_t channel, hit_object currHit);

  /**
   * Handling of a hit identified as a predecessor of a multihit.
   * @param link Link of the predecessor hit. (E-Link of SPADIC)
   * @param addr Address of the predecessor hit. (Address of AFCK)
   * @param channel Channel of the predecessor hit. (Channel on SPADIC)
   * @param currHit Hit object of the predecessor hit containing its data.
   * @return Edited predecessor hit object. 
   * */
  hit_object correct_predecessor(Int_t link, Int_t addr, Int_t channel, hit_object currHit);
  

  virtual void FillOutput(boost::any){;}
 private:

  TClonesArray* fSpadicRaw;

  std::map<std::pair<int,int>,int> fEpochMarkerArray;
//Array to store the previous Epoch counter
  std::map<std::pair<int,int>,int>  fPreviousEpochMarkerArray;
//Suppress multiple Epoch Messages in duplicated Microslices. NOTE:Currently Buggy
  const Bool_t SuppressMultipliedEpochMessages = false;
  std::map<std::pair<int,int>,int>  fSuperEpochArray;

  Int_t fEpochMarker;
  Int_t fSuperEpoch;
  Int_t fNrExtraneousSamples;
 
  ULong_t n_hit = 0;
  ULong_t n_multihit_pre = 0;
  ULong_t n_multihit_succ = 0;
  ULong_t n_multimessage_pre = 0;
  ULong_t n_multimessage_succ = 0;
  ULong_t n_final_multimessage = 0;
  ULong_t n_final_hit = 0;
  ULong_t n_zeroSample = 0;
  ULong_t n_pre = 0;
  ULong_t n_succ = 0;
  ULong_t n_finalMultiMessage = 0;

  void GetEpochInfo(Int_t link, Int_t addr);
  void FillEpochInfo(Int_t link, Int_t addr, Int_t epoch_count);

  CbmTSUnpackSpadic20(const CbmTSUnpackSpadic20&);
  CbmTSUnpackSpadic20 operator=(const CbmTSUnpackSpadic20&);

  
  //Buffer of hit_objects, which are identified as predecessors of multihits. 
  std::map<std::pair<int,int>,std::map<uint16_t, hit_object> > multihit_buffer;
  
  ClassDef(CbmTSUnpackSpadic20, 2)
      
      
};

#endif

