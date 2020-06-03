/**
 * CbmDeviceEventBuilderEtofStar2019.h
 *
 */

#ifndef CBMDEVICEEVENTBUILDERETOFSTAR2019_H_
#define CBMDEVICEEVENTBUILDERETOFSTAR2019_H_

#include "FairMQDevice.h"

#include "Timeslice.hpp"
#include "TMessage.h"

#include "TStopwatch.h"

class CbmStar2019EventBuilderEtofAlgo;
class CbmStar2019TofPar;

class CbmDeviceEventBuilderEtofStar2019: public FairMQDevice
{
  public:
      CbmDeviceEventBuilderEtofStar2019();
      virtual ~CbmDeviceEventBuilderEtofStar2019();

      virtual Bool_t DoUnpack(const fles::Timeslice& ts, size_t component);
      virtual void Reset();
      virtual void Finish();

      void SetParContainers();

      Bool_t InitContainers();

      Bool_t ReInitContainers();

      void SetSandboxMode( Bool_t bSandboxMode = kTRUE ){ fbSandboxMode = bSandboxMode; }
      void SetEventDumpEnable( Bool_t bDumpEna = kTRUE );

      /// Temp until we change from CbmMcbmUnpack to something else
      void AddMsComponentToList( size_t component, UShort_t usDetectorId );
      void SetNbMsInTs( size_t /*uCoreMsNb*/, size_t /*uOverlapMsNb*/ ){};

      CbmDeviceEventBuilderEtofStar2019(const CbmDeviceEventBuilderEtofStar2019&) = delete;
      CbmDeviceEventBuilderEtofStar2019 operator=(const CbmDeviceEventBuilderEtofStar2019&) = delete;

 protected:
      virtual void InitTask();
      bool HandleData(FairMQMessagePtr&, int);
      bool HandleParts(FairMQParts&, int);
      bool HandleMessage(FairMQMessagePtr&, int);
      virtual bool SendEvent(std::vector<Int_t>, int);
      virtual bool SendSubevent(uint,char*,int, int); 

 private:
      uint64_t fNumMessages;
      /// Control flags
      Bool_t fbMonitorMode;  //! Switch ON the filling of a minimal set of histograms
      Bool_t fbDebugMonitorMode; //! Switch ON the filling of a additional set of histograms
      Bool_t fbSandboxMode;  //! Switch OFF the emission of data toward the STAR DAQ
      Bool_t fbEventDumpEna; //! Switch ON the dumping of the events to a binary file

      /// Parameters management
      TList* fParCList;

      /// Statistics & first TS rejection
      uint64_t fulTsCounter;
      uint64_t fNumEvt;

      bool CheckTimeslice(const fles::Timeslice& ts);
      bool IsChannelNameAllowed(std::string channelName);

      std::vector<std::string> fAllowedChannels
	= {"tofcomponent","parameters","etofevts","syscmd"};
      std::vector<std::vector<std::string>> fChannelsToSend = { {},{},{} };

      /// Processing algo
      CbmStar2019EventBuilderEtofAlgo * fEventBuilderAlgo;
      TStopwatch fTimer;

      CbmStar2019TofPar* fUnpackPar;      //!

      /// Event dump to binary file
      std::fstream * fpBinDumpFile;
      const UInt_t   kuBinDumpBegWord = 0xFEEDBEAF;
      const UInt_t   kuBinDumpEndWord = 0xFAEBDEEF;
};

// special class to expose protected TMessage constructor
class CbmMQTMessage : public TMessage
{
  public:
    CbmMQTMessage(void* buf, Int_t len)
        : TMessage(buf, len)
    {
        ResetBit(kIsOwner);
    }
};

#endif /* CBMDEVICEEVENTBUILDERETOFSTAR2019_H_ */
