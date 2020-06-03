// -----------------------------------------------------------------------------
// ----- TMbsUnpackTofPar source file                                      -----
// -----                                                                   -----
// ----- created by C. Simon on 2014-03-18                                 -----
// -----                                                                   -----
// ----- based on TMbsUnpackTofPar by P.-A. Loizeau                        -----
// ----- https://subversion.gsi.de/fairroot/cbmroot/development/ploizeau/  -----
// -----   main/unpack/tof/TMbsUnpackTofPar.cxx                            -----
// ----- revision 21645, 2013-09-06                                        -----
// -----------------------------------------------------------------------------

#include "TMbsUnpackTofPar.h"

// TOF headers
#include "TofDef.h"

// FAIR headers
#include "FairLogger.h"
#include "FairParamList.h" 

// ROOT headers

ClassImp(TMbsUnpackTofPar)

TMbsUnpackTofPar::TMbsUnpackTofPar() :
    FairParGenericSet("TMbsUnpackTofPar","Mbs Unpack parameters for the ToF","TestDefaultContext"),
    fiMbsSourceMode(0),
    fuCbmOutOn(0),
    fuVftxDebug(0),
    fuNbVmeBoards(0),
    fiMbsVmeMapping(),
    fuNbNonVmeBoards(0),
    fiNonVmeMapping(),
    fuNbScalerB(0),
    fuNbScalerActiveB(0),
    fuNbTriglog(0),
    fuNbTriglogActive(0),
    fuNbScalOrMu(0),
    fuNbScalOrMuActive(0),
    fuNbTdc(0),
    fuNbTdcActive(0),
    fuNbV1290(0),
    fuNbVftx(0),
    fuNbGet4(0),
    fuTrbSebNb(0),
    fuTrbTdcNb(0),
    fuTrbHubNb(0),
    fMapFpgaAddrToInDataFlag(),
    fMapFpgaAddrToTrbSebAddr(),
    fMapTrbSebAddrToFpgaNb(),
    fMapTrbSebAddrToFpgaInDataNb(),
    fMapTrbSebAddrToUnpackTdcNb(),
    fMapTrbSebAddrToTrbSebIndex(),
    fMapTrbSebIndexToTrbSebAddr(),
    fMapFpgaAddrToActiveTdcIndex(),
    fMapActiveTdcIndexToFpgaAddr(),
    fActiveTrbTdcAddrToUnpackFlag(),
    fActiveTrbTdcIndexToUnpackFlag(),
    fiTriggerRejection(0),
    fuNbCtsTrigChs(0),
    fiCtsTriggerMap(),
    fiTrbTdcBitMode(0)
{
}

TMbsUnpackTofPar::TMbsUnpackTofPar(const char* name,
             const char* title,
             const char* context,
             Int_t iMode ) :
    FairParGenericSet(name,title,context),
    fiMbsSourceMode(iMode),
    fuCbmOutOn(0),
    fuVftxDebug(0),
    fuNbVmeBoards(0),
    fiMbsVmeMapping(),
    fuNbNonVmeBoards(0),
    fiNonVmeMapping(),
    fuNbScalerB(0),
    fuNbScalerActiveB(0),
    fuNbTriglog(0),
    fuNbTriglogActive(0),
    fuNbScalOrMu(0),
    fuNbScalOrMuActive(0),
    fuNbTdc(0),
    fuNbTdcActive(0),
    fuNbV1290(0),
    fuNbVftx(0),
    fuNbGet4(0),
    fuTrbSebNb(0),
    fuTrbTdcNb(0),
    fuTrbHubNb(0),
    fMapFpgaAddrToInDataFlag(),
    fMapFpgaAddrToTrbSebAddr(),
    fMapTrbSebAddrToFpgaNb(),
    fMapTrbSebAddrToFpgaInDataNb(),
    fMapTrbSebAddrToUnpackTdcNb(),
    fMapTrbSebAddrToTrbSebIndex(),
    fMapTrbSebIndexToTrbSebAddr(),
    fMapFpgaAddrToActiveTdcIndex(),
    fMapActiveTdcIndexToFpgaAddr(),
    fActiveTrbTdcAddrToUnpackFlag(),
    fActiveTrbTdcIndexToUnpackFlag(),
    fiTriggerRejection(0),
    fuNbCtsTrigChs(0),
    fiCtsTriggerMap(),
    fiTrbTdcBitMode(0)
{
}


TMbsUnpackTofPar::~TMbsUnpackTofPar()
{
}

void TMbsUnpackTofPar::clear(void)
{
	  status = kFALSE;
	  resetInputVersions();

   fiMbsSourceMode    = 0;
   fuCbmOutOn         = 0;
   fuVftxDebug        = 0;
   fuNbVmeBoards      = 0;
   fuNbNonVmeBoards   = 0;
   fuNbScalerB        = 0;
   fuNbScalerActiveB  = 0;
   fuNbTriglog        = 0;
   fuNbTriglogActive  = 0;
   fuNbScalOrMu       = 0;
   fuNbScalOrMuActive = 0;
   fuNbTdc            = 0;
   fuNbTdcActive      = 0;
   fuNbV1290          = 0;
   fuNbVftx           = 0;
   fuNbGet4           = 0;
   fuTrbSebNb         = 0;
   fuTrbTdcNb         = 0;
   fuTrbHubNb         = 0;
   fuNbCtsTrigChs     = 0;
   fiTrbTdcBitMode    = 0;

   fMapFpgaAddrToInDataFlag.clear();
   fMapFpgaAddrToTrbSebAddr.clear();

   fMapTrbSebAddrToFpgaNb.clear();
   fMapTrbSebAddrToFpgaInDataNb.clear();
   fMapTrbSebAddrToUnpackTdcNb.clear();

   fMapTrbSebAddrToTrbSebIndex.clear();
   fMapTrbSebIndexToTrbSebAddr.clear();

   fMapFpgaAddrToActiveTdcIndex.clear();
   fMapActiveTdcIndexToFpgaAddr.clear();

}

