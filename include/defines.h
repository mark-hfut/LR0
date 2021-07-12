#pragma once
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <stack>
#include <vector>
#include <set>
#include <iomanip>
#include <sstream>
using namespace std;

#define PATH "..\\files\\grammar"
#define MAXNUM 101
/*-------------------------data structure-------------------------*/
set <char> terminators;
set <char> non_terminators;

map <int, map <string, string> > ACTION_table;
map <int, map <string, int> > GOTO_table;

stack <char> symbol;
stack <int> status;

struct Generative {             //产生式
    string left;
    string right;
};
vector <Generative*> vt;        //记录已读入的产生式

struct Item {
    int id = 0;
    string left;
    string right;
    int dot = 0;
    multimap <char, int> NFAitem;
};
vector <Item*> extendedItems;

struct ItemSet {                //确定有限自动机的单个项目
    int id;
    vector <Item*> thisItemSet;
    map <char, int> next;
};
vector <ItemSet*> itemSetFam;   //Item set specific family项目规范集族

/*-------------------------end-------------------------*/


/*-------------------------utils-------------------------*/
void printTAndNT ();
void printGrammar ();
void printExtendedItems ();
bool isTerminator (char &c);
bool isNon_Terminator (char &c);


bool canMoveDot (const string &str);
void moveDot (string &str);
string deleteDot (const string &str);
int generativeNum (const string &left, const string &right);
void printTAndNT ();
void printTables ();
void printStatus ();
string printStatusStack ();
string printSymbolStakc ();
string getString ();
string stringToInt ();
string excludeLastSymbol (const string &str);
string leftAddDot (string &str);
void resetStack ();
void addRear (const string &str);
void makeTSet (const string &str);
void makeNTSet (const string &str);
void printGrammar ();

/*-------------------------end-------------------------*/


/*-------------------------functions-------------------------*/
void readIn ();
void makeTSet (const string &str);
void makeNTSet (const string &str);
void makeGenerativeSet (const string &s1, const string s3);
void extendItems ();
void makeItemSetFamily ();


void makeGenerativeSet (const string &s1, const string s3);
void makeItemSetFamily ();
void extend (vector <string> &left, vector <string> &right, string x);
void extendFirstLine (ItemSet *& itemSet);
void createItem (string lf, string rg);
int excludeDupli ();
void addItem ();
void initFirstItem ();
void readIn ();
void initTables ();
bool startAnalysis (const string &str);

/*-------------------------end-------------------------*/