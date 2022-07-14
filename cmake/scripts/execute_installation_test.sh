#!/bin/bash

install_dir=$1
source $install_dir/bin/CbmRootConfig.sh -a

cd $install_dir/share/cbmroot/macro/run

$install_dir/share/cbmroot/macro/run/run_tests.sh
return_value=$?

exit $return_value
