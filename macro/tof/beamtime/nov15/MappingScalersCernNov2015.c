// RPC-P5 + Box 1-3
offs     = 0x5;
IN 0 - 3 = > Unused++ +++++++++++IN 4 - 7 = > BOX3 - 1 to BOX3
                                                - 4 +++++++++++++IN 8 - 11 =
  > BOX2 - 1 to BOX2 - 4 +++++++++++++IN 12 - 15 =
    > BOX1 - 1 to BOX1
        - 4 -----------------------------------------------------------------OR
            / AND 0                 = > Unused++ +++++++++++OR / AND 1 =
      > BOX3 - 1 & 2 OR / AND 2     = > BOX3 - 3 & 4 +++++++++++++OR / AND 3 =
        > BOX2 - 1 & 2 OR / AND 4   = > BOX2 - 3 & 4 +++++++++++++OR / AND 5 =
          > BOX1 - 1 & 2 OR / AND 6 = > BOX1 - 3 & 4 +++++++++++++OR / AND 7 =
            > Unused

              // RPC-P2
              offs = 0x6;
IN 0
  - 15 = > P2 - OR - 01 to P2 - OR
             - 16 -----------------------------------------------------------------OR
                 / AND 0 = > P2
                               - 01 & 16 OR
                                   / AND 1 = > P2
                                                 - 02 & 15 OR
                                                     / AND 2 = > P2
                                                                   - 03 & 14 OR
                                                                       / AND 3 =
  > P2
      - 04 & 13 OR
          / AND 4 = > P2
                        - 05 & 12 OR
                            / AND 5 = > P2
                                          - 06 & 11 OR
                                              / AND 6 = > P2
                                                            - 07 & 10 OR
                                                                / AND 7 =
    > P2
        - 08 & 09 -----------------------------------------------------------------LEMO 0 =
      > Full OR(&) P2

      // PAD
      offs                             = 0x7;
IN 0 - 0                               = > PMT - OR - 1 IN 1
               - 6                     = > PAD - OR - 1 to PAD - OR - 6 IN 7
                         - 7           = > PMT - OR - 2 +++++++++++++IN 8
                                   - 8 = > CRPC - OR - 01 IN 9
                                             - 9 = > CRPC - OR
                                                       - 02 +++++++++++++IN 10
                                                       - 15 =
  > Unused-- ---------------------------------------------------------------OR
      / AND =
    > Unused-- ---------------------------------------------------------------LEMO 0 =
      > PAD Full OR LEMO 1 = > CRPC Full OR

                               // BUC 2012-2015
                               offs = 0x8;
IN 0 - 7            = > B2012 - OR - 1 to B2012 - OR - 8 +++++++++++++IN 8
               - 11 = > B2015 - OR - 1 to B2015 - OR - 4 +++++++++++++IN 12
                          - 15 =
  > Unused-- ---------------------------------------------------------------OR
      / AND 0 = > B2012 - 1 & 2 OR / AND 1 = > B2012
                                                 - 3 & 4 OR
                                                     / AND 2 = > B2012
                                                                   - 5 & 6 OR
                                                                       / AND 3 =
    > B2012
        - 7 & 8 +++++++++++++OR
            / AND 4                          = > B2015
                          - 1 & 2 OR / AND 5 = > B2015
                                                   - 3 & 4 +++++++++++++OR
                                                       / AND 6 = > Unused OR
                                                                     / AND 7 =
      > Unused-- ---------------------------------------------------------------LEMO 0 =
        > B2012 Full OR :

  // Buc-Ref
  offs = 0x9;
IN 0
  - 15 = > BREF - OR - 01 to BREF - OR
             - 16 -----------------------------------------------------------------OR
                 / AND 0 = > BREF
                               - 1 & 16 OR
                                   / AND 1 = > BREF
                                                 - 2 & 15 OR
                                                     / AND 2 = > BREF
                                                                   - 3 & 14 OR
                                                                       / AND 3 =
  > BREF
      - 4 & 13 OR
          / AND 4 = > BREF
                        - 5 & 12 OR
                            / AND 5 = > BREF
                                          - 6 & 11 OR
                                              / AND 6 = > BREF
                                                            - 7 & 10 OR
                                                                / AND 7 =
    > BREF
        - 8 & 9 -----------------------------------------------------------------LEMO 0 =
      > Full OR(&) BREF

      // HD-P5
      offs = 0xA;
IN 0 - 15 =
  > P5 - CH - 01 to P5 - CH
      - 16 -----------------------------------------------------------------OR
          / AND =
    > Unused-- ---------------------------------------------------------------LEMO 0 =
      > P5 Full OR
