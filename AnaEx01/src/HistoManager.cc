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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager(const G4bool enable)
	: fFactoryOn(false),
	  fEnabled(enable)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
	if(!fEnabled)
		return;
	// Create or get analysis manager
	// The choice of analysis technology is done via selection of a namespace
	// in HistoManager.hh
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetVerboseLevel(1);


	// Create directories 
	analysisManager->SetHistoDirectoryName("histo");


	// Open an output file
	//
	G4bool fileOpen = analysisManager->OpenFile("AnaEx01");
	if (! fileOpen) {
		G4cerr << "\n---> HistoManager::Book(): cannot open " 
			<< analysisManager->GetFileName() << G4endl;
		return;
	}

	// Create histograms.
	// Histogram ids are generated automatically starting from 0.
	// The start value can be changed by:
	// analysisManager->SetFirstHistoId(1);  

	// id = 0
	analysisManager->CreateH1("EScint","Edep in scintillator (MeV)", 100, 0., 10*MeV/MeV);
	// id = 1
	analysisManager->CreateH1("LScint","trackL in scintillator (cm)", 100, 0., 3.5*cm/cm);
	// id = 2
	analysisManager->CreateH1("myLScint","mytrackL in scintillator (cm)", 100, 0., 3.5*cm/cm);
	// id = 3
	analysisManager->CreateH1("photonsAbsorbed","photons_absorbed", 100, 130., 260);
	// id = 4
	analysisManager->CreateH1("photonsGenerated","photons_generated", 100, 410, 655 );
	// id = 5
	analysisManager->CreateH1("photonsCollectedPMT1","photons_collectedPMT1", 150, 95, 250);
	// id = 6
	analysisManager->CreateH1("Initial Photon Energy","Initial Photon Energy", 100, 1, 5*eV/eV);
	// id = 7
	analysisManager->CreateH1("Photon time at photocatode","photon time at photocatode", 200, 0, 30*ns/ns);
	// id = 8
	analysisManager->CreateH1("Modified Photon energy spectrum","Modified Photon energy spectrum", 100, 1,5*eV/eV);
	// id = 9 
	analysisManager->CreateH1("photons track length","photons track length", 100, 0,1200*cm/cm);
	// id = 10
	analysisManager->CreateH1("photonsCollectedPMT2","photons_collectedPMT2", 150, 95, 250);
	// id = 11
	analysisManager->CreateH1("Final Electrons Generated_A","final electrons generated_A", 100, 0, 1e12);
	// id = 12
	analysisManager->CreateH1("Final Electrons Generated_B","final electrons generated_B", 100, 0, 1e12);














	fFactoryOn = true;       

	G4cout << "\n----> Output file is open in " 
		<< analysisManager->GetFileName() << "." 
		<< analysisManager->GetFileType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Save()
{
	if(!fEnabled)
		return;
	if (! fFactoryOn) return;

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();    
	analysisManager->Write();
	analysisManager->CloseFile(); 

	G4cout << "\n----> Histograms are saved\n" << G4endl;

	delete G4AnalysisManager::Instance();
	fFactoryOn = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
	if(!fEnabled)
		return;
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
	analysisManager->FillH1(ih, xbin, weight);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
	if(!fEnabled)
		return;
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
	G4H1* h1 = analysisManager->GetH1(ih);
	if (h1) h1->scale(fac);
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
