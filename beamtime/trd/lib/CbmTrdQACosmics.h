/*
 * CbmTrdQACosmics.h
 *
 *  Created on: Apr 22, 2017
 *      Author: philipp
 */

#ifndef FLES_READER_TASKS_CBMTRQACOSMICS_H_
#define FLES_READER_TASKS_CBMTRQACOSMICS_H_

#include <CbmTrdQABase.h>
#include "CbmSpadicRawMessage.h"


class CbmTrdQACosmics: public CbmTrdQABase
{
  public:
    CbmTrdQACosmics ();
    void CreateHistograms ();
    void Exec (Option_t*);
    virtual
    ~CbmTrdQACosmics ();

    ClassDef(CbmTrdQACosmics,1);

};

#endif /* FLES_READER_TASKS_CBMTRQACOSMICS_H_ */
