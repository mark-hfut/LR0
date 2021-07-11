#include<iostream>
#include<map>
#include<string>
#include<cstring>
#include<fstream>
#include<stack>
#include<vector>
#include<iomanip>
#include<sstream>
 
using namespace std;
 
#define path "..\\files\\expression.txt" //产生式路径
#define finalPath "..\\files\\final.txt"  //终结符路径
#define notFinalPath "..\\files\\notFinal.txt"  //非终结符路径
 
 
int pf = 0;//终结符数量
int pnf = 0; //非终结符数量
 
string final[101];//终结符
string notFinal[101];//非终结符
 
map<int, map<string , string>>ACTION;//ACTION表
 
map<int, map<string, int>>GOTO; //GOTO表
 
stack<string>symbol;//符号栈
stack<int>status; //状态栈
 
struct production{//产生式结构
	string left; //左部
	string right; //右部
};
 
vector<production*>vt; ///记录读入的产生式
 
 
struct DFA{//状态
	int id; 
	vector<string>left;//产生式左部
	vector<string>right; //产生式右部
	//int rows; 
	//DFA():rows(0){};
	map<char, int>next;//记录当前状态到达的下一个状态。起连接各个状态作用
};
 
vector<DFA*>itemSet; //状态图
 
int getDotPos(const string & str){//获得 . 的位置
	int length = str.length();
	for(int i = 0; i < length; i++){
		if(str[i] == '.')
			return i; 
	}
	return -1; 
 
}
 
bool canMoveDot(const string  & str){// 看 . 是否移动到了最右边
	int length = str.length();
	if(str[length - 1] == '.')
		return false; 
	return true; 
	
}
 
void moveDot(string & str){//把点往右移动一位
	int length = str.length();
	int pos = getDotPos(str);
	if(pos == length -1){
		cout<<"can't move right any more";
		return ; 
	}
 
	string tmp = "";
	for(int i = 0; i < pos; i++)
		tmp += str[i];
	tmp += str[pos + 1];
	tmp += '.';
	for(int i = pos + 2; i < length; i++)
		tmp += str[i];
 
	str =tmp;
	return ;
 
}
 
string  leftAddDot(string  str){//str 左部加个点
	str = '.' + str;
	return str; 
}
 
//扩展非终结符x的项目
void solveExtend(vector<string>&left, vector<string>&right, string x){
	//cout<<"x:"<<x<<endl;
	int sz = vt.size();
	for(int i = 0; i < sz; i++){
		if(x == vt[i]->left){///找到x所在的产生式子
			//加入新状态下的项目
			left.push_back(x);
			right.push_back(leftAddDot(vt[i]->right));
 
			//cout<<"此时加入了新产生式： "<<x<<"->"<<leftAddDot(vt[i]->right)<<endl; 
			if(vt[i]->right[0] >= 'A' && vt[i]->right[0] <= 'Z'){//新加入的状态可以继续扩展
				string nextArg = "";
				//nextArg += right[0];///修改
				nextArg += vt[i]->right[0];
				solveExtend(left, right, nextArg);//递归扩展
			}
 
			
		}
	}
	return ; 
 
}

