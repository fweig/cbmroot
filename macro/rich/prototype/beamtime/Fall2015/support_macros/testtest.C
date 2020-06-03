void testtest()
{
	TString tdcIDs[16];
	tdcIDs[0] = "10";	tdcIDs[1] = "11";	tdcIDs[2] = "12";	tdcIDs[3] = "13";
	tdcIDs[4] = "20";	tdcIDs[5] = "21";	tdcIDs[6] = "22";	tdcIDs[7] = "23";
	tdcIDs[8] = "50";	tdcIDs[9] = "51";	tdcIDs[10] = "52";	tdcIDs[11] = "53";
	tdcIDs[12] = "60";	tdcIDs[13] = "61";	tdcIDs[14] = "62";	tdcIDs[15] = "63";

	TString histoName;

	for (Int_t tdc1J=0; tdc1J<=15; tdc1J++) {
		Int_t tdc2J = tdc1J;
		for (Int_t ch1=0; ch1<=15; ch1++) {
			for (Int_t ch2=ch1; ch2<=15; ch2++) {
				histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcIDs[tdc1J].Data(), ch1, tdcIDs[tdc2J].Data(), ch2);
				printf ("%s\n", histoName.Data());
			}
		}
		for (tdc2J=tdc1J+1; tdc2J<=15; tdc2J++) {
			for (Int_t ch1=0; ch1<=15; ch1++) {
				for (Int_t ch2=0; ch2<=15; ch2++) {
					histoName.Form("LeadingEdgeDiff_TDC%s_ch%d_TDC%s_ch%d", tdcIDs[tdc1J].Data(), ch1, tdcIDs[tdc2J].Data(), ch2);
					printf ("%s\n", histoName.Data());
				}
			}
		}
	}
}
