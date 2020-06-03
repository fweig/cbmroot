/*
 * CbmTrdsimpleClusterAnalysis.h
 *
 *  Created on: Sep 4, 2017
 *      Author: philipp
 */

#ifndef FLES_READER_TASKS_CBMTRDSIMPLECLUSTERANALYSIS_H_
#define FLES_READER_TASKS_CBMTRDSIMPLECLUSTERANALYSIS_H_

#include <CbmTrdQABase.h>


class CbmTrdSimpleClusterAnalysis : public CbmTrdQABase
{
public:
  CbmTrdSimpleClusterAnalysis ();
  virtual
  ~CbmTrdSimpleClusterAnalysis ();

  virtual void CreateHistograms();
  virtual void Exec(Option_t* opt);

  std::vector<TH1*> fSpectra;

  ClassDef(CbmTrdSimpleClusterAnalysis,1);
};

#endif /* FLES_READER_TASKS_CBMTRDSIMPLECLUSTERANALYSIS_H_ */
