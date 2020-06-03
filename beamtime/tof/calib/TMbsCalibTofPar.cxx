// ------------------------------------------------------------------
// -----                     TMbsCalibTofPar                   -----
// -----              Created 08/05/2013 by P.-A. Loizeau       -----
// ------------------------------------------------------------------
#include "TMbsCalibTofPar.h"

// TOF headers
#include "TofTdcDef.h"
#include "TofCaenDef.h"
#include "TofVftxDef.h"
#include "TofTrbTdcDef.h"
#include "TofGet4Def.h"

// FAIR headers
#include "FairLogger.h"
#include "FairParamList.h" 

// ROOT headers
#include "TString.h"

ClassImp(TMbsCalibTofPar)

TMbsCalibTofPar::TMbsCalibTofPar() :
    FairParGenericSet("TMbsCalibTofPar","Tof MBS Calibration Parameters","TestDefaultContext"),
    fuCalibDebug(0),
    fuCalibScalRefClk(0),
    fiMinNbHitsCalib(0),
    fiFactMinNbHitsOnlyNewCalib(0),
    fsInitialCalibrationFilename(""),
    fiNbVftxCalib(0),
    fiNbCaenCalib(0),
    fiNbTrb3Calib(0),
    fiNbGet4Calib(0),
    fiInitialCalIndexVftx(),
    fiInitialCalIndexCaen(),
    fiInitialCalIndexTrb3(),
    fiInitialCalIndexGet4(),
    fuEnableCalibOutput(0),
    fuSingleChannelCalibFilesEnable(0),
    fuSingleChannelCalibFilesOutput(0),
    fuUseCoarseCorrectedTime(0),
    fiNbVftxOffset(0),
    fiNbCaenOffset(0),
    fiNbTrb3Offset(0),
    fiNbGet4Offset(0),
    fiBoardIndexVftx(),
    fiBoardIndexCaen(),
    fiBoardIndexTrb3(),
    fiBoardIndexGet4(),
    fuEnableTimeOffsetCalibration(0),
    fdTimeOffsetVftx(),
    fdTimeOffsetCaen(),
    fdTimeOffsetTrb3(),
    fdTimeOffsetGet4(),
    fdMeanTimeOffsetVftx(),
    fdMeanTimeOffsetCaen(),
    fdMeanTimeOffsetTrb3(),
    fdMeanTimeOffsetGet4(),
    fuEnableTotCalibration(0),
    fdTotOffsetVftx(),
    fdTotOffsetCaen(),
    fdTotOffsetTrb3(),
    fdTotOffsetGet4(),
    fdMeanTotOffsetVftx(),
    fdMeanTotOffsetCaen(),
    fdMeanTotOffsetTrb3(),
    fdMeanTotOffsetGet4(),
    fuManualTdcOffsetEnable(0),
    fiTdcToTdcOffsetVftx(),
    fiTdcToTdcOffsetCaen(),
    fiTdcToTdcOffsetTrb3(),
    fiTdcToTdcOffsetGet4(),
    fuAutomaticOffsetEnable(0),
    fiMainReferenceTdcVftx(-1),
    fiMainReferenceTdcCaen(-1),
    fiMainReferenceTdcTrb3(-1),
    fiMainReferenceTdcGet4(-1),
    fiTotModeVftx(0),
    fiTotModeCaen(0),
    fiTotModeTrb3(0),
    fiTotModeGet4(0),
    fiNbVftxTotInvFlag(0),
    fiNbCaenTotInvFlag(0),
    fiNbTrb3TotInvFlag(0),
    fiNbGet4TotInvFlag(0),
    fiTotInversionFlagVftx(),
    fiTotInversionFlagCaen(),
    fiTotInversionFlagTrb3(),
    fiTotInversionFlagGet4(),
    fuTimeHistEnable(0),
    fuTimeHistSinglesEnable(0),
    fuTotHistoEnable(0),
    fuMultiHitsDistanceHistoEnable(0),
    fuTdcOffsetEnable(0),
    fiTdcOffsetEnaFlagVftx(),
    fiTdcOffsetEnaFlagCaen(),
    fiTdcOffsetEnaFlagTrb3(),
    fiTdcOffsetEnaFlagGet4(),
    fuTdcOffsetMainTdc(0),
    fuTrbOffsetLinearEnable(0),
    fiChannelResolutionTest(0)
{
}

TMbsCalibTofPar::TMbsCalibTofPar(const char* name,
                                  const char* title,
                                  const char* context ) : 
    FairParGenericSet(name,title,context),
    fuCalibDebug(0),
    fuCalibScalRefClk(0),
    fiMinNbHitsCalib(0),
    fiFactMinNbHitsOnlyNewCalib(0),
    fsInitialCalibrationFilename(""),
    fiNbVftxCalib(0),
    fiNbCaenCalib(0),
    fiNbTrb3Calib(0),
    fiNbGet4Calib(0),
    fiInitialCalIndexVftx(),
    fiInitialCalIndexCaen(),
    fiInitialCalIndexTrb3(),
    fiInitialCalIndexGet4(),
    fuEnableCalibOutput(0),
    fuSingleChannelCalibFilesEnable(0),
    fuSingleChannelCalibFilesOutput(0),
    fuUseCoarseCorrectedTime(0),
    fiNbVftxOffset(0),
    fiNbCaenOffset(0),
    fiNbTrb3Offset(0),
    fiNbGet4Offset(0),
    fiBoardIndexVftx(),
    fiBoardIndexCaen(),
    fiBoardIndexTrb3(),
    fiBoardIndexGet4(),
    fuEnableTimeOffsetCalibration(0),
    fdTimeOffsetVftx(),
    fdTimeOffsetCaen(),
    fdTimeOffsetTrb3(),
    fdTimeOffsetGet4(),
    fdMeanTimeOffsetVftx(),
    fdMeanTimeOffsetCaen(),
    fdMeanTimeOffsetTrb3(),
    fdMeanTimeOffsetGet4(),
    fuEnableTotCalibration(0),
    fdTotOffsetVftx(),
    fdTotOffsetCaen(),
    fdTotOffsetTrb3(),
    fdTotOffsetGet4(),
    fdMeanTotOffsetVftx(),
    fdMeanTotOffsetCaen(),
    fdMeanTotOffsetTrb3(),
    fdMeanTotOffsetGet4(),
    fuManualTdcOffsetEnable(0),
    fiTdcToTdcOffsetVftx(),
    fiTdcToTdcOffsetCaen(),
    fiTdcToTdcOffsetTrb3(),
    fiTdcToTdcOffsetGet4(),
    fuAutomaticOffsetEnable(0),
    fiMainReferenceTdcVftx(-1),
    fiMainReferenceTdcCaen(-1),
    fiMainReferenceTdcTrb3(-1),
    fiMainReferenceTdcGet4(-1),
    fiTotModeVftx(0),
    fiTotModeCaen(0),
    fiTotModeTrb3(0),
    fiTotModeGet4(0),
    fiNbVftxTotInvFlag(0),
    fiNbCaenTotInvFlag(0),
    fiNbTrb3TotInvFlag(0),
    fiNbGet4TotInvFlag(0),
    fiTotInversionFlagVftx(),
    fiTotInversionFlagCaen(),
    fiTotInversionFlagTrb3(),
    fiTotInversionFlagGet4(),
    fuTimeHistEnable(0),
    fuTimeHistSinglesEnable(0),
    fuTotHistoEnable(0),
    fuMultiHitsDistanceHistoEnable(0),
    fuTdcOffsetEnable(0),
    fiTdcOffsetEnaFlagVftx(),
    fiTdcOffsetEnaFlagCaen(),
    fiTdcOffsetEnaFlagTrb3(),
    fiTdcOffsetEnaFlagGet4(),
    fuTdcOffsetMainTdc(0),
    fuTrbOffsetLinearEnable(0),
    fiChannelResolutionTest(0)
{
}


TMbsCalibTofPar::~TMbsCalibTofPar()
{
}

void TMbsCalibTofPar::clear(void)
{
	  status = kFALSE;
	  resetInputVersions();
}

