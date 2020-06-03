// ------------------------------------------------------------------
// -----                     TTofCalibData                       -----
// -----              Created 17/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------

#ifndef TTOFCALIBDATA_H_
#define TTOFCALIBDATA_H_

#include "TObject.h"

class TTofCalibData : public TObject
{
   public:
      TTofCalibData();
      TTofCalibData( UInt_t uType, UInt_t uBoard, UInt_t uChan,
                     Double_t dCalibT,  Double_t dTot = 0,
                     UInt_t uEdge = 0 );
      ~TTofCalibData();
      
      virtual void Clear(Option_t *option = "");
                   
      // copy constructor, used by vector to copy content
      TTofCalibData(const TTofCalibData& src) :
         TObject(src),
         fbFilled(src.fbFilled),
         fuTdcType(src.fuTdcType),
         fuTdcBoard(src.fuTdcBoard),
         fuTdcChannel(src.fuTdcChannel), 
         fuEdge(src.fuEdge), 
         fdCalibTime(src.fdCalibTime), 
         fdTimeOverThreshold(src.fdTimeOverThreshold) {};
      // Comparison operator, needed for time sorting
//      virtual Bool_t operator <( const TTofCalibData& rhs) const; 
      virtual Int_t	Compare(      const TObject* obj) const;
      virtual Int_t	Compare(      const TTofCalibData*  obj) const;
      virtual Bool_t	IsSortable() const { return kTRUE; };
                   
      void SetData( UInt_t uType, UInt_t uBoard, UInt_t uChan, 
                     Double_t dCalibT,  Double_t dTot = 0,
                     UInt_t uEdge = 0 );
      void SetFilled(){ fbFilled = kTRUE; };
      void ClearFilled(){ fbFilled = kFALSE; };
      void SetType( UInt_t uType ){ fuTdcType = uType; };
      void SetBoard( UInt_t uBoard ){ fuTdcBoard = uBoard; };
      void SetChannel( UInt_t uChan ){ fuTdcChannel = uChan; };
      void SetEdge( UInt_t uEdge ){ fuEdge = uEdge; };
      void SetTime( Double_t dCalibT ){ fdCalibTime = dCalibT; };
      void SetTot(  Double_t dTot ){ fdTimeOverThreshold = dTot; };
      
      Bool_t   IsFilled()   const { return fbFilled; };
      UInt_t   GetType()    const { return fuTdcType; };
      UInt_t   GetBoard()   const { return fuTdcBoard; };
      UInt_t   GetChannel() const { return fuTdcChannel; };
      UInt_t   GetEdge()    const { return fuEdge; }; // 0 = Rising, 1 = Falling, 2 = Full, 3 = Ref ch
      Double_t GetTime()    const { return fdCalibTime; };
      Double_t GetTot()     const { return fdTimeOverThreshold; };
      
      Double_t GetTimeDifference( const Double_t      & dTimeIn );
      Double_t GetTimeDifference( const TTofCalibData & dataIn );
      
   private:
      Bool_t   fbFilled;
      UInt_t   fuTdcType;
      UInt_t   fuTdcBoard;
      UInt_t   fuTdcChannel;
      UInt_t   fuEdge; // 0 = Rising, 1 = Falling, 2 = Full hit, 3 = Ref ch
      Double_t fdCalibTime;         // [ps]
      Double_t fdTimeOverThreshold; // [ps]
      
   ClassDef(TTofCalibData, 1)
};

#endif // TTOFCALIBDATA_H_  
