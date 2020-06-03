#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ struct hadaq::HadTu+;
#pragma link C++ struct hadaq::HadTuId+;
#pragma link C++ struct hadaq::RawEvent+;
#pragma link C++ struct hadaq::RawSubevent+;
#pragma link C++ class hadaq::TrbIterator+;
#pragma link C++ class hadaq::LocalStampConverter+;
#pragma link C++ class hadaq::TdcIterator+;
#pragma link C++ struct hadaq::TdcMessage+;
//#ifdef BUILD_TOF_BEAMTIME
 #pragma link C++ class CbmHldSource+;
//#endif // NOT BUILD_BEAMTIME

#pragma link C++ class TMbsUnpackTofPar+;
#pragma link C++ class TMbsUnpackTofContFact+;
#pragma link C++ class TMbsUnpackTof+;
#pragma link C++ class TTriglogUnpackTof+;
#pragma link C++ class TGet4UnpackTof+;
#pragma link C++ class TTrbUnpackTof+;

#pragma link C++ class TTofTriglogBoard+;
#pragma link C++ class TTofScalerBoard+;
#pragma link C++ class TTofTriglogUnpacker+;
#pragma link C++ class TTofTriglogScalUnpacker+;
#pragma link C++ class TTofScomUnpacker+;
#pragma link C++ class TTofScal2014Unpacker+;
#pragma link C++ class TTofTrbTdcUnpacker+;
#pragma link C++ class TTofOrGenUnpacker+;

#pragma link C++ class TTofTdcData+;
#pragma link C++ class std::vector<TTofTdcData>+;
#pragma link C++ class TTofTdcBoard+;
#pragma link C++ class std::vector<TTofVftxData>+;
#pragma link C++ class TTofVftxData+;
#pragma link C++ class TTofVftxBoard+;
#pragma link C++ class TTofVftxUnpacker+;
#pragma link C++ class std::vector<TTofTrbTdcData>+;
#pragma link C++ class TTofTrbTdcData+;
#pragma link C++ class TTofTrbTdcBoard+;
#pragma link C++ class TTofTrbTdcUnpacker+;
#pragma link C++ class TTofTrbTdcCalib+;

#pragma link C++ class TGet4v1MessageExtended+;
#pragma link C++ class Get4v1Hit+;
#pragma link C++ class Get4v1Tdc+;
#pragma link C++ class Get4v1Event+;
#pragma link C++ class TTofGet4Data+;
#pragma link C++ class TTofGet4Board+;
#pragma link C++ class TimeWindowCond+;
#pragma link C++ struct TGet4v1Rec+;
#pragma link C++ class TGet4v1Param+;
#pragma link C++ class TTofGet4Unpacker+;

#pragma link C++ class TMbsUnpTofMonitor+;

#pragma link C++ class TMbsCalibTofPar+;
#pragma link C++ class TMbsCalibTof+;

#pragma link C++ class TMbsCalibTdcTof+;
#pragma link C++ class TTofCalibData+;

#pragma link C++ class TMbsCalibScalTof+;
#pragma link C++ class TTofCalibScaler+;

#pragma link C++ class TMbsMappingTofPar+;
#pragma link C++ class TMbsMappingTof+;

#pragma link C++ class Plastics_Hit+;
#pragma link C++ class Plastics_Event+;
#pragma link C++ class Rpc_Hit+;
#pragma link C++ class Rpc_Cluster+;
#pragma link C++ class Rpc_Event+;
#pragma link C++ class Scalers_Event+;
#pragma link C++ class TVftxBoardData+;
#pragma link C++ class TMbsConvTof+;
#pragma link C++ class TMbsConvTofPar+;

#pragma link C++ class CbmTofOnlineDisplay;

#endif

