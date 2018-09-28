#include <iostream>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <random>
#include <chrono>


using namespace std;


void macro_dinodes(){ 
	TCanvas *c1 = new TCanvas("c1", "c1",258,81,1104,559);
	//c1->SetLogx();

	TRandom *r0 = new TRandom();
	//TH1D* n_electrons_A = new TH1D("n_electrons_A","final number of electrons method A",100,1*1e3,20*1e10);
	TH1D* n_electrons_A = new TH1D("n_electrons_A","final number of electrons method A",100,100*1e7,20*1e7);
	TH1D* n_electrons_B = new TH1D("n_electrons_B","final number of electrons method B",100,100*1e7,20*1e7);
	TH1D* n_electrons_C = new TH1D("n_electrons_C","final number of electrons method C",100,100*1e7,20*1e7);

	TF1  *gaus_1 = new TF1("gauss_1","[0]*exp(-0.5*pow((x-[1])/[2],2))",0,1e13);
	TF1  *gaus_2 = new TF1("gauss_2","[0]*exp(-0.5*pow((x-[1])/[2],2))",0,1e13);	

	gaus_1->SetParameter(0,10000);
	gaus_1->SetParameter(1,4e11);
	gaus_1->SetParameter(2,2.6e9);

	gaus_2->SetParameter(0,10000);
	gaus_2->SetParameter(1,4e11);
	gaus_2->SetParameter(2,6.3e5);







	double totalGain =  5*1e7;
	int numberOfDinodes = 6;
	double idealGainPerDinode = pow(totalGain, pow(numberOfDinodes,-1));

	double fElectronsGenerated = 8000;


	int nevents = 10000;
	//BEGIN LOOP OVER THE EVENTS

	for(int i=0; i<nevents; i++){
		//cout << distribution(generator)<< endl;

		//method A

		double finalElectronsNumber_A = 0; 
		double finalElectronsNumber_B = 0;
		double finalElectronsNumber_C = 0;
		for (int je=0; je<fElectronsGenerated; je++){
			double nsec_A=1;	
			double nsec_B=1;	
			for(int n=0; n < numberOfDinodes; n++){
				double gainForThisDinode = r0->PoissonD(idealGainPerDinode);
				nsec_A*=gainForThisDinode;
			}
			finalElectronsNumber_A+=nsec_A;
			//method B

			nsec_B = 1*r0->PoissonD(totalGain) ;

			finalElectronsNumber_B+=nsec_B;



		}

		// method C

		finalElectronsNumber_C  =   fElectronsGenerated*r0->PoissonD(totalGain) ;


		n_electrons_A->Fill(finalElectronsNumber_A);
		n_electrons_B->Fill(finalElectronsNumber_B);
		n_electrons_C->Fill(finalElectronsNumber_C);



	};

	//END LOOP OVER THE EVENTS
	c1->Divide(2);




	//////// QUI METTI IL PRIMO FIT GAUSSIANO
	c1->cd(1);	
	n_electrons_A->Fit("gaus_1");
	gaus_1->Draw();
	n_electrons_A->Draw();



	//////// QUI METTI IL SECONDO FIT GAUSSIANO
	c1->cd(2); 


	n_electrons_B->Fit("gaus_2");
	gaus_2->Draw();
	n_electrons_B->Draw();
	//  //	c1->cd(3); n_electrons_C->Draw();



	//TF1  *fA = new TF1("fA","gaus",0,10)
	//TF1  *fB = new TF1("fB","gaus",0,10)

	//// fits


	//TStyle *st1 = new TStyle("st1","my style");
	//st1->SetHistLineColor(4);

	//n_electrons_A->Draw("");

	//st1->SetHistLineColor(2);
	//n_electrons_B->Draw("SAME");



};
