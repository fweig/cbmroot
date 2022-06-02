Example of how to use it (for a local installation in /data/cbmroot/cbmsource):
1) `sshfs -o reconnect,ServerAliveInterval=15,ServerAliveCountMax=3 lustre:/lustre/cbm/prod/beamtime/2022/05/mcbm/nickel/ /gsi/lustreMcbmNickel/`
2) `cd /data/cbmroot/mcbmsource/macro/mcbm/`
3) `root -l -b mcbm_transport.C`  \
   => Gets correct geometry loaded and stored as <setupname>.geo.root in ./data/
4) `cd /data/cbmroot/mcbmsource/macro/run/`
5) unpack (or use a already unpacked data set):
```
root -l -b -q 'run_unpack_tsa.C({"/gsi/lustreMcbmNickel/2391_node8_0_*.tsa","/gsi/lustreMcbmNickel/2391_node8_1_*.tsa","/gsi/lustreMcbmNickel/2391_node8_2_*.tsa","/gsi/lustreMcbmNickel/2391_node8_3_*.tsa","/gsi/lustreMcbmNickel/2391_node8_4_*.tsa","/gsi/lustreMcbmNickel/2391_node8_5_*.tsa","/gsi/lustreMcbmNickel/2391_node9_0_*.tsa","/gsi/lustreMcbmNickel/2391_node9_1_*.tsa","/gsi/lustreMcbmNickel/2391_node9_2_*.tsa","/gsi/lustreMcbmNickel/2391_node9_3_*.tsa","/gsi/lustreMcbmNickel/2391_node9_4_*.tsa","/gsi/lustreMcbmNickel/2391_node9_5_*.tsa"},2391,"mcbm_beam_2021_07_surveyed",100)'
```
5) `cd /data/cbmroot/mcbmsource/macro/beamtime/mcbm2022/`
6) copy the correct calibration files from Tof to your location (`<location>/2365.5.lxbk0600/<The files from later down (scp)>`)  and fix the path in the macro `mcbm_event_reco.C`
```
scp lustre:/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2022/2365.5.lxbk0600_set042032500_93_1tofClust.hst.root .
scp lustre:/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2022/2365.5.lxbk0600_tofFindTracks.hst.root .
```
7) `root -l -b -q mcbm_event_reco.C`
