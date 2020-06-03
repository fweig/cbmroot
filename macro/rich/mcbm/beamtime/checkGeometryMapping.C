/** @file RICH Geometry and Mapping checker
 ** @author Adrian Weber <a.weber@gsi.de>
 ** @date 05.08.2019
 ** ROOT macro to check the RICH Geometry and the Mapping in the Geo File
 */

void checkGeometryMapping()
{
    //TString buildDir = gSystem->Getenv("CBMROOT_BUILD");
    //gSystem->Load(buildDir + "/lib/libCbmRich.so");
    
    std::cout<<"checking the Geometry.."<<std::endl<<std::endl;
    std::cout<<"Import Geometry...";
    TString srcDir = gSystem->Getenv("VMCWORKDIR");
    TString geoFile = srcDir + "/macro/mcbm/data/mcbm_beam_2019_03.geo.root";
    gGeoManager->Import(geoFile.Data());        
    std::cout<<"DONE!"<<std::endl;
    
    std::cout<<"Create CbmRichMCbmDigiMapManager...";
    CbmRichMCbmDigiMapManager::GetInstance();
    std::cout<<"DONE!"<<std::endl;
    std::vector<CbmRichPixelData*> pixelData;
    std::vector<CbmRichPmtData*> PmtData;
    
    void DrawPMTs(std::vector<CbmRichPmtData*> pmt);
    void DrawFull(std::vector<CbmRichPmtData*> ,std::vector<CbmRichPixelData*>);
    
    // -----   Timer   --------------------------------------------------------
    TStopwatch timer;
    timer.Start();
    // ------------------------------------------------------------------------
    
    int Pixelcnt = 0;
    for (Int_t dirich =0x0000; dirich<= 0xFFFF; ++dirich) {
        for (Int_t ch =1; ch< 33; ++ch) {
            Int_t address = (dirich << 16) | (ch & 0x00FF);
            CbmRichPixelData* data =  CbmRichMCbmDigiMapManager::GetInstance().GetPixelDataByAddress(address);

            if (data != nullptr) {
                pixelData.push_back(data);
                int size = pixelData.size();
                //std::cout<<"X: "<<pixelData.back()->fX<<"  Y: "<<pixelData[size-1]->fY<<"  Z: "<<pixelData[size-1]->fZ<<std::endl;
                Pixelcnt++;
            }
        }
    }
    
    std::cout<<"[INFO] Found "<< Pixelcnt << " Pixels." <<std::endl;
    
    vector<Int_t> PmtIds = CbmRichMCbmDigiMapManager::GetInstance().GetPmtIds();

    std::cout<<"Number of Ids: "<<PmtIds.size()<<std::endl;
    for (auto i : PmtIds) {
        CbmRichPmtData* data =  CbmRichMCbmDigiMapManager::GetInstance().GetPmtDataById(i);
        PmtData.push_back(data);
        //std::cout<<"CbmRichPmtData.fX|fY: "<<PmtData.back()->fX<<"   "<<PmtData.back()->fY<<std::endl;
        //std::cout<<"CbmRichPmtData.fPixelAddresses.size: "<<PmtData.back()->fPixelAddresses.size()<<std::endl;
    }
    
    DrawPMTs(PmtData);
    DrawFull(PmtData,pixelData);
    // -----   Finish   -------------------------------------------------------
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    std::cout << std::endl << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
    std::cout << "Real time " << rtime << " s, CPU time " << ctime << " s"<< std::endl;
  // ------------------------------------------------------------------------

}

void DrawPMTs(std::vector<CbmRichPmtData*> pmts){
    TCanvas* c = new TCanvas("fhPmtMap","fhPmtMap",1000,1000);
    c->SetGrid(true, true);
    TH2D* pad = new TH2D("PmtMap", "PmtMap;X [cm];Y [cm]", 100, -40., 40., 100, -40., 40.);
    pad->SetStats(false);
    pad->Draw();
    
    for (auto pmt : pmts) {
        auto x = pmt->fX;
        auto y = pmt->fY;
        auto width  = pmt->fWidth;
        auto height = pmt->fHeight;
        TBox* box = new TBox(x-width/2,y-height/2,x+width/2,y+height/2);
        box->SetFillStyle(0);
        box->SetLineColor(kBlack);
        box->SetLineWidth(2);
        box->Draw();
        char id[4];
        sprintf(id,"0x%02x",pmt->fId);
        TText* t = new TText(x-width/2+0.5,y-0.5,id);
        t->SetTextSize(0.02);
        t->Draw();
    }
    
}

void DrawFull(std::vector<CbmRichPmtData*> pmts, std::vector<CbmRichPixelData*> pixels){
    TCanvas* c = new TCanvas("fhPmtMapFull","fhPmtMapFull",1000,1000);
    c->SetGrid(true, true);
    TH2D* pad = new TH2D("PmtMapFull", "PmtMapFull;X [cm];Y [cm]", 100, -40., 40., 100, -40., 40.);
    pad->SetStats(false);
    pad->Draw();
    
    for (auto pmt : pmts) {
        auto x = pmt->fX;
        auto y = pmt->fY;
        auto width  = pmt->fWidth;
        auto height = pmt->fHeight;
        TBox* box = new TBox(x-width/2,y-height/2,x+width/2,y+height/2);
        box->SetFillStyle(0);
        box->SetLineColor(kBlack);
        box->SetLineWidth(2);
        box->Draw();
    }
    
    for (auto pixel : pixels){
        auto x = pixel->fX;
        auto y = pixel->fY;
        auto width  = 0.6;//pmt->fWidth;
        auto height = 0.6;//pmt->fHeight;
        int addr = pixel->fAddress;
//         int addr_end = addr & 0xF;
//         if (addr_end == 0x0 || addr_end == 0xF || addr_end == 0x1 || addr_end == 0x2 ) {
//             width  = 0.625;//pmt->fWidth;
//         }
//         if ((((addr >>16) & 0x1) == 1 &&
//                 ( addr_end == 0xF || addr_end == 0xd || addr_end == 0xb || addr_end == 0x9 ||
//                   addr_end == 0x7 || addr_end == 0x5 || addr_end == 0x3 || addr_end == 0x1)
//             (((addr >>16) & 0x1) == 0 && 
//                 ( addr_end == 0xF || addr_end == 0xd || addr_end == 0xb || addr_end == 0x9 ||
//                   addr_end == 0x7 || addr_end == 0x5 || addr_end == 0x3 || addr_end == 0x1)
//             )
//         ){
//             width  = 0.625;//pmt->fWidth;
//         }
        TBox* box = new TBox(x-width/2,y-height/2,x+width/2,y+height/2);
        box->SetFillStyle(0);
        box->SetLineColor(kBlack);
        box->SetLineWidth(2);
        box->Draw();
        char id[4];
        sprintf(id,"0x%08x",addr);
        TText* t = new TText(x-width/2,y,id);
        t->SetTextSize(0.01);
        t->Draw();
    }
    

}