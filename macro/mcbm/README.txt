#
##   Howto run macros in macro/mcbm
#

# by David Emschermann
# 05.08.2019 - updated
# 04.04.2017 - initial version


# 05.08.2019
# mcbm_mc.C was split into two separate macros:
root -l mcbm_transport.C
root -l mcbm_digi.C

#=============================================

# to initially convert MUCH into miniMUCH run the following script once
mcbm_with_much.sh
# it modifies the much geometry class in your cbmroot installation

# then either run
root -l mcbm_mc.C
root -l mcbm_reco.C


# Details by Volker Friese:
The "correct" sequence is mcbm_mc.C (transport) and mcbm_reco.C (digitisation + reconstruction).

mcbm_sim.C uses the legacy setup scheme, but it is working also; I have tried to update it
accordingly.

To the naming: In macro/run, run_sim.C combines transport and digitisation. This is currently not
needed, since it is more convenient to do the digitisation in the reco macro, before the actual
reconstruction. However, in the time-based mode, digitisation and reconstruction will have to be
separated. But for the moment, in the event-by-event mode, you can just use these two macros (mc and
reco).

#  mcbm_sim.C was therefore moved to the legacy folder



#
##  STS v18f - acceptance
#

# Station 1 (2x2)
outer dimensions
x: -6.059 .. 6.059 = 12.118 cm
y: -6.065 .. 6.065          cm - shifted upwards
y: -6.200 .. 5.930 = 12.130 cm - delta 0.270 cm (halfladder)

atan(6./28) / acos(-1) * 180
12.0948
atan(12.13/2/28) / acos(-1) * 180
12.2219

# -0.1 for guard ring
atan(5.9/28) / acos(-1) * 180


# Station 2 (3x3)
outer dimensions
x: -9.015 .. 9.015 = 18.030 cm
y: -9.030 .. 9.030          cm - shifted upwards
y: -9.165 .. 8.895 = 18.060 cm - delta 0.270 cm

atan(9./42) / acos(-1) * 180
12.0948
atan(18.06/2/42) / acos(-1) * 180
12.1339

# -0.1 for guard ring
atan(8.9/42) / acos(-1) * 180


#
##  STS v18g - acceptance
#

# guard and bias ring width 
# sizes provided from DXF file by Johann
ring_xWidth = 0.125;  // width of surrounding ring in cm = 1.25 mm                     
ring_yWidth = 0.132;  // width of surrounding ring in cm = 1.32 mm

# Station 1 (2x2)
outer dimensions
x: -5.934 .. 5.934 = 11.868 cm
y: -5.933 .. 5.933 = 11.866 cm

atan(11.87/2/28) / acos(-1) * 180
11.9675

# Station 2 (3x3)
active area only
x: -8.889 .. 8.889 = 17.778 cm
y: -8.898 .. 8.898 = 17.796 cm

atan(17.78/2/42) / acos(-1) * 180
11.9512

# MUCH - layer 2
atan(16.65/79) / acos(-1) * 180
11.9015

# MUCH - layer 3
atan(16.65/89) / acos(-1) * 180
10.5963

# TRD - layer 4
atan(46./190) / acos(-1) * 180

# TOF - x
atan(58.5/236) / acos(-1) * 180
13.9219

atan(-93.5/236) / acos(-1) * 180
-21.6128

# TOF - y
atan(63.5/231) / acos(-1) * 180
15.3705

#
##   UrQMD input files for mCBM energies
#

# more urqmd input files are available here:

/hera/cbm/prod/gen/urqmd/agag/1.65gev/centr/urqmd.agag.1.65gev.centr.00001.root
/hera/cbm/prod/gen/urqmd/agag/1.65gev/mbias/urqmd.agag.1.65gev.mbias.00001.root
/hera/cbm/prod/gen/urqmd/nini/1.93gev/centr/urqmd.nini.1.93gev.centr.00001.root
/hera/cbm/prod/gen/urqmd/nini/1.93gev/mbias/urqmd.nini.1.93gev.mbias.00001.root
/hera/cbm/prod/gen/urqmd/pau/4.5gev/mbias/urqmd.pau.4.5gev.mbias.00001.root
