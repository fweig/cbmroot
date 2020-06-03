/**
 * CbmDeviceUnpackTofMcbm2018.cxx
 *
 * @since 2018-04-24
 * @author F. Uhlig
 */

#include "CbmDeviceUnpackTofMcbm2018.h"
#include "CbmMQDefs.h"

#include "CbmDigi.h"

#include "CbmMcbm2018UnpackerAlgoTof.h"
#include "CbmMcbm2018TofPar.h"
//#include "CbmHistManager.h"
#include "CbmTbDaqBuffer.h"
#include "CbmTofDigi.h"
#include "CbmTofAddress.h"
#include "CbmTofDetectorId_v14a.h" // in cbmdata/tof

#include "StorableTimeslice.hpp"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig
#include "FairRuntimeDb.h"
#include "FairParGenericSet.h"

#include "TH1.h"
#include "TH2.h"

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

// include this header to serialize vectors
#include <boost/serialization/vector.hpp>

#include <string>
#include <iomanip>
#include <array>

#include <stdexcept>
struct InitTaskError : std::runtime_error { using std::runtime_error::runtime_error; };

using namespace std;

//static Int_t iMess=0;
const  Int_t DetMask = 0x003FFFFF;

CbmDeviceUnpackTofMcbm2018::CbmDeviceUnpackTofMcbm2018()
  : fNumMessages(0)
  , fiSelectComponents(0)
  , fNumTint(0)
  , fEventHeader()
  , fiReqMode(0)
  , fiReqTint(0)
  , fiReqDigiAddr()
  , fiPulserMode(0)
  , fiPulMulMin(0)
  , fiPulTotMin(0)
  , fiPulTotMax(1000)
    //  , fAllowedChannels()
    //  , fChannelsToSend()
  //, fuMsAcceptsPercent(100)
  , fuTotalMsNb(0)
  , fuOverlapMsNb(0)
  , fuCoreMs(0)
   , fdMsSizeInNs(0)
  , fdTsCoreSizeInNs(0)
  , fuMinNbGdpb( 0 )
  //, fuCurrNbGdpb( 0 )
  , fuNrOfGdpbs(0)
  , fuNrOfFeePerGdpb(0)
  , fuNrOfGet4PerFee(0)
  , fuNrOfChannelsPerGet4(0)
  , fuNrOfChannelsPerFee(0)
  , fuNrOfGet4(0)
  , fuNrOfGet4PerGdpb(0)
  , fuNrOfChannelsPerGdpb(0)
  , fuGdpbId(0)
  , fuGdpbNr(0)
  , fuGet4Id(0)
  , fuGet4Nr(0)
  //, fiEquipmentId(0)
  , fMsgCounter(11,0) // length of enum MessageTypes initialized with 0
  , fGdpbIdIndexMap()
  , fvulGdpbTsMsb()
  , fvulGdpbTsLsb()
  , fvulStarTsMsb()
  , fvulStarTsMid()
  , fvulGdpbTsFullLast()
  , fvulStarTsFullLast()
  , fvuStarTokenLast()
  , fvuStarDaqCmdLast()
  , fvuStarTrigCmdLast()
  , fvulCurrentEpoch()
  , fvbFirstEpochSeen()
  , fNofEpochs(0)
  , fulCurrentEpochTime(0.)
  //, fdMsIndex(0.)
  , fdTShiftRef(0.)
  //, fuDiamondDpbIdx(3)
  //, fbEpochSuppModeOn( kTRUE )
  //, fbGet4M24b( kFALSE )
  //, fbGet4v20( kTRUE )
  //, fbMergedEpochsOn( kTRUE )
  , fDigi(nullptr)
  , fUnpackPar(nullptr)
  //, fdRefTime(0.)
  , fdLastDigiTime(0.)
  , fdFirstDigiTimeDif(0.)
  //, fdEvTime0(0.)
  , fhRawTDigEvT0( nullptr )
  , fhRawTDigRef0( nullptr )
  , fhRawTDigRef( nullptr )
  , fhRawTRefDig0( nullptr )
  , fhRawTRefDig1( nullptr )
  , fhRawDigiLastDigi( nullptr )
  , fhRawTotCh()
  , fhChCount()
  , fvbChanThere()
  , fhChanCoinc()
  , fhDetChanCoinc(nullptr)
  , fvuPadiToGet4()
  , fvuGet4ToPadi()
  , fvuElinkToGet4()
  , fvuGet4ToElink()
  , fviRpcType()
  , fviModuleId()
  , fviNrOfRpc()
  , fviRpcSide()
  , fviRpcChUId()
  , fvmEpSupprBuffer()
  , fBuffer(CbmTbDaqBuffer::Instance())
  , fulGdpbTsMsb(0.)
  , fulGdpbTsLsb(0.)
  , fulStarTsMsb(0.)
  , fulStarTsMid(0.)
  , fulGdpbTsFullLast(0.)
  , fulStarTsFullLast(0.)
  , fuStarTokenLast(0)
  , fuStarDaqCmdLast(0)
  , fuStarTrigCmdLast(0)
  , fUnpackerAlgo( nullptr )
{
   fUnpackerAlgo = new CbmMcbm2018UnpackerAlgoTof();
}

CbmDeviceUnpackTofMcbm2018::~CbmDeviceUnpackTofMcbm2018()
{
   delete fUnpackerAlgo;
}

void CbmDeviceUnpackTofMcbm2018::InitTask()
try
{
   // Get the information about created channels from the device
    // Check if the defined channels from the topology (by name)
    // are in the list of channels which are possible/allowed
    // for the device
    // The idea is to check at initilization if the devices are
    // properly connected. For the time beeing this is done with a
    // nameing convention. It is not avoided that someone sends other
    // data on this channel.
    //logger::SetLogLevel("INFO");

    int noChannel = fChannels.size();
    LOG(info) << "Number of defined channels: " << noChannel;
    for(auto const &entry : fChannels) {
      LOG(info) << "Channel name: " << entry.first;
      if (!IsChannelNameAllowed(entry.first)) throw InitTaskError("Channel name does not match.");
      if(entry.first == "syscmd") {
	OnData(entry.first, &CbmDeviceUnpackTofMcbm2018::HandleMessage);
	continue;
      }
      //if(entry.first != "tofdigis") OnData(entry.first, &CbmDeviceUnpackTofMcbm2018::HandleData);
      if(entry.first != "tofdigis") OnData(entry.first, &CbmDeviceUnpackTofMcbm2018::HandleParts);
      else {
	fChannelsToSend[0].push_back(entry.first);
	LOG(info) << "Init to send data to channel " << fChannelsToSend[0][0];
      }
    }
    InitContainers();
    const Int_t iHeaderSize=4;
    fEventHeader.resize(iHeaderSize); // define size of eventheader int[]
    for(int i=0; i<iHeaderSize; i++) fEventHeader[i]=0;
    fiSelectComponents = fConfig->GetValue<uint64_t>("SelectComponents");
    fiReqMode   = fConfig->GetValue<uint64_t>("ReqMode");
    fiReqTint   = fConfig->GetValue<uint64_t>("ReqTint");
    fiPulserMode= fConfig->GetValue<int64_t>("PulserMode");
    fiPulMulMin = fConfig->GetValue<uint64_t>("PulMulMin");
    fiPulTotMin = fConfig->GetValue<uint64_t>("PulTotMin");
    fiPulTotMax = fConfig->GetValue<uint64_t>("PulTotMax");
    fdTShiftRef = fConfig->GetValue<double_t>("TShiftRef");
//    Int_t iMaxAsicInactive = fConfig->GetValue<uint64_t>("MaxAsicInactive");
    //    fUnpackerAlgo->SetMaxAsicInactive( iMaxAsicInactive );
    Int_t iReqDet=1;
    Int_t iNReq=0;
    const Int_t iMaxReq=36;

    while(iNReq<iMaxReq) {   // FIXME, setup parameter hardwired!
      iReqDet = fConfig->GetValue<uint64_t>(Form("ReqDet%d",iNReq));
      if (iReqDet == 0) break;
      AddReqDigiAddr(iReqDet);
      iNReq++;
    }

    if(fiReqMode > 0)
    if(iNReq == 0) {  // take all defined detectors 
      for(UInt_t iGbtx= 0; iGbtx < fviNrOfRpc.size(); iGbtx++)  {
	 switch(fviRpcType[iGbtx]){  
	 case 0: // mTof modules
	 case 1: // eTof modules 
	   if(iGbtx%2 == 0) 
	     for(Int_t iRpc= 0; iRpc < fviNrOfRpc[iGbtx]; iRpc++)  {
	       Int_t iAddr = CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
							     iRpc,0,0,
							     fviRpcType[iGbtx]);
	       AddReqDigiAddr(iAddr);
	    }
	   break;
	   
	 case 9:  // HD 2-RPC boxes
	    for(Int_t iRpc= 0; iRpc < 2; iRpc++)  {
	      Int_t iAddr = CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
							    iRpc,0,0,
							    fviRpcType[iGbtx]);
	      AddReqDigiAddr(iAddr);
	    }
	    break;
	 case 6:  // Buc box
	   for(Int_t iRpc= 0; iRpc < 2; iRpc++)  {
	     Int_t iAddr = CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
							   iRpc,0,0,
							   fviRpcType[iGbtx]);
	     AddReqDigiAddr(iAddr);
	   }
	   break;
	   
	 case 7:  // CERN box
	   for(Int_t iRpc= 0; iRpc < 1; iRpc++)  {
	     Int_t iAddr = CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
							   iRpc,0,0,
							   7);
	     AddReqDigiAddr(iAddr);
	   }
	   break;
	 case 8:  // ceramics
	    for(Int_t iRpc= 0; iRpc < 8; iRpc++)  {
	      Int_t iAddr = CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
							    iRpc,0,0,
							    fviRpcType[iGbtx]);
	      AddReqDigiAddr(iAddr);
	    }
	    break;
	 case 5:  // add Diamond, single cell RPC
	    Int_t iAddr = CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
							  0,0,0,5);
	    AddReqDigiAddr(iAddr);
	    break;
	 }
       }        
    }

    LOG(info)<<"ReqMode "<<fiReqMode
	     <<" in " << fiReqTint << " ns "
	     <<" with "<<fiReqDigiAddr.size()
	     <<" detectors out of " << fviNrOfRpc.size() <<" GBTx, PulserMode "
	     << fiPulserMode << " with Mul " << fiPulMulMin<<", TotMin "<<fiPulTotMin;
    
} catch (InitTaskError& e) {
  LOG(error) << e.what();
  // Wrapper defined in CbmMQDefs.h to support different FairMQ versions
  cbm::mq::ChangeState(this, cbm::mq::Transition::ErrorFound);
}

