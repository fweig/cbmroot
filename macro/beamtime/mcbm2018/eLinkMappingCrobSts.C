

void eLinkMappingCrobStsFebA() {
  const UInt_t kuNbElinks             = 42;
  const UInt_t kuNbFebs               = 5;
  const UInt_t kuNbSmx                = 8;
  UInt_t uElinkMap[kuNbFebs][kuNbSmx] = {{38, 41, 33, 39, 34, 37, 35, 40},
                                         {31, 28, 32, 36, 22, 29, 19, 30},
                                         {14, 20, 15, 21, 27, 23, 25, 24},
                                         {5, 16, 6, 17, 7, 26, 13, 18},
                                         {3, 0, 4, 1, 8, 2, 12, 9}};
  UInt_t uAddrMap[kuNbFebs][kuNbSmx]  = {{7, 6, 5, 4, 3, 2, 1, 0},
                                        {7, 6, 5, 4, 3, 2, 1, 0},
                                        {7, 6, 5, 4, 3, 2, 1, 0},
                                        {7, 6, 5, 4, 3, 2, 1, 0},
                                        {7, 6, 5, 4, 3, 2, 1, 0}};

  Int_t iFebMap[kuNbElinks];
  UInt_t uSmxMap[kuNbElinks];
  UInt_t uElinkMapReordered[kuNbFebs][kuNbSmx];

  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    Bool_t bFoundSmx = kFALSE;
    for (UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx)
      for (UInt_t uSmxIdx = 0; uSmxIdx < kuNbSmx; ++uSmxIdx)
        if (uElinkMap[uFebIdx][uSmxIdx] == uElinkIdx) {
          //               UInt_t uGlobSmxIdx = uFebIdx * kuNbSmx + uAddrMap[ uFebIdx ][ uSmxIdx ];
          UInt_t uGlobSmxIdx = uFebIdx * kuNbSmx + uSmxIdx;
          uSmxMap[uElinkIdx] = uGlobSmxIdx;
          iFebMap[uElinkIdx] = uFebIdx;
          std::cout << Form("eLink %02u FEB %u SMX %u => Global SMX Idx %02u",
                            uElinkIdx,
                            uFebIdx,
                            uSmxIdx,
                            uGlobSmxIdx)
                    << std::endl;
          bFoundSmx = kTRUE;
        }  // if( uElinkMap[ uFebIdx ][ uSmxIdx ] == uElinkIdx )
    if (kFALSE == bFoundSmx) {
      uSmxMap[uElinkIdx] = 0xFFFF;
      iFebMap[uElinkIdx] = -1;
      std::cout << Form("eLink %02u               => Not Used!!", uElinkIdx)
                << std::endl;
    }
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )

  for (UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx)
    for (UInt_t uSmxIdx = 0; uSmxIdx < kuNbSmx; ++uSmxIdx) {
      uElinkMapReordered[uFebIdx][uAddrMap[uFebIdx][uSmxIdx]] =
        uElinkMap[uFebIdx][uSmxIdx];
    }  // Loop on FEB and not ordered smx

  std::cout << " Parameter map from eLink to Asic: " << std::endl;
  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    //      std::cout << Form( " %6u", uSmxMap[ uElinkIdx ] );
    std::cout << Form(" 0x%04X", uSmxMap[uElinkIdx]);
    if (kuNbElinks - 1 != uElinkIdx && 5 == uElinkIdx % 6)
      std::cout << " \\" << std::endl;
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )
  std::cout << std::endl;

  std::cout << " Parameter map from eLink to Asic for monitor: " << std::endl;
  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    //      std::cout << Form( " %6u", uSmxMap[ uElinkIdx ] );
    std::cout << Form(" 0x%04X,", uSmxMap[uElinkIdx]);
    if (kuNbElinks - 1 != uElinkIdx && 5 == uElinkIdx % 6)
      std::cout << std::endl;
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )
  std::cout << std::endl;

  std::cout << " Parameter map from eLink to FEB: " << std::endl;
  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    std::cout << Form(" %2d", iFebMap[uElinkIdx]);
    if (kuNbElinks - 1 != uElinkIdx && 7 == uElinkIdx % 8)
      std::cout << " \\" << std::endl;
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )
  std::cout << std::endl;

  std::cout << " Reordered map from (Feb, Asic) to eLink: " << std::endl;
  for (UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx) {
    for (UInt_t uSmxIdx = 0; uSmxIdx < kuNbSmx; ++uSmxIdx)
      std::cout << Form(" %2u", uElinkMapReordered[uFebIdx][uSmxIdx]);
    std::cout << std::endl;
  }  // for( UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx )

  return;
}

