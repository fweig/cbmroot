#!/bin/bash

LOCATION=/lustre/nyx

## choose cbm root installation
source $LOCATION/cbm/users/$USER/CBMsoft/cbm-env.sh -n

## export variables needed by urqmd
export ftn09="$1/input"
export ftn13="/tmp/urqmd.f13"
export ftn14="$1/urqmd.f14"
export ftn15="/tmp/urqmd.f15"
export ftn16="/tmp/urqmd.f16"
export ftn19="/tmp/urqmd.f19"
export ftn20="/tmp/urqmd.f20"
echo "inputfile:  $ftn09"
echo "outputfile: $ftn14"

## number of events to process
NEVT=$2

## check files in output directory
echo "$1"
ls -lhSr "$1"

## run urqmd
if [[ -f "$1/urqmd.f14" ]] || [[ -f "$1/urqmd.root" ]] ; then
    echo "urqmd file was produced already";
else
    echo "start urqmd generation";
    rm -rfv "$1/urqmd.f14"
    rm -rfv "$1/urqmd.root"
    time urqmd.$(uname -i)
fi


## run unigen
if [ -f "$1/urqmd.root" ] ; then
    echo "unigen file was produced already";
elif [ -f "$1/urqmd.f14" ] ; then
    echo "start unigen";
    time urqmd2u  "$1/urqmd.f14" "$1/urqmd.root" $NEVT
    echo
else
    echo "unigen production was not succesfull!";
fi

## validation
if [ -f "$1/urqmd.root" ] ; then
    echo
    ls -lhSr "$1"
    echo "clean up done";
    rm -v "$1/urqmd.f14"
    rm -v "$1/input"
    echo "everything done";
else
    echo "validation failed!"
    rm -rfv "$1/urqmd.root"
    if [ -f "$1/urqmd.f14" ] ; then
	echo
	ls -lhSr "$1"
	echo "only urqmd production okay";
    fi
fi


