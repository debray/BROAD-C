/*
 *  utilitySpec.hpp
 *  Specifications of subclasses of Utility class
 *
 *  Created by Debajyoti Ray on 20/04/2010.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "Utility.hpp"

// model 0 - Functional Prospect Theory
class UtilityFPT : public Utility {
	
public:
	double calcUtilityFPT();
};

// model 1 - Cumulative Prospect Theory
class UtilityCPT : public Utility {

public:
    double calcUtilityCPT();
};

// model 2 - Mean-Variance-Skewness
class UtilityMVS : public Utility {
	
public:
	double calcUtilityMVS();
};

// model 3 - Normalized Mean-Variance-Skewness
class UtilityNMVS : public Utility {

public:
    double calcUtilityNMVS();
};

// model 4
class UtilityEU : public Utility0 {
	
public:
	double calcUtilityEU();
};

// model 5
class UtilityCRRA : public Utility1 {
	
public:
	double calcUtilityCRRA();
};
