/* Copyright (C) 2014-2016 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Timur Ablyazimov [committer] */

#ifndef LXSETTINGS_INCLUDED
#define LXSETTINGS_INCLUDED

#define LXPI 3.141592653589793238462643383279

#define scaltype float

#define LXLAYERS 3
#define LXSTATIONS 6
#define LXMIDDLE 1
#define LXFIRSTSTATION 0
#define LXSTSSTATIONS 8
#define LXMAXPOINTSONSTATION 4096

#define MAKE_HISTOS

#ifdef MAKE_HISTOS
//#define MAKE_ANGLE_HISTOS
//#define MAKE_BRAKE_ANGLE_HISTOS
//#define MAKE_DISPERSE_HISTOS
//#define MAKE_DISPERSE_2D_HISTOS
//#define MAKE_MU_PLUS_MU_MINUS_HISTOS
//#define MAKE_DIFF_FIT_HISTOS
//#define MAKE_DIFF_FIRST_HISTOS
//#define MAKE_FIT_COEFFS_HISTOS
//#define MAKE_LAST_STS_NEAREST_Y
#define MAKE_TRIGGERING_HISTOS

#endif  //MAKE_HISTOS

#define MAKE_STS_MC_RESEARCHES
#define MAKE_EFF_CALC
//#define CALC_MUCH_DETECTORS_EFF
//#define CALC_LINK_WITH_STS_EFF

//#define CLUSTER_MODE

#ifdef CLUSTER_MODE

#define DENSE_CLUSTERS_FIRST
#define BEST_RAYS_ONLY

#ifdef BEST_RAYS_ONLY

#define REMOVE_SUBCLUSTER
#define BEST_SIX_POINTS

#endif  //BEST_RAYS_ONLY

#define REMEMBER_CLUSTERED_RAYS_IN_POINTS

#endif  //CLUSTER_MODE

//#define USE_KALMAN
//#define USE_KALMAN_FIT

#define LX_DEBUG

#define USE_OLD_STS_LINKING_RULE
//#define LX_EXT_LINK_SOPH

#endif  //LXSETTINGS_INCLUDED
