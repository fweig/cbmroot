#!/bin/bash
# Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig


# Rund the tests as they are run in the test suite
# Only for one setup
# Currently the script is manualy created from the CMakeLists.txt
# Also no dpendencies are available. The macros are executed in the
# given sequence


main () {
  nEvents=3
  nBeam=$((nEvents*3))
  setup=sis100_electron
  sname=sis100e

  echo ""

  # Testing the transport macros
  log_file_name=data/run_tra_file_urqmd.log
  check_string="Macro finished successfully"

  input=${VMCWORKDIR}/input/urqmd.auau.10gev.centr.root
  execute_macro "run_tra_file.C(\"${input}\", ${nEvents}, \"data/${sname}_coll\", \"${setup}\", kGeant3, 1, kTRUE)" \
                $log_file_name \
                $check_string
     
  
  log_file_name=data/run_tra_file_pluto.log
  check_string="Macro finished successfully"
  
  input=${VMCWORKDIR}/input/pluto.auau.8gev.omega.mpmm.0001.root
  execute_macro "run_tra_file.C(\"${input}\", ${nEvents}, \"data/${sname}_sign\", \"${setup}\", kGeant3, 1, kTRUE)" \
                $log_file_name \
                $check_string
 

  log_file_name=data/run_tra_beam.log
  check_string="Macro finished successfully"

  execute_macro "run_tra_beam.C(${nBeam}, \"Au\", 10, -1, \"data/${sname}_beam\", \"${setup}\", kGeant3, 1, kTRUE)" \
                $log_file_name \
                $check_string

  log_file_name=data/run_tra_json.log
  check_string="Macro finished successfully"

  execute_macro "run_transport_json_config.C(\"config_ci.json\")" \
                $log_file_name \
                $check_string
 
  # Testing the digitization macros
  log_file_name=data/run_digi_json.log
  check_string="Macro finished successfully"

  execute_macro "run_digi_json_config.C(\"config_ci.json\")" \
                $log_file_name \
                $check_string


  log_file_name=data/run_digi_eb.log
  check_string="Macro finished successfully"
  execute_macro "run_digi.C(\"data/${sname}_coll\", -1, \"data/${sname}_ev\", -1.)" \
                $log_file_name \
                $check_string


  log_file_name=data/run_digi_tb.log
  check_string="Macro finished successfully"

  eventrate=1.e7
  beamrate=1.e9
  tslength=1.e6
  execute_macro "run_digi.C(\"data/${sname}_coll\", -1, \"data/${sname}_ts\", \
                            ${eventrate}, ${tslength}, \"data/${sname}_sign\", \
                            \"data/${sname}_beam\", ${beamrate})" \
                $log_file_name \
                $check_string

  # Testing the reconstruction macros 
  log_file_name=data/run_reco_eb_eb_ideal.log
  check_string="Macro finished successfully"

  execute_macro "run_reco.C(\"data/${sname}_ev\", -1, 0, \"data/${sname}_eb_eb_ideal\", \
                            \"Ideal\", \"${setup}\", \"data/${sname}_coll\")" \
                $log_file_name \
                $check_string

  log_file_name=data/run_reco_eb_eb_real.log
  check_string="Macro finished successfully"

  execute_macro "run_reco.C(\"data/${sname}_ev\", -1, 0, \"data/${sname}_eb_eb_real\", \
                            \"Real\", \"${setup}\", \"data/${sname}_coll\")" \
                $log_file_name \
                $check_string

  log_file_name=data/run_reco_ts_eb_ideal.log
  check_string="Macro finished successfully"

  execute_macro "run_reco.C(\"data/${sname}_ts\", -1, 0, \"data/${sname}_ts_eb_ideal\", \
                            \"Ideal\", \"${setup}\", \"data/${sname}_coll\")" \
                $log_file_name \
                $check_string

  log_file_name=data/run_reco_ts_eb_real.log
  check_string="Macro finished successfully"

   execute_macro "run_reco.C(\"data/${sname}_ts\", -1, 0, \"data/${sname}_ts_eb_real\", \
                            \"Real\", \"${setup}\", \"data/${sname}_coll\")" \
                 $log_file_name \
                 $check_string

  log_file_name=data/run_reco_ts_tb.log
  check_string="Macro finished successfully"

  execute_macro "run_reco.C(\"data/${sname}_ts\", -1, 0, \"data/${sname}_ts_tb\", \
                            \"\", \"${setup}\", \"data/${sname}_coll\")" \
                 $log_file_name \
                 $check_string

  exit 0
}

check_result () {
  result=$1
  check_string=$2
  logfile=$3
  string_compare=$(grep "$check_string" $logfile)
  string_result=$?
#  if [[ ( $result -ne 0 ) || ( $string_result -ne 0 ) ]]; then
  if [[ ( $string_result -ne 0 ) ]]; then
    echo "Running the macro has failed"
    echo "See the error below"
    echo ""
    cat $logfile
    exit 1
  else
    echo "Macro finished successfuly"
    echo ""
  fi
}

execute_macro () {
  macro_info=$1
  log_file_name=$2
  check_string=$3
  echo "Executing macro $macro_info"
  root -l -q -b "$macro_info" &> $log_file_name
  result=$?
  check_result $result "$check_string" $log_file_name
}

main "$@"
