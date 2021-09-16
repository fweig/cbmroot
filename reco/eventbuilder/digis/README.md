Instructions for Event Building
=========================================

The following is a minimal example with which raw event building 
from digis can be carried out inside a Root macro: 

	// Interface to framework
	FairRunAna* run = new FairRunAna();

	// Create the task class
	CbmTaskBuildRawEvents* evBuildRaw = new CbmTaskBuildRawEvents();

	// Choose between NoOverlap, MergeOverlap, AllowOverlap modes
	evBuildRaw->SetEventOverlapMode(EOverlapModeRaw::AllowOverlap);

	// Remove detectors where digis not found
	if (!useRich) evBuildRaw->RemoveDetector(kRawEventBuilderDetRich);
	if (!useMuch) evBuildRaw->RemoveDetector(kRawEventBuilderDetMuch);
	....

	// Set reference detector (example STS)
	evBuildRaw->SetReferenceDetector(kRawEventBuilderDetSts);

	// Set timeslice parameters (StartTime, Length, OverlapLength)
	evBuildRaw->SetTsParameters(0.0, 1.e7, 0.0);

	// Use CbmMuchDigi instead of CbmMuchBeamtimeDigi
	evBuildRaw->ChangeMuchBeamtimeDigiFlag(kFALSE);

	// Apply cuts 
	// (minimum and maximum number of digis per detector for events)
	evBuildRaw->SetTriggerMinNumber(ECbmModuleId::kSts, 1000);
	evBuildRaw->SetTriggerMaxNumber(ECbmModuleId::kSts, -1);

	// Set time window for seed detector and others
	evBuildRaw->SetTriggerWindow(ECbmModuleId::kSts, -500, 500);
	...
		
	// Pass eventbuilder to framework
	run->AddTask(evBuildRaw);

	// Add QA task in case of Monte Carlo data
	if ( useMC ) {
	   CbmBuildEventsQa* evBuildQA = new CbmBuildEventsQa();
	   run->AddTask(evBuildQA);
	}

To use the sliding-window seed finder instead of a reference detector,
the following lines should be included:

	// Set the reference detector to "undefined" 
	// (disables the default setting)
	evBuildRaw->SetReferenceDetector(kRawEventBuilderDetUndef);

	// Activate sliding window seed builder and set parameters
	// (minDigis, WinDuration, DeadTime)
	evBuildRaw->SetSlidingWindowSeedFinder(1000, 500, 500);

	// Add a detector on which the seed finder will operate
	// (example STS)
	evBuildRaw->AddSeedTimeFillerToList(kRawEventBuilderDetSts);

Multiple detectors can be added to the seed finder by repeatedly calling
"AddSeedTimeFillerToList()" with different detectors. By default, the
digi times of all detectors are then combined into a single time-sorted 
array of digi timestamps before applying the seed finder. 

QA for seed finding can be activated by adding the line:

	evBuildRaw->SetSeedFinderQa(kTRUE);

This requires the presence of CbmMatch data (from Monte Carlo) in the
input tree and will output diagnostics concerning the contamination of event 
seeds with digis from other events and with noise digis. 

For more information about the event builder and seed finder see the
following documents:

https://indico.gsi.de/event/13111/contributions/55683/attachments/36768/49064/Smith_Seedfinding_26Aug2021.pdf

https://indico.gsi.de/event/12062/contributions/51740/attachments/34945/45841/Smith_CBM_Meeting_Eventbuilding.pdf
