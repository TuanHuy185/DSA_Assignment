#include "main.h"
int MAXSIZE;
class NodeOfHuff{
	public:
	    char x='\0';
		int tanso=0;
		NodeOfHuff* pLeft=nullptr;
		NodeOfHuff* pRight=nullptr;
	public:
	NodeOfHuff(int tanso, char x, NodeOfHuff* pLeft, NodeOfHuff* pRight) 
    : x(x), tanso(tanso), pLeft(pLeft), pRight(pRight) {}
};
class CusOfGoJo{
	public:
		int result;
		CusOfGoJo *pLeft=nullptr;
		CusOfGoJo *pRight=nullptr;
	public:
		CusOfGoJo(int result) : result(result) {}
};
class AreaOfSu{
	public:
		int ID;			
		list<int> top;
	public:
		AreaOfSu(int ID) : ID(ID) {}
		int size() const { return top.size(); }
		void add(int result) { top.push_front(result); }
		void removeCus(int number){
			while (number > 0 && !top.empty()){
				solution << top.back() << "-" << ID << "\n";
				top.pop_back();
				number--;
			}
		}
		void print(int number){
			for(list<int>::iterator it = top.begin(); number > 0 && it != top.end(); ++it, --number){
				solution << ID << "-" << *it << "\n";
			}
		}
};

