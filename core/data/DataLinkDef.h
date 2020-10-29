#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class CbmAddress;
#pragma link C++ class CbmTimeSlice;
#pragma link C++ class CbmDigiBranchBase + ;
#pragma link C++ class CbmEvent + ;
#pragma link C++ class CbmEventStore + ;
#pragma link C++ class CbmHit;
#pragma link C++ class CbmMCTrack + ;
#pragma link C++ class CbmPixelHit + ;
#pragma link C++ class CbmStripHit + ;
#pragma link C++ class CbmTrack + ;
#pragma link C++ class CbmTrackParam;
#pragma link C++ class CbmTrackMatch + ;
#pragma link C++ class CbmCluster + ;
#pragma link C++ class CbmMatch + ;
#pragma link C++ class CbmTrackMatchNew + ;
#pragma link C++ class CbmLink + ;
#pragma link C++ class CbmModuleList;
#pragma link C++ class CbmErrorMessage + ;

// ---In base
#pragma link C++ class CbmDigiBranchBase + ;
#pragma link C++ class CbmDigiContainer + ;

#pragma link C++ class CbmMCTrack + ;
#pragma link C++ class CbmMCEventList;
#pragma link C++ class CbmMCEventInfo;

#pragma link C++ class CbmMvdCluster + ;
#pragma link C++ class CbmMvdDetectorId + ;
#pragma link C++ class CbmMvdDigi + ;
#pragma link C++ class CbmMvdHit + ;
#pragma link C++ class CbmMvdPoint + ;
#pragma link C++ class CbmMvdHitMatch + ;

#pragma link C++ namespace CbmStsAddress;
#pragma link C++ class CbmStsCluster + ;
#pragma link C++ class CbmStsDigi + ;
#pragma link C++ class CbmStsHit + ;
#pragma link C++ class CbmStsPoint + ;
#pragma link C++ class CbmStsTrack + ;
#pragma link C++ enum EStsElementLevel;

#pragma link C++ class CbmRichPoint + ;
#pragma link C++ class CbmRichHit + ;
#pragma link C++ class CbmRichRing + ;
#pragma link C++ class CbmRichTrbDigi + ;
#pragma link C++ class CbmRichDigi + ;

#pragma link C++ class CbmMuchCluster + ;
#pragma link C++ class CbmMuchPixelHit + ;
#pragma link C++ class CbmMuchPoint + ;
#pragma link C++ class CbmMuchDigi + ;
#pragma link C++ class CbmMuchBeamTimeDigi + ;
#pragma link C++ class CbmMuchDigiMatch + ;
#pragma link C++ class CbmMuchTrack + ;
#pragma link C++ class CbmMuchAddress + ;

#pragma link C++ class CbmTrdHit + ;
#pragma link C++ class CbmTrdPoint + ;
#pragma link C++ class CbmTrdTrack + ;
#pragma link C++ class CbmTrdDigi + ;
#pragma link C++ class CbmTrdAddress + ;
#pragma link C++ class CbmTrdCluster + ;
#pragma link C++ class CbmTrdRawMessageSpadic + ;

#pragma link C++ class CbmTofHit + ;
#pragma link C++ class CbmTofPoint + ;
#pragma link C++ class CbmTofDigi + ;
#pragma link C++ class CbmTofAddress + ;
#pragma link C++ class CbmTofTracklet + ;
#pragma link C++ class CbmTofTrackletParam + ;

#pragma link C++ class CbmPsdDigi-;
#pragma link C++ class CbmPsdHit;
#pragma link C++ class CbmPsdPoint + ;
#pragma link C++ class CbmPsdAddress;
#pragma link C++ class CbmPsdMCbmHit;

#pragma link C++ class CbmGlobalTrack + ;
#pragma link C++ class CbmTofTrack + ;
#pragma link C++ class CbmVertex + ;

#pragma link C++ class stsxyter::Message;
#pragma link C++ class gdpbv100::Message;
#pragma link C++ class gdpbv100::FullMessage;
#pragma link C++ class TimesliceMetaData;
#pragma link C++ class PsdData::PsdGbtReader;

#pragma link C++ class std::vector < stsxyter::Message>;
#pragma link C++ class std::vector < gdpbv100::Message>;
#pragma link C++ class std::vector < CbmTrdRawMessageSpadic>;

