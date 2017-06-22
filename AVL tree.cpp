#include <iostream>
#include<algorithm>
using namespace std;

enum Order {pre,in,post};

template <typename key, typename info>
class Dictionary{
	struct node{
		key nkey;
		info ninfo;   	
   		node *left;
   		node *right;
   		int height;
		node(const key& k, const info& i):left(0),right(0),height(1),nkey(k),ninfo(i){}	
	};
	node* root;
	
	unsigned int size=0;
	
	int height(node* N)const{
		return (N==nullptr) ? 0 : N->height;
	}
	
//	int max(int a, int b){
//		return(a>b?a:b);
//	}
	
	int getBalance(node *N){
    	return (height(N->left)) - height(N->right); 				//   problem +1  
	}
	
	node* FindMin(node* root){
		while(root->left != nullptr) root = root->left;
		return root;
	}
	
	node* rotateRight(node *n){
		node *e = n->left;
   		node *temp = e->right;
 
   	 	e->right = n;
   		n->left = temp;
 
    	n->height = max(height(n->left), height(n->right))+1;
    	e->height = max(height(e->left), height(e->right))+1;
		
		return e;
	}
	
	node* rotateLeft(node *n){
		node * e = n->right;
	    node *temp = e->left;
 
   		e->left = n;
   		n->right = temp;

  		n->height = max(height(n->left), height(n->right))+1;
    	e->height = max(height(e->left), height(e->right))+1;
	
		return e;
	}
			
	
	node* balance(node* nptr){
		nptr->height=1+max(height(nptr->left),height(nptr->right));
		int bal = getBalance(nptr);
			if(bal > 1){
				if(height(nptr->left->left) > height(nptr->left->right)){		
					return rotateRight(nptr);						//right rotate
				}
				else{
					nptr->left=rotateLeft(nptr->left);				//left-right rotate 
					return rotateRight(nptr);
				}
			}	 
			if(bal < -1){
				if(height(nptr->right->right) > height(nptr->right->left)){			//left rotate
					return rotateLeft(nptr);
				}
				else{
					nptr->right=rotateRight(nptr->right);   //right-left rotate
					return rotateLeft(nptr);
				}
			}
			return nptr;	
	}
	
	
	node* insert(node* nptr,const key& k,const info& i){	
		if(nptr == nullptr){
			size++;
			return new node(k,i);
		}
 			
 		if(k==nptr->nkey){
 			cerr << "Key: " << k << " already exists" << endl;
			return nptr; 
		 }
 			
		if(k < nptr->nkey){
        	nptr->left = insert(nptr->left, k,i);
    	}
			
		else{
        	nptr->right = insert(nptr->right, k,i);
        }
        	//cout << nptr->nkey <<" "<< height(nptr->left)<<" "<< height(nptr->right)<< endl;
		nptr=balance(nptr); 
   	}
	
	node* deleteNode(node* nptr, const key& k ){
		if(nptr == nullptr){
			cerr << "Key: " << k << " not found" << endl;
			return nptr;
		}
 
    	if(k < nptr->nkey){
			nptr->left = deleteNode(nptr->left, k);
		}
 	   	else if(k > nptr->nkey){
        	nptr->right = deleteNode(nptr->right,  k);
		}
    	else{
        	// node with only one child or no child
        	if( (nptr->left == nullptr) || (nptr->right == nullptr) ){
            	node *temp = nptr->left ? nptr->left : nptr->right;
 			
            	// No child case
            	if(temp == nullptr){
                	temp = nptr;
                	nptr = nullptr;
            	}
            	else{ // One child case 
					*nptr=*temp;	// Copy the contents of the non-empty child
					//cout<<"check:" << nptr->height <<" "<<temp->height << endl;  //check  copy
            		size--;
					delete temp;
        		}
			}
        	else{
            	// node with two children, Get the inorder successor (smallest in the right subtree)
            	node* temp = FindMin(nptr->right);
				*nptr=*temp;
            	// Copy the inorder successor's data to this node
            	/*nptr->nkey=temp->nkey;
            	nptr->ninfo=temp->ninfo;  
            	nptr->height=temp->height;
            	*/
				// Delete the inorder successor
            	nptr->right = deleteNode(nptr->right, temp->nkey );
        	}
    	}
 
 	   if (nptr == nullptr)return nptr;	// If the tree had only one node then return
		   nptr=balance(nptr); //balance the tree after deletion
	}
	
