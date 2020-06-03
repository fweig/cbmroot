// ------------------------------------------------------------------
// -----                     TMbsConvTofPar                   -----
// -----              Created 09/06/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsConvTofPar.h"

// TOF headers
#include "CbmTofAddress.h"

// FAIR headers
#include "FairLogger.h"
#include "FairParamList.h" 

// ROOT headers
#include "TString.h"

ClassImp(TMbsConvTofPar)

TMbsConvTofPar::TMbsConvTofPar() :
    FairParGenericSet("TMbsConvTofPar","Tof MBS Conversion Parameters","TestDefaultContext"),
    fuConvDebug(0),
    fsFilenameOut(""),
    fiEnableTriglog(0),
    fiNbOutScalers(0),
    fiScalersIndex(),
    fiNbOutVftx(0),
    fiVftxIndex(),
    fiNbRpc(0),
    fiRpcUniqueAddress(),
    fiRpcChType(),
    fiNbPlastics(0),
    fiPlasticUniqueAddress(),
    fiPlasticEndsNb()
{
}

TMbsConvTofPar::TMbsConvTofPar(const char* name,
                               const char* title,
                               const char* context ) : 
    FairParGenericSet(name,title,context),
    fuConvDebug(0),
    fsFilenameOut(""),
    fiEnableTriglog(0),
    fiNbOutScalers(0),
    fiScalersIndex(),
    fiNbOutVftx(0),
    fiVftxIndex(),
    fiNbRpc(0),
    fiRpcUniqueAddress(),
    fiRpcChType(),
    fiNbPlastics(0),
    fiPlasticUniqueAddress(),
    fiPlasticEndsNb()
{
}

TMbsConvTofPar::~TMbsConvTofPar()
{
}

void TMbsConvTofPar::clear(void)
{
	  status = kFALSE;
	  resetInputVersions();
} 

void TMbsConvTofPar::putParams(FairParamList* l)
{
  if (!l) { return; }

  l->add("ConvDebug", (Int_t)fuConvDebug);
  
  l->add("EnableTriglog", fiEnableTriglog);
  l->add("FilenameOut",   fsFilenameOut);
  l->add("NbOutScalers",  fiNbOutScalers);
  l->add("ScalersIndex",  fiScalersIndex);
  l->add("NbOutVftx",     fiNbOutVftx);
  l->add("VftxIndex",     fiVftxIndex);
  l->add("NbRpc",         fiNbRpc);
  l->add("RpcUniqueAdd",  fiPlasticUniqueAddress);
  l->add("RpcChType",     fiRpcChType);
  l->add("NbPlastics",    fiNbPlastics);
  l->add("PlaUniqueAdd",  fiPlasticUniqueAddress);
  l->add("PlaEndsNb",     fiPlasticEndsNb);
}

