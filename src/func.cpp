#include "..\include\defines.h"

void readIn(){
    ifstream infile;
    infile.open(PATH, ios::in);
    if (!infile){
        cout << "open fail " << PATH << endl;
        system("pause");
        exit(-1);
    }
    
    while(!infile.eof()){
        Generative *tmp = new Generative();
        string s1, s2, s3;
        infile >> s1 >> s2 >> s3;
        tmp -> left = s1;
        tmp -> right = s3;
        vt.push_back(tmp);
        makeTSet(s1);
        makeNTSet(s2);      
    }
    printf("Grammar read from file is as followed\n");
    printGrammar();
}

void makeTSet (const string &str) {
    if (str.size() == 1 && str[0] > 'A' && str[0] < 'Z')
        terminators.insert(str[0]);
    else 
        cout << "terminator read error" << endl;
}

void makeNTSet (const string &str) {
    for (int i = 0; i < str.size(); i++) {
        if (str[0] > 'a' && str[0] < 'z')
            non_terminators.insert(str[i]);
    }
}

void extendItems() {

}