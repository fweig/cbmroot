/* Copyright (C) 2009-2013 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file LitTypes.h
 * \brief Header files for SSE operations.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2009
 */

#ifndef LITTYPES_H_
#define LITTYPES_H_

#ifdef HAVE_SSE
#include "vectors/P4_F32vec4.h"
#else
#include "vectors/PSEUDO_F32vec1.h"
#error LitNoSseFound
#endif

#endif /* LITTYPES_H_ */
