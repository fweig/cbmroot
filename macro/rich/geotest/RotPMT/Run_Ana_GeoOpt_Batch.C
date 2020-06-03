void Run_Ana_GeoOpt_Batch(Int_t nEvents = 10)
{
  TTree::SetMaxTreeSize(90000000000);
  gRandom->SetSeed(10);
 float PMTrotX=20, PMTrotY=10;
  int PMTtransY=180, PMTtransZ=80;
  float ThetaMin=250., ThetaMax=2500.;//devide by 100 later 

  float PhiMin=90., PhiMax=180.;
  int GeoCase=2, DimCase=2;
  float EnlargedPMTWidth=2., EnlargedPMTHight=4.;
 

  int PtNotP=1;
  float MomMin=0., MomMax=400;//devide by 100 later 

  float RotMir=-10;
  int extendedmir=0;
  int OldCode=0;
  int DefaultDims=0;
  int DefaultDims_LargePMT=0;

  TString ExtraText=".";//
  ExtraText="_Updated.";//
  if(DimCase ==0){ExtraText="";}
  
  TString script = TString(gSystem->Getenv("SCRIPT"));
  if (script == "yes"){
    cout<<" ----------------- running with script --------------------"<<endl;
   nEvents=TString(gSystem->Getenv("N_EVS")).Atof();
    PMTrotX=TString(gSystem->Getenv("PMT_ROTX")).Atof();
    PMTrotY=TString(gSystem->Getenv("PMT_ROTY")).Atof();
    PMTtransY=TString(gSystem->Getenv("PMT_TRAY")).Atof();
    PMTtransZ=TString(gSystem->Getenv("PMT_TRAZ")).Atof();

    ThetaMin=TString(gSystem->Getenv("THETAMIN")).Atof();
    ThetaMax=TString(gSystem->Getenv("THETAMAX")).Atof();
    PhiMin=TString(gSystem->Getenv("PHIMIN")).Atof();
    PhiMax=TString(gSystem->Getenv("PHIMAX")).Atof();

    GeoCase=TString(gSystem->Getenv("GEO_CASE")).Atof();
    DimCase=TString(gSystem->Getenv("DIM_CASE")).Atof();
    EnlargedPMTWidth=TString(gSystem->Getenv("ENL_PMTWIDTH")).Atof();
    EnlargedPMTHight=TString(gSystem->Getenv("ENL_PMTHIGHT")).Atof();

    PtNotP=TString(gSystem->Getenv("PT_NOT_P")).Atof();
    MomMin=TString(gSystem->Getenv("MOM_MIN")).Atof();
    MomMax=TString(gSystem->Getenv("MOM_MAX")).Atof();
    /////////

    RotMir=TString(gSystem->Getenv("ROTMIR")).Atof();
    extendedmir=TString(gSystem->Getenv("EXTENDEDMIR")).Atof();

    OldCode=TString(gSystem->Getenv("OLDCODE")).Atof();
    DefaultDims=TString(gSystem->Getenv("DEFAULDIMS")).Atof();
    DefaultDims_LargePMT=TString(gSystem->Getenv("DEFAULDIMSLPMT")).Atof();
     ExtraText=TString(gSystem->Getenv("EXTRATEXT"));
  }  
 
  TString outDir=GetOutDir(GeoCase);
  TString GeoText=GetGeoText(GeoCase);
  TString RotMirText=GetMirText(RotMir, extendedmir);
  cout<<"MirText = "<<RotMirText<<endl;
  TString PMTRotText=GetPMTRotText(PMTrotX, PMTrotY);
  TString PMTTransText=GetPMTTransText(PMTtransY, PMTtransZ);
 TString MomText=GetMomText(OldCode, PtNotP,MomMin,MomMax);
  cout<<MomText<<endl;  
  TString PhiText=GetPhiText(OldCode,PhiMin, PhiMax);
  TString ThetaText=GetThetaText(OldCode,ThetaMin,ThetaMax);
  cout<<ThetaText<<endl;
  TString DimentionText=GetDimentionText(DimCase, EnlargedPMTWidth, EnlargedPMTHight);
  cout<<DimentionText<<endl;
  
  
  TString NamingText;
  if(OldCode==1){
    NamingText=GeoText+"_"+RotMirText+"_"+PMTRotText+"_"+PMTTransText+"_"+MomText+"_"+ThetaText+DimentionText+ExtraText+"root";
  }else{
    NamingText=GeoText+"_"+RotMirText+"_"+PMTRotText+"_"+PMTTransText+"_"+MomText+"_"+ThetaText+"_"+PhiText+DimentionText+ExtraText+"root";
  }
 

  TString ParFile = outDir + "Parameters_"+NamingText;//
  TString SimFile = outDir + "Sim_"+NamingText;
  TString RecFile = outDir + "Rec_"+NamingText;
  TString AnaFile = outDir + "Ana_"+NamingText;
  cout<<"ParFile: "<<ParFile<<endl;
  cout<<"AnaFile: "<<AnaFile<<endl;
  //return;
  gDebug = 0;
  TStopwatch timer;
  timer.Start();
  cout<<" going to load libraries"<<endl;
  //  gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
  //  loadlibs();
  cout<<" got libraries"<<endl;
  
  FairRunAna *run= new FairRunAna();
  run->SetInputFile(SimFile);
  run->AddFriend(RecFile);
  run->SetOutputFile(AnaFile);
  
  CbmRichGeoOpt* richGeoOpt = new CbmRichGeoOpt();
  run->AddTask(richGeoOpt);
  
  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  FairParRootFileIo* parIo1 = new FairParRootFileIo();
  FairParAsciiFileIo* parIo2 = new FairParAsciiFileIo();
  parIo1->open(ParFile.Data());
  //parIo2->open(parFileList, "in");
  rtdb->setFirstInput(parIo1);
  //rtdb->setSecondInput(parIo2);
  rtdb->setOutput(parIo1);
  rtdb->saveOutput();
  
  
  run->Init();
  cout << "Starting run" << endl;
  run->Run(0,nEvents);
  
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished successfully." << endl;
  cout << "Output file is "    << AnaFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
}

