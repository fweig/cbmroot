
void TMbsCrateProc::Process1290(int num, int* pdata, unsigned int len)
{
   if (num >= MAX_1290) {
      cout << "Wrong 1290 index " << num << endl;
      return;
   }

   int expected_geo = 0;

   if (num<0) {
      // if number is not specified, try to identify it base on GEO value
      expected_geo = T1290Data::FindGeo(pdata, len);
      if (expected_geo<=0) {
         printf("ERROR: Did not found GEO in the data\n");
         return;
      }

      switch (expected_geo) {
         case 5: num = 8; break;
         case 6: num = 9; break;
         case 7: num = 10; break;
         case 8: num = 11; break;
         case 9: num = 12; break;
         case 10: num = 13; break;
         case 11: num = 14; break;
         default:
            printf("Unsupported GEO code %d\n", expected_geo);
            return;
      }

      // printf("Found GEO code %d defined num %d\n", expected_geo, num);

   } else {
      switch(num) {
         case 0: expected_geo = 8; break;
         case 1: expected_geo = 9; break;
         case 2: expected_geo = 10; break;
         case 3: expected_geo = 11; break;
         case 4: expected_geo = 12; break;
         case 5: expected_geo = 13; break;
         case 6: expected_geo = 14; break;
         case 7: expected_geo = 15; break;
       }
   }
   // printf("Process1290 num = %d expected GEO = %d len = %u\n", num, expected_geo, len);

   fOutputEvent->fMtdc[num].Unpack(pdata, len, expected_geo);

   if( 1 == fPar->uCaenTdcHistosEnabled[num] )
   {
      if( kTRUE == fPar->bCaenDebug || kTRUE == fPar->bCaenTriggerTime )
         fTDC[num].FillHistos(fOutputEvent->fMtdc[num], fPar->bCaenDebug, fPar->bCaenTriggerTime);
      else
      {
         fTDC[num].FillHistos(fOutputEvent->fMtdc[num]);
         if( CAEN1290_DEBUG )
            fTDC[num].FillDebugHistos(fOutputEvent->fMtdc[num]);
      }
   }

   fOutputEvent->SetValid(kTRUE);
} 

int T1290Data::FindGeo(int* pdata, unsigned len)
{
   for (unsigned iCha = 0; iCha < len; iCha++) {
      Int_t data32 = pdata[iCha];
      Int_t type_word = (data32 >> 27) & 0x1f; //data type
      if (type_word==8) return (data32 & 0x1f);
   }

   return 0;
}

void T1290Data::Unpack(int* pdata, unsigned len, int expected_geo)
{
   if (len<2) {
      printf("Error unpack V1290 data - too few messages = %u\n", len);
   }

   for (int n=0;n<NumChan;n++) {
      hit_lead[n] = 0;
      hit_trail[n] = 0;
   }

   Int_t geo(0);

   for (unsigned iCha = 0; iCha < len; iCha++) {
      Int_t data32 = pdata[iCha];
      Int_t type_word = (data32 >> 27) & 0x1f; //data type

//      printf("Msg:%2u Type:%d ", iCha, type_word);

      switch (type_word)  {
         case 8: {  //global_header
            // Int_t event_counter = (data32 & 0x07ffffff) >> 5;
            geo = data32 & 0x1f;
//            printf("Found GEO %d", geo);

            // SL: check that geo corresponds to expected number
            if ((expected_geo>0) && (expected_geo!=geo))
               printf("GEO mismatch, expected %d, is %d\n", expected_geo, geo);

            break;
         }
         case 16: { //Global trailer
            // Int_t word_count = (data32 & 0x001fffe0) >> 5;

            CalcWidth();

            static int errcnt = 0;

            if (iCha+1!=len) {
               if (errcnt++<3)
                  printf("Wrong V1290 data format - more messages after globar trailer\n");
               return;
            }

            break;
         }
         case 1: { //TDC Header
            // Int_t tdc = (data32 >> 24) & 0x03;
            // Int_t event_id = (data32 & 0x00fff000) >> 12;
            // Int_t bunch_id = data32 &0x00000fff;
            break;
         }
         case 3: { //TDC Trailer
            // Int_t tdc = (data32 & 0x03000000) >> 24;
            // Int_t event_id = (data32 & 0x00fff000) >> 12;
            // Int_t word_count = data32 &0x00000fff;
            break;
         }
         case 4: {  //TDC Error
            printf("??TDC ERROR  GEO = %d??\n", geo);
            break;
         }
         case 0: {  //TDC Data
            Int_t TDC_input = (data32 >> 21) & 0x1f;
            Int_t ch_data = data32 & 0x1fffff;
            Int_t front = (data32 >> 26) & 0x1;

            if (front==0) {
               if (hit_lead[TDC_input] == 0) {
                  lead[TDC_input] = ch_data;
               }
               if( hit_lead[ TDC_input] <MaxHits )
                  lead_multi[TDC_input][ hit_lead[ TDC_input] ] = ch_data;
               hit_lead[TDC_input] ++;

            } else {
               if (hit_trail[TDC_input] == 0) {
                  trail[TDC_input] = ch_data;
               }
               if(hit_trail[ TDC_input] <MaxHits )
                  trail_multi[TDC_input][hit_trail[ TDC_input] ] = ch_data;
               hit_trail[TDC_input] ++;
            }
            break;
         }
         case 17: { //Global trigger Time tag
            extended_trigger_time = data32 & 0x7ffffff;
            break;
         }
         default: {
            printf("GEO:%d Unknown tag %d\n", geo, type_word);
            break;
         }
      }
//      printf("\n");
   }
}