Bool_t TMbsConvTofPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  
  /*
   * TODO PAL: Thing I am not sure is whether order in readout has to 
   * match order in the ascii file => to check
   */
   
  LOG(debug2)<<"Get the Tof MBS Conversion parameters.";
  
  Int_t iTempCaster = 0;
  if ( ! l->fill("ConvDebug", &iTempCaster) ) return kFALSE;
  fuConvDebug = (UInt_t)iTempCaster;
  
  Int_t iMaxSizeFilename = 5000;
  Text_t *sTempText;
  sTempText = new Text_t[iMaxSizeFilename];
  if ( ! l->fill("FilenameOut", sTempText, iMaxSizeFilename ) ) return kFALSE;
  fsFilenameOut = sTempText;
  
  if ( ! l->fill("EnableTriglog", &fiEnableTriglog) ) return kFALSE;
  
  if ( ! l->fill("NbOutScalers", &fiNbOutScalers) ) return kFALSE;
  if( 0 < fiNbOutScalers )
  {
      fiScalersIndex.Set( fiNbOutScalers );
      if ( ! l->fill("ScalersIndex",  &fiScalersIndex) ) return kFALSE;
  } // if( 0 < fiNbOutScalers )
      
  if ( ! l->fill("NbOutVftx", &fiNbOutVftx) ) return kFALSE;
  if( 0 < fiNbOutVftx )
  {
      fiVftxIndex.Set( fiNbOutVftx );
      if ( ! l->fill("VftxIndex",  &fiVftxIndex) ) return kFALSE;
  } // if( 0 < fiNbOutVftx )
      
  if ( ! l->fill("NbRpc", &fiNbRpc) ) return kFALSE;
  if( 0 < fiNbRpc )
  {
      fiRpcUniqueAddress.Set( fiNbRpc );
      if ( ! l->fill("RpcUniqueAdd",  &fiRpcUniqueAddress) ) return kFALSE;
      
      fiRpcChType.Set( fiNbRpc );
      if ( ! l->fill("RpcChType",  &fiRpcChType) ) return kFALSE;
  } // if( 0 < fiNbRpc )
   
  if ( ! l->fill("NbPlastics", &fiNbPlastics) ) return kFALSE;
  if( 0 < fiNbPlastics )
  {
      fiPlasticUniqueAddress.Set( fiNbPlastics );
      if ( ! l->fill("PlaUniqueAdd",  &fiPlasticUniqueAddress) ) return kFALSE;
      
      fiPlasticEndsNb.Set( fiNbPlastics );
      if ( ! l->fill("PlaEndsNb",  &fiPlasticEndsNb) ) return kFALSE;
  } // if( 0 < fiNbPlastics )
  
  return kTRUE;
}

