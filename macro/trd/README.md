# Summary
1. [Generating the parameter files](#generating-the-parameter-files-from-a-geometry)
2. [Simulation](#run-simulation)
3. [TRD macros](#trd-macros)

## Generating the parameter files from a geometry
The following ordered steps should be performed in order to produce the parameter files for the TRD system. All files and directories are given relative to the CbmRoot source directory.

1. Run the create geometry macro. Examples of such macro can be found in  **macro/trd/geometry/trd.v??** or **macro/mcbm/geometry/trd** and are named **Create_TRD_Geometry_[tag].C** where tag is a unique identifier of the geometry. Examples are tag = "v22a_mcbm". The following files will be created by the previous procedure:
    1. trd_[tag].geo.info;
    2. trd_[tag].geo.root;
    3. trd_[tag]_geo.root;
    4. CbmTrdPads_[tag].h;
2. Copy the first two files **geometry/trd**. In the case when the geometry of the pad-plane is not of a default type as implemented in **core/detectors/trd/CbmTrdPads.h** replace also this file with the one newly generated CbmTrdPads_[tag].h.
3. Compile the code by running **make install** on your build directory
4. Execute **macro/trd/create_digipar_root.sh tag**. The following **four** files will be created: 
    - trd_[tag].asic.par
    - trd_[tag].digi.par
    - trd_[tag].gain.par
    - trd_[tag].gas.par
5. Move these files to the **parameters/trd** directory and execute item **3.** again.    
    
## Run simulation 
In the **macro/trd** directory execute the following scripts:
- root -l run_sim.C
- root -l run_reco.C
- root -l eventDisplay.C 

Standard file contains only 5 events for run_sim in **input/urqmd.ftn14**. If you need more, get urqmd.auau.25gev.centr.0000.ftn14
from [here](http://cbm-wiki.gsi.de/cgi-bin/view/Computing/CbmDataUrQMD13) and link it to the input directory.


## TRD macros

| Macro name | Description |
| -------- | ------------- |
| sim.C | makes the transport of UrQMD events. Output files are *test.mc.root* and *test.mc.param.root* |
| Sts.C | runs reconstruction in the STS. Output file is *test.stsTracks.root* |
| CbmTrdHitProducer.C | produces TRD hits from TRD points. Output file is *test.trdHits.root* |
| TrdTrackFinderIdeal.C | runs ideal TRD track finder and Kalman Filter track fitter together with performance tasks. Output file is *test.trdIdealTracks.root* |
| TrdTrackFinderSts.C | runs TRD track finder, based on track following from STS approach. Found tracks are fitted using the Kalman Filter fitter. Output file is *test.trdTracks.root* |


