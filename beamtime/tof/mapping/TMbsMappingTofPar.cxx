// ------------------------------------------------------------------
// -----                     TMbsMappingTofPar                   -----
// -----              Created 09/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsMappingTofPar.h"

// TOF headers
#include "TofTdcDef.h"
#include "TofCaenDef.h"
#include "TofVftxDef.h"
#include "TofTrbTdcDef.h"
#include "TofGet4Def.h"
#include "CbmTofAddress.h"

// FAIR headers
#include "FairLogger.h"
#include "FairParamList.h" 

// ROOT headers
#include "TString.h"

ClassImp(TMbsMappingTofPar)

// TODO: To be defined somewhere else later!! FIXME 
//                                                     HD   BUC  PLA CRPC Ref  BHD  newBUC
//const Int_t TMbsMappingTofPar::fgkiNbChDetType[7] = { 32,  72,   1,   8,  10,  56,     8 }; 
Int_t TMbsMappingTofPar::fgkiNbChDetType[fgkiNbDetType] =
// Feb2015           P3   Ref   PLA    P2    P5   Dia  2013  USTC   PAD   ThuStr 2012  
//                    |     |     |     |     |     |     |     |     |     |     |
//               {   56,   72,    1,   32,   16,   16,   64,   16,   8,    24,    8 };
// Nov2015           P3   Ref   PLA    P2    P5   Dia  2015  2012   PAD   Star  ThuStr   
//                    |     |     |     |     |     |     |     |     |     |     |
//               {    1,   72,    1,   32,   16,   16,   40,   40,   8,    32,   24 };  
                 {    1,   72,    1,   32,   16,   16,   64,   40,   8,    32,   24 };  
// This information should be collected from the DigiPar file (nh, Nov. 2015)

TMbsMappingTofPar::TMbsMappingTofPar() :
    FairParGenericSet("TMbsMappingTofPar","Tof MBS Mapping Parameters","TestDefaultContext"),
    fuMappingDebug(0),
    fiNbMappedTdc(0),
    fiNbChTdc(),
    fiTdcUniqueId(),
    fiMapping(),
    fiUseExtendedDigi(0),
    fiNbMappedDet(0),
    fiDetUniqueId()
{
}

TMbsMappingTofPar::TMbsMappingTofPar(const char* name,
                                  const char* title,
                                  const char* context ) : 
    FairParGenericSet(name,title,context),
    fuMappingDebug(0),
    fiNbMappedTdc(0),
    fiNbChTdc(),
    fiTdcUniqueId(),
    fiMapping(),
    fiUseExtendedDigi(0),
    fiNbMappedDet(0),
    fiDetUniqueId()
{
}

TMbsMappingTofPar::~TMbsMappingTofPar()
{
}

void TMbsMappingTofPar::clear(void)
{
	  status = kFALSE;
	  resetInputVersions();
} 

void TMbsMappingTofPar::putParams(FairParamList* l)
{
  if (!l) { return; }

  l->add("MappingDebug", (Int_t)fuMappingDebug);
  l->add("NbMappedTdc",  fiNbMappedTdc);
  l->add("NbChTdc",      fiNbChTdc);
  l->add("TdcUniqueId",  fiTdcUniqueId);
  l->add("Mapping",      fiMapping);
  l->add("UseExtDigi",   fiUseExtendedDigi);
  l->add("NbMappedDet",  fiNbMappedDet);
  l->add("DetUniqueId",  fiDetUniqueId);
}

