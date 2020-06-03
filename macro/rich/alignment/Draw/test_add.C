void test_add()
{
	TH2D *h1 = new TH2D("h1","h1",100,0.,20.,100,0.,20.);
	TH2D *h2 = new TH2D("h2","h2",100,0.,20.,100,0.,20.);
	TF2 *xyg = new TF2("xyg","xygaus",0,10,0,10);

	TCanvas *can = new TCanvas("can", "can", 10, 10, 1510, 510);
	can->Divide(3,1);

	xyg->SetParameters(1,5,2,5,2);  //amplitude, meanx,sigmax,meany,sigmay
	h1->FillRandom("xyg");
	can->cd(1);
	h1->Draw();

	xyg->SetParameters(1,10,1,10,1);  //amplitude, meanx,sigmax,meany,sigmay
	h2->FillRandom("xyg");
	can->cd(2);
	h2->Draw();

	TH2D *h3 = new TH2D("h3","h3",100,0.,20.,100,0.,20.);
	h3->Add(h1, h2, 1., -1.);
	can->cd(3);
	h3->Draw();
}
