// this is example how the ROC conditions can be initialized
void set_Get4v1Cond( )
{
   Bool_t bSyncTriggeredTimeWindow = kTRUE; // Sync triggered triggering
//   Bool_t bSyncTriggeredTimeWindow = kFALSE; // Data self triggering

   TimeWindowCond* wincond0 = 0;
   wincond0 = (TimeWindowCond*)gROOT->FindObject("TriggerWindow");
   if ((wincond0!=0) && (strcmp(wincond0->ClassName(), "TimeWindowCond")==0)) {
      cout<<"Set condition TriggerWindow"<<endl;
      if( kTRUE == bSyncTriggeredTimeWindow )
         wincond0->SetValues(-250.0, -50.0); // Sync triggered triggering
         else wincond0->SetValues(-100.0, 100.0); // Data self triggering
   }

   wincond0 = (TimeWindowCond*)gROOT->FindObject("AUXWindow");
   if ((wincond0!=0) && (strcmp(wincond0->ClassName(), "TimeWindowCond")==0)) {
      cout<<"Set condition AUXWindow"<<endl;
      if( kTRUE == bSyncTriggeredTimeWindow )
         wincond0->SetValues(-250.0, -50.0); // Sync triggered triggering
         else wincond0->SetValues(-100.0, 100.0); // Data self triggering
   }

   for (int rocid=0;rocid<20;rocid++) {

      TimeWindowCond* wincond1 = 0;
      wincond1 = (TimeWindowCond*)gROOT->FindObject( Form("Roc%d_EventWindow", rocid) );
      if (wincond1==0) continue;

      if (strcmp(wincond1->ClassName(), "TimeWindowCond")) {
         cout<<"Condition "<<wincond1->ClassName()<<" has wrong class"<<endl;
         continue;
      }

      cout<<"Set condition "<< wincond1->GetName()<<endl;
      if( kTRUE == bSyncTriggeredTimeWindow )
         wincond1->SetValues(-250.0, -50.0); // Sync triggered triggering
         else wincond1->SetValues(-100.0, 100.0); // Data self triggering

      TimeWindowCond* wincond2 = 0;
      wincond2 = (TimeWindowCond*)gROOT->FindObject( Form("Roc%d_AUXWindow", rocid) );
      if (wincond2==0) continue;

      if (strcmp(wincond2->ClassName(), "TimeWindowCond")) {
         cout<<"Condition "<<wincond2->ClassName()<<" has wrong class"<<endl;
         continue;
      }

      cout<<"Set condition "<<wincond2->GetName()<<endl;
      if( kTRUE == bSyncTriggeredTimeWindow )
         wincond2->SetValues(-250.0, -50.0); // Sync triggered triggering
         else wincond2->SetValues(-100.0, 100.0); // Data self triggering
   }
}
