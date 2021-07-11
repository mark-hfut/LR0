#include "..\include\defines.h"

void printTAndNT (){
    set <char> ::iterator it;
    for (it = terminators.begin(); it != terminators.end(); it++ )
        cout << *it << ' ';
    cout << endl;
    for (it = non_terminators.begin(); it != terminators.end(); it++ )
        cout << *it << ' ';
    cout << endl;
}