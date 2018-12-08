#include <tuple>
#include <iostream>
#include <queue>
#include <cmath>

using namespace std;

template<typename T, size_t M = 5>
class BTree{
  public:
	// possible results prom operations
	enum OP_RESULT{NOT_FOUND, FOUND, SUCCESS, DUPLICATE, OVERFLOW, UNDERFLOW};

  private:
	struct BNode{
		size_t countL;
		size_t countK;
		T *keys;      	   // the size of this is M, which allows easier handling of overflows
		BNode* *links;     // the size of this is M + 1, which allows easier handling of overflows

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
		tuple<BNode*,T,OP_RESULT> add(const T &val, BNode* node = nullptr){
			// if the node is empty, insert val and return success
			if(!countK){
				keys[0] = val;
				++countK;
				countL += 2;
				return tuple<BNode*,T,OP_RESULT>(nullptr, val, OP_RESULT::SUCCESS);
			}
			// else, insert the value in proper location
			else{
				// checks for duplicates before shifting any values
				for(size_t i = 0; i < countK; ++i)
					if(keys[i] == val)
						return tuple<BNode*,T,OP_RESULT>(nullptr,val,OP_RESULT::DUPLICATE);

				size_t index = countK;

				// shift over all the values that are smaller, and after loop insert val
				while(index > 0 && keys[index-1] > val){
					keys[index] = keys[index - 1];
					links[index+1] = links[index];
					--index;
				}

				// inserts the value and node in the proper position
				keys[index] = val;
				links[index + 1] = node;
				++countK;
				++countL;

				// if overflow happens, return OVERFLOW so parent can call 'split'
				if(countK == M)
					return tuple<BNode*,T,OP_RESULT>(nullptr, val, OP_RESULT::OVERFLOW);

				return tuple<BNode*,T,OP_RESULT>(nullptr, val, OP_RESULT::SUCCESS);
        	}
    	}
    
		// used to split a node when overflow happens
		tuple<BNode*,T,OP_RESULT> split(){
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
			return tuple<BNode*,T,OP_RESULT>(right, keys[median], OP_RESULT::SUCCESS);
		}

		// remove a value from a node
		tuple<BNode*,T,OP_RESULT> remove(const T &val){
			size_t index = 0;

			// checks if the node exists first, otherwise shifting will cause problems
			while(val >= keys[index] && index < countK)
				++index;
			if(val != keys[index - 1])
				return tuple<BNode*,T,OP_RESULT>(nullptr, val, OP_RESULT::NOT_FOUND);
			else{
				// shifts over elements, overwriting value
				while(index < countK){
					keys[index - 1] = keys[index];
					links[index] = links[index+1];
					++index;
				}
				links[index] = nullptr;
				--countK;
				--countL;
				if(countK < ceil(double(M)/2)-1)
					return tuple<BNode*,T,OP_RESULT>(this, val, OP_RESULT::UNDERFLOW);
				return tuple<BNode*,T,OP_RESULT>(this, val, OP_RESULT::SUCCESS);
			}
		}

		// merges 'this' node with 'node' and parent value
		tuple<BNode*,T,OP_RESULT> merge(BNode* &node, const T& parentVal){
			this->keys[countK++] = parentVal;
			this->links[countL++] = node->links[0];
			size_t index = 0;
			while(index < node->countK){
				this->keys[countK++] = node->keys[index++];
				this->links[countL++] = node->links[index];
			}
			delete node;
			node = nullptr;
			return tuple<BNode*,T,OP_RESULT> (this, parentVal, OP_RESULT::SUCCESS);
		}

		// print a given node, left to right
		void printNode(ostream& out = cout){
			for(size_t i = 0; i < countK; ++i){
				out << keys[i];
				if(i != countK-1)
					out << ", ";
			}
		}
	};

	BNode* root;
	size_t sz;
	size_t minKeys;

	tuple<BNode*, T, OP_RESULT> search(BNode*, const T&, ostream &out);
	tuple<BNode*, T, OP_RESULT> insert(BNode*, const T&);
	tuple<BNode*, T, OP_RESULT> remove(BNode*&, const T&);
	void deleteTree(BNode* );
	T findMin(BNode*);

  public:
	BTree();
	BTree(const T&);
	// copy constructor and operator= here if you get time
	~BTree();

	size_t size() const;
	void printTree(ostream& out = cout) const;
	OP_RESULT search(const T&, ostream &out = cout);
	OP_RESULT insert(const T&);
	OP_RESULT remove(const T&);
};


/////////////////////////////////		PRIVATE METHODS		  /////////////////////////////////////

template <typename T, size_t M>
tuple<typename BTree<T,M>::BNode*, T, typename BTree<T,M>::OP_RESULT> BTree<T,M>::search(BNode* node, const T& val, ostream &out){
	// if search reached a nullptr
	if(!node)
		return tuple<BNode*,T,OP_RESULT>(node, val, OP_RESULT::NOT_FOUND);

	size_t index;

	// check if val is found, or descend into correct link
	for(index = 0; index < node->countK; ++index){
		if(node->keys[index] == val){
			out << node->keys[index];
			return tuple<BNode*,T,OP_RESULT>(node, val, OP_RESULT::FOUND);
		}
		if(node->keys[index] > val){
			out << node->keys[index] << " -> ";
			return search(node->links[index], val, out);
		}
		out << node->keys[index] << " -> ";
	}
	//if we have to descend in the last link
	return search(node->links[node->countK], val, out);
}


