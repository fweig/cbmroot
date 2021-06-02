/** @file CbmMuchSignal.cxx
  * @copyright Copyright (C) 2017-2019 Variable Energy Cyclotron Centre, Kolkata
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Vikas Singhal [committer] **/

/**CbmMuchSignal.cxx
 * @class CbmMuchSignal
 **@author Vikas Singhal <vikas@vecc.gov.in>
 **@since 25/04/18
 **@version 3.0 // Removing fPileUpTime and fModifiedCharge
 **@author Vikas Singhal <vikas@vecc.gov.in>
 **@since 10/04/18
 **@version 2.0
 ** Removing fSignalShape, which is mainly useful for PileUp Cases,
 ** but taking too much memory. Will implement similar behaviour 
 ** via computing PileUp effect on the fly.
 **@since 17/05/16
 **@version 1.0
 **@brief Data class for an analog signal in the MUCH
 ** Simple data class used in the digitisation process of the MUCH. It describes
 ** an analog charge distribution produced in the MUCH GEM Detector and arriving at the
 ** readout. It contains time and charge information corresponding to the MUCH Pad.
 ** In the most general case, a signal can be produced by more than one
 ** MCPoint; that is why the MC reference is of type CbmMatch and not CbmLink.
 **
 **
 **
 **/
#include "CbmMuchSignal.h"

#include "CbmMuchAddress.h"
#include "CbmMuchDigi.h"
#include "CbmMuchDigiMatch.h"

#include "TMath.h"
#include "TRandom.h"

#include <memory>

using namespace std;

/*
CbmMuchSignal::CbmMuchSignal(CbmMuchSignal* signal)
	: TObject(),
	fAddress(signal->fAddress),
	fTimeStart(signal->fTimeStart),
	fTimeStop(signal->fTimeStop),
	fSignalShape(signal->fSignalShape)
{
}
*/
CbmMuchSignal::CbmMuchSignal(const CbmMuchSignal& rhs)
  : TObject()
  , fAddress(rhs.fAddress)
  , fTimeStart(rhs.fTimeStart)
  , fTimeStop(rhs.fTimeStop)
  ,
  //fPileUpTime(rhs.fPileUpTime),
  fCharge(rhs.fCharge)
  ,
  //fModifiedCharge(rhs.fModifiedCharge),
  fPileUpCount(rhs.fPileUpCount)
  ,
  //fSignalShape(rhs.fSignalShape),
  fMatch(rhs.fMatch)
{
}
/*
CbmMuchSignal& CbmMuchSignal::operator=(const CbmMuchSignal& rhs)
{

	if (this != &rhs) {
	TObject::operator=(rhs);
	fAddress = rhs.fAddress;
	fTimeStop = rhs.fTimeStop;
	fTimeStart = rhs.fTimeStart;
	fSignalShape = rhs.fSignalShape;
	fMatch	= rhs.fMatch;
  }
  return *this;
}
*/


/*void CbmMuchSignal::MakeSignalShape(UInt_t charge, TArrayD shape) {
	Int_t bin0 = 0;
			//Int_t((fT0)/gkResponseBin);
	Int_t nbins = bin0+shape.GetSize();
	if (fSignalShape.GetSize()<nbins) fSignalShape.Set(nbins);
	for (Int_t j=0;j<shape.GetSize();j++)  fSignalShape[bin0 + j]+=charge*shape[j];
}*/
// -------------------------------------------------------------------------
//
//
// -------------------------------------------------------------------------
//Below function will add the Signal shapes of 2 signal

void CbmMuchSignal::MergeSignal(CbmMuchSignal* signal)
{
  fPileUpCount++;
  fPileUpCount += signal->GetPileUpCount();
  Long_t StartDiff  = signal->GetTimeStart() - fTimeStart;
  Long_t StopDiff   = signal->GetTimeStop() - fTimeStop;
  Bool_t MeFirst    = kTRUE;
  Long_t PileUpTime = 0;
  LOG(debug4) << " Start Difference " << StartDiff << " Stop Difference " << StopDiff;
  if (StopDiff > 0)
    fTimeStop =
      signal
        ->GetTimeStop();  //Shows Second signal Stop time is larger than first signal stop time, therefore fStopTime modified.
  if (StartDiff < 0) {
    //Shows that New Signal is earlier than This signal.
    //	std::cout<<"First Signal start time " <<fTimeStart<<" Second Signal start time is " <<signal->GetTimeStart() << endl;
    MeFirst = kFALSE;
    //StartDiff = (-1)*StartDiff;
    PileUpTime = fTimeStart;
    fTimeStart = signal->GetTimeStart();
  }
  else {
    PileUpTime = signal->GetTimeStart();
  }
  Long_t PileUpDiff = PileUpTime - fTimeStart;
  if (PileUpDiff < 0) { LOG(info) << GetName() << " Problem: Check this particular pile up case."; }
  if (PileUpDiff < SLOWSHAPERPEAK) { fCharge += signal->GetCharge(); }
  else if (!MeFirst)
    fCharge = signal->GetCharge();

  /*TArrayD SecondSignalShape = signal->GetSignalShape();
	fSignalShape.Set(fSignalShape.GetSize()+StopDiff);
	//std::cout<<"MergeSignal called and size of fSignalShape "<<fSignalShape.GetSize()<<endl;
	for (Int_t j=0;j<fSignalShape.GetSize()&&j<SecondSignalShape.GetSize();j++)
		  
		fSignalShape[j+StartDiff]=fSignalShape[j+StartDiff]+SecondSignalShape[j];
		//SignalShape will be added in the first signal at location from where second signal start.
*/
}

void CbmMuchSignal::AddNoise(UInt_t meanNoise)
{
  fCharge += TMath::Abs(meanNoise * gRandom->Gaus());
  //	for (Int_t i=0;i<fSignalShape.GetSize();i++){
  //		fSignalShape[i]+=TMath::Abs(meanNoise*gRandom->Gaus());
  //  	}
}


/*Int_t CbmMuchSignal::GetMaxCharge(){
	Int_t max_charge = -1;
	for (Int_t i=0;i<fSignalShape.GetSize();i++){
		Int_t charge = fSignalShape[i];
		if (charge>max_charge) max_charge = charge;
	}
  return max_charge;
}*/


/*Int_t CbmMuchSignal::GetTimeStamp(Int_t threshold){
  //Int_t threshold = 10000;
  Int_t bin1 = -1;
  for (Int_t i=0;i<fSignalShape.GetSize();i++){
    if (bin1<0 && fSignalShape[i]>threshold) {
      bin1 = i;
      return fTimeStart+bin1*gkResponseBin;
    }
  }
  return -1;
}*/
// -------------------------------------------------------------------------


/* Below functions will not be used as ADC or Charge value will be calculated on the basis of fSignalShape of the Analog Signal.
// -----   Add charge   ----------------------------------------------------
void CbmMuchSignal::AddAdc(Int_t adc) {
  Int_t newAdc = GetAdc() + adc;
  SetAdc(newAdc);
}
// -------------------------------------------------------------------------


// -----   Set new charge   ------------------------------------------------
void CbmMuchSignal::SetAdc(Int_t adc) {
	//Charge value should not be more than saturation
	
	UShort_t saturation = 4095; //2 ^ 12 - 1;	
	if(adc>saturation){
		fCharge=saturation;
		fSaturationFlag=1;
	}
	else if (adc < 0) fCharge=0;
	else	fCharge = (UShort_t)adc;
}
// -------------------------------------------------------------------------
*/

ClassImp(CbmMuchSignal)
