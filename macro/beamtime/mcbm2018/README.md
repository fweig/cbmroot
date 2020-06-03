TO BE COMPLETED


Standard parameter files
- mMuchPar_16_23_28March19.par
- mMuchPar_30March19.par
- mMuchPar.par
- mRichPar.par
- mStsMuchPar.par
- mStsPar.par
- mT0Par.par
- mTofPar.par

Obsolete/debug/lab parameter files (for specialists)
- mTofParAll.par
- PulserPar.par
- MapTofGbtx.par

Macros for conversion of raw data from TSA to ROOT format
- convert_tsa_gdpb.C
- convert_tsa_mcbm.C
- convert_tsa_sdpb.C

Macros for Detectors and DAQ monitoring
- MonitorDataRates.C
- MonitorMuch.C
- MonitorSts.C
- MonitorT0.C
- MonitorTof.C
- DataRateMoniPlot.C

Macros for monitoring in debugging/lab tests (for specialists)
- McbmPulserMonitor.C
- McbmRateMonitor.C
- McbmSyncMonitor.C
- MonitorTofPulser.C
- SpillRatePlot.C
- T0MoniPlot.C
- T0CoincPlotSingleRun.C

Macros for data quality/format check
- AnalyzeDigiMcbmFull.C
- eLinkMappingCrobSts.C
- CheckDataFormatGdpb.C
- tsaCrcChecker.C

Macros for lab test monitors
- PulserMonitor.C
- TofFeeThr.C
- TofPulserPlot.C
- TofTestFee.C

Macros for mTOF analysis and plotting
- rootlogon.C
- ana_trks.C
- ana_digi_cal.C
- ana_digi_cos.C
- pl_all_2D.C
- pl_all_CluMul.C
- pl_all_CluPosEvol.C
- pl_all_CluRate.C
- pl_all_CluTimeEvol.C
- pl_all_DigiCor.C
- pl_all_Sel2D.C
- pl_raw_evt.C
=> Execution order: contact Norbert Herrmann

Scripts for TOF Calibration
- calib_batch.sh
- gen_digi.sh
- init_calib.sh
- iter_calib.sh
- iter_tracks.sh
=> Execution order: contact Norbert Herrmann

Official unpacking macro
- unpack_tsa_mcbm.C

Macros combining unpacking with another tasks
- unpack_tsa_build_events.C
- unpack_tsa_check_time.C

Unpacking macros for sub-sets of mCBM (maintained?)
- unpack_tsa.C
- unpack_tsa_much.C
- unpack_tsa_sts.C
- unpack_tsa_sts_much.C
- unpack_tsa_tof.C

Macros for next steps on unpacked data file
- build_events.C
- check_events.C
- check_timing.C

Typical chain for raw data:
1) unpack_tsa_mcbm.C
2) build_events.C
3) User analysis
