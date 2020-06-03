#
##   Optimisation of mCBM geometries and acceptance
#

# 03.11.2017

# by David Emschermann


# Last (2nd) mSTS module
geometrical acceptance:
12.0 degrees (see mCBM proposal table)

atan(5.9/28) * 180 / acos(-1.) = 11.90 degrees
atan(8.9/42) * 180 / acos(-1.) = 11.96 degrees


# Last (4th) mTRD module
geometrical acceptance:
13.6 degrees (see mCBM proposal table)

atan(45.5/190) * 180 / acos(-1.) = 13.47 degrees

46.0 / tan(12.0 / 180 * acos(-1.)) = 216 cm

215 cm - 190 cm = 25 cm
115 cm +  25 cm = 140 cm z-position


# mBUCH module

atan(27.0/100) * 180 / acos(-1.) = 15.11 degrees

27.0 / tan(12.0 / 180 * acos(-1.)) = 127 cm

=> 125 cm z-position

atan(27.0/125) * 180 / acos(-1.) = 12.19 degrees


# vertical TOF
front RPC cell position
z= 225 cm
sixe in x: -63.5 .. 63.5 cm

current angle:
atan(63.5/225) * 180 / acos(-1.) = 15.76 degrees

z position to reach 13.6 degrees:
63.5 / tan(13.6 / 180 * acos(-1.)) = 262.5 cm
=> this is +37.5 cm in z

z position to reach 12.0 degrees:
63.5 / tan(12.0 / 180 * acos(-1.)) = 298.7 cm
=> this is +73.7 cm in z



# mCBM subsystem acceptance

# MVD v18a / v18b

station 0   x = 0.55 cm,  y =  2.15 cm,  z =   5 cm  -> x: atan(0.55/5) = 6.28 deg, y: atan(2.14/5) = 23.3 deg
station 1   x = 3.2  cm,  y =  2.4  cm,  z =  10 cm  -> x: atan(3.2/10) = 17.7 deg, y: atan(2.4/10) = 13.5 deg


# STS v18e

station 1   y =  6 cm,  z =  30 cm  -> atan(6/30) = 11.3 deg
station 2   y =  9 cm,  z =  40 cm  -> atan(9/40) = 12.7 deg


# TRD v18b

station a   y = 28.5 cm,  z =  142.2 cm  -> atan(28.5/142.2) = 11.3 deg (hypothetical)
station b   y = 47.5 cm,  z =  187.2 cm  -> atan(47.5/187.2) = 14.2 deg (hypothetical)
station 1   y = 57   cm,  z =  232.2 cm  -> atan(57/232.2)   = 13.8 deg
station 2   y = 57   cm,  z =  277.2 cm  -> atan(57/277.2)   = 11.6 deg


# TOF v18b

station 1   y = 65 cm,  z =  350.0 cm  -> atan(65/350.0) = 10.5 deg
