#!/bin/bash

steps="transport digitization reconstruction AT"
source ${cbmRoot}
for step in ${steps}; do
  readStepInfo
  if [ ${run} == true ]; then
    export taskId=${SLURM_ARRAY_TASK_ID}
    plutoShift=$(getJsonVal "['accessory']['transport']['plutoShift']")
    export plutoFileId=$(printf %05d $((${taskId}-${plutoShift})))
    config=${srcDir}/${configName}
    macro=${srcDir}/${macroName}
    outFile=$(getJsonVal "['${step}']['output']['path']")
    outDir=$(dirname ${outFile})
    log=${outDir}/${step}.log

    mkdir -pv ${outDir}
    cd ${outDir}
    ln -sfv ${VMCWORKDIR}/macro/run/.rootrc ${outDir} 
    if [ ${step} == reconstruction ]; then
      rawFile=$(getJsonVal "['reconstruction']['rawFile']")
      nTimeSlices=$(getJsonVal "['reconstruction']['nTimeSlices']")
      firstTimeSlice=$(getJsonVal "['reconstruction']['firstTimeSlice']")
      sEvBuildRaw=$(getJsonVal "['reconstruction']['sEvBuildRaw']")
      traFile=$(getJsonVal "['reconstruction']['traFile']")
      isL1Matching=$(getJsonVal "['reconstruction']['isL1Matching']")
      isL1EffQA=$(getJsonVal "['reconstruction']['isL1EffQA']")
      echo "  "
      echo "Run reconstruction: ${macro}(\"${rawFile}\",${nTimeSlices},${firstTimeSlice},\"${outFile}\",\
	${overwrite},\"${sEvBuildRaw}\",\"${config}\",\"${traFile}\",${isL1Matching},${isL1EffQA})"
      root -b -l -q "${macro}(\"${rawFile}\",${nTimeSlices},${firstTimeSlice},\"${outFile}\",\
        ${overwrite},\"${sEvBuildRaw}\",\"${config}\",\"${traFile}\",${isL1Matching},${isL1EffQA})" &> ${log}
    elif [ ${step} == AT ]; then
      traFile=$(getJsonVal "['AT']['traFile']")
      rawFile=$(getJsonVal "['AT']['rawFile']")
      recFile=$(getJsonVal "['AT']['recFile']")
      unigenFile=$(getJsonVal "['AT']['unigenFile']")
      eventMode=$(getJsonVal "['digitization']['eventMode']")
      tslength=-1;
      if [ ${eventMode} == false ]; then
      	tslength=$(getJsonVal "['digitization']['timeSliceLength']")
      fi
      echo "  "
      echo "Run AT converter: ${macro}(\"${traFile}\",\"${rawFile}\",\"${recFile}\",\
	\"${unigenFile}\",\"${outFile}\",${overwrite},\"${config}\",\"${tslength}\")" 
      root -b -l -q "${macro}(\"${traFile}\",\"${rawFile}\",\"${recFile}\",\
	\"${unigenFile}\",\"${outFile}\",${overwrite},\"${config}\",\"${tslength}\")" &> ${log}
    else 
      if [ ${step} == digitization ]; then
        input=$(getJsonVal "['transport']['output']['path']")
        if [ ! -e ${outFile}.par.root ] || [ ${overwrite} == true ]; then
          cp -v ${input}.par.root ${outDir}
        fi
      echo "  "
      echo "Run digitization: ${macro}(\"${config}\",${nEvents})"
      fi
      
      if [ ${step} == transport ]; then
	echo "  "
	echo "Run transport: ${macro}(\"${config}\",${nEvents})"
      fi
       
      root -b -l -q "${macro}(\"${config}\",${nEvents})" &> ${log}  
    fi
    gzip -f ${log}

    rm -v .rootrc
    if [ ${step} == reconstruction ]; then
	if [ "${isL1EffQA}" == true ]; then
    	  rm -v *{core,moni,CA}* all_*.par 
	else
	  rm -v *{core,moni,CA,L1,Edep}* all_*.par
    	fi
    fi
    if [ ${step} == digitization ]; then
	rm -v *{moni,Fair}* all_*.par
    fi
    if [ ${step} == AT ]; then
	rm -v *{core,CA,L1}*
    fi

    cd -
    export taskId=
  fi
done
