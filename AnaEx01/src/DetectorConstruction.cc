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

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
	:G4VUserDetectorConstruction(),fScintMaterial(0),
	fSolidWorld(0),fLogicWorld(0),fPhysiWorld(0)
{

	// parameter values of the world

	fWorldSizeX     = 500.*cm;
	fWorldSizeY     = 500.*cm;
	fWorldSizeZ     = 500.*cm;


	// parameter values of the scintillator

	fScintThickness = 1.  *cm;
	fScintSizeX     = 20. *cm;
	fScintSizeY     = 122.*cm;



	// materials
	DefineMaterials();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	return ConstructScint();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
	// use G4-NIST materials data base
	//
	G4NistManager* man = G4NistManager::Instance();
	fDefaultMaterial = man->FindOrBuildMaterial("G4_Galactic");
	fScintMaterial   = man->FindOrBuildMaterial("G4_Cu");//"G4_POLYSTYRENE");



	// print table
	//
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructScint()
{

	// Clean old geometry, if any
	//
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();

	//     
	// World
	//
	fSolidWorld = new G4Box("World",                                  //its name
			fWorldSizeX/2,fWorldSizeY/2,fWorldSizeZ/2);       //its size

	fLogicWorld = new G4LogicalVolume(fSolidWorld,                    //its solid
 			fDefaultMaterial,       		          //its material
 			"World");                		          //its name
 
	fPhysiWorld = new G4PVPlacement(0,                                //no rotation
			G4ThreeVector(),                                  //at (0,0,0)
			fLogicWorld,               	 	          //its logical volume
			"World",                                          //its name
			0,                                                //its mother  volume
			false,                                            //no boolean operation
			0);                                               //copy number

	//     
	// Scintillator
	//
	fSolidScint = new G4Box("Scintillator",                           //its name
			fScintSizeX/2, fScintSizeY/2, fScintThickness/2); //its size

	fLogicScint = new G4LogicalVolume(fSolidScint,                    //its solid
			fScintMaterial,                                   //its material
			"Scintillator");                                  //its name

	fPhysiScint = new G4PVPlacement(0,                                //no rotation
			G4ThreeVector(),                                  //at (0,0,0)
			fLogicScint,                                      //its logica volume
			"Scintillator",                                   //its name
			fLogicWorld,                                      //its mother volume
			false,                                            //no boolean operation
			0);                                               //copy number  


	//                                        
	// Visualization attributes
	//
	fLogicWorld->SetVisAttributes (G4VisAttributes::GetInvisible());

	G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	simpleBoxVisAtt->SetVisibility(true);

	//
	//always return the physical World
	//
	return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

