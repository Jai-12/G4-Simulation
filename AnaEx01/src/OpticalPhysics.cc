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
#include "OpticalPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>


#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"

// G4int OpticalPhysics::fVerboseLevel = 1;
  G4int OpticalPhysics::fMaxNumPhotonStep = 20;
  G4Cerenkov* OpticalPhysics::fCerenkovProcess = 0;
  G4Scintillation* OpticalPhysics::fScintillationProcess = 0;
  G4OpAbsorption* OpticalPhysics::fAbsorptionProcess = 0;
  G4OpRayleigh* OpticalPhysics::fRayleighScatteringProcess = 0;
  G4OpMieHG* OpticalPhysics::fMieHGScatteringProcess = 0;
  G4OpBoundaryProcess* OpticalPhysics::fBoundaryProcess = 0;



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpticalPhysics::OpticalPhysics(const G4String& name)
	:  G4VPhysicsConstructor(name) {}

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	OpticalPhysics::~OpticalPhysics() {
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

void OpticalPhysics::ConstructParticle()
{




}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpticalPhysics::ConstructProcess()
{
	fCerenkovProcess = new G4Cerenkov("Cerenkov");
	fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
	fCerenkovProcess->SetMaxBetaChangePerStep(10.0);
	fCerenkovProcess->SetTrackSecondariesFirst(true);
	
	fScintillationProcess = new G4Scintillation("Scintillation");
	fScintillationProcess->SetScintillationYieldFactor(1.);
	fScintillationProcess->SetTrackSecondariesFirst(true);
	
	fAbsorptionProcess = new G4OpAbsorption();
	
	fRayleighScatteringProcess = new G4OpRayleigh();
	
	fMieHGScatteringProcess = new G4OpMieHG();
	
	fBoundaryProcess = new G4OpBoundaryProcess();

//	fCerenkovProcess->SetVerboseLevel(fVerboseLevel);
//	fScintillationProcess->SetVerboseLevel(fVerboseLevel);
//	fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
//	fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
//	fMieHGScatteringProcess->SetVerboseLevel(fVerboseLevel);
//	fBoundaryProcess->SetVerboseLevel(fVerboseLevel);

	// Use Birks Correction in the Scintillation process    ??????????
	//	if(G4Threading::IsMasterThread())
	//	{
	//			G4EmSaturation* emSaturation =
	//			G4LossTableManager::Instance()->EmSaturation();
	//		fScintillationProcess->AddSaturation(emSaturation);
	//	}

	auto particleIterator=GetParticleIterator();
	particleIterator->reset();
	while( (*particleIterator)() ){
		G4ParticleDefinition* particle = particleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();
		if (fCerenkovProcess->IsApplicable(*particle)) {      // condition for the Cerenkov process:
			pmanager->AddProcess(fCerenkovProcess);	      //     Charged and not short-lived particles
			pmanager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
		}
		if (fScintillationProcess->IsApplicable(*particle)) { // condition for the Scintillation process:
			pmanager->AddProcess(fScintillationProcess);  //     Charged and not shorted-lived particles
			pmanager->SetProcessOrderingToLast(fScintillationProcess, idxAtRest);
			pmanager->SetProcessOrderingToLast(fScintillationProcess, idxPostStep);
		}
		if (particleName == "opticalphoton") {                // condition for discrete processes
			G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
			pmanager->AddDiscreteProcess(fAbsorptionProcess);
			pmanager->AddDiscreteProcess(fRayleighScatteringProcess);
			pmanager->AddDiscreteProcess(fMieHGScatteringProcess);
			pmanager->AddDiscreteProcess(fBoundaryProcess);
		}
	}






}
