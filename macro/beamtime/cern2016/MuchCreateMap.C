
#include "/home/pmd/daqsoft/cbmroot_trunk/beamtime/MuchRealSize.h"
void MuchCreateMap()
{

  // FairLogger::GetLogger();
  // This code is for making MuchUnpackPar.par file with formating.
	
cout << "##############################################################################" <<endl;
cout << "# Class:   CbmMuchUnpackPar" <<endl;
cout << "# Context: For Gem Mapping" <<endl;
cout << "##############################################################################" <<endl;
cout << "[CbmMuchUnpackPar]" <<endl;
cout << "//----------------------------------------------------------------------------" <<endl;
cout << "NrOfnDpbsModA: Int_t 4 #ModA means M1 Gem Module 1" <<endl;
cout << "nDbpsIdsArray: Int_t \\" <<endl;
cout << "  0  1  2  3   # Need to be replaced with actual nDPBs ids." <<endl;
cout << "NrOfFebs: Int_t 16" <<endl;
cout << "nFebsIdsArray: Int_t \\" <<endl;
 cout << " 0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  #Id for the FEB" <<endl;
cout << "NrOfChannels: Int_t 2048" <<endl;
cout << "ChannelsToPadX: Int_t \\" <<endl;

  // The original maping was developed in Cosy2014 BeamTime
  Int_t NoOfFebs = 15;
  Int_t nrOfChannels=128;
    for (Int_t i=0; i<NoOfFebs; i++) {
	Int_t LineCounter =0;	
	Int_t CharacterCounter =0;
	for (Int_t j=0; j<nrOfChannels; j++) 	{
		
		if(LineCounter<8){
			cout << " ";
                        if (xl[i][j]>=0 && xl[i][j]<10) cout << " ";
			cout << xl[i][j];
			CharacterCounter++;
		} else {
			cout << " \\"<<endl;
			//cout << " \\"<<endl;
			LineCounter = 0;	
		}
		if(CharacterCounter==16){
			LineCounter++;  
			cout << " \\" << endl;
			CharacterCounter=0;
			if (LineCounter==8){
				cout << " \\"<<endl;
				//cout << " \\"<<endl;
				LineCounter = 0;	
			}			
		}	

		}
	}
cout << "ChannelsToPadY: Int_t \\" <<endl;
   for (Int_t i=0; i<NoOfFebs; i++) {
	Int_t LineCounter =0;	
	Int_t CharacterCounter =0;
	for (Int_t j=0; j<nrOfChannels; j++) 	{
		
		if(LineCounter<8){
			cout << " ";
                        if (yl[i][j]>=0 && yl[i][j]<10) cout << " ";
			cout << yl[i][j];
			CharacterCounter++;
		} else {
			cout << " \\"<<endl;
			//cout << " \\"<<endl;
			LineCounter = 0;	
		}
		if(CharacterCounter==16){
			LineCounter++;  
			cout << " \\" << endl;
			CharacterCounter=0;
			if (LineCounter==8){
				cout << " \\"<<endl;
				//cout << " \\"<<endl;
				LineCounter = 0;	
			}			
		}	

		}
	}

}