class HuffTree{
private:
	NodeOfHuff* root = nullptr;
public:
~HuffTree(){deleteTree(root);}
// tan so, tan so bang nha => a>b thì a đứng trước, chữ hoa lớn hơn thường
void SortPair(vector<pair<char, int>>& x){
        int n = x.size()-1;
		int i=0;
		while(i<n){
			int j=0;
			while(j<n-i){
				if (!ComparePare(x[j], x[j + 1])) swap(x[j], x[j + 1]);
				j++;
			}
			i++;
		}
	}
bool ComparePare(pair<char, int>x,pair<char, int>y){
		if (x.second != y.second) return x.second > y.second;
		else if (isupper(x.first) && islower(y.first)) return true;
		else if (islower(x.first) && isupper(y.first)) return false;
		return x.first > y.first;
	}
	// MÃ HÓA ra CAESAR NAME
vector<pair<char, int>> CaesarName(string &name){
		vector<pair<char, int>> pairOfTanso1(256);
        for (char c : name) pairOfTanso1[c].second++;
		int numCharInPair = 0;
        for (int i = 0; i < pairOfTanso1.size(); i++) {
        if (pairOfTanso1[i].second > 0) {
            numCharInPair++;
        }
        }
		if(numCharInPair<3) return{};
		for (char &c : name){
			if (isalpha(c)){
				char base = islower(c) ? 'a' : 'A';
				c = ((c - base + pairOfTanso1[c].second) % 26) + base;
			}
		}
		map<char, int> tempMap;
		for (char c : name){
			tempMap[c]++;
		}
		vector<pair<char, int>> pairOfTanso2(tempMap.begin(), tempMap.end());
		SortPair(pairOfTanso2);
		return pairOfTanso2;
}

int GetHeight(NodeOfHuff* node) {
        if (node == nullptr) return 0;
        return 1 + max(GetHeight(node->pLeft), GetHeight(node->pRight));
}

NodeOfHuff* RotateRight(NodeOfHuff* node) {
    NodeOfHuff* temp = node->pLeft;
    node->pLeft = temp->pRight;
    temp->pRight = node;
    return temp;
}

NodeOfHuff* RotateLeft(NodeOfHuff* node) {
    NodeOfHuff* temp = node->pRight;
    node->pRight = temp->pLeft;
    temp->pLeft = node;
    return temp;
}
int CompareHeight(NodeOfHuff* node){
    return GetHeight(node->pLeft)-GetHeight(node->pRight);
}
NodeOfHuff* balanceNode(NodeOfHuff* node, int& count) {
    // LL Case
    if (CompareHeight(node) > 1 && 
              CompareHeight(node->pLeft) >= 0) {
        count++;
        return RotateRight(node);
    }
    // RR Case
    if (CompareHeight(node) < -1 && 
            CompareHeight(node->pRight) <= 0) {
        count++;
        return RotateLeft(node);
    }
    // LR Case
    if (CompareHeight(node) > 1 && 
               CompareHeight(node->pLeft) <= 0) {
		count++;
        node->pLeft = RotateLeft(node->pLeft);
        return RotateRight(node);
    }
    // RL Case
    if (CompareHeight(node) < -1 && 
               CompareHeight(node->pRight) >= 0) {
		count++;		
		node->pRight = RotateRight(node->pRight);
        return RotateLeft(node);
    }
    return node;
}
// preoder
NodeOfHuff* balanceTree(NodeOfHuff* node, int count=0) {
    if (node == nullptr || count == 1) return node;
	node = balanceNode(node, count);
    node->pLeft = balanceTree(node->pLeft, count);
    node->pRight = balanceTree(node->pRight, count);
    return node;
}
NodeOfHuff* GetHuffTree(vector<pair<char, int>> pairOfTanso) {
    vector<NodeOfHuff*> tree;
    for (pair<char, int> x: pairOfTanso) 
        tree.push_back(new NodeOfHuff(x.second, x.first,nullptr,nullptr));
    while (tree.size() > 1) {
        NodeOfHuff* node1 = tree.back(); tree.pop_back();
        NodeOfHuff* node2 = tree.back(); tree.pop_back();
        NodeOfHuff* newNode = new NodeOfHuff(node1->tanso + node2->tanso, '\0', node1, node2);
        newNode = balanceTree(newNode);
		newNode = balanceTree(newNode);
		newNode = balanceTree(newNode);
        vector<NodeOfHuff*>::iterator vitri = find_if(tree.begin(), tree.end(), [&](NodeOfHuff* node) {
                 return node->tanso <= newNode->tanso;});
        tree.insert(vitri, newNode);
    }
    return tree[0];
}
    // node trái =0 , node phải =1
void BinaryChar(vector<string>& BinaryName, NodeOfHuff* node, string binary = ""){
    if (!node) return;
    if (node->x!='\0') BinaryName[node->x] += binary;
    BinaryChar(BinaryName, node->pLeft, binary + "0");
    BinaryChar(BinaryName, node->pRight, binary + "1");
}
int BinaryToDec(string TenBits){
	int dec = 0;
    int base = 1;
    for (int i = TenBits.size() - 1; i >= 0; i--) {
        if (TenBits[i] == '1') dec += base;
        base *= 2;
    }
	return dec;
}
int GetResult(NodeOfHuff * root,string name){
    if(root->pLeft == nullptr && root->pRight == nullptr) return 0;
    vector<string> BinaryName(256);
    BinaryChar(BinaryName, root);
    string newName;
    for (char x : name) newName += BinaryName[x];
    reverse(newName.begin(), newName.end());
    if (newName.length() < 10) newName = string(10 - newName.length(), '0') + newName;
    string TenBits = newName.substr(0, 10);
    return BinaryToDec(TenBits);
}

int ChooseRes(string name){
		vector<pair<char, int>> Tanso = CaesarName(name);
		if(Tanso.size()==0) return -1;
		deleteTree(root); 
		root = GetHuffTree(Tanso);
		if(root->pLeft == nullptr && root->pRight == nullptr) return 0;
		int result = GetResult(root ,name);
		return result;
}
void subPrint(NodeOfHuff* tree) {
	   if(tree == nullptr) return;
       subPrint(tree->pLeft);
       if(tree->x == '\0') solution << tree->tanso<< "\n";
       else solution << tree->x << "\n";
       subPrint(tree->pRight);
	}
	void print(){	
		subPrint(root);
	}
	void deleteTree(NodeOfHuff* root) {
    if (!root) return;
    stack<NodeOfHuff*> stack;
    stack.push(root);    
    while (!stack.empty()) {
        NodeOfHuff* current = stack.top();
        stack.pop();
        if (current->pLeft)  stack.push(current->pLeft);
        if (current->pRight) stack.push(current->pRight);
        delete current;
    }
}
};

