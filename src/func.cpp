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
        string s1, s2, s3;
        infile >> s1 >> s2 >> s3;
        makeTSet(s1);
        makeNTSet(s3);
        makeGenerativeSet(s1, s3);
    }
    printf("Grammar read from file is as follows\n");
    // printGrammar();
}

void makeTSet (const string &str) {
    if (str.size() == 1 && str[0] >= 'A' && str[0] <= 'Z')
        terminators.insert(str[0]);
    else 
        cout << "terminator read error" << endl;
}

void makeNTSet (const string &str) {
    for (int i = 0; i < str.size(); i++)
        if (str[i] >= 'a' && str[i] <= 'z')
            non_terminators.insert(str[i]);
}

void makeGenerativeSet (const string &s1, const string s3) {
    Generative *tmp = new Generative();
    tmp -> left = s1;
    tmp -> right = s3;
    vt.push_back(tmp);
}

void extendItems() {
    int counter = 0, i = 0;
    vector <Generative*> ::iterator it;
    for (it = vt.begin(); it != vt.end(); it++) 
        for (i = 0; i <= (*it) -> right.size(); i++){
            Item *tmp = new Item();
            tmp -> id = counter;
            tmp -> left = (*it) -> left;
            tmp -> right = (*it) -> right;
            tmp -> dot = i;
            extendedItems.push_back(tmp);
            counter++;
        }
}

void makeNFA() {
    vector <Item*> ::iterator it1, it2;
    for (it1 = extendedItems.begin(); it1 != extendedItems.end(); it1++) {     //非终结符的情况
        int dot = (*it1) -> dot;
        string right = (*it1) -> right;
        char symbol = right[(*it1) -> dot];
        multimap <char, int> *mp = &((*it1) -> NFAitem);
        if (dot < right.size()) {
            (*mp).insert(multimap <char, int> ::value_type(symbol, (*it1) -> id + 1));
            if (isNon_Terminator(symbol))
                for (it2 = extendedItems.begin(); it2 != extendedItems.end(); it2++)
                    if (isFirstItem((*it2), symbol))
                        (*mp).insert(multimap <char, int> ::value_type('e', (*it2) -> id));
        }
    }
}

bool isFirstItem(Item *item, char &symbol) {
    if (item -> dot == 0 && item -> left[0] == symbol)
        return true;
    return false;
}

void makeItemSetFamily () {
    queue <Item*> Q;
    Item *item = extendedItems[0];
    multimap <char, int> ::iterator iter;
    set <int> visited;          //用set不用vector是因为set有find函数
    int counter = 0;
    Q.push((item));

    while (!Q.empty()) {
        item = Q.front();
        Q.pop();
        ItemSet *itemSet = new ItemSet();
        itemSet -> id = counter;
        itemSet -> thisItemSet.push_back(item);
        for (iter = item -> NFAitem.begin(); iter != item -> NFAitem.end(); iter++) {   //遍历map，往这个ItemSet里塞东西
            if (iter -> first == 'e') {
                item = returnItemById(iter -> second);
                itemSet -> thisItemSet.push_back(item);
            }
        }
    }
    // for (it = extendedItems.begin(); it != extendedItems.end(); it++) {
    //     for (iter = (*it) -> NFAitem.begin(); iter != (*it) -> NFAitem.end(); iter++) {
    //         ItemSet *itemSet = new ItemSet();
    //         itemSet -> id = counter;
    //         if (iter -> first == 'e')
    //             itemSet -> thisItemSet.push_back(iter -> second);
    //         itemSetFam.insert(itemSet);
    //     }
    //     visited.insert((*it) -> id);


}

Item* returnItemById(int &id) {
    vector <Item*> ::iterator it;
    for (it = extendedItems.begin(); it != extendedItems.end(); it++)
        if ((*it) -> id == id)
            return (*it);

}

// void makeFirstItemSet () {
//     vector <Item*> ::iterator it;
//     it = extendedItems.begin();
//     ItemSet *tmp = new ItemSet();
//     tmp -> id = 0;
//     tmp -> thisItemSet.push_back(*it);
//     extendCLOSURE(*tmp);
//     for (it = tmp -> thisItemSet.begin(); it != tmp -> thisItemSet.end(); it++){
//         // tmp -> next[(*it) -> right[(*it) -> dot]] = 
//     }
// }

// void extendCLOSURE(ItemSet &itemSet) {
//     vector <Item*> ::iterator it1, it2;
//     for (it1 = itemSet.thisItemSet.begin(); it1 !=itemSet.thisItemSet.end(); it1++)
//         if ((*it1) -> dot < (*it1) -> right.size() \
//         && isNon_Terminator((*it1) -> right[(*it1) -> dot]))
//             for (it2 = extendedItems.begin(); it2 != extendedItems.end(); it2++)       //遍历拓展项目集
//                 if ((*it2) -> left[0] == (*it1) -> right[(*it1) -> dot] \
//                 && (*it2) -> dot == 0)
//                     itemSet.thisItemSet.push_back(*it2);
// }