bool CbmDeviceUnpackTofMcbm2018::IsChannelNameAllowed(std::string channelName)
{
  for(auto const &entry : fAllowedChannels) {
    LOG(info) << "Inspect " << entry;
    std::size_t pos1 = channelName.find(entry);
    if (pos1!=std::string::npos) {
      const vector<std::string>::const_iterator pos =
         std::find(fAllowedChannels.begin(), fAllowedChannels.end(), entry);
      const vector<std::string>::size_type idx = pos-fAllowedChannels.begin();
      LOG(info) << "Found " << entry << " in " << channelName;
      LOG(info) << "Channel name " << channelName
              << " found in list of allowed channel names at position " << idx;
      return true;
    }
  }
  LOG(info) << "Channel name " << channelName
            << " not found in list of allowed channel names.";
  LOG(error) << "Stop device.";
  return false;
}

Bool_t CbmDeviceUnpackTofMcbm2018::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmDeviceUnpackTofMcbm2018.";
//  FairRuntimeDb* fRtdb = FairRuntimeDb::instance();

  // NewSimpleMessage creates a copy of the data and takes care of its destruction (after the transfer takes place).
  // Should only be used for small data because of the cost of an additional copy
  std::string message{"CbmMcbm2018TofPar,111"};
  LOG(info) << "Requesting parameter container CbmMcbm2018TofPar, sending message: " << message;

  FairMQMessagePtr req(NewSimpleMessage("CbmMcbm2018TofPar,111"));
  FairMQMessagePtr rep(NewMessage());

  if (Send(req, "parameters") > 0)
    {
      if (Receive(rep, "parameters") >= 0)
        {
	  if (rep->GetSize() != 0)
            {
	      CbmMQTMessage tmsg(rep->GetData(), rep->GetSize());
	      fUnpackPar = dynamic_cast<CbmMcbm2018TofPar*>(tmsg.ReadObject(tmsg.GetClass()));
	      LOG(info) << "Received unpack parameter from parmq server: " << fUnpackPar;
	      fUnpackPar->Print();
            }
	  else
            {
	      LOG(error) << "Received empty reply. Parameter not available";
            }
        }
    }


   SetParContainers();

   Bool_t initOK = kTRUE;
   initOK &= fUnpackerAlgo->InitContainers();
   initOK &= ReInitContainers();  // needed for TInt parameters
   
   //  CreateHistograms();
   initOK &= fUnpackerAlgo->CreateHistograms();

  fvulCurrentEpoch.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
  fvbFirstEpochSeen.resize( fuNrOfGdpbs * fuNrOfGet4PerGdpb );
  fvbChanThere.resize( fviRpcChUId.size(), kFALSE );
  for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
    {
      for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
	{
	  fvulCurrentEpoch[GetArrayIndex(i, j)] = 0;
	  fvbFirstEpochSeen[GetArrayIndex(i, j)] = kFALSE;
	} // for( UInt_t j = 0; j < fuNrOfGet4PerGdpb; ++j )
    } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

  fNumTint = 0;
  return initOK;
}

void CbmDeviceUnpackTofMcbm2018::SetParContainers()
{
   FairRuntimeDb* fRtdb = FairRuntimeDb::instance();
  
   TList* fParCList = fUnpackerAlgo->GetParList();

   LOG(info) << "Setting parameter containers for " <<  fParCList->GetEntries() << " entries "; 

   for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
   {
      FairParGenericSet* tempObj = (FairParGenericSet*)(fParCList->At(iparC));
      fParCList->Remove(tempObj);

      std::string sParamName{ tempObj->GetName() };
   
      FairParGenericSet* newObj = dynamic_cast<FairParGenericSet*>( fRtdb->getContainer( sParamName.data() ) );
      LOG(info) << " - Get " << sParamName.data() << " at " << newObj; 
      if( nullptr == newObj )
      {

         LOG(error) << "Failed to obtain parameter container " << sParamName
                    << ", for parameter index " << iparC;
         return;
      } // if( nullptr == newObj )
      if(  iparC == 0 ) {
	newObj=(FairParGenericSet *) fUnpackPar;
	LOG(info) << " - Mod " << sParamName.data() << " to " << newObj; 
      }
     fParCList->AddAt(newObj, iparC);
//      delete tempObj;
   } // for( Int_t iparC = 0; iparC < fParCList->GetEntries(); ++iparC )
}

void CbmDeviceUnpackTofMcbm2018::AddMsComponentToList( size_t component, UShort_t usDetectorId )
{
   fUnpackerAlgo->AddMsComponentToList( component, usDetectorId );
}

Bool_t CbmDeviceUnpackTofMcbm2018::ReInitContainers()
{
  LOG(info) << "ReInit parameter containers for CbmDeviceUnpackMcbm2018TofPar.";

  fuNrOfGdpbs = fUnpackPar->GetNrOfGdpbs();
  LOG(info) << "Nr. of Tof GDPBs: " << fuNrOfGdpbs;
  fuMinNbGdpb = fuNrOfGdpbs;

  fuNrOfFeePerGdpb = fUnpackPar->GetNrOfFeesPerGdpb();
  LOG(info) << "Nr. of FEES per Tof GDPB: " << fuNrOfFeePerGdpb;

  fuNrOfGet4PerFee = fUnpackPar->GetNrOfGet4PerFee();
  LOG(info) << "Nr. of GET4 per Tof FEE: " << fuNrOfGet4PerFee;

  fuNrOfChannelsPerGet4 = fUnpackPar->GetNrOfChannelsPerGet4();
  LOG(info) << "Nr. of channels per GET4: " << fuNrOfChannelsPerGet4;

  fuNrOfChannelsPerFee = fuNrOfGet4PerFee * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per FEE: " << fuNrOfChannelsPerFee;

  fuNrOfGet4 = fuNrOfGdpbs * fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(info) << "Nr. of GET4s: " << fuNrOfGet4;

  fuNrOfGet4PerGdpb = fuNrOfFeePerGdpb * fuNrOfGet4PerFee;
  LOG(info) << "Nr. of GET4s per GDPB: " << fuNrOfGet4PerGdpb;

  fuNrOfChannelsPerGdpb = fuNrOfGet4PerGdpb * fuNrOfChannelsPerGet4;
  LOG(info) << "Nr. of channels per GDPB: " << fuNrOfChannelsPerGdpb;

  fGdpbIdIndexMap.clear();
   for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )
   {
      fGdpbIdIndexMap[fUnpackPar->GetGdpbId(i)] = i;
      LOG(info) << "GDPB Id of TOF  " << i << " : " << std::hex << fUnpackPar->GetGdpbId(i)
                ;
   } // for( UInt_t i = 0; i < fuNrOfGdpbs; ++i )

   fuTotalMsNb   = fUnpackPar->GetNbMsTot();
   fuOverlapMsNb = fUnpackPar->GetNbMsOverlap();
   fuCoreMs      = fuTotalMsNb - fuOverlapMsNb;
   fdMsSizeInNs  = fUnpackPar->GetSizeMsInNs();
   fdTsCoreSizeInNs = fdMsSizeInNs * fuCoreMs;
   LOG(info) << "Timeslice parameters: "
             << fuTotalMsNb << " MS per link, of which "
             << fuOverlapMsNb << " overlap MS, each MS is "
             << fdMsSizeInNs << " ns"
             ;

 /// STAR Trigger decoding and monitoring
   fvulGdpbTsMsb.resize(  fuNrOfGdpbs );
   fvulGdpbTsLsb.resize(  fuNrOfGdpbs );
   fvulStarTsMsb.resize(  fuNrOfGdpbs );
   fvulStarTsMid.resize(  fuNrOfGdpbs );
   fvulGdpbTsFullLast.resize(  fuNrOfGdpbs );
   fvulStarTsFullLast.resize(  fuNrOfGdpbs );
   fvuStarTokenLast.resize(  fuNrOfGdpbs );
   fvuStarDaqCmdLast.resize(  fuNrOfGdpbs );
   fvuStarTrigCmdLast.resize(  fuNrOfGdpbs );
   for (UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; ++uGdpb)
   {
      fvulGdpbTsMsb[ uGdpb ] = 0;
      fvulGdpbTsLsb[ uGdpb ] = 0;
      fvulStarTsMsb[ uGdpb ] = 0;
      fvulStarTsMid[ uGdpb ] = 0;
      fvulGdpbTsFullLast[ uGdpb ] = 0;
      fvulStarTsFullLast[ uGdpb ] = 0;
      fvuStarTokenLast[ uGdpb ]   = 0;
      fvuStarDaqCmdLast[ uGdpb ]  = 0;
      fvuStarTrigCmdLast[ uGdpb ] = 0;
   } // for (Int_t iGdpb = 0; iGdpb < fuNrOfGdpbs; ++iGdpb)

   fvmEpSupprBuffer.resize( fuNrOfGet4 );