class HusbandGojo{
 class AreaGojo{
	private:
		CusOfGoJo* root=nullptr;			  
		queue<int> TimeOfCus; 
	public:
		~AreaGojo(){
			while(!TimeOfCus.empty()){
				int temp = TimeOfCus.front();			
				TimeOfCus.pop(); 						
				root = removeCus(root ,temp);	
			}
		}
		int size()
		{
			return TimeOfCus.size();
		}
		void add(int result){
			root = addtoTree(root, result);
		}
		// bằng thì add phải
		CusOfGoJo *addtoTree(CusOfGoJo *node, int result){
			if (!node){
				node = new CusOfGoJo(result);
				TimeOfCus.push(result);
				return node;
			}
			else{
				if (result < node->result) node->pLeft = addtoTree(node->pLeft, result);
				else node->pRight = addtoTree(node->pRight, result);
			}
			return node;
		}
	    int SumNode(CusOfGoJo *node){
			return node==nullptr ? 0 : 1 + SumNode(node->pLeft) + SumNode(node->pRight);
		}
		unsigned long long Permutation(int k, int n){
			vector<vector<unsigned long long>> C(n+1, vector<unsigned long long>(k+1,0));
			for(unsigned long long i=0; i<=n;i++){
				for(unsigned long long j=0;j<=k && j<=i;j++){
					if(j==0||j==i)C[i][j]=1;
					else C[i][j]=(C[i-1][j-1]%MAXSIZE+C[i-1][j]%MAXSIZE);
				}
			}
			return C[n][k]%MAXSIZE;	
		}
		unsigned long long DepthSearch(CusOfGoJo *node){
			if (node == NULL) return 1;
			int SumLeft = SumNode(node->pLeft);
			int SumRight = SumNode(node->pRight);
			int LeftTree = DepthSearch(node->pLeft);
			int RightTree = DepthSearch(node->pRight);
			return (Permutation(SumLeft, SumLeft + SumRight) * LeftTree * RightTree)%MAXSIZE;
		}
		CusOfGoJo* removeCus(CusOfGoJo *node, int result){
			if (node == NULL) return node;
			if (result < node->result)
				node->pLeft = removeCus(node->pLeft, result);
			else if (result > node->result)
				node->pRight = removeCus(node->pRight, result);
			else{
				if (node->pLeft == NULL){
					CusOfGoJo *temp = node->pRight;
					delete node;
					return temp;
				}
				else if (node->pRight == NULL){
					CusOfGoJo *temp = node->pLeft;
					delete node;
					return temp;
				}
				CusOfGoJo *temp = node->pRight;
				while (temp->pLeft != nullptr){
				    temp = temp->pLeft;
			    }
				node->result = temp->result;
				node->pRight = removeCus(node->pRight, temp->result);
			}
			return node;
		}

		void remove(){
			if (TimeOfCus.size() == 0) return; 
			unsigned long long count = DepthSearch(root)%MAXSIZE;
			while (count != 0 && !TimeOfCus.empty())
			{
                int temp = TimeOfCus.front();
                TimeOfCus.pop();
                root = removeCus(root, temp);
				count--;
            }
		}
		void subPrint(CusOfGoJo* node){
			if(node != NULL){
				subPrint(node->pLeft);
				solution << node->result << "\n";	
				subPrint(node->pRight);
			}
		}
		void print(){
			subPrint(root);
		}
};
private:
	vector<AreaGojo*> area;
public:
	HusbandGojo() : area(MAXSIZE + 1) {
		for (int i = 1; i <= MAXSIZE; i++) {
            area[i] = new AreaGojo();
        }
	}
	~HusbandGojo() {
        for (int i = 1; i <= MAXSIZE; i++) {
            if (area[i]) delete area[i];
        }
	}
	void addCus(int result){
		int ID = result % MAXSIZE + 1;
		area[ID]->add(result);
	}
	void kokusen(){
		for (int i = 1; i < MAXSIZE + 1; i++)
			area[i]->remove();
	}
	void limitless(int number){
		if (number <= 0 || number > MAXSIZE) return; 
		area[number]->print();
	}
};	

