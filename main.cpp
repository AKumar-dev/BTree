#include <fstream>
#include <tuple>
#include <cstdlib>  // for srand, rand, and time
#include <iostream>

using BTuple = tuple<BNode<>*,int,CODE>;

using namespace std;

enum CODE{NOT_FOUND, SUCCESS, DUPLICATE, OVERFLOW};

template<int M = 5>
struct BNode{
    int countL;     // count of links
    int countK;     // count of keys
    int *keys;      // the size of this is M (M = order)
    int* *links;    // the size of this is M + 1

// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    // add/insert a value into a BNode
    BTuple add(const int &val){
        // if the node is empty, insert val and return success
        if(!countK){
            keys[0] = val;
            ++countK;
            return BTuple(nullptr, val, CODE::SUCCESS);
        }
        // else, insert the value in proper location
        else{
            // checks for duplicate so we don't waste time shifting elements if duplicate exists
            for(int i = 0; i < countK; ++i){
                if(keys[i] == val)
                    return BTuple(nullptr, val, CODE::DUPLICATE);
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
                return BTuple(nullptr, val, CODE::OVERFLOW);

            return BTuple(nullptr, val, CODE::SUCCESS);
        }
    }
    
    
    // TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    BTuple split(){
        int median = countK/2;
        int index = median + 1;
        int oldCount = countK;

        // new node that data past the median will be copied into
        BNode right = new BNode;
        
        // copies over the first link, so in the future when we copy a key we only have to copy the right side link
        right->links[0] = links[index];
        ++right->countL;
        links[index] = nullptr;
        --countL;
        
        // copies over the remaining values
        while(index < oldCount){
            right->links[right->countL] = links[index + 1];         // OPTIMIZE THIS LATER WITH COUNT++
            right->keys[right->countK] = keys[index];               // OPTIMIZE THIS LATER WITH COUNT++
            ++right->countK;
            ++right->countL;
            links[index + 1] = nullptr;
            --countL;
            --countK;
            ++ index;
        }
        --countK;
        return BTuple(right, keys[median], CODE::SUCCESS);
    }

    /*
    // TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    BTuple<M> merge(){

    }*/

    void printNode(){
        for(int i = 0; i < countK; ++i){
            cout << "link: " << (links[i]? "valid" : "nullptr") << endl;
            cout << "val: " << keys[i] << endl;
        }
        cout << "link: " << (links[countK]? "valid" : "nullptr") << endl;
    }
    BNode(){
        countK = 0;
        countL = 0;
        keys = new int[M];
        links = new int*[M+1];
        for(int i = 0; i <= M; ++i)
             links[i] = nullptr;
    }
    
    BNode(int val){
        countK = 1;
        countL = 2;
        keys = new int[M];
        keys[0] = val;
        links = new int*[M+1];
        for(int i = 0; i <= M; ++i)
            links[i] = nullptr;
    }
    ~BNode(){
        delete[] keys;
        delete[] links;
    }
};

// used to indicate outcome of a BNode operation

int main(){
    cout << endl;

    /* FOR CHECKING IF ADD FUNCTION WORKS

    srand(time(NULL));
    int val = rand()%10 + 1;
    for(int i = 0; i < 10; ++i){
        BNode<10> temp;
        CODE answer;
        while((answer = get<2>(temp.add(val))) != CODE::OVERFLOW){
            val = rand()%10 + 1;
            if(answer == CODE::DUPLICATE)
                cout << "DUPLICATE"  << endl;
        }
        temp.printNode();
        cout << endl;        
    }
    */

    BNode<5> temp;
    BNode<5> *root;
    for(int i = 0; i <= 10; ++i){
        BTuple result = temp.add(i);
        /*if(get<2>(result) == CODE::OVERFLOW){
            root = new BNode<5>;
            result = temp.split();
            root->keys[0] = get<1>(result);
            root->links[0] = temp;
            root->links[1] = get<0>(result);
            ++root->countK;
            ++root->countL;
            break;
        }*/
    }

    root->printNode();
    root->links[0]->printNode();
    root->links[1]->printNode();

    return 0;
}