#ifndef ANALYSIS_TREE_PSDMODULESCONVERTER_H_
#define ANALYSIS_TREE_PSDMODULESCONVERTER_H_

#include "AnalysisTree/Detector.hpp"
#include "CbmConverterTask.h"

class TClonesArray;

class CbmPsdModulesConverter final : public CbmConverterTask {
public:
  explicit CbmPsdModulesConverter(std::string out_branch_name,
                                  std::string match_to = "")
    : CbmConverterTask(std::move(out_branch_name), std::move(match_to)) {
    out_branch_ = "PsdModules";
    in_branches_.emplace_back("PsdHits");
  };

  ~CbmPsdModulesConverter() final;

  void Init(std::map<std::string, void*>&) final;
  void Exec() final;
  void Finish() final;

private:
  AnalysisTree::ModuleDetector* psd_modules_ {nullptr};
  TClonesArray* cbm_psd_hits_ {nullptr};

  ClassDef(CbmPsdModulesConverter, 1)
};

#endif  // ANALYSIS_TREE_PSDMODULESCONVERTER_H_
