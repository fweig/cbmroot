#!/bin/bash

echo ${MERGE_ROOT}
echo ${INDIR}

hadd -T ${MERGE_ROOT} ${INDIR}