void TMbsUnpackTofPar::putParams(FairParamList* l)
{
  if (!l) { return; }

  l->add("MbsSourceMode",     fiMbsSourceMode);
  l->add("WriteDataInCbmOut", (Int_t)fuCbmOutOn);
  l->add("VftxDebug",         (Int_t)fuVftxDebug);
  l->add("MbsNbVmeBoards",    (Int_t)fuNbVmeBoards);
  l->add("MbsVmeMapping",     fiMbsVmeMapping);
  l->add("NbNonVmeBoards",    (Int_t)fuNbNonVmeBoards);
  l->add("NonVmeMapping",     fiNonVmeMapping);
  l->add("TriggerRejection",  fiTriggerRejection);
  l->add("NbCtsTrigChs",      (Int_t)fuNbCtsTrigChs);
  l->add("CtsTriggerMap",     fiCtsTriggerMap);
  l->add("TrbTdcBitMode",     fiTrbTdcBitMode);
}

Bool_t TMbsUnpackTofPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }

  clear();

  LOG(debug2)<<"Get the MBS unpack parameters.";
  
  if ( ! l->fill("MbsSourceMode",  &fiMbsSourceMode) ) return kFALSE;
  
  Int_t iTempCaster = 0;
  if ( ! l->fill("WriteDataInCbmOut",      &iTempCaster) ) return kFALSE;
  fuCbmOutOn = (UInt_t)iTempCaster;

  iTempCaster = 0;
  if ( ! l->fill("VftxDebug",      &iTempCaster) ) return kFALSE;
  fuVftxDebug = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("MbsNbVmeBoards", &iTempCaster) ) return kFALSE;
  fuNbVmeBoards = (UInt_t)iTempCaster;

  if( 0 < fuNbVmeBoards )
  {  
    fiMbsVmeMapping.Set( fuNbVmeBoards * tofMbs::kuNbFieldsMapping );
    if ( ! l->fill("MbsVmeMapping",  &fiMbsVmeMapping) ) return kFALSE;
  }

  iTempCaster = 0;
  if ( ! l->fill("NbNonVmeBoards", &iTempCaster) ) return kFALSE;
  fuNbNonVmeBoards = (UInt_t)iTempCaster;

  if( 0 < fuNbNonVmeBoards )
  {
     fiNonVmeMapping.Set( fuNbNonVmeBoards * tofMbs::kuNbFieldsMapping );
     if ( ! l->fill("NonVmeMapping",  &fiNonVmeMapping) ) return kFALSE;
  } // if( 0 < fuNbNonVmeBoards )

  if ( ! l->fill("TriggerRejection",  &fiTriggerRejection) ) return kFALSE;

  iTempCaster = 0;
  if ( ! l->fill("NbCtsTrigChs", &iTempCaster) )
  {
    LOG(warn)<<"Field 'NbCtsTrigChs' not existing in the unpack parameter file!";
    LOG(warn)<<"Ignore this message if you analyze beamtime data prior to CERN February 2015.";
  }
  fuNbCtsTrigChs = (UInt_t)iTempCaster;

  if( 0 < fuNbCtsTrigChs )
  {
    fiCtsTriggerMap.Set( fuNbCtsTrigChs * tofMbs::kuNbFieldsCtsMapping );
    if ( ! l->fill("CtsTriggerMap", &fiCtsTriggerMap) )
    {
      LOG(warn)<<"Field 'CtsTriggerMap' not existing in the unpack parameter file!";
      LOG(warn)<<"Ignore this message if you analyze beamtime data prior to CERN February 2015.";
    }
  }

  if ( ! l->fill("TrbTdcBitMode",  &fiTrbTdcBitMode) )
  {
    LOG(warn)<<"Field 'TrbTdcBitMode' not existing in the unpack parameter file!";
    LOG(warn)<<"Ignore this message if you analyze beamtime data prior to CERN February 2015.";
  }

  FillBoardsNbPerType();
  FillActiveBoardsNbPerType();

  FillTrbMaps();

//  printParams();
  
  return kTRUE;
}