template <typename T, size_t M>
tuple<typename BTree<T,M>::BNode*, T, typename BTree<T,M>::OP_RESULT> BTree<T,M>::insert(BNode* node, const T& val){
	if(node->links[0] == nullptr){	// if this is a leaf node
		return node->add(val);
	}
	else{
		size_t index;

		// figures out where to insert
		for(index = 0; index < node->countK; ++index){
			if(node->keys[index] == val) // if duplicate found, we dont need to check further
				return tuple<BNode*,T,OP_RESULT>(nullptr, val, OP_RESULT::DUPLICATE);
			if(node->keys[index] > val)  // finds which link we need to insert on
				break;
		}

		// checks for overflow, and acts accordingly
		tuple<BNode*,T,OP_RESULT> result = insert(node->links[index], val);
		if(get<2>(result) == OP_RESULT::OVERFLOW){
			tuple<BNode*,T,OP_RESULT> splitResult = node->links[index]->split();
			return node->add(get<1>(splitResult), get<0>(splitResult)); //passes the result to above chain
		}
		return result; // when there was no overflow from the result
	}
}

// non-leaf values are removed by replacing them with immediate successor, and then removing immediate successor
template <typename T, size_t M>
tuple<typename BTree<T,M>::BNode*, T, typename BTree<T,M>::OP_RESULT> BTree<T,M>::remove(BNode* &node, const T& val){
	size_t index;
	tuple<BNode*,T,OP_RESULT> removeResult;
	bool uflow = false;

	// traverse keys
	for(index = 0; index < node->countK; ++index){
		if(node->keys[index] == val){	// if value found
			if(node->links[0]){	// if this is an internal node
				node->keys[index] = findMin(node->links[index+1]);
				removeResult =	remove(node->links[index+1], node->keys[index]);
				if(get<2>(removeResult) == OP_RESULT::UNDERFLOW){
					// index is increased because the imm. successor is in the next slot over
					++index;
					uflow = true;
					break;
				}
				return removeResult;
			}
			else{	// this is a leaf node, safe to delete from here
				return(node->remove(val));
			}
		}
		// now we know which link to descend into, since we have encountered a >.
		else if(node->keys[index] > val){
			break;
		}
	}
	if(!uflow){
		if(node->links[0]){	// if internal node
			removeResult = remove(node->links[index], val);
			if(get<2>(removeResult) == OP_RESULT::UNDERFLOW)
				uflow = true;
		}
		else	// leaf node, but val not found
			return tuple<BNode*,T,OP_RESULT>(node,val,OP_RESULT::NOT_FOUND);
	}

	if(!uflow){
		if(get<2>(removeResult) == OP_RESULT::SUCCESS || get<2>(removeResult) == OP_RESULT::NOT_FOUND)
			return removeResult;
	}

	// checking if node is not the leftmost
	if(index != 0){		
		// checking if the left sibling has more than minimum keys, so borrow can work
		if(node->links[index-1]->countK > minKeys){
			for(size_t i = node->links[index]->countK; i > 0; --i){
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
			return tuple<BNode*,T,OP_RESULT>(node,val,OP_RESULT::SUCCESS);
		}
	}

	// checking if node is not the rightmost
	if(index != node->countK){
		// checking if right sibling has more than minimum keys, so borrow can work
		if(node->links[index + 1]->countK > minKeys){
			node->links[index]->keys[node->links[index]->countK] = node->keys[index];
			node->links[index]->countK++;
			node->links[index]->links[node->links[index]->countL] = node->links[index+1]->links[0];
			node->links[index]->countL++;
			node->keys[index] = node->links[index+1]->keys[0];
			node->links[index+1]->links[0] = nullptr;
			
			for(size_t i = 0; i < node->links[index+1]->countK; ++i){
				node->links[index+1]->keys[i] = node->links[index+1]->keys[i+1];
				node->links[index+1]->links[i] = node->links[index+1]->links[i+1];
			}
			node->links[index+1]->links[node->links[index+1]->countK-1] = node->links[index+1]->links[node->links[index+1]->countK];
			node->links[index+1]->countL--;
			node->links[index+1]->countK--;
			
			return tuple<BNode*,T,OP_RESULT>(node,val,OP_RESULT::SUCCESS);
		}
	}

	// if we made it here, we must have got underflow, so we must merge
	tuple<BNode*,T,OP_RESULT> mergeResult;

	// if node is all the way on the right, it calls merge on the 2nd last node, passing in this last node
	// IN THIS IMPLEMENTATION, A NODE IS ALWAYS MERGED WITH IT'S RIGHT SIBLING
	if(index == node->countL - 1){
		mergeResult = node->links[index-1]->merge(node->links[index], node->keys[node->countK - 1]);
		node->remove(node->keys[node->countK - 1]);
		node->links[index-1] = get<0>(mergeResult);	// adds the correct child ptr to the node
		if(node->countK < minKeys)
			return tuple<BNode*,T,OP_RESULT>(node,val,OP_RESULT::UNDERFLOW); 
	}
	else{
		mergeResult = node->links[index]->merge(node->links[index+1], node->keys[index]);	// should be @ index
		node->remove(node->keys[index]);
		node->links[index] = get<0>(mergeResult);	// this seems redundant, happened in merge already
		if(node->countK < minKeys)
			return tuple<BNode*,T,OP_RESULT>(node,val,OP_RESULT::UNDERFLOW);
	}
	return tuple<BNode*,T,OP_RESULT>(node,val,OP_RESULT::SUCCESS);
}

// only to be used in the destructor, because insert cannot create a new root node
template <typename T, size_t M>
void BTree<T,M>::deleteTree(BNode* node){
	if(node){
		for(size_t i = 0; i < node->countL; ++i)
			deleteTree(node->links[i]);
		delete node;
	}
}

// used to find the immediate successor when removing value from internal node
template <typename T, size_t M>
T BTree<T,M>::findMin(BNode* node){
	if(node->links[0])
		return findMin(node->links[0]);
	return node->keys[0];
}

/////////////////////////////////		PUBLIC METHODS		  /////////////////////////////////////

template <typename T, size_t M>
BTree<T,M>::BTree():sz{0}{
	root = new BNode();
	minKeys = ceil(double(M)/2) - 1;
}

template <typename T, size_t M>
BTree<T,M>::BTree(const T& val):sz{1}{
	root = new BNode(val);
	minKeys = ceil(double(M)/2) - 1;
}

template <typename T, size_t M>
BTree<T,M>::~BTree(){
	deleteTree(root);
}

template <typename T, size_t M>
size_t BTree<T,M>::size() const{
	return sz;
}

// prints a tree in level order by using a queue
template <typename T, size_t M>
void BTree<T,M>::printTree(ostream& out) const{
	if(root && sz){
		queue<BNode*> nodes;
		nodes.push(root);

		// while the queue isn't empty
		while(nodes.size()){
			size_t levelCount = nodes.size();
			// levelCount used for getting number of nodes on particular level
			while(levelCount){
				out << "[";
				nodes.front()->printNode(out);
				levelCount > 1 ? (out << "] "): (out << "]");
				// appends all the non-null children of a node to the queue
				for(size_t i = 0; i < nodes.front()->countL && nodes.front()->links[i] != nullptr; ++i)
					nodes.push(nodes.front()->links[i]);
				nodes.pop();
				--levelCount;
			}
			if(nodes.size())
				out << endl;
		}
	}
}

// public search
template <typename T, size_t M>
typename BTree<T,M>::OP_RESULT BTree<T,M>::search(const T& val, ostream &out){
	if(sz == 0)
		return OP_RESULT::NOT_FOUND;
	return get<2>(search(root, val, out));
}

// public insert
template <typename T, size_t M>
typename BTree<T,M>::OP_RESULT BTree<T,M>::insert(const T& val){
	// if tree is empty, add to root
	if(sz == 0){
		++sz;
		return get<2>(root->add(val));
	}
	else{
		tuple<BNode*,T,OP_RESULT> insertResult = insert(root, val);
		
		// if duplicate found, we can just return it
		if(get<2>(insertResult) == OP_RESULT::DUPLICATE)
			return OP_RESULT::DUPLICATE;
		
		// when root overflows, split is called and new root is created
		else if(get<2>(insertResult) == OP_RESULT::OVERFLOW){
			++sz;
			tuple<BNode*,T,OP_RESULT> splitResult = root->split();
			BNode* newRoot = new BNode(get<1>(splitResult));
			newRoot->links[0] = root;
			newRoot->links[1] = get<0>(splitResult);
			root = newRoot;
			return OP_RESULT::SUCCESS;
		}
		else{	//success because root didn't overflow
			++sz;
			return OP_RESULT::SUCCESS;
		}
	}
}

// public remove
template <typename T, size_t M>
typename BTree<T,M>::OP_RESULT BTree<T,M>::remove(const T& val){
	// if tree is empty, no value can exist
	if(sz == 0)
		return OP_RESULT::NOT_FOUND;
	
	tuple<BNode*,T,OP_RESULT> result = remove(root,val);
	
	// if anything other than NOT_FOUND was returned (i.e. success or underflow)
	if(get<2>(result) != OP_RESULT::NOT_FOUND)
		--sz;
	
	// the root has no values
	if(!root->countK){
		// if the first child isn't null, we need to reduce height of tree by one and set root to that child
		if(root->links[0] && sz){
			BNode* oldRoot = root;
			root = root->links[0];	// might need to change this logic if a different child is the non-null one
			delete oldRoot;
		}
	}
	return get<2>(result);
}
