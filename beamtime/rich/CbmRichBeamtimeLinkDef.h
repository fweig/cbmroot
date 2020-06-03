// $Id: RichLinkDef.h,v 1.16 2006/09/13 14:56:13 hoehne Exp $

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

//prototype analysis (beamtime and lab)
#pragma link C++ class CbmRichTrbUnpack+;
#pragma link C++ class CbmRichTrbUnpack2+;
#pragma link C++ class CbmTrbEdgeMatcher+;
#pragma link C++ class CbmTrbCalibrator+;
#pragma link C++ class CbmRichTrbEventBuilder+;
#pragma link C++ class CbmRichProtoAnalysis+;
#pragma link C++ class CbmRichEventDebug+;
#pragma link C++ class CbmRichRingHitsAnalyser+;

#pragma link C++ class CbmRichTrbRecoQa+;
#pragma link C++ class CbmRichTrbPulserQa+;
#pragma link C++ class CbmRichHitInfo+;
#pragma link C++ class CbmRichTrbRecoQaStudyReport+;

#endif

