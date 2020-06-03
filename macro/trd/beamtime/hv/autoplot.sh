#!/bin/bash

FILE=1

while [ $FILE -le 141 ]
do
sed -n ${FILE}p /mount/scr1/p_kaeh01/hv/allnames.config > /mount/scr1/p_kaeh01/hv/tempname.config
root -b -q monHVlong.C++
let FILE=$FILE+1
done
