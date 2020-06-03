2nd version: PAL, 2014/05/05, ploting macros tested with HV files 
             Log_Slot*_200414_2249.csv, conversion macros tested on all .csv 
             files and matching macros tested on MbsTrbThu0036_(8 to 15).lmd

To plot directly HV slow control data from the ASCII files (no combination with 
MBS event scalers):
1) Try running the example macro called currentPlot.C for the Heidelberg RPC 
   with the command: root -l "currentPlot.C(0,200414, 2249)"
2) Try running this macro for your own counter after looking at the source file
3) Modify the macro by adding your own histograms or create a fully new macro 
   on the same model
Macro: currentPlot.C -> READS one ASCII file and PLOTS currents for one detector
                        (RPC or PMT)

To get the ASCII data taken during the beamtime in root format:
1) Copy the already generated root files from the gsi lx cluster at 
   /SAT/hera/cbm/users/tofGsiApr14/currentsCSV
   The total size is ~111Mb, with 68 Mb for the file containing the full 
   beamtime in a single tree and 49Mb for the individual runs.
OR
1) Copy the files currentTreeConv.C, loopTreeConv.C,  mergeTrees.C and 
   StructDef.h in the same folder as the ".csv" files (ASCII files).
2) Load your beamtime cbmroot version (beamlogin shortcut or ". config.sh" or 
   ...)
3) Run the command: root -l "loopTreeConv.C(0)" >> Log.txt
   This will create a root file for each slow control run and a common root 
   file with all data.
=> The data format used to store the data in the tree can be found in the file 
   StructDef.h
=> The parameter given to the macro to generate the files is an offset in sec. 
   relative to the MBS event. I kept it to 0 in the files on the lx server.
Macros: currentTreeConv.C -> READS one ASCII file and CONVERT it to ROOT format
        mergeTrees.C      -> Merge the ROOT files from all ASCII files
        loopTreeConv.C    -> Loop over all ASCII files, convert them and then 
                             merge them

To combine the HV data with the MBS scalers:
1) a. Add a line with "tofTriglogUnpacker->SetSaveTriglog();" in your analysis 
      macro (unpack_trb.C or ana_lmd.C or ... ) between the lines with 
     "TTriglogUnpackTof* tofTriglogUnpacker = new TTriglogUnpackTof();" and
     "source->AddUnpacker( tofTriglogUnpacker );"
      => This will activate the saving of the trigger board information in the 
         output file for each event (event time, trigger index, ...)
   b. Optionally, add a line with "tofTriglogUnpacker->SetSaveScalers();" in 
      your analysis macro (unpack_trb.C or ana_lmd.C or ... ) at the same place
      => This will activate the saving of the raw scalers data in the output 
         file for each event (just scaler value, include trigger board scalers)
   c. Add a line with "tofCalibration->SetSaveScalers();" in your analysis 
      macro (unpack_trb.C or ana_lmd.C or ... ) between the lines with 
"TMbsCalibTof* tofCalibration = new TMbsCalibTof("Tof Calibration", iVerbose);" 
      and "source->AddUnpacker( tofCalibration );"
      => This will activate the saving of the calibrated scalers data in the 
         output file for each event (scaler rate, with trigger board scalers)
2) Run your analysis (including the Finish statement) to generate a new 
   unpack.out.root file with those additional data.
3) Copy in the same folder the unpack.out.root file with the MBS scalers data, 
   the LogHv_Full_GsiApr14.root file and the hvScalUseExample.C macro
4) Try running the example macro with the command:
   root -l "hvScalUseExample.C(0, 2, 3)"
5) Modify the macro by adding your own histograms or create a fully new macro 
   on the same model
=> In principle it should be possible to run the macro using single run HV root
   files, but I am not sure of the correspondance between the LMD scaler runs 
   and the ASCII HV runs 
Macro: hvScalUseExample.C -> Example of how to find a common start point in time 
                             between the first MBS event and an HV point serie 
                             and of how to plot HV currents as function of rate