////////////////////////////////////////////
TString GetMomText(int OldCode=1, int PtNotP, float MomMin, float MomMax){
  TString Pstring="P"; if(PtNotP==1){Pstring="Pt";}
  char Ptxt[256];
  if(OldCode==1){
    sprintf(Ptxt,"%s%dto%d",Pstring.Data(),MomMin/100.,MomMax/100.);
  }else{
    
    int MomMinMod100=int(MomMin) % 100;
    int MomMaxMod100=int(MomMax) % 100;
    float IntegerMomMin100=(MomMin-MomMinMod100)/100.;
    float IntegerMomMax100=(MomMax-MomMaxMod100)/100.;
    
    int MomMinMod10=int(MomMinMod100) % 10;
    int MomMaxMod10=int(MomMaxMod100) % 10;
    float IntegerMomMin10=(MomMinMod100-MomMinMod10)/10.;
    float IntegerMomMax10=(MomMaxMod100-MomMaxMod10)/10.;
    cout<<"   #####################################  "<<endl;
    cout<<"   #### Momentum  "<<endl;

    cout<<IntegerMomMin100<<"."<<MomMinMod100<<",  "<<IntegerMomMax100<<"."<<MomMaxMod100<<endl;
    cout<<IntegerMomMin10<<"."<<MomMinMod10<<",  "<<IntegerMomMax10<<"."<<MomMaxMod10<<endl;

    cout<<IntegerMomMin100<<"."<<IntegerMomMin10<<",  "<<IntegerMomMax100<<"."<<IntegerMomMax10<<endl;
    cout<<"   #####################################  "<<endl;
        
    char MinMomTxt[256];char MaxMomTxt[256]; 
    sprintf(Ptxt,"%s%dpoint%d_to_%dpoint%d",Pstring.Data(),IntegerMomMin100,IntegerMomMin10,IntegerMomMax100,IntegerMomMax10);
    //cout<<Ptxt<<endl;
  }
  stringstream ss; 
  ss<<Ptxt;
  return ss.str();
}