/// TODO: move these constants somewhere shared, e.g the parameter file
   fvuPadiToGet4.resize( fuNrOfChannelsPerFee );
   fvuGet4ToPadi.resize( fuNrOfChannelsPerFee );
   /* source: Monitor
   UInt_t uGet4topadi[32] = {
       4,  3,  2,  1,  // provided by Jochen
      24, 23, 22, 21,
       8,  7,  6,  5,
      28, 27, 26, 25,
      12, 11, 10,  9,
      32, 31, 30, 29,
      16, 15, 14, 13,
      20, 19, 18, 17 };
   */

    UInt_t uGet4topadi[32] = {
       4,  3,  2,  1,  // provided by Jochen
       8,  7,  6,  5,
      12, 11, 10,  9,
      16, 15, 14, 13,
      20, 19, 18, 17,
      24, 23, 22, 21,
      28, 27, 26, 25,
      32, 31, 30, 29
    };

    UInt_t uPaditoget4[32] = {
       4,  3,  2,  1,  // provided by Jochen
      12, 11, 10,  9,
      20, 19, 18, 17,
      28, 27, 26, 25,
      32, 31, 30, 29,
       8,  7,  6,  5,
      16, 15, 14, 13,
      24, 23, 22, 21 };

    for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )
    {
      fvuPadiToGet4[ uChan ] = uPaditoget4[ uChan ] - 1;
      fvuGet4ToPadi[ uChan ] = uGet4topadi[ uChan ] - 1;
    } // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )

/// TODO: move these constants somewhere shared, e.g the parameter file
   fvuElinkToGet4.resize( kuNbGet4PerGbtx );
   fvuGet4ToElink.resize( kuNbGet4PerGbtx );
   UInt_t kuElinkToGet4[ kuNbGet4PerGbtx ] = { 27,  2,  7,  3, 31, 26, 30,  1,
                                               33, 37, 32, 13,  9, 14, 10, 15,
                                               17, 21, 16, 35, 34, 38, 25, 24,
                                                0,  6, 20, 23, 18, 22, 28,  4,
                                               29,  5, 19, 36, 39,  8, 12, 11
                                              };
   UInt_t kuGet4ToElink[ kuNbGet4PerGbtx ] = { 24,  7,  1,  3, 31, 33, 25,  2,
                                               37, 12, 14, 39, 38, 11, 13, 15,
                                               18, 16, 28, 34, 26, 17, 29, 27,
                                               23, 22,  5,  0, 30, 32,  6,  4,
                                               10,  8, 20, 19, 35,  9, 21, 36
                                              };
   for( UInt_t uLinkAsic = 0; uLinkAsic < kuNbGet4PerGbtx; ++uLinkAsic )
   {
      fvuElinkToGet4[ uLinkAsic ] = kuElinkToGet4[ uLinkAsic ];
      fvuGet4ToElink[ uLinkAsic ] = kuGet4ToElink[ uLinkAsic ];
   } // for( UInt_t uChan = 0; uChan < fuNrOfChannelsPerFee; ++uChan )

  UInt_t uNrOfGbtx  =  fUnpackPar->GetNrOfGbtx();
   fviRpcType.resize(uNrOfGbtx);
   fviModuleId.resize(uNrOfGbtx);
   fviNrOfRpc.resize(uNrOfGbtx);
   fviRpcSide.resize(uNrOfGbtx);

   for (UInt_t iGbtx = 0; iGbtx < uNrOfGbtx; ++iGbtx)
   {
     fviNrOfRpc[ iGbtx ]  = fUnpackPar->GetNrOfRpc( iGbtx );
     fviRpcType[ iGbtx ]  = fUnpackPar->GetRpcType( iGbtx );
     fviRpcSide[ iGbtx ]  = fUnpackPar->GetRpcSide( iGbtx );
     fviModuleId[ iGbtx ] = fUnpackPar->GetModuleId( iGbtx );
   }

   UInt_t uNrOfChannels = fuNrOfGet4 * fuNrOfChannelsPerGet4;
   LOG(info) << "Nr. of possible Tof channels: " << uNrOfChannels;

//   CbmTofDetectorId* fTofId = new CbmTofDetectorId_v14a();
   fviRpcChUId.resize(uNrOfChannels);
   UInt_t iCh= 0;
   for(UInt_t iGbtx= 0; iGbtx < uNrOfGbtx; iGbtx++)  {
     switch(fviRpcType[iGbtx]) {
       
     case 0:  // CBM modules
       if(fviRpcSide[iGbtx]<2){ // mTof modules
	 const Int_t RpcMap[5]={4,2,0,3,1}; 
	 for(Int_t iRpc= 0; iRpc < fviNrOfRpc[iGbtx]; iRpc++)  {
	   Int_t iStrMax=32;
	   Int_t iChNext=1;
       
	   for(Int_t iStr= 0; iStr < iStrMax; iStr++)  {
	     Int_t iStrMap = iStr;
	     Int_t iRpcMap = RpcMap[iRpc];

	     if( fviRpcSide[iGbtx] == 0) iStrMap=31-iStr;
	     if(fviModuleId[iGbtx] > -1) 
	       fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								iRpcMap,iStrMap,
								fviRpcSide[iGbtx],
								fviRpcType[iGbtx]);
	     else
	       fviRpcChUId[iCh]=0;
//	 LOG(debug)<<Form("Map Ch %d to Address 0x%08x",iCh,fviRpcChUId[iCh]);
	   
	     iCh += iChNext;
	   }
	 }
       }
       break;

       
     case 1:  // STAR eTOF  modules
       if(fviRpcSide[iGbtx]<2){    // mTof modules
	 const Int_t RpcMap[3]={0,1,2};
	 for(Int_t iRpc= 0; iRpc < fviNrOfRpc[iGbtx]; iRpc++)  {
	   Int_t iStrMax=32;
	   Int_t iChNext=1;

	   for(Int_t iStr= 0; iStr < iStrMax; iStr++)  {
	     Int_t iStrMap = iStr;
	     Int_t iRpcMap = RpcMap[iRpc];

	     if( fviRpcSide[iGbtx] == 0) iStrMap=31-iStr;
	     if(fviModuleId[iGbtx] > -1)
	       fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								iRpcMap,iStrMap,
								fviRpcSide[iGbtx],
								fviRpcType[iGbtx]);
	     else
	       fviRpcChUId[iCh]=0;
//	 LOG(DEBUG)<<Form("Map Ch %d to Address 0x%08x",iCh,fviRpcChUId[iCh]);
	     iCh += iChNext;
	   }
	 }
       }
       iCh += 64;
       break;
       
     case 5:  // Diamond 
       {
	 LOG(info) << " Map diamond  at GBTX  -  iCh = " << iCh;
         for( UInt_t uFee = 0; uFee < fUnpackPar->GetNrOfFeePerGbtx(); ++uFee )
	  {
	    for( UInt_t uCh = 0; uCh < fUnpackPar->GetNrOfChannelsPerFee(); ++uCh )
            {
	      if( uFee < 4 && ( 0 == uCh%4 || uCh<4 ))  {
		//  if(  0 == uCh )  {
                  fviRpcChUId[ iCh ] = CbmTofAddress::GetUniqueAddress(
                                             fviModuleId[iGbtx],
					     0, uFee*fUnpackPar->GetNrOfChannelsPerFee()/4 + uCh/4 + 40 * fviRpcSide[iGbtx],
					     //                           0, uFee + 10 * fviRpcSide[iGbtx],
                                             0, fviRpcType[iGbtx] );
		  LOG(info) << Form( "Map T0 Ch %d to Address 0x%08x", iCh, fviRpcChUId[iCh] );
	      }
	      else fviRpcChUId[ iCh ] = 0;

               iCh++;
            } // for( UInt_t uCh = 0; uCh < fUnpackPar->GetNrOfChannelsPerFee(); ++uCh )
         } // for( UInt_t uFee = 0; uFee < fUnpackPar->GetNrOfFeePerGbtx(); ++uFee )
       }
       break;

     case 78: // cern-20-gap + ceramic module  
	 {
	   LOG(info) << " Map CERN 20 gap  at GBTX  -  iCh = " << iCh;
	   const Int_t StrMap[32]={0,1,2,3,4,31,5,6,7,30,8,9,10,29,11,12,13,14,28,15,16,17,18,27,26,25,24,23,22,21,20,19};
	   Int_t iModuleId = 0;
	   Int_t iModuleType=7;
	   Int_t iRpcMap = 0;
	   for(Int_t iFeet= 0; iFeet <2; iFeet++){
	     for(Int_t iStr= 0; iStr < 32; iStr++)  {
	       Int_t iStrMap = 31-12-StrMap[iStr];
	       Int_t iSideMap = iFeet;

	       if (iStrMap < 20 )
		 fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(iModuleId,
								  iRpcMap,iStrMap,
								  iSideMap,
								  iModuleType);
	       else
		 fviRpcChUId[iCh]=0;
	       iCh++;
	     }
	   }
	 }
         // fall through is intended
     case 8: // ceramics 
	 {
	   Int_t iModuleId=0;
	   Int_t iModuleType=8;
	   for(Int_t iRpc= 0; iRpc < 8; iRpc++)  {
	     fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(iModuleId,
							      7-iRpc,0,0,
							      iModuleType);
	     iCh++;
	   }
	   iCh += (24 + 2*32);
	 }
	 LOG(info) << " Map end ceramics  box  at GBTX  -  iCh = " << iCh;
	 
	 break;
	 
     case 9: // Star2 boxes 
	 {
	   LOG(info) << " Map Star2 box  at GBTX  -  iCh = " << iCh;
	   const Int_t iRpc[5] ={1,-1,1,0,0};
	   const Int_t iSide[5]={1,-1,0,1,0};
	   for(Int_t iFeet= 0; iFeet <5; iFeet++){
	     for(Int_t iStr= 0; iStr < 32; iStr++)  {
	       Int_t iStrMap = iStr;
	       Int_t iRpcMap = iRpc[iFeet];
	       Int_t iSideMap = iSide[iFeet];
	       if(iSideMap == 0) iStrMap=31-iStr;
	       if( iSideMap > -1) 
		 fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								  iRpcMap,iStrMap,
								  iSideMap,
								  fviRpcType[iGbtx]);
	       else
		 fviRpcChUId[iCh]=0;
	       
	       iCh++;
	     }
	   }
	 }
	 break;

     case 6: // Buc box  
	 {
	   LOG(info) << " Map Buc box  at GBTX  -  iCh = " << iCh;
	   const Int_t iRpc[5] ={0,-1,0,1,1};
	   const Int_t iSide[5]={1,-1,0,1,0};
	   for(Int_t iFeet= 0; iFeet <5; iFeet++){
	     for(Int_t iStr= 0; iStr < 32; iStr++)  {
	       Int_t iStrMap = iStr;
	       Int_t iRpcMap = iRpc[iFeet];
	       Int_t iSideMap = iSide[iFeet];
	       //if(iSideMap == 0)iStrMap=31-iStr;
	       switch (fviRpcSide[iGbtx]) {
	       case 0: 
		 ;
		 break;
	       case  1:   // HD cosmic 2019, Buc2018, v18n
		 iStrMap=31-iStr;
		 iRpcMap=1-iRpcMap;
		 break;
	       case 2:  // v18m_cosmicHD
		 //		 iStrMap=31-iStr; 
		 iSideMap=1-iSideMap;
		 break;
	       case 3: 
		 iStrMap=31-iStr;
		 iRpcMap=1-iRpcMap;
		 iSideMap=1-iSideMap;
		 break;
	       case  4:   // HD cosmic 2019, Buc2018, v18o
		 iRpcMap=1-iRpcMap;
		 break;
	       default: 
		 ;
	       }
	       if( iSideMap > -1) 
		 fviRpcChUId[iCh]=CbmTofAddress::GetUniqueAddress(fviModuleId[iGbtx],
								  iRpcMap,iStrMap,
								  iSideMap,
								  fviRpcType[iGbtx]);
	       else
		 fviRpcChUId[iCh]=0;
	       
	       iCh++;
	     }
	   }
	 }
	 break;

     case -1:
       LOG(info) << " Found unused GBTX link at iCh = " << iCh;
	 iCh+=160;
         break; 

     default:
       LOG(error) << "Invalid Type  specifier for Gbtx " << iGbtx << ": "<< fviRpcType[iGbtx];
     }
   }

   for( UInt_t i = 0; i < uNrOfChannels; i=i+8)
   {
      if (i % 64 == 0)
	LOG(info)  << " Index "<< i;
      LOG(info) << Form("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x",
	fviRpcChUId[i],	fviRpcChUId[i+1], fviRpcChUId[i+2], fviRpcChUId[i+3], fviRpcChUId[i+4],fviRpcChUId[i+5],fviRpcChUId[i+6], fviRpcChUId[i+7]  );
   } // for( UInt_t i = 0; i < uNrOfChannels; ++i)

  return kTRUE;
}

