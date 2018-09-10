#include <iostream>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <random>
#include <chrono>


using namespace std;


void macro_dinodes(){ 
TCanvas *Canvas_1 = new TCanvas("Canvas_1", "Canvas_1",258,81,1104,559);
//Canvas_1->SetLogx();

TRandom *r0 = new TRandom();
TH1D* n_electrons_A = new TH1D("n_electrons_A","final number of electrons method A",100,1*1e3,20*1e10);
TH1D* n_electrons_B = new TH1D("n_electrons_B","final number of electrons method B",100,100*1e7,20*1e7);


double totalGain =  1e7;
int numberOfDinodes = 12;
double idealGainPerDinode = pow(totalGain, pow(numberOfDinodes,-1));
 
 long fElectronsGenerated = 8000;


int nevents = 10000;
//BEGIN LOOP OVER THE EVENTS

for(int i=0; i<nevents; i++){
//cout << distribution(generator)<< endl;




//method A 
 long finalElectronsNumber_A = fElectronsGenerated;

		 // BEGIN LOOP OVER THE DINODES
		 for(int n=0; n < numberOfDinodes; n++){

		         double gainForThisDinode = r0->Poisson(idealGainPerDinode);

//		cout << n  << " " << distribution_A(generator)<< endl;

 
		 finalElectronsNumber_A  *= gainForThisDinode ;
 
 
  			};
		// END LOOP OVER THE DINODES

//method B
 
long finalElectronsNumber_B = fElectronsGenerated*r0->Poisson(totalGain) ;
 
	n_electrons_A->Fill(finalElectronsNumber_A);
	n_electrons_B->Fill(finalElectronsNumber_B);



};

//END LOOP OVER THE EVENTS

n_electrons_A->Draw();
//n_electrons_B->Draw("SAME");

};