void extendFirstLine(DFA *& x){//扩展状态的首行项目
	//cout<<"开始执行extendFirstLine"<<endl; 
	string rg = x->right[0];//该项目右部串比如A->.E，获得.E
	int length = rg.length();
	int pos = getDotPos(rg);
	//cout<<"右部长度是： "<<length<<" 点的位置是: "<<pos<<endl; 
 
	if(pos < length -1 && (x->right[0][pos+1] >='A' && x->right[0][pos +1] <='Z')){
		//如果黑点后面是非终结符
		vector<string>tmpLeft;
		vector<string>tmpRight; 
		string Arg = "";
		Arg += x->right[0][pos + 1];
		//cout<<"要拿去扩展的Arg:"<<Arg<<" 长度是"<<Arg.length()<<endl; 
		//cout<<"tmpLeft:"<<tmpLeft<<" tmpRight:"<<tmpRight<<endl; 
 
		solveExtend(tmpLeft, tmpRight, Arg);//扩展该项目
 
		int sz = tmpLeft.size();
		//cout<<"加入当前项目"<<endl; 
		for(int i = 0; i < sz; i++){//加入状态x
			x->left.push_back(tmpLeft[i]);
			x->right.push_back(tmpRight[i]);
			//cout<<tmpLeft[i]<<"->"<<tmpRight[i]<<endl;
		}
	}
	/*cout<<"这这里检查项目内容"<<endl; 
	for(int i = 0; i < x->left.size(); i++){
		cout<<x->left[i]<<"->"<<x->right[i]<<endl; 
	}*/
	return ; 
	//if(rg[0] == '')
}
 
 
void createItem(string lf, string rg){//创建一个新状态
	//新状态的首行是lf -> (rg右移一个黑点的串) 
	DFA * tmp = new DFA();
	tmp->id = itemSet.size(); 
	tmp->left.push_back(lf);//左部
	//改变右部点位置再压入右部
	moveDot(rg);
 
	tmp->right.push_back(rg);
	
	//itemSet[itemSetNum++] = tmp;
	itemSet.push_back(tmp);//加入集合
}
 
int  excludeDupli(){//去掉重复的状态
	int sz = itemSet.size();
	for(int i = 0; i < sz - 1; i++){
		if(itemSet[i]->left[0] == itemSet[sz-1]->left[0]
		&& itemSet[i]->right[0] == itemSet[sz-1]->right[0])
		{
			itemSet.pop_back();{
				//break; 
				//itemSetNum--;
				return i; 
			}
		}
	
	}
	return -1; 
}
 
void addItem(){///增加一个新的状态
	for(int i = 0; i < itemSet.size(); i++){//对每个状态
		//cout<<"itemSet大小: "<<itemSetNum<<endl; 
		for(int j = 0; j < itemSet[i]->left.size(); j++){//对该状态下的项目
			string rg = itemSet[i]->right[j];
			//cout<<"rg: "<<rg<<endl; 
			if(!canMoveDot(rg))//不可移动黑点了，比如A->E. 
				break; 
			//cout<<"可以移动"<<endl; 
			createItem(itemSet[i]->left[j], itemSet[i]->right[j]);
			//cout<<"创建新状态成功了"<<endl; 
			///去除重复的状态
			int dup = excludeDupli();
			if(dup == -1){//状态不是重复的
				extendFirstLine(itemSet[itemSet.size()-1]);//用第一行项目扩展
				int dotPos = getDotPos(rg);
				//记录状态i到状态itemSet.size()-1经过的路径
				itemSet[i]->next[rg[dotPos + 1]] = (itemSet.size()-1);
			}else{//状态是和以前的重复的
				int dotPos = getDotPos(rg);
				itemSet[i]->next[rg[dotPos + 1]] = dup;
			}
		}
	}
	return ; 
}
 
void initFirstItem(){//初始化状态0
	DFA * tmp = new DFA();
	tmp->id = itemSet.size();
	//itemSetNum++;
	//填入. 比如S'->.S
	tmp->left.push_back(vt[1]->left);
	tmp->right.push_back(leftAddDot(vt[1]->right));
	
 
	itemSet.push_back(tmp);
	//根据该状态的第一个项目扩展其剩下的项目
	extendFirstLine(tmp);
	//cout<<"第一个项目扩展完毕"<<endl; 
	//cout<<"-------检查第一个状态-------"<<endl; 
	//cout<<tmp->left[0]<<"->"<<tmp->right[0]<<endl;
	//for(int i = 0; i < itemSet[0]->left.size(); i++){
		//cout<<itemSet[0]->left[i]<<"->"<<itemSet[0]->right[i]<<endl; 
 
	//}
	//cout<<vt[1]->left<<"->"<<vt[1]->right<<endl; 
}
 
