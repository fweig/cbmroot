/** @file CbmStsAlgoAnaCluster.cxx
  * @copyright Copyright (C) 2016-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Volker Friese [committer] **/

/** @file CbmStsAlgoAnaCluster.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 18.10.2016
 **/

#include "CbmStsAlgoAnaCluster.h"

#include "CbmDigiManager.h"
#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsParModule.h"
#include "CbmStsPhysics.h"

#include <TMath.h>

using std::unique_ptr;


ClassImp(CbmStsAlgoAnaCluster)


  // -----   Constructor   ----------------------------------------------------
  CbmStsAlgoAnaCluster::CbmStsAlgoAnaCluster()
  : fDigiMan(CbmDigiManager::Instance())
  , fPhysics(CbmStsPhysics::Instance())
{
}
// --------------------------------------------------------------------------


// -----   Algorithm for one-digi clusters   --------------------------------
void CbmStsAlgoAnaCluster::AnaSize1(CbmStsCluster& cluster, const CbmStsParModule* module)
{

  assert(module);
  Int_t index            = cluster.GetDigi(0);
  const CbmStsDigi* digi = fDigiMan->Get<CbmStsDigi>(index);
  assert(digi);
  auto& asic         = module->GetParAsic(digi->GetChannel());
  Double_t x         = Double_t(digi->GetChannel());
  Double_t time      = digi->GetTime();
  Double_t timeError = asic.GetTimeResol();
  Double_t charge    = asic.AdcToCharge(digi->GetCharge());
  Double_t xError    = 1. / sqrt(24.);

  cluster.SetProperties(charge, x, xError, time, timeError);
  cluster.SetSize(1);
}
// --------------------------------------------------------------------------


// -----   Algorithm for two-digi clusters   --------------------------------
void CbmStsAlgoAnaCluster::AnaSize2(CbmStsCluster& cluster, const CbmStsParModule* modPar)
{

  Int_t index1            = cluster.GetDigi(0);
  Int_t index2            = cluster.GetDigi(1);
  const CbmStsDigi* digi1 = fDigiMan->Get<CbmStsDigi>(index1);
  const CbmStsDigi* digi2 = fDigiMan->Get<CbmStsDigi>(index2);
  assert(digi1);
  assert(digi2);

  auto& asic1 = modPar->GetParAsic(digi1->GetChannel());
  auto& asic2 = modPar->GetParAsic(digi2->GetChannel());

  // --- Uncertainties of the charge measurements
  Double_t eNoiseSq = 0.5 * (asic1.GetNoise() * asic1.GetNoise() + asic2.GetNoise() * asic2.GetNoise());
  Double_t chargePerAdc =
    0.5 * (asic1.GetDynRange() / Double_t(asic1.GetNofAdc()) + asic2.GetDynRange() / Double_t(asic2.GetNofAdc()));
  Double_t eDigitSq = chargePerAdc * chargePerAdc / 12.;

  UInt_t chan1 = digi1->GetChannel();
  UInt_t chan2 = digi2->GetChannel();
  assert(chan2 == chan1 + 1 || chan2 == chan1 - modPar->GetNofChannels() / 2 + 1);

  // Channel positions and charge
  Double_t x1 = Double_t(chan1);
  Double_t q1 = asic1.AdcToCharge(digi1->GetCharge());
  Double_t q2 = asic2.AdcToCharge(digi2->GetCharge());

  // Periodic position for clusters round the edge
  if (chan1 > chan2) x1 -= Double_t(modPar->GetNofChannels() / 2);

  // Uncertainties of the charge measurements
  Double_t width1 = fPhysics->LandauWidth(q1);
  Double_t eq1sq  = width1 * width1 + eNoiseSq + eDigitSq;
  Double_t width2 = fPhysics->LandauWidth(q2);
  Double_t eq2sq  = width2 * width2 + eNoiseSq + eDigitSq;

  // Cluster time
  Double_t time      = 0.5 * (digi1->GetTime() + digi2->GetTime());
  Double_t timeError = 0.5 * (asic1.GetTimeResol() + asic2.GetTimeResol()) * 0.70710678;  // 1/sqrt(2)

  // Cluster position
  // See corresponding software note.
  Double_t x = x1 + 0.5 + (q2 - q1) / 3. / TMath::Max(q1, q2);

  // Correct negative position for clusters around the edge
  if (x < -0.5) x += Double_t(modPar->GetNofChannels() / 2);

  // Uncertainty on cluster position. See software note.
  Double_t ex0sq = 0.;  // error for ideal charge measurements
  Double_t ex1sq = 0.;  // error from first charge
  Double_t ex2sq = 0.;  // error from second charge
  if (q1 < q2) {
    ex0sq = (q2 - q1) * (q2 - q1) / q2 / q2 / 72.;
    ex1sq = eq1sq / q2 / q2 / 9.;
    ex2sq = eq2sq * q1 * q1 / q2 / q2 / q2 / q2 / 9.;
  }
  else {
    ex0sq = (q2 - q1) * (q2 - q1) / q1 / q1 / 72.;
    ex1sq = eq1sq * q2 * q2 / q1 / q1 / q1 / q1 / 9.;
    ex2sq = eq2sq / q1 / q1 / 9.;
  }
  Double_t xError = TMath::Sqrt(ex0sq + ex1sq + ex2sq);

  // Cluster charge
  Double_t charge = q1 + q2;

  cluster.SetProperties(charge, x, xError, time, timeError);
  cluster.SetSize(2);
}
// --------------------------------------------------------------------------


