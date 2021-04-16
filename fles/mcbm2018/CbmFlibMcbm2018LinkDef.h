/* Copyright (C) 2018-2021 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmMcbmUnpack;
#pragma link C++ class CbmMcbm2018Source;
#pragma link C++ class CbmMcbm2018MsCrcCheck;

#pragma link C++ class CbmMcbm2018RawConverterSdpb;
#pragma link C++ class CbmMcbm2018RawConverterGdpb;

#pragma link C++ class CbmMcbm2018UnpackerAlgoSts;
#pragma link C++ class CbmMcbm2018UnpackerTaskSts + ;
#pragma link C++ class CbmMcbm2018UnpackerAlgoMuch;
#pragma link C++ class CbmMcbm2018UnpackerTaskMuch + ;
#pragma link C++ class CbmMcbm2018UnpackerAlgoTof;
#pragma link C++ class CbmMcbm2018UnpackerTaskTof + ;
#pragma link C++ class CbmMcbm2018UnpackerAlgoRich;
#pragma link C++ class CbmMcbm2018UnpackerTaskRich + ;
#pragma link C++ class CbmMcbm2018UnpackerAlgoPsd;
#pragma link C++ class CbmMcbm2018UnpackerTaskPsd + ;
#pragma link C++ class CbmMcbm2018UnpackerAlgoTrdR + ;
#pragma link C++ class CbmMcbm2018UnpackerTaskTrdR + ;

#pragma link C++ class CbmCheckDataFormatGdpb2018;
#pragma link C++ class CbmMcbm2018MonitorDataRates;
#pragma link C++ class CbmMcbm2018MonitorAlgoT0;
#pragma link C++ class CbmMcbm2018MonitorTaskT0;
#pragma link C++ class CbmMcbm2018MonitorMcbmRate;
#pragma link C++ class CbmMcbm2018MonitorMcbmSync;
#pragma link C++ class CbmMcbm2018MonitorMcbmPulser;
//#pragma link C++ class CbmMcbm2018MonitorStsSync;
//#pragma link C++ class CbmMcbm2018MonitorStsPulser;
#pragma link C++ class CbmMcbm2018MonitorSts;
#pragma link C++ class CbmMcbm2018MonitorMuch;
#pragma link C++ class CbmMcbm2018MonitorMuchLite;
#pragma link C++ class CbmMcbm2018MonitorAlgoMuchLite;
#pragma link C++ class CbmMcbm2018MonitorTaskMuchLite;
//#pragma link C++ class CbmMcbm2018MonitorTrd;
#pragma link C++ class CbmMcbm2018MonitorTof;
#pragma link C++ class CbmMcbm2018MonitorTofPulser;
#pragma link C++ class CbmMcbm2018MonitorAlgoTof;
#pragma link C++ class CbmMcbm2018MonitorTaskTof;
#pragma link C++ class CbmMcbm2018MonitorAlgoTofPulser;
#pragma link C++ class CbmMcbm2018MonitorTaskTofPulser;
#pragma link C++ class CbmMcbm2018TofTestFee;
#pragma link C++ class CbmMcbm2018TofFeeThr;
#pragma link C++ class CbmMcbm2018MonitorAlgoPsd;
#pragma link C++ class CbmMcbm2018MonitorTaskPsd;

#pragma link C++ class CbmCheckDigisNbCorr;
#pragma link C++ class CbmCheckTiming;
#pragma link C++ class CbmCheckEvents;
#pragma link C++ class CbmMcbm2018EventBuilder;
#pragma link C++ class CbmMcbm2019CheckPulser;
#pragma link C++ class CbmMcbm2019CheckDigisMuch;
#pragma link C++ class CbmMcbm2019CheckDigisSts;
#pragma link C++ class CbmMcbm2019CheckDtInDet;
#pragma link C++ class CbmMcbm2019CheckTimingPairs + ;
#pragma link C++ class CbmMcbm2019TimeWinEventBuilderAlgo + ;
#pragma link C++ class CbmMcbm2019TimeWinEventBuilderTask + ;

#pragma link C++ class CbmMcbmCheckTimingAlgo + ;
#pragma link C++ class CbmMcbmCheckTimingTask + ;

#pragma link C++ class CbmMcbmSpillFindAlgo + ;
#pragma link C++ class CbmMcbmSpillFindTask + ;

#endif