Bool_t TMbsMappingTofPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  
  /*
   * TODO PAL: Thing I am not sure is whether order in readout has to 
   * match order in the ascii file => to check
   */
   
  LOG(debug2)<<"Get the Tof MBS mapping parameters.";
  
  Int_t iTempCaster = 0;
  if ( ! l->fill("MappingDebug", &iTempCaster) ) return kFALSE;
  fuMappingDebug = (UInt_t)iTempCaster;
  
  if ( ! l->fill("NbMappedTdc",    &fiNbMappedTdc) ) return kFALSE;
  
  if( 0 < fiNbMappedTdc )
  {
      fiNbChTdc.Set( fiNbMappedTdc );
      if ( ! l->fill("NbChanTdc",  &fiNbChTdc) ) return kFALSE;

      fiTdcUniqueId.Set( fiNbMappedTdc );
      if ( ! l->fill("TdcUniqueId", &fiTdcUniqueId) ) return kFALSE;

      Int_t iTotalNbChMapped = 0;
      for( Int_t iMappTdcIndex = 0; iMappTdcIndex < fiNbMappedTdc; iMappTdcIndex++)
         iTotalNbChMapped += fiNbChTdc[iMappTdcIndex];
      if( 0 < iTotalNbChMapped )
      {
         fiMapping.Set( iTotalNbChMapped );
         if ( ! l->fill("Mapping",  &fiMapping) ) return kFALSE;
      } // if( 0 < iTotalNbChMapped )
  } // if( 0 < fiNbMappedTdc )
  if ( ! l->fill("UseExtDigi",     &fiUseExtendedDigi) ) return kFALSE;
  
  if ( ! l->fill("NbMappedDet",    &fiNbMappedDet) ) return kFALSE;
  if( 0 < fiNbMappedDet )
  {
      fiDetUniqueId.Set( fiNbMappedDet );
      if ( ! l->fill("DetUniqueId", &fiDetUniqueId) ) return kFALSE;
  } // if( 0 < fiNbMappedDet )
  
  return kTRUE;
}

