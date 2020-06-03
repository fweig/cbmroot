#ifndef CBMRICHTRBPARAM_H
#define CBMRICHTRBPARAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "FairLogger.h"

#include "CbmRichHitInfo.h"
#include "CbmRichTrbDefines.h"

using namespace std;

class CbmRichTrbParam
{
private:

   CbmRichTrbParam() : fMap() {
      ReadMap();
   }

public:

   static CbmRichTrbParam* Instance() {
      static CbmRichTrbParam fInstance;
      return &fInstance;
   }

   ~CbmRichTrbParam() {
   }

   CbmRichHitInfo* GetRichHitInfo(UInt_t tdcId, UInt_t leadingChannel) {
      return fMap[tdcId][leadingChannel];
   }

   Bool_t IsPmtTrb(UInt_t trbId) const {
      return  (trbId==0x8015 || trbId==0x8025 || trbId==0x8035 || trbId==0x8045 ||
      trbId==0x8055 || trbId==0x8065 || trbId==0x8075 || trbId==0x8085 ||
      trbId==0x8095 || trbId==0x80a5 || trbId==0x80b5 || trbId==0x80c5 ||
      trbId==0x80d5 || trbId==0x80e5 || trbId==0x80f5 || trbId==0x8105 || trbId == 0x8115);
   }

   Bool_t IsPmtTdc(UInt_t tdcId) const {
      return ( ((tdcId & 0x000f) <= 3) && ((tdcId >> 8) == 0) ) ||
      (tdcId == 0x0100 || tdcId == 0x0101 || tdcId == 0x0102 || tdcId == 0x0103);
   }

   UInt_t TDCidToInteger (UInt_t tdcId) const {
      return ((((tdcId >> 4) & 0x00ff) - 1) * 4 + (tdcId & 0x000f));
   }

   UInt_t IntegerToTDCid (UInt_t index) const {
      return (((index/4) + 1) << 4) + (index%4);
   }

   // TDC actually has 33 channels with 0-th being a special channel for SYNC messages.
   // I intentionally put TDC after TDC with step of 32 channels - channels from 1 to 32 (incl.)
   // and then a section for 64 sync channels - one for each of 64 TDCs
   UInt_t TDCandCHtoInteger(UInt_t tdcId, UInt_t ch) const {
      if (ch == 0) {
         return 64*32 + this->TDCidToInteger(tdcId);
      } else {
         return this->TDCidToInteger(tdcId) * 32 + ch-1;
      }
   }

   Bool_t IsSyncChannel(UInt_t ch/*, UInt_t tdcId=0xffff*/) const {
      if (ch==0) return kTRUE;
      return kFALSE;
   }

   Bool_t IsLeadingEdgeChannel(UInt_t ch/*, UInt_t tdcId=0xffff*/) const {
      #ifdef INVERSEPOLARITY
      if (ch%2==0) return kTRUE;
      #else
      if (ch%2==1) return kTRUE;
      #endif
      // if (tdcId==.....) {
      // }
      return kFALSE;
   }

   Bool_t isStudiedTDC(Int_t tdcID)
   {
      if (tdcID == 0x0010 || tdcID == 0x0011 || tdcID == 0x0012 || tdcID == 0x0013 ||
          tdcID == 0x0020 || tdcID == 0x0021 || tdcID == 0x0022 || tdcID == 0x0023 ||
          tdcID == 0x0050 || tdcID == 0x0051 || tdcID == 0x0052 || tdcID == 0x0053 ||
          tdcID == 0x0060 || tdcID == 0x0061 || tdcID == 0x0062 || tdcID == 0x0063) {
         return kTRUE;
      } else {
         return kFALSE;
      }
   }

   Bool_t isNormalPixel_WLS_off(Int_t hitTDCid1, Int_t hitChannel1)
   {
      if ((hitTDCid1==0x0010 && hitChannel1/2==4) ||
          (hitTDCid1==0x0011 && hitChannel1/2==0) ||
          (hitTDCid1==0x0012 && hitChannel1/2==0) ||
          (hitTDCid1==0x0012 && hitChannel1/2==1) ||
          (hitTDCid1==0x0012 && hitChannel1/2==7) ||
          (hitTDCid1==0x0012 && hitChannel1/2==8) ||
          (hitTDCid1==0x0013 && hitChannel1/2==3) ||
          (hitTDCid1==0x0013 && hitChannel1/2==4) ||
          (hitTDCid1==0x0013 && hitChannel1/2==10) ||

          (hitTDCid1==0x0020 && hitChannel1/2==10) ||
          (hitTDCid1==0x0020 && hitChannel1/2==14) ||
          (hitTDCid1==0x0022 && hitChannel1/2==0) ||
          (hitTDCid1==0x0023 && hitChannel1/2==14) ||

          (hitTDCid1==0x0052 && hitChannel1/2==1) ||
          (hitTDCid1==0x0060 && hitChannel1/2==1) ||
          (hitTDCid1==0x0061 && hitChannel1/2==4) ||
          (hitTDCid1==0x0061 && hitChannel1/2==8) ||
          (hitTDCid1==0x0061 && hitChannel1/2==7) ) {
         return kFALSE;
      } else {
         return kTRUE;
      }
   }

