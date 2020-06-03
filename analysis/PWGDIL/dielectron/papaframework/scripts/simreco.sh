#!/bin/bash

LOCATION=/lustre/nyx

## choose cbm root installation
source $LOCATION/cbm/users/$USER/CBMsoft/cbm-env.sh -n

## go to directory with macros
export OUTDIR="$1/"
mkdir -p $OUTDIR
cd $OUTDIR
echo $PWD

## copy rootrc
cp -v "../.rootrc" "."

## get cbm setup macro forseen
export SETUP=$(basename $OUTDIR/../setup* .C)
SETUP=${SETUP/setup_/}
echo "cbm setup is: $SETUP"

## number of events to process
NEVT=$2

## check files in output directory
echo "$OUTDIR"
ls -lhSra "$OUTDIR"

## simulation
if [ -f "$1/run_sim_${SETUP}_ok" ] ; then
    echo "mc file was produced already";
else
    echo "start mc generation";
    rm -v "$1/${SETUP}_mc.root"
    rm -v "$1/${SETUP}_reco.root"
    root -l -b -q "$PWD/../run_sim_new.C( $NEVT,\"$SETUP\")"

    # validate the root file
    if [ -f /cvmfs/fairroot.gsi.de/cbm/bin/validate_cbmroot.sh ] ; then
	export MACRODIR="/cvmfs/fairroot.gsi.de/cbm/macros"
	/cvmfs/fairroot.gsi.de/cbm/bin/validate_cbmroot.sh "$1/${SETUP}_mc.root" $NEVT
	result=$?
	if [ "$result" = "0" ]; then
	    echo "mc production was succesfull!";
	else
	    echo "mc production was not succesfull!";
	    rm -v "$1/run_sim_${SETUP}_ok"
	    rm -v "$1/${SETUP}_mc.root"
	fi
    fi
    echo
fi


## reconstruction
if [ -f "$1/run_reco_${SETUP}_ok" ] ; then
    echo "reco file was produced already";
elif [ -f "$1/run_sim_${SETUP}_ok" ] ; then
    echo "start reconstruction";
    rm -v "$1/${SETUP}_reco.root"
    root -l -b -q "$PWD/../run_reco_new.C( $NEVT,\"$SETUP\")"

    # validate the root file
    if [ -f /cvmfs/fairroot.gsi.de/cbm/bin/validate_cbmroot.sh ] ; then
	export MACRODIR="/cvmfs/fairroot.gsi.de/cbm/macros"
	/cvmfs/fairroot.gsi.de/cbm/bin/validate_cbmroot.sh "$1/${SETUP}_reco.root" $NEVT
	result=$?
	if [ "$result" = "0" ]; then
	    echo "reconstruction was succesfull!";
	else
	    echo "reconstruction was not succesfull!";
	    rm -v "$1/run_reco_${SETUP}_ok"
	    rm -v "$1/${SETUP}_reco.root"
	fi
    fi
    echo
else
    echo "mc production was not succesfull!";
    rm -v "$1/${SETUP}_mc.root"
fi


## validation
if [ -f "$1/run_reco_${SETUP}_ok" ] ; then
    echo "everything okay"
    ls -lhSra "$1"
    echo "clean up diretory"
    rm -v "$1/.rootrc"
    rm -v "$1/L1_histo.root"
    rm -v "$1/TRhistos.root"
    rm -v "$1/all_*.par"
    rm -v "$1/gphysi.dat"
    ls -lhSra "$1"
    echo "everything done";
else
    echo "validation failed!"
    rm -v "$1/${SETUP}_reco.root"
    if [ -f "$1/run_sim_${SETUP}_ok" ] ; then
	echo
	ls -lhSra "$1"
	echo "simulation okay";
    else
	rm -v "$1/${SETUP}_mc.root"
    fi
fi


