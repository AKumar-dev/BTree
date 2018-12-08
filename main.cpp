#include <fstream>
#include <sstream>  // need to get rid of this later
#include <cstdlib>  // for srand, rand, and time
#include "BTree.h"

using namespace std;

int main(){

    BTree<int, 5> f;
    ifstream in("sampleIn");
    
    int temp;
    while(in >> temp){
        cout << "insert: " << temp << endl;
        f.insert(temp);
        f.printTree();
        cout << endl << endl;
    }

    cout << endl << endl << "---------- FINISHED INSERTING ----------" << endl << endl;
   
    ifstream in2("sampleIn");
    int count = 0;
    while(in2 >> temp){
        ++count;
        cout << endl << endl<<"remove: " << temp << endl;
        f.remove(temp);
        f.printTree();
    }

    cout << endl << "adding 1,2,3,4,5 to same tree, so checking that structure holds after removing" << endl;

    cout << endl;

    f.insert(1);
    f.insert(2);
    f.insert(3);
    f.insert(4);
    f.insert(5);
    
    f.printTree();

    cout << endl << endl;
    return 0;
}