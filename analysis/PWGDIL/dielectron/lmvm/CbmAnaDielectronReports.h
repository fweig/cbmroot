/**
 * \file CbmAnaDielectronReports.h
 * \brief Main class wrapper for report generation.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2012
 */
#ifndef CBM_ANA_DIELECTRON_REPORTS
#define CBM_ANA_DIELECTRON_REPORTS

#include <string>
#include <vector>
#include "TObject.h"

/**
 * \class CbmAnaDielectronReports
 * \brief Main class wrapper for report generation.
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2012
 *
 */
class CbmAnaDielectronReports : public TObject
{
public:
   /**
    * \brief Constructor.
    */
   CbmAnaDielectronReports();

   /**
    * \brief Destructor.
    */
   virtual ~CbmAnaDielectronReports();

   void CreateStudyReport(
         const std::string& title,
         const std::vector<std::string>& fileNames,
         const std::vector<std::string>& studyNames,
         const std::string& outputDir);

   ClassDef(CbmAnaDielectronReports, 1);

};

#endif