	void preOrder(node * root)const{
		if(root == nullptr) return;
       		cout << root->nkey << " ";
    	   	preOrder(root->left);
 	      	preOrder(root->right);    		
	}
	
	void inOrder(node *root)const {
			if(root == nullptr) return;
			inOrder(root->left);       //Visit left subtree
			cout << root->nkey << " ";
			inOrder(root->right);      // Visit right subtree
	}		
		
	void postOrder(node * root)const{
		if(root == nullptr) return;
    	   	postOrder(root->left);
 	      	postOrder(root->right);
			cout << root->nkey << " ";       		
	}
	
	bool search(node* nptr,const key& k)const{
		if(nptr==nullptr)return false;
		if(nptr->nkey==k) return true;
		else if(k<=nptr->nkey) return find(nptr->left,k);
		else return find(nptr->right,k);    		
	}
	
	node* find(node* nptr,const key& k)const{
		if(nptr==nullptr){
			cerr << "key not found: " << k << endl;
			return nptr;
		}
		if(nptr->nkey==k)return nptr;
		if(k>nptr->nkey){
			return find(nptr->right,k);
		} 
		else return find(nptr->left,k);    		
	}
	
	node* copy(node * orig)const{
		if(orig == nullptr){return nullptr;}
		
		node *n = new node(orig->nkey, orig->ninfo);
        
        n->left = copy(orig->left);
        n->right = copy(orig->right);
		return n;
    }    		
	
	void clearAll(node* nptr){
    	if(nptr!=nullptr){
			clearAll(nptr->left);
    		clearAll(nptr->right);
    		delete nptr;
    		size=0;
		}
	}
	
	bool search(const key& k)const{
		return search(root,k);
	}
	
	bool compare(node *n) const{
        if (n){
            if (!compare(n->left)){ //go to the left 
                return false;
            }
            
            if (!compare(n->right)){ //go to the right
                return false;
            }
            
            if (!search(n->nkey)){	//search for corresponding key //only checks for the equal key not info
                return false;
            }
        }
        
        return true;
    }
	
	void recInsert(node *nptr){
        if (nptr){
            insert(nptr->nkey, nptr->ninfo);
            
            recInsert(nptr->left);
            recInsert(nptr->right);
        }
    }
		
	
	void help_print(ostream &out_s, node *r)const{
		if(r != NULL){
			cout << r->nkey << " ";
	 		help_print(out_s, r->left);
	    	help_print(out_s, r->right);
		}
	}
	
	void print(ostream& out_s)const{
			help_print(out_s, root);
		}
	
	public:
		Dictionary():root(0){}
		Dictionary(const Dictionary& rhs){
			root=copy(rhs.root);
			this->size=rhs.size;
		}
		~Dictionary(){clearAll(root);}
		
		Dictionary& operator=(const Dictionary &rhs){
			if(*this==rhs){return *this;}
			clearAll(root);
			root=copy(rhs.root);
			this->size=rhs.size;
		}
			
		Dictionary& operator +=(const Dictionary& rhs){
			//if(this==rhs)return *this;
			if(*this==rhs){
				 Dictionary temp= rhs; //we need to check first check the roots if they are equal, we traverse to other nodes. First left then right
            	 recInsert(temp.root);
			}
			else{
				recInsert(rhs.root);
			}
			return *this;
		}
		
		Dictionary operator +(const Dictionary& rhs){
			Dictionary temp=*this;
			temp += rhs;
			return temp;	
		}
		
		bool operator == (const Dictionary& rhs)const{
        	if (height(root) != rhs.height(rhs.root)){
            	return false;
       		}
       		
       		if(size!=rhs.size){
       			return false;
			}
       		
        	return compare(rhs.root);
    	}
		
		bool operator !=(const Dictionary& rhs)const{
			return !(*this==rhs);
		}
		
		void insert(const key& k,const info& i){
			root = insert(root,k,i);
		}
		
		void remove(const key& k){
			root = deleteNode(root,k); 
		}
		
		const info getInfo(const key& k)const{
			node* n=find(root,k);	
			if(n){
				return n->ninfo;
			}
		}
		
		void print(int Order)const{
			if(!root){cerr << "Empty tree" << endl;return;}
				
				switch (Order){
        	   		case pre:
            	    	cout << "Pre Order: ";
             			preOrder(root);
            		break;
            		case in:
                		cout << "In Order: ";
                		inOrder(root);
            		break;
            		case post:
                		cout << "Post Order: ";
            			postOrder(root);
					break;
            		default:
                		cout << "invalid. exit." << endl;
            		return;
        		}
        		cout <<endl;
		}
		
				

