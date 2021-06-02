/** @file CbmLitTypes.h
  * @copyright Copyright (C) 2008-2013 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev [committer] **/

/**
 * \file CbmLitTypes.h
 * \author A.Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 * \brief Typedefs for data structures used in littrack.
 */

#ifndef CBMLITTYPES_H_
#define CBMLITTYPES_H_

#include "data/CbmLitFitNode.h"

#include <vector>
using std::pair;
using std::vector;
class CbmLitHit;
class CbmLitTrack;
class CbmLitTofTrack;

typedef vector<CbmLitHit*> HitPtrVector;
typedef vector<CbmLitHit*>::iterator HitPtrIterator;
typedef pair<HitPtrIterator, HitPtrIterator> HitPtrIteratorPair;
typedef vector<CbmLitHit*>::const_iterator HitPtrConstIterator;

//typedef vector<const CbmLitHit*> HitCnstPtrVector;
//typedef vector<const CbmLitHit*>::iterator HitCnstPtrIterator;
//typedef pair<HitCnstPtrIterator, HitCnstPtrIterator> HitCnstPtrIteratorPair;

typedef vector<CbmLitTrack*>::iterator TrackPtrIterator;
typedef vector<CbmLitTrack*> TrackPtrVector;

typedef vector<CbmLitTofTrack*>::iterator TofTrackPtrIterator;
typedef vector<CbmLitTofTrack*> TofTrackPtrVector;

typedef vector<CbmLitFitNode> FitNodeVector;
typedef vector<CbmLitFitNode>::iterator FitNodeIterator;

#endif /* CBMLITTYPES_H_ */
