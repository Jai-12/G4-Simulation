//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "HistoManager.hh"


#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include <map>
#include <CLHEP/Random/Randomize.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/RandBit.h>
#include <CLHEP/Random/RandGamma.h>
#include <CLHEP/Random/RandGaussQ.h>
#include <CLHEP/Random/RandPoissonQ.h>
#include <CLHEP/Random/RandExponential.h>
#include <CLHEP/Random/RandGeneral.h>




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det,
		EventAction* evt,  HistoManager* histo)
	: G4UserSteppingAction(), 
	fDetector(det), fEventAction(evt), fHistoManager(histo)

{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4double SteppingAction::myrand(){                      // Second uniform  distribution (from 0 to 1) 

        //generate a pseudo random number in the interval [0;1]
        G4double r;
         r =  (   CLHEP::HepRandom::getTheEngine()->flat() );
        //cout<<"  myrandpos: "<<r<<endl;
        return r;
 }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	// get the volume where the particle is located
	//     at the beginning of the current step
	G4VPhysicalVolume* init_volume
		= aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();


	// get the volume where the particle is located
	//     at the end of the current step
	G4VPhysicalVolume* final_volume
		= aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();



	// get the track of the current step
	G4Track* track = aStep->GetTrack(); 

	// get the name of the particle on track
	G4String ParticleName = track->GetDynamicParticle()-> 
		GetParticleDefinition()->GetParticleName(); 


//****************** COLLECT ENERY AND TRACK LENGTH OF MUONS STEP BY STEP *******************
	
	G4double edep  = 0;
	G4double stepl = 0;

/*	if (ParticleName == "mu+" &&  init_volume == fDetector->GetScint()) {
		edep  = aStep->GetTotalEnergyDeposit();
		stepl = aStep->GetStepLength();
	};
*/
//*************************** COMPUTING INITIAL PHOTON ENERGY and PHOTON NUMBER  *************************
G4int photons_generated = 0;
	G4double photon_energy = 0;

	{
		// define a vector of secondaries
		const std::vector<const G4Track*>* secondaries = 
			aStep->GetSecondaryInCurrentStep(); 

		// Actually here I am assuming that all the secondaries generated from muon are 
		//		optical photons (while they might be electrons)
		// For instance I am negletting all the optical photons generated from secondary ionizations
		// This is done in order to reduce the computing time.
		
		if (secondaries->size()>0) {
			for(unsigned int i=0; i<secondaries->size(); ++i) {
				if (secondaries->at(i)->GetParentID()>0) {
					if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition()
							== G4OpticalPhoton::OpticalPhotonDefinition()){
						if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
								== "Scintillation"){photons_generated++;
							photon_energy = secondaries->at(i)->GetTotalEnergy();
				//			fHistoManager->FillHisto(6,photon_energy/eV);

						}
						if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
								== "Cerenkov"){photons_generated++;

							photon_energy = secondaries->at(i)->GetTotalEnergy();
				//			fHistoManager->FillHisto(6,photon_energy/eV);

						}
					}   
				}
			}
		}


	}
//////////////////  COMPUTING THE TRACK LENGTH of PHOTONS  /////////////////////////
	G4int id = aStep->GetTrack()->GetTrackID(); 
	G4int status = aStep->GetTrack()->GetTrackStatus();  
	G4double photo_stepl = aStep->GetStepLength();

	if (ParticleName == "opticalphoton" ){
		//sum all steps lengths of the same track 
		photons_map[id] += photo_stepl; 

		//fill the histo only if the photon is absorbed in the scintillator 
		if(status==2  && final_volume == fDetector->GetScint())  {  //status = 2 means photon absorbed

			fHistoManager->FillHisto(9,photons_map[id]/cm);
		};

	};

//***  COUNTING PHOTONS COLLECTED BY THE PHOTOCATHODE, THEIR ARRIVAL TIME, and THE ELECTRONS PRODUCED)****
/////////////////////                      PMT 1                            ///////////////////////	


	
	/// ******************G4TrackStatus LEGEND***********************
     // Case 0: Continue the tracking
     // Case 1: Invoke active rest physics processes and
     //   and kill the current track afterward
     // Case 2: Kill the current track


     // Case 3: Kill the current track and also associated
     //    secondaries.
     // Case 4: Suspend the current track

     // Case 5: Postpones the tracking of thecurrent track 
     // to the next event.
	
       n_electrons_PMT1 = 0;
       photons_collected_PMT1 = 0;
	
	if (ParticleName == "opticalphoton" &&
	        final_volume == fDetector->GetPMT1()  &&  
		aStep->GetTrack()->GetTrackStatus()==2) // "2" means track killed
	{

		 G4double ph_energy = aStep->GetTrack()->GetTotalEnergy();
                 G4double q_eff = 0;
                 if(ph_energy > 0.06640*eV && ph_energy < 9.5*eV) q_eff=1;//real photocathode range: 2.1*eV -4.1*eV
                 if(myrand() < q_eff){ n_electrons_PMT1 = 1 ;
  //                       fHistoManager->FillHisto(8,ph_energy/eV);
                 };
 

		
//		fHistoManager->FillHisto(7,aStep->GetTrack()->GetGlobalTime()/ns);
		photons_collected_PMT1 = 1;		
	};


//***  COUNTING PHOTONS COLLECTED BY THE PHOTOCATHODE, THEIR ARRIVAL TIME, and THE ELECTRONS PRODUCED)****
/////////////////////                      PMT 2                            ///////////////////////	
	
       n_electrons_PMT2 = 0;
       photons_collected_PMT2 = 0;
	
	if (ParticleName == "opticalphoton" &&
	        final_volume == fDetector->GetPMT()  &&  
		aStep->GetTrack()->GetTrackStatus()==2) // "2" means track killed
	{

		 G4double ph_energy = aStep->GetTrack()->GetTotalEnergy();
                 G4double q_eff = 0;

	         fHistoManager->FillHisto(6,ph_energy/eV);


                 if(ph_energy > 0.06640*eV && ph_energy < 3*eV) q_eff=0.3;//real photocathode range: 2.1*eV -4.1*eV
                 if(myrand() < q_eff){ n_electrons_PMT2 = 1 ;
                         fHistoManager->FillHisto(8,ph_energy/eV);
                 };
 

		
		fHistoManager->FillHisto(7,aStep->GetTrack()->GetGlobalTime()/ns);
		photons_collected_PMT2 = 1;		
	};



//////////////////// ADDING QUANTITIES of THE CURRENT STEP  //////////////////////////
//G4cout << " ELECTRONS: " <<n_electrons << G4endl;
	fEventAction->AddScint(edep,stepl,photons_generated, photons_collected_PMT2,n_electrons_PMT2);

	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


