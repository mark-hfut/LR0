#pragma once
#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <stack>
#include <vector>
#include <set>
#include <queue>
#include <iomanip>
#include <sstream>
#include <algorithm>
using namespace std;

#define PATH "..\\files\\grammar"
#define MAXNUM 101
/*-------------------------data structure-------------------------*/
vector <char> terminators;
vector <char> non_terminators;

vector<vector<string>> ACTION_table;
vector<vector<int>> GOTO_table;

stack <char> symbolStack;
stack <int> statusStack;

struct Generative {             //产生式
    int id = -1;
    string left;
    string right;
};
vector <Generative*> vt;        //记录已读入的产生式

struct Item {
    int id = 0;
    int generativeId = -1;
    string left;
    string right;
    int dot = 0;
    multimap <char, int> NFAitem;
    int itemSetNumber = -1;
};
vector <Item*> extendedItems;

struct ItemSet {                //确定有限自动机的单个项目
    int id = 0;
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
bool isFirstItem (Item *item, char &c);
int returnTerminatorId(char c);
int returnNonTerminatorId(char c);
Item* returnItemById(int &id);
bool isExtendedItemInThisSet(int itemId, int itemSetId);
bool isInTerminators(char c);
bool isInNonTerminators(char c);
bool cmp(const ItemSet &a, const ItemSet &b);
string outStatusStack(stack<int> status);
string outSymbolStack(stack<char> symbol);
string getStr(const string &str, const int &p);
bool isInItemSetFam(int id);

/*-------------------------end-------------------------*/


/*-------------------------functions-------------------------*/
void readIn ();
void makeTSet (const string &str);
void makeNTSet (const string &str);
void makeGenerativeSet(const string &s1, const string s3, int &id);
void makeItemSetFamily ();
string solveActionValue(int status, int id);
int solveGotoValue(int status, int id);
void startAnalysis();

/*-------------------------end-------------------------*/