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


	// get the track of the current step
	G4Track* track = aStep->GetTrack(); 

	// get the name of the particle on track
	G4String ParticleName = track->GetDynamicParticle()-> 
		GetParticleDefinition()->GetParticleName(); 


	// collect energy and track length step by step
	G4double edep  = 0;
	G4double stepl = 0;

	if (ParticleName == "mu+" &&  volume == fDetector->GetScint() ) {
		edep  = aStep->GetTotalEnergyDeposit();
		stepl = aStep->GetStepLength();
	};



	// define a vector of secondaries
	const std::vector<const G4Track*>* secondaries = 
		aStep->GetSecondaryInCurrentStep(); 




	// get the number of photon generated in the current step	
	G4int nphot = 0;
	if (ParticleName == "mu+" && secondaries->size()>0) {nphot = secondaries->size();};



	fEventAction->AddScint(edep,stepl,nphot);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
