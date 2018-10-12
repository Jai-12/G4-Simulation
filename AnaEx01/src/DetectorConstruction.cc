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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4MaterialPropertyVector.hh"

#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"


#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"


#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4ThreeVector.hh"

DetectorConstruction::DetectorConstruction()
	:G4VUserDetectorConstruction(),fScintMaterial(0),
	fSolidWorld(0),fLogicWorld(0),fPhysiWorld(0)
{

	// parameter values of the world

	fWorldSizeZ     =1.*cm   + 1*micrometer;
	fWorldSizeX     =20.*cm  + 1*micrometer;
	fWorldSizeY     =122.*cm + 1*micrometer;


	// parameter values of the scintillator

	fScintThickness = 1. *cm;
	fScintSizeX     = 20.  *cm;
	fScintSizeY     = 122.  *cm;

	// parameter values of the wrapping
	fWrappingThickness = 50*cm ;


	// parameter values of the PMT
	PMT_dxa = 6*cm;
	PMT_dxb = fScintThickness;
	PMT_dyb = fScintSizeX ;
	PMT_dya = 6*cm; 
	PMT_dz  = 16*cm;


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
	fWorldMaterial 	 = man->FindOrBuildMaterial("G4_Galactic");
	fScintMaterial   = man->FindOrBuildMaterial("G4_POLYSTYRENE");
	fWrappingMaterial= man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	fPMTMaterial     = man->FindOrBuildMaterial("G4_GLASS_LEAD");


          //maximum at 418 nm = 3eV
         const G4int NUMENTRIES = 9;
         G4double photonEnergy[NUMENTRIES] = { 6.6*eV, 6.7*eV, 6.8*eV, 6.9*eV,
                 7.0*eV, 7.1*eV, 7.2*eV, 7.3*eV, 7.4*eV };
         G4double fScintMaterial_FAST[NUMENTRIES] = { 0.000134, 0.004432,
                 0.053991, 0.241971,
                 0.398942, 0.000134,
                 0.004432, 0.053991,
                 0.241971 };
 //      G4double fScintMaterial_SLOW[NUMENTRIES] = { 0.000010, 0.000020,
 //              0.000030, 0.004000,
 //              0.008000, 0.005000,
 //              0.020000, 0.001000,
 //              0.000010 };
 
 

	//***Elements

	G4Element * fC = man->FindOrBuildElement("C");
	G4Element * fH = man->FindOrBuildElement("H");



	//Polystyrene
	//   fScintMaterial = new G4Material("Polystyrene", 1.03*g/cm3, 2);
	//  fScintMaterial->AddElement(fC, 8);
	//  fScintMaterial->AddElement(fH, 8);


	G4MaterialPropertiesTable* fScintMaterial_MPT = new G4MaterialPropertiesTable();
	fScintMaterial_MPT->AddProperty("FASTCOMPONENT", photonEnergy, fScintMaterial_FAST, NUMENTRIES);
//	fScintMaterial_MPT->AddProperty("SLOWCOMPONENT", photonEnergy, fScintMaterial_SLOW, NUMENTRIES);

	// 100 photons per eV (plastic scintillator according to the "Techniques" book ) 
	fScintMaterial_MPT->AddConstProperty("SCINTILLATIONYIELD", 100000./MeV); //10000 

	fScintMaterial_MPT->AddConstProperty("RESOLUTIONSCALE", 2.0);
	fScintMaterial_MPT->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
	fScintMaterial_MPT->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
	fScintMaterial_MPT->AddConstProperty("YIELDRATIO", 0.8);
	fScintMaterial->SetMaterialPropertiesTable(fScintMaterial_MPT);


	G4double refractiveIndex1[] ={1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6,
		1.6
	};
	G4double absorption[] = {100*cm, 100*cm, 100*cm, 100*cm, 100*cm, 100*cm, 100*cm, 100*cm,
		100*cm

	};//*cm0*cm




	fScintMaterial_MPT->AddProperty("RINDEX",       photonEnergy, refractiveIndex1, NUMENTRIES);
	fScintMaterial_MPT->AddProperty("ABSLENGTH",    photonEnergy, absorption,     NUMENTRIES);



	///// glass lead 
	G4double refractiveIndex4[] ={1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6,
		1.6
	};
	G4double absorption4[] = {0.1*cm, 0.1*cm, 0.1*cm, 0.1*cm, 0.1*cm, 0.1*cm, 0.1*cm, 0.1*cm,
		0.1*cm};

	G4MaterialPropertiesTable* fPMTMaterial_MPT = new G4MaterialPropertiesTable();
	fPMTMaterial_MPT->AddProperty("RINDEX",   photonEnergy, refractiveIndex4, NUMENTRIES);
	fPMTMaterial_MPT->AddProperty("ABSLENGTH",    photonEnergy, absorption4,     NUMENTRIES);
	fPMTMaterial->SetMaterialPropertiesTable(fPMTMaterial_MPT);


	/// G4_galatic 

	G4MaterialPropertiesTable* fWorldMaterial_MPT = new G4MaterialPropertiesTable();
	G4double refractiveIndex3[] ={1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
		1.0
	};


	//	G4double absorption3[] ={0.01*nanometer, 0.01*nanometer};  //?? 


	fWorldMaterial_MPT->AddProperty("RINDEX",       photonEnergy, refractiveIndex3, NUMENTRIES);
	//	fWorldMaterial_MPT->AddProperty("ABSLENGTH",    photonEnergy, absorption2,     NUMENTRIES);

	fWorldMaterial->SetMaterialPropertiesTable(fWorldMaterial_MPT);





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
			fWorldMaterial,       		          //its material
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

	/*	//Wrapping (boolean solid)
		G4Box* box1 =
		new G4Box("Box1", (fScintSizeX+fWrappingThickness)/2,
		(fScintSizeY)/2, 
		(fScintThickness+fWrappingThickness)/2);
		G4Box* box2 =
		new G4Box("Box2", fScintSizeX/2, (fScintSizeY*2)/2, fScintThickness/2);
		G4SubtractionSolid* fSolidWrapping =
		new G4SubtractionSolid("Box1-Box2", box1, box2);


		G4LogicalVolume * fLogicWrapping = new G4LogicalVolume(fSolidWrapping,                    //its solid
		fWrappingMaterial,                                   //its material
		"Wrapping");                                  //its name

		G4PVPlacement * fPhysiWrapping = new G4PVPlacement(0,                                //no rotation
		G4ThreeVector(),                                  //at (0,0,0)
		fLogicWrapping,                                      //its logica volume
		"Wrapping",                                   //its name
		fLogicScint,                                      //its mother volume
		false,                                            //no boolean operation
		0);                                               //copy number  
	 */

//     
	// Shape 1 (PMT 1)
	//

	//rotation section
	G4ThreeVector pos1 = G4ThreeVector(0, +69*cm, 0);
	G4RotationMatrix rotm1  = G4RotationMatrix();
	rotm1.rotateY(270*deg);
	rotm1.rotateZ(90*deg);
	G4Transform3D transform1 = G4Transform3D(rotm1,pos1);


	// Trapezoid shape       
	fSolidPMT1 = new G4Trd("PMT",                      //its name
			0.5*PMT_dxa, 0.5*PMT_dxb,
			0.5*PMT_dya, 0.5*PMT_dyb, 0.5*PMT_dz); //its size

	fLogicPMT1 = new G4LogicalVolume(fSolidPMT1,         //its solid
			fPMTMaterial,          //its material
			"PMT");           //its name

	fPhysiPMT1 = new G4PVPlacement(transform1,      // rotation
			fLogicPMT1,             //its logical volume
			"PMT",                //its name
			fLogicScint,                //its mother  volume
			false,                   //no boolean operation
			0,
			false) ;                      //copy number


 
	//     
	// Shape 2 (PMT 2)
	//

	//rotation section
	G4ThreeVector pos2 = G4ThreeVector(0, -69*cm, 0);
	G4RotationMatrix rotm  = G4RotationMatrix();
	rotm.rotateY(90*deg);
	rotm.rotateZ(90*deg);
	G4Transform3D transform = G4Transform3D(rotm,pos2);


	// Trapezoid shape       
	fSolidPMT = new G4Trd("PMT",                      //its name
			0.5*PMT_dxa, 0.5*PMT_dxb,
			0.5*PMT_dya, 0.5*PMT_dyb, 0.5*PMT_dz); //its size

	fLogicPMT = new G4LogicalVolume(fSolidPMT,         //its solid
			fPMTMaterial,          //its material
			"PMT");           //its name

	fPhysiPMT = new G4PVPlacement(transform,                    // rotation
			fLogicPMT,             //its logical volume
			"PMT",                //its name
			fLogicScint,                //its mother  volume
			false,                   //no boolean operation
			0,
			false) ;                      //copy number
	
	//                                        
	// Visualization attributes
	//
	fLogicWorld->SetVisAttributes (G4VisAttributes::GetInvisible());

         G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(0.0, 0.7, 0.93,0.1));
         simpleBoxVisAtt->SetVisibility(true);
         simpleBoxVisAtt->SetForceSolid(true);
 
         G4VisAttributes* simpleBoxVisAtt2 = new G4VisAttributes(G4Colour(0.02, 0.91, 1, 0.1));
         simpleBoxVisAtt2->SetVisibility(true);
         simpleBoxVisAtt2->SetForceSolid(true);
 
 
 fLogicScint->SetVisAttributes(simpleBoxVisAtt);
 fLogicPMT->SetVisAttributes(simpleBoxVisAtt2);
 fLogicPMT1->SetVisAttributes(simpleBoxVisAtt2);


	//
	//always return the physical World
	//
	return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