void TMbsUnpackTofPar::printParams()
{   
   LOG(info)<<"Parameter values in TMbsUnpackTofPar: ";
   LOG(info)<<"  Mbs Source Mode: \t\t"<<GetMode();
   if( kTRUE == WriteDataInCbmOut() )
      LOG(info)<<"  Data writen in cbmroot out:  \t\tON";
      else LOG(info)<<"  Data writen in cbmroot out:  \t\tOFF";
   if( kTRUE == IsDebug() )
      LOG(info)<<"  VFTX Unp debug:  \t\tON";
      else LOG(info)<<"  VFTX Unp debug:  \t\tOFF";
   LOG(info)<<"  Mbs Nb Boards:   \t\t"<<GetBoardsNumber();
   LOG(info)<<"  Mbs Vme Mapping: \t\t BoardIndex Active    Address AddMod     TokenA     TokenB Type";
   TString sTemp = "";
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++ )
   {
      sTemp = Form("                   \t\t %10d %6d 0x%08X %6x 0x%08X 0x%08X %4d",
                   GetMappingField( uBoard, tofMbs::BoardIndex),
                   GetMappingField( uBoard, tofMbs::Active),
                   GetMappingField( uBoard, tofMbs::Address),
                   GetMappingField( uBoard, tofMbs::AddMod),
                   GetMappingField( uBoard, tofMbs::TokenA),
                   GetMappingField( uBoard, tofMbs::TokenB),
                   GetMappingField( uBoard, tofMbs::Type) );
      LOG(info)<<sTemp;
   } // for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++ )

   LOG(info)<<"  Nb Non VME Boards:\t\t"<<GetNotVmeBoardsNumber();
   if( 0 < GetNotVmeBoardsNumber() )
   {
      LOG(info)<<"  Not-Vme Mapping: \t\t BoardIndex Active    RocIndx AddMod     TokenA     TokenB Type";
      sTemp = "";
      for( UInt_t uBoard = 0; uBoard < GetNotVmeBoardsNumber(); uBoard++ )
      {
         sTemp = Form("                   \t\t %10d %6d 0x%08X %6x 0x%08X 0x%08X %4d",
               GetNotVmeMappingField( uBoard, tofMbs::BoardIndex),
               GetNotVmeMappingField( uBoard, tofMbs::Active),
               GetNotVmeMappingField( uBoard, tofMbs::Address),
               GetNotVmeMappingField( uBoard, tofMbs::AddMod),
               GetNotVmeMappingField( uBoard, tofMbs::TokenA),
               GetNotVmeMappingField( uBoard, tofMbs::TokenB),
               GetNotVmeMappingField( uBoard, tofMbs::Type) );
         LOG(info)<<sTemp;
      } // for( UInt_t uBoard = 0; uBoard < GetNotVmeBoardsNumber(); uBoard++ )
   } // if( 0 < GetNotVmeBoardsNumber() )

   if( -1 < GetTriggerToReject() )
      LOG(info)<<"  Trigger to reject:\t\t"<<GetTriggerToReject();
      else LOG(info)<<"  Trigger rejection:\t\tOFF";

   LOG(info)<<"  Nb CTS trigger ch:\t\t"<<fuNbCtsTrigChs;
   if( 0 < fuNbCtsTrigChs )
   {
      LOG(info)<<"  CTS trigger map: \t\t TriggerChannel TriggerType TriggerReadout";
      sTemp = "";
      for( UInt_t uChannel = 0; uChannel < fuNbCtsTrigChs; uChannel++ )
      {
         sTemp = Form("                   \t\t %*d         0x%1X %*d",
               14, GetCtsTrigMappingField( uChannel, tofMbs::TriggerChannel),
               GetCtsTrigMappingField( uChannel, tofMbs::TriggerType),
               14, GetCtsTrigMappingField( uChannel, tofMbs::TriggerUnpack)
                 );
         LOG(info)<<sTemp;
      }
   }

   if( IsTrbTdcJointEdgesChannel() )
   {
      LOG(info)<<"  TRB-TDC 96-bit mode: joint edge detection in the same TDC channel";
   }
   else
   {
	  LOG(info)<<"  TRB-TDC 128-bit mode: separate edge detection in different TDC channels";
   }

   return;
}

