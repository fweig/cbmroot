
set(NICA_CBM
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(KF_HEADERS
	${CBMROOT_SOURCE_DIR}/L1
	${CBMROOT_SOURCE_DIR}/L1/L1Algo
	${CBMROOT_SOURCE_DIR}/L1/ParticleFinder
	${CLHEP_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	${KFParticle_INCLUDE_DIR}
	${CBMROOT_SOURCE_DIR}/KF
	${CBMROOT_SOURCE_DIR}/KF/Interface
	${CBMROOT_SOURCE_DIR}/external/KFParticle/KFParticle
#	${CBMROOT_SOURCE_DIR}/analysis/nica_cbm/helpers
)
set(NICA_CBM_FORMAT
	${CBMBASE_DIR}
	${CBMDATA_DIR}
	${CBMDATA_DIR}/global
	${CBMDATA_DIR}/sts
	${CBMDATA_DIR}/tof
	${CBMDATA_DIR}/trd
	${CBMDATA_DIR}/rich
	${CBMDATA_DIR}/much
	${CBMDATA_DIR}/mvd
	${NICA_CBM}/format
	${NICA_CBM}/format/combined
	${NICA_CBM}/format/anatree
	${NICA_CBM}/format/anatree/reco
	${NICA_CBM}/format/anatree/mc
	${NICA_CBM}/format/anatree/femto
	${NICA_CBM}/format/unigen
)

set(NICAFEMTO_CUTS
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/eventcuts
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/trackcuts
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/trackcuts/detector
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/trackcuts/kinematics
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/trackcuts/mc
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/trackcuts/resolution
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/twotrackcuts
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/twotrackcuts/detector
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/twotrackcuts/kinematics
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/twotrackcuts/mc
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/cuts/cutmonitors
)

set(NICAFEMTO_FEATURES
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/features
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/features/hist
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/features/io
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/features/io/html
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/features/std
)

set(NICAFEMTO_DATA
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/dataformat
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/dataformat/formats/fair
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/dataformat/formats/fairextended
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/dataformat/formats/unigen
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/dataformat/detector
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/dataformat/compound
)

set(NICAFEMTO_ANA
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/base
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/base/chains
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/base/on_the_fly
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/base/on_the_fly/smear_codes
)
set(NICAFEMTO_HBT
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/ana
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/base
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/weights
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/corrfit
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/corrfit/mapgenerators
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/corrfit/fittingfunctions
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/imaging
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/base/source_models
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto/base/corrfunc
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/analysis/femto
)
set(NICAFEMTO_GEN
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/nicagenerators/readers
	${CBMROOT_SOURCE_DIR}/external/NicaFemto/nicagenerators

)

add_subdirectory(format)
add_subdirectory(cuts)
add_subdirectory(helpers)
