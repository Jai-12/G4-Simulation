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

#include "EventAction.hh"

#include "RunAction.hh"
#include "HistoManager.hh"


#include "G4SystemOfUnits.hh"
#include "G4Event.hh"


#include <CLHEP/Random/Randomize.h>
#include <CLHEP/Random/RandFlat.h>
#include <CLHEP/Random/RandBit.h>
#include <CLHEP/Random/RandGamma.h>
#include <CLHEP/Random/RandGaussQ.h>
#include <CLHEP/Random/RandPoissonQ.h>
#include <CLHEP/Random/RandExponential.h>
#include <CLHEP/Random/RandGeneral.h>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run, HistoManager* histo)
	:G4UserEventAction(),
	fRunAct(run),fHistoManager(histo),
	fEnergyScint(0.),
	fTrackLScint(0.), 
	p_particleX(0.),
	p_particleY(0.),
	p_particleTheta(0.),
	p_particlePhi(0.),
	p_particleEnergy(0.),
	myTrackLScint(0.),
	myEnergyScint(0.),
	fPhotonNumber(0),
	fPrintModulo(0)

{
	fPrintModulo = 100; }    // stil trying to understand the meaning of this variable

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
	// G4int evtNb = evt->GetEventID();

	// initialisation per event
	fEnergyScint  = 0;
	fTrackLScint  = 0;
	myTrackLScint = 0;
	myEnergyScint = 0;
	fPhotonNumber = 0;


//	evt->GetPrimaryVertex(0)->GetPrimary(0)  ;


	//primary particle position when generated 
	p_particleX = evt->GetPrimaryVertex(0)->GetX0() ;
	p_particleY = evt->GetPrimaryVertex(0)->GetY0();


	//primary particle direction when generated 
	p_particleTheta =   evt->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentumDirection().theta();
	p_particlePhi   =   evt->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentumDirection().phi();


	//primary particle total energy when generated 
	p_particleEnergy = evt->GetPrimaryVertex(0)->GetPrimary(0)->GetTotalEnergy();    	
	
	//primary particle mass
	G4double mass = evt->GetPrimaryVertex(0)->GetPrimary(0)->GetMass();

	//primary particle charge
	G4double charge = evt->GetPrimaryVertex(0)->GetPrimary(0)->GetCharge();



	G4double  d =1.*cm;        //Actually, here I should use the fScintThickness variable of the DetectorConstructor class

	G4double  x_down = p_particleX - d * tan(p_particleTheta)*cos(p_particlePhi);   
	G4double  y_down = p_particleY - d * tan(p_particleTheta)*sin(p_particlePhi);   


	// Also in the following IF statement I should use the parameters used in the DetectorConstuctor rather than the numerical values 
	// (if the particle does not pass through the entire scintillator the distance will be between 0 and 1cm with a uniform distribution) 

	if(!(x_down > -10.*cm  && y_down > -61.*cm  && x_down < 10.*cm  && y_down < 61.*cm )) myTrackLScint=CLHEP::HepRandom::getTheEngine()->flat()*cm;   
	//if the particle does not pass through the entire scintillator the distance will be between 0 and 1cm with a uniform distribution 
	else myTrackLScint = sqrt( pow((p_particleX-x_down),2) + pow((p_particleY-y_down),2) + pow(d,2) );   //computing the travelled distance 


/*
	//computing the mean energy loss with the Bethe-Block formula
	//Carbon, density:2g/cm3 A:12 Z:6 I: 81 eV 

	G4double m_e = 0.511 *MeV;

	G4double density = 8.96*g/cm3;
	G4double Z = 29;
	G4double A = 63.546 ;
	G4double I = 322.*eV;

	//G4double I = Z*(12 + 7/Z) *eV;                       //for Z<13        
	//G4double I = Z*(9.76 + 58.8*pow(Z,-1.19)) *eV;      // for Z >= 13 
	G4double  gamma = p_particleEnergy/mass;
	G4double beta = sqrt(1.-pow(gamma,-2));
	G4double eta = beta*gamma;
	G4double k = m_e/mass;        //mass= mass of the muon
	G4double W_max = (2.*m_e*eta*eta)/(1.+2.*k*sqrt(1.+eta*eta)+k*k);

	//shell and density corrections
	G4double C = -4.42;	
	G4double a = 0.1434;
	G4double y = 2.90;
	G4double X_1 = 3.28;
	G4double X_0 = 0.0254; 

	G4double X = log10(beta*gamma);


	G4double delta;
	if( X < X_0)                 delta = 0;
	else if (X > X_0 && X < X_1) delta = 4.6052*X+C+a*pow(X_1-X,y); 
	else if (X > X_1)            delta = 4.6052*X +C;



	//Bethe-Block
	G4double dE_over_dx = 0.1535*MeV*cm2/g*density*(Z/A)*pow(charge/beta, 2)*(log((2*m_e*gamma*gamma*beta*beta*W_max)/(I*I)) -2*beta*beta -delta-2*C/Z);




	myEnergyScint = dE_over_dx * myTrackLScint;



	//G4cout << "MYFUNC   "<< beta*gamma <<"   "<< dE_over_dx/density*g/MeV/cm2   << G4endl;




*/


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
	//G4double density = 8.96*g/cm3;

	//	G4cout << "MYFUNC   "<<beta*gamma<<"   "<<(fEnergyScint/fTrackLScint)/density*g/cm2/MeV   << G4endl;





/*
	//If you want to compute the energy loss with the track length comuputed by Geant4, then use this part:

	//computing the mean energy loss with the Bethe-Block formula
	//Carbon, density:2g/cm3 A:12 Z:6 I: 81 eV 

	G4double m_e = 0.511 *MeV;

	G4double density = 8.96*g/cm3;
	G4double Z = 29;
	G4double A = 63.546 ;
	G4double I = 322.*eV;

	//G4double I = Z*(12 + 7/Z) *eV;                       //for Z<13        
	//G4double I = Z*(9.76 + 58.8*pow(Z,-1.19)) *eV;      // for Z >= 13 
	G4double  gamma = p_particleEnergy/mass;
	G4double beta = sqrt(1.-pow(gamma,-2));
	G4double eta = beta*gamma;
	G4double k = m_e/mass;        //mass= mass of the muon
	G4double W_max = (2.*m_e*eta*eta)/(1.+2.*k*sqrt(1.+eta*eta)+k*k);

	//shell and density corrections
	G4double C = -4.42;	
	G4double a = 0.1434;
	G4double y = 2.90;
	G4double X_1 = 3.28;
	G4double X_0 = 0.0254; 

	G4double X = log10(beta*gamma);


	G4double delta;
	if( X < X_0)                 delta = 0;
	else if (X > X_0 && X < X_1) delta = 4.6052*X+C+a*pow(X_1-X,y); 
	else if (X > X_1)            delta = 4.6052*X +C;



	//Bethe-Block
	G4double dE_over_dx = 0.1535*MeV*cm2/g*density*(Z/A)*pow(charge/beta, 2)*(log((2*m_e*gamma*gamma*beta*beta*W_max)/(I*I)) -2*beta*beta -delta-2*C/Z);




	myEnergyScint = dE_over_dx * fTrackLScint;


*/










	//fill histograms
	//
	fHistoManager->FillHisto(0, fEnergyScint/MeV);
	fHistoManager->FillHisto(1, fTrackLScint/cm);
	fHistoManager->FillHisto(2, myTrackLScint/cm);
	fHistoManager->FillHisto(3, myEnergyScint/MeV);
	fHistoManager->FillHisto(4, fPhotonNumber);


}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