void CbmDeviceUnpackTofMcbm2018::CreateHistograms()
{
  LOG(info) << "create Histos for " << fuNrOfGdpbs <<" gDPBs ";

  fhRawTDigEvT0 = new TH1F( Form("Raw_TDig-EvT0"),
			    Form("Raw digi time difference to 1st digi ; time [ns]; cts"),
			    500, 0, 100.);
  //   fHM->Add( Form("Raw_TDig-EvT0"), fhRawTDigEvT0);

  fhRawTDigRef0 = new TH1F( Form("Raw_TDig-Ref0"),
			    Form("Raw digi time difference to Ref ; time [ns]; cts"),
			    6000, -10000, 50000);
  //   fHM->Add( Form("Raw_TDig-Ref0"), fhRawTDigRef0);

  fhRawTDigRef = new TH1F( Form("Raw_TDig-Ref"),
			   Form("Raw digi time difference to Ref ; time [ns]; cts"),
			   6000, -1000, 5000);
  //   fHM->Add( Form("Raw_TDig-Ref"), fhRawTDigRef);

  fhRawTRefDig0 = new TH1F( Form("Raw_TRef-Dig0"),
			    Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
			    9999, -50000, 50000);
  //   fHM->Add( Form("Raw_TRef-Dig0"), fhRawTRefDig0);

  fhRawTRefDig1 = new TH1F( Form("Raw_TRef-Dig1"),
			    Form("Raw Ref time difference to last digi  ; time [ns]; cts"),
			    9999, -5000, 5000);
  //   fHM->Add( Form("Raw_TRef-Dig1"), fhRawTRefDig1);

  fhRawDigiLastDigi = new TH1F( Form("Raw_Digi-LastDigi"),
				Form("Raw Digi time difference to last digi  ; time [ns]; cts"),
				9999, -5000, 5000);
  //                                 9999, -5000000, 5000000);
  //   fHM->Add( Form("Raw_Digi-LastDigi"), fhRawDigiLastDigi);

  fhRawTotCh.resize( fuNrOfGdpbs );
  fhChCount.resize( fuNrOfGdpbs );
  fhChanCoinc.resize( fuNrOfGdpbs * fuNrOfFeePerGdpb / 2 );
  for( UInt_t uGdpb = 0; uGdpb < fuNrOfGdpbs; uGdpb ++)
    {
      fhRawTotCh[ uGdpb ] = new TH2F( Form("Raw_Tot_gDPB_%02u", uGdpb),
                                      Form("Raw TOT gDPB %02u; channel; TOT [bin]", uGdpb),
                                      fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb,
                                      256, 0., 256. );
      //      fHM->Add( Form("Raw_Tot_gDPB_%02u", uGdpb), fhRawTotCh[ uGdpb ]);

      fhChCount[ uGdpb ] = new TH1I( Form("ChCount_gDPB_%02u", uGdpb),
                                     Form("Channel counts gDPB %02u; channel; Hits", uGdpb),
                                     fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );
      //      fHM->Add( Form("ChCount_gDPB_%02u", uGdpb), fhChCount[ uGdpb ]);
      /*
	for( UInt_t uLeftFeb = uGdpb*fuNrOfFebsPerGdpb / 2;
	uLeftFeb < (uGdpb + 1 )*fuNrOfFebsPerGdpb / 2;
	++uLeftFeb )
	{
	fhChanCoinc[ uLeftFeb ] = new TH2F( Form("fhChanCoinc_%02u", uLeftFeb),
	Form("Channels Coincidence %02; Left; Right", uLeftFeb),
	fuNrOfChannelsPerFee, 0., fuNrOfChannelsPerFee,
	fuNrOfChannelsPerFee, 0., fuNrOfChannelsPerFee );
	} // for( UInt_t uLeftFeb = 0; uLeftFeb < fuNrOfFebsPerGdpb / 2; uLeftFeb ++ )
      */
      fhChanCoinc[ uGdpb ] = new TH2F( Form("fhChanCoinc_%02u", uGdpb),
				       Form("Channels Coincidence %02u; Left; Right", uGdpb),
				       fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb,
				       fuNrOfChannelsPerGdpb, 0., fuNrOfChannelsPerGdpb );
    } // for( UInt_t uGdpb = 0; uGdpb < fuMinNbGdpb; uGdpb ++)
  fhDetChanCoinc = new TH2F( "fhDetChanCoinc",
			     "Det Channels Coincidence; Left; Right",
			     32, 0., 32,
			     32, 0., 32 );
}

// handler is called whenever a message arrives on "data", with a reference to the message and a sub-channel index (here 0)
bool CbmDeviceUnpackTofMcbm2018::HandleData(FairMQMessagePtr& msg, int /*index*/)
{
// Don't do anything with the data
// Maybe add an message counter which counts the incomming messages and add
// an output
  fNumMessages++;
  LOG(debug) << "Received message number "<<  fNumMessages
	     << " with size " << msg->GetSize();

  std::string msgStr(static_cast<char*>(msg->GetData()), msg->GetSize());
  std::istringstream iss(msgStr);
  boost::archive::binary_iarchive inputArchive(iss);

  fles::StorableTimeslice component{0};
  inputArchive >> component;

  CheckTimeslice(component);

  DoUnpack(component, 0);

  BuildTint(0);

  if(fNumMessages%10000 == 0) LOG(info)<<"Processed "<<fNumMessages<<" time slices";

  return true;
}

bool CbmDeviceUnpackTofMcbm2018::HandleParts(FairMQParts& parts, int /*index*/)
{
// Don't do anything with the data
// Maybe add an message counter which counts the incomming messages and add
// an output
  fNumMessages++;
  LOG(debug) << "Received message number "<<  fNumMessages
	     << " with " << parts.Size() << " parts" ;

  fles::StorableTimeslice ts{0};

  switch(fiSelectComponents) {
  case 0: {
    std::string msgStr(static_cast<char*>(parts.At(0)->GetData()),(parts.At(0))->GetSize());
    std::istringstream iss(msgStr);
    boost::archive::binary_iarchive inputArchive(iss);
    inputArchive >> ts;
    CheckTimeslice(ts);
    if( 1 == fNumMessages ) {
      LOG(info) << "Initialize TS components list to " << ts.num_components();
      for (size_t c {0}; c < ts.num_components(); c++) {
	auto systemID = static_cast<int>(ts.descriptor(c, 0).sys_id);
	LOG(info) << "Found systemID: " << std::hex << systemID << std::dec;
	fUnpackerAlgo->AddMsComponentToList( c, systemID );  // TOF data
      }
    }
    DoUnpack(ts, 0);
    }
    break;
  case 1: {
    fles::StorableTimeslice component{0};

    uint ncomp=parts.Size();
    for (uint i=0; i<ncomp; i++) {
      std::string msgStr(static_cast<char*>(parts.At(i)->GetData()),(parts.At(i))->GetSize());
      std::istringstream iss(msgStr);
      boost::archive::binary_iarchive inputArchive(iss);
      //fles::StorableTimeslice component{i};
      inputArchive >> component;

      CheckTimeslice(component);
      fUnpackerAlgo->AddMsComponentToList( 0, 0x60 );  // TOF data
      LOG(debug) << "HandleParts message " << fNumMessages << " with indx " << component.index();
      DoUnpack(component, 0);
    }
    }
    break;
  default:
    ;
  }

  BuildTint(0);

  if(fNumMessages%10000 == 0) LOG(info)<<"Processed "<<fNumMessages<<" time slices";

  return true;
}

