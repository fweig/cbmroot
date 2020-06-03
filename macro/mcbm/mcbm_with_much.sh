#
##   modify cbmMuch.cxx to include mMUCH
#

# by David Emschermann
# 10.03.2017

# 18.10.2017 - DE - MUCH switched to rootified geometries with rev 12112, this patching is no longer needed
# 05.04.2017 - DE - removed mcbm_sim.C
# 04.04.2017 - DE - added possibility to run mcbm_mc.C

cd ../..
patch much/geo/CbmMuch.cxx macro/mcbm/CbmMuch.cxx_to_McbmGeo.patch
nice make -C build
cd macro/mcbm

# now run mcbm_mc.C
root -l mcbm_mc.C

# to restore CbmMuch to the original version do:
# cd "trunk"
# rm much/geo/CbmMuch.cxx
# svn up much/geo/CbmMuch.cxx 
# nice make -j3 -C build