void TMbsCalibTofPar::putParams(FairParamList* l)
{
  if (!l) { return; }

  l->add("CalibDebug",            (Int_t)fuCalibDebug);
  l->add("CalibScalRefClk",       (Int_t)fuCalibScalRefClk);
  l->add("MinNbHitsCalib",        fiMinNbHitsCalib);
  l->add("FactMinNbOnlyNewCalib", fiFactMinNbHitsOnlyNewCalib);
  l->add("InitialCalibFilename",  fsInitialCalibrationFilename);
  l->add("NbVftxCalib",           fiNbVftxCalib);
  l->add("NbCaenCalib",           fiNbCaenCalib);
  l->add("NbTrb3Calib",           fiNbTrb3Calib);
  l->add("NbGet4Calib",           fiNbGet4Calib);
  l->add("InitialCalIndexVftx",   fiInitialCalIndexVftx);
  l->add("InitialCalIndexCaen",   fiInitialCalIndexCaen);
  l->add("InitialCalIndexTrb3",   fiInitialCalIndexTrb3);
  l->add("InitialCalIndexGet4",   fiInitialCalIndexGet4);
  l->add("EnableCalibOutput",     (Int_t)fuEnableCalibOutput);
  l->add("SingleChCalibFilesEna", (Int_t)fuSingleChannelCalibFilesEnable);
  l->add("SingleChCalibFilesOut", (Int_t)fuSingleChannelCalibFilesOutput);
  l->add("UseCoarseCorrTime",     (Int_t)fuUseCoarseCorrectedTime);
  l->add("NbVftxOffset",          fiNbVftxOffset);
  l->add("NbCaenOffset",          fiNbCaenOffset);
  l->add("NbTrb3Offset",          fiNbTrb3Offset);
  l->add("NbGet4Offset",          fiNbGet4Offset);
  l->add("BoardIndexVftx",        fiBoardIndexVftx);
  l->add("BoardIndexCaen",        fiBoardIndexCaen);
  l->add("BoardIndexTrb3",        fiBoardIndexTrb3);
  l->add("BoardIndexGet4",        fiBoardIndexGet4);
  l->add("EnableTimeOffCalib",    (Int_t)fuEnableTimeOffsetCalibration);
  l->add("TimeOffsetCaen",        fdTimeOffsetCaen);
  l->add("TimeOffsetVftx",        fdTimeOffsetVftx);
  l->add("TimeOffsetTrb3",        fdTimeOffsetTrb3);
  l->add("TimeOffsetGet4",        fdTimeOffsetGet4);
  l->add("MeanTimeOffsetCaen",    fdMeanTimeOffsetCaen);
  l->add("MeanTimeOffsetVftx",    fdMeanTimeOffsetVftx);
  l->add("MeanTimeOffsetTrb3",    fdMeanTimeOffsetTrb3);
  l->add("MeanTimeOffsetGet4",    fdMeanTimeOffsetGet4);
  l->add("EnableTotCalibration",  (Int_t)fuEnableTotCalibration);
  l->add("TotOffsetVftx",         fdTotOffsetVftx);
  l->add("TotOffsetCaen",         fdTotOffsetCaen);
  l->add("TotOffsetTrb3",         fdTotOffsetTrb3);
  l->add("TotOffsetGet4",         fdTotOffsetGet4);
  l->add("MeanTotOffsetVftx",     fdMeanTotOffsetVftx);
  l->add("MeanTotOffsetCaen",     fdMeanTotOffsetCaen);
  l->add("MeanTotOffsetTrb3",     fdMeanTotOffsetTrb3);
  l->add("MeanTotOffsetGet4",     fdMeanTotOffsetGet4);
  l->add("ManualTdcOffsetEnable", (Int_t)fuManualTdcOffsetEnable);
  l->add("AutomaticOffsetEnable", (Int_t)fuAutomaticOffsetEnable);
  l->add("MainReferenceTdcVftx",  fiMainReferenceTdcVftx);
  l->add("MainReferenceTdcCaen",  fiMainReferenceTdcCaen);
  l->add("MainReferenceTdcTrb3",  fiMainReferenceTdcTrb3);
  l->add("MainReferenceTdcGet4",  fiMainReferenceTdcGet4);
  l->add("TdcToTdcOffsetVftx",    fiTdcToTdcOffsetVftx);
  l->add("TdcToTdcOffsetCaen",    fiTdcToTdcOffsetCaen);
  l->add("TdcToTdcOffsetTrb3",    fiTdcToTdcOffsetTrb3);
  l->add("TdcToTdcOffsetGet4",    fiTdcToTdcOffsetGet4);
  l->add("TotModeVftx",           fiTotModeVftx);
  l->add("TotModeCaen",           fiTotModeCaen);
  l->add("TotModeTrb3",           fiTotModeTrb3);
  l->add("TotModeGet4",           fiTotModeGet4);
  l->add("NbVftxTotInvFlag",      fiNbVftxTotInvFlag);
  l->add("NbCaenTotInvFlag",      fiNbCaenTotInvFlag);
  l->add("NbTrb3TotInvFlag",      fiNbTrb3TotInvFlag);
  l->add("NbGet4TotInvFlag",      fiNbGet4TotInvFlag);
  l->add("TotInversionFlagVftx",  fiTotInversionFlagVftx);
  l->add("TotInversionFlagCaen",  fiTotInversionFlagCaen);
  l->add("TotInversionFlagTrb3",  fiTotInversionFlagTrb3);
  l->add("TotInversionFlagGet4",  fiTotInversionFlagGet4);
  l->add("TimeHistEnable",        (Int_t)fuTimeHistEnable);
  l->add("TimeHistSinglesEnable", (Int_t)fuTimeHistSinglesEnable);
  l->add("TotHistoEnable",        (Int_t)fuTotHistoEnable);
  l->add("MultiHitsDistHistoEna", (Int_t)fuMultiHitsDistanceHistoEnable);
  l->add("TdcOffsetEnable",       (Int_t)fuTdcOffsetEnable);
  l->add("TdcOffsetMainTdc",      (Int_t)fuTdcOffsetMainTdc);
  l->add("TdcOffsetEnaFlagVftx",  fiTdcOffsetEnaFlagVftx);
  l->add("TdcOffsetEnaFlagCaen",  fiTdcOffsetEnaFlagCaen);
  l->add("TdcOffsetEnaFlagTrb3",  fiTdcOffsetEnaFlagTrb3);
  l->add("TdcOffsetEnaFlagGet4",  fiTdcOffsetEnaFlagGet4);
  l->add("TrbOffsetLinearEnable", (Int_t)fuTrbOffsetLinearEnable);
  l->add("ChannelResolutionTest", fiChannelResolutionTest);
}

