/** @file run_qa_to_html.C
  * @copyright Copyright (C) 2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * run_qa_to_html.C
 * simple macro for export of QA into html report
 * note: due to some security settings in modern browsers, the histograms are not working on local data, to browse them upload
 * report to server, you can also use command "python3 -m http.server"
 *  Created on: 11 mar 2021
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

void run_qa_to_html(TString rootFile = "cbm_qa.root", TString outDir = "html")
{
  TFile* f    = new TFile(rootFile);
  TList* list = f->GetListOfKeys();
  gSystem->mkdir(outDir);
  NicaHtmlFile* html = new NicaHtmlFile(Form("%s/index.html", outDir.Data()));
  NicaHtmlTable table("main_table", "nicatable", "");
  NicaHtmlRow row1("", "red_", "");  //id/class/style
  NicaHtmlCellCol cell1("HtmlReport", 2);
  row1.AddContent(cell1);
  table.AddContent(row1);
  Int_t histo_count = 0;
  for (int i = 0; i < list->GetEntries(); i++) {
    TKey* key    = (TKey*) list->At(i);
    TObject* obj = f->Get(key->GetName());
    if (obj->InheritsFrom("TDirectory")) {
      TDirectory* dir = (TDirectory*) obj;
      TList* dirList  = dir->GetListOfKeys();
      NicaHtmlRow dirRow("", "green_", "");
      dirRow.AddContent(NicaHtmlCell(dir->GetName()));
      NicaHtmlCell cell2;
      cell2.SetStringContent(NicaHtmlCore::GetHideButtonRow(Form("list_%i", i), "Show/Hide"));
      dirRow.AddContent(cell2);
      table.AddContent(dirRow);
      for (int j = 0; j < dirList->GetEntries(); j++) {
        TKey* key2    = (TKey*) dirList->At(j);
        TObject* hobj = dir->Get(key2->GetName());
        if (hobj->InheritsFrom("TH1")) {
          TH1* histo = (TH1*) hobj;
          NicaHtmlRow rowHist("", Form("list_%i light_blue", i), "display:none");
          NicaHtmlCell cellName(key2->GetName());
          cellName.SetClass("light_blue");
          rowHist.AddContent(cellName);
          NicaHtmlCell cellContent;
          cellContent.SetStringContent(NicaHtmlCore::GetLinkToHistogram(histo, histo_count++, outDir));
          rowHist.AddContent(cellContent);
          table.AddContent(rowHist);
        }
      }
    }
  }
  f->Close();
  html->AddContent(table);
  html->Save();
  delete html;
}
