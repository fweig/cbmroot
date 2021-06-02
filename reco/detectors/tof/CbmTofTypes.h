/** @file CbmTofTypes.h
  * @copyright Copyright (C) 2008-2016 PI-UHd, GSI
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors A.Lebedev, Norbert Herrmann [committer], Florian Uhlig **/

/**
 * \file CbmTOFTypes.h
 * \author A.Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 * \brief Typedefs for data structures used in littrack.
 */

#ifndef CBMTOFTYPES_H_
#define CBMTOFTYPES_H_

#include <vector>

class CbmTofHit;
class CbmTofTrack;

typedef std::vector<CbmTofHit*> HitPtrVector;
typedef std::vector<CbmTofHit*>::iterator HitPtrIterator;
typedef std::pair<HitPtrIterator, HitPtrIterator> HitPtrIteratorPair;
typedef std::vector<CbmTofHit*>::const_iterator HitPtrConstIterator;

typedef std::vector<CbmTofTrack*>::iterator TrackPtrIterator;
typedef std::vector<CbmTofTrack*> TrackPtrVector;

#endif /* CBMLITTYPES_H_ */
