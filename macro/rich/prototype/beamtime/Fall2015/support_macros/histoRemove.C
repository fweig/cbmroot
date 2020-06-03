void histoRemove(TString filename, TString histoname)
{
	TFile f(filename, "UPDATE");
	f.Delete(histoname);
	f.Close();
}