//*************** VME boards ****************************************************/
Int_t TMbsUnpackTofPar::GetActiveBoardsNumber()
{
   UInt_t uNbActive = 0;
   // Count all active VME boards of all types
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++ )
      if( kTRUE == IsActive( uBoard ) )
         uNbActive++;
   return uNbActive;
}
Int_t TMbsUnpackTofPar::GetMappingField( UInt_t uBoard, UInt_t uField)
{
   LOG(debug3)<<"TMbsUnpackTofPar::GetMappingField: Trying to get combination ("
              <<uBoard<<"/"<<uField<<") => Index "<<uBoard*tofMbs::kuNbFieldsMapping + uField;
   if( static_cast<Int_t>(fuNbVmeBoards*tofMbs::kuNbFieldsMapping) == fiMbsVmeMapping.GetSize() )
   {
      if( uBoard < fuNbVmeBoards && uField < tofMbs::kuNbFieldsMapping )
         return fiMbsVmeMapping[ uBoard*tofMbs::kuNbFieldsMapping + uField ];
         else
         {
            LOG(warn)<<"TMbsUnpackTofPar::GetMappingField: wrong Board/field combination ("
                        <<uBoard<<"/"<<uField<<")";
            return -1;
         } // else of if( uBoard < fuNbVmeBoards && uField < tofMbs::kuNbFieldsMapping )
   } // if( fuNbVmeBoards*tofMbs::kuNbFieldsMapping == fiMbsVmeMapping.GetSize() )
      else
      {
         LOG(error)<<"TMbsUnpackTofPar::GetMappingField: wrong fiMbsVmeMapping size ("
                     <<fiMbsVmeMapping.GetSize()<<" instead of "
                     <<fuNbVmeBoards*tofMbs::kuNbFieldsMapping<<"), maybe not initialized!";
         return -1;
      } // else of if( fuNbVmeBoards*tofMbs::kuNbFieldsMapping == fiMbsVmeMapping.GetSize() )
}
ULong64_t TMbsUnpackTofPar::GetBoardTag( UInt_t uBoard)
{
   if( static_cast<Int_t>(fuNbVmeBoards*tofMbs::kuNbFieldsMapping) == fiMbsVmeMapping.GetSize() )
   {
      if( uBoard < fuNbVmeBoards )
      {
         ULong64_t uFullTag = ( ( (ULong64_t)GetMappingField( uBoard, tofMbs::TokenB ) )<< 32 ) |
                                  (ULong64_t)GetMappingField( uBoard, tofMbs::TokenA );
//         LOG(info)<<Form(" %016llX %08llX %08llX", uFullTag, (uFullTag&0xFFFFFFFF00000000)>>32, (uFullTag&0xFFFFFFFF) );
         return uFullTag;
      } // if( uBoard < fuNbVmeBoards )
         else
         {
            LOG(warn)<<"TMbsUnpackTofPar::GetBoardTag: wrong Board ("
                        <<uBoard<<")";
            return 0;
         } // else of if( uBoard < fuNbVmeBoards && uField < tofMbs::kuNbFieldsMapping )
   } // if( fuNbVmeBoards*tofMbs::kuNbFieldsMapping == fiMbsVmeMapping.GetSize() )
      else
      {
         LOG(error)<<"TMbsUnpackTofPar::GetBoardTag: wrong fiMbsVmeMapping size ("
                     <<fiMbsVmeMapping.GetSize()<<" instead of "
                     <<fuNbVmeBoards*tofMbs::kuNbFieldsMapping<<"), maybe not initialized!";
         return 0;
      } // else of if( fuNbVmeBoards*tofMbs::kuNbFieldsMapping == fiMbsVmeMapping.GetSize() )
}
Bool_t TMbsUnpackTofPar::IsActive( UInt_t uBoard)
{
   if( static_cast<Int_t>(fuNbVmeBoards*tofMbs::kuNbFieldsMapping) == fiMbsVmeMapping.GetSize() )
   {
      if( uBoard < fuNbVmeBoards )
         return (1 == GetMappingField( uBoard, tofMbs::Active ) )? kTRUE: kFALSE;
         else
         {
            LOG(warn)<<"TMbsUnpackTofPar::IsActive: wrong Board ("
                        <<uBoard<<")";
            return kFALSE;
         } // else of if( uBoard < fuNbVmeBoards && uField < tofMbs::kuNbFieldsMapping )
   } // if( fuNbVmeBoards*tofMbs::kuNbFieldsMapping == fiMbsVmeMapping.GetSize() )
      else
      {
         LOG(error)<<"TMbsUnpackTofPar::IsActive: wrong fiMbsVmeMapping size ("
                     <<fiMbsVmeMapping.GetSize()<<" instead of "
                     <<fuNbVmeBoards*tofMbs::kuNbFieldsMapping<<"), maybe not initialized!";
         return kFALSE;
      } // else of if( fuNbVmeBoards*tofMbs::kuNbFieldsMapping == fiMbsVmeMapping.GetSize() )
}
Int_t TMbsUnpackTofPar::GetBoardType( UInt_t uBoard)
{ 
   return GetMappingField( uBoard, tofMbs::Type); 
};
UInt_t TMbsUnpackTofPar::GetGlobalToTypeInd( UInt_t uBoardToFind )
{
   UInt_t uIndBoardSameType = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
   {
      // When find target return its index among boards
      // of same type
      if( uBoardToFind == uBoard )
         return uIndBoardSameType;
      // Count active boards of same type as target
      if( GetBoardType( uBoardToFind ) == GetBoardType( uBoard ) )
         uIndBoardSameType++;
   } // for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
   // If the board was not found:
   // return total number of boards as it will be rejected by other functions!
   return fuNbVmeBoards;
}
UInt_t TMbsUnpackTofPar::GetGlobalToActiveInd( UInt_t uBoardToFind )
{
   UInt_t uIndActiveSameBoard = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
   {
      // When find target return its index among active boards
      // of same type
      if( uBoardToFind == uBoard )
         return uIndActiveSameBoard;
      // Count active boards of same type as target
      if( GetBoardType( uBoardToFind ) == GetBoardType( uBoard ) &&
          kTRUE == IsActive( uBoard ) )
          uIndActiveSameBoard++;
   } // for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
   // If the board was not found:
   // return total number of boards as it will be rejected by other functions!
   return fuNbVmeBoards;
}
UInt_t TMbsUnpackTofPar::GetActiveToAllTypeInd( UInt_t uBoardToFind, UInt_t uType )
{
   UInt_t uIndBoardSameType = 0;
   UInt_t uIndActiveSameBoard = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
   {
      // When find target return its index among boards
      // of same type
      if( uBoardToFind == uIndActiveSameBoard &&
           kTRUE == IsActive( uBoard ))
         return uIndBoardSameType;
      // Count all boards of same type as target
      if( static_cast<Int_t>(uType) == GetBoardType( uBoard ) )
      {
         uIndBoardSameType++;
         // Count active boards of same type as target
         if( kTRUE == IsActive( uBoard ) )
            uIndActiveSameBoard++;
       }
   } // for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
   // If the boad was not found:
   // return total number of boards as it will be rejected by other functions!
   return fuNbVmeBoards;
}
//*******************************************************************************/
//*********** Not VME boards ****************************************************/
Int_t TMbsUnpackTofPar::GetActiveNotVmeBoardsNumber()
{
   UInt_t uNbActive = 0;
   // Count all active Non-VME boards of all types
   for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++ )
      if( kTRUE == IsActiveNotVme( uBoard ) )
         uNbActive++;
   return uNbActive;
}
Int_t TMbsUnpackTofPar::GetNotVmeMappingField( UInt_t uBoard, UInt_t uField)
{
   LOG(debug3)<<"TMbsUnpackTofPar::GetNotVmeMappingField: Trying to get combination ("
              <<uBoard<<"/"<<uField<<") => Index "<<uBoard*tofMbs::kuNbFieldsMapping + uField;
   if( static_cast<Int_t>(fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping) == fiNonVmeMapping.GetSize() )
   {
      if( uBoard < fuNbNonVmeBoards && uField < tofMbs::kuNbFieldsMapping )
         return fiNonVmeMapping[ uBoard*tofMbs::kuNbFieldsMapping + uField ];
         else
         {
            LOG(warn)<<"TMbsUnpackTofPar::GetNotVmeMappingField: wrong Board/field combination ("
                        <<uBoard<<"/"<<uField<<")";
            return -1;
         } // else of if( uBoard < fuNbNonVmeBoards && uField < tofMbs::kuNbFieldsMapping )
   } // if( fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping == fiMbsVmeMapping.GetSize() )
      else
      {
         LOG(error)<<"TMbsUnpackTofPar::GetNotVmeMappingField: wrong fiNonVmeMapping size ("
                     <<fiNonVmeMapping.GetSize()<<" instead of "
                     <<fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping<<"), maybe not initialized!";
         return -1;
      } // else of if( fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping == fiNonVmeMapping.GetSize() )
}
ULong64_t TMbsUnpackTofPar::GetNotVmeBoardTag( UInt_t uBoard)
{
   if( static_cast<Int_t>(fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping) == fiNonVmeMapping.GetSize() )
   {
      if( uBoard < fuNbNonVmeBoards )
      {
         ULong64_t uFullTag = ( ( (ULong64_t)GetNotVmeMappingField( uBoard, tofMbs::TokenB ) )<< 32 ) |
                                  (ULong64_t)GetNotVmeMappingField( uBoard, tofMbs::TokenA );
         return uFullTag;
      } // if( uBoard < fuNbVmeBoards )
         else
         {
            LOG(warn)<<"TMbsUnpackTofPar::GetNotVmeBoardTag: wrong Board ("
                        <<uBoard<<")";
            return 0;
         } // else of if( uBoard < fuNbNonVmeBoards && uField < tofMbs::kuNbFieldsMapping )
   } // if( fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping == fiNonVmeMapping.GetSize() )
      else
      {
         LOG(error)<<"TMbsUnpackTofPar::GetNotVmeBoardTag: wrong fiNonVmeMapping size ("
                     <<fiNonVmeMapping.GetSize()<<" instead of "
                     <<fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping<<"), maybe not initialized!";
         return 0;
      } // else of if( fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping == fiNonVmeMapping.GetSize() )
}
Bool_t TMbsUnpackTofPar::IsActiveNotVme( UInt_t uBoard)
{
   if( static_cast<Int_t>(fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping) == fiNonVmeMapping.GetSize() )
   {
      if( uBoard < fuNbNonVmeBoards )
         return (1 == GetNotVmeMappingField( uBoard, tofMbs::Active ) )? kTRUE: kFALSE;
         else
         {
            LOG(warn)<<"TMbsUnpackTofPar::IsActiveNotVme: wrong Board ("
                        <<uBoard<<")";
            return kFALSE;
         } // else of if( uBoard < fuNbNonVmeBoards && uField < tofMbs::kuNbFieldsMapping )
   } // if( fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping == fiNonVmeMapping.GetSize() )
      else
      {
         LOG(error)<<"TMbsUnpackTofPar::IsActiveNotVme: wrong fiNonVmeMapping size ("
                     <<fiNonVmeMapping.GetSize()<<" instead of "
                     <<fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping<<"), maybe not initialized!";
         return kFALSE;
      } // else of if( fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping == fiNonVmeMapping.GetSize() )
}
Int_t TMbsUnpackTofPar::GetNotVmeBoardType( UInt_t uBoard)
{
   return GetNotVmeMappingField( uBoard, tofMbs::Type);
};
UInt_t TMbsUnpackTofPar::GetGlobToTypeIndNoVme( UInt_t uBoardToFind )
{
   UInt_t uIndBoardSameType = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++)
   {
      // When find target return its index among boards
      // of same type
      if( uBoardToFind == uBoard )
         return uIndBoardSameType;
      // Count active boards of same type as target
      if( GetNotVmeBoardType( uBoardToFind ) == GetNotVmeBoardType( uBoard ) )
         uIndBoardSameType++;
   } // for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++)
   // If the board was not found:
   // return total number of boards as it will be rejected by other functions!
   return fuNbNonVmeBoards;
}
UInt_t TMbsUnpackTofPar::GetGlobToActiveIndNoVme( UInt_t uBoardToFind )
{
   UInt_t uIndActiveSameBoard = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++)
   {
      // When find target return its index among active boards
      // of same type
      if( uBoardToFind == uBoard )
         return uIndActiveSameBoard;
      // Count active boards of same type as target
      if( GetNotVmeBoardType( uBoardToFind ) == GetNotVmeBoardType( uBoard ) &&
          kTRUE == IsActiveNotVme( uBoard ) )
          uIndActiveSameBoard++;
   } // for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++)
   // If the board was not found:
   // return total number of boards as it will be rejected by other functions!
   return fuNbNonVmeBoards;
}
UInt_t TMbsUnpackTofPar::GetActivToAllTypeIndNoVme( UInt_t uBoardToFind, UInt_t uType )
{
   UInt_t uIndBoardSameType = 0;
   UInt_t uIndActiveSameBoard = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++)
   {
      // When find target return its index among boards
      // of same type
      if( uBoardToFind == uIndActiveSameBoard && 
           kTRUE == IsActiveNotVme( uBoard ))
         return uIndBoardSameType;
      // Count all boards of same type as target
      if( static_cast<Int_t>(uType) == GetNotVmeBoardType( uBoard ) )
      {
         uIndBoardSameType++;
         // Count active boards of same type as target
         if( kTRUE == IsActiveNotVme( uBoard ) )
            uIndActiveSameBoard++;
       }
   } // for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++)
   // If the boad was not found: 
   // return total number of boards as it will be rejected by other functions!
   return fuNbNonVmeBoards;
}
//*******************************************************************************/
//*************** All boards ****************************************************/
UInt_t TMbsUnpackTofPar::GetNbActiveBoards()
{
   UInt_t uNbActiveBoards = 0;

   uNbActiveBoards += GetActiveBoardsNumber();
   uNbActiveBoards += GetActiveNotVmeBoardsNumber();

   return uNbActiveBoards;
}
UInt_t TMbsUnpackTofPar::GetNbActiveBoards( UInt_t uType )
{
   UInt_t uNbActiveSameBoard = 0;

   if( tofMbs::trb == uType)
      uType = tofMbs::trbtdc;

   // Count active boards of asked type
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
      if( static_cast<Int_t>(uType) == GetBoardType( uBoard ) &&
          kTRUE == IsActive( uBoard ) )
          uNbActiveSameBoard++;

   // Count active boards of asked type
   for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++)
      if( static_cast<Int_t>(uType) == GetNotVmeBoardType( uBoard ) &&
          kTRUE == IsActiveNotVme( uBoard ) )
          uNbActiveSameBoard++;

   return uNbActiveSameBoard;
}
UInt_t TMbsUnpackTofPar::GetNbTdcs( )
{
   // If 0 recheck if it was simply not filled from params
   if( 0 == fuNbTdc )
      FillBoardsNbPerType();
      
   return fuNbTdc;
}
UInt_t TMbsUnpackTofPar::GetNbActiveTdcs( )
{
   // If 0 recheck if it was simply not filled from params
   if( 0 == fuNbTdcActive )
      FillActiveBoardsNbPerType();
      
   return fuNbTdcActive;
}
UInt_t TMbsUnpackTofPar::GetNbScalersB( )
{
   // If 0 recheck if it was simply not filled from params
   if( 0 == fuNbScalerB )
      FillBoardsNbPerType();
      
   return fuNbScalerB;
}
UInt_t TMbsUnpackTofPar::GetNbActiveScalersB( )
{
   // If 0 recheck if it was simply not filled from params
   if( 0 == fuNbScalerActiveB )
      FillActiveBoardsNbPerType();
      
   return fuNbScalerActiveB;
}
Bool_t TMbsUnpackTofPar::WithActiveTriglog( )
{
   if( 0 == fuNbTriglogActive )
      FillActiveBoardsNbPerType();
   
   if( 0 == fuNbTriglogActive )
      return kFALSE;
      else return kTRUE;
}
   
