#!/bin/bash

# Rund the tests as they are run in the test suite
# Only for one setup
# Currently the script is manualy created from the CMakeLists.txt
# Also no dpendencies are available. The macros are executed in the
# given sequence

nEvents=3
nBeam=$((nEvents*3))
setup=sis100_electron
sname=sis100e


# Testing the transport macros
input=${VMCWORKDIR}/input/urqmd.auau.10gev.centr.root
root -l -q -b "run_tra_file.C(\"${input}\", ${nEvents}, \"data/${sname}_coll\", \"${setup}\")"


input=${VMCWORKDIR}/input/pluto.auau.8gev.omega.mpmm.0001.root
root -l -b -q "run_tra_file.C(\"${input}\", ${nEvents}, \"data/${sname}_sign\", \"${setup}\")"

root -l -b -q "run_tra_beam.C(${nBeam}, \"Au\", 10, -1, \"data/${sname}_beam\", \"${setup}\")"

root -l -b -q run_transport_json_config.C


# Testing the digitization macros
root -l -b -q run_digi_json_config.C

root -l -b -q "run_digi.C(\"data/${sname}_coll\", -1, \"data/${sname}_ev\", -1.)"

eventrate=1.e7
beamrate=1.e9
tslength=1.e6
root -l -b -q "run_digi.C(\"data/${sname}_coll\", -1, \"data/${sname}_ts\", \
                          ${eventrate}, ${tslength}, \"data/${sname}_sign\", \
                          \"data/${sname}_beam\", ${beamrate})"


# Testing the reconstruction macros 
root -l -b -q "run_reco.C(\"data/${sname}_ev\", -1, 0, \"data/${sname}_eb_eb_ideal\", \
                         \"Ideal\", \"${setup}\", \"data/${sname}_coll\")"

root -l -b -q "run_reco.C(\"data/${sname}_ev\", -1, 0, \"data/${sname}_eb_eb_real\", \
                          \"Real\", \"${setup}\", \"data/${sname}_coll\")"

root -l -b -q "run_reco.C(\"data/${sname}_ts\", -1, 0, \"data/${sname}_ts_eb_ideal\", \
                          \"Ideal\", \"${setup}\", \"data/${sname}_coll\")"

root -l -b -q "run_reco.C(\"data/${sname}_ts\", -1, 0, \"data/${sname}_ts_eb_real\", \
                          \"Real\", \"${setup}\", \"data/${sname}_coll\")"

root -l -b -q "run_reco.C(\"data/${sname}_ts\", -1, 0, \"data/${sname}_ts_tb\", \
                          \"\", \"${setup}\", \"data/${sname}_coll\")"

