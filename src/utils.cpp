#include "..\include\defines.h"

void printTAndNT (){
    vector <char> ::iterator it;
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
        string str = "", right = (*it) -> right;
        str = to_string((*it) -> id) + ". " + (*it) -> left + " -> " \
        + right.insert((*it) -> dot, "·");
        cout << str << endl;
    }
    cout << "---------------------------------------------------" << endl << endl;
}

void printNFA () {
    vector <Item*> ::iterator it;
    multimap <char, int> ::iterator iter;
    cout << "---------------the NFA is as follows---------------" << endl;
    for (it = extendedItems.begin(); it != extendedItems.end(); it++) {
        string str = "", right = (*it) -> right;
        str = to_string((*it) -> id) + ". " + (*it) -> left + " -> " \
        + right.insert((*it) -> dot, "·");
        for (iter = (*it) -> NFAitem.begin(); iter != (*it) -> NFAitem.end(); iter++)
            str += " (" + string(1, iter -> first) + ", " + to_string(iter -> second) + ") ";
        cout << str << endl;
    }
    cout << "---------------------------------------------------" << endl << endl;
}

void printItemSet (ItemSet* itemSet) {
    vector<Item *> ::iterator it;
    for (it = itemSet->thisItemSet.begin(); it != itemSet->thisItemSet.end(); it++) {
        string str = "", right = (*it) -> right;
        str = to_string((*it) -> id) + ". " + (*it) -> left + " -> " \
        + right.insert((*it) -> dot, "·");
        cout << str << endl;
    }
}

void printItemSetFamily() {
    vector<ItemSet *> ::iterator it;
    map<char, int> ::iterator mp;
    cout << "---------the Item Set Family is as follows---------" << endl;
    for (it = itemSetFam.begin(); it != itemSetFam.end(); it++) {
        cout << "---------------" << (*it)->id << ": " << "---------------" << endl;
        ItemSet *itemSet = (*it);
        printItemSet(itemSet);
        cout << endl << "edges of this set : " << endl;
        for (mp = (*it)->next.begin(); mp != (*it)->next.end(); mp++)
            cout << " (" << mp->first << ", " << mp->second << ") ";
        cout << endl << "---------------------------------" << endl << endl;
    }
    cout << endl;
}

void printTables() {
    vector<char>::iterator it1, it2;
    cout << "------------------------ACTION------------------------" << endl;
    for (it1 = terminators.begin(); it1 != terminators.end(); it1++ )
        cout << '\t' << *it1;
    cout << endl;
    for (int i = 0; i < ACTION_table.size(); i++) {
        cout << i << '\t';
        for (int j = 0; j < ACTION_table[0].size(); j++) {
            if (ACTION_table[i][j] != "err")
                cout << ACTION_table[i][j];
            cout << '\t';
        }
        cout << endl;
    }
    cout << "------------------------------------------------------" << endl;
    cout << "-------------------------GOTO-------------------------" << endl;
    for (it2 = non_terminators.begin(); it2 != non_terminators.end(); it2++ )
        cout << '\t' << *it2;
    cout << endl;
    for (int i = 0; i < GOTO_table.size(); i++) {
        cout << i << '\t';
        for (int j = 0; j < GOTO_table[0].size(); j++) {
            if (GOTO_table[i][j] != -1)
                cout << GOTO_table[i][j];
            cout << '\t';
        }
        cout << endl;
    }
    cout << "-------------------------------------------------------" << endl;
}

string outStatusStack(stack<int>status) {
    string res = "";
	int tmp; 
	//stringstream ss; 
	while(status.size()){
		tmp = status.top();
		//ss<<tmp;
		//res = ss.str() + res;
		res = " " + to_string((long long)tmp) + res; 
		status.pop();
	}
	//cout<<"输出状态栈 "<<res<<endl; 
	return res; 
}

string outSymbolStack(stack<char>symbol) {
	string res;
	string tmp; 
	while(symbol.size()){
		tmp = symbol.top();
		res = tmp + res; 
		symbol.pop();
	}
	return res; 
}

string getStr(const string & str, const int & p) {//截取串str从下标p之后的串
	string res = "";
	int length = str.length();
	for(int i = p; i < length; i++)
		res += str[i];
	return res; 
}

bool isTerminator (char &c) {
    vector <char> ::iterator it;
    for (it = terminators.begin(); it != terminators.end(); it++) {
        if ((*it) == c)
            return 1;
    }
    return -1;
}

bool isNon_Terminator (char &c) {
    vector <char> ::iterator it;
    for (it = non_terminators.begin(); it != non_terminators.end(); it++) {
        if ((*it) == c)
            return 1;
    }
    return -1;
}

bool isFirstItem(Item *item, char &symbol) {
    if (item->dot == 0 && item->left[0] == symbol)
        return true;
    return false;
}

bool isExtendedItemInThisSet(int itemId, int itemSetId) {
    vector<Item*> ::iterator it;
    for (it = extendedItems.begin(); it != extendedItems.end(); it++)
        if ((*it)->id == itemId && (*it)->itemSetNumber == itemSetId)
            return true;
    return false;
}

Item* returnItemById(int &id) {
    vector <Item*> ::iterator it;
    for (it = extendedItems.begin(); it != extendedItems.end(); it++)
        if ((*it)->id == id)
            return (*it);
    return NULL;
}

int returnTerminatorId(char c) {
    for (int i = 0; i < terminators.size(); i++)
        if (terminators[i] == c)
            return i;
    return -1;
}

int returnNonTerminatorId(char c) {
    for (int i = 0; i < non_terminators.size(); i++)
        if (non_terminators[i] == c)
            return i;
    return -1;
}

bool isInTerminators(char c) {
    for (int i = 0; i < terminators.size(); i++)
        if (terminators[i] == c)
            return true;
    return false;
}

bool isInNonTerminators(char c) {
    for (int i = 0; i < non_terminators.size(); i++)
        if (non_terminators[i] == c)
            return true;
    return false;
}

bool isInItemSetFam (int id) {
    vector<ItemSet*>::iterator it;
    for (it = itemSetFam.begin(); it != itemSetFam.end(); it++)
        if ((*it)->id == id)
            return true;
    return false;
}