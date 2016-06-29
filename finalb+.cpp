#include <bits/stdc++.h>
#include "sha256.h"
using namespace std;

// A BTree node
class BTreeNode
{
public:
	int *keys;
	int t,n; //n is number of keys and t is order
	BTreeNode **C, *next, *parent; 
	bool leaf; 
	BTreeNode(int _t, bool _leaf){ // Constructor
		t = _t;
		leaf = _leaf;
		keys = new int[t];
		C = new BTreeNode *[t+1];
		n = 0;
		next = NULL;
		parent = NULL;
	};
};
class SearchNode{
public:
	BTreeNode *node;
	int index;
	bool found;
	SearchNode(){
		node = NULL;
		index = -1;
		found = false;
	}
};
BTreeNode *parentnotfull(BTreeNode *ptr, int k, BTreeNode *z);
BTreeNode *update_parent(BTreeNode *ptr, int k);
int flag = 0;
BTreeNode *search(BTreeNode *root, int k){
	if(root->leaf == true) {
		cout << "leaf " << root->keys[0] << endl; 
		return root;
	}
	
	int start = 0;
	for(int i = 0; i < root->n ; i++){
		if(k < root->keys[i]) {
				cout << "break" << root->keys[i] << " ";
				break;
		}else {
			cout << root->keys[i] << " ";
			start++;
		}
	}
	return search(root->C[start],k);
}


BTreeNode *insertNonFull(int k, BTreeNode *ptr){
	int i = ptr->n-1;
	while (i >= 0 && ptr->keys[i] > k)
	{
		ptr->keys[i+1] = ptr->keys[i];
		i--;
	}
	ptr->keys[i+1] = k;
	ptr->n = ptr->n+1;
	
	return ptr;
}

BTreeNode *update_parent(BTreeNode *ptr, int k,BTreeNode *z){
	BTreeNode *tmp = new BTreeNode(ptr->t, false);
	vector <int>a;
	int q;
	for(q = 0; q < ptr->n; q++){
		a.push_back(ptr->keys[q]);
	}
	a.push_back(k);
	sort(a.begin(), a.end());

	for(int i = 0; i < ptr->t/2; i++){
		ptr->keys[i] = a[i];
		tmp->keys[i] = a[i+1+ptr->t/2];
	}
	int sv = a[ptr->t/2];
	a.erase (a.begin(),a.end());
	tmp->n = ptr->t/2;
	BTreeNode *par;
	if(ptr->parent != NULL){
		if(ptr->parent->n == ptr->t){
			par = update_parent(ptr->parent, sv, tmp);	
		} else{ 
			cout << "parent not full\n";
			par = parentnotfull(ptr->parent, sv,tmp);
		}
		//return par;
	} else{
		cout << "creating new root\n";
		BTreeNode *new_root = new BTreeNode(ptr->t, false);
		new_root->keys[0] = sv;
		new_root->C[0] = ptr;
		new_root->C[1] = tmp;
		new_root->n = 1;
		ptr->parent = new_root;
		tmp->parent = new_root;
		//return new_root;
	}
	if(sv == k){
		for(int i = ptr->t/2+1; i <= ptr->n; i++){
			tmp->C[i-ptr->t/2] = ptr->C[i];
			ptr->C[i]->parent = tmp;
		}
		tmp->C[0] = z;
		z->parent = tmp;
		ptr->n = ptr->t/2;
	}
	else if(sv > k){
		for (int j = ptr->t/2; j <= ptr->n; j++){
			tmp->C[j-ptr->t/2] = ptr->C[j];
			ptr->C[j]->parent = tmp;
		}

		ptr->n = ptr->t/2;
		int i = ptr->n-1;
		while(i >= 0 && ptr->keys[i] > k) i--;

		for(int j = ptr->n-1; j > i; j--){
			ptr->C[j+1] = ptr->C[j];
		}

		ptr->C[i+1] = z;
		z->parent = ptr;
	} else if(sv < k) {
		int i = tmp->n-1;
		while(i >= 0 && tmp->keys[i] > k) i--;
	
		int k=0;
		for(int j = 0; j <= tmp->n; j++){
			if(j == i+1){ 
				tmp->C[j] = z;
				z->parent = tmp;
			} else  {
				tmp->C[j] = ptr->C[ptr->t/2+1+k];
				ptr->C[ptr->t/2+1+k]->parent = tmp;
				k++;
			} 
		}
		ptr->n = ptr->t/2;
	}
	return ptr->parent;
}

