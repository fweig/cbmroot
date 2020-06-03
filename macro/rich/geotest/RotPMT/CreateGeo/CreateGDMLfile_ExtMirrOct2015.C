#include <iostream>
#include <fstream>  
void CreateGDMLfile_ExtMirrOct2015(float pmt_pos_y_addend = 10, float pmt_pos_z_addend = 80, int RotMir=-10, float PMTrotX=5, float PMTrotY=5 , float EnlargedPMTWidth=4., float EnlargedPMTHight=2.){

  //////////////////////////////////////
  //Define variables
  float Delta_pmt_width=EnlargedPMTWidth;
  float Delta_pmt_height=EnlargedPMTHight;

  float Delta_RICH_height=2.2*Delta_pmt_height;//1900;
  float Delta_RICH_entrance_width=2.2*Delta_pmt_width;//950;
  float Delta_RICH_exit_width=2.2*Delta_pmt_width;
  float Delta_RICH_length=0.;//800;

  float Delta_RICH_position_from_IP=0.;//-800.;//==Delta_RICH_length
  float Delta_sens_plane_pos=1200.;//1220.;
  float Delta_sens_plane_area=0.1; 

  ////////////////////////////////////
  float RICH_trapezoid_angle = 116.;
  float RICH_entrance_width = 2551.17 + Delta_RICH_entrance_width;
  float RICH_exit_width = 5136. + Delta_RICH_exit_width;
  float RICH_height = 4100. + Delta_RICH_height;
  float RICH_length = 1899.5 + Delta_RICH_length;
  float RICH_covering_thickness = 5.;
  float entrance_width_addend = 2.;
  float RICH_pipe_entrance_r = 103.5;
  float RICH_pipe_exit_r = 205.;
  float RICH_pipe_entrance_thickness = 3.;
  float RICH_pipe_exit_thickness = 5.;
  float RICH_pipe_angle = 0.;
  float RICH_position_from_IP = 1800. + Delta_RICH_position_from_IP;

  float RICH_mirror_position_from_IP = 3500.;
  float RICH_mirror_r = 3000.;
  float RICH_mirror_thickness = 6.;
  float mirror_cont_supp_addend_out = 6.;
  float pmt_width = 1000. + Delta_pmt_width;
  float pmt_height = 600. + Delta_pmt_height;
  float pmt_thickness = 0.5;
  float TTT = 1500.;
  float RICH_mirror_Y_shift = 805.;
  float RICH_mirror_angle = RotMir;
  float pmt_pos_x_addend = 0.;

  float sens_plane_pos =  RICH_mirror_position_from_IP-RICH_position_from_IP-Delta_sens_plane_pos;

  float DefaultRotX=32.952765; float DefaultRotY=18.477;//for rotmir=-10
  if(RotMir==1){DefaultRotX=10.952765.;}
  if(RotMir==-1){DefaultRotX=14.952765.;}
  //if(RotMir==-7){DefaultRotX=14.952765.;}
  
  float pmt_rot_x_addend = PMTrotX-DefaultRotX;//-27.952765;
  float pmt_rot_y_addend = PMTrotY-DefaultRotY;//-13.477;
  
  char GeoFileName[256];
  //  char* InFileLower="CreateGeo/2015_minus10deg_LowerPart_ModAug2015.gdml";
  char* InFileLower="CreateGeo/minus10deg_ext_mirror_LowerPart.gdml";
  //char* InFileLower="minus10deg_ext_mirror_LowerPart.gdml";
  cout<<InFileLower<<endl;
 
  char RotMirText[256];
  if(RotMir<0){sprintf( RotMirText,"RotMir_m%d_Extended",RotMir*-1);}
  else{sprintf( RotMirText,"RotMir_p%d_Extended",RotMir);}

  int ShiftXmod10=(int(PMTrotX*10)) % 10;  
  float IntegerXValue=PMTrotX-(float (ShiftXmod10))/10.;
  int ShiftYmod10=(int(PMTrotY*10)) % 10;
  float IntegerYValue=PMTrotY-(float (ShiftYmod10))/10.;
  
  char ShiftXTxt[256]; char ShiftYTxt[256];
  sprintf(ShiftXTxt,"Xpos%dpoint%d",IntegerXValue,ShiftXmod10);
  sprintf(ShiftYTxt,"Ypos%dpoint%d",IntegerYValue,ShiftYmod10);
  if(PMTrotY<0){sprintf(ShiftYTxt,"Yneg%dpoint%d",-1.*IntegerYValue,-1.*ShiftYmod10);}
  /////////////////////////////////// Translation in z
  char ZTransText[256];
  if(pmt_pos_z_addend < 0) {sprintf( ZTransText,"Z_m%d",-1*pmt_pos_z_addend);}
  else{sprintf( ZTransText,"Z_p%d",pmt_pos_z_addend);}
  char YTransText[256];
  if(pmt_pos_y_addend < 0) {sprintf( YTransText,"Y_m%d",-1*pmt_pos_y_addend);}
  else{sprintf( YTransText,"Y_p%d",pmt_pos_y_addend);}
  
  char PMTDimsText[256];
  sprintf( PMTDimsText,"PMTW%d_H%d",pmt_width, pmt_height);
  
  std::ifstream infile2 (InFileLower);
  char *Path1="/data/cbmroot";
  TString ExtraText=".";//
     
  sprintf(GeoFileName,"/geometry/rich/GeoOpt/rich_geo_%s_RotPMT_%s_%s_TransPMT_%s_%s_%s%s",RotMirText,ShiftXTxt,ShiftYTxt,YTransText,ZTransText,PMTDimsText,ExtraText.Data());
  cout<<GeoFileName<<endl;
  
  char OutFile_GDML_Ch[256];
  sprintf(OutFile_GDML_Ch,"%s%sgdml",Path1,GeoFileName);
  cout<<OutFile_GDML_Ch<<endl;
  
  string line;
  std::vector<std::string> lines;
  std::ofstream OutFile_GDML (OutFile_GDML_Ch);
  
  OutFile_GDML <<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<gdml xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd\">" << std::endl;
  
  OutFile_GDML <<"<define>" << std::endl;
  OutFile_GDML <<"<position name=\"central\" x=\"0\" y=\"0\" z=\"0\" unit=\"mm\"/>" << std::endl;
  OutFile_GDML <<"<rotation name=\"identity\" x=\"0\" y=\"0\" z=\"0\" unit=\"degree\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_trapezoid_angle\" value=\""<<RICH_trapezoid_angle<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_entrance_width\" value=\""<<RICH_entrance_width<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_exit_width\" value=\""<<RICH_exit_width<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_height\" value=\""<<RICH_height<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_length\" value=\""<<RICH_length<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_covering_thickness\" value=\""<<RICH_covering_thickness<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"entrance_width_addend\" value=\""<<entrance_width_addend<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_pipe_entrance_r\" value=\""<<RICH_pipe_entrance_r<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_pipe_exit_r\" value=\""<<RICH_pipe_exit_r<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_pipe_entrance_thickness\" value=\""<<RICH_pipe_entrance_thickness<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_pipe_exit_thickness\" value=\""<<RICH_pipe_exit_thickness<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_pipe_angle\" value=\""<<RICH_pipe_angle<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_position_from_IP\" value=\""<<RICH_position_from_IP<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"sens_plane_pos\" value=\""<<sens_plane_pos<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"Delta_sens_plane_area\" value=\""<<Delta_sens_plane_area<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_mirror_position_from_IP\" value=\""<<RICH_mirror_position_from_IP<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_mirror_r\" value=\""<<RICH_mirror_r<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_mirror_thickness\" value=\""<<RICH_mirror_thickness<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"mirror_cont_supp_addend_out\" value=\""<<mirror_cont_supp_addend_out<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"pmt_width\" value=\""<<pmt_width<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"pmt_height\" value=\""<<pmt_height<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"pmt_thickness\" value=\""<<pmt_thickness<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"TTT\" value=\""<<TTT<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_mirror_Y_shift\" value=\""<<RICH_mirror_Y_shift<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"RICH_mirror_angle\" value=\""<<RICH_mirror_angle<<"\"/>" << std::endl;
  // OutFile_GDML <<"<variable name=\"pmt_rot_x_addend\" value=\""<<pmt_rot_x_addend<<"\"/>" << std::endl;
  // OutFile_GDML <<"<variable name=\"pmt_rot_y_addend\" value=\""<<pmt_rot_y_addend<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"pmt_pos_x_addend\" value=\""<<pmt_pos_x_addend<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"pmt_pos_y_addend\" value=\""<<pmt_pos_y_addend<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"pmt_pos_z_addend\" value=\""<<pmt_pos_z_addend<<"\"/>" << std::endl;
  
  OutFile_GDML <<"<variable name=\"kapton_thickness\" value=\""<<0.25<<"\"/>" << std::endl;
  OutFile_GDML <<"<variable name=\"DEGtoRAD\" value=\""<<1.74532925199433E-02<<"\"/>" << std::endl;
  
  OutFile_GDML <<"<variable name=\"pmt_rot_x_addend\" value=\""<<PMTrotX-DefaultRotX<<"\"/>";  
  OutFile_GDML << " <!-- rot x ="<< PMTrotX <<" --> " << std::endl;
  OutFile_GDML <<"<variable name=\"pmt_rot_y_addend\" value=\""<<PMTrotY-DefaultRotY<<"\"/>";
  OutFile_GDML << " <!-- rot y ="<< PMTrotY <<" --> " << std::endl;
  
  OutFile_GDML <<"</define>" << std::endl;
  
  OutFile_GDML << "                  " << std::endl;
  string line;
  //std::vector<std::string> lines;
  while ( getline (infile2,line) ){OutFile_GDML << line<<std::endl;}
  // cout<<"####################################################"<<endl;
  OutFile_GDML.close(); 
  
  /////////////////////////////GDML to ROOT  
  /////////////////////////////GDML to ROOT  
  stringstream ss; 
  ss<<GeoFileName;
  TString GeoFileName_Str=ss.str();//FileName=ss.str();
  
  TString OutFile_GDML_Str=GeoFileName_Str+"gdml";
  TString OutFile_ROOT=GeoFileName_Str+"root";
  cout<<OutFile_GDML_Str<<endl;
  cout<<OutFile_ROOT<<endl;

  TString vmcDir = TString(gSystem->Getenv("VMCWORKDIR"));
  
  TGeoManager *gdml = new TGeoManager("gdml", "FAIRGeom");
  
  TGDMLParse parser;
  // Define your input GDML file HERE
  TGeoVolume* gdmlTop = parser.GDMLReadFile (vmcDir + OutFile_GDML_Str);
  TGeoVolume* rootTop = new TGeoVolumeAssembly("TOP");
  
  gGeoManager->SetTopVolume(rootTop);
  
  // Define your position HERE
  TGeoRotation* rot = new TGeoRotation ("rot", 0., 0., 0.);
  TGeoCombiTrans* posrot = new TGeoCombiTrans (0., 0., 270., rot);
  
  rootTop->AddNode (gdmlTop, 1, posrot);
  
  gGeoManager->CloseGeometry();
  
  // Define you output ROOT file HERE
  TFile* outfile = new TFile(vmcDir + OutFile_ROOT, "RECREATE");
  rootTop->Write();
  outfile->Close();

}