void TMbsMappingTofPar::printParams()
{   
   LOG(info)<<"Parameter values in TMbsMappingTofPar: ";
   
   if(1 == fuMappingDebug)
      LOG(info)<<"  Mapping Debug                       ON!!!!!!";
      else LOG(info)<<"  Mapping Debug                       OFF";   
      
   LOG(info)<<"  Nb of TDC mapped to detectors:      "<<fiNbMappedTdc;
   
   if( 0 < fiNbMappedTdc )
   {
      TString sTdcInd  = "Tdc #:                --- ";
      TString sNbChTdc = "Nb ch per Mapped TDC: |-> ";
      TString sUnIdTdc = "TDC Unique Id:        |-> ";
      
      TString sChanInd = "Channel Index:               --- ";
      TString* sMapping = new TString[fiNbMappedTdc];
      
      Int_t iMappingIndex = 0;
      Int_t iMaxChannels  = fiNbChTdc[0];
      for( Int_t iMappTdcIndex = 0; iMappTdcIndex < fiNbMappedTdc; iMappTdcIndex++)
      {
         sTdcInd  += Form("  %8d ",  iMappTdcIndex);
         sNbChTdc += Form("  %8d ",  fiNbChTdc[iMappTdcIndex]);
         sUnIdTdc += Form("0x%08X ", fiTdcUniqueId[iMappTdcIndex]);
         
         sMapping[iMappTdcIndex] = Form("Tdc #%03d, mapped to Det UId: |-> ", iMappTdcIndex);
         if( iMaxChannels < fiNbChTdc[iMappTdcIndex] )
            sChanInd = "Channel Index:           ---";
         for( Int_t iChanInd = 0; iChanInd < fiNbChTdc[iMappTdcIndex]; iChanInd++)
         {
            if( 0xFFFFFFFF == static_cast<UInt_t>(fiMapping[ iMappingIndex + iChanInd ]) )
               sMapping[iMappTdcIndex] += "0xFFFFFFFF ";
               else sMapping[iMappTdcIndex] += Form("0x%08X ", fiMapping[ iMappingIndex + iChanInd ]);
            if( 0 == iMappTdcIndex )
               sChanInd += Form("  %8d ", iChanInd);
            else if( iMaxChannels < fiNbChTdc[iMappTdcIndex] )
               sChanInd += Form("  %8d ", iChanInd);
         } // for( Int_t iChanInd = 0; iChanInd < fiNbChTdc[iMappTdcIndex]; iChanInd++)
         if( iMaxChannels < fiNbChTdc[iMappTdcIndex] )
            iMaxChannels = fiNbChTdc[iMappTdcIndex];
         
         iMappingIndex += fiNbChTdc[iMappTdcIndex];
      } // for( Int_t iMappTdcIndex = 0; iMappTdcIndex < fiNbMappedTdc; iMappTdcIndex++)
      LOG(info)<<sTdcInd;
      LOG(info)<<sNbChTdc;
      LOG(info)<<sUnIdTdc;
      LOG(info)<<sChanInd;
      for( Int_t iMappTdcIndex = 0; iMappTdcIndex < fiNbMappedTdc; iMappTdcIndex++)
         LOG(info)<<sMapping[iMappTdcIndex];

      delete [] sMapping;
   } // if( 0 < fiNbMappedTdc )
   
   
   if(1 == fiUseExtendedDigi)
      LOG(info)<<"  Type of Digis used:                 Extended";
      else LOG(info)<<"  Type of Digis used:                 Compressed";   
      
   LOG(info)<<"  Nb of detectors in mapping:         "<<fiNbMappedDet;
   if( 0 < fiNbMappedDet )
   {
      TString sDetInd  = "Det #:                --- ";
      TString sUnIdDet = "Det Unique Id:        |-> ";
      for( Int_t iMappDetIndex = 0; iMappDetIndex < fiNbMappedDet; iMappDetIndex++)
      {
         sDetInd  += Form("  %8d ",  iMappDetIndex);
         sUnIdDet += Form("0x%08X ", fiDetUniqueId[iMappDetIndex]);
      } // for( Int_t iMappDetIndex = 0; iMappDetIndex < fiNbMappedDet; iMappDetIndex++)
      LOG(info)<<sDetInd;
      LOG(info)<<sUnIdDet;
   } // if( 0 < fiNbMappedDet )
   return;
}
// ------------------------------------------------------------------ 
Int_t TMbsMappingTofPar::GetMappedTdcUId( Int_t iMappedTdcInd ) const
{
   if( iMappedTdcInd < fiNbMappedTdc )
      return fiTdcUniqueId[iMappedTdcInd];
      else return -1; // Equal to 0xFFFFFFFF => inactive
}
Int_t TMbsMappingTofPar::GetMappedTdcInd( Int_t iTdcUId ) const
{
   for( Int_t iMappedTdcInd = 0; iMappedTdcInd < GetNbMappedTdc(); iMappedTdcInd++)
      if( iTdcUId == GetMappedTdcUId( iMappedTdcInd ) )
         return iMappedTdcInd;
         
   // If none of the TDC matches => return -1, calling function/method has to catch it
   // and ignore the corresponding data object
   return -1;
}
Int_t TMbsMappingTofPar::GetMapping( Int_t iMappedTdcInd, Int_t iMappedChan ) const
{
   if( iMappedTdcInd < fiNbMappedTdc )
   {
      if( iMappedChan < fiNbChTdc[iMappedTdcInd] )
      {
         Int_t iMappingIndex = 0;
         for( Int_t iPrevTdcIndx = 0; iPrevTdcIndx < iMappedTdcInd; iPrevTdcIndx++)
            iMappingIndex += fiNbChTdc[iPrevTdcIndx];
         return fiMapping[ iMappingIndex + iMappedChan ];
      } // if( iMappedChan < fiNbChTdc[iMappedTdcInd] )
      else return -1; // Equal to 0xFFFFFFFF => inactive
   } // if( iMappedTdcInd < fiNbMappedTdc )
      else return -1; // Equal to 0xFFFFFFFF => inactive
}
Int_t TMbsMappingTofPar::GetMappedDetUId( Int_t iMappedDetInd ) const
{
   if( iMappedDetInd < fiNbMappedDet )
      return fiDetUniqueId[iMappedDetInd];
      else return -1; // Equal to 0xFFFFFFFF => inactive
}
Int_t TMbsMappingTofPar::GetMappedDetInd( Int_t iMappedDetUId ) const
{
   if( 0xFFFFFFFF == static_cast<UInt_t>(iMappedDetUId) )
      return -1; // Equal to 0xFFFFFFFF => inactive
      
   for( Int_t iDetIndx = 0; iDetIndx < fiNbMappedDet; iDetIndx++)
      if( kTRUE == CbmTofAddress::SameModule( iMappedDetUId, fiDetUniqueId[iDetIndx] ) )
         return iDetIndx;
  
   // If not matching any detector =>
   return -1; // Equal to 0xFFFFFFFF => inactive
}
// ------------------------------------------------------------------ 
Int_t TMbsMappingTofPar::GetSmTypeNbCh(   Int_t iSmType ) const
{
   if( -1 < iSmType && iSmType < fgkiNbDetType )
      return fgkiNbChDetType[iSmType];
      else return 0;
}
