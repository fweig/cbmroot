#!/bin/bash

install_dir=$1
source $install_dir/bin/CbmRootConfig.sh

cd $install_dir/share/cbmroot/macro/run

$install_dir/share/cbmroot/macro/run/run_tests.sh
return_value=$?

exit $return_value

#cd $install_dir/share/cbmroot/macro
#pwd
#ls -la
#$install_dir/share/cbmroot/macro/checklibs.sh $PWD $install_dir/lib
