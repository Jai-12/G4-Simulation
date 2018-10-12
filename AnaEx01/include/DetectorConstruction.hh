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

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Trd;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
	public:

		DetectorConstruction();
		virtual ~DetectorConstruction();

	public:

		virtual G4VPhysicalVolume* Construct();

	public:



		//		G4double GetWorldSizeX()           {return fWorldSizeX;}; 
		//		G4double GetWorldSizeY()           {return fWorldSizeY;};
		//		G4double GetWorldSizeZ()           {return fWorldSizeZ;};


		const G4VPhysicalVolume* GetWorld()  	   {return fPhysiWorld;};           
		const G4VPhysicalVolume* GetScint()        {return fPhysiScint;};
		const G4VPhysicalVolume* GetPMT()          {return fPhysiPMT;};
		const G4VPhysicalVolume* GetPMT1()         {return fPhysiPMT1;};
		const G4VPhysicalVolume* GetWrapping()     {return fPhysiWrapping;};



	private:
//world
		G4Material*        fWorldMaterial;
		G4double           fWorldSizeX;
		G4double           fWorldSizeY;
		G4double  	   fWorldSizeZ;

//scintillator
		G4Material* 	   fScintMaterial;  
		G4double           fScintThickness;
		G4double           fScintSizeX;
		G4double           fScintSizeY;
//wrapping
	        G4Material*        fWrappingMaterial;
		G4double	   fWrappingThickness;
//PMT
		G4Material*        fPMTMaterial;
		G4double           fPMTThickness;
		G4double           fPMTSizeX;
		G4double           fPMTSizeY;



		G4Box*             fSolidWorld;    //pointer to the solid World 
		G4LogicalVolume*   fLogicWorld;    //pointer to the logical World
		G4VPhysicalVolume* fPhysiWorld;    //pointer to the physical World


		G4Box*             fSolidScint;    //pointer to the solid Scintillator
		G4LogicalVolume*   fLogicScint;    //pointer to the logical Scintillator
		G4VPhysicalVolume* fPhysiScint;    //pointer to the physical Scintillator

		G4VPhysicalVolume* fPhysiWrapping;    //pointer to the physical wrapping

		G4Box*             fSolidPMT1;    //pointer to the solid PTM 1
		G4LogicalVolume*   fLogicPMT1;    //pointer to the logical PMT 1
		G4VPhysicalVolume* fPhysiPMT1;    //pointer to the physical PMT 1

		G4Box*             fSolidPMT;    //pointer to the solid PTM 2
		G4LogicalVolume*   fLogicPMT;    //pointer to the logical PMT 2
		G4VPhysicalVolume* fPhysiPMT;    //pointer to the physical PMT 



	private:

		void DefineMaterials();
		G4VPhysicalVolume* ConstructScint();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

