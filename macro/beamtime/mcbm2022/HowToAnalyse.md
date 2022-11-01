
# Running the same sequence as in the CI tests by hand with the current master version of cbmroot

1. Download and compile Cbmroot master
1. Download/copy the example file(s) from
   ```
   <lustre>/cbm/users/ploizeau//mcbm2022/2391_first20Ts.tsa
   <lustre>/cbm/users/ploizeau//mcbm2022/2488_first20Ts.tsa
   ```
1. Go to your cbmroot folder in `<cbmroot>/macro/run`, load you cbmroot config script
1. Create the geometry file for the target run (macro will decide if it is needed):
   ```
   root -l -b -q 'create_mcbm_geo_setup.C(2391)'
   ```
1. Unpack the test run:
   ```
   root -l -b -q 'run_unpack_tsa.C("<RAW_DATA_PATH>/2391_first20Ts.tsa", 2391)'
   ```
1. Run the reco macro with L1
   ```
   #                                                               run  evts  in DIR     out DIR  File   MVD    STS    TRD   TRD2D  RICH    MUCH    TOF   TOF TR  PSD    ALIGN  EVENTS   L1    L1 QA      INPUT FILE
   root -l -b -q '../macro/beamtime/mcbm2022/mcbm_event_reco_L1.C( 2391, 20, "./data/", "./data/", -1, kFALSE, kTRUE, kTRUE, kTRUE, kTRUE, kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kTRUE, kFALSE, kFALSE, "./data/2391_first20Ts.digi.root")'
   ```

# Example of how to use it (for a local installation in /data/cbmroot/cbmsource):

1. `sshfs -o reconnect,ServerAliveInterval=15,ServerAliveCountMax=3 lustre:/lustre/cbm/prod/beamtime/2022/05/mcbm/nickel/ /gsi/lustreMcbmNickel/`
1. `cd /data/cbmroot/mcbmsource/macro/mcbm/`
1. `root -l -b mcbm_transport.C`  \
   => Gets correct geometry loaded and stored as <setupname>.geo.root in ./data/
1. `cd /data/cbmroot/mcbmsource/macro/run/`
1. unpack (or use a already unpacked data set):
   ```
   root -l -b -q 'run_unpack_tsa.C({"/gsi/lustreMcbmNickel/2391_node8_0_*.tsa","/gsi/lustreMcbmNickel/2391_node8_1_*.tsa","/gsi/lustreMcbmNickel/2391_node8_2_*.tsa","/gsi/lustreMcbmNickel/2391_node8_3_*.tsa","/gsi/lustreMcbmNickel/2391_node8_4_*.tsa","/gsi/lustreMcbmNickel/2391_node8_5_*.tsa","/gsi/lustreMcbmNickel/2391_node9_0_*.tsa","/gsi/lustreMcbmNickel/2391_node9_1_*.tsa","/gsi/lustreMcbmNickel/2391_node9_2_*.tsa","/gsi/lustreMcbmNickel/2391_node9_3_*.tsa","/gsi/lustreMcbmNickel/2391_node9_4_*.tsa","/gsi/lustreMcbmNickel/2391_node9_5_*.tsa"},2391,"mcbm_beam_2021_07_surveyed",100)'
   ```
1. `cd /data/cbmroot/mcbmsource/macro/beamtime/mcbm2022/`
1. copy the correct calibration files from Tof to your location (`<location>/2365.5.lxbk0600/<The files from later down (scp)>`)  and fix the path in the macro `mcbm_event_reco.C`
   ```
   scp lustre:/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2022/2365.5.lxbk0600_set042032500_93_1tofClust.hst.root .
   scp lustre:/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2022/2365.5.lxbk0600_tofFindTracks.hst.root .
   ```
1. `root -l -b -q mcbm_event_reco.C`
