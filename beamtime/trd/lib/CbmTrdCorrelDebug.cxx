#include "CbmTrdRawBeamProfile.h"

#include "CbmSpadicRawMessage.h"
#include "CbmNxyterRawMessage.h"
#include "CbmTrbRawMessage.h"
#include "CbmTrdDigi.h"
#include "CbmTrdCluster.h"
#include "CbmHistManager.h"
#include "CbmBeamDefaults.h"
#include "CbmTrdAddress.h"
#include "CbmTrdDaqBuffer.h"

#include "FairLogger.h"

#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TString.h"
#include "TStyle.h"

#include <cmath>
#include <map>
//#include <multimap>
#include <vector>
// ---- Default constructor -------------------------------------------
CbmTrdCorrelDebug::CbmTrdCorrelDebug()
  : FairTask("CbmTrdCorrelDebug"),
    fRawSpadic(NULL),
    fNxyterRaw(NULL),
    fTrbRaw(NULL),
    fDigis(NULL),
    fClusters(NULL),
    fiDigi(0),
    fiCluster(0),
    fHM(new CbmHistManager()),
    fmaxTimeGlobal(0),
    fmaxTimeGroup(),
    fmaxFullTimeGlobal(0),
    fmaxFullTimeGroup(),
    flastDlmTriggerTime(),
    fEpoch(-1),
    flastEpoch(-1),
    fSuperEpoch(-1),
    flastSuperEpoch(-1),
    fSpadicMessageCounter(0),
    fNxyterMessageCounter(0),
    fTrbMessageCounter(0),
    fContainerCounter(0),
    fInfoCounter(0),
    fHitCounter(0),
    fMultiHitCounter(0),
    fErrorCounter(0),
    fLostHitCounter(0),
    fDoubleCounter(0),
    fFragmentedCounter(0),
    fSys0Spa0Buffer(),
    fSys0Spa1Buffer(),
    fTimeBuffer(),
    fFullTimeBuffer(),
    fFullTimeBufferAll(),
    fTrbBuffer()
{
  LOG(debug) << "Default Constructor of CbmTrdCorrelDebug";
}
