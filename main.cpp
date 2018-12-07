#include <fstream>
#include <sstream>  // need to get rid of this later
#include <cstdlib>  // for srand, rand, and time
#include "BTree.h"

using namespace std;

int main(){
    BTree<int, 5> f;

    // 29 44 77 30 33 79 83 35 99 88 25 22 26 31 45 58 9 3 2 49 67 70
    ifstream in("sampleIn");
    
    int temp;
    while(in >> temp)
        f.insert(temp);

    f.printTree();

    // f.remove(88);
    // f.remove(99);
    // f.remove(58);
    // f.remove(70);
    // f.remove(83);
    // f.remove(2);

     
    ifstream in2("sampleIn");
    int count = 0;
    while(in2 >> temp){
        ++count;
        cout << endl << endl<<"remove " << temp << endl;
        f.remove(temp);
        f.printTree();
        cout << "after-print, count = " << count << endl;
        if(count == 12){  // 5 works, test 6
            cout << "end-of-while" << endl;
            break;
        }
        cout << "end-of-while" << endl;
    }

    // for some reason, 937 is being copied over!

    return 0;
}