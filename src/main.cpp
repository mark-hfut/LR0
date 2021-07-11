#include ".\utils.cpp"
#include "..\include\defines.h"

int main () {
	string str;
	readIn();
	printTAndNT();
	initFirstItem();
	addItem();
	printStatus();
	initTables();
	printTables();

	while(true){
		printf("input string to analyse, end with '#'\n \
		input exit to exit the system\n");
		cin >> str;

		if (str == "exit")
			break;

		addRear(str);
		bool res = startAnalysis(str);
		if(res){
			cout<<"input string "<<excludeLastSymbol(str)<<" grammatically correct"<<endl;
			cout<<"------------------------------------------"<<endl; 
		}
		else{
			cout<<"input string "<<excludeLastSymbol(str)<<" grammatically incorrect"<<endl; 
			cout<<"------------------------------------------"<<endl; 
		}
		resetStack();
	}
}