void in(){/*读入终结符合非终结符以及产生式到一维数组中*/
	
	ifstream infile;
	infile.open(finalPath, ios::in);
	if(!infile){
		cout<<"error in opening "<<finalPath<<endl;
		system("pause");
		exit(-1);
	}
	
	string tmp; 
	while(!infile.eof()){
		infile>>tmp;
		if(tmp == "end")
			break; 
		final[pf++] = tmp; 
	}
	infile.close();//关闭
	//------读入非终结符
	infile.open(notFinalPath, ios::in);
	if(!infile){
		cout<<"error in opening "<<notFinalPath<<endl;
		system("pause");
		exit(-1);
	}
	while(!infile.eof()){
		infile>>tmp;
		if(tmp == "end")
			break; 
		notFinal[pnf++] = tmp; 
	}
 
	infile.close();
 
	infile.open(path, ios::in);
 
	if(!infile){
		cout<<"error in opening "<<path<<endl; 
		system("pause");
		exit(-1);
	}
	
	///产生式0号位置霸占掉
	production * occupy = new production();
	occupy->left = "";
	occupy->right = "";
	vt.push_back(occupy);
 
	while(!infile.eof()){/*读到产生式的左部，  右部*/
		
		production * tmp = new production(); 
		string s1, s2, s3; 
		infile>>s1>>s2>>s3; 
		tmp->left = s1;
 
		tmp->right = s3;
		//cout<<"middle: "<<s2<<endl; 
		vt.push_back(tmp);
	}
 
	cout<<"The grammar (generated form) read from the file after broadening is as followed:"<<endl; 
	cout<<"The grammar (generated form) read from the file after broadening is as followed:"<<endl; 
	int size = vt.size();
	for(int i = 1; i < size; i++){
		cout<<vt[i]->left<<"->"<<vt[i]->right<<endl;
		//cout<<vt[i]->left<<vt[i]->middle<<vt[i]->right<<endl; 
	}
}
 
 
void checkSymbol(){/*检查各个符号*/
	cout<<"------------Terminator-----------"<<endl; 
	for(int i = 0; i < pf; i++){
		cout<<final[i]<<" ";
	}
	cout<<endl; 
	
	cout<<"------------Non-terminator-----------"<<endl; 
 
	for(int i = 0; i < pnf; i++){
		cout<<notFinal[i]<<" ";
	}
	cout<<endl; 
}
 
 
string deleteDot(const string & str){/*删除串str的小黑点*/
	int length = str.length();
	string res = "";
	for(int i = 0; i < length; i++){
		if(str[i] != '.'){
			res += str[i];
		}
	}
	return res; 
		
}
 
///查看left->right是第几条产生式
int fromWhere(const string & left, const string & right){
	int sz = vt.size();
	for(int i = 1; i < sz; i++){
		if(vt[i]->left == left && vt[i]->right == right)
			return i; 
	}
	return -1; 
}
 
///
void initTable(){///填写ACITON表和GOTO表
 
	//首先ACTION表全部为error
	for(int i = 0; i < itemSet.size(); i++){
		for(int j = 0; j < pf; j++){
			ACTION[i][final[j]] = "error";
		}
	}
	//GOTO表全部为-1
	for(int i = 0; i < itemSet.size(); i++){
		for(int j = 0; j < pnf; j++){
			GOTO[i][notFinal[j]] = -1; 
		}
	}
 
	int sz = itemSet.size();
	//先填写移进关系，比如s1, s2到ACTION表和GOTO表
	for(int i = 0; i < sz; i++){
		map<char, int>::iterator it;
		for(it = itemSet[i]->next.begin(); it !=itemSet[i]->next.end();it++){
			if(it->first >= 'a' && it->first <= 'z'){//是移进关系
				string tmp ="";
				tmp += it->first;
				//int status = itemSet[i]->next[it->first];
				int status = it->second; 
 
				cout<<"status"<<i<<" via"<<it->first<<" reaching status"<<status<<endl; 
				//以s开头填入表
				ACTION[i][tmp] = "s" +to_string((long long)status);
 
			}
			else{
				string tmp = "";
				tmp += it->first;
				//int status = itemSet[i]->next[it->first];
				int status = it->second; 
				cout<<"status"<<i<<"via"<<it->first<<"has reached status"<<status<<endl; 
				GOTO[i][tmp] = status; 
			}		
		}
	}
 
	//找到并填写表的acc
	for(int i = 0 ; i < sz; i++){
		if(itemSet[i]->left.size() == 1){
			string rg = itemSet[i]->right[0];
			int len = rg.length();
			if(len == 2 && rg[len-1]=='.' && rg[len-2] == vt[2]->left[0])
				ACTION[i]["#"] = "acc";
			break; 
		}
	}
 
	//填写表的规约关系
	for(int i = 0; i < sz; i++){
		if(itemSet[i]->left.size() == 1 && ACTION[i]["#"] != "acc"){//规约项目并且不是acc状态
			string lf = itemSet[i]->left[0];
			string rg = itemSet[i]->right[0];
			rg = deleteDot(rg);
			long long  index = fromWhere(lf, rg);
 
			for(int j = 0; j < pf; j++){
				ACTION[i][final[j]] = "r" + to_string(index);
			}
			
 
		}
	}
	
}
 
