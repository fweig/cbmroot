#!/bin/bash

#SBATCH -J MqStop
#SBATCH --oversubscribe

# Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# author: Pierre-Alain Loizeau [committer]

pkill -SIGINT RepReqTsSampler;

STILL_RUNNING=`ps | grep RepReqTsSampler | wc -l`
while [ 0 -gt $STILL_RUNNING ]; do
 sleep 1
 STILL_RUNNING=`ps | grep RepReqTsSampler | wc -l`
done

pkill -SIGINT MqUnpack;
STILL_RUNNING=`ps | grep MqUnpack | wc -l`
while [ 0 -gt $STILL_RUNNING ]; do
 sleep 1
 STILL_RUNNING=`ps | grep MqUnpack | wc -l`
done

pkill -SIGINT BuildDig;
STILL_RUNNING=`ps | grep BuildDigiEvents | wc -l`
while [ 0 -gt $STILL_RUNNING ]; do
 sleep 1
 STILL_RUNNING=`ps | grep BuildDigiEvents | wc -l`
done

pkill -SIGINT DigiEventSink;
STILL_RUNNING=`ps | grep DigiEventSink | wc -l`
while [ 0 -gt $STILL_RUNNING ]; do
 sleep 1
 STILL_RUNNING=`ps | grep DigiEventSink | wc -l`
done

pkill -SIGINT MqHistoServer;
STILL_RUNNING=`ps | grep MqHistoServer | wc -l`
while [ 0 -gt $STILL_RUNNING ]; do
 sleep 1
 STILL_RUNNING=`ps | grep MqHistoServer | wc -l`
done

pkill -SIGINT parmq-server
STILL_RUNNING=`ps | grep parmq-server | wc -l`
while [ 0 -gt $STILL_RUNNING ]; do
 sleep 1
 STILL_RUNNING=`ps | grep parmq-server | wc -l`
done
