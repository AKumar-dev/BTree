#include <fstream>
#include <cstdlib>  // for srand, rand, and time
#include "BTree.h"

using namespace std;

// MIGHT HAVE TO FIX SOME OF THE DESTRUCTOR STUFF, CHECK VALGRIND
int main(){
    cout << endl;

    // 29 44 77 30 33 79 83 35 99 88 25 22 26 31 45 58 9 3 2 49 67 70

    BTree<int, 5> first;

    first.insert(29);
    first.insert(44);
    first.insert(77);
    first.insert(30);
    first.insert(33);
    first.insert(79);
    first.insert(83);
    first.insert(35);
    first.insert(99);
    first.insert(88);
    first.insert(25);
    first.insert(22);
    first.insert(26);
    first.insert(31);
    first.insert(45);
    first.insert(58);
    first.insert(9);
    first.insert(3);
    first.insert(2);
    first.insert(49);
    first.insert(67);
    first.insert(70);

    first.printTree();


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