void checkTable(){//检查GOTO表和ACTION表
	
	cout<<"-----------ACTION table without data is filled with error, GOTO table without data is filled with -1-----------"<<endl; 
	cout<<"-----------ACTION table and GOTO table are as followed---------------------------"<<endl; 
	cout<<setw(6)<<setiosflags(ios::left)<<"status"<<"	";
	for(int i = 0; i < pf; i++){
		//cout<<final[i]<<"		";
		cout<<setw(6)<<setiosflags(ios::left)<<final[i]<<"	";
	}
 
	for(int i = 0; i < pnf; i++){
		//cout<<notFinal[i]<<"		";
		cout<<setw(6)<<setiosflags(ios::left)<<notFinal[i]<<"	";
	}
	cout<<endl; 
	//cout<<"fff"<<endl; 
 
	for(int i = 0; i < itemSet.size(); i++){
		//cout<<i<<"";
		cout<<setw(6)<<setiosflags(ios::left)<<i<<"	";
		for(int j  =0; j < pf; j++){
			cout<<setw(6)<<setiosflags(ios::left)<<ACTION[i][final[j]]<<"	";
		}
		for(int j = 0; j < pnf; j++){
			cout<<setw(6)<<setiosflags(ios::left)<<GOTO[i][notFinal[j]]<<"	";
		}
		cout<<endl; 
	}
	
 
	cout<<"------------Finish check ACTION & GOTO ------------"<<endl; 
 
}
 