Bool_t TMbsCalibTofPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  
  /*
   * TODO PAL: Thing I am not sure is whether order in readout has to 
   * match order in the ascii file => to check
   */

  LOG(debug2)<<"Get the Tof MBS calib parameters.";
  
  Int_t iTempCaster = 0;
  if ( ! l->fill("CalibDebug", &iTempCaster) ) return kFALSE;
  fuCalibDebug = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("CalibScalRefClk", &iTempCaster) ) return kFALSE;
  fuCalibScalRefClk = (UInt_t)iTempCaster;
  
  if ( ! l->fill("MinNbHitsCalib",  &fiMinNbHitsCalib) ) return kFALSE;
  if ( ! l->fill("FactMinNbOnlyNewCalib",  &fiFactMinNbHitsOnlyNewCalib) ) return kFALSE;
  
  Int_t iMaxSizeFilename = 5000;
  Text_t *sTempText;
  sTempText = new Text_t[iMaxSizeFilename];
  if ( ! l->fill("InitialCalibFilename", sTempText, iMaxSizeFilename ) ) return kFALSE;
  fsInitialCalibrationFilename = sTempText;
  
  if ( ! l->fill("NbVftxCalib",  &fiNbVftxCalib) ) return kFALSE;
  if( 0<fiNbVftxCalib )
  {
     fiInitialCalIndexVftx.Set( fiNbVftxCalib );
     if ( ! l->fill("InitialCalIndexVftx",  &fiInitialCalIndexVftx) ) return kFALSE;
  } // if( 0<fiNbVftxCalib )
  
  if ( ! l->fill("NbCaenCalib",  &fiNbCaenCalib) ) return kFALSE;
  if( 0<fiNbCaenCalib )
  {
     fiInitialCalIndexCaen.Set( fiNbCaenCalib );
     if ( ! l->fill("InitialCalIndexCaen",  &fiInitialCalIndexCaen) ) return kFALSE;
  } // if( 0<fiNbCaenCalib )
  
  if ( ! l->fill("NbTrb3Calib",  &fiNbTrb3Calib) ) return kFALSE;
  if( 0<fiNbTrb3Calib )
  {
     fiInitialCalIndexTrb3.Set( fiNbTrb3Calib );
     if ( ! l->fill("InitialCalIndexTrb3",  &fiInitialCalIndexTrb3) ) return kFALSE;
  } // if( 0<fiNbTrb3Calib )
  
  if ( ! l->fill("NbGet4Calib",  &fiNbGet4Calib) ) return kFALSE;
  if( 0<fiNbGet4Calib )
  {
     fiInitialCalIndexGet4.Set( fiNbGet4Calib );
     if ( ! l->fill("InitialCalIndexGet4",  &fiInitialCalIndexGet4) ) return kFALSE;
  } // if( 0<fiNbGet4Calib )
  
  iTempCaster = 0;
  if ( ! l->fill("EnableCalibOutput", &iTempCaster) ) return kFALSE;
  fuEnableCalibOutput = (UInt_t)iTempCaster;
  
  if ( ! l->fill("SingleChCalibFilesEna", &iTempCaster) ) return kFALSE;
  fuSingleChannelCalibFilesEnable = (UInt_t)iTempCaster;
  
  if ( ! l->fill("SingleChCalibFilesOut", &iTempCaster) ) return kFALSE;
  fuSingleChannelCalibFilesOutput = (UInt_t)iTempCaster;
  
  if ( ! l->fill("UseCoarseCorrTime", &iTempCaster) ) return kFALSE;
  fuUseCoarseCorrectedTime = (UInt_t)iTempCaster;
    
  iTempCaster = 0;
  if ( ! l->fill("EnableTimeOffCalib", &iTempCaster) ) return kFALSE;
  fuEnableTimeOffsetCalibration = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("EnableTotCalibration", &iTempCaster) ) return kFALSE;
  fuEnableTotCalibration = (UInt_t)iTempCaster;
  
      // Vtfx -----------------------------------------------------------//
  if ( ! l->fill("NbVftxOffset",  &fiNbVftxOffset) ) return kFALSE;
  if( 0 < fiNbVftxOffset )
  {
     fiBoardIndexVftx.Set( fiNbVftxOffset );
     if ( ! l->fill("BoardIndexVftx",  &fiBoardIndexVftx) ) return kFALSE;
     
     fdTimeOffsetVftx.Set( fiNbVftxOffset * vftxtdc::kuNbChan/2 );
     if ( ! l->fill("TimeOffsetVftx",  &fdTimeOffsetVftx) ) return kFALSE;
  } // if( 0 < fiNbVftxOffset )
     
  fdMeanTimeOffsetVftx.Set( vftxtdc::kuNbChan/2 );
  if ( ! l->fill("MeanTimeOffsetVftx",  &fdMeanTimeOffsetVftx) ) return kFALSE;
     
  if( 0 < fiNbVftxOffset )
  {
     fdTotOffsetVftx.Set( fiNbVftxOffset * vftxtdc::kuNbChan/2 );
     if ( ! l->fill("TotOffsetVftx",  &fdTotOffsetVftx) ) return kFALSE;
  } // if( 0 < fiNbVftxOffset )
     
  fdMeanTotOffsetVftx.Set( vftxtdc::kuNbChan/2 );
  if ( ! l->fill("MeanTotOffsetVftx",  &fdMeanTotOffsetVftx) ) return kFALSE;
      // ----------------------------------------------------------------//
  
      // Caen -----------------------------------------------------------//
  if ( ! l->fill("NbCaenOffset",  &fiNbCaenOffset) ) return kFALSE;
  if( 0 < fiNbCaenOffset )
  {
     fiBoardIndexCaen.Set( fiNbCaenOffset );
     if ( ! l->fill("BoardIndexCaen",  &fiBoardIndexCaen) ) return kFALSE;

     fdTimeOffsetCaen.Set( fiNbCaenOffset * caentdc::kuNbChan );
     if ( ! l->fill("TimeOffsetCaen",  &fdTimeOffsetCaen) ) return kFALSE;
  } // if( 0 < fiNbCaenOffset )
     
  fdMeanTimeOffsetCaen.Set( caentdc::kuNbChan );
  if ( ! l->fill("MeanTimeOffsetCaen",  &fdMeanTimeOffsetCaen) ) return kFALSE;
     
  if( 0 < fiNbCaenOffset )
  {
     fdTotOffsetCaen.Set( fiNbCaenOffset * caentdc::kuNbChan );
     if ( ! l->fill("TotOffsetCaen",  &fdTotOffsetCaen) ) return kFALSE;
  } // if( 0 < fiNbCaenOffset )
     
  fdMeanTotOffsetCaen.Set( caentdc::kuNbChan );
  if ( ! l->fill("MeanTotOffsetCaen",  &fdMeanTotOffsetCaen) ) return kFALSE;
      // ----------------------------------------------------------------//
  
      // Trb3 -----------------------------------------------------------//
  if ( ! l->fill("NbTrb3Offset",  &fiNbTrb3Offset) ) return kFALSE;
  if( 0 < fiNbTrb3Offset )
  {
     fiBoardIndexTrb3.Set( fiNbTrb3Offset );
     if ( ! l->fill("BoardIndexTrb3",  &fiBoardIndexTrb3) ) return kFALSE;
     
     fdTimeOffsetTrb3.Set( fiNbTrb3Offset * trbtdc::kuNbChan/2 );
     if ( ! l->fill("TimeOffsetTrb3",  &fdTimeOffsetTrb3) ) return kFALSE;
  } // if( 0 < fiNbTrb3Offset )
     
  fdMeanTimeOffsetTrb3.Set( trbtdc::kuNbChan/2 );
  if ( ! l->fill("MeanTimeOffsetTrb3",  &fdMeanTimeOffsetTrb3) ) return kFALSE;
     
  if( 0 < fiNbTrb3Offset )
  {
     fdTotOffsetTrb3.Set( fiNbTrb3Offset * trbtdc::kuNbChan/2 );
     if ( ! l->fill("TotOffsetTrb3",  &fdTotOffsetTrb3) ) return kFALSE;
  } // if( 0 < fiNbTrb3Offset )
     
  fdMeanTotOffsetTrb3.Set( trbtdc::kuNbChan/2 );
  if ( ! l->fill("MeanTotOffsetTrb3",  &fdMeanTotOffsetTrb3) ) return kFALSE;
      // ----------------------------------------------------------------//
  
      // GET4 -----------------------------------------------------------//
  if ( ! l->fill("NbGet4Offset",  &fiNbGet4Offset) ) return kFALSE;
  if( 0 < fiNbGet4Offset )
  {
     fiBoardIndexGet4.Set( fiNbGet4Offset );
     if ( ! l->fill("BoardIndexGet4",  &fiBoardIndexGet4) ) return kFALSE;
     
     fdTimeOffsetGet4.Set( fiNbGet4Offset * get4tdc::kuNbChan/2 );
     if ( ! l->fill("TimeOffsetGet4",  &fdTimeOffsetGet4) ) return kFALSE;
  } // if( 0 < fiNbGet4Offset )
     
  fdMeanTimeOffsetGet4.Set( get4tdc::kuNbChan/2 );
  if ( ! l->fill("MeanTimeOffsetGet4",  &fdMeanTimeOffsetGet4) ) return kFALSE;
     
  if( 0 < fiNbGet4Offset )
  {
     fdTotOffsetGet4.Set( fiNbGet4Offset * get4tdc::kuNbChan/2 );
     if ( ! l->fill("TotOffsetGet4",  &fdTotOffsetGet4) ) return kFALSE;
  } // if( 0 < fiNbGet4Offset )
     
  fdMeanTotOffsetGet4.Set(  get4tdc::kuNbChan/2 );
  if ( ! l->fill("MeanTotOffsetGet4",  &fdMeanTotOffsetGet4) ) return kFALSE;
      // ----------------------------------------------------------------//
  
  iTempCaster = 0;
  if ( ! l->fill("ManualTdcOffsetEnable", &iTempCaster) ) return kFALSE;
  fuManualTdcOffsetEnable = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("AutomaticOffsetEnable", &iTempCaster) ) return kFALSE;
  fuAutomaticOffsetEnable = (UInt_t)iTempCaster;
  
  if ( ! l->fill("MainReferenceTdcVftx",  &fiMainReferenceTdcVftx) ) return kFALSE;
  if ( ! l->fill("MainReferenceTdcCaen",  &fiMainReferenceTdcCaen) ) return kFALSE;
  if ( ! l->fill("MainReferenceTdcTrb3",  &fiMainReferenceTdcTrb3) ) return kFALSE;
  if ( ! l->fill("MainReferenceTdcGet4",  &fiMainReferenceTdcGet4) ) return kFALSE;
  
  if( 0 < fiNbVftxCalib )
  {
     fiTdcToTdcOffsetVftx.Set( fiNbVftxCalib );
     if ( ! l->fill("TdcToTdcOffsetVftx",  &fiTdcToTdcOffsetVftx) ) return kFALSE;
  } // if( 0 < fiNbVftxCalib )
  if( 0 < fiNbCaenCalib )
  {
     fiTdcToTdcOffsetCaen.Set( fiNbCaenCalib );
     if ( ! l->fill("TdcToTdcOffsetCaen",  &fiTdcToTdcOffsetCaen) ) return kFALSE;
  } // if( 0 < fiNbCaenCalib )
  if( 0 < fiNbTrb3Calib )
  {
     fiTdcToTdcOffsetTrb3.Set( fiNbTrb3Calib );
     if ( ! l->fill("TdcToTdcOffsetTrb3",  &fiTdcToTdcOffsetTrb3) ) return kFALSE;
  } // if( 0 < fiNbTrb3Calib )
  if( 0 < fiNbGet4Calib )
  {
     fiTdcToTdcOffsetGet4.Set( fiNbGet4Calib );
     if ( ! l->fill("TdcToTdcOffsetGet4",  &fiTdcToTdcOffsetGet4) ) return kFALSE;
  } // if( 0 < fiNbGet4Calib )
  
  if ( ! l->fill("TotModeVftx",  &fiTotModeVftx) ) return kFALSE;
  if ( ! l->fill("TotModeCaen",  &fiTotModeCaen) ) return kFALSE;
  if ( ! l->fill("TotModeTrb3",  &fiTotModeTrb3) ) return kFALSE;
  if ( ! l->fill("TotModeGet4",  &fiTotModeGet4) ) return kFALSE;
  
  if ( ! l->fill("NbVftxTotInvFlag",  &fiNbVftxTotInvFlag) ) return kFALSE;
  if ( ! l->fill("NbCaenTotInvFlag",  &fiNbCaenTotInvFlag) ) return kFALSE;
  if ( ! l->fill("NbTrb3TotInvFlag",  &fiNbTrb3TotInvFlag) ) return kFALSE;
  if ( ! l->fill("NbGet4TotInvFlag",  &fiNbGet4TotInvFlag) ) return kFALSE;
  
  if( 0 < fiNbVftxTotInvFlag )
  {
     fiTotInversionFlagVftx.Set( fiNbVftxTotInvFlag * vftxtdc::kuNbChan/2 );
     if ( ! l->fill("TotInversionFlagVftx",  &fiTotInversionFlagVftx) ) return kFALSE;
  } // if( 0 < fiNbVftxTotInvFlag )
  
  if( 0 < fiNbCaenTotInvFlag )
  {
     fiTotInversionFlagCaen.Set( fiNbCaenTotInvFlag * caentdc::kuNbChan );
     if ( ! l->fill("TotInversionFlagCaen",  &fiTotInversionFlagCaen) ) return kFALSE;
  } // if( 0 < fiNbCaenTotInvFlag )
  if( 0 < fiNbTrb3TotInvFlag )
  {
     fiTotInversionFlagTrb3.Set( fiNbTrb3TotInvFlag * trbtdc::kuNbChan/2 );
     if ( ! l->fill("TotInversionFlagTrb3",  &fiTotInversionFlagTrb3) ) return kFALSE;
  } // if( 0 < fiNbTrb3TotInvFlag )
  if( 0 < fiNbGet4TotInvFlag )
  {
     fiTotInversionFlagGet4.Set( fiNbGet4TotInvFlag * get4tdc::kuNbChan/2 );
     if ( ! l->fill("TotInversionFlagGet4",  &fiTotInversionFlagGet4) ) return kFALSE;
  } // if( 0 < fiNbGet4TotInvFlag )
  
  
  iTempCaster = 0;
  if ( ! l->fill("TimeHistEnable", &iTempCaster) ) return kFALSE;
  fuTimeHistEnable = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("TimeHistSinglesEnable", &iTempCaster) ) return kFALSE;
  fuTimeHistSinglesEnable = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("TotHistoEnable", &iTempCaster) ) return kFALSE;
  fuTotHistoEnable = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("MultiHitsDistHistoEna", &iTempCaster) ) return kFALSE;
  fuMultiHitsDistanceHistoEnable = (UInt_t)iTempCaster;
  
  
  iTempCaster = 0;
  if ( ! l->fill("TdcOffsetEnable", &iTempCaster) ) return kFALSE;
  fuTdcOffsetEnable = (UInt_t)iTempCaster;

  // The parameter to enable the TDC offset on a single TDC basis is
  // used only if the offset is NOT enabled for all TDCs
  if( 0 == fuTdcOffsetEnable )
  {
     if( 0 < fiNbVftxCalib )
     {
        fiTdcOffsetEnaFlagVftx.Set( fiNbVftxCalib );
        if ( ! l->fill("TdcOffsetEnaFlagVftx",  &fiTdcOffsetEnaFlagVftx) ) return kFALSE;
     } // if( 0 < fiNbVftxCalib )
     if( 0 < fiNbCaenCalib )
     {
        fiTdcOffsetEnaFlagCaen.Set( fiNbCaenCalib );
        if ( ! l->fill("TdcOffsetEnaFlagCaen",  &fiTdcOffsetEnaFlagCaen) ) return kFALSE;
     } // if( 0 < fiNbCaenCalib )
     if( 0 < fiNbTrb3Calib )
     {
        fiTdcOffsetEnaFlagTrb3.Set( fiNbTrb3Calib );
        if ( ! l->fill("TdcOffsetEnaFlagTrb3",  &fiTdcOffsetEnaFlagTrb3) ) return kFALSE;
     } // if( 0 < fiNbTrb3Calib )
     if( 0 < fiNbGet4Calib )
     {
        fiTdcOffsetEnaFlagGet4.Set( fiNbGet4Calib );
        if ( ! l->fill("TdcOffsetEnaFlagGet4",  &fiTdcOffsetEnaFlagGet4) ) return kFALSE;
     } // if( 0 < fiNbGet4Calib )
  } // if( 0 == fuTdcOffsetEnable )

  iTempCaster = 0;
  if ( ! l->fill("TdcOffsetMainTdc", &iTempCaster) ) fuTdcOffsetMainTdc = 0;
     else fuTdcOffsetMainTdc = (UInt_t)iTempCaster;
  
  iTempCaster = 0;
  if ( ! l->fill("TrbOffsetLinearEnable", &iTempCaster) ) return kFALSE;
  fuTrbOffsetLinearEnable = (UInt_t)iTempCaster;

  if ( ! l->fill("ChannelResolutionTest", &fiChannelResolutionTest) ) return kFALSE;