class HusbandSu{
private:
	vector<AreaOfSu*> area; 
	list<AreaOfSu*> LRU;	 
private:
	bool CheckReheap(int x, int y){
		if (area[x]->top.size() < area[y]->top.size()){
			return true;
		}else if (area[x]->top.size() == area[y]->top.size()){
			list<AreaOfSu*>::iterator temp1 = find(LRU.begin(), LRU.end(), area[x]);
			list<AreaOfSu*>::iterator temp2 = find(LRU.begin(), LRU.end(), area[y]);
			if (temp1 != LRU.end() && temp2 != LRU.end()){
				return distance(LRU.begin(), temp1) > distance(LRU.begin(), temp2);
			}
		}
		return false;
	}
	void reheapDown(int position){
		int child = position;
		int pLeft = 2 * position + 1;
		int pRight = 2 * position + 2;
		if (pRight < (int)area.size() && CheckReheap(pRight, child)){
			child = pRight;
		}
		if (pLeft < (int)area.size() && CheckReheap(pLeft, child)){
			child = pLeft;
		}
		if (child != position){
			swap(area[position], area[child]);
			reheapDown(child);
		}
	}
	void reheapUp(int position){
		int parent = (position - 1) / 2;
		if(position==0 || !CheckReheap(position, parent)) return;
		else{
			swap(area[position], area[parent]);
			reheapUp(parent);
		}
	}
	void checkLRU(AreaOfSu *node){
		list<AreaOfSu*>::iterator cus = find(LRU.begin(), LRU.end(), node);
		if (cus != LRU.end()){
			LRU.erase(cus);
			LRU.push_front(node);
		}else{
			LRU.push_front(node);
		}
	}
	void removeLRU(AreaOfSu *node){
		list<AreaOfSu*>::iterator temp = find(LRU.begin(), LRU.end(), node);
		if (temp != LRU.end()) LRU.erase(temp);
	}

public:
	HusbandSu(){}
	~HusbandSu(){
		for(int i = 0; i < (int)area.size(); i++) delete area[i];
	}
	void addArea(int ID, int result){
			area.push_back(new AreaOfSu(ID));
			int index = area.size() - 1;
			area[index]->add(result);
			checkLRU(area[index]);
		    reheapUp(index);
	}
	void addCus(int result){
		int ID = result % MAXSIZE + 1;
		int index = -1;
		for (int i = 0; i < (int)area.size(); i++){
			if (area[i]->ID == ID){
				index = i;
				break;
			}
		}
		if(index==-1) addArea(ID, result);
		else{
			area[index]->add(result);
			checkLRU(area[index]);
			reheapDown(index);
		}
	}
	void keiteiken(int number){
		if(area.size() <= 0) return;
		vector<AreaOfSu* > areaTableNew(area.begin(), area.end());
		queue<AreaOfSu* > listDelete;
		for(int i = 0; area.size() && i < number; i++){
			AreaOfSu* nodeDelete = area[0];
			swap(area[0], area[area.size() - 1]);
			area.pop_back();
			reheapDown(0);
			listDelete.push(nodeDelete);
		}
		area = areaTableNew;
		while(listDelete.size()){
			AreaOfSu* nodeDelete = listDelete.front();
			listDelete.pop();
			nodeDelete->removeCus(number);
			int index = 0;
			while(area[index] !=  nodeDelete) index++;
			if(nodeDelete->top.size() == 0){
				swap(area[index], area[area.size() - 1]);
				removeLRU(area[area.size() - 1]);
				delete area[area.size() - 1];
				area.pop_back();
			}
			reheapDown(index);
		}

 	}
	void preOder(int index, int number){	
		if(index >= (int)area.size()|| number <= 0) return;
		area[index]->print(number);
		preOder(index * 2 + 1, number);
		preOder(index * 2 + 2, number);
	}
	void limitless(int number){
		preOder(0, number);
	}
};

class Assignment2{
private:
	HuffTree* newCus=new HuffTree();
	HusbandGojo* hash=new HusbandGojo();
	HusbandSu* heap=new HusbandSu();
public:
    ~Assignment2() {
    delete newCus;
    delete hash;
    delete heap;
    }
	void LAPSE(string name){
		if (name[0] >= '0' && name[0] <= '9')
		{
			int result = stoi(name);
			if (result % 2 == 1)
				hash->addCus(result);
			else
				heap->addCus(result);
			return;
		}
		int result = newCus->ChooseRes(name);
		if (result == -1) return;
		if (result % 2 == 1)hash->addCus(result);
		else heap->addCus(result);
	}	
	void KOKUSEN(){
		hash->kokusen();
	}
	void LIMITLESS(int num){
		hash->limitless(num);
	}
	void KEITEIKEN(int num){
		heap->keiteiken(num);
	}
	void CLEAVE(int num){
	    heap->limitless(num);
	}
	void HAND(){
		newCus->print();
	}
};
void simulate(string filename)
{

	ifstream ss(filename);
	string str, name;
	int num;

	ss >> str;
	ss >> MAXSIZE; //* nhập maxsize đầu tiên

	Assignment2 *NOT_LIKE_CODE = new Assignment2();
	// //* xử lí file
	while (ss >> str)
	{
		if (str == "LAPSE") // LAPSE <NAME>
		{
			ss >> name;
			NOT_LIKE_CODE->LAPSE(name);
		}
		else if (str == "KOKUSEN") // KOKUSEN
		{
			NOT_LIKE_CODE->KOKUSEN();
		}
		else if (str == "KEITEIKEN") // KEITEIKEN <NUM>
		{
			ss >> num;
			NOT_LIKE_CODE->KEITEIKEN(num);
		}
		else if (str == "HAND") // HAND
		{
			NOT_LIKE_CODE->HAND();
		}
		else if (str == "LIMITLESS") // LIMITLESS <NUM>
		{
			ss >> num;
			NOT_LIKE_CODE->LIMITLESS(num);
		}
		else if (str == "CLEAVE") // CLEAVE <NUM>
		{
			ss >> num;
			NOT_LIKE_CODE->CLEAVE(num);
		}
	}
	delete NOT_LIKE_CODE;
}