void traverse(BTreeNode *root){
	if(root != NULL) {
		if(root->leaf == false){
			for (int i = 0; i < root->n+1; i++)
				traverse(root->C[i]);
		}else{
			if(root->parent!= NULL) cout << root->parent->keys[0] << ": ";
			for (int i = 0; i < root->n; i++)
				cout << root->keys[i] << " ";
			cout << endl;
		}
	}
}

/*void traverse(BTreeNode *root)
{
    int i;
    for (i = 0; i < root->n; i++)
    {
        if (root->leaf == false)
           traverse(root->C[i]);
        cout << " " << root->keys[i];
    }
    if (root->leaf == false)
        traverse(root->C[i]);
}*/
BTreeNode *parentnotfull(BTreeNode *ptr, int k, BTreeNode *tmp){
	int i = ptr->n-1;
	cout << "parentnotfull\n" << " " << ptr->keys[0] <<" " << i;
	cout << "key " << k << endl;
	while(i >= 0 && ptr->keys[i] > k) {
		i--;
	}
	cout << i << endl;
	for (int j = ptr->n; j >= i+2; j--){
		ptr->C[j+1] = ptr->C[j];
	}
	 
	ptr->C[i+2] = tmp;
	tmp->parent = ptr;
	for (int j = ptr->n-1; j > i; j--){
		ptr->keys[j+1] = ptr->keys[j];
	}
	ptr->keys[i+1] = k;
	ptr->n = ptr->n+1;
	return ptr;
}


BTreeNode *splitChild (BTreeNode *ptr, int k){
	BTreeNode *tmp = new BTreeNode(ptr->t, true);
	vector <int>a;
	int q;
	for(q = 0; q < ptr->n; q++){
		a.push_back(ptr->keys[q]);
	}
	a.push_back(k);
	sort(a.begin(), a.end());
	
	for(int i = 0; i < (ptr->t/2); i++){
		ptr->keys[i] = a[i];
	}
	for(int i = 0; i <= ptr->t/2; i++){
		tmp->keys[i] = a[i+ptr->t/2];
	}
	
	int sv = a[ptr->t/2];
	a.erase (a.begin(),a.end());
	tmp->n = ptr->t/2+1;
	ptr->n = ptr->t/2;
	tmp->next = ptr->next;
	ptr->next = tmp;
	BTreeNode *par = ptr->parent;
	
	if(par != NULL){
		cout << "pm " << par->keys[0] << endl;
		if(ptr->parent->n == ptr->t){
			BTreeNode *prnt = update_parent(ptr->parent, sv,tmp);//going in par or tmp->parent or ptr->parent and coming in par->parent			
			while(prnt->parent){
				prnt = prnt->parent;
			}
			return prnt;		
		} else{ 
			cout << "parent not full " << sv;
			par = parentnotfull(ptr->parent, sv,tmp);
			while(par->parent){
				par = par->parent;
			} 
			return par;
		}
		
	} else{
		//cout << "creating new root\n";
		BTreeNode *new_root = new BTreeNode(ptr->t, false);
		new_root->keys[0] = sv;
		new_root->C[0] = ptr;
		new_root->C[1] = tmp;
		new_root->n = 1;
		ptr->parent = new_root;
		tmp->parent = new_root;
		return new_root;
	}
}

