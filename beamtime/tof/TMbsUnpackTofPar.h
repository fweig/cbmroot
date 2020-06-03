// -----------------------------------------------------------------------------
// ----- TMbsUnpackTofPar header file                                      -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-03-18                                 -----
// -----                                                                   -----
// ----- based on TMbsUnpackTofPar by P.-A. Loizeau                        -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/TMbsUnpackTofPar.h                              -----
// ----- revision 21006, 2013-08-08                                        -----
// -----------------------------------------------------------------------------

#ifndef _TMBSUNPACKTOFPAR_H_
#define _TMBSUNPACKTOFPAR_H_

#include <map>

#include "FairParGenericSet.h"
#include "TArrayI.h"

class TMbsUnpackTofPar : public FairParGenericSet 
{
   public:
      TMbsUnpackTofPar() ;

      TMbsUnpackTofPar( const char* name,
                      const char* title="Mbs Unpack parameters for the ToF",
                      const char* context="TestDefaultContext", 
                      Int_t iMode = 0 ); // <- no input as default !!!

      virtual ~TMbsUnpackTofPar();

      /** Type of MBS source: file, transport client, ... */
      void SetMode(Int_t iMode) { fiMbsSourceMode = iMode; };
      Int_t GetMode()  const { return fiMbsSourceMode; };

      Bool_t WriteDataInCbmOut() const { return (1 == fuCbmOutOn)? kTRUE: kFALSE; };
      
      Bool_t IsDebug() const { return (1 == fuVftxDebug)? kTRUE: kFALSE; };

      void SetBoardsNumber( UInt_t uNbBoards ){ fuNbVmeBoards = uNbBoards; };
      UInt_t GetBoardsNumber() { return fuNbVmeBoards; };
      Int_t GetActiveBoardsNumber();
      Int_t GetMappingField( UInt_t uBoard, UInt_t uField);
      ULong64_t GetBoardTag( UInt_t uBoard);
      Bool_t IsActive( UInt_t uBoard);
      Int_t GetBoardType( UInt_t uBoard);
      // Get the index among all boards of same type from global index
      UInt_t GetGlobalToTypeInd( UInt_t uBoardToFind );
      // Get the index among active boards of same type from global index
      UInt_t GetGlobalToActiveInd( UInt_t uBoardToFind );
      // Get the index of an active board among all boards of same type from global index
      UInt_t GetActiveToAllTypeInd( UInt_t uBoardToFind, UInt_t uType );

      void SetNotVmeBoardsNumber( UInt_t uNbBoards ){ fuNbNonVmeBoards = uNbBoards; };
      UInt_t GetNotVmeBoardsNumber() { return fuNbNonVmeBoards; };
      Int_t GetActiveNotVmeBoardsNumber();
      Int_t GetNotVmeMappingField( UInt_t uBoard, UInt_t uField);
      ULong64_t GetNotVmeBoardTag( UInt_t uBoard);
      Bool_t IsActiveNotVme( UInt_t uBoard);
      Int_t GetNotVmeBoardType( UInt_t uBoard);
      // Get the index among all boards of same type from global index, for non-VME boards
      UInt_t GetGlobToTypeIndNoVme( UInt_t uBoardToFind );
      // Get the index among active boards of same type from global index, for non-VME boards
      UInt_t GetGlobToActiveIndNoVme( UInt_t uBoardToFind );
      // Get the index of an active board among all boards of same type from global index, for non-VME boards
      UInt_t GetActivToAllTypeIndNoVme( UInt_t uBoardToFind, UInt_t uType );

      // Get Total number of identified active boards
      UInt_t GetNbActiveBoards();
      // Get number of active boards of asked type
      UInt_t GetNbActiveBoards( UInt_t uType );
      // Get Total number of identified Tdc boards
      UInt_t GetNbTdcs();
      // Get Total number of identified active Tdc boards
      UInt_t GetNbActiveTdcs();
      // Get Total number of identified Scaler boards
      UInt_t GetNbScalersB();
      // Get Total number of identified active Scalerboards
      UInt_t GetNbActiveScalersB();
      Bool_t WithActiveTriglog();

      UInt_t GetActiveTrbSebNb() { return fMapTrbSebAddrToTrbSebIndex.size(); };

      Bool_t IsTrbFpgaInData( UInt_t uTrbNetAddress ) { return fMapFpgaAddrToInDataFlag[ uTrbNetAddress ]; };
      UInt_t GetTrbSebAddrForFpga( UInt_t uTrbNetAddress );

      UInt_t GetFpgaNbPerTrbSeb( UInt_t uTrbNetAddress );
      UInt_t GetInDataFpgaNbPerTrbSeb( UInt_t uTrbNetAddress );
      UInt_t GetUnpackTdcNbPerTrbSeb( UInt_t uTrbNetAddress );