////////////////////////////////////////////
TString GetGeoText(int GeoCase){
  //GeoCase=-2 ==> old geometry with rich_v08a.geo (RICH starts at 1600, Mirror tilt -1)
  //GeoCase=-1 ==> old geometry with rich_v14a.gdml (RICH starts at 1800, Mirror tilt -1)
  //GeoCase=0 ==> old geometry with *.geo (own creation)(RICH starts at 1600, Mirror tilt -1)
  //GeoCase=1 ==> gdml-geo: RICH starts at 1800, Mirror tilt -1 or 10, 
  //                        mirror does NOT cover full acceptance)
  //GeoCase=2 ==> gdml-geo: RICH starts at 1800, Mirror tilt -1 or 10, 
  //                        mirror does cover full acceptance)

  if(GeoCase==-2){return "RichGeo_v08a";}
  if(GeoCase==-1){return "RichGeo_v14a";}
  if(GeoCase==0){return "RichGeo_ascii";}
  if(GeoCase==1){return "RichGeo_OldGdml";}
  if(GeoCase==2){return "RichGeo_NewGdml";}
}
////////////////////////////////////////////
TString GetOutDir(int GeoCase){
  return "/nas/Tariq/GeoOpt/";
  // return "/hera/cbm/users/tariq/MomScan/";
  return "/nas/Tariq/Test/";
//   return "/nas/Tariq/GeoOpt/";
  return "/data/GeoOpt/Test2/";
//   return "/data/GeoOpt/OptiPMTSize/";
  //return "/data/GeoOpt/";
  //return "/data/GeoOpt/RotPMT/";
  return "/hera/cbm/users/tariq/GeoOptRootFiles/OptimisedGeo/";
  return "/hera/cbm/users/tariq/GeoOptRootFiles/";
  // if(GeoCase<=0){return "/data/GeoOpt/RotPMT/OlderGeo/";}
  // if(GeoCase==1){return "/data/GeoOpt/RotPMT/OldGeo/";}
  // if(GeoCase==2){return "/data/GeoOpt/RotPMT/NewGeo/";}
}
////////////////////////////////////////////
TString GetMirText(int RotMir, bool extend){
  char RotMir_txt[256];
  if(RotMir<0){sprintf( RotMir_txt,"RotMir_m%d",RotMir*-1);}
  else{sprintf(RotMir_txt,"RotMir_p%d",RotMir);}

  stringstream ss; 
  ss<<RotMir_txt;
  if(extend){ss<<"_Extended";}
  return ss.str();
}
////////////////////////////////////////////////////////
TString  GetPMTRotText(float PMTrotX, float PMTrotY){
  int ShiftXmod10=(int(PMTrotX*10)) % 10;
  int ShiftYmod10=(int(PMTrotY*10)) % 10;
  float IntegerXValue=PMTrotX-(float (ShiftXmod10))/10.;
  float IntegerYValue=PMTrotY-(float (ShiftYmod10))/10.;
  
  char ShiftXTxt[256]; char ShiftYTxt[256];  
  sprintf(ShiftXTxt,"Xpos%dpoint%d",IntegerXValue,ShiftXmod10);
  sprintf(ShiftYTxt,"Ypos%dpoint%d",IntegerYValue,ShiftYmod10);
  if(PMTrotY<0){sprintf(ShiftYTxt,"Yneg%dpoint%d",-1.*IntegerYValue,-1.*ShiftYmod10);}
  
  stringstream ss; 
  ss<<"RotPMT_"<<ShiftXTxt<<"_"<<ShiftYTxt;
  return ss.str();
}
///////////////////////////////////////////////
TString  GetPMTTransText(int PMTTransY, int PMTTransZ){
  char ZTransText[256]; char YTransText[256];  
  if(PMTTransY < 0) {sprintf( YTransText,"Y_m%d",-1*PMTTransY);}
  else{sprintf( YTransText,"Y_p%d",PMTTransY);}
  if(PMTTransZ < 0) {sprintf( ZTransText,"Z_m%d",-1*PMTTransZ);}
  else{sprintf( ZTransText,"Z_p%d",PMTTransZ);}
  stringstream ss; 
  ss<<"TransPMT_"<<YTransText<<"_"<<ZTransText;
  return ss.str();
}
///////////////////////////////////////////////
TString  GetDimentionText(int DimCase, int EnlargedPMTWidth, int EnlargedPMTHight){
  if(DimCase ==0){return ".";}
  else if(DimCase ==1){return "_DefaultRichDims";}
  else if(DimCase ==2){return "_DefaultRichDims_LargePMT";}
  else if(DimCase ==3){
    float PMTWidth=1000. +EnlargedPMTWidth,  PMTHight=600. +EnlargedPMTHight; 
    char PMTDimsText[256];
    sprintf( PMTDimsText,"_PMTW%d_H%d",PMTWidth, PMTHight);

    stringstream ss; 
    ss<<PMTDimsText;
    return ss.str();
  }
}

