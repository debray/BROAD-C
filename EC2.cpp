/*
 *  EC2.cpp
 *  Definitions of functions to compute Edge-Cutting objective
 *
 *  Created by Debajyoti Ray on 5/6/10.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "EC2.hpp"

// find the score of all designs and select the minimum
int EC2 :: designEC (LikelihoodFPT likFPT, LikelihoodCPT likCPT,
                     LikelihoodMVS likMVS, LikelihoodNMVS likNMVS,
                     LikelihoodEU likEU, LikelihoodCRRA likCRRA, 
                     int UsedD[], int numR) {
	
	double Score, minScore = 100;
	double designScore[this->num_Designs];
	int bestD;
	
	int numM = this->numM;
	
	int numHT[numM];
	numHT[0]=pow(likFPT.gridSize, likFPT.num_Params);
    numHT[1]=pow(likCPT.gridSize, likCPT.num_Params);
	numHT[2]=pow(likMVS.gridSize, likMVS.num_Params);
    numHT[3]=pow(likNMVS.gridSize, likNMVS.num_Params);
	numHT[4]=1;
	numHT[5]=pow(likCRRA.gridSize, likCRRA.num_Params);
	
	// assign memory for count table - responses for (H,Theta) that are valid
	// An array of pointers - pointing to array of responses
	int *ResponseHT[numM];
	for (int h=0; h<numM; h++) { ResponseHT[h] = new int[numHT[h]]; }
	
	// assign scores to the designs
	for (int d=0; d<this->num_Designs; d++) {

		// create ResponseHT (the table of MLE responses)
		this->tableHT(likFPT, likCPT, likMVS, likNMVS, likEU, likCRRA, 
                               d, ResponseHT, numHT);
		
		// score this design
		Score = this->scoreEC(ResponseHT, numHT);
		//cout << "Design: " << d << ", Score: " << Score << endl;
		designScore[d] = Score;
		
		if (Score < minScore) { 
			// has this design been used already?
			bool isUsed = 0;
			for (int r=0; r<numR; r++) {
				if (d==UsedD[r]) { isUsed = 1; }
			}
			
			if (isUsed==0) {
				minScore = Score; 
				this->best_Design[0] = *(this->all_Design0+d);
				this->best_Design[1] = *(this->all_Design1+d);
				bestD = d;
			}
		}
	}
    
    //output to a file?
    ofstream junkFile;
    junkFile.open("junkfile.dat");
	//cout << " Picked: " << bestD << " minScore: " << minScore << endl; 
    junkFile << bestD << ", " << minScore << endl;
	
	this->design_Score = &designScore[0];
	
	// free memory for count table
	for (int i=0; i<numM; i++) { delete[] ResponseHT[i]; }
	
	return bestD;
}


// create table of responses - MLE for given design
int EC2 :: tableHT (LikelihoodFPT likFPT, LikelihoodCPT likCPT,
                    LikelihoodMVS likMVS, LikelihoodNMVS likNMVS, 
					 LikelihoodEU likEU, LikelihoodCRRA likCRRA, 
					 int d, int *ResponseHT[], int numHT[]) {
	
	// allocate temporary variables for Likelihood
	LikelihoodFPT tempFPT; LikelihoodCPT tempCPT;
    LikelihoodMVS tempMVS; LikelihoodNMVS tempNMVS; 
	LikelihoodEU tempEU; LikelihoodCRRA tempCRRA;
	tempFPT = likFPT; tempCPT = likCPT;
    tempMVS = likMVS; tempNMVS = likNMVS;
	tempEU = likEU; tempCRRA = likCRRA;
	
	// find X_nA consistent with this design d
	tempFPT.Design[0] = *(this->all_Design0+d);
	tempFPT.Design[1] = *(this->all_Design1+d);
    tempCPT.Design[0] = *(this->all_Design0+d);
	tempCPT.Design[1] = *(this->all_Design1+d);
	tempMVS.Design[0] = *(this->all_Design0+d);
	tempMVS.Design[1] = *(this->all_Design1+d);
    tempNMVS.Design[0] = *(this->all_Design0+d);
	tempNMVS.Design[1] = *(this->all_Design1+d);
	tempEU.Design[0] = *(this->all_Design0+d);
	tempEU.Design[1] = *(this->all_Design1+d);
	tempCRRA.Design[0] = *(this->all_Design0+d);
	tempCRRA.Design[1] = *(this->all_Design1+d);
	
	// fill table for FPT
	tempFPT.MLEresponse(ResponseHT[0]);	// pass pointer to array as argument
    // fill table for CPT
	tempCPT.MLEresponse(ResponseHT[1]);	// pass pointer to array as argument
    // fill table for MVS
	tempMVS.MLEresponse(ResponseHT[2]);
    // fill table for NMVS
	tempNMVS.MLEresponse(ResponseHT[3]);
	// fill table for EU
	tempEU.MLEresponse(ResponseHT[4]);
	// fill table for CRRA
	tempCRRA.MLEresponse(ResponseHT[5]);
	
	return 1;
}

// Compute score of design d from samples 
double EC2 :: scoreEC (int *ResponseHT[],  int numHT[]) {
	
	int numM = this->numM;
	
	double * AggWtHT[numM];
	for (int h=0; h<numM; h++) {
		AggWtHT[h] = new double[numHT[h]];
	}
	
	int ind = 0;
	// get aggregate likelihood by marginalizing over all noise levels
	for (int h=0; h<numM; h++) {
		for (int n=0; n<numHT[h]; n++) {
			for (int p=0; p<this->numN; p++) {
			
				*(AggWtHT[h]+n) += this->WtHT[ind][p];
			}
			ind++;
			//cout << "Weight: "<<h<<", "<<n<<", :"<< *(AggWtHT[h]+n)<<"   ;";
		}
	}
	
	// calculate conditional, P(x_d=0 | x_A)
	//int NumCond0 = 0, NumCondAll = 0;
	double MCond0 = 0., MCondAll = 0.;
	double PCond0 = 0.;
	// conditional count for each hypothesis
	for (int h=0; h<numM; h++) {
		for (int n=0; n<numHT[h]; n++) {
			
			MCondAll += *(AggWtHT[h]+n);
			if (*(ResponseHT[h]+n)==0) { MCond0 += *(AggWtHT[h]+n); }
		}
	}
	PCond0 = MCond0 / MCondAll;
	//cout << "PCond0: " << PCond0 << endl;
	
	// calculate joint, P(x_A, x_d=0)
	//int NumJ0 = 0, NumJ1 = 0, NumAll = 0;
	double MJ0 = 0., MJ1 = 0., MAll = 0.;
	double PJoint0 = 0., PJoint1 = 0.;
	// joint count
	for (int h=0; h<numM; h++) {
		for (int n=0; n<numHT[h]; n++) {
			
			MAll += *(AggWtHT[h]+n);
				
			if (*(ResponseHT[h]+n)==0) { MJ0 += *(AggWtHT[h]+n); }
			else if (*(ResponseHT[h]+n)==1) { MJ1 += *(AggWtHT[h]+n); }
		}
	}
	PJoint0 = MJ0 / MAll;
	PJoint1 = MJ1 / MAll;
	
	// calculate joint with hypothesis, P(H, x_A, x_d=0)
	double MH0[numM], MH1[numM];
	double PH0[numM], PH1[numM];
	// initialize
	for (int h=0; h<numM; h++) { MH0[h]=0.; MH1[h]=0.; PH0[h]=0.; PH1[h]=0.;  }
	// add mass for each hypothesis
	for (int h=0; h<numM; h++) {
		for (int n=0; n<numHT[h]; n++) {
			
			if (*(ResponseHT[h]+n)==0) { MH0[h] += *(AggWtHT[h]+n); }
			else if (*(ResponseHT[h]+n)==1) { MH1[h] += *(AggWtHT[h]+n); }
		}
		PH0[h] = MH0[h] / MAll;
		PH1[h] = MH1[h] / MAll;
	}
	
	// sum square of P(H, x_A, x_d=0)
	double PH0all2 = 0., PH1all2 = 0.;
	for (int h=0; h<numM; h++) {
		PH0all2 += PH0[h]*PH0[h]; PH1all2 += PH1[h]*PH1[h]; 
	}
	
	// compute score for design
	double Score = 0.;
	Score = PCond0*(PJoint0*PJoint0 - PH0all2) + 
						(1-PCond0)*(PJoint1*PJoint1 - PH1all2);
	//cout << "Score: " << Score << endl;
	
	// free memory for WtHT
	for (int h=0; h<numM; h++) { delete[] AggWtHT[h]; }
	
	return Score;
}

// update valid (H, Theta) after observation
int EC2 :: updateHT (LikelihoodFPT likFPT, LikelihoodCPT likCPT,
                     LikelihoodMVS likMVS, LikelihoodNMVS likNMVS, 
					 LikelihoodEU likEU, LikelihoodCRRA likCRRA,
					 int *ErrHT[], int d, int Xobs) {
	
	// allocate temporary variables
	LikelihoodFPT tempFPT; LikelihoodCPT tempCPT;
    LikelihoodMVS tempMVS; LikelihoodNMVS tempNMVS; 
	LikelihoodEU tempEU; LikelihoodCRRA tempCRRA;
	tempFPT = likFPT; tempCPT = likCPT;
    tempMVS = likMVS; tempNMVS = likNMVS;
	tempEU = likEU; tempCRRA = likCRRA;
	
	// find X_nA consistent with this design d
	tempFPT.Design[0] = *(this->all_Design0+d);
	tempFPT.Design[1] = *(this->all_Design1+d);
    tempCPT.Design[0] = *(this->all_Design0+d);
	tempCPT.Design[1] = *(this->all_Design1+d);
	tempMVS.Design[0] = *(this->all_Design0+d);
	tempMVS.Design[1] = *(this->all_Design1+d);
    tempNMVS.Design[0] = *(this->all_Design0+d);
	tempNMVS.Design[1] = *(this->all_Design1+d);
	tempEU.Design[0] = *(this->all_Design0+d);
	tempEU.Design[1] = *(this->all_Design1+d);
	tempCRRA.Design[0] = *(this->all_Design0+d);
	tempCRRA.Design[1] = *(this->all_Design1+d);
	
	// number of parameters for each model
	int numHT[this->numM];
	numHT[0]=pow(likFPT.gridSize, likFPT.num_Params);
    numHT[1]=pow(likCPT.gridSize, likCPT.num_Params);
	numHT[2]=pow(likMVS.gridSize, likMVS.num_Params);
    numHT[3]=pow(likNMVS.gridSize, likNMVS.num_Params);
	numHT[4]=1;
	numHT[5]=pow(likCRRA.gridSize, likCRRA.num_Params);
	
	// allocate memory
	int *X_HT[this->numM];
	for (int h=0; h<this->numM; h++) { X_HT[h] = new int[numHT[h]]; }
	
	// what is the MLE response of (H, Theta) to design d?
	tempFPT.MLEresponse(X_HT[0]);
    tempCPT.MLEresponse(X_HT[1]);
	tempMVS.MLEresponse(X_HT[2]);
    tempNMVS.MLEresponse(X_HT[3]);
	tempEU.MLEresponse(X_HT[4]);
	tempCRRA.MLEresponse(X_HT[5]);
	
	int ind=0;
	// Reweight (H, Theta) based on observation
	for (int h=0; h<this->numM; h++) {
		for (int n=0; n<numHT[h]; n++) {
			
			// does prediction of (H, Theta) correspond to observation?
			if (*(X_HT[h]+n)!=Xobs) {
				
				*(ErrHT[h]+n) = *(ErrHT[h]+n) + 1;	// increase error count
				
				//cout<< "pre-F: "<<this->WtHT[ind][0]<<", "<<this->WtHT[ind][1]
				//<<", "<<this->WtHT[ind][2]<<", "<<this->WtHT[ind][3]<<endl;
				
				this->WtHT[ind][0] = this->WtHT[ind][0] * this->noiseProb[0]; 
				this->WtHT[ind][1] = this->WtHT[ind][1] * this->noiseProb[1]; 
				this->WtHT[ind][2] = this->WtHT[ind][2] * this->noiseProb[2]; 
				this->WtHT[ind][3] = this->WtHT[ind][3] * this->noiseProb[3];  
				
				//cout<< "F: "<< this->WtHT[ind][0]<<", "<<this->WtHT[ind][1]
				//<<", "<<this->WtHT[ind][2]<<", "<<this->WtHT[ind][3]<<endl;
			}
			else {
				//cout<< "pre-T: "<<this->WtHT[ind][0]<<", "<<this->WtHT[ind][1]
				//<<", "<<this->WtHT[ind][2]<<", "<<this->WtHT[ind][3]<<endl;
				
				this->WtHT[ind][0] = this->WtHT[ind][0] * (1-this->noiseProb[0]); 
				this->WtHT[ind][1] = this->WtHT[ind][1] * (1-this->noiseProb[1]); 
				this->WtHT[ind][2] = this->WtHT[ind][2] * (1-this->noiseProb[2]); 
				this->WtHT[ind][3] = this->WtHT[ind][3] * (1-this->noiseProb[3]);
				
				//cout<< "T: "<<this->WtHT[ind][0]<<", "<<this->WtHT[ind][1]
				//<<", "<<this->WtHT[ind][2]<<", "<<this->WtHT[ind][3]<<endl;
			}
			
			// Eliminate (H, Theta) if more than maxErr errors made for all noise
			if (*(ErrHT[h]+n) > this->maxErr[0]) { this->WtHT[ind][0]=0.; }
			if (*(ErrHT[h]+n) > this->maxErr[1]) { this->WtHT[ind][1]=0.; }
			if (*(ErrHT[h]+n) > this->maxErr[2]) { this->WtHT[ind][2]=0.; }
			if (*(ErrHT[h]+n) > this->maxErr[3]) { this->WtHT[ind][3]=0.; }
			
			ind++;
		}
	}
	
	// deallocate memory
	for (int i=0; i<numM; i++) { delete[] X_HT[i]; }
		
	return 1;
}

// update model posteriors from valid counts
int EC2 :: updateModelPost(int numHT[]) {

	int numM = this->numM;
	double MassH[numM];
	double TotalM = 0.;
	int ind=0;
	
	for (int m=0; m<numM; m++) { MassH[m]=0; }
	
	for (int m=0; m<numM; m++) {
		for (int n=0; n<numHT[m]; n++) {
			for (int p=0; p<this->numN; p++) {
			
				MassH[m] += this->WtHT[ind][p];
			}
			ind++;
			//cout<< "M: "<<*(WtHTn0[m]+n)<<", "<<*(WtHTn1[m]+n)<<", "<<*(WtHTn2[m]+n)<<", "<<*(WtHTn3[m]+n)<<endl;
		}
	}
	
	for (int m=0; m<numM; m++) { TotalM += MassH[m]; }
	//cout<< "H: "<<MassH[0]<<", "<<MassH[1]<<", "<<MassH[2]<<", "<<MassH[3]<<endl;
	
	for (int m=0; m<numM; m++) {
		this->probModel[m] = MassH[m] / TotalM;
	}
    
    return 0;
}


int EC2 :: randomDesign (int Used[], int numR) {
	
	bool notdone = 1;
	int low = 0, high = this->num_Designs-1;
	int range = high-low;
	int d;
	
	srand((unsigned)time(0));
	
	while (notdone) {
		
		// choose a random design i
		d = low + int(range*(rand()/(RAND_MAX + 1.0)));
        
        this->best_Design[0] = *(this->all_Design0+d);
        this->best_Design[1] = *(this->all_Design1+d);
		
		// check if it has been used already
		bool isUsed = 0;
		for (int r=0; r<numR; r++) {
			if (d==Used[r]) { isUsed = 1; }
		}
		if (isUsed) { notdone = 1; } else { notdone = 0; }
	}
	
	return d;
}
