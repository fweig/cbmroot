// -------------------------------------------------------------------------
// -----            CbmCern2017UnpackParSts source file                -----
// -----            Created 25/07/17  by P.-A. Loizeau                 -----
// -------------------------------------------------------------------------

#include "CbmCern2017UnpackParSts.h"

#include "FairParamList.h"
#include "FairDetParIo.h"
#include "FairParIo.h"
#include "FairLogger.h"

#include "TString.h"

using namespace std;
// -----   Standard constructor   ------------------------------------------
CbmCern2017UnpackParSts::CbmCern2017UnpackParSts(const char* name,
                                                 const char* title,
                                                 const char* context) :
   FairParGenericSet(name, title, context),
   fuNrOfDpbs( 0 ),
   fiDbpIdArray(),
   fuNbElinksPerDpb( 0 ),
   fuNbStsXyters( 0 ),
   fuNbChanPerAsic( 0 ),
   fiElinkToAsicMap(),
   fuAsicIdxSts1N( 0 ),
   fuAsicIdxSts1P( 0 ),
   fuAsicIdxSts2N( 0 ),
   fuAsicIdxSts2P( 0 )
{
  detName="Much";
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
CbmCern2017UnpackParSts::~CbmCern2017UnpackParSts()
{
}
// -------------------------------------------------------------------------


// -----   Public method clear   -------------------------------------------
void CbmCern2017UnpackParSts::clear()
{
   status = kFALSE;
   resetInputVersions();
}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------

void CbmCern2017UnpackParSts::putParams(FairParamList* l)
{
   if (!l) return;

#ifdef VERSION_GREATER_160601
   l->add("NrOfDpbs",       fuNrOfDpbs );
   l->add("DbpIdArray",     fiDbpIdArray);
   l->add("NbElinksPerDpb", fuNbElinksPerDpb );
   l->add("NbStsXyters",    fuNbStsXyters );
   l->add("NbChanPerAsic",  fuNbChanPerAsic );
   l->add("ElinkToAsicMap", fiElinkToAsicMap);

   l->add("AsicIdxSts1N", fuAsicIdxSts1N);
   l->add("AsicIdxSts1P", fuAsicIdxSts1P);
   l->add("AsicIdxSts2N", fuAsicIdxSts2N);
   l->add("AsicIdxSts2P", fuAsicIdxSts2P);
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

   Int_t iAsicIdxSts1N = fuAsicIdxSts1N;
   Int_t iAsicIdxSts1P = fuAsicIdxSts1P;
   Int_t iAsicIdxSts2N = fuAsicIdxSts2N;
   Int_t iAsicIdxSts2P = fuAsicIdxSts2P;
   l->add("AsicIdxSts1N", iAsicIdxSts1N);
   l->add("AsicIdxSts1P", iAsicIdxSts1P);
   l->add("AsicIdxSts2N", iAsicIdxSts2N);
   l->add("AsicIdxSts2P", iAsicIdxSts2P);
#endif // VERSION_GREATER_160601
}

// -------------------------------------------------------------------------

Bool_t CbmCern2017UnpackParSts::getParams(FairParamList* l) {

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

   if ( ! l->fill("AsicIdxSts1N", &fuAsicIdxSts1N ) ) return kFALSE;
   if ( ! l->fill("AsicIdxSts1P", &fuAsicIdxSts1P ) ) return kFALSE;
   if ( ! l->fill("AsicIdxSts2N", &fuAsicIdxSts2N ) ) return kFALSE;
   if ( ! l->fill("AsicIdxSts2P", &fuAsicIdxSts2P ) ) return kFALSE;
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

   Int_t iAsicIdxSts1N = 0;
   Int_t iAsicIdxSts1P = 0;
   Int_t iAsicIdxSts2N = 0;
   Int_t iAsicIdxSts2P = 0;
   if ( ! l->fill("AsicIdxSts1N", &iAsicIdxSts1N ) ) return kFALSE;
   if ( ! l->fill("AsicIdxSts1P", &iAsicIdxSts1P ) ) return kFALSE;
   if ( ! l->fill("AsicIdxSts2N", &iAsicIdxSts2N ) ) return kFALSE;
   if ( ! l->fill("AsicIdxSts2P", &iAsicIdxSts2P ) ) return kFALSE;
   fuAsicIdxSts1N = iAsicIdxSts1N;
   fuAsicIdxSts1P = iAsicIdxSts1P;
   fuAsicIdxSts2N = iAsicIdxSts2N;
   fuAsicIdxSts2P = iAsicIdxSts2P;
#endif // VERSION_GREATER_160601

  return kTRUE;
}
// -------------------------------------------------------------------------
UInt_t CbmCern2017UnpackParSts::GetDpbId( UInt_t uDpbIdx )
{
   if( uDpbIdx < fuNrOfDpbs )
      return fiDbpIdArray[ uDpbIdx ];
      else
      {
         LOG(warning) << "CbmCern2017UnpackParSts::GetDpbId => Index out of bound, "
                      << "returning crazy value!";
         return 0xFFFFFFFF;
      } // else of if( uDpbIdx < fuNrOfDpbs )
}
UInt_t CbmCern2017UnpackParSts::GetElinkToAsicIdx( UInt_t uElinkIdx )
{
   if( uElinkIdx < fuNrOfDpbs * fuNbElinksPerDpb )
      return fiElinkToAsicMap[ uElinkIdx ];
      else
      {
         LOG(warning) << "CbmCern2017UnpackParSts::GetElinkToAsicIdx => Index out of bound, "
                      << "returning crazy value!";
         return (fuNbStsXyters + 1);
      } // else of if( uDpbIdx < fuNrOfDpbs )
}

ClassImp(CbmCern2017UnpackParSts)
