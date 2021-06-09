/* Copyright (C) 2016-2019 Variable Energy Cyclotron Centre, Kolkata
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Vikas Singhal [committer], Volker Friese */

/*
 * CbmMuchReadoutBuffer.cxx
 *
 *  Modified on: 19.04.2018 // Removed fSignalShape from CbmMuchSignal
 *  Created on: 11.05.2016
 *      Author: vikas@vecc.gov.in
 */


#include "CbmMuchReadoutBuffer.h"

// -----   Initialisation of static variables   ------------------------------
CbmMuchReadoutBuffer* CbmMuchReadoutBuffer::fgInstance = NULL;
// ---------------------------------------------------------------------------

//This Modify Function will be called from CbmReadoutBuffer::Fill().
//2 CbmMuchSignal has to be merged and create one, as both hit on the same Detector or Pad within detector deadtime window.
//SignalStart time and SignalEnd time is already modified.
//ADC has to be summed up
//Matches of two also be joined.
Int_t CbmMuchReadoutBuffer::Merge(CbmMuchSignal* oldData1, CbmMuchSignal* oldData2,
                                  std::vector<CbmMuchSignal*>& newDataList)
{

  //std::cout << "Modify: old data 1 at t = " << oldData1->GetTimeStart()
  //                      << " to " << oldData1->GetTimeStop();
  //std::cout << "Modify: old data 2 at t = " << oldData2->GetTimeStart()
  //                      << " to " << oldData2->GetTimeStop();
  LOG(debug4) << "Modify: old data 1 at t = " << oldData1->GetTimeStart() << " to " << oldData1->GetTimeStop();
  LOG(debug4) << "Modify: old data 2 at t = " << oldData2->GetTimeStart() << " to " << oldData2->GetTimeStop();

  CbmMuchSignal* firstData  = oldData1;
  CbmMuchSignal* secondData = oldData2;
  if (oldData1->GetTimeStart() > oldData2->GetTimeStart()) {
    firstData  = oldData2;
    secondData = oldData1;
  }

  /* Create a new signal from the first signal and then correspondingly modify the Match of that signal.
			 * For this add secondData information to the newdata
			 */
  // Create new signal object same as firstData
  CbmMuchSignal* newData = new CbmMuchSignal(firstData->GetAddress());
  newData->SetTimeStart(firstData->GetTimeStart());
  newData->SetTimeStop(firstData->GetTimeStop());
  //newData->SetSignalShape(firstData->GetSignalShape()); // Only storing Charge 				value not the entire fSignalShape
  newData->SetCharge(firstData->GetCharge());
  // Set the parameters of the new signal,
  // it contains Address=which will be same,
  // fSignalShape will be modified according to Real Behavier
  // StartTime = Time of the first digi,
  // StopTime = Max stop time of both digis.

  //Addup charges according to fSignalShape of  both signal's.
  //Implemented pile up accordingly, not add up of ADCs
  //Weight of match object contains pure total added charge, can be use if needed.
  //newData->MergeSignal(firstData);
  newData->MergeSignal(secondData);

  Double_t stopTime = std::max(oldData1->GetTimeStop(), oldData2->GetTimeStop());
  newData->SetTimeStop(stopTime);

  //Merge matches of both data, created signal is new therefore contain nothing in the fMatch variable.
  //adding links of firstData->Match to NewMatch
  (newData->GetMatch())->AddLinks(*(firstData->GetMatch()));
  //adding links of SecondData->Match to NewMatch
  (newData->GetMatch())->AddLinks(*(secondData->GetMatch()));


  LOG(debug4) << "Modify: new data at t = " << newData->GetTimeStart() << " to " << newData->GetTimeStop();
  //Add new data object to the result list
  newDataList.push_back(newData);

  return 1;  // Number of return data objects
}

// -----   Instance   --------------------------------------------------------
CbmMuchReadoutBuffer* CbmMuchReadoutBuffer::Instance()
{
  if (!fgInstance) fgInstance = new CbmMuchReadoutBuffer();
  return fgInstance;
}
// ---------------------------------------------------------------------------

ClassImp(CbmMuchReadoutBuffer)
