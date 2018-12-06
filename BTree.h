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
		size_t minKeys;
		T *keys;      	   // the size of this is M (M = order)
		BNode* *links;     // the size of this is M + 1

		BNode(){	// default constructor
			countK = 0;
			countL = 0;
			minKeys = ceil(double(M)/2) - 1;
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
		tuple<BNode*,T,CODE> remove(const T &val){
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
				if(countK < minKeys)
					return tuple<BNode*,T,CODE>(this, val, CODE::UNDERFLOW);
				return tuple<BNode*,T,CODE>(this, val, CODE::SUCCESS);
			}
		}

		tuple<BNode*,T,CODE> merge(BNode* &node, const T& parentVal){

			// what if we call marge on the right side of the list? I can always call merge with the one on the left with right ptr passed in!

			this->keys[countK++] = parentVal;
			this->links[countL++] = node->links[0];
			size_t index = 0;
			while(index < node->countK){
				this->keys[countK++] = node->keys[index++];
				this->links[countL++] = node->links[index];
			}
			delete node;
			node = nullptr;
			return tuple<BNode*,T,CODE> (this, parentVal, CODE::SUCCESS);
		}

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
	tuple<BNode*, T, CODE> remove(BNode*&, const T&);	// using immediate successor
	void deleteTree(BNode* );
	T findMin(BNode*);

  public:
	BTree();
	BTree(const T&);
	// copy constructor and operator= here if you get time
	~BTree();

	void printTree(ostream& out = cout) const;
	CODE insert(const T&);
	CODE remove(const T&);
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
tuple<typename BTree<T,M>::BNode*, T, typename BTree<T,M>::CODE> BTree<T,M>::remove(BNode* &node, const T& val){
	size_t index;
	bool underflow = false;
	tuple<BNode*,T,CODE> removeResult;

	for(index = 0; index < node->countK; ++index){
		if(node->keys[index] == val){	// if value found
			if(links[0]){	// if this is a non-leaf node
				node->keys[index] = findMin(links[index+1]);
				removeResult =	remove(links[index+1], node->keys[index]);
				if(get<2>(removeResult) == CODE::UNDERFLOW)
					break;
			}
			else{	// this is a leaf node, safe to delete from here
				return(node->remove(val));
			}
		}
		// now we know which link to descend into, since we have encountered a >.
		else if(node->keys[index] > val){
			if(links[0]){	// if non-leaf node
				removeResult = remove(links[index], val);
				if(get<2>(removeResult) == CODE::UNDERFLOW)
					break;
			}
			else	// leaf-node, but val not found
				return CODE::NOT_FOUND;
		}
	}

	// in case the remove worked out fine
	if(get<2>(removeResult) == CODE::SUCCESS || get<2>(removeResult) == CODE::NOT_FOUND)
		return removeResult;

	//we had an underflow if we made it to this point!
	if(index != 0){	// trying to find left sibling to borrow from
		if(node->links[index - 1]->countK > minKeys){	// if left has enough keys
			for(int i = node->links[index]->countK; i > 0; ++i){
				node->links[index]->keys[i] = node->links[index]->keys[i-1];
				node->links[index]->links[i+1] = node->links[index]->links[i];
			}
			node->links[index]->links[1] = node->links[index]->links[0];
			node->links[index]->links[0] = node->links[index-1]->links[node->links[index-1]->countK];
			node->links[index]->countL++;
			node->links[index]->keys[0] = node->keys[index-1];
			node->links[index]->countK++;
			node->keys[index - 1] = node->links[index-1]->keys[node->links[index-1]->countK-1];
			node->links[index-1]->links[node->links[index-1]->countK] = nullptr;
			node->links[index-1]->countK--;
			node->links[index-1]->countL--;
			return tuple<BNode*,T,CODE>(this,val,CODE::SUCCESS);
		}
	}


	if(index != node->countK - 1){	// if the underflowing node not all the way to the right
		if(node->links[index + 1]->countK > minKeys){ // now we are getting the 0th key from right side, and adding to end, not beginning
		//also need to shift the values on the right node, not the node that is borrowing, since val will be places at the end
			
			node->links[index]->keys[node->links[index]->countK] = node->keys[index];
			node->links[index]->countK++;
			node->links[index]->links[node->links[index]->countL] = node->links[index+1]->links[0];
			node->links[index]->countL++;
			node->keys[index] = node->links[index+1]->keys[0];
			node->links[index+1]->links[0] = nullptr;
			
			for(int i = 0; i < node->links[index+1]->countK; ++i){
				node->links[index+1]->keys[i] = node->links[index+1]->keys[i+1];
				node->links[index+1]->links[0] = node->links[index+1]->keys[i+1];
			}
			node->links[index+1]->links[node->links[index+1]->countK-1] = node->links[index+1]->links[node->links[index+1]->countK];
			node->links[index+1]->countL--;
			node->links[index+1]->countK--;
			
			return tuple<BNode*,T,CODE>(this,val,CODE::SUCCESS);
		}
	}

	tuple<BNode*,T,CODE> mergeResult;
	// FIRST GET IT WORKING, OPTIMIZE LATER!
	if(index == node->countK - 1){		// need to fix this check so I'm checking if its the last link, not the last value
		mergeResult = node->links[index-1]->merge(node->links[index], node->keys[node->countK - 1]);
		node->remove(node->keys[node->countK - 1]);
		node->links[index] = get<0>(mergeResult);	// adds the correct child ptr to the node
		if(node->countK < minKeys)
			return tuple<BNode*,T,CODE>(this,val,CODE::UNDERFLOW); 
	}
	else{
		mergeResult = node->links[index]->merge(node->links[index+1], node->keys[index+1]);
		node->remove(node->keys[index+1]);
		node->links[index] = get<0>(mergeResult);
		if(node->countK < minKeys)
			return tuple<BNode*,T,CODE>(this,val,CODE::UNDERFLOW);
	}
	return tuple<BNode*,T,CODE>(this,val,CODE::SUCCESS);
}

template <typename T, size_t M>
void BTree<T,M>::deleteTree(BNode* node){
	if(node){
		for(int i = 0; i < node->countL; ++i)
			deleteTree(node->links[i]);
		delete node;
	}
}

template <typename T, size_t M>
T BTree<T,M>::findMin(BNode* node){
	if(node->links[0])
		return findMin(node->links[0]);
	return node->keys[0];				// this will only be reached when we are at a leaf node
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

template <typename T, size_t M>
typename BTree<T,M>::CODE BTree<T,M>::remove(const T& val){
	if(sz == 0)
		return CODE::NOT_FOUND;
	if(!root->countK){
		if(root->links[0]){
			BNode* oldRoot = root;
			root = root->links[0];	// might need to change this logic if a different child is the non-null one
			delete oldRoot;
		}
	}
	--sz;
}