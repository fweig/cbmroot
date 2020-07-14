// -------------------------------------------------------------------------
// -----                   CbmTofDetectorId source file                -----
// -----                  Created 20/11/12  by F. Uhlig                -----
// -------------------------------------------------------------------------


#include "CbmTofDetectorId_v20a.h"

const Int_t CbmTofDetectorId_v20a::shiftarray[] = {0,4,11,15,21,25,26};
const Int_t CbmTofDetectorId_v20a::bitarray[]   = {4,7, 4, 6, 4, 1, 6};


CbmTofDetectorId_v20a::CbmTofDetectorId_v20a()
  : CbmTofDetectorId(),
    result_array(),
    maskarray(),
    modulemask(0)
{
  for (Int_t i=0; i < array_length; i++) {
	maskarray[i]=(1 << bitarray[i]) - 1;
  }

  modulemask =( (maskarray[0] << shiftarray[0]) |
		  (maskarray[1] << shiftarray[1]) |
		  (maskarray[2] << shiftarray[2]) |
		  (maskarray[3] << shiftarray[3]) |
		  (maskarray[4] << shiftarray[4]) |
		  (maskarray[5] << shiftarray[5]) |
		  (maskarray[6] << shiftarray[6])
  );
}

CbmTofDetectorInfo CbmTofDetectorId_v20a::GetDetectorInfo(const Int_t detectorId)
{
  for (Int_t i=0; i < array_length; i++) {
   result_array[i] = (( detectorId >> shiftarray[i] ) & maskarray[i] );
  }

  return CbmTofDetectorInfo(result_array[0], result_array[2], result_array[1], 
			    result_array[3], result_array[4], result_array[5]);

}

Int_t  CbmTofDetectorId_v20a::GetSystemId(Int_t detectorId) 
{
  return (detectorId & maskarray[0]);
}

//-----------------------------------------------------------

Int_t CbmTofDetectorId_v20a::GetSMType(const Int_t detectorId)
{
  return (( detectorId >> shiftarray[2] ) & maskarray[2] );
}
Int_t CbmTofDetectorId_v20a::GetModuleType(const Int_t detectorId)
{
  return GetSMType(detectorId);
}
Int_t CbmTofDetectorId_v20a::GetCounterType(const Int_t detectorId)
{
  return GetCounterType(detectorId);
}

//-----------------------------------------------------------

Int_t CbmTofDetectorId_v20a::GetSModule(const Int_t detectorId)
{
  return (( detectorId >> shiftarray[1] ) & maskarray[1] );
}
Int_t CbmTofDetectorId_v20a::GetModuleId(const Int_t detectorId)
{
  return GetSModule(detectorId);
}

//-----------------------------------------------------------

Int_t CbmTofDetectorId_v20a::GetCounter(const Int_t detectorId)
{
   return (( detectorId >> shiftarray[3] ) & maskarray[3] );
}

//-----------------------------------------------------------

Int_t CbmTofDetectorId_v20a::GetSide(const Int_t detectorId)
{
   return (( detectorId >> shiftarray[4] ) & maskarray[4] );
}
Int_t CbmTofDetectorId_v20a::GetGap(const Int_t detectorId)
{
  return GetSide(detectorId); 
}

//-----------------------------------------------------------

Int_t CbmTofDetectorId_v20a::GetCell(const Int_t detectorId)
{
   return (( detectorId >> shiftarray[5] ) & maskarray[5] );
}

Int_t CbmTofDetectorId_v20a::GetStrip(const Int_t detectorId)
{
   return GetCell(detectorId);
}

//-----------------------------------------------------------

Int_t CbmTofDetectorId_v20a::GetRegion(const Int_t /*detectorId*/)
{
  return -1;
}

Int_t CbmTofDetectorId_v20a::GetCellId(const Int_t detectorId)
{
  return (detectorId & modulemask);
}

//-----------------------------------------------------------

Int_t CbmTofDetectorId_v20a ::SetDetectorInfo(const CbmTofDetectorInfo detInfo)
{
  return ( (((detInfo.fDetectorSystem) & maskarray[0]) << shiftarray[0]) | 
           (((detInfo.fSMtype)         & maskarray[2]) << shiftarray[2]) | 
           (((detInfo.fSModule)        & maskarray[1]) << shiftarray[1]) | 
           (((detInfo.fCounter)        & maskarray[3]) << shiftarray[3])  | 
           (((detInfo.fCounterType)    & maskarray[4]) << shiftarray[4])  |
           (((detInfo.fGap)            & maskarray[5]) << shiftarray[5])  |
           (((detInfo.fCell)           & maskarray[6]) << shiftarray[6])  
         ); 
}

