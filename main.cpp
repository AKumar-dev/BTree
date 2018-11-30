#include <iostream>
#include <fstream>
#include <tuple>

using namespace std;

enum CODE{NOT_FOUND, SUCESS, DUPLICATE, OVERFLOW};

struct BNode{
    int countL;
    int countK;
    int *keys;      // the size of this is 1 greater than (M-1)
    int* *links;    // the size of this is 1 greater than M


// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    /*tuple<BNode*, int, CODE> add(const int val){
        int index = countL - 1;
        while(index >= 0 && val <= keys[index]){
            if(val == keys[index])
                return tuple<BNode*,int,CODE>(nullptr, val, CODE::DUPLICATE);
            keys[index + 1] = keys[index]; // draw this out on paper
            // also need to copy over the links
            index -= 1;
        }
        keys[index[]]
    }
    // TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    tuple<BNode*, int, CODE> split(){

    }
    // TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!// TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
    tuple<BNode*, int, CODE> merge(){

    }*/
    // TEST THIS FUNCTION FOR ALL CASES BEFORE YOU MOVE ON!!!
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
        keys = new int[4];
        links = new int*[5];
        for(int i = 0; i < 5; ++i)
             links[i] = nullptr;
    }
    
    BNode(int val){
        countK = 1;
        countL = 2;
        keys = new int[4];
        keys[0] = val;
        links = new int*[5];
        for(int i = 0; i < 5; ++i)
            links[i] = nullptr;
    }
    ~BNode(){
        delete[] keys;
        delete[] links;
    }
};

int main(){
    BNode one(15);
    BNode two(20);
    BNode three;

    one.printNode();
    cout << endl;
    two.printNode();
    cout << endl;
    three.printNode();
    return 0;
}