string outStatusStack(stack<int>status){//打印状态栈
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
 
string outSymbolStack(stack<string>symbol){//打印符号栈
	string res;
	string tmp; 
	while(symbol.size()){
		tmp = symbol.top();
		res = tmp + res; 
 
		symbol.pop();
	}
 
	return res; 
}
 
string getStr(const string & str, const int & p){//截取串str从下标p之后的串
	string res = "";
	int length = str.length();
	for(int i = p; i < length; i++)
		res += str[i];
	return res; 
}
 
//截取str从下标p之后的串并把其转化为整数
int stringToInt(const string & str, const int & p){
	string res="";
	int length = str.length();
	for(int i = p; i < length; i++){
		res += str[i];
	}
	
	//cout<<"res: "<<res<<endl; 
	int len = res.length();
	int resInt = 0; 
	int base = 1; 
	for(int i = len - 1; i >= 0; i--){
		resInt += (((int)res[i]-48) * base);
		base *= 10; 
	}
	return resInt; 
		
}
 
 
void resetStack(){//重置栈
	status = stack<int>();//清空状态栈
	symbol = stack<string>();//清空符号栈
 
}
 
//开始文法分析
bool startAnalyze(const string & str){
	status.push(0);
	symbol.push("#");
 
	cout<<setw(16)<<setiosflags(ios::left)<<"status stack"<<"	"<<
		setw(16)<<setiosflags(ios::left)<<"symbol stack"<<"	"<<
		setw(16)<<setiosflags(ios::left)<<"input string"<<"	"<<
		setw(16)<<setiosflags(ios::left)<<"action description"<<"	"<<endl; 
 
	
	int length = str.length();
	//cout<<"length: "<<length<<endl; 
	int now = 0; 
	
	while(now < length){
		//输出表头
		cout<<setw(16)<<setiosflags(ios::left)<<outStatusStack(status)<<"	"
			<<setw(16)<<setiosflags(ios::left)<<outSymbolStack(symbol)<<"	"
			<<setw(16)<<setiosflags(ios::left)<<getStr(str, now)<<"	";
		
 
		int topStatus = status.top();//状态栈栈顶
		string symb = "";
		symb += str[now];//输入串当前符号
 
	 
		//比较状态栈栈顶topStatus和当前输入符号对比
		if(ACTION[topStatus][symb] == "acc"){
			cout<<"acc analysis success"<<endl;
			return true; 
		}
		if(ACTION[topStatus][symb] == "error"){//ACTION表找不到分析失败
			cout<<"anlysis fail"<<endl; 
			return false; 
		}
 
		if(ACTION[topStatus][symb][0] == 's'){//是移进关系
			int st = stringToInt(ACTION[topStatus][symb], 1);//获得查ACITON表之后的状态
		
			status.push(st);//状态入状态栈
			symbol.push(symb);//符号入符号栈
			//cout<<"状态"<<st<<"入栈"<<endl; 
		
			now++;//输入串指针右移
			cout<<"ACTION["<<topStatus<<"]["
				<<symb<<"]="<<ACTION[topStatus][symb]<<"as status"<<st<<"into a stack"<<endl; 
			
			continue; 
		}else{//是规约关系
		
			//获得第num条产生式
			int num = stringToInt(ACTION[topStatus][symb], 1);
			string right = vt[num]->right;//第num条产生式右部的长度
			for(int i = 0; i < right.size(); i++){//同时出栈
				symbol.pop();
				status.pop();
			}
			
	
			symbol.push(vt[num]->left);//产生式左部入符号栈
			//查GOTO表
			int newStatus = GOTO[status.top()][symbol.top()];
		
			if(newStatus == -1)
				return false;
 
			cout<<"r"<<num<<": "<<"using"<<vt[num]->left<<"->"<<vt[num]->right<<"statute and "
				<<"GOTO("<<status.top()<<","<<symbol.top()<<")=";
 
			cout<<newStatus<<"push stack"<<endl; 
			
			//把查GOTO表得到的状态入栈
			status.push(newStatus);
			
			cout<<endl; 
			continue; 
		}
	}
 
	return false; 
}
 
//除掉str的最后一个符号。 
string excludeLastSymbol(const string & str){
	int length = str.length();
	string res = "";
	for(int i = 0; i < length - 1 ; i++)
		res += str[i];
	return res; 
}
 
 
void checkItem(){
	cout<<"----------checking all the status------------------"<<endl; 
	int sz = itemSet.size();
	for(int i = 0; i < sz; i++){
		cout<<"I"<<i<<endl;
		for(int j = 0; j < itemSet[i]->left.size(); j++){
			cout<<itemSet[i]->left[j]<<"->"<<itemSet[i]->right[j]<<endl; 
		}
		cout<<"------------------"<<endl; 
	}
}
 
void addRear(string & str){
	int length = str.length();
	if(str[length -1] != '#')
		str += '#';
	return ; 
}

int main()
{
	string str; //待分析的串
	in();//读入终结符，非终结符以及产生式
	checkSymbol();
	//----
	cout<<"---------------------"<<endl; 
 
	initFirstItem();//初始化状态0
	addItem();//拓展状态
 
	checkItem();//检查所有的状态及其下方的下方的项目
 
	///----
	initTable();//填写ACTION GOTO表
	checkTable();
	
	while(true){
		cout<<"input string to anaylse, end with#  input finish to exit the system"<<endl; 
		cin>>str;
 
		if(str == "finish")
			break; 
		addRear(str);//防止用户忘记在要分析的串加#结尾。 
		bool res = startAnalyze(str);
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
 
 
	system("pause");
}