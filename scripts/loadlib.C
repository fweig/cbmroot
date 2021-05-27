Int_t loadlib(TString libname)
{
  Int_t retval = gSystem->Load(libname);
  return retval;
}
