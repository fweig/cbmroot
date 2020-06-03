#!/bin/bash
var1=$(date +%s)
NOF=10

root -l -b -q "run_transport.C($NOF,\"sis100_electron\",\"00001\",\"/home/levi/work/cbmroot180419/cbmroot/macro/analysis/conversion2/data/test\",\"/home/levi/work/simulations/inputurqmd/Ievgenii_urqmd.00001.root\")" 
root -l -b -q "run_digi.C($NOF,\"sis100_electron\",\"00001\",\"/home/levi/work/cbmroot180419/cbmroot/macro/analysis/conversion2/data/test\")" 
root -l -b -q "run_reco_event.C($NOF,\"sis100_electron\",\"00001\",\"/home/levi/work/cbmroot180419/cbmroot/macro/analysis/conversion2/data/test\")" 
root -l -b -q "run_analysis.C($NOF,\"sis100_electron\",\"00001\",\"/home/levi/work/cbmroot180419/cbmroot/macro/analysis/conversion2/data/test\")" 
cd data
rootbrowse test_sis100_electron.analysis.00001.root


# source /home/levi/work/root616/build/bin/thisroot.sh
# cd /home/levi/work/cbm/mirror_rotation/
# root -l -b -q "Import_Semen.C"
# cp /home/levi/work/cbm/mirror_rotation/rich_vtest.geo.root /home/levi/work/cbmroot300318/cbmtest/geometry/rich/
# cd /home/levi/work/cbmroot300318/cbmtest/macro/analysis/conversion2/

# source /home/levi/work/cbmroot300318/cbmtest/build/config.sh

# cd data
# root -l -b -q "generator_input_file.C"
# cd -

# root -l "eventDisplay.C"

var2=$(date +%s)
echo $(($var2-$var1))
