/** @file LxTBDefinitions.h
  * @copyright Copyright (C) 2016 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

#ifndef LXTBDEFINITIONS_H
#define LXTBDEFINITIONS_H

//#define LXTB_TIE
#define LXTB_QA

#ifdef LXTB_QA
#define LXTB_EMU_TS
//#define LXTB_DEBUG
#endif  //LXTB_QA

#define scaltype float
#define timetype double

#endif /* LXTBDEFINITIONS_H */
