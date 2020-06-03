/*  Description: This macro train the ANN for fake
 rejection algorithm.
 
 Author : Semen Lebedev
 E-mail : S.Lebedev@gsi.de
 */
void run_train_ann(
                   Int_t nEvents = 1000)
{
    gRandom->SetSeed(10);
    
	TString mcFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/mc.00000.root";
	TString parFile = "/Users/slebedev/Development/cbm/data/sim/rich/reco/param.00000.root";
	TString recoFile ="/Users/slebedev/Development/cbm/data/sim/rich/reco/reco.00000.root";
	TString outFile ="/Users/slebedev/Development/cbm/data/sim/rich/reco/selectann.00000.root";
    
    // please specify what you want to run
    // select = train ANN for fake rejection algorithm
    // electron = train ANN for electron identification algorithm
    TString option = "electron";
    
    gDebug = 0;
    
    gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
    basiclibs();
    gROOT->LoadMacro("$VMCWORKDIR/macro/rich/cbmlibs.C");
    cbmlibs();
    
    FairRunAna *run = new FairRunAna();
    run->SetInputFile(mcFile);
    run->AddFriend(recoFile);
    run->SetOutputFile(outFile);
    
    FairTask* richAnn;
    if (option == "select") richAnn = new CbmRichTrainAnnSelect();
    if (option == "electron") richAnn = new CbmRichTrainAnnElectrons();
    run->AddTask(richAnn);
    
    // -----  Parameter database   --------------------------------------------
    FairRuntimeDb* rtdb = run->GetRuntimeDb();
    FairParRootFileIo* parIo1 = new FairParRootFileIo();
    parIo1->open(parFile.Data());
    rtdb->setFirstInput(parIo1);
    rtdb->setOutput(parIo1);
    rtdb->saveOutput();
    
    run->Init();
    run->Run(0, nEvents);
}