void TMbsUnpackTofPar::FillBoardsNbPerType()
{
   // Scaler boards
   fuNbScalerB  = 0;
   fuNbTriglog  = 0;
   fuNbScalOrMu = 0;
      // TDC boards
   fuNbTdc      = 0;
   fuNbV1290    = 0;
   fuNbVftx     = 0;
   fuTrbTdcNb  = 0;
   fuNbGet4     = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
      switch( GetBoardType( uBoard ) )
      {
         case tofMbs::caenV1290:
            fuNbTdc++;
            fuNbV1290++;
            break;
         case tofMbs::vftx:
            fuNbTdc++;
            fuNbVftx++;
            break;
         case tofMbs::get4:
            fuNbTdc++;
            fuNbGet4++;
            break;
         case tofMbs::triglog:
            fuNbScalerB++;
            fuNbTriglog++;
            break;
         case tofMbs::scalormu:
            fuNbScalerB++;
            fuNbScalOrMu++;
            break;
         case tofMbs::scaler2014:
            fuNbScalerB++;
            break;
         case tofMbs::orgen:
            fuNbScalerB++;
            break;
         case tofMbs::triglogscal:
            fuNbScalerB++;
            break;
         case tofMbs::caenv965:
         case tofMbs::lecroy1182:
         case tofMbs::vulqfwread:
            break;
         case tofMbs::undef:
         default:
            LOG(warn)<<"TMbsUnpackTofPar::FillBoardsNbPerType => Unknown board type "
                        <<GetBoardType( uBoard )
                        <<"!!!!" ;
      } // switch( GetBoardType( uBoard ) )
   for( UInt_t uBoard = 0; uBoard < GetNotVmeBoardsNumber(); uBoard++)
      switch( GetNotVmeBoardType( uBoard ) )
      {
         case tofMbs::caenV1290:
            fuNbTdc++;
            fuNbV1290++;
            break;
         case tofMbs::vftx:
            fuNbTdc++;
            fuNbVftx++;
            break;
         case tofMbs::trbseb:
            fuTrbSebNb++;
            break;
         case tofMbs::trbtdc:
            fuNbTdc++;
            fuTrbTdcNb++;
            break;
         case tofMbs::trbhub:
            fuTrbHubNb++;
            break;
         case tofMbs::get4:
            fuNbTdc++;
            fuNbGet4++;
            break;
         case tofMbs::triglog:
            fuNbScalerB++;
            fuNbTriglog++;
            break;
         case tofMbs::scalormu:
            fuNbScalerB++;
            fuNbScalOrMu++;
            break;
         case tofMbs::scaler2014:
            fuNbScalerB++;
            break;
         case tofMbs::orgen:
            fuNbScalerB++;
            break;
         case tofMbs::triglogscal:
            fuNbScalerB++;
            break;
         case tofMbs::caenv965:
         case tofMbs::lecroy1182:
         case tofMbs::vulqfwread:
            break;
         case tofMbs::undef:
         default:
            LOG(warn)<<"TMbsUnpackTofPar::FillBoardsNbPerType => Unknown board type "
                        <<GetNotVmeBoardType( uBoard )
                        <<"!!!!" ;
      } // switch( GetNotVmeBoardType( uBoard ) )
}    
void TMbsUnpackTofPar::FillActiveBoardsNbPerType()
{
   // Scaler boards
   fuNbScalerActiveB = 0;
   fuNbTriglogActive = 0;
   fuNbScalOrMuActive = 0;
      // TDC boards
   fuNbTdcActive     = 0;
   for( UInt_t uBoard = 0; uBoard < fuNbVmeBoards; uBoard++)
      if( kTRUE == IsActive( uBoard ) )
         switch( GetBoardType( uBoard ) )
         {
            case tofMbs::caenV1290:
            case tofMbs::vftx:
            case tofMbs::get4:
               fuNbTdcActive++;
               break;
            case tofMbs::triglog:
               fuNbTriglogActive++;
               fuNbScalerActiveB++;
               break;
            case tofMbs::scalormu:
               fuNbScalOrMuActive++;
               fuNbScalerActiveB++;
               break;
            case tofMbs::scaler2014:
               fuNbScalerActiveB++;
               break;
            case tofMbs::orgen:
               fuNbScalerActiveB++;
               break;
            case tofMbs::triglogscal:
               fuNbScalerActiveB++;
               break;
            case tofMbs::caenv965:
            case tofMbs::lecroy1182:
            case tofMbs::vulqfwread:
               break;
            case tofMbs::undef:
            default:
               LOG(warn)<<"TMbsUnpackTofPar::FillBoardsNbPerType => Unknown board type "
                           <<GetBoardType( uBoard )
                           <<"!!!!" ;
         } // switch( GetBoardType( uBoard ) )
   for( UInt_t uBoard = 0; uBoard < GetNotVmeBoardsNumber(); uBoard++)
      if( kTRUE == IsActiveNotVme( uBoard ) )
         switch( GetNotVmeBoardType( uBoard ) )
         {
            case tofMbs::caenV1290:
            case tofMbs::vftx:
            case tofMbs::trbtdc:
            case tofMbs::get4:
               fuNbTdcActive++;
               break;
            case tofMbs::triglog:
               fuNbTriglogActive++;
               fuNbScalerActiveB++;
               break;
            case tofMbs::scalormu:
               fuNbScalOrMuActive++;
               fuNbScalerActiveB++;
               break;
            case tofMbs::scaler2014:
               fuNbScalerActiveB++;
               break;
            case tofMbs::orgen:
               fuNbScalerActiveB++;
               break;
            case tofMbs::triglogscal:
               fuNbScalerActiveB++;
               break;
            case tofMbs::caenv965:
            case tofMbs::lecroy1182:
            case tofMbs::vulqfwread:
            case tofMbs::trbseb:
            case tofMbs::trbhub:
               break;
            case tofMbs::undef:
            default:
               LOG(warn)<<"TMbsUnpackTofPar::FillBoardsNbPerType => Unknown board type "
                           <<GetNotVmeBoardType( uBoard )
                           <<"!!!!" ;
         } // switch( GetNotVmeBoardType( uBoard ) )
}