//  printParams();
  
  return kTRUE;
}

void TMbsCalibTofPar::printParams()
{   
   LOG(info)<<"Parameter values in TMbsCalibTofPar: ";
   
   if(1 == fuCalibDebug)
      LOG(info)<<"  Calibration Debug                   ON!!!!!!";
      else LOG(info)<<"  Calibration Debug                   OFF";
      
   // Scaler boards reference clock frequency calibration
   if(1 == fuCalibScalRefClk)
      LOG(info)<<"  Scaler boards ref. clk freq. cal.:  ON";
      else LOG(info)<<"  Scaler boards ref. clk freq. cal.:  OFF";
      
   // Channel Bin to Time calibration
   if( 0 < fiMinNbHitsCalib )
   {
      LOG(info)<<"  Min nb hits for calib update  :     "<<fiMinNbHitsCalib;
      if( 0 < fiFactMinNbHitsOnlyNewCalib )
         LOG(info)<<"  Calib update  :                     Use Initial + New until "
                  <<fiMinNbHitsCalib*fiFactMinNbHitsOnlyNewCalib
                  <<" then only New data";
         else LOG(info)<<"  Calib update  :                     Always use Initial + New when available";
   } // if( -1 != fiMinNbHitsCalib )
      else LOG(info)<<"  Calib update  :                     Use Only Initial";
         
   if( kTRUE != fsInitialCalibrationFilename.EqualTo("") && kTRUE != fsInitialCalibrationFilename.EqualTo("-") )
   {
      LOG(info)<<"  Filename for initial calib hist:    "<<fsInitialCalibrationFilename;
      if( 0 < fiNbVftxCalib )
      {
         TString sTemp = "  VFTX FPGA TDC index:       |-      ";
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxCalib; iIndexVftx++)
            sTemp += Form("%3d ", iIndexVftx);
         LOG(info)<<sTemp;
            
         sTemp = "  Tdc initial calib. index:  |->     ";
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxCalib; iIndexVftx++)
            sTemp += Form("%3d ", fiInitialCalIndexVftx[iIndexVftx] );
         LOG(info)<<sTemp;
      }
      
      if( 0 < fiNbCaenCalib )
      {
         TString sTemp = "  CAEN V1290 TDC index:      |-      ";
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenCalib; iIndexCaen++)
            sTemp += Form("%3d ", iIndexCaen);
         LOG(info)<<sTemp;
            
         sTemp = "  Tdc initial calib. index:  |->     ";
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenCalib; iIndexCaen++)
            sTemp += Form("%3d ", fiInitialCalIndexCaen[iIndexCaen] );
         LOG(info)<<sTemp;
      }
      if( 0 < fiNbTrb3Calib )
      {
         TString sTemp = "  TRB3 FPGA TDC index:       |-      ";
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Calib; iIndexTrb3++)
            sTemp += Form("%3d ", iIndexTrb3);
         LOG(info)<<sTemp;
            
         sTemp = "  Tdc initial calib. index:  |->     ";
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Calib; iIndexTrb3++)
            sTemp += Form("%3d ", fiInitialCalIndexTrb3[iIndexTrb3] );
         LOG(info)<<sTemp;
      }
      if( 0 < fiNbGet4Calib )
      {
         TString sTemp = "  GET4 TDC index:            |-      ";
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Calib; iIndexGet4++)
            sTemp += Form("%3d ", iIndexGet4);
         LOG(info)<<sTemp;
            
         sTemp = "  Tdc initial calib. index:  |->     ";
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Calib; iIndexGet4++)
            sTemp += Form("%3d ", fiInitialCalIndexGet4[iIndexGet4] );
         LOG(info)<<sTemp;
      }
      
   } // if( filename not empty )
   
   if(1 == fuEnableCalibOutput)
      LOG(info)<<"  Calibration saved in root file:     ON";
      else LOG(info)<<"  Calibration saved in root file:     OFF";
   if(1 == fuSingleChannelCalibFilesEnable)
      LOG(info)<<"  Use single channel calib files:     ON";
      else LOG(info)<<"  Use single channel calib files:     OFF";
   switch( fuSingleChannelCalibFilesOutput )
   {
      case 0:
         LOG(info)<<"  Single channel calib files output:  OFF";
         break;
      case 1:
         LOG(info)<<"  Single channel calib files output:  New files from data";
         break;
      case 2:
         LOG(info)<<"  Single channel calib files output:  Update files with data";
         break;
      default:
         break;
   } // switch( fuSingleChannelCalibFilesOutput )
   
   // Coarse overflow correction?
   if( 1 == fuUseCoarseCorrectedTime )
      LOG(info)<<"  Coarse time cycle correc.:          ON";
      else LOG(info)<<"  Coarse time cycle correc.:          OFF";
   
   // Board related stuff
   if( 1 == fuEnableTimeOffsetCalibration ||
       1 == fuEnableTotCalibration )
   {
      if( 0 < fiNbVftxOffset )
      {
         TString sTemp = "  VFTX FPGA TDC slot index:  |-      ";
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxOffset; iIndexVftx++)
            sTemp += Form("%3d ", iIndexVftx);
         LOG(info)<<sTemp;
         
         sTemp = "  VFTX FPGA TDC board index: |->     ";
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxOffset; iIndexVftx++)
            sTemp += Form("%3d ", fiBoardIndexVftx[iIndexVftx]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbVftxOffset )
      
      if( 0 < fiNbCaenOffset )
      {
         TString sTemp = "CAEN 1290 TDC slot index:  |-      ";
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenOffset; iIndexCaen++)
            sTemp += Form("%3d ", iIndexCaen);
         LOG(info)<<sTemp;
         
         sTemp = "CAEN 1290 TDC board index: |->     ";
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenOffset; iIndexCaen++)
            sTemp += Form("%3d ", fiBoardIndexCaen[iIndexCaen]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbCaenOffset )
      
      if( 0 < fiNbTrb3Offset )
      {
         TString sTemp = "TRB3 TDC slot index:       |-      ";
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Offset; iIndexTrb3++)
            sTemp += Form("%3d ", iIndexTrb3);
         LOG(info)<<sTemp;
         
         sTemp = "TRB3 TDC board index:      |->     ";
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Offset; iIndexTrb3++)
            sTemp += Form("%3d ", fiBoardIndexTrb3[iIndexTrb3]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbTrb3Offset )
      
      if( 0 < fiNbGet4Offset )
      {
         TString sTemp = "GET4 TDC slot index:       |-      ";
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Offset; iIndexGet4++)
            sTemp += Form("%3d ", iIndexGet4);
         LOG(info)<<sTemp;
         
         sTemp = "GET4 TDC chip index:       |->     ";
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Offset; iIndexGet4++)
            sTemp += Form("%3d ", fiBoardIndexGet4[iIndexGet4]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbGet4Offset )
      
   } // if( 1 == fuEnableTimeOffsetCalibration || 1 == fuEnableTotCalibration )
   
   if( 1 == fuEnableTimeOffsetCalibration )
   {
      LOG(info)<<"  Time offset calibration:            ON";;
      if( 0 < fiNbVftxOffset )
      {
         LOG(info)<<"  ------- VFTX TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxOffset; iIndexVftx++)
            if( -1 < fiBoardIndexVftx[iIndexVftx] && fiBoardIndexVftx[iIndexVftx] < fiNbVftxOffset )
            {
               sTemp = Form("  Slot %02d Board #%02d Time offset: |-> ", iIndexVftx, fiBoardIndexVftx[iIndexVftx]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
                  sTemp += Form("%5.0f ", fdTimeOffsetVftx[ vftxtdc::kuNbChan/2 * fiBoardIndexVftx[iIndexVftx] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexVftx[iIndexVftx] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Time offset: |-> ", iIndexVftx );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5.0f ", fdMeanTimeOffsetVftx[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexVftx[iIndexVftx] )
         LOG(info);
      } // if( 0 < fiNbVftxOffset )
      
      if( 0 < fiNbCaenOffset )
      {
         LOG(info)<<"  ------- CAEN V1290 TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenOffset; iIndexCaen++)
            if( -1 < fiBoardIndexCaen[iIndexCaen] && fiBoardIndexCaen[iIndexCaen] < fiNbCaenOffset )
            {
               sTemp = Form("  Slot %02d Board #%02d Time offset: |-> ", iIndexCaen, fiBoardIndexCaen[iIndexCaen]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan; uIndexChannel++)
                  sTemp += Form("%5.0f ", fdTimeOffsetCaen[ caentdc::kuNbChan * fiBoardIndexCaen[iIndexCaen] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexCaen[iIndexCaen] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Time offset: |-> ", iIndexCaen );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5.0f ", fdMeanTimeOffsetCaen[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexCaen[iIndexCaen] )
         LOG(info);
      } // if( 0 < fiNbCaenOffset )
      if( 0 < fiNbTrb3Offset )
      {
         LOG(info)<<"  ------- TRB3 TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Offset; iIndexTrb3++)
            if( -1 < fiBoardIndexTrb3[iIndexTrb3] && fiBoardIndexTrb3[iIndexTrb3] < fiNbTrb3Offset )
            {
               sTemp = Form("  Slot %02d Board #%02d Time offset: |-> ", iIndexTrb3, fiBoardIndexTrb3[iIndexTrb3]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
                  sTemp += Form("%5.0f ", fdTimeOffsetTrb3[ trbtdc::kuNbChan/2 * fiBoardIndexTrb3[iIndexTrb3] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexTrb3[iIndexTrb3] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Time offset: |-> ", iIndexTrb3 );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5.0f ", fdMeanTimeOffsetTrb3[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexTrb3[iIndexTrb3] )
         LOG(info);
      } // if( 0 < fiNbTrb3Offset )
      if( 0 < fiNbGet4Offset )
      {
         LOG(info)<<"  ------- GET4 TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < get4tdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Offset; iIndexGet4++)
            if( -1 < fiBoardIndexGet4[iIndexGet4] && fiBoardIndexGet4[iIndexGet4] < fiNbGet4Offset )
            {
               sTemp = Form("  Slot %02d Board #%02d Time offset: |-> ", iIndexGet4, fiBoardIndexGet4[iIndexGet4]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < get4tdc::kuNbChan/2; uIndexChannel++)
                  sTemp += Form("%5.0f ", fdTimeOffsetGet4[ get4tdc::kuNbChan/2 * fiBoardIndexGet4[iIndexGet4] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexGet4[iIndexGet4] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Time offset: |-> ", iIndexGet4 );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < get4tdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5.0f ", fdMeanTimeOffsetGet4[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexGet4[iIndexGet4] )
         LOG(info);
      } // if( 0 < fiNbGet4Offset )
      
   } // if( 1 == uEnableTimeOffsetCalibration )
      else LOG(info)<<"  Time offset calibration:            OFF";;
      
   if( 1 == fuEnableTotCalibration)
   {
      LOG(info)<<"  Tot offset calibration:             ON";
      if( 0 < fiNbVftxOffset )
      {
         LOG(info)<<"  ------- VFTX TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxOffset; iIndexVftx++)
            if( -1 < fiBoardIndexVftx[iIndexVftx] && fiBoardIndexVftx[iIndexVftx] < fiNbVftxOffset )
            {
               sTemp = Form("  Slot %02d Board #%02d Tot offset:  |-> ", iIndexVftx, fiBoardIndexVftx[iIndexVftx]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
                  sTemp += Form("%5.0f ", fdTotOffsetVftx[ vftxtdc::kuNbChan/2 * fiBoardIndexVftx[iIndexVftx] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexVftx[iIndexVftx] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Tot offset:  |-> ", iIndexVftx );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5.0f ", fdMeanTotOffsetVftx[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexVftx[iIndexVftx] )
         LOG(info);
      } // if( 0 < fiNbVftxOffset )
      
      if( 0 < fiNbCaenOffset )
      {
         LOG(info)<<"  ------- CAEN V1290 TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenOffset; iIndexCaen++)
            if( -1 < fiBoardIndexCaen[iIndexCaen] && fiBoardIndexCaen[iIndexCaen] < fiNbCaenOffset )
            {
               sTemp = Form("  Slot %02d Board #%02d Tot offset:  |-> ", iIndexCaen, fiBoardIndexCaen[iIndexCaen]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan; uIndexChannel++)
                  sTemp += Form("%5.0f ", fdTotOffsetCaen[ caentdc::kuNbChan * fiBoardIndexCaen[iIndexCaen] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexCaen[iIndexCaen] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Tot offset:  |-> ", iIndexCaen );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5.0f ", fdMeanTotOffsetCaen[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexCaen[iIndexCaen] )
         LOG(info);
      } // if( 0 < fiNbCaenOffset )
      if( 0 < fiNbTrb3Offset )
      {
         LOG(info)<<"  ------- TRB3 TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Offset; iIndexTrb3++)
            if( -1 < fiBoardIndexTrb3[iIndexTrb3] && fiBoardIndexTrb3[iIndexTrb3] < fiNbTrb3Offset )
            {
               sTemp = Form("  Slot %02d Board #%02d Tot offset:  |-> ", iIndexTrb3, fiBoardIndexTrb3[iIndexTrb3]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
                  sTemp += Form("%5.0f ", fdTotOffsetTrb3[ trbtdc::kuNbChan/2 * fiBoardIndexTrb3[iIndexTrb3] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexTrb3[iIndexTrb3] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Tot offset:  |-> ", iIndexTrb3 );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5.0f ", fdMeanTotOffsetTrb3[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexTrb3[iIndexTrb3] )
         LOG(info);
      } // if( 0 < fiNbTrb3Offset )
      if( 0 < fiNbGet4Offset )
      {
         LOG(info)<<"  ------- GET4 TDC ------- ";;
         LOG(info)<<"  Channel:                       |-  ";
         TString sTemp="";
         for( UInt_t uIndexChannel = 0; uIndexChannel < 4 * get4tdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%5u ", uIndexChannel);
         LOG(info)<<sTemp;
         
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Offset; iIndexGet4++)
            if( -1 < fiBoardIndexGet4[iIndexGet4] && fiBoardIndexGet4[iIndexGet4] < fiNbGet4Offset )
            {
               sTemp = Form("  Slot %02d Board #%02d Tot offset:  |-> ", iIndexGet4, fiBoardIndexGet4[iIndexGet4]);
               for( UInt_t uIndexChannel = 0; uIndexChannel < get4tdc::kuNbChan/2; uIndexChannel++)
                  sTemp += Form("%5f ", fdTotOffsetGet4[ get4tdc::kuNbChan/2 * fiBoardIndexGet4[iIndexGet4] + uIndexChannel]);
               LOG(info)<<sTemp;
            } // if( -1 < fiBoardIndexGet4[iIndexGet4] )
               else
               {
                  sTemp = Form("  Slot %02d def. mean Tot offset:  |-> ", iIndexGet4 );
                  for( UInt_t uIndexChannel = 0; uIndexChannel < get4tdc::kuNbChan/2; uIndexChannel++)
                     sTemp += Form("%5f ", fdMeanTotOffsetGet4[uIndexChannel] );
                  LOG(info)<<sTemp;
               } // else of if( -1 < fiBoardIndexGet4[iIndexGet4] )
         LOG(info);
      } // if( 0 < fiNbGet4Offset )
      
   } // if( 1 == uEnableTotCalibration)
      else LOG(info)<<"  Tot offset calibration:             OFF";
      
   // TDC to TDC offsets
   if(1 == fuManualTdcOffsetEnable)
   {
      LOG(info)<<"  Manual tdc to tdc offset:           ON";
      if( 0 < fiNbVftxCalib )
      {
         TString sTemp = "  VFTX FPGA TDC index:       |-      ";
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxCalib; iIndexVftx++)
            sTemp += Form("%3d ", iIndexVftx);
         LOG(info)<<sTemp;
         
         sTemp = "  Manual offset val., clock: |->     ";
         for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxCalib; iIndexVftx++)
            sTemp += Form("%3d ", fiTdcToTdcOffsetVftx[iIndexVftx]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbVftxCalib )
      
      if( 0 < fiNbCaenCalib )
      {
         TString sTemp = "  VFTX FPGA TDC index:       |-      ";
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenCalib; iIndexCaen++)
            sTemp += Form("%3d ", iIndexCaen);
         LOG(info)<<sTemp;
         
         sTemp = "  Manual offset val., clock: |->     ";
         for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenCalib; iIndexCaen++)
            sTemp += Form("%3d ", fiTdcToTdcOffsetCaen[iIndexCaen]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbCaenCalib )
      if( 0 < fiNbTrb3Calib )
      {
         TString sTemp = "  VFTX FPGA TDC index:       |-      ";
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Calib; iIndexTrb3++)
            sTemp += Form("%3d ", iIndexTrb3);
         LOG(info)<<sTemp;
         
         sTemp = "  Manual offset val., clock: |->     ";
         for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Calib; iIndexTrb3++)
            sTemp += Form("%3d ", fiTdcToTdcOffsetTrb3[iIndexTrb3]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbTrb3Calib )
      if( 0 < fiNbGet4Calib )
      {
         TString sTemp = "  VFTX FPGA TDC index:       |-      ";
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Calib; iIndexGet4++)
            sTemp += Form("%3d ", iIndexGet4);
         LOG(info)<<sTemp;
         
         sTemp = "  Manual offset val., clock: |->     ";
         for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Calib; iIndexGet4++)
            sTemp += Form("%3d ", fiTdcToTdcOffsetGet4[iIndexGet4]);
         LOG(info)<<sTemp;
      } // if( 0 < fiNbGet4Calib )
      
   }
      else LOG(info)<<"  Manual tdc offset detection:        OFF";
   if(1 == fuAutomaticOffsetEnable)
   {
      LOG(info)<<"  Automatic tdc offset detection:     ON";
      LOG(info)<<"  Auto tdc offset det. main VFTX TDC: "<<fiMainReferenceTdcVftx;
      LOG(info)<<"  Auto tdc offset det. main CAEN TDC: "<<fiMainReferenceTdcCaen;
      LOG(info)<<"  Auto tdc offset det. main TRB3 TDC: "<<fiMainReferenceTdcTrb3;
      LOG(info)<<"  Auto tdc offset det. main GET4 TDC: "<<fiMainReferenceTdcGet4;
   }
      else LOG(info)<<"  Automatic tdc offset detection:     OFF";
     
   /** Tot calculation **/ 
   switch( fiTotModeVftx )
   {
      case 0:
         LOG(info)<<"  VFTX TOT calculation mode:          None";
         break;
      case 2:
         LOG(info)<<"  VFTX TOT calculation mode:          2 TDC channels per input channel (even = Falling, odd = Rising)";
         break;
      default:
         LOG(info)<<"  VFTX TOT calculation mode:          Wrong one => none";
         break;
   } // switch( fiTotModeVftx )
   switch( fiTotModeCaen )
   {
      case 0:
         LOG(info)<<"  CAEN TOT calculation mode:          None";
         break;
      case 1:
         LOG(info)<<"  CAEN TOT calculation mode:          Same TDC channel, diff data (Use edge flag)";
         break;
      case 2:
         LOG(info)<<"  CAEN TOT calculation mode:          2 input channels per front end channel (even = Falling, odd = Rising)";
         break;
      case 3:
         LOG(info)<<"  CAEN TOT calculation mode:          Same input channels in 2 consecutive boards (even = Rising, odd = Falling)";
         break;
      default:
         LOG(info)<<"  CAEN TOT calculation mode:          Wrong one => none";
         break;
   } // switch( fiTotModeCaen )
   switch( fiTotModeTrb3 )
   {
      case 0:
         LOG(info)<<"  TRB3 TOT calculation mode:          None";
         break;
      case 2:
         LOG(info)<<"  TRB3 TOT calculation mode:          2 TDC channels per input channel (even = Falling, odd = Rising)";
         break;
      default:
         LOG(info)<<"  TRB3 TOT calculation mode:          Wrong one => none";
         break;
   } // switch( fiTotModeTrb3 )
   switch( fiTotModeGet4 )
   {
      case 0:
         LOG(info)<<"  GET4 TOT calculation mode:          None";
         break;
      case 1:
         LOG(info)<<"  GET4 TOT calculation mode:          Same TDC channel, diff data (Use edge flag)";
         break;
      case 2:
         LOG(info)<<"  GET4 TOT calculation mode:          2 TDC channels per front end channel (even = Falling, odd = Rising)";
         break;
      case 4:
         LOG(info)<<"  GET4 TOT calculation mode:          Same TDC channel, same data";
         break;
      default:
         LOG(info)<<"  GET4 TOT calculation mode:          Wrong one => none";
         break;
   } // switch( fiTotModeVftx )
   if( 0 < fiTotModeVftx && 0 < fiNbVftxTotInvFlag )
   {
      LOG(info)<<"  VFTX TOT Inversion flags:";
      LOG(info)<<"  Channel:                    |-  ";
      TString sTemp="";
      for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
         sTemp += Form("%3u ", uIndexChannel);
      LOG(info)<<sTemp;
      
      for( Int_t iIndexTdc = 0; iIndexTdc < fiNbVftxTotInvFlag; iIndexTdc++)
      {
         sTemp = Form("  Slot %02d Tot Inversion flags: |-> ", iIndexTdc);
         for( UInt_t uIndexChannel = 0; uIndexChannel < vftxtdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%3d ", fiTotInversionFlagVftx[ vftxtdc::kuNbChan/2 * iIndexTdc + uIndexChannel]);
         LOG(info)<<sTemp;
      }  // for( Int_t iIndexTdc = 0; iIndexTdc < fiNbVftxTotInvFlag; iIndexTdc++)
      LOG(info);
   } // if( 0 < fiTotModeVftx && 0 < fiNbVftxTotInvFlag )
   
   if( 0 < fiTotModeCaen && 0 < fiNbCaenTotInvFlag )
   {
      LOG(info)<<"  CAEN TOT Inversion flags:";
      LOG(info)<<"  Channel:                    |-  ";
      TString sTemp="";
      for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan; uIndexChannel++)
         sTemp += Form("%3u ", uIndexChannel);
      LOG(info)<<sTemp;
      
      for( Int_t iIndexTdc = 0; iIndexTdc < fiNbCaenTotInvFlag; iIndexTdc++)
      {
         sTemp = Form("  Slot %02d Tot Inversion flags: |-> ", iIndexTdc);
         for( UInt_t uIndexChannel = 0; uIndexChannel < caentdc::kuNbChan; uIndexChannel++)
            sTemp += Form("%3d ", fiTotInversionFlagCaen[ caentdc::kuNbChan * iIndexTdc + uIndexChannel]);
         LOG(info)<<sTemp;
      }  // for( Int_t iIndexTdc = 0; iIndexTdc < fiNbCaenTotInvFlag; iIndexTdc++)
      LOG(info);
   } // if( 0 < fiTotModeCaen && 0 < fiNbCaenTotInvFlag )
   if( 0 < fiTotModeTrb3 && 0 < fiNbTrb3TotInvFlag )
   {
      LOG(info)<<"  TRB3 TOT Inversion flags:";
      LOG(info)<<"  Channel:                    |-  ";
      TString sTemp="";
      for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
         sTemp += Form("%3u ", uIndexChannel);
      LOG(info)<<sTemp;
      
      for( Int_t iIndexTdc = 0; iIndexTdc < fiNbTrb3TotInvFlag; iIndexTdc++)
      {
         sTemp = Form("  Slot %02d Tot Inversion flags: |-> ", iIndexTdc);
         for( UInt_t uIndexChannel = 0; uIndexChannel < trbtdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%3d ", fiTotInversionFlagTrb3[ trbtdc::kuNbChan/2 * iIndexTdc + uIndexChannel]);
         LOG(info)<<sTemp;
      }  // for( Int_t iIndexTdc = 0; iIndexTdc < fiNbTrb3TotInvFlag; iIndexTdc++)
      LOG(info);
   } // if( 0 < fiTotModeTrb3 && 0 < fiNbTrb3TotInvFlag )
   if( 0 < fiTotModeGet4 && 0 < fiNbGet4TotInvFlag )
   {
      LOG(info)<<"  GET4 TOT Inversion flags:";
      LOG(info)<<"  Channel:                    |-  ";
      TString sTemp="";
      for( UInt_t uIndexChannel = 0; uIndexChannel < get4tdc::kuNbChan/2; uIndexChannel++)
         sTemp += Form("%3u ", uIndexChannel);
      LOG(info)<<sTemp;
      
      for( Int_t iIndexTdc = 0; iIndexTdc < fiNbGet4TotInvFlag; iIndexTdc++)
      {
         sTemp = Form("  Slot %02d Tot Inversion flags: |-> ", iIndexTdc);
         for( UInt_t uIndexChannel = 0; uIndexChannel < get4tdc::kuNbChan/2; uIndexChannel++)
            sTemp += Form("%3d ", fiTotInversionFlagGet4[ get4tdc::kuNbChan/2 * iIndexTdc + uIndexChannel]);
         LOG(info)<<sTemp;
      }  // for( Int_t iIndexTdc = 0; iIndexTdc < fiNbGet4TotInvFlag; iIndexTdc++)
      LOG(info);
   } // if( 0 < fiTotModeGet4 && 0 < fiNbGet4TotInvFlag )
   
   
   // Histograms
   if( 1 == fuTimeHistEnable )
      LOG(info)<<"  Trigger - Calib. Time Histograms:   ON";
      else LOG(info)<<"  Trigger - Calib. Time Histograms:   OFF";
   if( 1 == fuTimeHistSinglesEnable )
      LOG(info)<<"  Trig. - Calib. Time Histo, Multi 1: ON";
      else LOG(info)<<"  Trig. - Calib. Time Histo, Multi 1: OFF";
   if( 1 == fuTotHistoEnable )
      LOG(info)<<"  Tot  Histograms               :     ON";
      else LOG(info)<<"  Tot  Histograms               :     OFF";
   if( 1 == fuMultiHitsDistanceHistoEnable )
      LOG(info)<<"  Histograms Distance Multi Hits:     ON";
      else LOG(info)<<"  Histograms Distance Multi Hits:     OFF";
   LOG(info);
   
   // TDC offsets
   if( 1 == fuTdcOffsetEnable )
   {
      LOG(info)<<"  TDC to TDC  offset event wise, all: ON";
   }
      else
      {
         LOG(info)<<"  TDC to TDC  offset event wise, all: OFF";
         LOG(info)<<"++TDC to TDC  offset event wise, per TDC:";
         if( 0 < fiNbVftxCalib )
         {
            TString sTemp = "  VFTX FPGA TDC index:       |-      ";
            for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxCalib; iIndexVftx++)
               sTemp += Form("%3d ", iIndexVftx);
            LOG(info)<<sTemp;

            sTemp = "  TDC w/ offset enabled:     |->     ";
            for( Int_t iIndexVftx = 0; iIndexVftx < fiNbVftxCalib; iIndexVftx++)
               sTemp += Form("%3d ", fiTdcOffsetEnaFlagVftx[iIndexVftx]);
            LOG(info)<<sTemp;
         } // if( 0 < fiNbVftxCalib )

         if( 0 < fiNbCaenCalib )
         {
            TString sTemp = "  VFTX FPGA TDC index:       |-      ";
            for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenCalib; iIndexCaen++)
               sTemp += Form("%3d ", iIndexCaen);
            LOG(info)<<sTemp;

            sTemp = "  TDC w/ offset enabled:     |->     ";
            for( Int_t iIndexCaen = 0; iIndexCaen < fiNbCaenCalib; iIndexCaen++)
               sTemp += Form("%3d ", fiTdcOffsetEnaFlagCaen[iIndexCaen]);
            LOG(info)<<sTemp;
         } // if( 0 < fiNbCaenCalib )
         if( 0 < fiNbTrb3Calib )
         {
            TString sTemp = "  VFTX FPGA TDC index:       |-      ";
            for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Calib; iIndexTrb3++)
               sTemp += Form("%3d ", iIndexTrb3);
            LOG(info)<<sTemp;

            sTemp = "  TDC w/ offset enabled:     |->     ";
            for( Int_t iIndexTrb3 = 0; iIndexTrb3 < fiNbTrb3Calib; iIndexTrb3++)
               sTemp += Form("%3d ", fiTdcOffsetEnaFlagTrb3[iIndexTrb3]);
            LOG(info)<<sTemp;
         } // if( 0 < fiNbTrb3Calib )
         if( 0 < fiNbGet4Calib )
         {
            TString sTemp = "  VFTX FPGA TDC index:       |-      ";
            for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Calib; iIndexGet4++)
               sTemp += Form("%3d ", iIndexGet4);
            LOG(info)<<sTemp;

            sTemp = "  TDC w/ offset enabled:     |->     ";
            for( Int_t iIndexGet4 = 0; iIndexGet4 < fiNbGet4Calib; iIndexGet4++)
               sTemp += Form("%3d ", fiTdcOffsetEnaFlagGet4[iIndexGet4]);
            LOG(info)<<sTemp;
         } // if( 0 < fiNbGet4Calib )
      } // else of if( 1 == fuTdcOffsetEnable )

   LOG(info)<<"  Main TDC for TDC offset w/ ref. ch: "<<fuTdcOffsetMainTdc;

   if( 1 == fuTrbOffsetLinearEnable )
      LOG(info)<<"  Linear calib. for TRB-TDC offsets:  Allowed";
      else LOG(info)<<"  Trigger - Calib. Time Histograms:   OFF";

   if( -1 <  fiChannelResolutionTest )
      LOG(info)<<"  Channel for TDC to TDC reso. check: "<<fiChannelResolutionTest;
      else LOG(info)<<"  TDC to TDC resolution check:   OFF";
   LOG(info);
   
   return;
}
// ----------------------------------------------------------------//
// Accessors
Int_t TMbsCalibTofPar::GetNbCalibBoards( UInt_t uType) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         return fiNbCaenCalib;
         break;
      case toftdc::vftx:
         return fiNbVftxCalib;
         break;
      case toftdc::trb:
         return fiNbTrb3Calib;
         break;
      case toftdc::get4:
         return fiNbGet4Calib;
         break;
      case toftdc::undef:
      default:
         return 0;
         break;
   } // switch( uType )
}
Int_t TMbsCalibTofPar::GetInitialCalInd( UInt_t uType, UInt_t uBoard ) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         if( static_cast<Int_t>(uBoard) < fiNbCaenCalib && 0 < fiInitialCalIndexCaen.GetSize() )
            return fiInitialCalIndexCaen[uBoard];
            else return -1;
         break;
      case toftdc::vftx:
         if( static_cast<Int_t>(uBoard) < fiNbVftxCalib && 0 < fiInitialCalIndexVftx.GetSize() )
            return fiInitialCalIndexVftx[uBoard];
            else return -1;
         break;
      case toftdc::trb:
         if( static_cast<Int_t>(uBoard) < fiNbTrb3Calib && 0 < fiInitialCalIndexTrb3.GetSize() )
            return fiInitialCalIndexTrb3[uBoard];
            else return -1;
         break;
      case toftdc::get4:
         if( static_cast<Int_t>(uBoard) < fiNbGet4Calib && 0 < fiInitialCalIndexGet4.GetSize() )
            return fiInitialCalIndexGet4[uBoard];
            else return -1;
         break;
      case toftdc::undef:
      default:
         return -1;
         break;
   } // switch( uType )
}
Int_t TMbsCalibTofPar::GetNbOffsetBoard( UInt_t uType) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         return fiNbCaenOffset;
         break;
      case toftdc::vftx:
         return fiNbVftxOffset;
         break;
      case toftdc::trb:
         return fiNbTrb3Offset;
         break;
      case toftdc::get4:
         return fiNbGet4Offset;
         break;
      case toftdc::undef:
      default:
         return 0;
         break;
   } // switch( uType )
}
Int_t TMbsCalibTofPar::GetOffsetBoardInd( UInt_t uType, UInt_t uSlot ) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         if( static_cast<Int_t>(uSlot) < fiNbCaenOffset && 0 < fiBoardIndexCaen.GetSize() )
            return fiBoardIndexCaen[uSlot];
            else return -1;
         break;
      case toftdc::vftx:
         if( static_cast<Int_t>(uSlot) < fiNbVftxOffset && 0 < fiBoardIndexVftx.GetSize() )
            return fiBoardIndexVftx[uSlot];
            else return -1;
         break;
      case toftdc::trb:
         if( static_cast<Int_t>(uSlot) < fiNbTrb3Offset && 0 < fiBoardIndexTrb3.GetSize() )
            return fiBoardIndexTrb3[uSlot];
            else return -1;
         break;
      case toftdc::get4:
         if( static_cast<Int_t>(uSlot) < fiNbGet4Offset && 0 < fiBoardIndexGet4.GetSize() )
            return fiBoardIndexGet4[uSlot];
            else return -1;
         break;
      case toftdc::undef:
      default:
         return -1;
         break;
   } // switch( uType )
}
Int_t TMbsCalibTofPar::GetTimeOffsetVal( UInt_t uType, UInt_t uSlot, UInt_t uChan ) const
{
   Int_t iBoardIndex = GetOffsetBoardInd( uType, uSlot);
   if( 0 <= iBoardIndex )
   {
      switch( uType )
      {
         case toftdc::caenV1290:
            if( 0 < fdMeanTimeOffsetCaen.GetSize() && 
                uChan < caentdc::kuNbChan )
               return fdTimeOffsetCaen[ iBoardIndex * caentdc::kuNbChan + uChan];
               else return 0;
            break;
         case toftdc::vftx:
            if( 0 < fdMeanTimeOffsetVftx.GetSize() && 
                uChan < vftxtdc::kuNbChan/2 )
               return fdTimeOffsetVftx[iBoardIndex * vftxtdc::kuNbChan/2 + uChan];
               else return 0;
            break;
         case toftdc::trb:
            if( 0 < fdMeanTimeOffsetTrb3.GetSize() && 
                uChan < trbtdc::kuNbChan/2 )
               return fdTimeOffsetTrb3[iBoardIndex * trbtdc::kuNbChan/2 + uChan];
               else return 0;
            break;
         case toftdc::get4:
            if( 0 < fdMeanTimeOffsetGet4.GetSize() && 
                uChan < get4tdc::kuNbChan/2 )
               return fdTimeOffsetGet4[iBoardIndex * get4tdc::kuNbChan/2 + uChan];
               else return 0;
            break;
         case toftdc::undef:
         default:
            return 0;
            break;
      } // switch( uType )
   } // if( 0 <= iBoardIndex )
      else switch( uType )
      {
         case toftdc::caenV1290:
            if( static_cast<Int_t>(caentdc::kuNbChan) == fdMeanTimeOffsetCaen.GetSize() && 
                uChan < caentdc::kuNbChan )
               return fdMeanTimeOffsetCaen[uChan];
               else return 0;
            break;
         case toftdc::vftx:
            if( static_cast<Int_t>(vftxtdc::kuNbChan/2) == fdMeanTimeOffsetVftx.GetSize() && 
                uChan < vftxtdc::kuNbChan/2 )
               return fdMeanTimeOffsetVftx[uChan];
               else return 0;
            break;
         case toftdc::trb:
            if( static_cast<Int_t>(trbtdc::kuNbChan/2) == fdMeanTimeOffsetTrb3.GetSize() && 
                uChan < trbtdc::kuNbChan/2 )
               return fdMeanTimeOffsetTrb3[uChan];
               else return 0;
            break;
         case toftdc::get4:
            if( static_cast<Int_t>(get4tdc::kuNbChan) == fdMeanTimeOffsetGet4.GetSize() &&
                uChan < get4tdc::kuNbChan/2 )
               return fdMeanTimeOffsetGet4[uChan];
               else return 0;
            break;
         case toftdc::undef:
         default:
            return 0;
            break;
      } // else switch( uType ) of if( 0 <= iBoardIndex )
}
Int_t TMbsCalibTofPar::GetTotOffsetVal( UInt_t uType, UInt_t uSlot, UInt_t uChan ) const
{
   Int_t iBoardIndex = GetOffsetBoardInd( uType, uSlot);
   if( 0 <= iBoardIndex )
   {
      switch( uType )
      {
         case toftdc::caenV1290:
            if( 0 < fdTotOffsetCaen.GetSize() && 
                uChan < caentdc::kuNbChan )
               return fdTotOffsetCaen[ iBoardIndex * caentdc::kuNbChan + uChan];
               else return 0;
            break;
         case toftdc::vftx:
            if( 0 < fdTotOffsetVftx.GetSize() && 
                uChan < vftxtdc::kuNbChan/2 )
               return fdTotOffsetVftx[iBoardIndex * vftxtdc::kuNbChan/2 + uChan];
               else return 0;
            break;
         case toftdc::trb:
            if( 0 < fdTotOffsetTrb3.GetSize() && 
                uChan < trbtdc::kuNbChan/2 )
               return fdTotOffsetTrb3[iBoardIndex * trbtdc::kuNbChan/2 + uChan];
               else return 0;
            break;
         case toftdc::get4:
            if( 0 < fdTotOffsetGet4.GetSize() && 
                uChan < get4tdc::kuNbChan/2 )
               return fdTotOffsetGet4[iBoardIndex * get4tdc::kuNbChan/2 + uChan];
               else return 0;
            break;
         case toftdc::undef:
         default:
            return 0;
            break;
      } // switch( uType )
   } // if( 0 <= iBoardIndex )
      else switch( uType )
      {
         case toftdc::caenV1290:
            if( static_cast<Int_t>(caentdc::kuNbChan) == fdMeanTotOffsetCaen.GetSize() && 
                uChan < caentdc::kuNbChan )
               return fdMeanTotOffsetCaen[uChan];
               else return 0;
            break;
         case toftdc::vftx:
            if( static_cast<Int_t>(vftxtdc::kuNbChan/2) == fdMeanTotOffsetVftx.GetSize() && 
                uChan < vftxtdc::kuNbChan/2 )
               return fdMeanTotOffsetVftx[uChan];
               else return 0;
            break;
         case toftdc::trb:
            if( static_cast<Int_t>(trbtdc::kuNbChan/2) == fdMeanTotOffsetTrb3.GetSize() && 
                uChan < trbtdc::kuNbChan/2 )
               return fdMeanTotOffsetTrb3[uChan];
               else return 0;
            break;
         case toftdc::get4:
            if( static_cast<Int_t>(get4tdc::kuNbChan) == fdMeanTotOffsetGet4.GetSize() &&
                uChan < get4tdc::kuNbChan/2 )
               return fdMeanTotOffsetGet4[uChan];
               else return 0;
            break;
         case toftdc::undef:
         default:
            return 0;
            break;
      } // else switch( uType ) of if( 0 <= iBoardIndex )
}
Int_t TMbsCalibTofPar::GetTdcTdcOffset( UInt_t uType, UInt_t uBoard ) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         if( static_cast<Int_t>(uBoard) < fiNbCaenCalib && 0 < fiTdcToTdcOffsetCaen.GetSize() )
            return fiTdcToTdcOffsetCaen[uBoard];
            else return 0;
         break;
      case toftdc::vftx:
         if( static_cast<Int_t>(uBoard) < fiNbVftxCalib && 0 < fiTdcToTdcOffsetVftx.GetSize() )
            return fiTdcToTdcOffsetVftx[uBoard];
            else return 0;
         break;
      case toftdc::trb:
         if( static_cast<Int_t>(uBoard) < fiNbTrb3Calib && 0 < fiTdcToTdcOffsetTrb3.GetSize() )
            return fiTdcToTdcOffsetTrb3[uBoard];
            else return 0;
         break;
      case toftdc::get4:
         if( static_cast<Int_t>(uBoard) < fiNbGet4Calib && 0 < fiTdcToTdcOffsetGet4.GetSize() )
            return fiTdcToTdcOffsetGet4[uBoard];
            else return 0;
         break;
      case toftdc::undef:
      default:
         return 0;
         break;
   } // switch( uType )
}
Int_t TMbsCalibTofPar::GetMainTdcTdcOffset( UInt_t uType ) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         return fiMainReferenceTdcCaen;
         break;
      case toftdc::vftx:
         return fiMainReferenceTdcVftx;
         break;
      case toftdc::trb:
         return fiMainReferenceTdcTrb3;
         break;
      case toftdc::get4:
         return fiMainReferenceTdcGet4;
         break;
      case toftdc::undef:
      default:
         return 0;
         break;
   } // switch( uType )
}
Int_t TMbsCalibTofPar::GetTotMode( UInt_t uType ) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         return fiTotModeCaen;
         break;
      case toftdc::vftx:
         return fiTotModeVftx;
         break;
      case toftdc::trb:
         return fiTotModeTrb3;
         break;
      case toftdc::get4:
         return fiTotModeGet4;
         break;
      case toftdc::undef:
      default:
         return 0;
         break;
   } // switch( uType )
}

Bool_t TMbsCalibTofPar::CheckTotInvFlag( UInt_t uType, UInt_t uSlot ) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         if( static_cast<Int_t>(uSlot) < fiNbCaenTotInvFlag && 
             0 < fiTotInversionFlagCaen.GetSize() )
         return kTRUE;
            else return kFALSE;
         break;
      case toftdc::vftx:
         if( static_cast<Int_t>(uSlot) < fiNbVftxTotInvFlag && 
             0 < fiTotInversionFlagVftx.GetSize() )
         return kTRUE;
            else return kFALSE;
         break;
      case toftdc::trb:
         if( static_cast<Int_t>(uSlot) < fiNbTrb3TotInvFlag && 
             0 < fiTotInversionFlagTrb3.GetSize() )
         return kTRUE;
            else
            {
             LOG(error)<<Form("Only %d inversion flags available.",fiNbTrb3TotInvFlag);
             return kFALSE;
            }
         break;
      case toftdc::get4:
         if( static_cast<Int_t>(uSlot) < fiNbGet4TotInvFlag && 
             0 < fiTotInversionFlagGet4.GetSize() )
            return kTRUE;
            else return kFALSE;
         break;
      case toftdc::undef:
      default:
         return kFALSE;
         break;
   } // switch( uType )
}

