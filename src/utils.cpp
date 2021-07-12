#include "..\include\defines.h"

void printTAndNT (){
    set <char> ::iterator it;
    cout << "------------terminators are as follows------------" << endl;
    for (it = terminators.begin(); it != terminators.end(); it++ )
        cout << *it << ' ';
    cout << endl << "--------------------------------------------------" << endl << endl;
    cout << "----------non_terminators are as follows----------" << endl;
    for (it = non_terminators.begin(); it != non_terminators.end(); it++ )
        cout << *it << ' ';
    cout << endl << "--------------------------------------------------" << endl << endl;
}

void printGrammar () {
    vector <Generative*> ::iterator it;
    cout << "-------------the grammar is as follows-------------" << endl;
    for (it = vt.begin(); it != vt.end(); it++)
        cout << (*it) -> left << " -> " << (*it) -> right << endl;
    cout << "---------------------------------------------------" << endl << endl;
}

void printExtendedItems () {
    vector <Item*> ::iterator it;
    cout << "--------the extended item set is as follows--------" << endl;
    for (it = extendedItems.begin(); it != extendedItems.end(); it++) {
        string str = "";
        str = to_string((*it) -> id) + ". " + (*it) -> left + " -> " \
        + (*it) -> right.insert((*it) -> dot, "·");
        cout << str << endl;
    }
    cout << "---------------------------------------------------" << endl << endl;
}

bool isTerminator (char &c) {
    set <char> ::iterator it;
    for (it = terminators.begin(); it != terminators.end(); it++) {
        if (*it == c)
            return 1;
    }
    return -1;
}

bool isNon_Terminator (char &c) {
    set <char> ::iterator it;
    for (it = non_terminators.begin(); it != non_terminators.end(); it++) {
        if (*it == c)
            return 1;
    }
    return -1;
}