/*
 * CbmTrdDigiAnalysis.h
 *
 *  Created on: Apr 22, 2017
 *      Author: philipp
 */

#ifndef FLES_READER_TASKS_CBMTRDDIGIANALYSIS_H_
#define FLES_READER_TASKS_CBMTRDDIGIANALYSIS_H_

#include <CbmTrdQABase.h>
#include "CbmTrdDigi.h"


class CbmTrdDigiAnalysis : public CbmTrdQABase
{
  public:
    CbmTrdDigiAnalysis ();
    void CreateHistograms ();
    void Exec (Option_t*);
    virtual
    ~CbmTrdDigiAnalysis ();

    ClassDef(CbmTrdDigiAnalysis,1);

};

#endif /* FLES_READER_TASKS_CBMTRDDIGIANALYSIS_H_ */
