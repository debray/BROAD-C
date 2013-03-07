/*
 *  runThis.cpp
 *  
 *
 *  Created by Debajyoti Ray on 5/29/10.
 *  Copyright 2010 Caltech. All rights reserved.
 *
 */

#include "testSubject.hpp"

int main() {
    
    system("clear");
	
	int SubjectID;
	
	cout << "Enter Subject ID\n";
	cin >> SubjectID;
	
	char input[10];
    
    // Do 1 example round
    // testExample(SubjectID);
	
	testSubject(SubjectID);
	
	cout << "You can take a 2 minute break \n";
	cout << "Enter 0 when you are ready\n";
	cin >> input;
	
	testSubjectRandom(SubjectID);
	
	return 0;
}