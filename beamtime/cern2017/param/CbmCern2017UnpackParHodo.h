// -------------------------------------------------------------------------
// -----            CbmCern2017UnpackParHodo header file               -----
// -----            Created 25/07/17  by P.-A. Loizeau                 -----
// -------------------------------------------------------------------------

#ifndef CBMCERN2017UNPACKPARHODO_H
#define CBMCERN2017UNPACKPARHODO_H

#include "FairParGenericSet.h"

#include "TArrayI.h"

class FairParIo;
class FairParamList;


class CbmCern2017UnpackParHodo : public FairParGenericSet
{

 public:

   /** Standard constructor **/
   CbmCern2017UnpackParHodo(const char* name    = "CbmCern2017UnpackParHodo",
                         const char* title   = "Hodo unpacker parameters",
                         const char* context = "Default");


   /** Destructor **/
   virtual ~CbmCern2017UnpackParHodo();

   /** Reset all parameters **/
   virtual void clear();

   void putParams(FairParamList*);
   Bool_t getParams(FairParamList*);

   UInt_t GetNrOfDpbs()       { return fuNrOfDpbs; }
   UInt_t GetDpbId( UInt_t uDpbIdx );
   UInt_t GetNbElinksPerDpb() { return fuNbElinksPerDpb; }
   UInt_t GetNbStsXyters()    { return fuNbStsXyters; }
   UInt_t GetNbChanPerAsic()  { return fuNbChanPerAsic; }
   UInt_t GetElinkToAsicIdx( UInt_t uElinkIdx );

   Int_t GetChannelToFiberMap(UInt_t channel);
   Int_t GetChannelToPixelMap(UInt_t channel);
   Int_t GetChannelToPlaneMap(UInt_t channel);

   UInt_t GetAsicIndexHodo1() { return fuAsicIdxHodo1; }
   UInt_t GetAsicIndexHodo2() { return fuAsicIdxHodo2; }

   Bool_t IsXySwappedHodo1() { return fbSwapHodo1XY; }
   Bool_t IsXySwappedHodo2() { return fbSwapHodo2XY; }
   Bool_t IsXInvertedHodo1() { return fbInvertHodo1X; }
   Bool_t IsYInvertedHodo1() { return fbInvertHodo1Y; }
   Bool_t IsXInvertedHodo2() { return fbInvertHodo1X; }
   Bool_t IsYInvertedHodo2() { return fbInvertHodo2Y; }

   Bool_t GetChannelToPlaneMapHodo1(UInt_t channel);
   Bool_t GetChannelToPlaneMapHodo2(UInt_t channel);

 private:

   UInt_t  fuNrOfDpbs;       // Total number of STS DPBs in system
   TArrayI fiDbpIdArray;     // Array to hold the unique IDs (equipment ID) for all STS DPBs
   UInt_t  fuNbElinksPerDpb; // Number of elinks connected to each DPB
   UInt_t  fuNbStsXyters;    // Number of StsXyter ASICs
   UInt_t  fuNbChanPerAsic;  // Number of channels per StsXyter ASIC
   TArrayI fiElinkToAsicMap; // Array to hold for each link the corresponding ASIC index [fuNrOfDpbs * fuNbElinksPerDpb]

   TArrayI fiChannelToFiberMap; // Array which stores the corresponding fiber for each channel
   TArrayI fiChannelToPixelMap; // Array which stores the corresponding pixel for each channel
   TArrayI fiChannelToPlaneMap; // Array which stores the corresponding side for each channel

   UInt_t  fuAsicIdxHodo1;   // Index of StsXyter ASIC for Hodoscope 1
   UInt_t  fuAsicIdxHodo2;   // Index of StsXyter ASIC for Hodoscope 2
   Bool_t  fbSwapHodo1XY;    // Flag indicating wether the X and Y axis of Hodoscope 1 are swapped
   Bool_t  fbSwapHodo2XY;    // Flag indicating wether the X and Y axis of Hodoscope 2 are swapped
   Bool_t  fbInvertHodo1X;   // Flag indicating wether the X axis of Hodoscope 1 is inverted
   Bool_t  fbInvertHodo1Y;   // Flag indicating wether the Y axis of Hodoscope 1 is inverted
   Bool_t  fbInvertHodo2X;   // Flag indicating wether the X axis of Hodoscope 2 is inverted
   Bool_t  fbInvertHodo2Y;   // Flag indicating wether the Y axis of Hodoscope 2 is inverted

  ClassDef(CbmCern2017UnpackParHodo,1);
};
#endif // CBMCERN2017UNPACKPARHODO_H
