/**
 * \file CbmStudyReport.h
 * \brief Base class for study reports.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#ifndef CBMSTUDYREPORT_H_
#define CBMSTUDYREPORT_H_

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t

#include <string>  // for string
#include <vector>  // for vector

#include "CbmReport.h"  // for CbmReport

class CbmHistManager;

/**
 * \class CbmStudyReport
 * \brief Base class for study reports.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
class CbmStudyReport : public CbmReport {
public:
  /**
    * \brief Constructor.
    */
  CbmStudyReport();

  /**
    * \brief Destructor.
    */
  virtual ~CbmStudyReport();

  /**
    * \brief Main function which creates report data.
    *
    * Non virtual interface pattern is used here.
    * User always creates simulation report using this public non virtual method.
    * In order to configure concrete report generation class
    * user has to implement protected Create() method
    * and getters for the file names.
    *
    * \param[in] histManagers Array of histogram managers for which report is created.
    * \param[in] studyNames Names of studies.
    * \param[in] outputDir name of the output directory.
    */
  void Create(const std::vector<CbmHistManager*>& histManagers,
              const std::vector<std::string>& studyNames,
              const std::string& outputDir);

  /**
    * \brief Main function which creates report data.
    *
    * Same pattern is used here.
    *
    * \param[in] fileNames Array of file names for which report is created.
    * \param[in] studyNames Names of studies.
    * \param[in] outputDir name of the output directory.
    */
  void Create(const std::vector<std::string>& fileNames,
              const std::vector<std::string>& studyNames,
              const std::string& outputDir);

  /**
     * \brief Inherited from CbmReport. Pure abstract function which is called from public Create() function.
     */
  virtual void Create() = 0;

  /* Accessors */
  const std::vector<CbmHistManager*>& HM() const { return fHM; }
  CbmHistManager* HM(Int_t index) const { return fHM[index]; }
  const std::vector<std::string>& GetStudyNames() const { return fStudyNames; }
  const std::string& GetStudyName(Int_t index) const {
    return fStudyNames[index];
  }

private:
  std::vector<CbmHistManager*> fHM;      // Histogram managers for each study
  std::vector<std::string> fStudyNames;  // Names of studies

  ClassDef(CbmStudyReport, 1)
};

#endif /* CBMSTUDYREPORT_H_ */
