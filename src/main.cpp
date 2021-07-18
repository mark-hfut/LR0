#include "..\include\defines.h"
#include "..\src\utils.cpp"
#include "..\src\func.cpp"

int main() {
    readIn();
    printTAndNT();
    printGrammar();
    extendItems();
    printExtendedItems();
    makeNFA();
    printNFA();
    makeItemSetFamily();
    printItemSetFamily();
    makeTable();
    printTables();
    printGrammar();
    startAnalysis();
}