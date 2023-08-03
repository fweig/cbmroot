#!/bin/bash

# This macro is used to generate the ROOT dictionaries
# To use the ctest launchers one needs some environment variables which
# are only present when running CMake. To have the same environment at
# the time the dictionary is build this script is used which is build
# at the time cmake runs.

# Setup the needed environment
export LD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib::/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib
export DYLD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib::/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib
export ROOTSYS=

/home/cbmdata/_deps/fairsoft_apr22/install/bin/rootcling -f /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/analysis/femto/G__NicaFemtoDict.cxx -rmf /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaFemto.rootmap -rml NicaFemto.so  -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/detector -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/compound -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/dataformat/hiddeninfo -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/chains -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/smear_codes -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/ana -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerators -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/imaging -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/source_models -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/corrfunc -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/resolution -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/detector -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/mc -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/cbmdata/_deps/fairroot_v18.6.9/install/include -I/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/cbmdata/_deps/fairsoft_apr22/install/include NicaFemtoBasicAna.h NicaFemtoFreezoutsAna.h NicaFemtoDumpPairAna.h NicaFemtoEventBinsAna.h NicaFemtoDPhiDEta.h NicaFemto1DCF.h NicaFemto3DCF.h NicaFemto3DCFQinv.h NicaFemtoFastCut.h NicaFemtoFreezoutGenerator.h NicaFemtoPairKinematics.h NicaFemtoPair.h NicaFemtoMiniPair.h NicaFemtoSourceModel.h NicaFemtoConst.h NicaFemtoSourceModelExp.h NicaFemtoSourceModelGauss.h NicaFemtoSourceModelGausMt.h NicaCorrFit.h NicaCorrFitFunc.h NicaCorrFitMath1DCF.h NicaCorrFitMath3DCF.h NicaCorrFit1DCF.h NicaCorrFit3DCF.h NicaCorrFitKisiel.h NicaCorrFitWielanek.h NicaCorrFitGUI.h NicaCorrFitHDFunc.h NicaFemto1DCFAnaMap.h NicaFemto1DCFAnaMapMC.h NicaWielanekMapGenerator.h NicaFemto1DCFAnaMapPairsDumped.h NicaCorrFit1DCFSquare.h NicaCorrFit1DCFLorentz.h NicaCorrFit3DCFSquare.h NicaCorrFit3DCFLorentz.h NicaCorrFit1DCFExp.h NicaCorrFit1DCFGauss.h NicaCorrFit1DCFPoly.h NicaCorrFit3DCFGauss.h NicaCorrFit3DCFGauss2.h NicaCorrFit3DCFGaussCross.h NicaCorrFit1DCFCumac.h NicaCorrFit3DCFBowlerSinyukov.h NicaCorrFit3DCFBowlerSinyukovExtra.h NicaFemtoFsiParsed.h NicaFemtoWeightGeneratorLednicky2.h NicaFemtoWeightGenerator.h NicaFemtoWeightGeneratorBasic.h NicaFemtoWeightGeneratorLednicky.h NicaFemtoWeightGeneratorLednickyResidual.h NicaFemtoWeightGeneratorSupressed.h NicaFemtoCorrFunc.h NicaFemtoCorrFuncPt.h NicaFemtoCorrFuncKt.h NicaFemtoCorrFuncAzimuthalKt.h NicaFemtoCorrFuncSimple.h NicaFemtoImaging.h NicaFemtoImaging1D.h NicaFemtoImaging3D.h NicaFemtoSHCF.h NicaFemtoYlm.h NicaCorrFitSHCF.h NicaCorrFitSmear1DCF.h NicaCorrFitDatabase.h NicaCorrFitDumpedPairAna.h NicaCorrFitInfo.h NicaCorrFit3DCFMultiDim.h NicaCorrFitFunctor.h NicaCorrFitPairGeneratorYPt.h NicaCorrFitMapRebin.h NicaFemtoLinkDef.h
