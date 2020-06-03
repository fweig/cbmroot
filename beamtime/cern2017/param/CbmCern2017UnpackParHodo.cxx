// -------------------------------------------------------------------------
// -----            CbmCern2017UnpackParHodo source file                -----
// -----            Created 25/07/17  by P.-A. Loizeau                 -----
// -------------------------------------------------------------------------

#include "CbmCern2017UnpackParHodo.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairLogger.h"

#include "TString.h"

using namespace std;
// -----   Standard constructor   ------------------------------------------
CbmCern2017UnpackParHodo::CbmCern2017UnpackParHodo(const char* name,
                                                 const char* title,
                                                 const char* context) :
   FairParGenericSet(name, title, context),
   fuNrOfDpbs( 0 ),
   fiDbpIdArray(),
   fuNbElinksPerDpb( 0 ),
   fuNbStsXyters( 0 ),
   fuNbChanPerAsic( 0 ),
   fiElinkToAsicMap(),
	fiChannelToFiberMap(),
	fiChannelToPixelMap(),
	fiChannelToPlaneMap(),
   fuAsicIdxHodo1( 0 ),
   fuAsicIdxHodo2( 0 ),
   fbSwapHodo1XY( kFALSE ),
   fbSwapHodo2XY( kFALSE ),
   fbInvertHodo1X( kFALSE ),
   fbInvertHodo1Y( kFALSE ),
   fbInvertHodo2X( kFALSE ),
   fbInvertHodo2Y( kFALSE )
{
  detName="Hodo";
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
CbmCern2017UnpackParHodo::~CbmCern2017UnpackParHodo()
{
}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmCern2017UnpackParHodo::clear()
{
   status = kFALSE;
   resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmCern2017UnpackParHodo::putParams(FairParamList* l)
{
   if (!l) return;

#ifdef VERSION_GREATER_160601
   l->add("NrOfDpbs",       fuNrOfDpbs );
   l->add("DbpIdArray",     fiDbpIdArray);
   l->add("NbElinksPerDpb", fuNbElinksPerDpb );
   l->add("NbStsXyters",    fuNbStsXyters );
   l->add("NbChanPerAsic",  fuNbChanPerAsic );
   l->add("ElinkToAsicMap", fiElinkToAsicMap);

   l->add("ChannelToFiberMap", fiChannelToFiberMap);
   l->add("ChannelToPixelMap", fiChannelToPixelMap);
   l->add("ChannelToPlaneMap", fiChannelToPlaneMap);

   l->add("AsicIdxHodo1", fuAsicIdxHodo1);
   l->add("AsicIdxHodo2", fuAsicIdxHodo2);
   l->add("SwapHodo1XY",  fbSwapHodo1XY);
   l->add("SwapHodo2XY",  fbSwapHodo2XY);
   l->add("InvertHodo1X", fbInvertHodo1X);
   l->add("InvertHodo1Y", fbInvertHodo1Y);
   l->add("InvertHodo2X", fbInvertHodo2X);
   l->add("InvertHodo2Y", fbInvertHodo2Y);

#else
   Int_t iNrOfDpbs       = fuNrOfDpbs;
   Int_t iNbElinksPerDpb = fuNbElinksPerDpb;
   Int_t iNbStsXyters    = fuNbStsXyters;
   Int_t iNbChanPerAsic  = fuNbChanPerAsic;

   l->add("NrOfDpbs",       iNrOfDpbs );
   l->add("DbpIdArray",     fiDbpIdArray);
   l->add("NbElinksPerDpb", iNbElinksPerDpb );
   l->add("NbStsXyters",    iNbStsXyters );
   l->add("NbChanPerAsic",  iNbChanPerAsic );
   l->add("ElinkToAsicMap", fiElinkToAsicMap);

   l->add("ChannelToFiberMap", fiChannelToFiberMap);
   l->add("ChannelToPixelMap", fiChannelToPixelMap);
   l->add("ChannelToPlaneMap", fiChannelToPlaneMap);

   Int_t iAsicIdxHodo1 = fuAsicIdxHodo1;
   Int_t iAsicIdxHodo2 = fuAsicIdxHodo2;
   l->add("AsicIdxHodo1", iAsicIdxHodo1);
   l->add("AsicIdxHodo2", iAsicIdxHodo2);
   l->add("SwapHodo1XY",  fbSwapHodo1XY);
   l->add("SwapHodo2XY",  fbSwapHodo2XY);
   l->add("InvertHodo1X", fbInvertHodo1X);
   l->add("InvertHodo1Y", fbInvertHodo1Y);
   l->add("InvertHodo2X", fbInvertHodo2X);
   l->add("InvertHodo2Y", fbInvertHodo2Y);
#endif // VERSION_GREATER_160601
}

// -------------------------------------------------------------------------

Bool_t CbmCern2017UnpackParHodo::getParams(FairParamList* l) {

   if (!l) return kFALSE;

#ifdef VERSION_GREATER_160601
   if ( ! l->fill("NrOfDpbs", &fuNrOfDpbs ) ) return kFALSE;

   fiDbpIdArray.Set( fuNrOfDpbs );
   if ( ! l->fill("DbpIdArray", &fiDbpIdArray ) ) return kFALSE;

   if ( ! l->fill("NbElinksPerDpb", &fuNbElinksPerDpb ) ) return kFALSE;

   if ( ! l->fill("NbStsXyters", &fuNbStsXyters ) ) return kFALSE;

   if ( ! l->fill("NbChanPerAsic", &fuNbChanPerAsic ) ) return kFALSE;

   fiElinkToAsicMap.Set( fuNrOfDpbs * fuNbElinksPerDpb );
   if ( ! l->fill("ElinkToAsicMap", &fiElinkToAsicMap ) ) return kFALSE;

   fiChannelToFiberMap.Set(fuNbChanPerAsic);
   fiChannelToPixelMap.Set(fuNbChanPerAsic);
   fiChannelToPlaneMap.Set(fuNbChanPerAsic);
   if ( ! l->fill("ChannelToFiberMap", &fiChannelToFiberMap) ) return kFALSE;
   if ( ! l->fill("ChannelToPixelMap", &fiChannelToPixelMap) ) return kFALSE;
   if ( ! l->fill("ChannelToPlaneMap", &fiChannelToPlaneMap) ) return kFALSE;

   if ( ! l->fill("AsicIdxHodo1", &fuAsicIdxHodo1 ) ) return kFALSE;
   if ( ! l->fill("AsicIdxHodo2", &fuAsicIdxHodo2 ) ) return kFALSE;
   Int_t iSwapHodo1XY = 0;
   Int_t iSwapHodo2XY = 0;
   Int_t iInvertHodo1X = 0;
   Int_t iInvertHodo1Y = 0;
   Int_t iInvertHodo2X = 0;
   Int_t iInvertHodo2Y = 0;
   if ( ! l->fill("SwapHodo1XY",  &iSwapHodo1XY ) ) return kFALSE;
   if ( ! l->fill("SwapHodo2XY",  &iSwapHodo2XY ) ) return kFALSE;
   if ( ! l->fill("InvertHodo1X", &iInvertHodo1X ) ) return kFALSE;
   if ( ! l->fill("InvertHodo1Y", &iInvertHodo1Y ) ) return kFALSE;
   if ( ! l->fill("InvertHodo2X", &iInvertHodo2X ) ) return kFALSE;
   if ( ! l->fill("InvertHodo2Y", &iInvertHodo2Y ) ) return kFALSE;
   fbSwapHodo1XY  = iSwapHodo1XY;
   fbSwapHodo2XY  = iSwapHodo2XY;
   fbInvertHodo1X = iInvertHodo1X;
   fbInvertHodo1Y = iInvertHodo1Y;
   fbInvertHodo2X = iInvertHodo2X;
   fbInvertHodo2Y = iInvertHodo2Y;
#else
   Int_t iNrOfDpbs       = 0;
   Int_t iNbElinksPerDpb = 0;
   Int_t iNbStsXyters    = 0;
   Int_t iNbChanPerAsic  = 0;

   if ( ! l->fill("NrOfDpbs", &iNrOfDpbs ) ) return kFALSE;
   fuNrOfDpbs = iNrOfDpbs;

   fiDbpIdArray.Set( fuNrOfDpbs );
   if ( ! l->fill("DbpIdArray", &fiDbpIdArray ) ) return kFALSE;

   if ( ! l->fill("NbElinksPerDpb", &iNbElinksPerDpb ) ) return kFALSE;
   fuNbElinksPerDpb = iNbElinksPerDpb;

   if ( ! l->fill("NbStsXyters", &iNbStsXyters ) ) return kFALSE;
   fuNbStsXyters = iNbStsXyters;

   if ( ! l->fill("NbChanPerAsic", &iNbChanPerAsic ) ) return kFALSE;
   fuNbChanPerAsic = iNbChanPerAsic;

   fiElinkToAsicMap.Set( fuNrOfDpbs * fuNbElinksPerDpb );
   if ( ! l->fill("ElinkToAsicMap", &fiElinkToAsicMap ) ) return kFALSE;

   fiChannelToFiberMap.Set(fuNbChanPerAsic);
   fiChannelToPixelMap.Set(fuNbChanPerAsic);
   fiChannelToPlaneMap.Set(fuNbChanPerAsic);
   if ( ! l->fill("ChannelToFiberMap", &fiChannelToFiberMap) ) return kFALSE;
   if ( ! l->fill("ChannelToPixelMap", &fiChannelToPixelMap) ) return kFALSE;
   if ( ! l->fill("ChannelToPlaneMap", &fiChannelToPlaneMap) ) return kFALSE;

   Int_t iAsicIdxHodo1 = 0;
   Int_t iAsicIdxHodo2 = 0;
   Int_t iSwapHodo1XY = 0;
   Int_t iSwapHodo2XY = 0;
   Int_t iInvertHodo1X = 0;
   Int_t iInvertHodo1Y = 0;
   Int_t iInvertHodo2X = 0;
   Int_t iInvertHodo2Y = 0;
   if ( ! l->fill("AsicIdxHodo1", &iAsicIdxHodo1 ) ) return kFALSE;
   if ( ! l->fill("AsicIdxHodo2", &iAsicIdxHodo2 ) ) return kFALSE;
   if ( ! l->fill("SwapHodo1XY",  &iSwapHodo1XY ) ) return kFALSE;
   if ( ! l->fill("SwapHodo2XY",  &iSwapHodo2XY ) ) return kFALSE;
   if ( ! l->fill("InvertHodo1X", &iInvertHodo1X ) ) return kFALSE;
   if ( ! l->fill("InvertHodo1Y", &iInvertHodo1Y ) ) return kFALSE;
   if ( ! l->fill("InvertHodo2X", &iInvertHodo2X ) ) return kFALSE;
   if ( ! l->fill("InvertHodo2Y", &iInvertHodo2Y ) ) return kFALSE;
   fuAsicIdxHodo1 = iAsicIdxHodo1;
   fuAsicIdxHodo2 = iAsicIdxHodo2;
   fbSwapHodo1XY  = iSwapHodo1XY;
   fbSwapHodo2XY  = iSwapHodo2XY;
   fbInvertHodo1X = iInvertHodo1X;
   fbInvertHodo1Y = iInvertHodo1Y;
   fbInvertHodo2X = iInvertHodo2X;
   fbInvertHodo2Y = iInvertHodo2Y;

#endif // VERSION_GREATER_160601

  return kTRUE;
}
// -------------------------------------------------------------------------
UInt_t CbmCern2017UnpackParHodo::GetDpbId( UInt_t uDpbIdx )
{
   if( uDpbIdx < fuNrOfDpbs )
      return fiDbpIdArray[ uDpbIdx ];
      else
      {
         LOG(warning) << "CbmCern2017UnpackParHodo::GetDpbId => Index out of bound, "
                      << "returning crazy value!";
         return 0xFFFFFFFF;
      } // else of if( uDpbIdx < fuNrOfDpbs )
}
UInt_t CbmCern2017UnpackParHodo::GetElinkToAsicIdx( UInt_t uElinkIdx )
{
   if( uElinkIdx < fuNrOfDpbs * fuNbElinksPerDpb )
      return fiElinkToAsicMap[ uElinkIdx ];
      else
      {
         LOG(warning) << "CbmCern2017UnpackParHodo::GetElinkToAsicIdx => Index out of bound, "
                      << "returning crazy value!";
         return (fuNbStsXyters + 1);
      } // else of if( uDpbIdx < fuNrOfDpbs )
}
// -------------------------------------------------------------------------
Int_t CbmCern2017UnpackParHodo::GetChannelToFiberMap(UInt_t channel)
{
   if( channel < fuNbChanPerAsic )
      return fiChannelToFiberMap[channel];
      else return -1;
}
Int_t CbmCern2017UnpackParHodo::GetChannelToPixelMap(UInt_t channel)
{
   if( channel < fuNbChanPerAsic )
      return fiChannelToPixelMap[channel];
      else return -1;
}
Int_t CbmCern2017UnpackParHodo::GetChannelToPlaneMap(UInt_t channel)
{
   if( channel < fuNbChanPerAsic )
      return fiChannelToPlaneMap[channel];
      else return -1;
}
Bool_t CbmCern2017UnpackParHodo::GetChannelToPlaneMapHodo1(UInt_t channel)
{
   // Should throw error when invalid channel leading to -1!
   Bool_t bChanInY = (1 == GetChannelToPlaneMap( channel ) );
   if( fbSwapHodo1XY )
      return !bChanInY;
      else return bChanInY;
}
Bool_t CbmCern2017UnpackParHodo::GetChannelToPlaneMapHodo2(UInt_t channel)
{
   // Should throw error when invalid channel leading to -1!
   Bool_t bChanInY = (1 == GetChannelToPlaneMap( channel ) );
   if( fbSwapHodo2XY )
      return !bChanInY;
      else return bChanInY;
}

ClassImp(CbmCern2017UnpackParHodo)
