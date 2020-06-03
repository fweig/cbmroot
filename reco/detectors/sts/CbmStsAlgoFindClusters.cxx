/** @file CbmStsAlgoFindClusters.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 21.03.2020
 **/

#include "CbmStsAlgoFindClusters.h"

#include <cassert>
#include <iostream>

#include "CbmStsCluster.h"
#include "CbmStsParAsic.h"
#include "CbmStsParModule.h"
#include "CbmStsDigi.h"

using std::pair;
using std::vector;

using InputData = CbmStsAlgoFindClusters::InputData;


// ----- Search for a matching cluster for a given channel   ---------------
Bool_t CbmStsAlgoFindClusters::CheckChannel(Short_t channel,
                                            Double_t time) {

  // Neighbours of first or last channel without cross-connection
  if (channel == -1 || channel == fNofChannels) return kFALSE;

  // Check channel number
  assert( channel < fNofChannels );

  // No match if no active digi in the channel
  if ( ! IsActive(channel) ) return kFALSE;

  // Check time ordering of input digis
  assert( time >= fStatus[channel].second );

  // Time difference
  Double_t tResol = fModPar->GetParAsic(channel).GetTimeResol();
  Double_t deltaT = ( fTimeCutAbs > 0. ? fTimeCutAbs :
      fTimeCutSig * 1.4142 * tResol );

  // Channel is active, but time is not matching: close cluster
  if ( time - fStatus[channel].second > deltaT ) {
    CreateCluster(channel);
    return kFALSE;
  }

  // Matching digi found
  return kTRUE;
}
// -------------------------------------------------------------------------



// -----   Algorithm execution   -------------------------------------------
Long64_t
CbmStsAlgoFindClusters::Exec(const vector<InputData>& input,
                             vector<CbmStsCluster>& output,
                             UInt_t address,
                             UShort_t nChannels,
                             UShort_t channelOffset,
                             Double_t timeCutSigma,
                             Double_t timeCutAbs,
                             Bool_t connectEdge,
                             const CbmStsParModule* modPar) {

  // --- Set parameters and output
  fAddress       = address;
  fNofChannels   = nChannels;
  fChannelOffset = channelOffset;
  fTimeCutSig    = timeCutSigma;
  fTimeCutAbs    = timeCutAbs;
  fConnectEdge   = connectEdge;
  fOutput        = &output;
  fModPar        = modPar;

  // --- Reset buffer and output
  fStatus.assign(fNofChannels, {-1, 0.});
  fOutput->clear();

  // --- Process digis one by one
  Long64_t nDigis = 0;
  Bool_t result = kTRUE;
  for ( auto data : input ) {
    UShort_t channel = data.first->GetChannel();
    assert (channel >= channelOffset);
    channel -= channelOffset;    // Transform into [0, nofChannels-1]
    result = ProcessDigi(channel,
                         data.first->GetTime(),
                         data.second);
    if ( result ) nDigis++;
  }

  // --- Create clusters from the remaining digis in the buffer
  for (UShort_t channel = 0; channel < nChannels; channel++) {
    CreateCluster(channel);
  }

  return nDigis;
}
// -------------------------------------------------------------------------



// -----   Close a cluster   -----------------------------------------------
void CbmStsAlgoFindClusters::CreateCluster(UShort_t channel) {

  assert(channel < fNofChannels);
  if ( ! IsActive(channel) ) return;

  // --- Find start and stop channel of cluster
  Short_t start = channel;
  Short_t stop = channel;
  while ( IsActive(ChanLeft(start)) ) start = ChanLeft(start);
  while ( IsActive(ChanRight(stop)) ) stop = ChanRight(stop);

  // --- Just a check
  assert(start >=0 && start < fNofChannels);
  assert(stop >=0 && stop < fNofChannels);
  if ( ! fConnectEdge ) assert(stop >= start);

  // --- Create a cluster object
  CbmStsCluster cluster { };
  cluster.SetAddress(fAddress);

  // --- Add digis to cluster and reset the respective buffer channels
  Short_t iChannel = start;
  while ( kTRUE ) {
    assert( IsActive(iChannel) );
    cluster.AddDigi(fStatus[iChannel].first);
    fStatus[iChannel].first = -1;
    fStatus[iChannel].second = 0.;
    if ( iChannel == stop ) break;
    iChannel = ChanRight(iChannel);
  }

  // --- Add cluster to output array
  fOutput->push_back(std::move(cluster));

}
// -------------------------------------------------------------------------



// -----   Process one digi   ----------------------------------------------
Bool_t CbmStsAlgoFindClusters::ProcessDigi(UShort_t channel, Double_t time,
                                           Int_t index) {

  // Assert channel number
  assert ( channel < fNofChannels );

  // Check for matching digi in the same channel (can only happen if
  // time resolution is not much smaller than the dead time.)
  // In this case, the digi is ignored.
  if ( CheckChannel(channel, time) )  return kFALSE;

  assert(ChanLeft(channel) >= -1);
  assert(ChanLeft(channel) < fNofChannels+1);
  assert(ChanRight(channel) < fNofChannels+1);

  // Check for not-matching digis in the neighbour channels
  CheckChannel(ChanLeft(channel), time);
  CheckChannel(ChanRight(channel), time);

  // Set channel status with this digi
  fStatus[channel].first = index;
  fStatus[channel].second = time;

  return kTRUE;
}
// -------------------------------------------------------------------------