      Int_t  GetTrbSebIndex( UInt_t uTrbNetAddress );
      UInt_t GetTrbSebAddr( Int_t iTrbSebIndex );

      Int_t  GetActiveTrbTdcIndex( UInt_t uTrbNetAddress );
      UInt_t GetActiveTrbTdcAddr( Int_t iActiveTrbTdcIndex );

      Bool_t UnpackTrbTdcAddress( UInt_t uTrbNetAddress ) { return fActiveTrbTdcAddrToUnpackFlag[ uTrbNetAddress ]; };
      Bool_t UnpackTrbTdcIndex( Int_t iActiveTrbTdcIndex ) { return fActiveTrbTdcIndexToUnpackFlag[ iActiveTrbTdcIndex  ]; };

      Int_t  GetTriggerToReject() { return fiTriggerRejection; };
      Bool_t OnlyOneTriglog();

      Int_t GetCtsTrigMappingField( UInt_t uChannel, UInt_t uField);

      Bool_t IsTrbEventUnpacked(UInt_t uTriggerPattern);
      UInt_t GetFirstTrbTrigger(UChar_t cTriggerType, UInt_t uTriggerPattern);

      Bool_t IsTrbTdcJointEdgesChannel() const { return (1 == fiTrbTdcBitMode)? kTRUE: kFALSE; };

      // Example taken from CbmTofDigiPar
      void clear(void);
      void putParams(FairParamList*);
      Bool_t getParams(FairParamList*);
      virtual void printParams();
    
   private:

      /** Type of MBS source: file, transport client, ... */
      Int_t fiMbsSourceMode;

      /** CBMroot steering related parameters */
      UInt_t  fuCbmOutOn; // Switch ON/OFF the writing of data in the output root file of cbmroot analysis

      /** VFTX related parameters */
      UInt_t  fuVftxDebug; // Switch ON/OFF debug tests in VFTX unpacker

      // Mapping of all possible VME boards in our setup
      // { BoardIndex, Active, Address, AddMod, TokenA, TokenB, Type }
      UInt_t  fuNbVmeBoards;
      TArrayI fiMbsVmeMapping; 
      // Mapping of all possible Non-VME boards in our setup
      // { BoardIndex, Active, Address(ROC), AddMod, TokenA, TokenB, Type }
      UInt_t  fuNbNonVmeBoards;
      TArrayI fiNonVmeMapping;

         // Scaler boards. This numbers are not saved to file
         // as can be recovered from mapping
      UInt_t  fuNbScalerB;
      UInt_t  fuNbScalerActiveB;
      UInt_t  fuNbTriglog;
      UInt_t  fuNbTriglogActive;
      UInt_t  fuNbScalOrMu;
      UInt_t  fuNbScalOrMuActive;
         // TDC boards. This numbers are not saved to file
         // as can be recovered from mapping
      UInt_t  fuNbTdc;
      UInt_t  fuNbTdcActive;
      UInt_t  fuNbV1290;
      UInt_t  fuNbVftx;
      UInt_t  fuNbGet4;

      // TRB components
      UInt_t  fuTrbSebNb;
      UInt_t  fuTrbTdcNb;
      UInt_t  fuTrbHubNb;

      std::map<UInt_t,Bool_t> fMapFpgaAddrToInDataFlag;
      std::map<UInt_t,UInt_t> fMapFpgaAddrToTrbSebAddr;

      std::map<UInt_t,UInt_t> fMapTrbSebAddrToFpgaNb;
      std::map<UInt_t,UInt_t> fMapTrbSebAddrToFpgaInDataNb;
      std::map<UInt_t,UInt_t> fMapTrbSebAddrToUnpackTdcNb;

      std::map<UInt_t,Int_t>  fMapTrbSebAddrToTrbSebIndex;
      std::map<Int_t,UInt_t>  fMapTrbSebIndexToTrbSebAddr;

      std::map<UInt_t,Int_t>  fMapFpgaAddrToActiveTdcIndex;
      std::map<Int_t,UInt_t>  fMapActiveTdcIndexToFpgaAddr;

      std::map<UInt_t, Bool_t> fActiveTrbTdcAddrToUnpackFlag;
      std::map<Int_t, Bool_t>  fActiveTrbTdcIndexToUnpackFlag;

      // Trigger Rejection
      Int_t   fiTriggerRejection;

      // TRB-CTS trigger mapping
      UInt_t fuNbCtsTrigChs;
      TArrayI fiCtsTriggerMap;

      // TRB-TDC bit mode
      Int_t fiTrbTdcBitMode;

      TMbsUnpackTofPar(const TMbsUnpackTofPar&);
      TMbsUnpackTofPar& operator=(const TMbsUnpackTofPar&);
      void FillBoardsNbPerType();
      void FillActiveBoardsNbPerType();
      void FillTrbMaps();
    
  ClassDef(TMbsUnpackTofPar, 1)
};

#endif //TMBSUNPACKTOFPAR_H
