#include "..\include\defines.h"

void readIn(){
    ifstream infile;
    infile.open(PATH, ios::in);
    if (!infile){
        cout << "open fail " << PATH << endl;
        system("pause");
        exit(-1);
    }
    int counter = 0;
    while(!infile.eof()){
        string s1, s2, s3;
        infile >> s1 >> s2 >> s3;
        makeNTSet(s1);
        makeTSet(s3);
        makeGenerativeSet(s1, s3, counter);
        counter++;
    }
    terminators.push_back('#');
    printGrammar();
}

void makeTSet (const string &str) {
    for (int i = 0; i < str.size(); i++)
        if (str[i] >= 'a' && str[i] <= 'z' && !isInTerminators(str[i]))
            terminators.push_back(str[i]);
}

void makeNTSet (const string &str) {
    if (str.size() == 1 && str[0] >= 'A' && str[0] <= 'Z' && !isInNonTerminators(str[0]))
        non_terminators.push_back(str[0]);
}

void makeGenerativeSet (const string &s1, const string s3, int &id) {
    Generative *tmp = new Generative();
    tmp->id = id;
    tmp->left = s1;
    tmp->right = s3;
    vt.push_back(tmp);
}

void extendItems() {
    int counter = 0, i = 0;
    vector <Generative*> ::iterator it;
    for (it = vt.begin(); it != vt.end(); it++) 
        for (i = 0; i <= (*it)->right.size(); i++){
            Item *tmp = new Item();
            tmp->id = counter;
            tmp->generativeId = (*it)->id;
            tmp->left = (*it)->left;
            tmp->right = (*it)->right;
            tmp->dot = i;
            extendedItems.push_back(tmp);
            counter++;
        }
}

void makeNFA() {
    vector <Item*> ::iterator it1, it2;
    for (it1 = extendedItems.begin(); it1 != extendedItems.end(); it1++) {     //非终结符的情况
        int dot = (*it1)->dot;
        string right = (*it1)->right;
        char symbol = right[(*it1)->dot];
        multimap <char, int> *mp = &((*it1)->NFAitem);
        if (dot < right.size()) {
            (*mp).insert(multimap <char, int> ::value_type(symbol, (*it1)->id + 1));
            if (isNon_Terminator(symbol))
                for (it2 = extendedItems.begin(); it2 != extendedItems.end(); it2++)
                    if (isFirstItem((*it2), symbol))
                        (*mp).insert(multimap <char, int> ::value_type('*', (*it2)->id));
        }
    }
}

void makeItemSetFamily () {
    queue <ItemSet*> Q;
    Item *item = extendedItems[0];
    multimap <char, int> ::iterator iter;
    vector <Item*> ::iterator it;
    int counter = 0;
    
    ItemSet *itemSet = new ItemSet();       // 初始化第一个itemSet并将第一个item塞进去
    itemSet->id = counter++;
    itemSet->thisItemSet.push_back(item);
    item->itemSetNumber = 0;
    itemSetFam.push_back(itemSet);
    Q.push((itemSet));
    while (!Q.empty()) {
        itemSet = Q.front();
        Q.pop();
        item = itemSet->thisItemSet[0];
        for (iter = item->NFAitem.begin(); iter != item->NFAitem.end(); iter++) {   //遍历map，往这个ItemSet里塞东西，相当于bfs中的初始化当前节点
            if (iter->first == '*') {
                Item *tmp = returnItemById(iter->second);
                if (item->itemSetNumber != -1 && tmp->itemSetNumber == -1) {
                    tmp->itemSetNumber = itemSet->id;
                    itemSet->thisItemSet.push_back(tmp);
                }
                else if (item->itemSetNumber != -1 && tmp->itemSetNumber != -1) {
                    Item *tmp = new Item();
                    tmp = returnItemById(iter->second);
                    tmp->itemSetNumber = itemSet->id;
                    itemSet->thisItemSet.push_back(tmp);
                }
            }
        }

        for (it = itemSet->thisItemSet.begin(); it != itemSet->thisItemSet.end(); it++) {   //遍历这个item集里的item，进行初始化和连接其他item集，相当与bfs中的拓展可连接节点
            for (iter = (*it)->NFAitem.begin(); iter != (*it)->NFAitem.end(); iter++) {     //遍历每个item的map
                if (iter->first != '*' ) {
                    Item *nextItem = returnItemById(iter->second);      //要连接的item
                    if (nextItem->itemSetNumber == -1) {                    //非自环连接
                        ItemSet *nextItemSet = new ItemSet();               //新建一个item set
                        nextItemSet->id = counter++;                        //给item set初始化
                        itemSet->next[iter->first] = nextItemSet->id;       //给当前item set连接
                        nextItem->itemSetNumber = nextItemSet->id;          //对这个即将塞入的元素进行设置
                        nextItemSet->thisItemSet.push_back(nextItem);       //给这个item set加入第一个元素
                        itemSetFam.push_back(nextItemSet);                  //把这个item set加入族中

                        Q.push(nextItemSet);
                    }

                    else if (nextItem->itemSetNumber != -1) {
                        itemSet->next[iter->first] = nextItem->itemSetNumber;
                    }
                }
            }
        if (!isInItemSetFam(itemSet->id))
            itemSetFam.push_back(itemSet);
        }
    }
}

