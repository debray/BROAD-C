/*
 *  testLikelihood.cpp
 *  Test the Integrate class & subclass, and their functions
 *
 *  Created by Debajyoti Ray on 5/4/10.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "testSubject.hpp"

int testSubjectRandom(int id) {
	
	ofstream file;
	
	char name[70];
	sprintf(name, "Results/Random-Subject-%d.dat", id);
	file.open(name);
	
	file << "Subject " << id << "\n";
	
	// calculate running time
	//clock_t t_start, t_response;
    time_t t_start, t_response;
	
	// initialize objects
	LikelihoodFPT testLikFPT; LikelihoodCPT testLikCPT;
    LikelihoodMVS testLikMVS; LikelihoodNMVS testLikNMVS;
	LikelihoodEU testLikEU; 
    LikelihoodCRRA testLikCRRA;
	EC2 testEC;
	
	// unchangeable
	testLikFPT.num_Params=3; testLikCPT.num_Params=3;
    testLikMVS.num_Params=3; testLikNMVS.num_Params=3; 
    testLikCRRA.num_Params=1;
	
	// to increase this, need to generate bigger simplex
	int num_Out = 3;
	testLikFPT.num_Out=num_Out; testLikCPT.num_Out=num_Out;
    testLikMVS.num_Out=num_Out; testLikNMVS.num_Out=num_Out;
	testLikEU.num_Out=num_Out; 
    testLikCRRA.num_Out=num_Out;
    
	int gridSize = 5;
	
	// make da grid
	testLikFPT.gridSize = gridSize;
	testLikFPT.MinR[0]=0.5; testLikFPT.MinR[1]=1.0; testLikFPT.MinR[2]=0.4;
	testLikFPT.MaxR[0]=1.1; testLikFPT.MaxR[1]=3.0; testLikFPT.MaxR[2]=1.0;
    
    testLikCPT.gridSize = gridSize;
	testLikCPT.MinR[0]=0.5; testLikCPT.MinR[1]=1.0; testLikCPT.MinR[2]=0.4;
	testLikCPT.MaxR[0]=1.1; testLikCPT.MaxR[1]=3.0; testLikCPT.MaxR[2]=1.0;
	
	testLikMVS.gridSize = gridSize;
	testLikMVS.MinR[0]=0.6; testLikMVS.MinR[1]=0.0005; testLikMVS.MinR[2]=0.0;
	testLikMVS.MaxR[0]=1.0; testLikMVS.MaxR[1]=0.0105; testLikMVS.MaxR[2]=0.0005;
    
    testLikNMVS.gridSize = gridSize;
	testLikNMVS.MinR[0]=0.6; testLikNMVS.MinR[1]=0.05; testLikNMVS.MinR[2]=0.0;
	testLikNMVS.MaxR[0]=1.0; testLikNMVS.MaxR[1]=0.45; testLikNMVS.MaxR[2]=0.4;
	
	testLikCRRA.gridSize = gridSize;
	testLikCRRA.MinR=0.2;
	testLikCRRA.MaxR=1.0;
	
	// initialize parameters
	for (int i=0; i<3; i++) {
		testLikFPT.Params[i]=testLikFPT.MinR[i]; 
        testLikCPT.Params[i]=testLikCPT.MinR[i];
		testLikMVS.Params[i]=testLikMVS.MinR[i];
        testLikNMVS.Params[i]=testLikNMVS.MinR[i];
	}
	for (int i=0; i<1; i++) {
		testLikCRRA.Params=testLikCRRA.MinR; 
	}
    
    int numM = 6;
	testEC.numM = numM;
	// define EC2 noise object
	for (int i=0; i<numM; i++) { testEC.probModel[i] = 1./numM; }
    
    // noise parameters
	testEC.numN = 4;		// number of noise levels
	testEC.noiseProb[0]=0; testEC.noiseProb[1]=0.05; 
	testEC.noiseProb[2]=0.10; testEC.noiseProb[3]=0.30;		// noise probs
	testEC.maxErr[0]=0; testEC.maxErr[1]=4; 
	testEC.maxErr[2]=7; testEC.maxErr[3]=15;			// max num of errors
	
	int num_Designs = 32000;
	double * Design0[num_Designs], *Design1[num_Designs];
	makeAllDesigns(Design0, Design1, num_Designs);
	
	testEC.num_Designs = num_Designs;
	testEC.all_Design0 = &Design0[0];
	testEC.all_Design1 = &Design1[0];
    
    // initially all (H,Theta) are valid
	int numHT[numM];
	numHT[0]=pow(testLikFPT.gridSize, testLikFPT.num_Params);
    numHT[1]=pow(testLikCPT.gridSize, testLikCPT.num_Params);
	numHT[2]=pow(testLikMVS.gridSize, testLikMVS.num_Params);
    numHT[3]=pow(testLikNMVS.gridSize, testLikNMVS.num_Params);
	numHT[4]=1;
	numHT[5]=pow(testLikCRRA.gridSize, testLikCRRA.num_Params);
    
    int ind;
	for (int p=0; p<testEC.numN; p++) {
		ind = 0;
		for (int n=0; n<numHT[0]; n++) { testEC.WtHT[ind][p] = 1./(numM*numHT[0]); ind++; }
		for (int n=0; n<numHT[1]; n++) { testEC.WtHT[ind][p] = 1./(numM*numHT[1]); ind++; }
		for (int n=0; n<numHT[2]; n++) { testEC.WtHT[ind][p] = 1./(numM*numHT[2]); ind++; }
		for (int n=0; n<numHT[3]; n++) { testEC.WtHT[ind][p] = 1./(numM*numHT[3]); ind++; }
        for (int n=0; n<numHT[4]; n++) { testEC.WtHT[ind][p] = 1./(numM*numHT[4]); ind++; }
		for (int n=0; n<numHT[5]; n++) { testEC.WtHT[ind][p] = 1./(numM*numHT[5]); ind++; }
	}
    
    // counts for number of errors
	int * ErrHT[numM];		// array of pointers to indicator arrays
	
	int * Err_FPT = new int[numHT[0]];
    int * Err_CPT = new int[numHT[1]];
	int * Err_MVS = new int[numHT[2]];
    int * Err_NMVS = new int[numHT[3]];
	int * Err_EU = new int[numHT[4]];
	int * Err_CRRA = new int[numHT[5]];
    
    for (int n=0; n<numHT[0]; n++) { Err_FPT[n] = 0; }
    for (int n=0; n<numHT[1]; n++) { Err_CPT[n] = 0; }
	for (int n=0; n<numHT[2]; n++) { Err_MVS[n] = 0; }
    for (int n=0; n<numHT[3]; n++) { Err_NMVS[n] = 0; }
	for (int n=0; n<numHT[4]; n++) { Err_EU[n] = 0; }
	for (int n=0; n<numHT[5]; n++) { Err_CRRA[n] = 0; }
    
    ErrHT[0] = Err_FPT;
    ErrHT[1] = Err_CPT;
    ErrHT[2] = Err_MVS; 
    ErrHT[3] = Err_NMVS; 
    ErrHT[4] = Err_EU; 
    ErrHT[5] = Err_CRRA;
	
	cout << "Starting Experiment..." << endl;
	
	int maxRounds = 30;
	int UsedD[maxRounds];
    
    file.close();
    
	// loop over best designs
	for (int numRound=0; numRound<maxRounds; numRound++) {
        
        file.open(name, ios::app);
		
		file << numRound << "\n";
	
		UsedD[numRound] = testEC.randomDesign(UsedD, numRound);
	
		double *Lotto0, *Lotto1;
		Lotto0 = testEC.best_Design[0]; Lotto1 = testEC.best_Design[1];	
        
        system("clear");
        
        //cout << "Rounds Left: " << maxRounds-numRound << endl << endl;
        cout << "\n CHOOSE: \n\n";
		cout << " Lottery 1:" << endl << endl;
        cout << "Amount:     $"<< *(Lotto0) << ", $" << *(Lotto0+1) << ", $" << *(Lotto0+2) << endl << endl;
        
        // middle prob
        double midProb = *(Lotto0+4);
        if (midProb < 0.0001) { midProb = 0.0; }
		cout << "Probability: " << *(Lotto0+3) << ", " << midProb << ", " << *(Lotto0+5) << endl << endl;
        
		cout << " OR \n\n"; 
        cout << "Lottery 2: " << endl << endl;
        cout << "Amount:     $" << *(Lotto1) << ", $" << *(Lotto1+1) << ", $" << *(Lotto1+2) << endl << endl;
        
        midProb = *(Lotto1+4);
        if (midProb < 0.0001) { midProb = 0.0; }
		cout << "Probability: "<< *(Lotto1+3) << ", " << midProb << ", " << *(Lotto1+5) << endl << endl;
		file << *(Lotto0) << "," << *(Lotto0+1) << "," << *(Lotto0+2) << "\n";
		file << *(Lotto0+3) << "," << *(Lotto0+4) << "," << *(Lotto0+5) << "\n";
		file << *(Lotto1) << "," << *(Lotto1+1) << "," << *(Lotto1+2) << "\n";
		file << *(Lotto1+3) << "," << *(Lotto1+4) << "," << *(Lotto1+5) << "\n";
        
        // Output to .gpi file to be read by gnuplot
        ofstream GUIfile;
        
        char GUIname[50];
        sprintf(GUIname, "GUI.dat");
        GUIfile.open(GUIname);
        GUIfile << *(Lotto0) << " " << *(Lotto0+3) << " " << abs((int)*(Lotto0))/5 << endl;
        GUIfile << *(Lotto0+1) << " " << *(Lotto0+1+3) << " " << abs((int)*(Lotto0+1))/5 << endl;
        GUIfile << *(Lotto0+2) << " " << *(Lotto0+2+3) << " " << abs((int)*(Lotto0+2))/5 << endl;
        GUIfile << *(Lotto1) << " " << *(Lotto1+3) << " " << abs((int)*(Lotto1))/5 << endl;
        GUIfile << *(Lotto1+1) << " " << *(Lotto1+1+3) << " " << abs((int)*(Lotto1+1))/5 << endl;
        GUIfile << *(Lotto1+2) << " " << *(Lotto1+2+3) << " " << abs((int)*(Lotto1+2))/5 << endl;
        
        GUIfile.close();
        
        // Start response time
        //t_start = clock();
        t_start = time(NULL);
        
        // make graph - output as Output.ps file
        int Status = system("gnuplot makeGUI.gpi");
        // display graph
        system("gv Output.ps -geometry 950x500+0+0");
	
		// assign design to objects
		testLikFPT.Design[0]=testEC.best_Design[0]; testLikFPT.Design[1]=testEC.best_Design[1];
        testLikCPT.Design[0]=testEC.best_Design[0]; testLikCPT.Design[1]=testEC.best_Design[1];
		testLikMVS.Design[0]=testEC.best_Design[0]; testLikMVS.Design[1]=testEC.best_Design[1];
        testLikNMVS.Design[0]=testEC.best_Design[0]; testLikNMVS.Design[1]=testEC.best_Design[1];
		testLikEU.Design[0]=testEC.best_Design[0]; testLikEU.Design[1]=testEC.best_Design[1];
		testLikCRRA.Design[0]=testEC.best_Design[0]; testLikCRRA.Design[1]=testEC.best_Design[1];
	
		// record observation from keyboard input
		int ynow;
		cout << "Enter choice: 1 or 2: ";
		cin >> ynow;
        // Convert to 0 or 1
        ynow = ynow - 1;
		
		bool invalid = 1;
		if (ynow == 0 || ynow == 1) { invalid = 0; }
		while (invalid) {
			cout << endl << "Invalid Input!" << endl;
			cout << "Enter choice: 0 or 1: ";
			cin >> ynow;
			if (ynow == 0 || ynow == 1) { invalid = 0; }
		}
		file << ynow << "\n";
        
        //t_response = clock();
        t_response = time(NULL);
		
		// update which (H, Theta) are valid after observation i.e. ValidHT
		testEC.updateHT(testLikFPT, testLikCPT,
                        testLikMVS, testLikNMVS, 
                        testLikEU, testLikCRRA,
						ErrHT, UsedD[numRound], ynow);
        
        // calculate model posteriors from valid (H, Theta)
		testEC.updateModelPost(numHT);
		
        // output
		//cout << "Model posteriors: " << testEC.probModel[0] << ", " << testEC.probModel[1];
		//cout << ", " << testEC.probModel[2] << ", " << testEC.probModel[3];
        //cout << ", " << testEC.probModel[4] << ", " << testEC.probModel[5] << "\n";
        file << testEC.probModel[0] << "," << testEC.probModel[1];
        file << "," << testEC.probModel[2] << "," << testEC.probModel[3];
        file << "," << testEC.probModel[4] << "," << testEC.probModel[5] << "\n";
	
		//float RT = (float)t_response/CLOCKS_PER_SEC - (float)t_start/CLOCKS_PER_SEC;
        long RT = (long) (t_response - t_start);
		cout << "Response Time: " << RT << endl;
        file << RT << "\n";
        
		cout << endl << "REST" << endl << endl << endl;
        
        // remove the .gpi file
        system("rm GUI.dat");
        
        file.close();
	}
	
	return 0;	
}