#pragma link C++ enum ECbmTreeAccess;
#pragma link C++ enum ECbmModuleId;
#pragma link C++ enum ECbmDataType;

#pragma link C++ class vector < CbmMvdDigi> + ;
#pragma link C++ class vector < CbmStsDigi> + ;
#pragma link C++ class vector < CbmRichDigi> + ;
#pragma link C++ class vector < CbmMuchDigi> + ;
#pragma link C++ class vector < CbmMuchBeamTimeDigi> + ;
#pragma link C++ class vector < CbmTrdDigi> + ;
#pragma link C++ class vector < CbmTofDigi> + ;
#pragma link C++ class vector < CbmPsdDigi> + ;
#pragma link C++ class vector < CbmMatch> + ;
#pragma link C++ class CbmDigiVector < CbmMvdDigi> + ;
#pragma link C++ class CbmDigiVector < CbmStsDigi> + ;
#pragma link C++ class CbmDigiVector < CbmRichDigi> + ;
#pragma link C++ class CbmDigiVector < CbmMuchDigi> + ;
#pragma link C++ class CbmDigiVector < CbmMuchBeamTimeDigi> + ;
#pragma link C++ class CbmDigiVector < CbmTrdDigi> + ;
#pragma link C++ class CbmDigiVector < CbmTofDigi> + ;
#pragma link C++ class CbmDigiVector < CbmPsdDigi> + ;
#pragma link C++ class vector < CbmEventStore> + ;

#pragma read sourceClass="CbmTofDigi" version="[1-2]" targetClass="CbmTofDigi" \
   source="UInt_t fuAddress" target="fuAddress" \
   include="Logger.h" \
   code="{ UInt_t system = (onfile.fuAddress >> 0) & ((1 << 4) - 1); \
           UInt_t smId = (onfile.fuAddress >> 4) & ((1 << 8) - 1); \
           UInt_t smType = (onfile.fuAddress >> 12) & ((1 << 4) - 1); \
           UInt_t rpcId = (onfile.fuAddress >> 16) & ((1 << 7) - 1); \
           UInt_t chSide = (onfile.fuAddress >> 23) & ((1 << 1) - 1); \
           UInt_t chId = (onfile.fuAddress >> 24) & ((1 << 8) - 1); \
           if ( smId > 127 || rpcId > 63 || chId > 63 ) { \
             LOG(error) << \"You are trying to read an outdated version of CbmTofDigi\"; \
             LOG(error) << \"where the unique tof address can't be converted\"; \
             LOG(error) << \"automatically to the new tof addressing scheme.\"; \
             LOG(fatal) << \"Stop execution.\"; \
           } \
           UInt_t rpcType = 0; \
           fuAddress = (system & ((1 << 4) - 1)) \
                     + ((smId & ((1 << 7) - 1)) << 4) \
                     + ((smType & ((1 << 4) - 1)) << 11) \
                     + ((chSide & ((1 << 1) - 1)) << 21) \
                     + ((rpcId & ((1 << 6) - 1)) << 15) \
                     + ((chId & ((1 << 6) - 1)) << 22) \
                     + ((rpcType & ((1 << 4) - 1)) << 28); \
         }"

#pragma read sourceClass="CbmTofHit" version="[1-4]" targetClass="CbmTofHit" \
   source="" target="" \
   include="Logger.h" \
   code="{ \
           LOG(error); \
           LOG(error) << \"You are trying to read an outdated version of CbmTofHit\"; \
           LOG(error) << \"where the unique tof address can't be converted\"; \
           LOG(error) << \"automatically to the new tof addressing scheme.\"; \
           LOG(error); \
           LOG(fatal) << \"Stop execution.\"; \
         }"

#pragma read sourceClass="CbmTofPoint" version="[1-3]" targetClass="CbmTofPoint" \
   source="" target="" \
   include="Logger.h" \
   code="{ \
           LOG(error); \
           LOG(error) << \"You are trying to read an outdated version of CbmTofPoint\"; \
           LOG(error) << \"where the unique tof address can't be converted\"; \
           LOG(error) << \"automatically to the new tof addressing scheme.\"; \
           LOG(error); \
           LOG(fatal) << \"Stop execution.\"; \
         }"
#endif
