#include <fstream>
#include <sstream>  // need to get rid of this later
#include <cstdlib>  // for srand, rand, and time
#include "BTree.h"

using namespace std;

int main(){
    BTree<int, 5> f;

    // 29 44 77 30 33 79 83 35 99 88 25 22 26 31 45 58 9 3 2 49 67 70
    ifstream in("sampleIn");
    ofstream out("sampleOut");
    
    int temp;
    while(in >> temp)
        f.insert(temp);

    f.printTree();

    // f.remove(88);
    // f.remove(99);
    // f.remove(58);
    // f.remove(70);
    // f.remove(83);
    //f.remove(2);
    f.remove(73);

    cout << endl << endl;
    f.printTree(cout);

    f.remove(49);
    cout << endl << endl;
    f.printTree(cout);
    cout << endl;

    return 0;
}