// -----   Algorithm for clusters with more than two digis   ----------------
void CbmStsAlgoAnaCluster::AnaSizeN(CbmStsCluster& cluster, const CbmStsParModule* modPar)
{

  Double_t tSum        = 0.;       // sum of digi times
  Int_t chanF          = 9999999;  // first channel in cluster
  Int_t chanL          = -1;       // last channel in cluster
  Double_t qF          = 0.;       // charge in first channel
  Double_t qM          = 0.;       // sum of charges in middle channels
  Double_t qL          = 0.;       // charge in last cluster
  Double_t eqFsq       = 0.;       // uncertainty of qF
  Double_t eqMsq       = 0.;       // uncertainty of qMid
  Double_t eqLsq       = 0.;       // uncertainty of qL
  Double_t prevChannel = 0;
  Double_t tResolSum   = 0.;

  for (Int_t iDigi = 0; iDigi < cluster.GetNofDigis(); iDigi++) {

    Int_t index            = cluster.GetDigi(iDigi);
    const CbmStsDigi* digi = fDigiMan->Get<CbmStsDigi>(index);
    assert(digi);
    Int_t channel = digi->GetChannel();
    auto& asic    = modPar->GetParAsic(channel);

    // --- Uncertainties of the charge measurements
    Double_t eNoiseSq     = asic.GetNoise() * asic.GetNoise();
    Double_t chargePerAdc = asic.GetDynRange() / Double_t(asic.GetNofAdc());
    Double_t eDigitSq     = chargePerAdc * chargePerAdc / 12.;
    tResolSum += asic.GetTimeResol();

    tSum += digi->GetTime();
    Double_t charge    = asic.AdcToCharge(digi->GetCharge());
    Double_t lWidth    = fPhysics->LandauWidth(charge);
    Double_t eChargeSq = lWidth * lWidth + eNoiseSq + eDigitSq;

    // Check ascending order of channel number
    if (iDigi > 0) assert(channel == prevChannel + 1 || channel == prevChannel - modPar->GetNofChannels() / 2 + 1);
    prevChannel = channel;

    if (iDigi == 0) {  // first channel
      chanF = channel;
      qF    = charge;
      eqFsq = eChargeSq;
    }
    else if (iDigi == cluster.GetNofDigis() - 1) {  // last channel
      chanL = channel;
      qL    = charge;
      eqLsq = eChargeSq;
    }
    else {  // one of the middle channels
      qM += charge;
      eqMsq += eChargeSq;
    }

  }  //# digis in cluster

  // Periodic channel position for clusters round the edge
  if (chanF > chanL) chanF -= modPar->GetNofChannels() / 2;

  // Cluster time and total charge
  tSum            = tSum / Double_t(cluster.GetNofDigis());
  Double_t tError = (tResolSum / Double_t(cluster.GetNofDigis())) / TMath::Sqrt(Double_t(cluster.GetNofDigis()));
  Double_t qSum   = qF + qM + qL;

  // Average charge in middle strips
  qM /= Double_t(cluster.GetNofDigis() - 2);
  eqMsq /= Double_t(cluster.GetNofDigis() - 2);

  // Cluster position
  Double_t x = 0.5 * (Double_t(chanF + chanL) + (qL - qF) / qM);

  // Correct negative cluster position for clusters round the edge
  if (x < -0.5) x += Double_t(modPar->GetNofChannels() / 2);

  // Cluster position error
  Double_t exFsq  = eqFsq / qM / qM / 4.;  // error from first charge
  Double_t exMsq  = eqMsq * (qL - qF) * (qL - qF) / qM / qM / qM / qM / 4.;
  Double_t exLsq  = eqLsq / qM / qM / 4.;
  Double_t xError = TMath::Sqrt(exFsq + exMsq + exLsq);

  // Correction for corrupt clusters
  if (x < chanF || x > chanL) x = WeightedMean(cluster, modPar);

  assert(x >= chanF && x <= chanL);

  cluster.SetProperties(qSum, x, xError, tSum, tError);
  cluster.SetSize(chanL - chanF + 1);
}
// --------------------------------------------------------------------------


// -----   Algorithm execution   --------------------------------------------
void CbmStsAlgoAnaCluster::Exec(CbmStsCluster& cluster, const CbmStsParModule* modPar)
{

  Int_t nDigis = cluster.GetNofDigis();
  assert(nDigis >= 0);

  switch (nDigis) {

    case 0: break;
    case 1: AnaSize1(cluster, modPar); break;
    case 2: AnaSize2(cluster, modPar); break;
    default: AnaSizeN(cluster, modPar); break;

  }  //? number of digis
}
// --------------------------------------------------------------------------


// -----   Weighted mean calculation   --------------------------------------
Double_t CbmStsAlgoAnaCluster::WeightedMean(CbmStsCluster& cluster, const CbmStsParModule* modPar)
{

  Double_t qSum = 0.;
  Double_t xSum = 0.;
  for (Int_t iDigi = 0; iDigi < cluster.GetNofDigis(); iDigi++) {
    Int_t index            = cluster.GetDigi(iDigi);
    const CbmStsDigi* digi = fDigiMan->Get<CbmStsDigi>(index);
    assert(digi);
    Int_t channel   = digi->GetChannel();
    auto& asic      = modPar->GetParAsic(channel);
    Double_t charge = asic.AdcToCharge(digi->GetCharge());
    qSum += charge;
    xSum += charge * Double_t(channel);
  }

  return xSum / qSum;
}
// --------------------------------------------------------------------------