Int_t TMbsCalibTofPar::GetTotInvFlag( UInt_t uType, UInt_t uSlot, UInt_t uChan ) const
{
   switch( uType )
   {
      case toftdc::caenV1290:
         if( static_cast<Int_t>(uSlot) < fiNbCaenTotInvFlag && 
             0 < fiTotInversionFlagCaen.GetSize() && 
             uChan < caentdc::kuNbChan )
         return fiTotInversionFlagCaen[uSlot * caentdc::kuNbChan + uChan];
            else return 0;
         break;
      case toftdc::vftx:
         if( static_cast<Int_t>(uSlot) < fiNbVftxTotInvFlag && 
             0 < fiTotInversionFlagVftx.GetSize() && 
             uChan < vftxtdc::kuNbChan/2 )
         return fiTotInversionFlagVftx[uSlot * vftxtdc::kuNbChan/2 + uChan];
            else return 0;
         break;
      case toftdc::trb:
         if( static_cast<Int_t>(uSlot) < fiNbTrb3TotInvFlag && 
             0 < fiTotInversionFlagTrb3.GetSize() && 
             uChan < trbtdc::kuNbChan/2 )
         return fiTotInversionFlagTrb3[uSlot * trbtdc::kuNbChan/2 + uChan];
            else return 0;
         break;
      case toftdc::get4:
         if( static_cast<Int_t>(uSlot) < fiNbGet4TotInvFlag && 
             0 < fiTotInversionFlagGet4.GetSize() && 
             uChan < get4tdc::kuNbChan/2 )
            return fiTotInversionFlagGet4[uSlot * get4tdc::kuNbChan/2 + uChan];
            else return 0;
         break;
      case toftdc::undef:
      default:
         return 0;
         break;
   } // switch( uType )
}

