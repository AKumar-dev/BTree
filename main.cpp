#include <iostream>
#include <fstream>
#include <tuple>
#include <cstdlib>

#define Btuple tuple<BNode*,int,CODE>

using namespace std;

enum CODE{NOT_FOUND, SUCCESS, DUPLICATE, OVERFLOW};

template<int M = 5>
struct BNode{
    int countL;
    int countK;
    int *keys;      // the size of this is 1 greater than (M-1)
    int* *links;    // the size of this is 1 greater than M


// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    Btuple add(const int &val){
        if(!countK){
            keys[0] = val;
            ++countK;
            return Btuple(nullptr, val, CODE::SUCCESS);
        }
        else{
            for(int i = 0; i < countK; ++i){
                if(keys[i] == val)
                    return Btuple(nullptr, val, CODE::DUPLICATE);
            } // checks for duplicates

            int index = countK;
            while(index > 0 && keys[index-1] > val){
                keys[index] = keys[index - 1];
                --index;
            } // shifts over all the values that are smaller

            keys[index] = val;
            ++countK;

            if(countK == M) // if overflow happened
                return Btuple(nullptr, val, CODE::OVERFLOW);
            return Btuple(nullptr, val, CODE::SUCCESS);
        }
    }
    
    /*
    // TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    BTuple split(){

    }
    // TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    BTuple merge(){

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

    return 0;
}