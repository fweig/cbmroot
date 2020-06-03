1. Start Root

   root -l

2. Create FairRunOnline Instance

   FairRunOnline* run = new FairRunOnline();

3. Run the setup script which defines the source and the list of tasks. A
simple example can be found in setup_trb.C which was created out of
unpack_trb.C.

   .x setup_trb.C

4. Now the FairRunOnline is initialized and one can execute the event loop.
After the required number of events was executed you end up again at the
root command prompt, which allows you to execute any command or run the
event loop again. The last task in the tasklist (defined in setup_trb.C) should be
the CbmTofOnlineDisplay which creates a TCanvas with the defined list of
histograms. These list and also new Canvases can be defined in the
CbmTofOnlineDisplay class. After the defined number of Events the Canvas is
updated automatically. The update interval is also set in setup_trb.C.

   run->Run(10,0);
   run->Run(1000,0);
   ....

5. If one wants to finish the execution one has to call the Finish function
which will call the necessary code to savely close the input file, update
the output file write the histograms to file and close the file in the end.
After this is done it is not possible to run the event loop another time.

   run->Finish();