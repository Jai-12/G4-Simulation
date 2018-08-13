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

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det,
		EventAction* evt)
	: G4UserSteppingAction(), 
	fDetector(det), fEventAction(evt)                                         
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
	// get volume of the current step
	G4VPhysicalVolume* volume 
		= aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

	
	// collect energy and track length step by step
	G4double edep  = aStep->GetTotalEnergyDeposit();
	G4double stepl = aStep->GetStepLength();

	// count scintillation and cherenkov photons separately

	G4int	fScintillationCounter = 0; 
	G4int	fCerenkovCounter = 0; 
	
	G4Track* track = aStep->GetTrack(); 

	G4String ParticleName = track->GetDynamicParticle()-> 
		GetParticleDefinition()->GetParticleName(); 

	const std::vector<const G4Track*>* secondaries = 
		aStep->GetSecondaryInCurrentStep(); 

	if (ParticleName != "opticalphoton" && secondaries->size()>0) { 
		for(unsigned int i=0; i<secondaries->size(); ++i) { 
			if (secondaries->at(i)->GetParentID()>0) { 
				if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition() 
						== G4OpticalPhoton::OpticalPhotonDefinition()){ 
					if (secondaries->at(i)->GetCreatorProcess()->GetProcessName() 
							== "Scintillation")fScintillationCounter++; 
					if (secondaries->at(i)->GetCreatorProcess()->GetProcessName() 
							== "Cerenkov")fCerenkovCounter++; 
				} 
			} 
		} 
	} 




	// sum scintillation and cherenkov photons	
	G4int nphot = fScintillationCounter + fCerenkovCounter; 
	// add energy dep, legth and photon numbers for this step
	if (volume == fDetector->GetScint()) fEventAction->AddScint(edep,stepl,nphot);




}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