Bool_t TMbsUnpackTofPar::OnlyOneTriglog()
{
   return ( 1 == GetNbActiveBoards( tofMbs::triglog ) ? kTRUE: kFALSE);
}

UInt_t TMbsUnpackTofPar::GetTrbSebAddrForFpga( UInt_t uTrbNetAddress )
{
  UInt_t uSebAddr = 0xffff;

  if( fMapFpgaAddrToTrbSebAddr.count(uTrbNetAddress) )
  {
    uSebAddr = fMapFpgaAddrToTrbSebAddr[uTrbNetAddress];
    return uSebAddr;
  }
  else
  {
    return uSebAddr;
  }
}

UInt_t TMbsUnpackTofPar::GetFpgaNbPerTrbSeb( UInt_t uTrbNetAddress )
{
  UInt_t uFpgaNb = 0;

  if( fMapTrbSebAddrToFpgaNb.count(uTrbNetAddress) )
  {
    uFpgaNb = fMapTrbSebAddrToFpgaNb[uTrbNetAddress];
    return uFpgaNb;
  }
  else
  {
    return uFpgaNb;
  }
}

UInt_t TMbsUnpackTofPar::GetInDataFpgaNbPerTrbSeb( UInt_t uTrbNetAddress )
{
  UInt_t uInDataFpgaNb = 0;

  if( fMapTrbSebAddrToFpgaInDataNb.count(uTrbNetAddress) )
  {
    uInDataFpgaNb = fMapTrbSebAddrToFpgaInDataNb[uTrbNetAddress];
    return uInDataFpgaNb;
  }
  else
  {
    return uInDataFpgaNb;
  }
}

