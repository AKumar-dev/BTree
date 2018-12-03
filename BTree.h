#include <tuple>
#include <iostream>
#include <queue>

using namespace std;

template<typename T, size_t M = 5>
class BTree{
  public:
	enum CODE{NOT_FOUND, SUCCESS, DUPLICATE, OVERFLOW};

  private:
	struct BNode{
		size_t countL;     // count of links
		size_t countK;     // count of keys
		T *keys;      	   // the size of this is M (M = order)
		BNode* *links;     // the size of this is M + 1

		BNode(){	// default constructor
			countK = 0;
			countL = 0;
			keys = new T[M];
			links = new BNode*[M+1];
			for(size_t i = 0; i <= M; ++i)	// intialize links to nullptr
				links[i] = nullptr;
		}
		
		BNode(T val){	// for creating a BNode with a single value
			countK = 1;
			countL = 2;
			keys = new T[M];
			keys[0] = val;
			links = new BNode*[M+1];
			for(size_t i = 0; i <= M; ++i)	// initialize links to nullptr
				links[i] = nullptr;
		}
		~BNode(){		// destructor
			delete[] keys;
			delete[] links;
		}		
		
		// add/insert a value into a BNode
		tuple<BNode*,int,CODE> add(const T &val, BNode* node = nullptr){
			// if the node is empty, insert val and return success
			if(!countK){
				keys[0] = val;
				++countK;
				countL += 2;
				return tuple<BNode*,T,CODE>(nullptr, val, CODE::SUCCESS);
			}
			// else, insert the value in proper location
			else{
				size_t index = countK;

				// shift over all the values that are smaller, and after loop insert val
				while(index > 0 && keys[index-1] > val){
					keys[index] = keys[index - 1];
					links[index+1] = links[index];
					--index;
				}
				keys[index] = val;
				links[index + 1] = node;
				++countK;
				++countL;

				// if overflow happens, return OVERFLOW so we can call 'split'
				if(countK == M)
					return tuple<BNode*,T,CODE>(nullptr, val, CODE::OVERFLOW);

				return tuple<BNode*,T,CODE>(nullptr, val, CODE::SUCCESS);
        	}
    	}
    
		// used to split a node in half when overflow happens
		tuple<BNode*,T,CODE> split(){
			size_t median = countK/2;
			size_t index = median + 1;
			size_t oldCount = countK;

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

		//remove always happens from a leaf node
		tuple<BNode*,T,CODE> remove(const T &val, BNode* node = nullptr){
			int index = 0;
			while(val >= keys[index] && index < countK)
				++index;
			if(val != keys[index - 1])
				return tuple<BNode*,T,CODE>(nullptr, val, CODE::NOT_FOUND);
			else{
				while(index < countK){
					keys[index - 1] = keys[index];
					links[index - 1] = links[index];
					++index;
				}
				links[index - 1] = links[index];    // THIS MIGHT BE A LITTLE SKETCH ! ! !
				--countK;
				--countL;
				if(countK < (ceil(double(M)/2) - 1))
					return tuple<BNode*,T,CODE>(this, val, CODE::UNDERFLOW);
				return tuple<BNode*,T,CODE>(this, val, CODE::SUCCESS);
			}
		}

		/*
		// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
		tuple<BNode*,T,CODE> merge(){

		}*/

		void printNode(ostream& out = cout){
			for(size_t i = 0; i < countK; ++i){
				out << keys[i]; ", ";
				if(i != countK-1)
					out << ", ";
			}
		}
	};

	BNode* root;
	size_t sz;
	size_t numLevels;

	tuple<BNode*, T, CODE> insert(BNode*, const T&);
	tuple<BNode*, T, CODE> remove(BNode*, const T&);	// using immediate successor
	void deleteTree(BNode* );

  public:
	BTree();
	BTree(const T&);
	// copy constructor and operator= here if you get time
	~BTree();

	void printTree(ostream& out = cout) const;
	CODE insert(const T&);
};


/////////////////////////////////		PRIVATE METHODS		  /////////////////////////////////////

template <typename T, size_t M>
tuple<typename BTree<T,M>::BNode*, T, typename BTree<T,M>::CODE> BTree<T,M>::insert(BNode* node, const T& val){
	if(node->links[0] == nullptr){	// if this is a leaf node
		return node->add(val);
	}
	else{
		size_t index;

		// figures out where to insert
		for(index = 0; index < node->countK; ++index){
			if(node->keys[index] == val) // if duplicate found, we dont need to check further
				return tuple<BNode*,T,CODE>(nullptr, val, CODE::DUPLICATE);
			if(node->keys[index] > val)  // finds which link we need to insert on
				break;
		}

		// checks for overflow, and acts accordingly
		tuple<BNode*,T,CODE> result = insert(node->links[index], val);
		if(get<2>(result) == CODE::OVERFLOW){
			tuple<BNode*,T,CODE> splitResult = node->links[index]->split();
			return node->add(get<1>(splitResult), get<0>(splitResult)); //passes the result to above chain
		}
		return result; // when there was no overflow from the result
	}
}

template <typename T, size_t M>
void BTree<T,M>::deleteTree(BNode* node){
	if(node){
		for(int i = 0; i < node->countL; ++i)
			deleteTree(node->links[i]);
		delete node;
	}
}

/////////////////////////////////		PUBLIC METHODS		  /////////////////////////////////////

template <typename T, size_t M>
BTree<T,M>::BTree():sz{0},numLevels{1}{
	root = new BNode();
}

template <typename T, size_t M>
BTree<T,M>::BTree(const T& val):sz{1},numLevels{1}{
	root = new BNode(val);
}

template <typename T, size_t M>
BTree<T,M>::~BTree(){
	deleteTree(root);
}

template <typename T, size_t M>
void BTree<T,M>::printTree(ostream& out) const{
	if(root){
		queue<BNode*> nodes;
		nodes.push(root);
		while(nodes.size()){
			int levelCount = nodes.size();
			while(levelCount){
				out << "[";
				nodes.front()->printNode(out);
				levelCount > 1 ? (out << "] "): (out << "]");
				for(size_t i = 0; nodes.front()->links[i] != nullptr; ++i)
					nodes.push(nodes.front()->links[i]);
				nodes.pop();
				--levelCount;
			}
			if(nodes.size())
				out << endl;
		}
	}
}

template <typename T, size_t M>
typename BTree<T,M>::CODE BTree<T,M>::insert(const T& val){
	if(sz == 0){	// if root is empty, just add to it
		++sz;
		return get<2>(root->add(val));
		++sz;
	}
	else{	// tree already has values
		tuple<BNode*,T,CODE> insertResult = insert(root, val);
		if(get<2>(insertResult) == CODE::DUPLICATE)	//duplicate found, do nothing special
			return CODE::DUPLICATE;
		else if(get<2>(insertResult) == CODE::OVERFLOW){ //root returned overflow, must split
			++sz;
			++numLevels;
			tuple<BNode*,T,CODE> splitResult = root->split();
			BNode* newRoot = new BNode(get<1>(splitResult));
			newRoot->links[0] = root;
			newRoot->links[1] = get<0>(splitResult);
			root = newRoot;
			return CODE::SUCCESS;
		}
		else{	//success
			++sz;
			return CODE::SUCCESS;
		}
	}
}