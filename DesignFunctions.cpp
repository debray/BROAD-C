/*
 *  DesignFunctions.cpp
 *  
 *
 *  Created by Debajyoti Ray on 5/10/10.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include <iostream>
#include "DesignFunctions.hpp"
using namespace std;

void makeAllDesigns(double * Design0[], double * Design1[], int& num_Designs) {

	int num_Prob = 7;
	int num_Xlow = 3, num_Xhi = 3;
	int num_Outcome = num_Xlow * num_Xhi;
	int num_Sim = num_Prob*((num_Prob+1)/2)-1;
	
	// specify outcomes of lottery
	double X_low[num_Xlow], X_hi[num_Xhi], X_mid;
	
	X_low[0]=-5.; X_low[1]=-10.; X_low[2]=-20.;
	X_hi[0]=10.; X_hi[1]=20.; X_hi[2]=50.; //X_hi[3]=50.;
	X_mid = 5.;
	
	// specify probabilities of lottery
	double Prob[num_Prob];
	
	//for (int i=0; i<num_Prob; i++) {
		//Prob[i] = i*0.1;
	//}
	Prob[0] = 0.01; Prob[1]=0.1; Prob[2]=0.25; Prob[3]=0.5; Prob[4]=0.75; Prob[5]=0.9;
	Prob[6] = 0.99;
	
	// make Probability Simplex
	double * Simplex[num_Prob*num_Prob];
	double * myProb;
	double tempP;
	int ind = 0;
	
	for (int i=0; i<num_Prob; i++) {
		for (int j=0; j<(num_Prob-i); j++) {
			
			myProb = new double[3];
			
            tempP = 1-Prob[i]-Prob[j]; if (tempP<0) { tempP=0; }
            
			*(myProb) = Prob[i];
            *(myProb+1) = tempP;    // middle one gets remaining mass
			*(myProb+2) = Prob[j];
			
			Simplex[ind] = myProb;
			ind++;
		}
	}
	num_Sim = ind;
	
	// make Outcomes using X_low, 0, X_hi
	double * Outcomes[num_Outcome];
	double * myOut;
	
	ind = 0;
	for (int i=0; i<num_Xlow; i++) {
		for (int j=0; j<num_Xhi; j++) {
			
			myOut = new double[3];
			
			*(myOut) = X_low[i];
			*(myOut+1) = X_mid;
			*(myOut+2) = X_hi[j];
			
			Outcomes[ind] = myOut;
			ind++;
		}
	}
	
	// combine Probability Simplex with Outcomes to create Lottery set
	double * Lottery[num_Outcome*num_Sim];
	double * myLotto;
	ind = 0;
	
	for (int i=0; i<num_Outcome; i++) {
		for (int j=0; j<num_Sim; j++) {
			
			myLotto = new double[6];
			
			*(myLotto) = *(Outcomes[i]);
			*(myLotto+1) = *(Outcomes[i]+1);
			*(myLotto+2) = *(Outcomes[i]+2);
			*(myLotto+3) = *(Simplex[j]);
			*(myLotto+4) = *(Simplex[j]+1);
			*(myLotto+5) = *(Simplex[j]+2);
			
			Lottery[ind] = myLotto;
			ind++;
		}
	}
	
	int num_Lotto = ind;
	
	int num=0;
	//double * Design0[num_Designs], * Design1[num_Designs];
	
	// create all Designs
	for (int i=0; i<num_Lotto; i++) {
		for (int j=i+1; j<num_Lotto; j++) {
	
			Design0[num] = Lottery[i];
			Design1[num] = Lottery[j];
			num++;
		}
	}
	
	num_Designs = num;
	
	//cout << "Number of Designs: "<<num_Designs<<endl;
}