Bool_t TMbsCalibTofPar::GetTdcOffEnaFlag( UInt_t uType, UInt_t uBoard ) const
{
   // The parameter to enable the TDC offset on a single TDC basis is
   // used only if the offset is NOT enabled for all TDCs
   if( kFALSE == IsTdcOffsetEna() )
   {
      switch( uType )
      {
         case toftdc::caenV1290:
            if( static_cast<Int_t>(uBoard) < fiNbCaenCalib && 0 < fiTdcOffsetEnaFlagVftx.GetSize() )
               return (1 == fiTdcOffsetEnaFlagVftx[uBoard])? kTRUE: kFALSE;
               else return kFALSE;
            break;
         case toftdc::vftx:
            if( static_cast<Int_t>(uBoard) < fiNbVftxCalib && 0 < fiTdcOffsetEnaFlagCaen.GetSize() )
               return (1 == fiTdcOffsetEnaFlagCaen[uBoard])? kTRUE: kFALSE;
               else return kFALSE;
            break;
         case toftdc::trb:
            if( static_cast<Int_t>(uBoard) < fiNbTrb3Calib && 0 < fiTdcOffsetEnaFlagTrb3.GetSize() )
               return (1 == fiTdcOffsetEnaFlagTrb3[uBoard])? kTRUE: kFALSE;
               else return kFALSE;
            break;
         case toftdc::get4:
            if( static_cast<Int_t>(uBoard) < fiNbGet4Calib && 0 < fiTdcOffsetEnaFlagGet4.GetSize() )
               return (1 == fiTdcOffsetEnaFlagGet4[uBoard])? kTRUE: kFALSE;
               else return kFALSE;
            break;
         case toftdc::undef:
         default:
            return kFALSE;
            break;
      } // switch( uType )
   }
      else return IsTdcOffsetEna();
}
