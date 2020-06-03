void Run_Sim_GeoOpt_Batch(Int_t nEvents = 1)
{

  float PMTrotX=20, PMTrotY=10;
  int PMTtransY=-40, PMTtransZ=80;
  float ThetaMin=250., ThetaMax=2500.;//devide by 100 later 
  float PhiMin=90., PhiMax=180.;
  int GeoCase=2, DimCase=3;
  float EnlargedPMTWidth=60., EnlargedPMTHight=36.;
 

  int PtNotP=1;
  float MomMin=0., MomMax=400;//devide by 100 later 

  float RotMir=-10;
  int extendedmir=1;
  int OldCode=0;
  int DefaultDims=0;
  int DefaultDims_LargePMT=0;

  bool StoreTraj=0;
  

  TString ExtraText=".";//
  //ExtraText="_Updated.";//
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

  cout<<"PMTrotX="<< PMTrotX<<"  PMTrotY="<< PMTrotY<<"  PMTtransY="<<
    PMTtransY<<"  PMTtransZ="<< PMTtransZ<<"  ThetaMin="<<
    ThetaMin<<"  ThetaMax="<< ThetaMax<<"  PhiMin="<<
    PhiMin<<"  PhiMax="<< PhiMax<<"  GeoCase="<<
    GeoCase<<"  DimCase="<< DimCase<<"  EnlargedPMTWidth="<<
    EnlargedPMTWidth<<"  EnlargedPMTHight="<< EnlargedPMTHight<<"  PtNotP="<<
    PtNotP<<"  MomMin="<<
    MomMin<<"  MomMax="<< MomMax<<"  rotmir="<<
    RotMir<<"  extendedmir="<<
    extendedmir<<"  DefaultDims="<<
    DefaultDims<<"  DefaultDims_LargePMT="<<
    DefaultDims_LargePMT<<"  ="<<endl;
  //return;
  // if(EnlargedPMTWidth == 0 && EnlargedPMTHight==0){DefaultDims=1; DefaultDims_LargePMT=0;}
  
  TTree::SetMaxTreeSize(90000000000);
  //******************************
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
  cout<<"DimentionText "<<DimentionText<<endl;

  TString richGeom=GetRICH_GeoFile( RotMirText, PMTRotText, PMTTransText, GeoCase, DimentionText, ExtraText);
  cout<<"rich geo = "<<richGeom<<endl;
  TString pipeGeom=GetPipe_GeoFile(GeoCase);
  pipeGeom="";
  //  return;
  TString NamingText;
  if(OldCode==1){
    NamingText=GeoText+"_"+RotMirText+"_"+PMTRotText+"_"+PMTTransText+"_"+MomText+"_"+ThetaText+DimentionText+ExtraText+"root";
  }else{
    NamingText=GeoText+"_"+RotMirText+"_"+PMTRotText+"_"+PMTTransText+"_"+MomText+"_"+ThetaText+"_"+PhiText+DimentionText+ExtraText+"root";
  }
 
  TString ParFile = outDir + "Parameters_"+NamingText;//
  TString SimFile = outDir + "Sim_"+NamingText;
  TString OutPutGeoFile = outDir + "OutPutGeo_"+NamingText;
  // gSystem->Exec( ("rm "+ParFile).Data() );
  // gSystem->Exec( ("rm "+SimFile).Data() );
  // gSystem->Exec( ("rm "+OutPutGeoFile).Data() );

  cout<<"par: "<<ParFile<<endl;
  cout<<"sim: "<<SimFile<<endl;
  cout<<"++++++++++++++++++++++++++++++++++++++++++++"<<endl; 
  //////////////////////////////////////////////
  ///////////// Important change of variables
  ThetaMin/=100.; ThetaMax/=100.; MomMin/=100.; MomMax/=100.;
  cout<<"ThetaMin ="<<ThetaMin<<"  ThetaMax="<<ThetaMax<<"  MomMin="<<MomMin <<"  MomMax="<<MomMax<<endl;
  //////////////////////////////////////////////
  //////////////////////////////////////////////

  //return;
  TString caveGeom = "cave.geo";
  TString magnetGeom = "magnet/magnet_v12b.geo.root";
  TString magnetGeom = "";
  TString stsGeom = "";
  TString fieldMap = "field_v12a";
  Double_t fieldZ = 50.; // field center z position
  Double_t fieldScale =  1.0; // field scaling factor
  
  ///////////////////////////////////////////////////
  gDebug = 0;
  TStopwatch timer;
  timer.Start();
  cout<<" going to load libraries"<<endl;
  
  gROOT->LoadMacro("$VMCWORKDIR/macro/littrack/loadlibs.C");
  loadlibs();
  cout<<" got libraries"<<endl;
  
  FairRunSim* fRun = new FairRunSim();
  fRun->SetName("TGeant3"); // Transport engine
  fRun->SetOutputFile(SimFile);
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  
  fRun->SetMaterials("media.geo"); // Materials
  
  if ( caveGeom != "" ) {
    FairModule* cave = new CbmCave("CAVE");
    cave->SetGeometryFileName(caveGeom);
    fRun->AddModule(cave);
  }
  
  if ( pipeGeom != "") {
    FairModule* pipe = new CbmPipe("PIPE");
    pipe->SetGeometryFileName(pipeGeom);
    fRun->AddModule(pipe);
  }
  
  if ( magnetGeom != "") {
    FairModule* magnet = new CbmMagnet("MAGNET");
    magnet->SetGeometryFileName(magnetGeom);
    fRun->AddModule(magnet);
  }
  
  if ( stsGeom != "") {
    FairDetector* sts = new CbmStsMC(kTRUE);
    sts->SetGeometryFileName(stsGeom);
    fRun->AddModule(sts);
  }
  
  if ( richGeom != "") {
    //FairDetector* rich = new CbmRich("RICH", kTRUE);
    CbmRich* rich = new CbmRich("RICH", kTRUE);
    rich->SetGeometryFileName(richGeom);
    rich->SetRegisterPhotonsOnSensitivePlane(kTRUE); // Cerenkov photons are also registered in the sim tree
    fRun->AddModule(rich);
  }
  
  CbmFieldMap* magField = new CbmFieldMapSym2(fieldMap);
  magField->SetPosition(0., 0., fieldZ);
  magField->SetScale(fieldScale);
  fRun->SetField(magField);
  
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  
  // e+/-
  FairBoxGenerator* boxGen1 = new FairBoxGenerator(11, 1);
  if(PtNotP==1){boxGen1->SetPtRange(MomMin,MomMax); }
  else{boxGen1->SetPRange(MomMin,MomMax); }
  // boxGen1->SetPRange(0.,10.);
  //boxGen1->SetPtRange(0.,4.);
  boxGen1->SetPhiRange(PhiMin,PhiMax);//0.,360.);
  boxGen1->SetThetaRange(ThetaMin,ThetaMax);//2.5,25.);
  boxGen1->SetCosTheta();

  FairBoxGenerator* boxGen2 = new FairBoxGenerator(-11, 1);
  if(PtNotP==1){boxGen2->SetPtRange(MomMin,MomMax); }
  else{boxGen2->SetPRange(MomMin,MomMax); }
  // boxGen2->SetPtRange(0.,4.);
  // boxGen2->SetPRange(0.,10.);
  boxGen2->SetPhiRange(PhiMin,PhiMax);//0.,360.);
  boxGen2->SetThetaRange(ThetaMin,ThetaMax);//2.5,25.);
  boxGen2->SetCosTheta();

  if(ExtraText == "_MirrUpdated_Electrons."){
    cout<<"gggggggggggggggggggggg electrons "<<endl;    
boxGen1->Init();//el
    primGen->AddGenerator(boxGen1);
  }
  else if(ExtraText == "_MirrUpdated_Positrons."){
      cout<<"gggggggggggggggggggggg positrons "<<endl;    
boxGen2->Init();//pos
  primGen->AddGenerator(boxGen2);
  }else{
    cout<<"gggggggggggggggggggggg electrons and positrons"<<endl;    
    boxGen1->Init();//el
    primGen->AddGenerator(boxGen1);
    boxGen2->Init();//pos
    primGen->AddGenerator(boxGen2);
  }
  
  fRun->SetGenerator(primGen);
  if(StoreTraj){fRun->SetStoreTraj(kTRUE);}
  fRun->Init();
     if(StoreTraj){
    FairTrajFilter* trajFilter = FairTrajFilter::Instance();
    trajFilter->SetStepSizeCut(0.01); // 1 cm
    trajFilter->SetVertexCut(-2000., -2000., -2000., 2000., 2000., 2000.);
    trajFilter->SetMomentumCutP(0.); // p_lab > 0
    trajFilter->SetEnergyCut(0., 10.); // 0 < Etot < 10 GeV
    trajFilter->SetStorePrimaries(kTRUE);//kFALSE);//kTRUE);
  }
  CbmFieldPar* fieldPar = (CbmFieldPar*) rtdb->getContainer("CbmFieldPar");
  fieldPar->SetParameters(magField);
  fieldPar->setChanged();
  fieldPar->setInputVersion(fRun->GetRunId(),1);
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(ParFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  
  fRun->Run(nEvents);
  fRun->CreateGeometryFile(OutPutGeoFile);
  
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << SimFile << endl;
  cout << "Parameter file is " << ParFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << "s" << endl << endl;
  cout << " Test passed" << endl;
  cout << " All ok " << endl;
  cout << "====================================================" << endl;
  cout << "=================== Test Overlaps ==================" << endl;
  gGeoManager->CheckOverlaps(0.001);
  gGeoManager->PrintOverlaps();
  TObjArray *overlapArray = gGeoManager->GetListOfOverlaps();
  Int_t numOverlaps = overlapArray->GetEntries();
  if ( numOverlaps != 0 ) {
    cout << "=================== Test failed =============="<< endl;
    cout << " We have in total "<<numOverlaps<<" overlaps."<<endl;
  }
  cout << "=================== Overlaps Tested ================" << endl;
  cout << "====================================================" << endl;

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
//  return "/data/GeoOpt/Test2/";
//   return "/data/GeoOpt/OptiPMTSize/";
  return "/hera/cbm/users/tariq/MomScan/";
  return "/hera/cbm/users/tariq/GeoOptRootFiles/OptimisedGeo/";
  // if(GeoCase<=0){return "/data/GeoOpt/RotPMT/OlderGeo/";}
  // if(GeoCase==1){return "/data/GeoOpt/RotPMT/OldGeo/";}
  // if(GeoCase==2){return "/data/GeoOpt/RotPMT/NewGeo/";}
}
////////////////////////////////////////////
TString GetMirText(int RotMir, int extend){
  char RotMir_txt[256];
  if(RotMir<0){sprintf( RotMir_txt,"RotMir_m%d",RotMir*-1);}
  else{sprintf(RotMir_txt,"RotMir_p%d",RotMir);}

  stringstream ss; 
  ss<<RotMir_txt;
  if(extend==1){ss<<"_Extended";}
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
TString GetRICH_GeoFile( char *RotMirText, TString PMTRotText, TString PMTTransText, int GeoCase, TString PMTDimsText,TString ExtraText){
  //return "rich/rich_v14a.root";
  // return "rich/minus10deg_ext_mirror.gdml";
  //GeoCase=-2 ==> old geometry with rich_v08a.geo (RICH starts at 1600, Mirror tilt -1)
  //GeoCase=-1 ==> old geometry with rich_v14a.gdml (RICH starts at 1800, Mirror tilt -1)
  if(GeoCase==-2){return "rich/rich_v08a.geo";}
  if(GeoCase==-1){return "rich/rich_v14a.root";}
  //GeoCase=0 ==> old geometry with *.geo (own creation)(RICH starts at 1600, Mirror tilt -1)
  //GeoCase=1 ==> gdml-geo: RICH starts at 1800, Mirror tilt -1 or 10, 
  //                        mirror does NOT cover full acceptance)
  //GeoCase=2 ==> gdml-geo: RICH starts at 1800, Mirror tilt -1 or 10, 
  //                        mirror does cover full acceptance)

  //  return "/hera/cbm/users/tariq/cbmroot/geometry/rich/rich_v14a.root";
// GeoOpt/2015_minus10deg_.gdml";
  // return "/data/cbmroot/macro/rich/geotest/RotPMT/CreateGeo/RichGeo_NominalDimensions_minus10deg_07122014.gdml";
  // return "rich/GeoOpt/rich_geo_RotMir_m10_RotPMT_Xpos5point0_Ypos5point0_TransPMT_Y_p0_Z_p0_New.root";
  //TString Dir="rich/GeoOpt/RotPMT/";
  TString Dir="rich/GeoOpt";

  TString Dir2="/NewGeo/";
  TString Endung="root";
  if(GeoCase==0){Dir2="/OldGeo/"; Endung=".geo";}
  if(GeoCase==1){Dir2="/OldGeo/";}
  if(GeoCase==2){Dir2="/";}
  stringstream ss; 
  //ss<<Dir<<Dir2<<"rich_geo_"<<RotMirText<<"_"<<PMTRotText<<Endung;
  ss<<Dir<<Dir2<<"rich_geo_"<<RotMirText<<"_"<<PMTRotText<<"_"<<PMTTransText<<PMTDimsText<<ExtraText<<Endung;
  //rich_geo_RotMir_m10_Extended_RotPMT_Xpos5point0_Ypos5point0_TransPMT_Y_p10_Z_p80_PMTW1004_H602.root
  return ss.str();
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
////////////////////////////////////////////////////////
TString GetPipe_GeoFile(int GeoCase){
  if(GeoCase == -2 || GeoCase == 0){return "pipe/pipe_standard.geo";}
  else{return "pipe/pipe_v14h.root";}
}

/*
  with old geo files (rotx=5, and roty=5) Notice that the actual rotx is 6.2 !!
  
PMT position in (x,y,z) [cm]: 51  172.525  225.928
Tilted PMT position in (x,y,z) [cm]: 31.115  196.327  209.516
PMT size in x and y [cm]: 50  30
PMT was tilted around x by 6.20664 degrees
PMT was tilted around y by -5 degrees
Refractive index for lowest photon energies: 1.00045, (n-1)*10000: 4.46242
Mirror center (x,y,z): 0 140.394 54.0827
Mirror radius: 300
Mirror tilting angle: -10 degrees
*/
////////////////////

