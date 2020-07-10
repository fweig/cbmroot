#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmMvd + ;

#pragma link C++ class CbmDigitize < CbmMvdDigi> + ;
#pragma link C++ class CbmMvdDigitizer + ;
//#pragma link C++ class CbmMvdDigitizerTB+;
#pragma link C++ class CbmMvdHitfinder + ;
#pragma link C++ class CbmMvdClusterfinder + ;
#pragma link C++ class CbmMvdDigiToHit + ;
#pragma link C++ class CbmMvdClusterfinderTB + ;
#pragma link C++ class CbmMvdDigiToHitTB + ;
#pragma link C++ class CbmMvdReadout + ;

#pragma link C++ class CbmMvdPileupManager + ;
#pragma link C++ class CbmMvdPixelCharge + ;

#pragma link C++ class CbmMvdStationPar + ;
#pragma link C++ class CbmMvdSensorDataSheet + ;
#pragma link C++ class CbmMvdMimosa26AHR + ;
#pragma link C++ class CbmMvdMimosa34 + ;
#pragma link C++ class CbmMvdMimosis + ;

#pragma link C++ class CbmMvdSensor + ;
#pragma link C++ class CbmMvdDetector + ;

#pragma link C++ class CbmMvdSensorPlugin + ;

//#pragma link C++ class CbmMvdSensorBuffer+;
//#pragma link C++ class CbmMvdSensorTrackingBuffer+;
//#pragma link C++ class CbmMvdSensorFrameBuffer+;

#pragma link C++ class CbmMvdSensorTask + ;
//#pragma link C++ class CbmMvdSensorFindHitTask+;
#pragma link C++ class CbmMvdSensorHitfinderTask + ;  //OLD
#pragma link C++ class CbmMvdSensorDigitizerTask + ;
#pragma link C++ class CbmMvdSensorReadoutTask + ;
#pragma link C++ class CbmMvdSensorDigitizerTBTask + ;
#pragma link C++ class CbmMvdSensorClusterfinderTask + ;  //OLD
//KHUN
#pragma link C++ class CbmMvdSensorDigiToHitTask + ;


#pragma link C++ class CbmMvdGeoHandler + ;

#pragma link C++ class CbmMvdQa + ;
//#pragma link C++ class CbmMvdClusterAna+;

#pragma link C++ class CbmMvdReadoutSimple + ;
#pragma link C++ class CbmMvdReadoutCluster + ;


//#pragma link C++ class MyG4UniversalFluctuationForSi+;
//#pragma link C++ class CbmMvdFindHits+;
//#pragma link C++ class CbmMvdDigitizeL+;
//#pragma link C++ class CbmMvdHitProducer+;
//#pragma link C++ class CbmMvdStation+;

#endif
