#/bin/bash
# check_radlen.sh - checks radiation length of materials in user supplied geometry
# Author: Eoin Clerkin (FAIR)  2022-01-31

echo "Scanning the geometry" $1

root -l -b -q '$VMCWORKDIR/macro/geometry/scan_geometry.C("'$1'")' &>tmp
ROOT_EXIT=$?;

if [ 0 -ne $ROOT_EXIT ]; then
  echo "root command failed, nothing more done";
  exit ${ROOT_EXIT};
fi;

grep '^M\(at\|ix\)' tmp | \
sort | \
uniq -c | \
sort -g -k 1 1>MATERIALS

COUNT=0;
FAIL=0;
SKIP=0;
OKAY=0;


while IFS= read -r line;
do

variables=`echo "$line" | sed -e 's/eff//g' | sed -e 's/index/jndex/g' | sed -e 's/.*A=/ A=/g' | sed -e 's/ / -v /g'`

    awk \
    -v TOL=0.1 \
    $variables \
    'BEGIN{\
        if(Z<1){
        printf "SKIP \t "; \
        exit 3;
        };
        cal_rad_len=(716.4*A/(Z*(Z+1)*log(287/sqrt(Z)))/rho);\
        #print cal_rad_len;
        diff=(cal_rad_len - radlen);\
        if(diff*diff <= TOL*TOL*radlen*radlen ){\
        printf "OKAY \t " cal_rad_len; exit 1;
        }else{\
        printf "FAIL \t " cal_rad_len; exit 2;
        }}'

    STATUS=$?;

    if [ $STATUS -eq 1 ]; then OKAY=$((OKAY+1)); fi;
    if [ $STATUS -eq 2 ]; then FAIL=$((FAIL+1)); fi;
    if [ $STATUS -eq 3 ]; then SKIP=$((SKIP+1)); fi;

    COUNT=$((COUNT+1));

    echo -e " \t $line"

done < MATERIALS

rm tmp MATERIALS


echo ${FAIL}" failures in " $COUNT " materials"

echo "ENDED successfully"

exit ${FAIL};