UInt_t TMbsUnpackTofPar::GetUnpackTdcNbPerTrbSeb( UInt_t uTrbNetAddress )
{
  UInt_t uActiveTdcNb = 0;

  if( fMapTrbSebAddrToUnpackTdcNb.count(uTrbNetAddress) )
  {
    uActiveTdcNb = fMapTrbSebAddrToUnpackTdcNb[uTrbNetAddress];
    return uActiveTdcNb;
  }
  else
  {
    return uActiveTdcNb;
  }
}

Int_t TMbsUnpackTofPar::GetTrbSebIndex( UInt_t uTrbNetAddress )
{
  Int_t iIndex = -1;

  if( fMapTrbSebAddrToTrbSebIndex.count(uTrbNetAddress) )
  {
    iIndex = fMapTrbSebAddrToTrbSebIndex[uTrbNetAddress];
    return iIndex;
  }
  else
  {
    return iIndex;
  }

}

UInt_t TMbsUnpackTofPar::GetTrbSebAddr( Int_t iTrbSebIndex )
{
  UInt_t uSebAddr = 0xffff;

  if( fMapTrbSebIndexToTrbSebAddr.count(iTrbSebIndex) )
  {
    uSebAddr = fMapTrbSebIndexToTrbSebAddr[iTrbSebIndex];
    return uSebAddr;
  }
  else
  {
    return uSebAddr;
  }

}

Int_t TMbsUnpackTofPar::GetActiveTrbTdcIndex( UInt_t uTrbNetAddress )
{
  Int_t iIndex = -1;

  if( fMapFpgaAddrToActiveTdcIndex.count(uTrbNetAddress) )
  {
    iIndex = fMapFpgaAddrToActiveTdcIndex[uTrbNetAddress];
    return iIndex;
  }
  else
  {
    return iIndex;
  }
}

UInt_t TMbsUnpackTofPar::GetActiveTrbTdcAddr( Int_t iActiveTrbTdcIndex )
{
  UInt_t uFpgaAddr = 0xffff;

  if( fMapActiveTdcIndexToFpgaAddr.count(iActiveTrbTdcIndex) )
  {
    uFpgaAddr = fMapActiveTdcIndexToFpgaAddr[iActiveTrbTdcIndex];
    return uFpgaAddr;
  }
  else
  {
    return uFpgaAddr;
  }

}