bool CbmDeviceUnpackTofMcbm2018::HandleMessage(FairMQMessagePtr& msg, int /*index*/)
{
  const char *cmd = (char *)(msg->GetData());
  const char cmda[4]={*cmd};
  LOG(info) << "Handle message " << cmd <<", " << cmd[0];
  cbm::mq::LogState(this);

  // only one implemented so far "Stop"

  if( strcmp(cmda,"STOP") ) {
    LOG(info) << "STOP";
    cbm::mq::ChangeState(this, cbm::mq::Transition::Ready);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::DeviceReady);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::Idle);
    cbm::mq::LogState(this);
    cbm::mq::ChangeState(this, cbm::mq::Transition::End);
    cbm::mq::LogState(this);
  }
  return true;
}

Bool_t CbmDeviceUnpackTofMcbm2018::DoUnpack(const fles::Timeslice& ts, size_t component)
{
   LOG(debug) << "Timeslice " << ts.index() << " contains " << ts.num_microslices(component)
              << " microslices of component " << component;
 
   if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )
   {
      LOG(error) << "Failed processing TS " << ts.index()
                 << " in unpacker algorithm class";
      return kTRUE;
   } // if( kFALSE == fUnpackerAlgo->ProcessTs( ts ) )

   /// Copy the digis in the DaqBuffer
   std::vector< CbmTofDigi > vDigi = fUnpackerAlgo->GetVector();

   /*
   // time sort vDigis
   sort(vDigi.begin(), vDigi.end(),
        [](const CbmTofDigi & a, const CbmTofDigi & b) -> bool
        {
          return a.GetTime() < b.GetTime();
        });
   */

   LOG(debug) << "Insert " << vDigi.size() << " digis into DAQ buffer  with size " << fBuffer->GetSize();

   for( auto digi: vDigi) {
     // copy Digi for insertion into DAQ buffer 
     fDigi = new CbmTofDigi(digi);

     LOG(debug) << "BufferInsert digi " << Form("0x%08x at %012.2f",fDigi->GetAddress(),fDigi->GetTime())
		<< Form(", first %012.2f, last %012.2f, size %u",fBuffer->GetTimeFirst(),fBuffer->GetTimeLast(),fBuffer->GetSize());

     fBuffer->InsertData<CbmTofDigi>(fDigi);
   }
   vDigi.clear();
   fUnpackerAlgo->ClearVector();

   return kTRUE;
}

static Int_t iErrorMess=0;
static Int_t iWarnMess=0;


void CbmDeviceUnpackTofMcbm2018::FillHitInfo( gdpbv100::Message mess )
{
   UInt_t uChannel = mess.getGdpbHitChanId();  // Get4 channel nr
   UInt_t uTot     = mess.getGdpbHit32Tot();
//   UInt_t uFts     = mess.getGdpbHitFineTs();

   ULong_t  ulCurEpochGdpbGet4 = fvulCurrentEpoch[ fuGet4Nr ];

   // In Ep. Suppr. Mode, receive following epoch instead of previous
   if( 0 < ulCurEpochGdpbGet4 )
      ulCurEpochGdpbGet4 --;
      else ulCurEpochGdpbGet4 = gdpbv100::kuEpochCounterSz; // Catch epoch cycle!

   UInt_t uChannelNr      = fuGet4Id * fuNrOfChannelsPerGet4 + uChannel;
   UInt_t uChannelNrInFee = (fuGet4Id % fuNrOfGet4PerFee) * fuNrOfChannelsPerGet4 + uChannel;
   UInt_t uFeeNr          = (fuGet4Id / fuNrOfGet4PerFee);
   UInt_t uFeeNrInSys     = fuGdpbNr * fuNrOfFeePerGdpb + uFeeNr;
   UInt_t uGbtxNr         = (uFeeNr / kuNbFeePerGbtx);
//   UInt_t uFeeInGbtx      = (uFeeNr % kuNbFeePerGbtx);
   UInt_t uGbtxNrInSys    = fuGdpbNr * kuNbGbtxPerGdpb + uGbtxNr;
   UInt_t uRemappedChannelNr = fuGdpbNr * fuNrOfChannelsPerGdpb 
                                                + uFeeNr * fuNrOfChannelsPerFee + fvuGet4ToPadi[ uChannelNrInFee ];
   /// Diamond FEE have straight connection from Get4 to eLink and from PADI to GET4
   if(  fviRpcType[uGbtxNrInSys]==5 ) {
      uRemappedChannelNr = fuGdpbNr * fuNrOfChannelsPerGdpb + uChannelNr;
   }
   /*
   UInt_t uRemappedChannelNr = fuGdpbNr * fuNrOfChannelsPerGdpb +
                                                       + ( fviRpcType[uGbtxNrInSys]==5 ? uChannelNr // Diamond
	                                                :  uFeeNr * fuNrOfChannelsPerFee +fvuGet4ToPadi[ uChannelNrInFee ] );
   */
   //     + ( fviRpcType[uGbtxNrInSys]==5 ? uChannelNrInFee : fvuGet4ToPadi[ uChannelNrInFee ] );
   //   UInt_t uRemappedChannelNr = uFeeNr * fuNrOfChannelsPerFee + uChannelNrInFee;
   /*
   if( fuGdpbNr==2)
      LOG(info)<<" Fill Hit GdpbNr" << fuGdpbNr
	       <<", ChNr "<<uChannelNr<<", CIF "<< uChannelNrInFee
	       <<", FNr "<<uFeeNr<<", FIS "<<uFeeNrInSys
	       <<", GbtxNr "<<uGbtxNr
	       <<", Remap "<<uRemappedChannelNr;
   */
//   ULong_t  ulHitTime = mess.getMsgFullTime(ulCurEpochGdpbGet4);
   Double_t dHitTime  = mess.getMsgFullTimeD(ulCurEpochGdpbGet4);

//   uFts = mess.getGdpbHitFullTs() % 112;

   if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
   {
      Double_t dHitTot   = uTot;     // in bins

      //if( fUnpackPar->GetNumberOfChannels() < uRemappedChannelNr )
      if( fviRpcChUId.size() < uRemappedChannelNr )
      {
	if(iErrorMess++ < 10000)
	{
	  LOG(error) << "Invalid mapping index " << uRemappedChannelNr
		     << " vs " << fviRpcChUId.size()
		     << ", from GdpbNr " << fuGdpbNr
		     << ", Get4 " << fuGet4Id
		     << ", Ch " << uChannel
		     << ", ChNr " << uChannelNr
		     << ", ChNrIF " << uChannelNrInFee
		     << ", FiS " << uFeeNrInSys
		     ;
	  return;
        } else
	  LOG(error) << "Max number of error messages reached "
		     ;

      } // if( fUnpackPar->GetNumberOfChannels() < uChanUId )

      fvbChanThere[ uRemappedChannelNr ] = kTRUE;

      // UInt_t uChanUId = fUnpackPar->GetChannelToDetUIdMap( uRemappedChannelNr );
      UInt_t uChanUId =  fviRpcChUId[ uRemappedChannelNr ];
      if( 0 == uChanUId ) {
	if(iWarnMess++ < 1000)
	{
	  LOG(warn) << "Invalid ChanUId for " << uRemappedChannelNr
		    << ", ChOff "  << fuGdpbNr * fuNrOfChannelsPerGdpb + uFeeNr * fuNrOfChannelsPerFee
		    << ", ChIF "   << ( fviRpcType[uGbtxNrInSys]==5 ? uChannelNrInFee : fvuGet4ToPadi[ uChannelNrInFee ] ) 
		    << ", GdpbNr " << fuGdpbNr
		    << ", GbtxNr " << uGbtxNrInSys
		    << ", Get4 "   << fuGet4Id
		    << ", Ch "     << uChannel
		    << ", ChNr "   << uChannelNr
		    << ", ChNrIF " << uChannelNrInFee
		    << ", FiS "    << uFeeNrInSys
                  ;
	} else {
	  if(iWarnMess==1000)
	     LOG(warn) << "No more messages. Fix your mapping problem!";
	  //FairMQStateMachine::ChangeState(PAUSE);
	}
	return;   // Hit not mapped to digi
      }
      if( (uChanUId & DetMask) == 0x00005006 )  dHitTime += fdTShiftRef;
      fdLastDigiTime = dHitTime;

      LOG(debug) << Form("Insert 0x%08x digi with time ", uChanUId ) << dHitTime << Form(", Tot %4.0f",dHitTot)
                 << " into buffer with " << fBuffer->GetSize() << " data from "
                 << Form("%11.1f to %11.1f ", fBuffer->GetTimeFirst(), fBuffer->GetTimeLast())
                 << " at epoch " << ulCurEpochGdpbGet4
                 ;

      fDigi = new CbmTofDigi(uChanUId, dHitTime, dHitTot);

      fBuffer->InsertData<CbmTofDigi>(fDigi);

      // Histograms filling
      // fhRawTotCh[ fuGdpbNr ]->Fill( uRemappedChannelNr, dHitTot);
      // fhChCount[ fuGdpbNr ] ->Fill( uRemappedChannelNr );
      // for debugging 
      if(0){
	if(fuGdpbNr == 2) {
	  LOG(info) << Form("Insert 0x%08x digi at %d with time ", uChanUId, uRemappedChannelNr)
		              << dHitTime << Form(",  Tot %4.0f",dHitTot);
	}
      }

   } // if( kTRUE == fvbFirstEpochSeen[ fuGet4Nr ] )
}


