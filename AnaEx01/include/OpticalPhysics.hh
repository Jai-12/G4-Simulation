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
#ifndef OpticalPhysics_h
#define OpticalPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"



 #include "G4Cerenkov.hh"
 #include "G4Scintillation.hh"
 #include "G4OpAbsorption.hh"
 #include "G4OpRayleigh.hh"
 #include "G4OpMieHG.hh"
 #include "G4OpBoundaryProcess.hh"



class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;



class OpticalPhysics : public G4VPhysicsConstructor
{
	public:

		OpticalPhysics(const G4String& name = "optical");
		virtual ~OpticalPhysics();

		// This method will be invoked in the Construct() method.
		// each particle type will be instantiated
		virtual void ConstructParticle();

		// This method will be invoked in the Construct() method.
		// each physics process will be instantiated and
		// registered to the process manager of each particle type
		virtual void ConstructProcess();


	private:

		static G4int fMaxNumPhotonStep;

		static G4Cerenkov* 	    fCerenkovProcess;
		static G4Scintillation*     fScintillationProcess;
		static G4OpAbsorption*      fAbsorptionProcess;
		static G4OpRayleigh*   	    fRayleighScatteringProcess;
		static G4OpMieHG*           fMieHGScatteringProcess;
		static G4OpBoundaryProcess* fBoundaryProcess;


};

#endif
