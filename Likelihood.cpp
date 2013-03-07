/*
 *  Likelihood.cpp
 *  Definitions of functions to compute P(y | d, m)
 *
 *  Created by Debajyoti Ray on 5/6/10.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "Likelihood.hpp"

// define probability of observation for FPT, P(y=0|d,M_fpt)
double LikelihoodFPT :: probObs() {
	
	double U1, U2, probObs;
	double Lambda = 0.1;
	
	this->Lottery = this->Design[0];
	U1 = this->calcUtilityFPT();
	
	this->Lottery = this->Design[1];
	U2 = this->calcUtilityFPT();
	
	probObs = 1./(1. + exp((-U1 + U2)/Lambda));
	
	return probObs;
}


// define probability of observation for CPT, P(y=0|d,M_cpt)
double LikelihoodCPT :: probObs() {
	
	double U1, U2, probObs;
	double Lambda = 0.1;
	
	this->Lottery = this->Design[0];
	U1 = this->calcUtilityCPT();
	
	this->Lottery = this->Design[1];
	U2 = this->calcUtilityCPT();
	
	probObs = 1./(1. + exp((-U1 + U2)/Lambda));
	
	return probObs;
}


// define probability of observation for MVS, P(y=0|d,M_mvs)
double LikelihoodMVS :: probObs() {
	
	double U1, U2, probObs;
	double Lambda = 0.1;
	
	this->Lottery = this->Design[0];
	U1 = this->calcUtilityMVS();
	
	this->Lottery = this->Design[1];
	U2 = this->calcUtilityMVS();
	
	probObs = 1./(1. + exp((-U1 + U2)/Lambda));
	
	return probObs;
}


// define probability of observation for NMVS, P(y=0|d,M_nmvs)
double LikelihoodNMVS :: probObs() {
	
	double U1, U2, probObs;
	double Lambda = 0.1;
	
	this->Lottery = this->Design[0];
	U1 = this->calcUtilityNMVS();
	
	this->Lottery = this->Design[1];
	U2 = this->calcUtilityNMVS();
	
	probObs = 1./(1. + exp((-U1 + U2)/Lambda));
	
	return probObs;
}


// define probability of observation for EU, P(y=0|d,M_fpt)
double LikelihoodEU :: probObs() {
	
	double U1, U2, probObs;
	double Lambda = 0.1;
	
	this->Lottery = this->Design[0];
	U1 = this->calcUtilityEU();
	
	this->Lottery = this->Design[1];
	U2 = this->calcUtilityEU();
	
	probObs = 1./(1. + exp((-U1 + U2)/Lambda));
	
	return probObs;
	
}


// define probability of observation for CRRA, P(y=0|d,M_fpt)
double LikelihoodCRRA :: probObs() {
	
	double U1, U2, probObs;
	double Lambda = 0.1;
	
	this->Lottery = this->Design[0];
	U1 = this->calcUtilityCRRA();
	
	this->Lottery = this->Design[1];
	U2 = this->calcUtilityCRRA();
	
	probObs = 1./(1. + exp((-U1 + U2)/Lambda));
	
	return probObs;
	
}


// obtain MLE responses for all parameters of FPT
int LikelihoodFPT :: MLEresponse(int *Response) {
	
	double inc0 = 0., inc1 = 0., inc2 = 0.;
	int count = 0;
	
	for (int i=0; i<this->gridSize; i++) {
		
		inc0 = (this->MaxR[0]-this->MinR[0]) / (this->gridSize-1);
		this->Params[0] = this->MinR[0] + i*inc0;
		
		for (int j=0; j<this->gridSize; j++) {
			
			inc1 = (this->MaxR[1]-this->MinR[1]) / (this->gridSize-1);
			this->Params[1] = this->MinR[1] + j*inc1;
			
			for (int k=0; k<this->gridSize; k++) {
		
				inc2 = (this->MaxR[2]-this->MinR[2]) / (this->gridSize-1);
				this->Params[2] = this->MinR[2] + k*inc2;
			
				if (this->probObs() < 0.5) { *(Response+count) = 1; } 
				else { *(Response+count) = 0; }
			
				count++;
			}
		}
	}
	
	return 1;
}


// obtain MLE responses for all parameters of CPT
int LikelihoodCPT :: MLEresponse(int *Response) {
	
	double inc0 = 0., inc1 = 0., inc2 = 0.;
	int count = 0;
	
	for (int i=0; i<this->gridSize; i++) {
		
		inc0 = (this->MaxR[0]-this->MinR[0]) / (this->gridSize-1);
		this->Params[0] = this->MinR[0] + i*inc0;
		
		for (int j=0; j<this->gridSize; j++) {
			
			inc1 = (this->MaxR[1]-this->MinR[1]) / (this->gridSize-1);
			this->Params[1] = this->MinR[1] + j*inc1;
			
			for (int k=0; k<this->gridSize; k++) {
                
				inc2 = (this->MaxR[2]-this->MinR[2]) / (this->gridSize-1);
				this->Params[2] = this->MinR[2] + k*inc2;
                
				if (this->probObs() < 0.5) { *(Response+count) = 1; } 
				else { *(Response+count) = 0; }
                
				count++;
			}
		}
	}
	
	return 1;
}


// obtain MLE responses for all parameters of MVS
int LikelihoodMVS :: MLEresponse(int *Response) {
	
	//int numElem = this->num_Params * this->gridSize;
	//int *X = new int[numElem]; for (int n=0; n<numElem; n++) { *(X+n)=0; }
	
	double inc0 = 0., inc1 = 0., inc2 = 0.;
	int count = 0;
	
	for (int i=0; i<this->gridSize; i++) {
		
		inc0 = (this->MaxR[0]-this->MinR[0]) / (this->gridSize-1);
		this->Params[0] = this->MinR[0] + i*inc0;
		
		for (int j=0; j<this->gridSize; j++) {
			
			inc1 = (this->MaxR[1]-this->MinR[1]) / (this->gridSize-1);
			this->Params[1] = this->MinR[1] + j*inc1;
			
			for (int k=0; k<this->gridSize; k++) {
				
				inc2 = (this->MaxR[2]-this->MinR[2]) / (this->gridSize-1);
				this->Params[2] = this->MinR[2] + k*inc2;
				
				if (this->probObs() < 0.5) { *(Response+count) = 1; }
				else { *(Response+count) = 0; }
				count++;
			}
		}
	}
	
	return 1;
}


// obtain MLE responses for all parameters of NMVS
int LikelihoodNMVS :: MLEresponse(int *Response) {
	
	double inc0 = 0., inc1 = 0., inc2 = 0.;
	int count = 0;
	
	for (int i=0; i<this->gridSize; i++) {
		
		inc0 = (this->MaxR[0]-this->MinR[0]) / (this->gridSize-1);
		this->Params[0] = this->MinR[0] + i*inc0;
		
		for (int j=0; j<this->gridSize; j++) {
			
			inc1 = (this->MaxR[1]-this->MinR[1]) / (this->gridSize-1);
			this->Params[1] = this->MinR[1] + j*inc1;
			
			for (int k=0; k<this->gridSize; k++) {
				
				inc2 = (this->MaxR[2]-this->MinR[2]) / (this->gridSize-1);
				this->Params[2] = this->MinR[2] + k*inc2;
				
				if (this->probObs() < 0.5) { *(Response+count) = 1; }
				else { *(Response+count) = 0; }
				count++;
			}
		}
	}
	
	return 1;
}


// obtain MLE response for all parameters of EU
int LikelihoodEU :: MLEresponse(int *Response) {
    
	*(Response) = 0;
	
	if (this->probObs()<0.5) { *(Response) = 1; }
	
	return 1;
}


// obtain MLE responses for all parameters of CRRA
int LikelihoodCRRA :: MLEresponse(int *Response) {
	
	int count = 0;
	double inc0 = 0.;
	
	for (int i=0; i<this->gridSize; i++) {
		
		inc0 = (this->MaxR-this->MinR) / (this->gridSize-1);
		this->Params = this->MinR + i*inc0;
		
		if (this->probObs() < 0.5) { *(Response+count) = 1; }
		else { *(Response+count) = 0; }
		
		count++;
	}
	
	return 1;
}