BTreeNode *insert(BTreeNode *root, int k){
	if (root == NULL)
	{
		root = new BTreeNode(4,true);
		root->keys[0] = k; 
		root->n = 1; 
	}
	else { 
		BTreeNode *ptr = search(root, k);
		if(ptr->n == ptr->t){
			ptr = splitChild(ptr,k); //returnung ptr->parent
			root = ptr;
		}else{
			ptr = insertNonFull(k,ptr);
		}
		
	}
	return root;
}
string hashcompute(BTreeNode *root){
	if(root->leaf == true){
		string hs;
		for(int i = 0; i < root->n;i++){
			stringstream ss;
			ss << root->keys[i] ;
			string newstring = ss.str();
			hs.append(newstring);
		}
		return sha256(hs);
	}
	string rs;
	for(int i = 0; i <= root->n;i++){
		rs.append(hashcompute(root->C[i]));
	}
	return sha256(rs);
}
SearchNode *temp = new SearchNode;
/*bool founded(BTreeNode *root ,int data){	
	if(root->leaf){
		for(int i = 0; i < root->n;i++){
			//cout <<" in search of i " <<i <<" " <<root->keys[i]<<" data = " <<data <<endl;
			if(data == root->keys[i]){	
				//cout <<" found at " <<i<< "th location" << endl;			
				temp->index = i;
				temp->node = root;
				temp->found = true;
				return 1;
			}
		}			
		temp->index = -1;
		temp->node = NULL;
		temp->found = false;
		//return 0;
	}
	return 0;
} 
SearchNode *searchvalue(BTreeNode *root ,int data){
	if(root->leaf == true) {
		if(founded(root,data)){
			return temp;
		}
	}	
	int start = 0;
	for(int i = 0; i < root->n ; i++){
		if(founded(root->C[i],data)){
			return temp;
		}
		else{
			int start = 0;
			for(int i = 0; i < root->n ; i++){
				if(data < root->keys[i]) break;
				else {
					start++;
				}
			}
			return searchvalue(root->C[start],data);
		}
	}
	
}*/
bool searchvalue(BTreeNode *root ,int data1,int data2){
	while(root->leaf == false){
		if(root->C[0]){
			root = root->C[0];
		}
	}
	int flag = 0;
	while(root != NULL){
		for(int i = 0; i< root->n;i++){
			if(root->keys[i] >= data1 && root->keys[i] <= data2){
				flag = 1;
				cout << root->keys[i] <<" ";
			}
		}
		root = root->next;
	}
	return flag;
}
	
int main()
{
	BTreeNode *root = NULL;
	BTreeNode *head ;
	int val;
	cin >> val;
	while(val != -1){
		root = insert(root, val);
		//cout <<"root add " << &root<< " " << root->n <<" " <<root->keys[root->n-1] <<endl;
		traverse(root);
		cout << "roots" << endl;
		for(int i = 0; i < root->n;i++){
			cout <<root->keys[i] <<" ";
		}
		cout << endl;
		if(root->C[1]){
			for(int i = 0 ; i <= root->C[1]->n;i++){
				cout << root->C[1]->keys[i] << " ";
			}
		}
		cin >> val;
		cout << endl;
		cout << endl;
	}
	int level = 0;
	while(root->C[0] != NULL){
		root = root->C[0];
		level++;
	}
	cout << level << endl;
	string hashvalue = hashcompute(root) ;
	cout << "hashvalue of data " << hashvalue <<endl;
	cout << "search a range"<<endl;
	int s1,s2; cin >> s1 >> s2;
	if(!searchvalue(root,s1,s2)){
		cout <<"could not find " << endl;
	}
	/*SearchNode *start = ;
	//SearchNode *end = searchvalue(root,s2);
	if(start->found == true){
		cout << "found " <<s1 <<" at index " << start->index <<" of root " << start->node->keys[0] << endl;
	}
	else{
		cout <<"Not found " << endl;
	}*/

	return 0;
}