void eLinkMappingCrobStsFebB() {
  const UInt_t kuNbElinks             = 42;
  const UInt_t kuNbFebs               = 5;
  const UInt_t kuNbSmx                = 8;
  UInt_t uElinkMap[kuNbFebs][kuNbSmx] = {{35, 40, 34, 37, 33, 39, 38, 41},
                                         {19, 30, 22, 29, 32, 36, 31, 28},
                                         {25, 24, 27, 23, 15, 21, 14, 20},
                                         {13, 18, 7, 26, 6, 17, 5, 16},
                                         {12, 9, 8, 2, 4, 1, 3, 0}};
  UInt_t uAddrMap[kuNbFebs][kuNbSmx]  = {{1, 0, 3, 2, 5, 4, 7, 6},
                                        {1, 0, 3, 2, 5, 4, 7, 6},
                                        {1, 0, 3, 2, 5, 4, 7, 6},
                                        {1, 0, 3, 2, 5, 4, 7, 6},
                                        {1, 0, 3, 2, 5, 4, 7, 6}};

  Int_t iFebMap[kuNbElinks];
  UInt_t uSmxMap[kuNbElinks];
  UInt_t uElinkMapReordered[kuNbFebs][kuNbSmx];

  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    Bool_t bFoundSmx = kFALSE;
    for (UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx)
      for (UInt_t uSmxIdx = 0; uSmxIdx < kuNbSmx; ++uSmxIdx)
        if (uElinkMap[uFebIdx][uSmxIdx] == uElinkIdx) {
          //               UInt_t uGlobSmxIdx = uFebIdx * kuNbSmx + uAddrMap[ uFebIdx ][ uSmxIdx ];
          UInt_t uGlobSmxIdx = uFebIdx * kuNbSmx + uSmxIdx;
          uSmxMap[uElinkIdx] = uGlobSmxIdx;
          iFebMap[uElinkIdx] = uFebIdx;
          std::cout << Form("eLink %02u FEB %u SMX %u => Global SMX Idx %02u",
                            uElinkIdx,
                            uFebIdx,
                            uSmxIdx,
                            uGlobSmxIdx)
                    << std::endl;
          bFoundSmx = kTRUE;
        }  // if( uElinkMap[ uFebIdx ][ uSmxIdx ] == uElinkIdx )
    if (kFALSE == bFoundSmx) {
      uSmxMap[uElinkIdx] = 0xFFFF;
      iFebMap[uElinkIdx] = -1;
      std::cout << Form("eLink %02u               => Not Used!!", uElinkIdx)
                << std::endl;
    }
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )

  for (UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx)
    for (UInt_t uSmxIdx = 0; uSmxIdx < kuNbSmx; ++uSmxIdx) {
      uElinkMapReordered[uFebIdx][uAddrMap[uFebIdx][uSmxIdx]] =
        uElinkMap[uFebIdx][uSmxIdx];
    }  // Loop on FEB and not ordered smx

  std::cout << " Parameter map from eLink to Asic: " << std::endl;
  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    //      std::cout << Form( " %6u", uSmxMap[ uElinkIdx ] );
    std::cout << Form(" 0x%04X", uSmxMap[uElinkIdx]);
    if (kuNbElinks - 1 != uElinkIdx && 5 == uElinkIdx % 6)
      std::cout << " \\" << std::endl;
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )
  std::cout << std::endl;

  std::cout << " Parameter map from eLink to Asic for monitor: " << std::endl;
  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    //      std::cout << Form( " %6u", uSmxMap[ uElinkIdx ] );
    std::cout << Form(" 0x%04X,", uSmxMap[uElinkIdx]);
    if (kuNbElinks - 1 != uElinkIdx && 5 == uElinkIdx % 6)
      std::cout << std::endl;
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )
  std::cout << std::endl;

  std::cout << " Parameter map from eLink to FEB: " << std::endl;
  for (UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElinkIdx) {
    std::cout << Form(" %2d", iFebMap[uElinkIdx]);
    if (kuNbElinks - 1 != uElinkIdx && 7 == uElinkIdx % 8)
      std::cout << " \\" << std::endl;
  }  // for( UInt_t uElinkIdx = 0; uElinkIdx < kuNbElinks; ++uElInkIdx )
  std::cout << std::endl;

  std::cout << " Reordered map from (Feb, Asic) to eLink: " << std::endl;
  for (UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx) {
    for (UInt_t uSmxIdx = 0; uSmxIdx < kuNbSmx; ++uSmxIdx)
      std::cout << Form(" %2u", uElinkMapReordered[uFebIdx][uSmxIdx]);
    std::cout << std::endl;
  }  // for( UInt_t uFebIdx = 0; uFebIdx < kuNbFebs; ++uFebIdx )

  return;
}

void eLinkMappingCrobSts() {
  std::cout << "================= FEB A ===================" << std::endl;
  eLinkMappingCrobStsFebA();

  std::cout << "================= FEB B ===================" << std::endl;
  eLinkMappingCrobStsFebB();

  return;
}