////////////////////////////////////////////////////////
TString GetThetaText(int OldCode=1,  float ThetaMin=250., float ThetaMax=2500.){
  TString Tstring="Theta"; 
  char THtxt[256];
  if(OldCode==1){
    sprintf(THtxt,"%s_%d",Tstring.Data(),ThetaMax/100.);
  }else{  
    int ThetaMinMod100=int(ThetaMin) % 100;
    int ThetaMaxMod100=int(ThetaMax) % 100;
    float IntegerThetaMin100=(ThetaMin-ThetaMinMod100)/100.;
    float IntegerThetaMax100=(ThetaMax-ThetaMaxMod100)/100.;
    

    int ThetaMinMod10=int(ThetaMinMod100) % 10;
    int ThetaMaxMod10=int(ThetaMaxMod100) % 10;
    float IntegerThetaMin10=(ThetaMinMod100-ThetaMinMod10)/10.;
    float IntegerThetaMax10=(ThetaMaxMod100-ThetaMaxMod10)/10.;
    cout<<"   #####################################  "<<endl;
    cout<<"   #### Theta  "<<endl;

    cout<<IntegerThetaMin100<<"."<<ThetaMinMod100<<",  "<<IntegerThetaMax100<<"."<<ThetaMaxMod100<<endl;
    cout<<IntegerThetaMin10<<"."<<ThetaMinMod10<<",  "<<IntegerThetaMax10<<"."<<ThetaMaxMod10<<endl;

    cout<<IntegerThetaMin100<<"."<<IntegerThetaMin10<<",  "<<IntegerThetaMax100<<"."<<IntegerThetaMax10<<endl;
    cout<<"   #####################################  "<<endl;

    char MinThetaTxt[256];char MaxThetaTxt[256]; 
    sprintf(THtxt,"%s%dpoint%d_to_%dpoint%d",Tstring.Data(),IntegerThetaMin100,IntegerThetaMin10,IntegerThetaMax100,IntegerThetaMax10);
  }
  stringstream ss; 
  ss<<THtxt;
  return ss.str();
}
////////////////////////////////////////////////////////
TString GetPhiText(int OldCode=1, float PhiMin, float PhiMax){
  char PHtxt[256];
  if(OldCode==1){return "";}
  else{sprintf(PHtxt,"Phi_%d_to_%d",PhiMin, PhiMax);}
  stringstream ss; 
  ss<<PHtxt;
  return ss.str();
}