		friend ostream& operator << (ostream &out_s,const Dictionary &t){
			t.print(out_s);
			return out_s;
		}
};


int main(){
	

Dictionary<int,int> a;
	a.insert(101,10);
	a.insert(201,20);
	a.insert(301,30);
	a.insert(401,40);
	a.insert(-50,50);
	a.insert(25,25);
	cout << "a: ";
	a.print(0);
	
/*	a.insert(30,30);
	a.print();
	a.insert(99,0);
	a.print();
	a.remove(99);
	a.print();
	a.insert(100,100);
	a.print();
	a.insert(99,99);
	a.print();
	a.remove(100);
	a.print();
	//c.print();
	//a.remove(99);
	//a.print();
	/*a.remove(20);
	a.remove(25);
	a.remove(30);
	a.remove(40);
	a.remove(50);
	a.remove(99);
	a.print();
	//*/
	//a.print();
	///*
	Dictionary<int,int> b;
	b.insert(30,30);
	b.print(0);
	b.insert(20,20);
	b.print(0);
	b.insert(10,10);
	b.print(0);
	b.insert(25,25);
	b.print(0);
	b.insert(50,50);
	b.print(0);
	b.insert(40,40);
	b.print(0);
	b.insert(99,99);
	b.print(0);
	b.remove(99);
	b.print(0);
	b.insert(99,100);
	b.print(0);
	b.insert(100,100);
	b.print(0);
	b.insert(115,115);
	b.print(0);
	b.insert(120,120);
	b.print(0);
	b.remove(10);
	b.remove(25);
	cout << "b: ";
	b.print(0);
	Dictionary<int,int> c=b;
	c.print(0);
	c.insert(10,0);
	c.insert(25,0);
	c.print(0);
	Dictionary<int,int> k;
	k.insert(1,1);
	k.print(0);
	k+=c;
	k.print(0);
	cout << "c: ";
	c.print(0); 
	//k+=b;
	//k.print(0);
	k+=a;
	cout << "k: ";
	k.print(0);
	//cout << "b: ";
	//b.print(0);
	Dictionary<int,int> s;
	s=c+a;
	cout << "s: ";
	s.print(0);
	Dictionary<int,int> l=s;
	cout << "l: ";
	l.print(0);
	if(k!=s){
		cout << "k!=s" << endl;
	}
	if(l==s){
		cout << "l==s" << endl;
	}
	cout << "a: ";
	a.print(0);
	Dictionary<int,int> p;
	p.insert(101,40);
	p.insert(201,24);
	p.insert(301,30);
	p.insert(401,44);
	p.insert(-50,54);
	p.insert(25,25);
	cout << "p: ";
	p.print(0);
	if(a==p){
		cout << "a==p" << endl;
	}
	
	//cout << p.size << endl;
	
	p+=p;
	p.print(0);
	p.remove(0);
	cout << p.getInfo(10)<<endl;
	cout << p<<endl;
	cout << b<<endl;
	//*/
	/*
	a=b;
	a.print(0);
	a.insert(11,11); // here -1
	cout << a.root->right->right->right->nkey << endl;
	a.print(0);
	//*/
	/*
	Dictionary<int,int> c=b;
	c.print(0);
	c.remove(99);
	c.print(0);
	
	//c.insert(100,100);
	///c.print(0);
	c.insert(99,99);
	c.print(0);
	c.insert(100,100);
	c.print(0);					//here +1
	c.remove(100);
	cout << c.height(c.root)<<endl;
	c.print(0);
	//Dictionary<int,int> e;
	//e.print(0);
	//c.print(0);
	//int b = getBalance(c.root);
	//cout << b;// << endl;
	//a.remove(100);*/

/*
a.remove(30);
	a.print(0);
	a.remove(20);
	a.print(0);
	a.insert(100,100);
	a.insert(5,5);
	a.print(0);
	a.remove(50);
	a.print(0);
	//Dictionary<int,int> d;//=a;
//	d.insert(1,1);
	//d=a;
	//d.print(0);
	//if((d==a)){
	//	cout << " e";
	//}
	/*a.insert(5,5);
	a.insert(-5,-5);
	a.print(0);
	a.insert(-10,-10);
	*/
	
}	
