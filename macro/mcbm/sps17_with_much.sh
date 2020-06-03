#
##   modify cbmMuch.cxx to include mMUCH
#

# by David Emschermann
# 10.03.2017

# 15.08.2017 - DE - modify for SPS 2017
# 05.04.2017 - DE - removed mcbm_sim.C
# 04.04.2017 - DE - added possibility to run mcbm_mc.C

cd ../..
patch much/geo/CbmMuch.cxx macro/mcbm/CbmMuch.cxx_to_Sps17Geo.patch
nice make -C build
cd macro/mcbm

# now run sps17_mc.C
root -l sps17_mc.C

# to restore CbmMuch to the original version do:
# cd "trunk"
# rm much/geo/CbmMuch.cxx
# svn up much/geo/CbmMuch.cxx 
# nice make -j3 -C build
