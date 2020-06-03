#!/bin/bash
# shell script to iterate Analyzer histograms
cRun='CernSps05Mar0041'
#cRun='CernSps02Mar2227_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_020_nb_up'
#cRun='CernSps02Mar2202_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_030_nb_up'
#cRun='CernSps02Mar2151_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_030_nb_up'
#cRun='CernSps02Mar2120_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_010_nb_up'
#cRun='CernSps02Mar2056_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2044_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2031_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2024_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps01Mar1031_hdref_3ff_hdp2_220_thupad_170_thustrip_23c_bucref_0C0_buc2013_140_ustc_220_diam_0c0_nb'

iSel=39; iSet=397; iSel2=7
#iSel=94; iSet=943; iSel2=3
#iSel=16; iSet=168; iSel2=8
#iSel=86; iSet=168; iSel2=1
#iSel=61; iSet=168; iSel2=8
#iSel=81; iSet=168; iSel2=6
#iSel=18; iSet=168; iSel2=6
#iSel=68; iSet=168; iSel2=1
#iSel=81; iSet=168; iSel2=6

dDTres=1000000

while [[ $dDTres > 0 ]]; do

for iCal in 1 2 3 4 1
do
root -b -q 'ana_hits.C(10000000,'$iSel','$iCal',"'$cRun'",'$iSet','$iSel2') '
cp -v tofAnaTestBeam.hst.root ${cRun}_${iSet}_${iSel}${iSel2}_tofAnaTestBeam.hst.root
done

Tres=`cat Test.res`
dTdif=`echo "$dDTres - $Tres" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

echo got Tres = $Tres, compare to $dDTres, dTdif = $dTdif, compare_result = $compare_result

if [[ $compare_result > 0 ]]; then
dDTres=$Tres
else
dDTres=0
fi

done
