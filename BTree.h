#include <tuple>
#include <iostream>

using namespace std;

template<typename T, int M = 5>
class BTree{
  public:
	enum CODE{NOT_FOUND, SUCCESS, DUPLICATE, OVERFLOW};

  private:
	struct BNode{
		int countL;     // count of links
		int countK;     // count of keys
		T *keys;      // the size of this is M (M = order)
		BNode* *links;    // the size of this is M + 1

		BNode(){	// default constructor
			countK = 0;
			countL = 0;
			keys = new T[M];
			links = new BNode*[M+1];
			for(int i = 0; i <= M; ++i)	// intialize links to nullptr
				links[i] = nullptr;
		}
		
		BNode(T val){	// for creating a BNode with a single value
			countK = 1;
			countL = 2;
			keys = new T[M];
			keys[0] = val;
			links = new BNode*[M+1];
			for(int i = 0; i <= M; ++i)	// initialize links to nullptr
				links[i] = nullptr;
		}
		~BNode(){		// destructor
			delete[] keys;
			delete[] links;
		}		
		
		// add/insert a value into a BNode
		tuple<BNode*,int,CODE> add(const T &val){
			// if the node is empty, insert val and return success
			if(!countK){
				keys[0] = val;
				++countK;
				return tuple<BNode*,T,CODE>(nullptr, val, CODE::SUCCESS);
			}
			// else, insert the value in proper location
			else{
				// checks for duplicate so we don't waste time shifting elements if duplicate exists
				for(int i = 0; i < countK; ++i){
					if(keys[i] == val)
						return tuple<BNode*,T,CODE>(nullptr, val, CODE::DUPLICATE);
				}
				int index = countK;

				// shift over all the values that are smaller, and after loop insert val
				while(index > 0 && keys[index-1] > val){
					keys[index] = keys[index - 1];
					--index;
				}
				keys[index] = val;
				++countK;

				// if overflow happens, return OVERFLOW so we can call 'split'
				if(countK == M)
					return tuple<BNode*,T,CODE>(nullptr, val, CODE::OVERFLOW);

				return tuple<BNode*,T,CODE>(nullptr, val, CODE::SUCCESS);
        	}
    	}
    
		// used to split a node in half when overflow happens
		tuple<BNode*,T,CODE> split(){
			int median = countK/2;
			int index = median + 1;
			int oldCount = countK;

			// new node that data past the median will be copied into
			BNode* right = new BNode;
			
			// copies over the first link, so in the future when we copy a key we only have to copy the right side link
			right->links[0] = links[index];
			++right->countL;
			links[index] = nullptr;
			--countL;
			
			// copies over the remaining values
			while(index < oldCount){
				right->links[(right->countL)++] = links[index + 1];
				right->keys[(right->countK)++] = keys[index];
				links[++index] = nullptr;
				--countL;
				--countK;
			}
			--countK; // because median was removed
			return tuple<BNode*,T,CODE>(right, keys[median], CODE::SUCCESS);
		}

		/*
		// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
		tuple<BNode*,int,CODE><M> merge(){

		}*/

		void printNode(){
			for(int i = 0; i < countK; ++i){
				cout << "link: " << (links[i]? "valid" : "nullptr") << endl;
				cout << "val: " << keys[i] << endl;
			}
			cout << "link: " << (links[countK]? "valid" : "nullptr") << endl;
		}
	};

	BNode* root;
	size_t sz;
	tuple<BNode*, T, CODE> insert(BNode*, const T&);
	tuple<BNode*, T, CODE> remove(BNode*, const T&);	// using immediate successor

  public:
	BTree();
	BTree(const T&);
	// copy constructor and operator= here if you get time
	~BTree();

	void printTree();
	CODE insert(const T&);
};


/////////////////////////////////		PRIVATE METHODS		  /////////////////////////////////////

template <typename T, int M>
tuple<typename BTree<T,M>::BNode*, T, typename BTree<T,M>::CODE> BTree<T,M>::insert(BNode* node, const T& val){

}

/////////////////////////////////		PUBLIC METHODS		  /////////////////////////////////////

template <typename T, int M>
BTree<T,M>::BTree():root{nullptr},sz{0}{}

template <typename T, int M>
BTree<T,M>::BTree(const T& val):sz{1}{
	root = new BNode(val);
}

template <typename T, int M>
BTree<T,M>::~BTree(){
	// need to make this a traversal, and delete individual nodes
	delete root;
}