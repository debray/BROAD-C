/*
 *  UtilitySpec.cpp
 *  Definitions of Utility functions derived from Utility class
 *
 *  Created by Debajyoti Ray on 20/04/2010.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "UtilitySpec.hpp"

// model 0 - Functional Prospect Theory
double UtilityFPT :: calcUtilityFPT() {
	
	double U = 0;
	double Prob = 0;
	double tempX = 0;
	
	// Lottery is (X, P) - first num_Out elements is outcome, next is probs
	// Lottery[2*num_Out] = (X[num_Out], P[num_Out])
	
	for (int i=0; i<this->num_Out; i++) {
		// take logs of Pi and Xi, add and exponentiate
		// exponentiate Pi twice instead of using pow function
		if (*(this->Lottery+num_Out+i)<=0) {
			Prob = 0;
		}
		else if (*(this->Lottery+num_Out+i)==1) {
			Prob = 1;
		}
		else {
			Prob = exp(this->Params[2]*log(-log(*(this->Lottery+num_Out+i))));
			Prob = exp(-Prob);
		}
		
		// exponentiate abs(Xi), multiply by lambda if neg
		if (*(this->Lottery+i)<0) {
			tempX = exp(this->Params[0]*log(-*(this->Lottery+i)));
			tempX = -this->Params[1]*tempX;
		}
		else if (*(this->Lottery+i)==0) {
			tempX = 0;
		}
		else {
			tempX = exp(this->Params[0]*log(*(this->Lottery+i)));
		}
		
		U += Prob*tempX;
	}
	
	return U;
}

// model 1 - Cumulative Prospect Theory
double UtilityCPT :: calcUtilityCPT() { 

    double U = 0;
    double tempX = 0;
    double ProbWt[this->num_Out];
    
    int n = this->num_Out;
    
    int IndPos[n]; int IndNeg[n];
    int numPos = 0, numNeg = 0;
    
    // Separate into Negative and Positive Domains
    // Find the positive and negative indices
    for (int i=0; i<n; i++) {
        if (*(this->Lottery+i) < 0) { IndNeg[numNeg]=i; numNeg++; }
        else { IndPos[numPos]=i; numPos++; }
    }
    
    double cumProb = 0., cumProbWt = 0.;
    
    int ind = 0, ind1 = 0;
    // Calculate decision weights in Negative Domain
    ind = IndNeg[0];
    // For lowest index, get Prelec weight
    //ProbWt[ind] = PrelecWt();
    if (*(this->Lottery+num_Out+ind)<=0) {
        ProbWt[ind] = 0;
    }
    else if (*(this->Lottery+num_Out+ind)==1) {
        ProbWt[ind] = 1;
    }
    else {
        ProbWt[ind] = exp(this->Params[2]*log(-log(*(this->Lottery+num_Out+ind))));
        ProbWt[ind] = exp(-ProbWt[ind]);
    }
    
    // Rest of the negative weights, from lowest index 1
    for (int i=1; i<numNeg; i++) {
        ind = IndNeg[ind];
        
        cumProb = 0., cumProbWt = 0.;
        // get Cumulative probability - at least as worse as i
        
        for (int j=0; j<=i; j++) {
            cumProb += *(this->Lottery+num_Out+IndNeg[j]);
        }
        
        if (cumProb<=0) { cumProbWt = 0; }
        else if (cumProb==1) { cumProbWt = 1; }
        else {
            cumProbWt = exp(this->Params[2]*log(-log(cumProb)));
            cumProbWt = exp(-cumProbWt);
        }
        
        ind1 = IndNeg[i-1];
        ProbWt[ind] = cumProbWt - ProbWt[ind1];
    }
    
    // Calculate decision weights in Positive Domain
    ind = IndPos[numPos-1];
    // For highest index, get Prelec weight
    //ProbWt[ind] = PrelecWt();
    if (*(this->Lottery+num_Out+ind)<=0) {
        ProbWt[ind] = 0;
    }
    else if (*(this->Lottery+num_Out+ind)==1) {
        ProbWt[ind] = 1;
    }
    else {
        ProbWt[ind] = exp(this->Params[2]*log(-log(*(this->Lottery+num_Out+ind))));
        ProbWt[ind] = exp(-ProbWt[ind]);
    }
    
    // Rest of the positive weights, from highest index numPos-1
    for (int i=(numPos-1)-1; i>=0; i--) {
        ind = IndPos[i];
        
        cumProb = 0., cumProbWt = 0.;
        // get Cumulative probability - at least as good as i
        for (int j=i; j<numPos; j++) {
            cumProb += *(this->Lottery+num_Out+IndPos[j]);
        }
        
        if (cumProb<=0) { cumProbWt = 0; }
        else if (cumProb==1) { cumProbWt = 1; }
        else {
            cumProbWt = exp(this->Params[2]*log(-log(cumProb)));
            cumProbWt = exp(-cumProbWt);
        }
        
        ind1 = IndPos[i+1];
        ProbWt[ind] = cumProbWt - ProbWt[ind1];
    }
    
    // Combine decision weights and utility for combined utility
    for (int i=0; i<this->num_Out; i++) {
        
        // exponentiate abs(Xi), multiply by lambda if neg
		if (*(this->Lottery+i)<0) {
			tempX = exp(this->Params[0]*log(-*(this->Lottery+i)));
			tempX = -this->Params[1]*tempX;
		}
		else if (*(this->Lottery+i)==0) {
			tempX = 0;
		}
		else {
			tempX = exp(this->Params[0]*log(*(this->Lottery+i)));
		}
		
		U += ProbWt[i] * tempX;
    }

    return U;
}

// model 2 - Mean-Variance-Skewness
double UtilityMVS :: calcUtilityMVS() {
	
	double Mu = 0;
	// calculate mean
	for (int i=0; i<this->num_Out; i++) {
		Mu += *(this->Lottery+num_Out+i)*(*(this->Lottery+i));
	}
	Mu = Mu / this->num_Out;
	
	double Var = 0;
	// calculate variance
	for (int i=0; i<this->num_Out; i++) {
		Var += *(this->Lottery+num_Out+i)*(*(this->Lottery+i)-Mu)*(*(this->Lottery+i)-Mu);
	}
	
	double Skew = 0;
	// calculate skewness
	for (int i=0; i<this->num_Out; i++) {
		Skew += *(this->Lottery+num_Out+i)*
				(*(this->Lottery+i)-Mu)*(*(this->Lottery+i)-Mu)*(*(this->Lottery+i)-Mu);
	}
	
	double U = 0;
	U = this->Params[0]*Mu - this->Params[1]*Var + this->Params[2]*Skew;
	
	return U;
}

// model 3 - Normalized MVS
double UtilityNMVS :: calcUtilityNMVS() {

    double U;
    
    double Mu = 0;
	// calculate mean
	for (int i=0; i<this->num_Out; i++) {
		Mu += *(this->Lottery+num_Out+i)*(*(this->Lottery+i));
	}
	Mu = Mu / this->num_Out;
    
    double Std = 0;
	// calculate standard deviation
	for (int i=0; i<this->num_Out; i++) {
		Std += *(this->Lottery+num_Out+i)*(*(this->Lottery+i)-Mu)*(*(this->Lottery+i)-Mu);
	}
	Std = sqrt(Std);
    
    double Skew = 0;
	// calculate skewness
	for (int i=0; i<this->num_Out; i++) {
		Skew += *(this->Lottery+num_Out+i)*
        (*(this->Lottery+i)-Mu)*(*(this->Lottery+i)-Mu)*(*(this->Lottery+i)-Mu);
	}
	if (Std==0) {
		Skew = 0;
	}
	else {
		Skew = Skew / (Std*Std*Std);
	}
    
	U = this->Params[0]*Mu - this->Params[1]*Std + this->Params[2]*Skew;
    
    return U;
}

// model 4 - Expected Utility
double UtilityEU :: calcUtilityEU() {
	
	// Lottery is (X, P) - first num_Out elements is outcome, next is probs
	// Lottery[2*num_Out] = (X[num_Out], P[num_Out])
	
	double Mu = 0;
	// calculate mean
	for (int i=0; i<this->num_Out; i++) {
		Mu += *(this->Lottery+num_Out+i)*(*(this->Lottery+i));
	}
	Mu = Mu / this->num_Out;
	
	return Mu;
}

// model 5 - Constant Relative Risk Aversion
double UtilityCRRA :: calcUtilityCRRA() {
	
	// Lottery is (X, P) - first num_Out elements is outcome, next is probs
	// Lottery[2*num_Out] = (X[num_Out], P[num_Out])
	
	double U=0., logX=0.; 
	double Endow = 20;
	double X = Endow;
	
	if (this->Params==1) {
		for (int i=0; i<this->num_Out; i++) {
			
			X = X + *(this->Lottery+i);
			if (X==0) { logX=0; }
			else { logX = log(X); };
			U += *(this->Lottery+num_Out+i)*logX;
		}
	}
	else {
		for (int i=0; i<this->num_Out; i++) {
			
			X = X + *(this->Lottery+i);
			if (X==0) { logX=0; }
			else { logX = log(X); };
			U += *(this->Lottery+num_Out+i)*(exp((1-this->Params)*logX) / (1-this->Params));
		}
	}
	
	return U;
}