void TMbsUnpackTofPar::FillTrbMaps()
{
   LOG(debug2)<<"**** TMbsUnpackTofPar: Call FillTrbMaps()...";

   if( static_cast<Int_t>(fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping) == fiNonVmeMapping.GetSize() )
   {

     Int_t iActiveTrbTdcIndex = 0;
     Int_t iTrbSebIndex = 0;

     for( UInt_t uBoard = 0; uBoard < fuNbNonVmeBoards; uBoard++ )
     {
       Int_t iBoardType = fiNonVmeMapping[ uBoard*tofMbs::kuNbFieldsMapping + tofMbs::Type ];

       if( tofMbs::trb == iBoardType/10 )
       {
         Bool_t bFpgaInData = (Bool_t)fiNonVmeMapping[ uBoard*tofMbs::kuNbFieldsMapping + tofMbs::Active ];
         UInt_t uFpgaAddr = (UInt_t)fiNonVmeMapping[ uBoard*tofMbs::kuNbFieldsMapping + tofMbs::Address ];
         UInt_t uSebAddr = (UInt_t)fiNonVmeMapping[ uBoard*tofMbs::kuNbFieldsMapping + tofMbs::TokenA ];
         Bool_t bUnpackTdc = (Bool_t)fiNonVmeMapping[ uBoard*tofMbs::kuNbFieldsMapping + tofMbs::TokenB ];

         if( 0 < fMapFpgaAddrToInDataFlag.count(uFpgaAddr) )
         {
           LOG(fatal)<<Form("FPGA address 0x%.4x appears more than once in the NonVmeMapping list. Abort program execution!", uFpgaAddr);
         }

         fMapFpgaAddrToInDataFlag[uFpgaAddr] = bFpgaInData;
         fMapFpgaAddrToTrbSebAddr[uFpgaAddr] = uSebAddr;

         if( bFpgaInData )
         {
           if( 0 == fMapTrbSebAddrToFpgaInDataNb.count(uSebAddr) )
           {
             fMapTrbSebAddrToFpgaInDataNb[uSebAddr] = 0;
           }
           if( 0 < fMapTrbSebAddrToFpgaInDataNb.count(uSebAddr) )
           {
             fMapTrbSebAddrToFpgaInDataNb[uSebAddr]++;
           }

           if( tofMbs::trbseb == iBoardType )
           {
             fMapTrbSebAddrToTrbSebIndex[uFpgaAddr] = iTrbSebIndex;
             fMapTrbSebIndexToTrbSebAddr[iTrbSebIndex] = uFpgaAddr;
             iTrbSebIndex++;
           }

           if( tofMbs::trbtdc == iBoardType )
           {
             if(bUnpackTdc)
             {
               if( 0 == fMapTrbSebAddrToUnpackTdcNb.count(uSebAddr) )
               {
                 fMapTrbSebAddrToUnpackTdcNb[uSebAddr] = 0;
               }
               if( 0 < fMapTrbSebAddrToUnpackTdcNb.count(uSebAddr) )
               {
                 fMapTrbSebAddrToUnpackTdcNb[uSebAddr]++;
               }

               fActiveTrbTdcAddrToUnpackFlag[uFpgaAddr] = bUnpackTdc;
               fActiveTrbTdcIndexToUnpackFlag[iActiveTrbTdcIndex] = bUnpackTdc;
             }

             fMapFpgaAddrToActiveTdcIndex[uFpgaAddr] = iActiveTrbTdcIndex;
             fMapActiveTdcIndexToFpgaAddr[iActiveTrbTdcIndex] = uFpgaAddr;
             iActiveTrbTdcIndex++;
           }

         }
         else
         {
           if( 0 == fMapTrbSebAddrToFpgaNb.count(uSebAddr) )
           {
             fMapTrbSebAddrToFpgaNb[uSebAddr] = 0;
           }
           if( 0 < fMapTrbSebAddrToFpgaNb.count(uSebAddr) )
           {
             fMapTrbSebAddrToFpgaNb[uSebAddr]++;
           }
         }

       }
     }

   }

   else
   {
     LOG(error)<<"TMbsUnpackTofPar::FillTrbMaps: wrong fiNonVmeMapping size ("
               <<fiNonVmeMapping.GetSize()<<" instead of "
               <<fuNbNonVmeBoards*tofMbs::kuNbFieldsMapping<<"), maybe not initialized!";
   }
}

Int_t TMbsUnpackTofPar::GetCtsTrigMappingField( UInt_t uChannel, UInt_t uField)
{
   LOG(debug3)<<"TMbsUnpackTofPar::GetCtsTrigMappingField: Trying to get combination ("
              <<uChannel<<"/"<<uField<<") => Index "<<uChannel*tofMbs::kuNbFieldsCtsMapping + uField;
   if( static_cast<Int_t>(fuNbCtsTrigChs*tofMbs::kuNbFieldsCtsMapping) == fiCtsTriggerMap.GetSize() )
   {
      if( uChannel < fuNbCtsTrigChs && uField < tofMbs::kuNbFieldsCtsMapping )
         return fiCtsTriggerMap[ uChannel*tofMbs::kuNbFieldsCtsMapping + uField ];
      else
      {
        LOG(warn)<<"TMbsUnpackTofPar::GetCtsTrigMappingField: wrong channel/field combination ("
                    <<uChannel<<"/"<<uField<<")";
        return -1;
      }
   }
   else
   {
      LOG(error)<<"TMbsUnpackTofPar::GetCtsTrigMappingField: wrong fiCtsTriggerMap size ("
                <<fiCtsTriggerMap.GetSize()<<" instead of "
                <<fuNbCtsTrigChs*tofMbs::kuNbFieldsCtsMapping<<"), maybe not initialized!";
      return -1;
   }
}

Bool_t TMbsUnpackTofPar::IsTrbEventUnpacked(UInt_t uTriggerPattern)
{
   Bool_t bUnpack = kTRUE;

   for( UInt_t uChannel = 0; uChannel < fuNbCtsTrigChs; uChannel++)
   {
     if( 1 == GetCtsTrigMappingField(uChannel,tofMbs::TriggerUnpack) )
     {
       bUnpack = bUnpack && ((uTriggerPattern >> uChannel) & 0x1);
     }
   }

   return bUnpack;
}

UInt_t TMbsUnpackTofPar::GetFirstTrbTrigger(UChar_t cTriggerType, UInt_t uTriggerPattern)
{
  UInt_t uFirstPattern = uTriggerPattern;

  for( UInt_t uChannel = 0; uChannel < fuNbCtsTrigChs; uChannel++)
  {
    if( (uFirstPattern >> uChannel) & 0x1 )
    {
      if( (UChar_t)GetCtsTrigMappingField(uChannel,tofMbs::TriggerType) != cTriggerType)
      {
        uFirstPattern = uFirstPattern ^ (0x1 << uChannel);
      }
    }
  }

  return uFirstPattern;
}
