/* Copyright (C) 2013-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer], Florian Uhlig */

/** @file CbmStsSimModule.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 14.05.2013
 **/

#include "CbmStsSimModule.h"

#include "CbmStsDigitize.h"
#include "CbmStsElement.h"


using namespace std;

ClassImp(CbmStsSimModule)


  // -----   Default constructor   -------------------------------------------
  CbmStsSimModule::CbmStsSimModule(CbmStsElement* element, const CbmStsParModule* params, CbmStsDigitize* digitizer)
  : fElement(element)
  , fDigitizer(digitizer)
  , fParams(params)
{
}
// -------------------------------------------------------------------------


// --- Destructor   --------------------------------------------------------
CbmStsSimModule::~CbmStsSimModule()
{

  // --- Clean analog buffer
  for (auto chanIt = fAnalogBuffer.begin(); chanIt != fAnalogBuffer.end(); chanIt++) {
    for (auto sigIt = (chanIt->second).begin(); sigIt != (chanIt->second).end(); sigIt++) {
      delete (*sigIt);
    }
  }
}
// -------------------------------------------------------------------------


// -----   Add a signal to the buffer   ------------------------------------
void CbmStsSimModule::AddSignal(UShort_t channel, Double_t time, Double_t charge, Int_t index, Int_t entry, Int_t file)
{

  // --- Check channel number
  assert(channel < GetNofChannels());

  // --- Discard charge if the channel is dead
  if (!fParams->IsChannelActive(channel)) return;

  // --- If the channel is not yet active: create a new set and insert
  // --- new signal into it.
  if (fAnalogBuffer.find(channel) == fAnalogBuffer.end()) {
    CbmStsSignal* signal = new CbmStsSignal(time, charge, index, entry, file);
    fAnalogBuffer[channel].insert(signal);
    return;
  }  //? Channel not yet active

  // --- The channel is active: there are already signals in.
  // --- Loop over all signals in the channels and compare their time.
  //TODO: Loop over all signals is not needed, since they are time-ordered.
  Bool_t isMerged = kFALSE;
  sigset::iterator it;
  Double_t deadTime = fParams->GetParAsic(channel).GetDeadTime();
  for (it = fAnalogBuffer[channel].begin(); it != fAnalogBuffer[channel].end(); it++) {

    // Time between new and old signal smaller than dead time: merge signals
    if (TMath::Abs((*it)->GetTime() - time) < deadTime) {

      // Current implementation of merging signals:
      // Add charges, keep first signal time
      // TODO: Check with STS electronics people on more realistic behaviour.
      (*it)->SetTime(TMath::Min((*it)->GetTime(), time));
      (*it)->AddLink(charge, index, entry, file);
      isMerged = kTRUE;  // mark new signal as merged
      break;             // Merging should be necessary only for one buffer signal

    }  //? Time difference smaller than dead time

  }  // Loop over signals in buffer for this channel

  // --- If signal was merged: no further action
  if (isMerged) return;

  // --- Arriving here, the signal did not interfere with existing ones.
  // --- So, it is added to the analog buffer.
  CbmStsSignal* signal = new CbmStsSignal(time, charge, index, entry, file);
  fAnalogBuffer[channel].insert(signal);
}
// -------------------------------------------------------------------------


// -----   Status of analogue buffer   -------------------------------------
void CbmStsSimModule::BufferStatus(Int_t& nofSignals, Double_t& timeFirst, Double_t& timeLast)
{


  Int_t nSignals   = 0;
  Double_t tFirst  = -1.;
  Double_t tLast   = -1.;
  Double_t tSignal = -1.;

  // --- Loop over active channels
  for (auto chanIt = fAnalogBuffer.begin(); chanIt != fAnalogBuffer.end(); chanIt++) {

    // --- Loop over signals in channel
    for (auto sigIt = (chanIt->second).begin(); sigIt != (chanIt->second).end(); sigIt++) {

      tSignal = (*sigIt)->GetTime();
      nSignals++;
      tFirst = tFirst < 0. ? tSignal : TMath::Min(tFirst, tSignal);
      tLast  = TMath::Max(tLast, tSignal);

    }  // signals in channel

  }  // channels in module

  nofSignals = nSignals;
  timeFirst  = tFirst;
  timeLast   = tLast;
}
// -------------------------------------------------------------------------


// -----   Digitise an analogue charge signal   ----------------------------
void CbmStsSimModule::Digitize(UShort_t channel, CbmStsSignal* signal)
{

  // --- Check channel number
  assert(channel < GetNofChannels());

  auto& asic = fParams->GetParAsic(channel);

  // --- No action if charge is below threshold
  Double_t charge = signal->GetCharge();

  // --- Digitise charge
  Short_t adc = asic.ChargeToAdc(charge);
  if (adc < 0) return;  // Charge below threshold

  // --- Digitise time
  Double_t deltaT = gRandom->Gaus(0., asic.GetTimeResol());
  Long64_t dTime  = Long64_t(round(signal->GetTime() + deltaT));

  // --- Send the message to the digitiser task
  UInt_t address = fElement->GetAddress();
  if (fDigitizer) fDigitizer->CreateDigi(address, channel, dTime, adc, signal->GetMatch());

  // --- If no digitiser task is present (debug mode): create a digi and
  // --- add it to the digi buffer.
  else
    return;
}
// -------------------------------------------------------------------------


