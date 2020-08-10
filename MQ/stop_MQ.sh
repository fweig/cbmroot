#!/bin/bash
pkill -SIGINT Hit
pkill -SIGINT Unp
sleep 30
pkill -9 Tsa
pkill -9 Hit
pkill -9 Unp
pkill -9 parmq