void makeTable() {
    ACTION_table.resize(itemSetFam.size());
    GOTO_table.resize(itemSetFam.size());
    for (int i = 0 ; i < itemSetFam.size(); i++) {
        ACTION_table[i].resize(terminators.size());
        GOTO_table[i].resize(non_terminators.size());
    }
    for (int i = 0; i < ACTION_table.size(); i++)
        for (int j = 0; j < ACTION_table[0].size(); j++)
            ACTION_table[i][j] = solveActionValue(i, j);

    for (int i = 0; i < GOTO_table.size(); i++) 
        for (int j = 0; j < GOTO_table.size(); j++)
            {
                GOTO_table[i][j] = solveGotoValue(i, j);
                if (GOTO_table[i][j] != -1)
                    cout << "goto:\t" << i << ", " << j << '\t' << GOTO_table[i][j] << endl;
            }
}

string solveActionValue(int status, int id) {
    vector<ItemSet *>::iterator it;
    map<char, int>::iterator mp;
    string str = "err";
    char c = terminators[id];
    for (it = itemSetFam.begin(); it != itemSetFam.end(); it++) { //定位状态
        if ((*it)->id == status)
            break;
    }

    if ((*it)->next.size() > 0) {                               // s情况
        for (mp = (*it)->next.begin(); mp != (*it)->next.end(); mp++) { //定位边
            char edge = mp->first;
            if (c == edge)
                break;
        }
        if (mp != (*it)->next.end())
            str = "s" + to_string(mp->second);
    }
    else if ((*it)->next.size() == 0) {                         //r情况
        Item* thisOnlyItem = (*it)->thisItemSet[0];
        str = "r" + to_string((*it)->thisItemSet[0]->generativeId);
        if (thisOnlyItem->left[0] == non_terminators[0] \
        && thisOnlyItem->dot == (thisOnlyItem->left.size())) {
            if(c == '#')
                str = "acc";
            else
                str = "err";
        }
    }
    return str;
}

int solveGotoValue(int status, int id) {
    vector<ItemSet *>::iterator it;
    map<char, int>::iterator mp;
    int v = -1;
    char c = non_terminators[id];
    for (it = itemSetFam.begin(); it != itemSetFam.end(); it++) //定位item set
        if ((*it)->id == status)
            break;
    
    if ((*it)->next.size() > 0) {
        for (mp = (*it)->next.begin(); mp != (*it)->next.end(); mp++) {
            char edge = mp->first;
            if (c == edge)
                break;
        }
        if (mp != (*it)->next.end())
            v = mp->second;
    }
    if (v != -1)
        cout << "status: " << status << "\tid: " << id << "\tv:" << v << endl;
    return v;
}

void startAnalysis() {
    string str = "";
    while(true) {
        cout << "input string to analyse, input 'exit' to exit the system\n";
        cin >> str;
        if (str == "exit")
            return;
        str += '#';
        statusStack.push(0);
        symbolStack.push('#');

        cout << setw(16) << setiosflags(ios::left) << "status stack"
             << "	" << setw(16) << setiosflags(ios::left) << "symbol stack"
             << "	" << setw(16) << setiosflags(ios::left) << "input string"
             << "	" << setw(16) << setiosflags(ios::left) << "action description"
             << "	" << endl;

        int length = str.length();
        int now = 0;

        while (now < length) {
            cout << setw(16) << setiosflags(ios::left) << outStatusStack(statusStack) << "	"
                 << setw(16) << setiosflags(ios::left) << outSymbolStack(symbolStack) << "	"
                 << setw(16) << setiosflags(ios::left) << getStr(str, now) << "	";
            int topStatus = statusStack.top();
            int symbol = returnTerminatorId(str[now]);

            if (ACTION_table[topStatus][symbol] == "acc") {
                cout << "analysis succeed" << endl;
                break;
            }

            else if (ACTION_table[topStatus][symbol] == "err") {
                cout << "analysis fail" << endl;
                break;
            }

            else if (ACTION_table[topStatus][symbol][0] == 's') {
                int st = ACTION_table[topStatus][symbol][1] - '0';
                statusStack.push(st);
                symbolStack.push(str[now]);
                now++;
                cout << "ACTION(" << topStatus << ", " << symbol << ")= "
                     << ACTION_table[topStatus][symbol] << " as status " \
                     << st << " into stack " << endl;
            }
            else if (ACTION_table[topStatus][symbol][0] == 'r') {
                int st = ACTION_table[topStatus][symbol][1] - '0';
                int len = vt[st]->right.size();
                for (int i = 0; i < len; i++) {
                    symbolStack.pop();
                    statusStack.pop();
                }
                symbolStack.push(vt[st]->left[0]);
                int nextStatus = GOTO_table[statusStack.top()][returnNonTerminatorId(symbolStack.top())];
                // cout << "nextStatus" << nextStatus << endl;
                if (nextStatus == -1)
                    break;
                cout << "r" << st << ": "
                     << "using " << vt[st]->left << " -> "
                     << vt[st]->right << " statute and GOTO (" << statusStack.top()
                     << ", " << symbolStack.top() << ") = " << nextStatus << "push stack"
                     << endl;

                statusStack.push(nextStatus);
                cout << endl;
                continue;
            }
        }
        statusStack = stack<int>();
        symbolStack = stack<char>();
    }
}