void CbmDeviceUnpackTofMcbm2018::FillEpochInfo( gdpbv100::Message mess )
{
   ULong64_t ulEpochNr = mess.getGdpbEpEpochNb();

   //LOG(debug) << "Get4Nr "<<fuGet4Nr<< " in epoch "<<ulEpochNr;

   fvulCurrentEpoch[ fuGet4Nr ] = ulEpochNr;

   if( kFALSE == fvbFirstEpochSeen[ fuGet4Nr ] )
      fvbFirstEpochSeen[ fuGet4Nr ] = kTRUE;

   fulCurrentEpochTime = mess.getMsgFullTime(ulEpochNr);
   fNofEpochs++;

   /// In Ep. Suppr. Mode, receive following epoch instead of previous
   /// Re-align the epoch number of the message in case it will be used later:
   /// We received the epoch after the data instead of the one before!
  if( 0 < ulEpochNr )
      mess.setGdpbEpEpochNb( ulEpochNr - 1 );
      else mess.setGdpbEpEpochNb( gdpbv100::kuEpochCounterSz );

   Int_t iBufferSize = fvmEpSupprBuffer[ fuGet4Nr ].size();
   if( 0 < iBufferSize )
   {
     LOG(debug) << "Now processing "<<iBufferSize<<" stored messages for get4 "
                << fuGet4Nr << " with epoch number "
                << (fvulCurrentEpoch[ fuGet4Nr ] - 1);
     const Int_t MaxBufferSize = 1000;  // FIXME: hardwired setup parameter
     if(iBufferSize < MaxBufferSize) {
       /// Data are sorted between epochs, not inside => Epoch level ordering
       /// Sorting at lower bin precision level
       std::stable_sort( fvmEpSupprBuffer[fuGet4Nr].begin(), fvmEpSupprBuffer[fuGet4Nr].begin() );

       for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
	 {
	   FillHitInfo( fvmEpSupprBuffer[ fuGet4Nr ][ iMsgIdx ] );
	 } // for( Int_t iMsgIdx = 0; iMsgIdx < iBufferSize; iMsgIdx++ )
     }
     fvmEpSupprBuffer[fuGet4Nr].clear();
   } // if( 0 < fvmEpSupprBuffer[fGet4Nr] )
}

void CbmDeviceUnpackTofMcbm2018::PrintSlcInfo(gdpbv100::Message /*mess*/)
{
   /// Nothing to do, maybe later use it to trakc parameter changes like treshold?
/*
  if( fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find( rocId ) )
     LOG(info) << "GET4 Slow Control message, epoch " << static_cast<Int_t>(fCurrentEpoch[rocId][get4Id])
                << ", time " << std::setprecision(9) << std::fixed
                << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                << " for board ID " << std::hex << std::setw(4) << rocId << std::dec
                << " +++++++ > Chip = " << std::setw(2) << mess.getGdpbGenChipId()
                << ", Chan = " << std::setw(1) << mess.getGdpbSlcChan()
                << ", Edge = " << std::setw(1) << mess.getGdpbSlcEdge()
                << ", Type = " << std::setw(1) << mess.getGdpbSlcType()
                << ", Data = " << std::hex << std::setw(6) << mess.getGdpbSlcData() << std::dec
                << ", Type = " << mess.getGdpbSlcCrc();
*/
}

void CbmDeviceUnpackTofMcbm2018::PrintGenInfo(gdpbv100::Message mess)
{
    Int_t mType = mess.getMessageType();
   Int_t channel = mess.getGdpbHitChanId();
   uint64_t uData = mess.getData();

   LOG(debug) << "Get4 MSG type " << mType << " from gdpbId " << fuGdpbId
              << ", getId " << fuGet4Id << ", (hit channel) " << channel
              << " data " << std::hex << uData
              ;
}

void CbmDeviceUnpackTofMcbm2018::PrintSysInfo(gdpbv100::Message mess)
{
   if (fGdpbIdIndexMap.end() != fGdpbIdIndexMap.find(fuGdpbId))
      LOG(debug) << "GET4 System message,       epoch "
                 << static_cast<Int_t>(fvulCurrentEpoch[fuGet4Nr]) << ", time " << std::setprecision(9)
                 << std::fixed << Double_t(fulCurrentEpochTime) * 1.e-9 << " s "
                 << " for board ID " << std::hex << std::setw(4) << fuGdpbId
                 << std::dec ;

   switch( mess.getGdpbSysSubType() )
   {
      case gdpbv100::SYS_GET4_ERROR:
      {
         uint32_t uData = mess.getGdpbSysErrData();
         if( gdpbv100::GET4_V2X_ERR_TOT_OVERWRT == uData
          || gdpbv100::GET4_V2X_ERR_TOT_RANGE   == uData
          || gdpbv100::GET4_V2X_ERR_EVT_DISCARD == uData
          || gdpbv100::GET4_V2X_ERR_ADD_RIS_EDG == uData
          || gdpbv100::GET4_V2X_ERR_UNPAIR_FALL == uData
          || gdpbv100::GET4_V2X_ERR_SEQUENCE_ER == uData
           )
            LOG(debug) << " +++++++ > gDPB: " << std::hex << std::setw(4) << fuGdpbId
                       << std::dec << ", Chip = " << std::setw(2)
                       << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                       << mess.getGdpbSysErrChanId() << ", Edge = "
                       << std::setw(1) << mess.getGdpbSysErrEdge() << ", Empt = "
                       << std::setw(1) << mess.getGdpbSysErrUnused()
                       << ", Data = " << std::hex << std::setw(2) << uData
                       << std::dec << " -- GET4 V1 Error Event"
                       ;
            else LOG(debug) << " +++++++ >gDPB: " << std::hex << std::setw(4) << fuGdpbId
                            << std::dec << ", Chip = " << std::setw(2)
                            << mess.getGdpbGenChipId() << ", Chan = " << std::setw(1)
                            << mess.getGdpbSysErrChanId() << ", Edge = "
                            << std::setw(1) << mess.getGdpbSysErrEdge() << ", Empt = "
                            << std::setw(1) << mess.getGdpbSysErrUnused()
                            << ", Data = " << std::hex << std::setw(2) << uData
                            << std::dec << " -- GET4 V1 Error Event "
                            ;
         break;
       } // case gdpbv100::SYSMSG_GET4_EVENT
       case gdpbv100::SYS_GDPB_UNKWN:
       {
         LOG(debug) << "Unknown GET4 message, data: " << std::hex << std::setw(8)
                    << mess.getGdpbSysUnkwData() << std::dec
                    <<" Full message: " << std::hex << std::setw(16)
                    << mess.getData() << std::dec
                    ;
         break;
      } // case gdpbv100::SYS_GDPB_UNKWN:
      case gdpbv100::SYS_PATTERN:
      {
         LOG(debug) << "ASIC pattern for missmatch, disable or resync";
         break;
      } // case gdpbv100::SYS_PATTERN:

   } // switch( getGdpbSysSubType() )
}

void CbmDeviceUnpackTofMcbm2018::FillStarTrigInfo(gdpbv100::Message mess)
{
  Int_t iMsgIndex = mess.getStarTrigMsgIndex();

  //mess.printDataCout();

  switch( iMsgIndex )
  {
      case 0:
         fulGdpbTsMsb = mess.getGdpbTsMsbStarA();
         break;
      case 1:
         fulGdpbTsLsb = mess.getGdpbTsLsbStarB();
         fulStarTsMsb = mess.getStarTsMsbStarB();
         break;
      case 2:
         fulStarTsMid = mess.getStarTsMidStarC();
         break;
      case 3:
      {
         ULong64_t ulNewGdpbTsFull = ( fulGdpbTsMsb << 24 )
                           + ( fulGdpbTsLsb       );
         ULong64_t ulNewStarTsFull = ( fulStarTsMsb << 48 )
                           + ( fulStarTsMid <<  8 )
                           + mess.getStarTsLsbStarD();
         UInt_t uNewToken  = mess.getStarTokenStarD();
         UInt_t uNewDaqCmd  = mess.getStarDaqCmdStarD();
         UInt_t uNewTrigCmd = mess.getStarTrigCmdStarD();
         if( ( uNewToken == fuStarTokenLast ) && ( ulNewGdpbTsFull == fulGdpbTsFullLast ) &&
             ( ulNewStarTsFull == fulStarTsFullLast ) && ( uNewDaqCmd == fuStarDaqCmdLast ) &&
             ( uNewTrigCmd == fuStarTrigCmdLast ) )

        {
            LOG(debug) << "Possible error: identical STAR tokens found twice in a row => ignore 2nd! "
                         << Form("token = %5u ", fuStarTokenLast )
                         << Form("gDPB ts  = %12llu ", fulGdpbTsFullLast )
                         << Form("STAR ts = %12llu ", fulStarTsFullLast )
                         << Form("DAQ cmd = %2u ", fuStarDaqCmdLast )
                         << Form("TRG cmd = %2u ", fuStarTrigCmdLast )
                         ;
            return;
         } // if exactly same message repeated
/*
         if( (uNewToken != fuStarTokenLast + 1) &&
             0 < fulGdpbTsFullLast && 0 < fulStarTsFullLast &&
             ( 4095 != fuStarTokenLast || 1 != uNewToken)  )
            LOG(warn) << "Possible error: STAR token did not increase by exactly 1! "
                         << Form("old = %5u vs new = %5u ", fuStarTokenLast,   uNewToken)
                         << Form("old = %12llu vs new = %12llu ", fulGdpbTsFullLast, ulNewGdpbTsFull)
                         << Form("old = %12llu vs new = %12llu ", fulStarTsFullLast, ulNewStarTsFull)
                         << Form("old = %2u vs new = %2u ", fuStarDaqCmdLast,  uNewDaqCmd)
                         << Form("old = %2u vs new = %2u ", fuStarTrigCmdLast, uNewTrigCmd);
*/
         fulGdpbTsFullLast = ulNewGdpbTsFull;
         fulStarTsFullLast = ulNewStarTsFull;
         fuStarTokenLast   = uNewToken;
         fuStarDaqCmdLast  = uNewDaqCmd;
         fuStarTrigCmdLast = uNewTrigCmd;

         Double_t dTot = 1.;
         Double_t dTime = fulGdpbTsFullLast * 6.25;
         if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )
         {
            fdFirstDigiTimeDif = dTime - fdLastDigiTime;
            LOG(info) << "Reference fake digi time shift initialized to " << fdFirstDigiTimeDif
		      << ", default: " << fdTShiftRef;
         } // if( 0. == fdFirstDigiTimeDif && 0. != fdLastDigiTime )

	 //         dTime -= fdFirstDigiTimeDif;
         // dTime += fdTShiftRef;

         LOG(debug) << "Insert fake digi with time " << dTime << ", Tot " << dTot;
         fhRawTRefDig0->Fill( dTime - fdLastDigiTime);
         fhRawTRefDig1->Fill( dTime - fdLastDigiTime);

         fDigi = new CbmTofDigi(0x00005006, dTime, dTot); // fake start counter signal
         fBuffer->InsertData<CbmTofDigi>(fDigi);
         break;
	  } // case 3
      default:
         LOG(error) << "Unknown Star Trigger messageindex: " << iMsgIndex;
  } // switch( iMsgIndex )
}

