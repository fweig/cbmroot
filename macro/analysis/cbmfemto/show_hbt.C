/*
 * show_hbt.C
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#ifndef __CLING__
#include <TCanvas.h>
#include <TH1.h>

#include "NicaAnaFile.h"
#include "NicaFemto1DCF.h"
#include "NicaFemtoCorrFuncKt.h"
#endif


void show_hbt()
{
  NicaAnaFile* f             = new NicaAnaFile("test2.root");
  NicaFemtoCorrFuncKt* cf_kt = (NicaFemtoCorrFuncKt*) f->GetMainObject(2);
  TH1* h                     = cf_kt->GetKtNum();
  NicaFemto1DCF* cf          = (NicaFemto1DCF*) cf_kt->GetCF(0);

  TH1* mon = f->GetHistogramPassed(ENicaCutUpdate::kTrackUpdate, 0, 3);

  TCanvas* c = new TCanvas();
  c->Divide(2, 2);
  c->cd(1);
  h->Draw();
  c->cd(2);
  mon->Draw("colz");
  c->cd(3);
  cf->Draw();
}
