#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmAddress;
#pragma link C++ class CbmTimeSlice;
#pragma link C++ class CbmDigiBranchBase+;
#pragma link C++ class CbmEvent+;
#pragma link C++ class CbmEventStore+;
#pragma link C++ class CbmHit;
#pragma link C++ class CbmMCTrack+;
#pragma link C++ class CbmPixelHit+;
#pragma link C++ class CbmStripHit+;
#pragma link C++ class CbmTrack+;
#pragma link C++ class CbmTrackParam;
#pragma link C++ class CbmTrackMatch+;
#pragma link C++ class CbmCluster+;
#pragma link C++ class CbmMatch+;
#pragma link C++ class CbmTrackMatchNew+;
#pragma link C++ class CbmLink+;
#pragma link C++ class CbmModuleList;
#pragma link C++ class CbmErrorMessage+;

// ---In base
#pragma link C++ class CbmDigiBranchBase+;
#pragma link C++ class CbmDigiContainer+;

#pragma link C++ class CbmMCTrack+;
#pragma link C++ class CbmMCEventList;
#pragma link C++ class CbmMCEventInfo;

#pragma link C++ class CbmMvdCluster+;
#pragma link C++ class CbmMvdDetectorId+;
#pragma link C++ class CbmMvdDigi+;
#pragma link C++ class CbmMvdHit+;
#pragma link C++ class CbmMvdPoint+;
#pragma link C++ class CbmMvdHitMatch+;

#pragma link C++ namespace CbmStsAddress;
#pragma link C++ class CbmStsCluster+;
#pragma link C++ class CbmStsDigi+;
#pragma link C++ class CbmStsHit+;
#pragma link C++ class CbmStsPoint+;
#pragma link C++ class CbmStsTrack+;
#pragma link C++ enum EStsElementLevel;

#pragma link C++ class CbmRichPoint+;
#pragma link C++ class CbmRichHit+;
#pragma link C++ class CbmRichRing+;
#pragma link C++ class CbmRichTrbDigi+;
#pragma link C++ class CbmRichDigi+;

#pragma link C++ class CbmMuchCluster+;
#pragma link C++ class CbmMuchPixelHit+;
#pragma link C++ class CbmMuchPoint+;
#pragma link C++ class CbmMuchDigi+;
#pragma link C++ class CbmMuchBeamTimeDigi+;
#pragma link C++ class CbmMuchDigiMatch+;
#pragma link C++ class CbmMuchTrack+;
#pragma link C++ class CbmMuchAddress+;

#pragma link C++ class CbmTrdHit+;
#pragma link C++ class CbmTrdPoint+;
#pragma link C++ class CbmTrdTrack+;
#pragma link C++ class CbmTrdDigi+;
#pragma link C++ class CbmTrdAddress+;
#pragma link C++ class CbmTrdCluster+;
#pragma link C++ class CbmTrdRawMessageSpadic+;

#pragma link C++ class CbmTofHit+;
#pragma link C++ class CbmTofPoint+;
#pragma link C++ class CbmTofDigi+;
#pragma link C++ class CbmTofAddress+;
#pragma link C++ class CbmTofTracklet+;
#pragma link C++ class CbmTofTrackletParam+;

/*
#pragma link C++ class CbmEcalPoint+;
#pragma link C++ class CbmEcalHit+;
#pragma link C++ class CbmEcalHitFastMC+;
#pragma link C++ class CbmEcalPointLite+;
#pragma link C++ class CbmEcalCell+;
#pragma link C++ class CbmEcalMaximum+;
#pragma link C++ class CbmEcalCluster+;
#pragma link C++ class CbmEcalRecParticle+;
#pragma link C++ class CbmEcalIdParticle+;
*/

#pragma link C++ class CbmPsdDigi;
#pragma link C++ class CbmPsdHit;
#pragma link C++ class CbmPsdPoint+;
#pragma link C++ class CbmPsdAddress;
#pragma link C++ class CbmPsdMCbmHit;

#pragma link C++ class CbmGlobalTrack+;
#pragma link C++ class CbmTofTrack+;
#pragma link C++ class CbmVertex+;

/*
#pragma link C++ class CbmRawMessage;
#pragma link C++ class CbmSpadicRawMessage;
#pragma link C++ class CbmTrdRawMessage;
#pragma link C++ class CbmNxyterRawMessage;
#pragma link C++ class CbmNxyterRawSyncMessage;
#pragma link C++ class CbmStsxyterRawHitMessage;
#pragma link C++ class CbmTrbRawMessage;
*/
#pragma link C++ class stsxyter::Message;
#pragma link C++ class gdpbv100::Message;
#pragma link C++ class gdpbv100::FullMessage;
#pragma link C++ class TimesliceMetaData;
#pragma link C++ class PsdData::PsdGbtReader;

#pragma link C++ class std::vector< stsxyter::Message >;
#pragma link C++ class std::vector< gdpbv100::Message >;

#pragma link C++ enum ECbmTreeAccess;
#pragma link C++ enum ECbmModuleId;
#pragma link C++ enum ECbmDataType;

#pragma link C++ class vector<CbmMvdDigi>+;
#pragma link C++ class vector<CbmStsDigi>+;
#pragma link C++ class vector<CbmRichDigi>+;
#pragma link C++ class vector<CbmMuchDigi>+;
#pragma link C++ class vector<CbmMuchBeamTimeDigi>+;
#pragma link C++ class vector<CbmTrdDigi>+;
#pragma link C++ class vector<CbmTofDigi>+;
#pragma link C++ class vector<CbmPsdDigi>+;
#pragma link C++ class vector<CbmMatch>+;
#pragma link C++ class CbmDigiVector<CbmMvdDigi>+;
#pragma link C++ class CbmDigiVector<CbmStsDigi>+;
#pragma link C++ class CbmDigiVector<CbmRichDigi>+;
#pragma link C++ class CbmDigiVector<CbmMuchDigi>+;
#pragma link C++ class CbmDigiVector<CbmMuchBeamTimeDigi>+;
#pragma link C++ class CbmDigiVector<CbmTrdDigi>+;
#pragma link C++ class CbmDigiVector<CbmTofDigi>+;
#pragma link C++ class CbmDigiVector<CbmPsdDigi>+;
#pragma link C++ class vector<CbmEventStore>+;

#endif