void CbmDeviceUnpackTofMcbm2018::PrintMicroSliceDescriptor(const fles::MicrosliceDescriptor& mdsc)
{
  LOG(info) << "Header ID: Ox" << std::hex << static_cast<int>(mdsc.hdr_id)
            << std::dec;
  LOG(info) << "Header version: Ox" << std::hex << static_cast<int>(mdsc.hdr_ver)
            << std::dec;
  LOG(info) << "Equipement ID: " << mdsc.eq_id;
  LOG(info) << "Flags: " << mdsc.flags;
  LOG(info) << "Sys ID: Ox" << std::hex << static_cast<int>(mdsc.sys_id)
            << std::dec;
  LOG(info) << "Sys version: Ox" << std::hex << static_cast<int>(mdsc.sys_ver)
            << std::dec;
  LOG(info) << "Microslice Idx: " << mdsc.idx;
  LOG(info) << "Checksum: " << mdsc.crc;
  LOG(info) << "Size: " << mdsc.size;
  LOG(info) << "Offset: " << mdsc.offset;
}

bool CbmDeviceUnpackTofMcbm2018::CheckTimeslice(const fles::Timeslice& ts)
{
  if ( 0 == ts.num_components() ) {
    LOG(error) << "No Component in TS " << ts.index();
    return 1;
  }
  auto tsIndex = ts.index();

  LOG(debug) << "Found " << ts.num_components()
            << " different components in timeslice "
	    << tsIndex;

/*
  for (size_t c = 0; c < ts.num_components(); ++c) {
    LOG(debug) << "Found " << ts.num_microslices(c)
              << " microslices in component " << c;
    LOG(debug) << "Component " << c << " has a size of "
              << ts.size_component(c) << " bytes";
    LOG(debug) << "Sys ID: Ox" << std::hex << static_cast<int>(ts.descriptor(0,0).sys_id)
            << std::dec;

    for (size_t m = 0; m < ts.num_microslices(c); ++m) {
      PrintMicroSliceDescriptor(ts.descriptor(c,m));
    }
  }
*/
  return true;
}

void CbmDeviceUnpackTofMcbm2018::BuildTint( int iMode=0 )
{
  // iMode - sending condition
  // 0 (default)- build time interval only if last buffer entry is older the start + TSLength
  // 1 (finish), empty buffer without checking
  // Steering variables
  double TSLENGTH    = 1.E6;
  double fdMaxDeltaT = (double) fiReqTint; // in ns

  LOG(debug) << " Buffer size " << fBuffer->GetSize()
	     << ", DeltaT " << (fBuffer->GetTimeLast()-fBuffer->GetTimeFirst())/1.E9 << " s";

  while (fBuffer->GetSize()>0) {
    Double_t fTimeBufferLast = fBuffer->GetTimeLast();

    switch(iMode){
    case 0:
      if( fTimeBufferLast - fBuffer->GetTimeFirst() < TSLENGTH ) return;
      break;
    case 1:
      ;
      break;
    }

    CbmTofDigi* digi = (CbmTofDigi*)fBuffer->GetNextData(fTimeBufferLast);
    Double_t dTEnd = digi->GetTime() + fdMaxDeltaT;   
    Double_t dTEndMax = digi->GetTime() + 2*fdMaxDeltaT;
    if(dTEnd > fTimeBufferLast)  {
      LOG(warn)<<Form("Remaining buffer < %f with %d entries is not sufficient for digi ending at %f -> skipped ",
		     fTimeBufferLast, fBuffer->GetSize(), dTEnd );
      return;
    }

    Bool_t bDet[fiReqDigiAddr.size()][2];
    for(UInt_t i=0; i<fiReqDigiAddr.size(); i++) for(Int_t j=0; j<2; j++) bDet[i][j]=kFALSE; //initialize
    Bool_t bPul[fiReqDigiAddr.size()][2];
    for(UInt_t i=0; i<fiReqDigiAddr.size(); i++) for(Int_t j=0; j<2; j++) bPul[i][j]=kFALSE; //initialize

    std::vector<CbmTofDigi*> vdigi;
    UInt_t nDigi=0;
    const Int_t AddrMask=0x003FFFFF;
    Bool_t bOut=kFALSE;

    while(digi) { // build digi array
      if (nDigi == vdigi.size()) vdigi.resize(nDigi+100);
      vdigi[nDigi++]=digi;
      for(UInt_t i=0; i<fiReqDigiAddr.size(); i++)
	if( (digi->GetAddress() & AddrMask) == fiReqDigiAddr[i]) {
	  Int_t j = ((CbmTofDigi *)digi)->GetSide();
	  bDet[i][j]=kTRUE;
	  if (  fiReqDigiAddr[i] == 0x00005006 ) bDet[i][1]=kTRUE;                             // diamond with pad readout
	  if ( (fiReqDigiAddr[i] & 0x0000F00F ) == 0x00008006) bDet[i][1]=kTRUE; // ceramic with pad readout
	  Int_t str = ((CbmTofDigi *)digi)->GetChannel();

	  switch(j){   // treat both strip ends separately
	  case 0:
	    switch(fiPulserMode) {
	    case 0:
	    case 1:
	      if (str==31)  if(digi->GetTot()>fiPulTotMin && digi->GetTot()<fiPulTotMax)  bPul[i][0]=kTRUE;
	      if (str==0)    bPul[i][1]=kFALSE;
	      if (  fiReqDigiAddr[i] == 0x00005006 ) { //special mapping for MAr2019 diamond (T0)
		if (str==0)    bPul[i][0]=kTRUE;
		if (str==40)  bPul[i][1]=kTRUE;
	      }
	      break;
	    case 2:
	      if (str==0)   if(digi->GetTot()>fiPulTotMin && digi->GetTot()<fiPulTotMax)  {
		bPul[i][0]=kTRUE;
		if ( (fiReqDigiAddr[i] & 0x000FF00F ) == 0x00078006) {
		  bPul[i][1]=kTRUE;    // ceramic with pad readout
		  bDet[i][1]=kFALSE;   // remove Hit flag
		}
		if (str==31) bPul[i][1]=kFALSE;
	      }
	    default:
	      ;
	    }
	    break;
	    
	  case 1:
	    switch(fiPulserMode) {
	    case 0:
	    case 1:
	      if (str==31)   bPul[i][0]=kFALSE;
	      if (str==0)    if(digi->GetTot()>fiPulTotMin && digi->GetTot()<fiPulTotMax)  bPul[i][1]=kTRUE;
	      break;
	    case 2:
	      if (str==0)    bPul[i][0]=kFALSE;
	      if (str==31)   if(digi->GetTot()>fiPulTotMin && digi->GetTot()<fiPulTotMax)  bPul[i][1]=kTRUE;
	      break;
	    default:
	      ;
	    }
	    break;
	  default:
	    ;
	  }
	}
      //if(bOut) LOG(info)<<Form("Found 0x%08x, Req 0x%08x ", digi->GetAddress(), fiReqDigiAddr);
      if(dTEnd - digi->GetTime() < fdMaxDeltaT*0.5) {
	if( digi->GetTime() + fdMaxDeltaT*0.5 < dTEndMax) dTEnd=digi->GetTime() + fdMaxDeltaT*0.5 ;
	else dTEnd=dTEndMax;
      };
      digi = (CbmTofDigi*)fBuffer->GetNextData(dTEnd);
    } // end while

    LOG(debug) << nDigi << " digis associated to dTEnd = " <<dTEnd<<":";
    //for(UInt_t iDigi=0; iDigi<nDigi; iDigi++) LOG(debug)<<Form(" 0x%08x",vdigi[iDigi]->GetAddress());
    for(UInt_t iDigi=0; iDigi<nDigi; iDigi++) LOG(debug)<<vdigi[iDigi]->ToString();

    UInt_t iDetMul=0;
    if(fiReqDigiAddr.size()==0) bOut=kTRUE;    // output everything
    else {
      if( fiReqMode == 0 ) {           // check for presence of requested detectors
	for(UInt_t i=0; i<fiReqDigiAddr.size(); i++)
	  if(bDet[i][0]==kFALSE || bDet[i][1]==kFALSE ) break;
	  else if( i == fiReqDigiAddr.size()-1 ) {
	    bOut=kTRUE;
	    iDetMul=i;
	  }
      } else {                        // check for presence of any known detector
	for(UInt_t i=0; i<fiReqDigiAddr.size(); i++)
	  if(bDet[i][0]==kTRUE && bDet[i][1]==kTRUE ) {
	    iDetMul++;
	  }
	if(iDetMul >= fiReqMode) {bOut=kTRUE;}
      }
    }

    if( bOut && fiReqDigiAddr.size() > 1) {
      LOG(debug) << "Found Req coinc in event with " <<nDigi << " digis in "<<iDetMul
		 <<" detectors, dTEnd = " <<dTEnd;
    }

    // determine Pulser status
    UInt_t iPulMul=0;  // Count Potential Pulser Signals 
    for(UInt_t i=0; i<fiReqDigiAddr.size(); i++) {
      if( bPul[i][0]==kTRUE  && bPul[i][1]==kTRUE ) iPulMul++;
    }

    if(fiPulserMode>0 && iPulMul > fiPulMulMin) { 
      LOG(debug)<<"@Event "<< fEventHeader[0] <<": iPulMul = " << iPulMul;
      bOut=kTRUE;
    }
    LOG(debug)<<"Process Ev " <<  fEventHeader[0] <<"  with iDetMul = "<<iDetMul << ", iPulMul = " << iPulMul;

    fEventHeader[0]++;

    if(bOut) {
      fEventHeader[1]=iDetMul;
      fEventHeader[2]=fiReqMode;
      fEventHeader[3]=iPulMul;
      vdigi.resize(nDigi);
      const Int_t NDigiMax=10000;
      if(nDigi > NDigiMax) {
	LOG(warn) << "Oversized event, truncated! ";
	for(UInt_t iDigi=NDigiMax; iDigi<nDigi; iDigi++) vdigi[iDigi]->Delete();
	nDigi=1; //NDigiMax; 
	vdigi.resize(nDigi);
      }
      SendDigis(vdigi,0);

      for(UInt_t iDigi=0; iDigi<nDigi; iDigi++) vdigi[iDigi]->Delete(); 
    }
    else {
      for(UInt_t iDigi=0; iDigi<nDigi; iDigi++){
	digi=vdigi[iDigi];
	digi->Delete();
      }
      //LOG(debug) << " Digis deleted ";
      //vdigi.clear();
      //delete &vdigi;  // crashes, since local variable, will be done at return (?)
    }
  }
}

