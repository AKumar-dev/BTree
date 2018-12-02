#include <fstream>
#include <sstream>  // need to get rid of this later
#include <cstdlib>  // for srand, rand, and time
#include "BTree.h"

using namespace std;

// MIGHT HAVE TO FIX SOME OF THE DESTRUCTOR STUFF, CHECK VALGRIND
int main(){
    BTree<int, 5> f;
    BTree<int, 7> s;

    ifstream in("sampleIn");
    ofstream out("sampleOut");
    
    int temp;
    while(in >> temp)
        f.insert(temp);

    f.printTree(out);


    return 0;
}