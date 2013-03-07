/*
 *  Utility.h
 *  Defines necessary variables for Utility class 
 *
 *  Created by Debajyoti Ray on 20/04/2010.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

class Utility {

public:
	int num_Params;
	int num_Out;
	
	//double Params[num_Params];
	double Params[3];
	
	//double (X[num_Out], P[num_Out]);
	double *Lottery;
};


class Utility1 {
	
public:
	int num_Params;
	int num_Out;
	
	//double Params[num_Params];
	double Params;
	
	//double (X[num_Out], P[num_Out]);
	double *Lottery;
};


class Utility0 {
	
public:
	int num_Out;
	
	//double (X[num_Out], P[num_Out]);
	double *Lottery;
};