bool CbmDeviceUnpackTofMcbm2018::SendDigis( std::vector<CbmTofDigi*> vdigi, int idx )
{
  LOG(debug) << "Send Digis for event "<<fNumTint<<" with size "<< vdigi.size()<< Form(" at %p ",&vdigi);
  LOG(debug) << "EventHeader: "<< fEventHeader[0] << " " << fEventHeader[1] << " " << fEventHeader[2] << " " << fEventHeader[3];

//  Int_t NDigi=vdigi.size();

  std::stringstream ossE;
  boost::archive::binary_oarchive oaE(ossE);
  oaE << fEventHeader;
  std::string* strMsgE = new std::string(ossE.str());

  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << vdigi;
  std::string* strMsg = new std::string(oss.str());

  FairMQParts parts;
  parts.AddPart(NewMessage(const_cast<char*>(strMsgE->c_str()), // data
                           strMsgE->length(), // size
                           [](void* , void* object){ delete static_cast<std::string*>(object); },
                           strMsgE)); // object that manages the data

  parts.AddPart(NewMessage(const_cast<char*>(strMsg->c_str()), // data
                           strMsg->length(), // size
                           [](void* , void* object){ delete static_cast<std::string*>(object); },
                           strMsg)); // object that manages the data


  /*
  std::vector<CbmTofDigi>vTofDigi;
  vTofDigi.resize(vdigi.size());
  for (Int_t i=0; i<vdigi.size(); i++)   {
    CbmTofDigi *pdigi = (CbmTofDigi *) vdigi[i];
    CbmTofDigi digi = *pdigi;
    vTofDigi[i] = digi;
    LOG(debug) << vTofDigi[i].ToString()<<" bits "<<Form("0x%08x",vTofDigi[i].TestBits(0xFFFF));
  }
    FairMQMessagePtr msg(NewMessage(static_cast<std::vector<CbmTofDigi>*> (&vTofDigi), // data
                                  NDigi*sizeof(CbmTofDigi), // size
				  [](void* , void* object){ delete static_cast<CbmTofDigi*>(object); }
                                  )); // object that manages the data

  // transfer of TofDigi array, ... works
  CbmTofDigi aTofDigi[NDigi];
  //  const Int_t iNDigiOut=100;
  //  NDigi=TMath::Min(NDigi,iNDigiOut);
  //  std::array<CbmTofDigi,iNDigiOut> aTofDigi;
  for (Int_t i=0; i<NDigi; i++) {
    aTofDigi[i] = *vdigi[i];
    LOG(debug) << aTofDigi[i].ToString()<<" bits "<<Form("0x%08x",aTofDigi[i].TestBits(0xFFFF));
  }
  FairMQMessagePtr msg(NewMessage(static_cast<CbmTofDigi*> (&aTofDigi[0]), // data
                                  NDigi*sizeof(CbmTofDigi), // size
				  [](void* , void* object){ delete static_cast<CbmTofDigi*>(object); }
                                  )); // object that manages the data


  LOG(info) << "Send aTofDigi sizes "<<NDigi<<", "<<sizeof(CbmTofDigi)<<", msg size "<<msg->GetSize();

  // serialize the timeslice and create the message

  std::stringstream oss;
  boost::archive::binary_oarchive oa(oss);
  oa << vdigi;
  std::string* strMsg = new std::string(oss.str());

  LOG(debug) << "send strMsg with length " << strMsg->length()<<" "<<strMsg;
  FairMQMessagePtr msg(NewMessage(const_cast<char*>(strMsg->c_str()), // data
                                                    strMsg->length(), // size
                                                    [](void* , void* object){ delete static_cast<std::string*>(object); },
                                                    strMsg)); // object that manages the data
  */
  /*
  FairMQMessagePtr msg(NewMessage(static_cast<CbmTofDigi*> (vTofDigi.data()), // data
                                                vTofDigi.size()*sizeof(CbmTofDigi), // size
                                                [](void* , void* object){ delete static_cast<CbmTofDigi*>(object); }
                                                )); // object that manages the data
  */

  /* --------------------------------------- compiles but crashes .... ---------------------------------------------------
  const Int_t WSize=8;
  FairMQMessagePtr msg(NewMessage(static_cast<std::vector<CbmTofDigi>*> (&vTofDigi), // data
                                  vTofDigi.size()*sizeof(CbmTofDigi)*WSize, // size, FIXME, numerical value in code!
				  [](void* , void* object){ delete static_cast<std::vector<CbmTofDigi>*>(object); }
                                  )); // object that manages the data

  LOG(info) << "Send TofDigi sizes "<<vTofDigi.size()<<", "<<sizeof(CbmTofDigi)<<", msg size "<<msg->GetSize();
  int *pData = static_cast <int *>(vTofDigi.data());

  int *pData = static_cast <int *>(msg->GetData());
  const Int_t NBytes=4;
  for (int iData=0; iData<msg->GetSize()/NBytes; iData++) {
    LOG(info) << Form(" ind %d, poi %p, data: 0x%08x",iData,pData,*pData++);
  }
  */
  /*
    auto msg = NewMessageFor("my_channel", 0,
                         static_cast<void*>(vTofDigi.data()),
                         vTofDigi.size() * sizeof(CbmTofDigi),
			 FairMQNoCleanup, nullptr);
  */

  // TODO: Implement sending same data to more than one channel
  // Need to create new message (copy message??)
  /*
  if (fChannelsToSend[idx]>1) {
    LOG(info) << "Need to copy FairMessage ?";
  }
  */
  // in case of error or transfer interruption,
  // return false to go to IDLE state
  // successfull transfer will return number of bytes
  // transfered (can be 0 if sending an empty message).

  LOG(debug) << "Send data to channel "<< idx << " " << fChannelsToSend[idx][0];


  //  if (Send(msg, fChannelsToSend[idx][0]) < 0) {
  if (Send(parts, fChannelsToSend[idx][0]) < 0) {
    LOG(error) << "Problem sending data " << fChannelsToSend[idx][0];
    return false;
  }
  /*
  LOG(info) << "Sent message # "<<  fNumTint << " at " << msg->GetData()
            << ", size " << msg->GetSize()<<" for "<<vTofDigi.size()<<" Digis at "
	    << vTofDigi.data() << ", "<<&vTofDigi;
  */
  fNumTint++;
  //if(fNumTint==100) FairMQStateMachine::ChangeState(PAUSE); //sleep(10000); // Stop for debugging ...
  /*
  LOG(info) << "Send message " << fNumTint << " with a size of "
            << msg->GetSize();
  */
  return true;
}

void CbmDeviceUnpackTofMcbm2018::AddReqDigiAddr(Int_t iAddr)
{
  UInt_t iNReq=fiReqDigiAddr.size();
  for(UInt_t i=0; i<iNReq; i++) if (  fiReqDigiAddr[i] == iAddr ) return;  // det already present, avoid double counting         
  fiReqDigiAddr.resize(iNReq+1); // hopefully the old entries are preserved ...
  fiReqDigiAddr[iNReq]=iAddr;
  LOG(info) <<Form("Request Digi Address 0x%08x ",iAddr);
}

void CbmDeviceUnpackTofMcbm2018::SetIgnoreOverlapMs( Bool_t bFlagIn )
{
   fUnpackerAlgo->SetIgnoreOverlapMs( bFlagIn );
}

void CbmDeviceUnpackTofMcbm2018::SetTimeOffsetNs( Double_t dOffsetIn )
{
   fUnpackerAlgo->SetTimeOffsetNs( dOffsetIn );
}
void CbmDeviceUnpackTofMcbm2018::SetDiamondDpbIdx( UInt_t uIdx )
{
   fUnpackerAlgo->SetDiamondDpbIdx( uIdx );
}
