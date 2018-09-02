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



 G4double SteppingAction::random_quantum_eff(){                   //
         G4double x;
         G4double f;
 
         do {
                 x = (CLHEP::pi/2.) * myrand();
                 f = (3.*sqrt(3.)/4) * myrand();
         }while(f > (4)*cos(x)*cos(x)*cos(x)*sin(x));
 
         return x;
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


	// collect energy and track length step by step
	G4double edep  = 0;
	G4double stepl = 0;

	if (ParticleName == "opticalphoton" &&  init_volume == fDetector->GetScint()) {
		edep  = aStep->GetTotalEnergyDeposit();
		stepl = aStep->GetStepLength();
	};


	// count the optical photons that exit the Scintillator 
	// ATTENTION: in the DetectorConstruction class you have to set the world sizes correctly to make it work 	
	G4int photons_collected = 0;		
	if (ParticleName == "opticalphoton" //&& !( 
		/*     		  aStep->GetPostStepPoint()->GetPosition().getZ()<0.5*cm
				  &&  aStep->GetPostStepPoint()->GetPosition().getZ()>-0.5*cm
				  &&  aStep->GetPostStepPoint()->GetPosition().getX()<10*cm
				  &&  aStep->GetPostStepPoint()->GetPosition().getX()>-10*cm
				  &&  aStep->GetPostStepPoint()->GetPosition().getY()<61*cm
				  &&  aStep->GetPostStepPoint()->GetPosition().getY()>-61*cm) 

				  && (  (aStep->GetPreStepPoint()->GetPosition().getZ()==0.5*cm
				  ||  aStep->GetPreStepPoint()->GetPosition().getZ()==-0.5*cm ) && (
				  aStep->GetPreStepPoint()->GetPosition().getX()==10*cm
				  ||  aStep->GetPreStepPoint()->GetPosition().getX()==-10*cm ) && (
				  aStep->GetPreStepPoint()->GetPosition().getY()==61*cm 
				  ||  aStep->GetPreStepPoint()->GetPosition().getY()==-61*cm )
				  )	*/
			//&& init_volume == fDetector->GetScint() 
			&&   final_volume == fDetector->GetPMT()  &&   aStep->GetTrack()->GetTrackStatus()==2) // "2" means track killed


	{
		//G4cout << "finSPECTRUM: "<< aStep->GetTrack()->GetTotalEnergy()/eV <<  G4endl;
		G4cout << "MY_ID  " << aStep->GetTrack()->GetTrackID() << "  " << aStep->GetTrack()->GetTrackStatus() <<G4endl;	
		fHistoManager->FillHisto(7,aStep->GetTrack()->GetGlobalTime()/ns);


		photons_collected = 1;		
	};


	G4int photons_generated = 0;
	G4double photon_energy = 0;

	if(1/*init_volume == fDetector->GetScint()*/)


	{

		// define a vector of secondaries
		const std::vector<const G4Track*>* secondaries = 
			aStep->GetSecondaryInCurrentStep(); 




		// get the number of photon generated in the current step	
		// I CAN DO IT LIKE THIS:

		// Actually here I am assuming that all the secondaries generated from muon are optical photons
		// For instance I am negletting all the optical photons generated from secondary ionizations
		// This is mainly done in order to reduce the computing time   
		//	if (/*ParticleName == "mu+" &&*/ secondaries->size()>0) {photons_generated = secondaries->size();};

		// OR I CAN DO IT THIS WA
		if (secondaries->size()>0) {
			for(unsigned int i=0; i<secondaries->size(); ++i) {
				if (secondaries->at(i)->GetParentID()>0) {
					if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition()
							== G4OpticalPhoton::OpticalPhotonDefinition()){
						if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
								== "Scintillation"){photons_generated++;
							photon_energy = secondaries->at(i)->GetTotalEnergy();
							fHistoManager->FillHisto(6,photon_energy/eV);

						}
						if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
								== "Cerenkov"){photons_generated++;

							photon_energy = secondaries->at(i)->GetTotalEnergy();
							fHistoManager->FillHisto(6,photon_energy/eV);

						}
					}   
				}
			}
		}


	}


	G4int id = aStep->GetTrack()->GetTrackID(); 
	G4int status = aStep->GetTrack()->GetTrackStatus(); 
	G4double photo_stepl = aStep->GetStepLength();

	if (ParticleName == "opticalphoton" ){

		photons_map[id] = photons_map[id] + photo_stepl;

		if(status==2  && final_volume == fDetector->GetScint()) 

		{

			//photons_map[id/cm]=random_quantum_eff();
			fHistoManager->FillHisto(8,photons_map[id]/cm);
		};

	};



	fEventAction->AddScint(edep,stepl,photons_generated, photons_collected);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
