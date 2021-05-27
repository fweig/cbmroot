/*
 * CbmRichPmt.cxx
 *
 *  Created on: Dec 16, 2015
 *      Author: slebedev
 */

#include "CbmRichPmt.h"

#include "CbmRichGeoManager.h"  // for CbmRichGeoManager
#include "CbmRichPmtType.h"     // for CbmRichPmtTypeEnum, CbmRichPmtTypeCer...
#include "CbmRichRecGeoPar.h"   // for CbmRichRecGeoPar

#include <Logger.h>  // for LOG, Logger

#include <TAxis.h>    // for TAxis
#include <TGraph.h>   // for TGraph
#include <TRandom.h>  // for TRandom, gRandom

#include <utility>  // for pair

#include <stddef.h>  // for size_t

using namespace std;

const Double_t CbmRichPmt::c = 2.998E8;     // speed of light
const Double_t CbmRichPmt::h = 6.626E-34;   // Planck constant
const Double_t CbmRichPmt::e = 1.6022E-19;  // elementary charge

CbmRichPmt::CbmRichPmt() : fCollectionEfficiency(1.), fPmtDataMap(), fRefractiveIndex(-1.) { InitQE(); }

CbmRichPmt::~CbmRichPmt() { ClearMap(); }

Double_t CbmRichPmt::getLambda(Double_t momentum)
{
  Double_t refractiveIndex = CbmRichGeoManager::GetInstance().fGP->fNRefrac;
  Double_t lambda          = c / refractiveIndex * h / e / momentum;  // wavelength in nm
  return lambda;
}

Bool_t CbmRichPmt::isPhotonDetected(CbmRichPmtTypeEnum detType, Double_t momentum)
{
  map<CbmRichPmtTypeEnum, CbmRichPmtQEData*>::iterator it = fPmtDataMap.find(detType);
  if (it == fPmtDataMap.end()) {
    LOG(error) << "CbmRichPmt::isPhotonDetected - Wrong detector type :" << detType;
    ;
    return false;
  }

  CbmRichPmtQEData* pmtData = it->second;
  if (nullptr == pmtData) {
    LOG(error) << "CbmRichPmt::isPhotonDetected - data is nullptr for detType :" << detType;
    return false;
  }

  if (fRefractiveIndex < 0.) fRefractiveIndex = CbmRichGeoManager::GetInstance().fGP->fNRefrac;
  Double_t lambda = c / fRefractiveIndex * h / e / momentum;  // wavelength in nm

  if (lambda >= pmtData->fLambdaMin && lambda < pmtData->fLambdaMax) {
    Int_t ilambda = (Int_t)((lambda - pmtData->fLambdaMin) / pmtData->fLambdaStep);
    Double_t rand = gRandom->Rndm();
    if (ilambda < 0 || static_cast<size_t>(ilambda) >= pmtData->fEfficiency.size()) {
      LOG(error) << "CbmRichPmt::isPhotonDetected ilambda out of range.";
      return false;
    }
    else {
      if (pmtData->fEfficiency[ilambda] * fCollectionEfficiency > rand) return true;
    }
  }
  return false;
}

TGraph* CbmRichPmt::getQEGraph(CbmRichPmtTypeEnum detType)
{
  map<CbmRichPmtTypeEnum, CbmRichPmtQEData*>::iterator it = fPmtDataMap.find(detType);
  if (it == fPmtDataMap.end()) {
    LOG(info) << "HRich700Pmt::getQEGraph - Wrong detector type :" << detType;
    return nullptr;
  }
  CbmRichPmtQEData* pmtData = it->second;
  if (nullptr == pmtData) {
    LOG(info) << "CbmRichPmt::getQEGraph - data is nullptr for detType :" << detType;
    return nullptr;
  }

  const Int_t n = pmtData->fEfficiency.size();
  Double_t x[n], y[n];
  for (Int_t i = 0; i < n; i++) {
    x[i] = pmtData->fLambdaMin + i * pmtData->fLambdaStep;
    y[i] = pmtData->fEfficiency[i];
  }
  TGraph* gr = new TGraph(n, x, y);
  gr->GetXaxis()->SetTitle("Wavelangth [nm]");
  gr->GetYaxis()->SetTitle("QE");
  gr->SetTitle("");
  return gr;
}

