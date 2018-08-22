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
#include "PrimaryGeneratorAction.hh"

#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"


#include <CLHEP/Random/Randomize.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/RandBit.h>
#include <CLHEP/Random/RandGamma.h>
#include <CLHEP/Random/RandGaussQ.h>
#include <CLHEP/Random/RandPoissonQ.h>
#include <CLHEP/Random/RandExponential.h>
#include <CLHEP/Random/RandGeneral.h>


G4double PrimaryGeneratorAction::myrand(){                         // First uniform  distribution (from -1 to 1)

	//generate a pseudo random number in the interval [-1;1]
	G4double r,sign;
	r = (  CLHEP::HepRandom::getTheEngine()->flat() );
	sign=1.; 
	if(r<0.5) sign=-1.;
	r = (  CLHEP::HepRandom::getTheEngine()->flat() );
	r=r*sign;
	//  cout<<"  myrand: "<<r<<endl;
	return r;
}



G4double PrimaryGeneratorAction::myrandpos(){                      // Second uniform  distribution (from 0 to 1) 

	//generate a pseudo random number in the interval [0;1]
	G4double r;
	r =  (   CLHEP::HepRandom::getTheEngine()->flat() );
	//cout<<"  myrandpos: "<<r<<endl;
	return r;
}



G4double PrimaryGeneratorAction::random_E(){                       // Energy distribution
	G4double r;
	G4double r_max = 1000*GeV;

	r = r_max * myrandpos();

	return 107*MeV;  //return 1*GeV;
}


G4double PrimaryGeneratorAction::random_X(){ 			   // X distribution
	G4double r;
	G4double r_max = 10*cm;

	r = r_max * myrand();

	return r;
}


G4double PrimaryGeneratorAction::random_Y(){                       // Y distribution
	G4double r;
	G4double r_max = 61*cm;

	r = r_max * myrand();

	return r;
}


G4double PrimaryGeneratorAction::random_Theta(){                   // Theta distribution (cos^2)
	G4double x;
	G4double f;

	do {
		x = (CLHEP::pi/2.) * myrandpos();
		f = (3.*sqrt(3.)/4) * myrandpos();
	}while(f > (4)*cos(x)*cos(x)*cos(x)*sin(x));

	return x;
}


G4double PrimaryGeneratorAction::random_Phi(){                     // Phi distribution
	G4double r;
	r = 2*CLHEP::pi * myrandpos();
	//cout<<"  myrandpos: "<<r<<endl;
	return r;
}






PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	generalParticleGun = new G4GeneralParticleSource();

	G4int n_particle = 1;
	particleGun = new G4ParticleGun(n_particle);
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete generalParticleGun;
	delete particleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries( G4Event* anEvent )
{

	// CLHEP::HepRandom::setTheSeed(time(0));
	
	//particle definition
	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName;
	particleGun->SetParticleDefinition( //I could use mu-, or generate randomly both mu- and mu+
			particleTable->
			FindParticle(particleName="mu+")); 


	//Setting energy position and momentum direction

	particleEnergy = random_E(); // here use the right distributions for the energy
	particleX      = random_X();
	particleY      = random_Y(); 
	particleZ      = 0.5 *cm;    //we generate particles on the upper surface of the scintillator to make them hit always the detector 
	particleTheta  = random_Theta();
	particlePhi    = random_Phi(); 


	//SETTING THE KINETIC ENERGY, AND NOT THE TOTAL ENERGY
	particleGun->SetParticleEnergy(particleEnergy);   

	//you can sett the Momentum if you prefer
	//	particleGun->SetParticleMomentum(particleEnergy);


	G4ThreeVector ParticlePosition(particleX, particleY, particleZ);
	particleGun->SetParticlePosition(ParticlePosition);


	G4ThreeVector ParticleMomentumDirection;
	ParticleMomentumDirection.setRThetaPhi(-1, particleTheta, particlePhi);
	particleGun->SetParticleMomentumDirection(ParticleMomentumDirection); 




	particleGun->GeneratePrimaryVertex( anEvent );            // I can either generate a primary vertex either  
	//generalParticleGun->GeneratePrimaryVertex( anEvent );   // with the particle gun or with the general particle source (GPS)
}

