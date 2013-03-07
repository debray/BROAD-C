/*
 * EC2.hpp
 *  Declare variables and functions to compute Edge-Cutting objective
 *
 *  Created by Debajyoti Ray on 5/6/10.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "Likelihood.hpp"

class EC2 {
	
public:
	int num_Designs;
	
	int numM;			// number of models to test
	
	double noiseProb[4];	// probability of observation error - vector
	int maxErr[4];			// maximum number of errors to tolerate - vector
	
	double **all_Design0;	// pointer to array of pointers to designs
	double **all_Design1;
	
	double *design_Score;		// pointer to array of scores of all designs
	
	double *best_Design[2];		// design with best Score
	
	double probModel[6];	// probability of each model
	
	//int numH;			// number of (H,Theta)
	int numN;			// number of noise levels
	
    // Number of hypotheses: 4 * 5^3 + 1 + 5 = 506
	double WtHT[506][4];	// weights for (H, Theta) at numN noise levels
	
	// create table of MLE responses for valid (H, Theta)
	int tableHT(LikelihoodFPT likFPT, LikelihoodCPT likCPT,
                LikelihoodMVS likMVS, LikelihoodNMVS likNMVS, 
				LikelihoodEU likEU, LikelihoodCRRA likCRRA, 
				int d, int *ResponseHT[], int numHT[]);
	
	// compute score of all designs and return index of best design 
	int designEC(LikelihoodFPT likFPT, LikelihoodCPT likCPT,
                 LikelihoodMVS likMVS, LikelihoodNMVS likNMVS, 
				 LikelihoodEU likEU, LikelihoodCRRA likCRRA, 
                 int UsedD[], int numR);	
	
	// EC2 score criteria
	double scoreEC(int *ResponseHT[], int numHT[]);
	
	// update valid (H, Theta) after observation
	int updateHT(LikelihoodFPT likFPT, LikelihoodCPT likCPT,
                 LikelihoodMVS likMVS, LikelihoodNMVS likNMVS, 
				 LikelihoodEU likEU, LikelihoodCRRA likCRRA,
				 int *ErrHT[], int d, int Xobs);
	
	// update model posteriors
	int updateModelPost(int numHT[]);
    
    // choose a design at random
	int randomDesign(int UsedD[], int numR);
};