void CbmRichPmt::ClearMap()
{
  for (map<CbmRichPmtTypeEnum, CbmRichPmtQEData*>::iterator it = fPmtDataMap.begin(); it != fPmtDataMap.end(); it++) {
    if (nullptr != it->second) { delete it->second; }
  }
  fPmtDataMap.clear();
}

void CbmRichPmt::InitQE()
{
  // See CbmRichPmtType.h for on details about each Pmt detector type

  LOG(debug) << "CbmRichPmt init QE";


  ClearMap();

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCosy17NoWls;
    data->fLambdaMin       = 170.;
    data->fLambdaMax       = 800.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {
      0.,         0.0373,     0.0768,     0.104238,   0.130168,   0.162537,   0.196098,   0.219972,
      0.238386,   0.257846,   0.27616,    0.300536,   0.315942,   0.323639,   0.327806,   0.329745,
      0.330278,   0.330734,   0.330734,   0.327052,   0.325578,   0.320677,   0.316751,   0.309921,
      0.301695,   0.294525,   0.284986,   0.272276,   0.255125,   0.23644,    0.223164,   0.210214,
      0.200552,   0.183554,   0.14927,    0.118152,   0.10057,    0.0880096,  0.0707358,  0.0612509,
      0.0527653,  0.0464113,  0.0378286,  0.0296114,  0.022314,   0.0150159,  0.00877741, 0.003258,
      0.00178254, 0.00248491, 0.00353476, 0.00327258, 0.00367864, 0.00352639, 0.00288406, 0.00317378,
      0.00255156, 0.0024984,  0.00256586, 0.00174571, 0.00201436, 0.00212824, 0.00221443, 0.00254726};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCosy17WithWls;
    data->fLambdaMin       = 170.;
    data->fLambdaMax       = 800.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {
      0.15,       0.1873,     0.2268,     0.281283,   0.319146,   0.318419,   0.322942,   0.310789,
      0.296113,   0.308306,   0.320059,   0.33156,    0.338313,   0.332637,   0.335656,   0.334134,
      0.332542,   0.329217,   0.329217,   0.329065,   0.324032,   0.317459,   0.306133,   0.307902,
      0.300078,   0.293836,   0.28501,    0.27188,    0.254544,   0.237351,   0.225093,   0.213887,
      0.203469,   0.184299,   0.149514,   0.120047,   0.105304,   0.0954967,  0.0765149,  0.0673676,
      0.0601531,  0.0536015,  0.0442024,  0.0359242,  0.0276292,  0.0193571,  0.0113956,  0.00387145,
      0.00455862, 0.00264693, 0.00440582, 0.00444134, 0.00523567, 0.00505885, 0.00397938, 0.00436081,
      0.0035522,  0.00354125, 0.00355477, 0.00232293, 0.0030541,  0.00306825, 0.00311562, 0.00358131};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeProtvino;
    data->fLambdaMin       = 100.;
    data->fLambdaMax       = 700.;
    data->fLambdaStep      = 20.;

    data->fEfficiency = {0.216, 0.216, 0.216, 0.216, 0.216, 0.216, 0.216, 0.216, 0.216, 0.216,
                         0.216, 0.227, 0.23,  0.227, 0.216, 0.2,   0.176, 0.15,  0.138, 0.1,
                         0.082, 0.06,  0.044, 0.032, 0.022, 0.015, 0.01,  0.006, 0.004};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCsi;
    data->fLambdaMin       = 130.;
    data->fLambdaMax       = 210.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.45, 0.4, 0.35, 0.32, 0.25, 0.2, 0.1, 0.03};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeH8500;
    data->fLambdaMin       = 260.;
    data->fLambdaMax       = 740.;
    data->fLambdaStep      = 20.;


    data->fEfficiency = {0.06, 0.12, 0.2,   0.22, 0.22,  0.22,  0.21,  0.2,    0.18,   0.16,    0.14,    0.11,
                         0.1,  0.06, 0.047, 0.03, 0.021, 0.012, 0.006, 0.0023, 0.0008, 0.00022, 0.00007, 0.00002};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeH8500_03;
    data->fLambdaMin       = 200.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 20.;

    data->fEfficiency = {0.095, 0.13, 0.16, 0.2,  0.23, 0.24,  0.25,  0.25, 0.24,  0.24,  0.23,
                         0.22,  0.2,  0.16, 0.14, 0.1,  0.065, 0.045, 0.02, 0.017, 0.007, 0.0033};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeH8500WithWls;
    data->fLambdaMin       = 160.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 20.;

    data->fEfficiency = {0.1,  0.2,  0.2, 0.2,  0.2,  0.2, 0.23,  0.24,  0.25, 0.25,  0.24,  0.24,
                         0.23, 0.22, 0.2, 0.16, 0.14, 0.1, 0.065, 0.045, 0.02, 0.017, 0.007, 0.0033};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern11H8500_6;
    data->fLambdaMin       = 160.;
    data->fLambdaMax       = 700.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.0,    0.0,    0.0324, 0.0586, 0.0945, 0.1061, 0.1265, 0.1482, 0.1668, 0.1887, 0.2093,
                         0.2134, 0.2303, 0.2482, 0.2601, 0.2659, 0.2702, 0.283,  0.2863, 0.2863, 0.2884, 0.286,
                         0.2811, 0.2802, 0.272,  0.2638, 0.2562, 0.2472, 0.2368, 0.2218, 0.2032, 0.186,  0.1735,
                         0.1661, 0.1483, 0.121,  0.0959, 0.0782, 0.0647, 0.0538, 0.0372, 0.0296, 0.0237, 0.0176,
                         0.0123, 0.0083, 0.005,  0.003,  0.0017, 0.0008, 0.0006, 0.0003, 0.0003, 0.0002, 0.0001};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern11H8500_10;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.178, 0.200, 0.218, 0.222, 0.226, 0.228, 0.214, 0.210, 0.229, 0.231, 0.244, 0.253,
                         0.259, 0.263, 0.266, 0.277, 0.280, 0.274, 0.275, 0.270, 0.264, 0.263, 0.254, 0.246,
                         0.239, 0.229, 0.219, 0.207, 0.193, 0.179, 0.161, 0.149, 0.135, 0.117, 0.103, 0.082,
                         0.065, 0.056, 0.036, 0.030, 0.024, 0.018, 0.013, 0.009, 0.006, 0.004, 0.002};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern11H8500_11;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.202, 0.207, 0.210, 0.214, 0.218, 0.219, 0.206, 0.202, 0.220, 0.222, 0.235, 0.243,
                         0.249, 0.253, 0.256, 0.266, 0.270, 0.264, 0.265, 0.260, 0.254, 0.253, 0.244, 0.237,
                         0.229, 0.221, 0.210, 0.199, 0.186, 0.172, 0.155, 0.143, 0.129, 0.113, 0.099, 0.079,
                         0.063, 0.054, 0.035, 0.028, 0.023, 0.018, 0.013, 0.009, 0.006, 0.004, 0.002};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern11H8500_12;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;


    data->fEfficiency = {0.060, 0.080, 0.096, 0.109, 0.130, 0.152, 0.172, 0.194, 0.214, 0.218, 0.235, 0.253,
                         0.265, 0.271, 0.275, 0.288, 0.291, 0.292, 0.294, 0.292, 0.287, 0.286, 0.278, 0.269,
                         0.262, 0.252, 0.242, 0.227, 0.208, 0.178, 0.170, 0.155, 0.129, 0.102, 0.083, 0.069,
                         0.058, 0.041, 0.033, 0.027, 0.020, 0.015, 0.010, 0.006, 0.004, 0.002, 0.001};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeR11265_13;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.,    0.,    0.,    0.,    0.,    0.,    0.,    0.,    0.,    0.,    0.315, 0.344,
                         0.366, 0.378, 0.384, 0.400, 0.403, 0.404, 0.407, 0.403, 0.396, 0.395, 0.383, 0.370,
                         0.359, 0.347, 0.331, 0.310, 0.285, 0.263, 0.244, 0.232, 0.213, 0.182, 0.151, 0.126,
                         0.106, 0.092, 0.069, 0.060, 0.051, 0.042, 0.034, 0.026, 0.019, 0.014, 0.009};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeR11265_14;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.239, 0.294, 0.332, 0.351, 0.352, 0.338, 0.303, 0.286, 0.307, 0.307, 0.324, 0.340,
                         0.354, 0.364, 0.371, 0.390, 0.389, 0.392, 0.395, 0.393, 0.388, 0.388, 0.378, 0.367,
                         0.358, 0.347, 0.333, 0.310, 0.384, 0.265, 0.248, 0.238, 0.220, 0.188, 0.150, 0.123,
                         0.104, 0.089, 0.068, 0.058, 0.050, 0.041, 0.033, 0.025, 0.018, 0.013, 0.008};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern12H8500_15;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.037, 0.063, 0.103, 0.110, 0.131, 0.153, 0.172, 0.195, 0.215, 0.217, 0.232, 0.249,
                         0.261, 0.267, 0.271, 0.285, 0.286, 0.285, 0.287, 0.285, 0.280, 0.279, 0.272, 0.264,
                         0.256, 0.248, 0.239, 0.223, 0.204, 0.189, 0.177, 0.170, 0.155, 0.130, 0.105, 0.087,
                         0.073, 0.060, 0.041, 0.033, 0.027, 0.020, 0.015, 0.010, 0.006, 0.004, 0.003};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern12H8500_16;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.202, 0.240, 0.269, 0.277, 0.279, 0.273, 0.245, 0.228, 0.243, 0.243, 0.253, 0.259,
                         0.262, 0.263, 0.265, 0.278, 0.279, 0.281, 0.283, 0.281, 0.277, 0.275, 0.267, 0.260,
                         0.253, 0.245, 0.234, 0.219, 0.201, 0.187, 0.175, 0.167, 0.150, 0.124, 0.098, 0.080,
                         0.066, 0.055, 0.040, 0.033, 0.026, 0.020, 0.014, 0.010, 0.006, 0.004, 0.002};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern12H10966A_17;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.007, 0.040, 0.085, 0.103, 0.130, 0.160, 0.186, 0.215, 0.244, 0.256, 0.281, 0.310,
                         0.332, 0.344, 0.355, 0.376, 0.382, 0.386, 0.390, 0.390, 0.387, 0.386, 0.376, 0.365,
                         0.356, 0.345, 0.328, 0.302, 0.278, 0.257, 0.241, 0.227, 0.191, 0.153, 0.128, 0.112,
                         0.098, 0.085, 0.064, 0.055, 0.047, 0.039, 0.030, 0.023, 0.017, 0.011, 0.007};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeCern12H10966A_18;
    data->fLambdaMin       = 180.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 10.;

    data->fEfficiency = {0.241, 0.304, 0.351, 0.364, 0.368, 0.357, 0.311, 0.279, 0.299, 0.304, 0.321, 0.329,
                         0.336, 0.342, 0.350, 0.370, 0.374, 0.379, 0.383, 0.384, 0.381, 0.382, 0.372, 0.362,
                         0.354, 0.344, 0.327, 0.300, 0.275, 0.259, 0.244, 0.231, 0.195, 0.155, 0.130, 0.113,
                         0.097, 0.083, 0.065, 0.055, 0.046, 0.038, 0.030, 0.022, 0.016, 0.011, 0.007};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }

  {
    CbmRichPmtQEData* data = new CbmRichPmtQEData();
    data->fDetectorType    = CbmRichPmtTypeIdeal;
    data->fLambdaMin       = 160.;
    data->fLambdaMax       = 640.;
    data->fLambdaStep      = 20.;

    data->fEfficiency = {0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99,
                         0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 0.99};

    fPmtDataMap.insert(pair<CbmRichPmtTypeEnum, CbmRichPmtQEData*>(data->fDetectorType, data));
  }
}
ClassImp(CbmRichPmt)
