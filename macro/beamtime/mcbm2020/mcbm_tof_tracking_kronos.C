// --------------------------------------------------------------------------
//
// Macro for reconstruction "TOF tracks" in mcbm data (2020)
// Runs on output of event-based combined hit reconstruction for all systems.
// Wrapper setting the paths for analysis using the HPC farm's common files.
//
// --------------------------------------------------------------------------

#include "mcbm_tof_tracking.C"

Bool_t mcbm_tof_tracking_kronos(UInt_t uRunIdx = 28,
                                Bool_t bEventWin  = kFALSE,
                                Int_t iTrackMode  = 2,
                                Int_t iCalOpt     = 0,
                                Int_t nTimeslices = 0,
                                TString sInpDir = "/lustre/cbm/users/ploizeau/mcbm2020/"
                                                  "unp_evt_data_7f229b3f_20201103",
                                TString sOutDir = "./data") {
  UInt_t uRunId = 0;

  if (99999 != uRunIdx) {
    std::vector<UInt_t> vuListRunId = {
      692, 698, 702, 704, 705, 706, 707,            //  7 =>  0 -  6
      744, 750, 759, 760, 761, 762, 799,            //  7 =>  7 - 13
      811, 812, 816, 817, 819,                      //  5 => 14 - 18
      820, 821, 822, 824, 826, 827, 828, 829,       //  8 => 19 - 26
      830, 831, 836,                                //  3 => 27 - 29
      841, 846, 849,                                //  3 => 30 - 32
      850, 851, 852, 854, 855, 856, 857, 858, 859,  //  9 => 33 - 41
      860, 861, 862, 863, 864, 865, 866             //  7 => 42 - 48

      /*
      /// With runs < 1 min due to missmatch!
      811, 812, 816, 817, 818, 819,                  //  6 => 14 - 19
      820, 821, 822, 824, 826, 827, 828, 829,        //  8 => 20 - 27
      830, 831, 836, 839,                            //  4 => 28 - 31
      840, 841, 842, 844, 845, 846, 848, 849,        //  8 => 32 - 39
      850, 851, 852, 854, 855, 856, 857, 858, 859,   //  9 => 40 - 48
      860, 861, 862, 863, 864, 865, 866              //  7 => 49 - 55
      */
    };
    if (vuListRunId.size() <= uRunIdx) return kFALSE;

    uRunId = vuListRunId[uRunIdx];
  }  // if( 99999 != uRunIdx )

  if (uRunId < 692 && 0 != uRunId) return kFALSE;

  return mcbm_tof_tracking(uRunId,
                           bEventWin,
                           iTrackMode,
                           iCalOpt,
                           nTimeslices,
                           sInpDir,
                           sOutDir );
}