void TMbsConvTofPar::printParams()
{   
   LOG(info)<<"Parameter values in TMbsConvTofPar: ";
   
   if(1 == fuConvDebug)
      LOG(info)<<"  Conversion Debug                    ON!!!!!!";
      else LOG(info)<<"  Conversion Debug                    OFF";   
   
   // Output Filename
   LOG(info)<<"  ROOT output filename                "<<fsFilenameOut;
    
   // TRIGLOG
   if( 1 == fiEnableTriglog )
      LOG(info)<<"  TRIGLOG data in output:             ON";
      else LOG(info)<<"  TRIGLOG data in output:             OFF";
      
   // SCALERS
   if( 0 < fiNbOutScalers )
   {
      LOG(info)<<"  Nb Scalers output:                  "<<fiNbOutScalers;   
      
      TString sOutScal = "Output scaler Index: --- ";
      TString sCalScal = "Calib. scaler Index: |-> ";
      for( Int_t iOutScalIndx = 0; iOutScalIndx < fiNbOutScalers; iOutScalIndx++ )
      {
         sOutScal += Form( "%03d ", iOutScalIndx);
         sCalScal += Form( "%03d ", fiScalersIndex[iOutScalIndx]);
      } // for( Int_t iOutScalIndx = 0; iOutScalIndx < fiNbOutScalers; iOutScalIndx++ )
      LOG(info)<<sOutScal;
      LOG(info)<<sCalScal;
   } // if( 0 < fiNbOutScalers )
      else LOG(info)<<"  Nb Scalers output:                  NONE";   
      
   // VFTX
   if( 0 < fiNbOutVftx )
   {
      LOG(info)<<"  Nb VFTX in output:                  "<<fiNbOutVftx;   
      
      TString sOutVftx = "Output VFTX   Index: --- ";
      TString sCalVftx = "Calib. VFTX   Index: |-> ";
      for( Int_t iOutVftxIndx = 0; iOutVftxIndx < fiNbOutVftx; iOutVftxIndx++ )
      {
         sOutVftx += Form( "%03d ", iOutVftxIndx);
         sCalVftx += Form( "%03d ", fiVftxIndex[iOutVftxIndx]);
      } // for( Int_t iOutVftxIndx = 0; iOutVftxIndx < fiNbOutVftx; iOutVftxIndx++ )
      LOG(info)<<sOutVftx;
      LOG(info)<<sCalVftx;
   } // if( 0 < fiNbOutVftx )
      else LOG(info)<<"  Nb VFTX in output:                  NONE";  
  
   // RPC 
   if( 0 < fiNbRpc )
   {
      LOG(info)<<"  Nb RPC  in output:                  "<<fiNbRpc;   
      
      TString sOutRpc    = "Output RPC Index: --- ";
      TString sRpcUId    = "RPC unique ID:    |-> ";
      TString sRpcChType = "RPC Channel Type  |-> ";
      for( Int_t iRpcIndx = 0; iRpcIndx < fiNbRpc; iRpcIndx++ )
      {
         sOutRpc += Form( "%10d ", iRpcIndx);
         sRpcUId += Form( "0x%08x ", (UInt_t)(fiRpcUniqueAddress[iRpcIndx]) );
         if( 1 == fiRpcChType[iRpcIndx] )
            sRpcChType += "      PADS ";
            else if( 2 == fiRpcChType[iRpcIndx] )
            sRpcChType += "    STRIPS ";
            else sRpcChType += "     UNDEF ";
      } // for( Int_t iOutVftxIndx = 0; iOutVftxIndx < fiNbOutVftx; iOutVftxIndx++ )
      LOG(info)<<sOutRpc;
      LOG(info)<<sRpcUId;
      LOG(info)<<sRpcChType;
   } // if( 0 < fiNbRpc )
      else LOG(info)<<"  Nb RPC  in output:                  NONE";  
   
   // Plastics
   if( 0 < fiNbPlastics )
   {
      LOG(info)<<"  Nb Plastics in output:              "<<fiNbPlastics;    
      
      TString sOutPla   = "Output Plastic Index: --- ";
      TString sPlaUId   = "Plastic unique ID:    |-> ";
      TString sPlaEndNb = "Plastic Ends Nb:      |-> ";
      for( Int_t iPlasticIndx = 0; iPlasticIndx < fiNbPlastics; iPlasticIndx++ )
      {
         sOutPla += Form( "%10d ", iPlasticIndx);
         sPlaUId += Form( "0x%08x ", (UInt_t)fiPlasticUniqueAddress[iPlasticIndx]);
         if( 1 == fiPlasticEndsNb[iPlasticIndx] )
            sPlaEndNb += "    Single ";
            else if( 2 == fiPlasticEndsNb[iPlasticIndx] )
            sPlaEndNb += "    Double ";
            else sPlaEndNb += "     UNDEF ";
      } // for( Int_t iOutVftxIndx = 0; iOutVftxIndx < fiNbOutVftx; iOutVftxIndx++ )
      LOG(info)<<sOutPla;
      LOG(info)<<sPlaUId;
      LOG(info)<<sPlaEndNb;
   } // if( 0 < fiNbPlastics )
      else LOG(info)<<"  Nb Plastics in output:              NONE";  
   
   return;
}
// ------------------------------------------------------------------ 
Int_t TMbsConvTofPar::GetScalerInd(    Int_t outScalerInd ) const
{
   if( outScalerInd < fiNbOutScalers && 0 < fiScalersIndex.GetSize()  )
      return fiScalersIndex[outScalerInd];
      else return -1;
}
Int_t TMbsConvTofPar::GetVftxInd(      Int_t outBoardInd ) const
{
   if( outBoardInd < fiNbOutVftx && 0 < fiVftxIndex.GetSize()  )
      return fiVftxIndex[outBoardInd];
      else return -1;
}
Int_t TMbsConvTofPar::GetVftxOutInd(   Int_t inputBoardInd ) const
{
   for( Int_t iOutInd = 0; iOutInd < fiNbOutVftx; iOutInd++)
      if( inputBoardInd == fiVftxIndex[iOutInd]  )
         return iOutInd;
         
   return -1;
}
Int_t TMbsConvTofPar::GetDetectorUid(  Int_t detInd ) const
{
   if( detInd < fiNbRpc && 0 < fiRpcUniqueAddress.GetSize() )
      return fiRpcUniqueAddress[detInd];
      else if( detInd - fiNbRpc < fiNbPlastics && 0 < fiPlasticUniqueAddress.GetSize() )
         return fiPlasticUniqueAddress[detInd - fiNbRpc];
      else return -1;
}
Int_t TMbsConvTofPar::GetDetectorInd(  Int_t channelAddress ) const
{
   Int_t iDetectorIndex = 0;
   // loop over all defined output rpc to find if the address matches one of them
   for( Int_t iRpcIndx = 0; iRpcIndx < fiNbRpc; iRpcIndx++ )
      if( kTRUE == CbmTofAddress::SameModule( channelAddress, fiRpcUniqueAddress[iRpcIndx] ) )
         return iDetectorIndex;
         else iDetectorIndex++;
   
   // loop over all defined output plastics to find if the address matches one of them
   for( Int_t iPlasticIndx = 0; iPlasticIndx < fiNbPlastics; iPlasticIndx++ )
      if( kTRUE == CbmTofAddress::SameModule( channelAddress, fiPlasticUniqueAddress[iPlasticIndx] ) )
         return iDetectorIndex;
         else iDetectorIndex++;
         
   // If none matching
   return -1;
}
Int_t TMbsConvTofPar::GetDetChTyp(     Int_t detInd ) const
{
   if( detInd < fiNbRpc && 0 < fiRpcUniqueAddress.GetSize() )
      return GetRpcChType(detInd);
      else if( detInd - fiNbRpc < fiNbPlastics && 0 < fiPlasticUniqueAddress.GetSize() )
         return GetPlasticEndNb(detInd - fiNbRpc);
      else return -1;
}
Int_t TMbsConvTofPar::GetRpcUid(       Int_t rpcInd ) const
{
   if( rpcInd < fiNbRpc && 0 < fiRpcUniqueAddress.GetSize() )
      return fiRpcUniqueAddress[rpcInd];
      else return -1;
}
Int_t TMbsConvTofPar::GetRpcInd(       Int_t channelAddress ) const
{
   // loop over all defined output rpc to find if the address matches one of them
   for( Int_t iRpcIndx = 0; iRpcIndx < fiNbRpc; iRpcIndx++ )
      if( kTRUE == CbmTofAddress::SameModule( channelAddress, fiRpcUniqueAddress[iRpcIndx] ) )
         return iRpcIndx;
   
   // If none matching
   return -1;
}
Int_t TMbsConvTofPar::GetRpcChType(    Int_t rpcInd ) const
{
   if( rpcInd < fiNbRpc && 0 < fiRpcChType.GetSize() )
      return fiRpcChType[rpcInd];
      else return -1;
}
Int_t TMbsConvTofPar::GetPlasticUid(   Int_t plasticInd ) const
{
   if( plasticInd < fiNbPlastics && 0 < fiPlasticUniqueAddress.GetSize() )
      return fiPlasticUniqueAddress[plasticInd];
      else return -1;
}
Int_t TMbsConvTofPar::GetPlasticInd(   Int_t channelAddress ) const
{
   // loop over all defined output plastics to find if the address matches one of them
   for( Int_t iPlasticIndx = 0; iPlasticIndx < fiNbPlastics; iPlasticIndx++ )
      if( kTRUE == CbmTofAddress::SameModule( channelAddress, fiPlasticUniqueAddress[iPlasticIndx] ) )
         return iPlasticIndx;
   
   // If none matching
   return -1;
}
Int_t TMbsConvTofPar::GetPlasticEndNb( Int_t plasticInd ) const
{
   if( plasticInd < fiNbPlastics && 0 < fiPlasticEndsNb.GetSize() )
      return fiPlasticEndsNb[plasticInd];
      else return -1;
}