// -----   Generate noise   ------------------------------------------------
Int_t CbmStsSimModule::GenerateNoise(Double_t t1, Double_t t2)
{

  assert(t2 > t1);
  Int_t nNoiseAll      = 0;
  UInt_t nAsicChannels = fParams->GetNofAsicChannels();

  for (UInt_t iAsic = 0; iAsic < fParams->GetNofAsics(); iAsic++) {
    auto& asic = fParams->GetAsicParams().at(iAsic);

    // --- Mean number of noise digis in [t1, t2]
    Double_t nNoiseMean = asic.GetNoiseRate() * nAsicChannels * (t2 - t1);

    // --- Sample number of noise digis
    Int_t nNoise = gRandom->Poisson(nNoiseMean);

    // --- Create noise digis
    for (Int_t iNoise = 0; iNoise < nNoise; iNoise++) {

      // --- Random channel number, time and charge
      UInt_t channel       = UInt_t(gRandom->Uniform(Double_t(nAsicChannels)));
      Double_t time        = gRandom->Uniform(t1, t2);
      Double_t charge      = asic.GetRandomNoiseCharge();
      UInt_t moduleChannel = iAsic * nAsicChannels + channel;

      // --- Insert a signal object (without link index, entry and file)
      // --- into the analogue buffer.
      AddSignal(moduleChannel, time, charge, -1, -1, -1);
    }  //# noise digis

    nNoiseAll += nNoise;
  }

  return nNoiseAll;
}
// -------------------------------------------------------------------------


// -----   Get the unique address from the sensor name (static)   ----------
Int_t CbmStsSimModule::GetAddressFromName(TString name)
{

  Bool_t isValid = kTRUE;
  if (name.Length() != 16) isValid = kFALSE;
  if (isValid) {
    if (!name.BeginsWith("STS")) isValid = kFALSE;
    if (name[4] != 'U') isValid = kFALSE;
    if (name[8] != 'L') isValid = kFALSE;
    if (name[13] != 'M') isValid = kFALSE;
  }
  assert(isValid);

  Int_t unit    = 10 * (name[5] - '0') + name[6] - '0' - 1;
  Int_t ladder  = 10 * (name[9] - '0') + name[10] - '0' - 1;
  Int_t hLadder = (name[11] == 'U' ? 0 : 1);
  Int_t module  = 10 * (name[14] - '0') + name[15] - '0' - 1;

  return CbmStsAddress::GetAddress(unit, ladder, hLadder, module);
}
// -------------------------------------------------------------------------


// -----  Initialise the analogue buffer   ---------------------------------
void CbmStsSimModule::InitAnalogBuffer()
{

  for (UShort_t channel = 0; channel < fParams->GetNofChannels(); channel++) {
    multiset<CbmStsSignal*, CbmStsSignal::Before> mset;
    fAnalogBuffer[channel] = mset;
  }  //# channels
}
// -------------------------------------------------------------------------


// -----   Process the analogue buffer   -----------------------------------
Int_t CbmStsSimModule::ProcessAnalogBuffer(Double_t readoutTime)
{

  // --- Counter
  Int_t nDigis = 0;

  // Create iterators needed for inner loop
  sigset::iterator sigIt;
  ;
  sigset::iterator oldIt;
  sigset::iterator endIt;

  // --- Iterate over active channels
  for (auto& chanIt : fAnalogBuffer) {

    // Only do something if there are signals for the channel
    if (!(chanIt.second).empty()) {
      auto& asic = fParams->GetParAsic(chanIt.first);

      // --- Time limit up to which signals are digitised and sent to DAQ.
      // --- Up to that limit, it is guaranteed that future signals do not
      // --- interfere with the buffered ones. The readoutTime is the time
      // --- of the last processed StsPoint. All coming points will be later
      // --- in time. So, the time limit is defined by this time minus
      // --- 5 times the time resolution (maximal deviation of signal time
      // --- from StsPoint time) minus the dead time, within which
      // --- interference of signals can happen.
      Double_t timeLimit = readoutTime - 5. * asic.GetTimeResol() - asic.GetDeadTime();

      // --- Digitise all signals up to the specified time limit
      sigIt = (chanIt.second).begin();
      oldIt = sigIt;
      endIt = (chanIt.second).end();
      while (sigIt != endIt) {

        // --- Exit loop if signal time is larger than time limit
        // --- N.b.: Readout time < 0 means digitise everything
        if (readoutTime >= 0. && (*sigIt)->GetTime() > timeLimit) break;

        // --- Digitise signal
        Digitize(chanIt.first, (*sigIt));
        nDigis++;

        // --- Increment iterator before it becomes invalid
        oldIt = sigIt;
        sigIt++;

        // --- Delete digitised signal
        delete (*oldIt);
        (chanIt.second).erase(oldIt);
      }  // Iterate over signals in channel
    }    // if there are signals
  }      // Iterate over channels

  return nDigis;
}
// -------------------------------------------------------------------------


// -----   String output   -------------------------------------------------
string CbmStsSimModule::ToString() const
{
  stringstream ss;
  auto& asic = fParams->GetParAsic(0);
  ss << "Module  " << fElement->GetName() << ": dynRange " << asic.GetDynRange() << "e, thresh. " << asic.GetThreshold()
     << "e, nAdc " << asic.GetNofAdc() << ", time res. " << asic.GetTimeResol() << "ns, dead time "
     << asic.GetDeadTime() << "ns, noise " << asic.GetNoise() << "e, zero noise rate " << asic.GetZeroNoiseRate()
     << "/ns";
  return ss.str();
}
// -------------------------------------------------------------------------
