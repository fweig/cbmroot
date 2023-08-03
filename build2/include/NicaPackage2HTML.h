/*
 * NicaPackage2HTML.h
 *
 *  Created on: 20-10-2013
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef NICAPACKAGE2HTML_H_
#define NICAPACKAGE2HTML_H_

#include <TObject.h>
#include <TString.h>

#include "NicaStd.h"

class NicaHtmlFile;
class NicaHtmlObject;
class NicaPackage;
class TGraph;
class TObjString;

class TH1;
class TH2;
class TH3;
/**
 * make HTML reports
 */
class NicaPackage2HTML : public TObject {
  friend class NicaAnaFile;

 private:
  TString AddToUrl(TString add_dir, TString url);
  void GetCollectionsNumbers();
  void CreateImagePage(TString filename, TString image_flag, Int_t no,
                       TString path = "");
  TString GetLinkCutMon(TH1 *h1, TH1 *h2, Int_t no, TString path = "");
  TString GetLink1D(TH1 *h1, TH1 *h2, Int_t no, TString path = "");
  TString GetLink2D(TH2 *h1, TH2 *h2, Int_t no, TString path = "");
  TString GetLink3D(TH3 *h1, TH3 *h2, Int_t no, TString path = "");
  TString GetLinkTH(TH1 *h1, Int_t no, TString path = "");
  TString GetLinkTH2(TH2 *h1, Int_t no, TString path = "");
  TString GetLinkTH3(TH3 *h1, Int_t no, TString path = "");
  TString GetLink(TObject *h, Int_t no, TString path = "");
  TString GetLinkGraph(TGraph *h, Int_t no, TString path = "");
  TString GetLinkList(TList *h, Int_t no, TString path = "");
  TString GetLinkVector(TVector2 *vect);
  TString GetLinkString(TObjString *str);
  TString GetLinkToCut(ENicaCutUpdate update, Int_t collection_no, Int_t cut_no,
                       Bool_t fast);
  TString GetLinkToCutMonitor(ENicaCutUpdate udpate, Int_t collection_no,
                              Int_t cut_mon_no);
  void CreatePackageList(NicaHtmlObject &table, NicaPackage *pack,
                         TString prim_style, TString sec_style, TString path,
                         Int_t depth,
                         Option_t *opt = "drawcomment+drawclassname");
  void CreateCutAndMonitorList(NicaHtmlObject &table, TString path);
  void ExportCollections(NicaHtmlObject &object, TString path);
  void CreateCutHTML(NicaHtmlObject &table, ENicaCutUpdate cut_upd,
                     Int_t collection_no);
  void CreateCutLink(NicaHtmlObject &table, ENicaCutUpdate cut_update,
                     NicaPackage *obj, Int_t no, Bool_t fast);
  void CreateCutMonitorLink(NicaHtmlObject &table, ENicaCutUpdate update,
                            NicaPackage *obj, Int_t no, Int_t counter,
                            TString path);
  void CreateCutMonitorHTML(NicaHtmlObject &table, ENicaCutUpdate cut_upd,
                            Int_t collection_no, Int_t &counter, TString path);
  void CreateListTable(NicaHtmlObject &table, TList *list, Int_t no,
                       TString path_data, TString path_url, TString drawClass);
  void DrawContainer(NicaPackage *pack, Int_t tier_no, Int_t tier,
                     Int_t collections_from_no, Int_t collections_to_no,
                     Int_t layer = 0, Int_t line_style = 0,
                     Int_t tier_jump = 1);
  void DrawCircle(Double_t tiers_no, Double_t tier, Double_t collections_no,
                  Double_t collection, Double_t pass_ratio, Double_t radii,
                  Color_t passed_col, Color_t failed_col, TString label);
  void DrawLine(Double_t tiers_no, Double_t tier_from, Double_t tier_to,
                Double_t collections_from_no, Double_t collections_to_no,
                Double_t collection_from, Double_t collection_to,
                Double_t width, Int_t style, Color_t color);
  TString GetGroupListName(ENicaCutUpdate update) const;
  NicaHtmlFile *fHTML;
  TString fDir;
  Int_t fObjectCounter, fDynamicTableCounter, fTaskTableCounter;
  Int_t fCutContainerPosition, fPackageID;
  Int_t fCollectionsNo[4];
  Double_t fBasicRadius, fWindowWidth, fWIndowHeight;
  NicaPackage *fCurrentCutContainer;
  Int_t fListDeep;
  Int_t fSoftVer;
  void TryExtractAnaResult(NicaHtmlObject &table, NicaPackage *pack,
                           TString path);

 protected:
  /**
   * constructor used by package extractor
   * @param ana_pack package with analysis
   * @param global_metadata package with global metadata
   * @param dir place where report will be created
   * @param task_id id of task
   */
  NicaPackage2HTML(NicaPackage *ana_pack, NicaPackage *global_metadata,
                   const TString dir, Int_t task_id);

 public:
  /**
   * extract entire file to HTML report
   * @param filename name of file
   * @param dir place to create report
   */
  NicaPackage2HTML(TString filename, const TString dir);
  /**
   * extract only single package into HTML report
   * @param filename name of file
   * @param dir place where report will be extracted
   * @param packname name of package to extraction
   */
  NicaPackage2HTML(TString filename, const TString dir, TString packname);
  /**
   * export single package (even if it is not package with analysis
   * @param pack package for extraction
   * @param dir place where report is created
   * @param main_dir - if true separate coptions of html-scripts is created in
   * dir
   */
  NicaPackage2HTML(NicaPackage *pack, const TString dir,
                   Bool_t mainDir = kTRUE);
  virtual ~NicaPackage2HTML();
  ClassDef(NicaPackage2HTML, 1)
};

#endif /* NICAPACKAGE2HTML_H_ */
