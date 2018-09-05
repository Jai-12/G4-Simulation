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

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"


class RunAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
	public:
		EventAction(RunAction*, HistoManager*);
		virtual ~EventAction();

		virtual void  BeginOfEventAction(const G4Event*);
		virtual void  EndOfEventAction(const G4Event*);

		void AddScint(G4double de, G4double dl, G4int dn1, G4int dn2, G4int dn3) {
						fEnergyScint += de; 
						fTrackLScint += dl;
						fPhotonNumberGen += dn1; 
						fPhotonNumberCol += dn2;
						fElectronsGenerated += dn3 ;    ;} 

		//		G4cout << "TOTAL_ELECTRONS: " << fElectronsGenerated << G4endl;	};

	private:
		RunAction*    fRunAct;
		HistoManager* fHistoManager;

		G4double p_particleX;
		G4double p_particleY;
		G4double p_particleTheta;
		G4double p_particlePhi;
		G4double p_particleEnergy;


//		G4double gamma;
//		G4double beta;


		G4double  myTrackLScint;    	//add comments
		G4double  myEnergyScint;  	//add comments

		G4int 	  fPhotonNumberGen;	
		G4int 	  fPhotonNumberCol;	

		G4int fElectronsGenerated;		

		G4double  fEnergyScint;
		G4double  fTrackLScint;

		G4int     fPrintModulo;        // still trying to understand the meaning of this variable...                      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


