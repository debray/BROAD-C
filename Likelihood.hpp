/*
 *  Likelihood.hpp
 *  Declare variables and functions to compute P(y|d, m_{k})
 *
 *  Created by Debajyoti Ray on 5/6/10.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "UtilitySpec.hpp"

class LikelihoodFPT : public UtilityFPT {
	
public:
	double * Design[2];		// pair of pdfs 
	
	int gridSize;			// number of elementers per dimension
	
	double MinR[3];			// min of param range
	
	double MaxR[3];			// max of param range
	
	double probObs();
	
	// MLE response of model to a design (H,Theta)
	int MLEresponse(int *Response);
};


class LikelihoodCPT : public UtilityCPT {

public:
	double * Design[2];		// pair of pdfs 
	
	int gridSize;			// number of elementers per dimension
	
	double MinR[3];			// min of param range
	
	double MaxR[3];			// max of param range
	
	double probObs();
	
	// MLE response of model to a design (H,Theta)
	int MLEresponse(int *Response);
};


class LikelihoodMVS : public UtilityMVS {
	
public:
	double * Design[2];		// pair of pdfs 
	
	int gridSize;			// number of elementers per dimension
	
	double MinR[3];			// min of param range
	
	double MaxR[3];			// max of param range
	
	double probObs();
	
	// MLE response of model to a design (H,Theta)	
	int MLEresponse(int *Response);
};


class LikelihoodNMVS : public UtilityNMVS { 

public:
	double * Design[2];		// pair of pdfs 
	
	int gridSize;			// number of elementers per dimension
	
	double MinR[3];			// min of param range
	
	double MaxR[3];			// max of param range
	
	double probObs();
	
	// MLE response of model to a design (H,Theta)	
	int MLEresponse(int *Response);
};


class LikelihoodEU : public UtilityEU {
	
public:
	double * Design[2];		// pair of pdfs 
	
	double probObs();
	
	// MLE response of model to a design (H,Theta)
	int MLEresponse(int *Response);
};


class LikelihoodCRRA : public UtilityCRRA {
	
public:
	double * Design[2];		// pair of pdfs 
	
	int gridSize;			// number of elementers per dimension
	
	double MinR;			// min of param range
	
	double MaxR;			// max of param range
	
	double probObs();
	
	// MLE response of model to a design (H,Theta)
	int MLEresponse(int *Response);
};