   Bool_t isNormalPixel_WLS_on(Int_t hitTDCid1, Int_t hitChannel1)
   {
      if ( hitTDCid1==0x0050 || hitTDCid1==0x0051 || hitTDCid1==0x0052 || hitTDCid1==0x0053 ||
           hitTDCid1==0x0060 || hitTDCid1==0x0061 || hitTDCid1==0x0062 || hitTDCid1==0x0063 ||

           (hitTDCid1==0x0010 && hitChannel1/2==4) ||

           (hitTDCid1==0x0011 && hitChannel1/2==0) ||
           (hitTDCid1==0x0011 && hitChannel1/2==4) ||

           (hitTDCid1==0x0013 && hitChannel1/2==3) ||
           (hitTDCid1==0x0013 && hitChannel1/2==6) ||
           (hitTDCid1==0x0013 && hitChannel1/2==8) ||
           (hitTDCid1==0x0013 && hitChannel1/2==9) ||
           (hitTDCid1==0x0013 && hitChannel1/2==10) ||
           (hitTDCid1==0x0013 && hitChannel1/2==11) ||
           (hitTDCid1==0x0013 && hitChannel1/2==13) ||

           (hitTDCid1==0x0020 && hitChannel1/2==0) ||
           (hitTDCid1==0x0020 && hitChannel1/2==4) ||
           (hitTDCid1==0x0020 && hitChannel1/2==7) ||
           (hitTDCid1==0x0020 && hitChannel1/2==10) ||
           (hitTDCid1==0x0020 && hitChannel1/2==14) ||

           (hitTDCid1==0x0021 && hitChannel1/2==0) ||
           (hitTDCid1==0x0021 && hitChannel1/2==5) ||
           (hitTDCid1==0x0021 && hitChannel1/2==7) ||
           (hitTDCid1==0x0021 && hitChannel1/2==8) ||
           (hitTDCid1==0x0021 && hitChannel1/2==14) ||
           (hitTDCid1==0x0021 && hitChannel1/2==15) ||

           (hitTDCid1==0x0022 && hitChannel1/2==0) ||
           (hitTDCid1==0x0022 && hitChannel1/2==4) ||

           (hitTDCid1==0x0023 && hitChannel1/2==14) ) {
         return kFALSE;
      } else {
         return kTRUE;
      }
   }

   Bool_t isNormalPixel(Int_t hitTDCid1, Int_t hitChannel1)
   {
      return (this->isNormalPixel_WLS_off(hitTDCid1, hitChannel1) && this->isNormalPixel_WLS_on(hitTDCid1, hitChannel1));
   }

   UInt_t GetCorrespondingLeadingEdgeChannel(UInt_t ch, UInt_t /*tdcId=0xffff*/) const
   {
      /*if (tdcId == 0x0110) {
         return ch-1;
      } else if (tdcId == 0x0111) {
         if (ch == 2) return 1;
         if (ch == 4) return 1;
         if (ch == 6) return 5;
         if (ch == 8) return 5;
         if (ch == 16) return 13;
         if (ch == 14) return 13;
      } else if (tdcId == 0x0112) {
         return ch-1;
      } else if (tdcId == 0x0113) {
         if (ch == 12) return 9;
         if (ch == 10) return 9;
      } else */{

         #ifdef INVERSEPOLARITY
         return ch+1;
         #else // standard polarity
         return ch-1;
         #endif
      }
   }

   Bool_t IsReferenceTimeTdc(UInt_t tdcId) {
      return (tdcId == 0x0110);
   }

private:

   map<UInt_t, map<UInt_t, CbmRichHitInfo*> > fMap;

   void ReadMap()
   {
      string line;
      ifstream myfile ("pixel-coord_channel-register.ascii");
      if (myfile.is_open()) {
         getline (myfile,line);
         getline (myfile,line);

         while ( !myfile.eof() ){
            UInt_t pmtNum, pixelNum;
            Double_t xmm, ymm;
            UInt_t simpleX, simpleY, tdcId, chLeadingEdge, chTrailingEdge, padiwaNum, trbNum;
            string pmtType;
            myfile >> pmtNum >> pixelNum >> xmm >> ymm >> simpleX >> simpleY;
            myfile >> std::hex >> tdcId;
            myfile >> std::dec >> chLeadingEdge >> chTrailingEdge >> padiwaNum >> trbNum >> pmtType;
            LOG(debug4) << pmtNum << " " <<  pixelNum << " " << xmm << " " << ymm << " "
               << simpleX << " " << simpleY << " " << tdcId << " " << chLeadingEdge << " " << chTrailingEdge
               << " " << padiwaNum << " " << trbNum << " " << pmtType;
            UInt_t pmtTypeId = GetPmtTypeIdByString(pmtType);
            fMap[tdcId][chLeadingEdge] = new CbmRichHitInfo(pmtNum, pixelNum, xmm / 10., ymm / 10., simpleX, simpleY, tdcId, chLeadingEdge, chTrailingEdge, padiwaNum, trbNum, pmtTypeId);
         }
         myfile.close();
         LOG(info) << "Imported pixel map from pixel-coord_channel-register.ascii";
      } else {
         LOG(fatal) << "[CbmRichTrbParam::ReadMap] Failed to open ASCII map file.";
      }
   }

   UInt_t GetPmtTypeIdByString(const string& str) {
      if ( str == "H12700") return 1;
      else if (str == "H8500") return 2;
      else return -1;
   }

};

#endif // CBMRICHTRBPARAM_H
