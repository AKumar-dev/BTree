#include <fstream>
#include <cstdlib>  // for srand, rand, and time
#include "BTree.h"

using namespace std;

// MIGHT HAVE TO FIX SOME OF THE DESTRUCTOR STUFF, CHECK VALGRIND
int main(){
    cout << endl;

    BTree<int, 5> temp(10);

    /*// FOR CHECKING IF ADD FUNCTION WORKS

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

    /*// FOR CHECKING IF SPLIT FUNCTION WORKS 

    BNode<6> temp;
    BNode<6> *root;
    for(int i = 0; i <= 10; ++i){
        tuple<BNode<6>*,int,CODE> result = temp.add(i);
        if(get<2>(result) == CODE::OVERFLOW){
            root = new BNode<6>;
            result = temp.split();
            root->keys[0] = get<1>(result);
            root->links[0] = &temp;
            root->links[1] = get<0>(result);
            ++root->countK;
            ++root->countL;
            break;
        }
    }

    root->printNode();
    cout << endl;
    root->links[0]->printNode();
    cout << endl;
    root->links[1]->printNode();
    */




    return 0;
}