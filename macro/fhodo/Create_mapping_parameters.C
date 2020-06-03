void Create_mapping_parameters()
{

  FairLogger::GetLogger();
  // This code was copied from the Go4 analysis used for previous beamtimes
  // The original maping was developed by Christian Pauli 

  Int_t nrOfChannels=128;

  Int_t fHodoFiber[nrOfChannels];
  Int_t fHodoPlane[nrOfChannels];
  Int_t fHodoPixel[nrOfChannels];
  for (Int_t i=0; i<nrOfChannels; i++) {
    fHodoFiber[i] = -1;
    fHodoPlane[i] = -1;
    fHodoPixel[i] = -1;
  }
  
  for (Int_t ifiber=1; ifiber<=64; ifiber++) {
    // Calculate fiber number [1..64] from feb channel
    // lcn: linearconnectornumber, is the wire number on one of the
    // flat cables. [1..16]
    // each 16 fibers go to one connector.
    // fibersubnr[0..15] linear fiber counter in groups of 16

    Int_t fibersubnr=(ifiber-1)%16;

    Int_t lcn=15-fibersubnr*2;
    if (fibersubnr>=8) lcn=(fibersubnr-7)*2;
    
    Int_t channel=-1;
    Int_t cable=(ifiber-1)/16+1;
    Int_t pixel= ((lcn-1)/2)*8 +((lcn-1)%2);
    if (cable==1) {
      channel=(lcn-1)*4+0;
      pixel=pixel+1;
    }
    if (cable==2) {
      channel=(lcn-1)*4+2;
      pixel=pixel+3;
    }
    if (cable==3) {
      channel=(lcn-1)*4+1;
      pixel=pixel+5;
    }
    if (cable==4) {
      channel=(lcn-1)*4+3;
      pixel=pixel+7;
    }
    
    // new code to resolve cabling problem during cern-oct12
    int ifiber_bis = ifiber;
    if (ifiber <= 8 )  ifiber_bis = ifiber + 56; else
      if (ifiber <= 16 ) ifiber_bis = ifiber + 40; else
        if (ifiber <= 24 ) ifiber_bis = ifiber + 24; else
          if (ifiber <= 32 ) ifiber_bis = ifiber + 8; else 
            if (ifiber <= 40 ) ifiber_bis = ifiber - 8; else 
              if (ifiber <= 48 ) ifiber_bis = ifiber - 24; else
                if (ifiber <= 56 ) ifiber_bis = ifiber - 40; else
                  if (ifiber <= 64 ) ifiber_bis = ifiber - 56;
    
    // and swap at the end
    ifiber_bis = 65 - ifiber_bis;

    fHodoFiber[channel] = ifiber_bis - 1;
    fHodoPlane[channel] = 0;
    fHodoPixel[channel] = pixel;

    fHodoFiber[channel+64] = ifiber_bis - 1;
    fHodoPlane[channel+64] = 1;
    fHodoPixel[channel+64] = pixel;

  }
  
  cout << "NrOfChannels: Int_t " << nrOfChannels << endl;

  cout << "ChannelToFiberMap: Int_t \\" << endl;
  Int_t counter=0;
  for (Int_t i=0; i<nrOfChannels; i++) {
    if (fHodoFiber[i]<10) {
      cout << " ";
    }
    if (counter<9) {
      cout << fHodoFiber[i] << " ";
      counter++;
    } else {
      cout << fHodoFiber[i] << " \\" << endl;
      counter=0;
    }
  }
  cout << endl;

  cout << "ChannelToPixelMap: Int_t \\" << endl;
  counter=0;
  for (Int_t i=0; i<nrOfChannels; i++) {
    if (fHodoPixel[i]<10) {
      cout << " ";
    }
    if (counter<9) {
      cout << fHodoPixel[i] << " ";
      counter++;
    } else {
      cout << fHodoPixel[i] << " \\" << endl;
      counter=0;
    }
  }
  cout << endl;

  cout << "ChannelToPlaneMap: Int_t \\" << endl;
  counter=0;
  for (Int_t i=0; i<nrOfChannels; i++) {
    if (fHodoPlane[i]<10) {
      cout << " ";
    }
    if (counter<9) {
      cout << fHodoPlane[i] << " ";
      counter++;
    } else {
      cout << fHodoPlane[i] << " \\" << endl;
      counter=0;
    }
  }
  cout << endl;


/*
  for (Int_t i=0; i<nrOfChannels; i++) {
    LOG(INFO) << "Channel[" << i << "]: " << fHodoFiber[i] << ", " 
              << fHodoPlane[i] << ", " << fHodoPixel[i] 
              << FairLogger::endl;
  }
*/

}