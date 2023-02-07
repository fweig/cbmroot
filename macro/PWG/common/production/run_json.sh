#!/bin/bash

function getJsonVal () {
  val=$(${_python} -c "import json;print(json.dumps(json.load(open('${config}'))$1))";)
  eval echo ${val}
} # e.x. 'outPath=$(getJsonVal "['transport']['output']['path']")'

#function checkJsonKey () {
#  python -c "import json,sys;print ('$1' in json.load(open('${config}')))";
#} # returns True if key exists, False if not, e.x. 'run_transport=$(checkJsonKey "transport")'

function readStepInfo () {
  run=$(getJsonVal "['accessory']['${step}']['run']")
  if [ ${run} == true ]; then
    overwrite=$(getJsonVal "['${step}']['output']['overwrite']")
    outDir=$(getJsonVal "['${step}']['output']['path']")
    [[ ${outDir} != */ ]] && outDir=$(dirname ${outFile})
    srcDir=${outDir}/macro
    macro=$(getJsonVal "['accessory']['${step}']['macro']")
    macroName=$(basename ${macro})
  fi
}

# find the proper python executable and exit if not found
if [[ ! -z $PYHON_EXEC ]]; then
  _python=$PYHON_EXEC
else
  _python=$(which python)
  if [[ -z $_python ]]; then
    _python=$(which python2)
    if [[ -z $_python ]]; then
      _python=$(which python3)
      if [[ -z $_python ]]; then
        echo "No python interpreter found"
        exit 1
      fi
    fi
  fi
fi
export _python=${_python}

submitScript=${0}
echo "Submit script : ${submitScript}"
config=${1}
batch=$(getJsonVal "['accessory']['batch']")
jobScript=$(getJsonVal "['accessory']['jobScript']")
export cbmRoot=$(getJsonVal "['accessory']['cbmRoot']")
cbmRoot="${cbmRoot} -a"
echo "CbmRoot : ${cbmRoot}"
source ${cbmRoot}

steps="transport digitization reconstruction AT"
for step in ${steps}; do
  readStepInfo
  if [ ${run} == true ]; then
    mkdir -pv ${srcDir}
    cp -v ${macro} ${srcDir}
    cp -v ${config} ${srcDir}
    cp -v ${submitScript} ${srcDir}
    cp -v ${jobScript} ${srcDir}
  fi
done

export -f getJsonVal
export -f readStepInfo

export configName=$(basename ${config})
export config=${srcDir}/${configName}
export nEvents=$(getJsonVal "['accessory']['nEvents']")
jobRange=$(getJsonVal "['accessory']['jobRange']")
logDir=$(getJsonVal "['accessory']['logDir']")
mkdir -pv ${logDir}

if [ ${batch} == true ];then
  account=$(getJsonVal "['accessory']['account']")
  ram=$(getJsonVal "['accessory']['ram']")
  partition=$(getJsonVal "['accessory']['partition']")
  time=$(getJsonVal "['accessory']['time']")
  jobName=$(getJsonVal "['accessory']['jobName']")
  excludeNodes=$(getJsonVal "['accessory']['excludeNodes']")
  sbatch -A ${account} --mem=${ram} -p ${partition} -t ${time} -J ${jobName}\
    -a ${jobRange} -o ${logDir}/%a_%A.log --export=ALL \
    --exclude=${excludeNodes} -- ${jobScript}
else
  echo "Jobscript: ${jobScript}"
  export SLURM_ARRAY_TASK_ID=${jobRange}
  ${jobScript} &> ${logDir}/${jobRange}.log
fi
