#include <fstream>
#include <sstream>  // need to get rid of this later
#include <cstdlib>  // for srand, rand, and time
#include "BTree.h"

using namespace std;

// MIGHT HAVE TO FIX SOME OF THE DESTRUCTOR STUFF, CHECK VALGRIND
int main(){
    BTree<int, 5> f;

    ifstream in("sampleIn");
    ofstream out("sampleOut");
    
    int temp;
    while(in >> temp)
        f.insert(temp);

    f.printTree();
    cout << endl << endl;

    // f.remove(88);
    // f.remove(99);
    // f.remove(58);
    // f.remove(70);
    // f.remove(83);
    f.remove(2);
    cout << endl << endl;
    f.printTree(cout);
    cout << endl;

    return 0;
}