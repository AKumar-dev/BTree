#include <fstream>
#include <string> // for getting filename
#include "BTree.h"

using namespace std;

template<typename T, size_t M = 5>
void loadFile(BTree<T,M> &tree, const string &filename){
    ifstream file(filename.c_str());
    if(file){
        int value;
        while(file >> value){
            cout << "INSERTING: " << value << endl;
            typename BTree<T,M>::CODE test;
            if((test = tree.insert(value)) == BTree<T,M>::CODE::DUPLICATE){
                cout << "duplicate found, " << value << " not inserted." << endl;
            };
            cout << "code returned: " << test << endl;
        }
    }
    else{
        cout << "invalid file passed for loading -- skipping to next instruction" << endl;
    }
}

int main(int argc, char *argv[]){

    ifstream inputFile(argv[1]);

    BTree<int, 5> tree;

    if(inputFile){
        char command;
        while(inputFile >> command){
            char colon;
            inputFile >> colon;
            if(command == 'L'){
                string filename;
                inputFile >> filename;
                loadFile(tree, filename);
            }
            else if(command == 'A'){
                int value;
                inputFile >> value;
                if(tree.insert(value) == BTree<int,5>::CODE::DUPLICATE){
                    cout << "duplicate found, " << value << " not inserted." << endl;
                };
                // check for duplicate
            }
            else if(command == 'D'){
                int value;
                inputFile >> value;
                tree.remove(value);
                // check for not found
            }
            else if(command == 'S'){
                int value;
                inputFile >> value;
                // STILL NEED TO DEFINE THE SEARCH FUNCTION !_!_!_!_!_!_!_!_!_!_!_!_!_!_!_
            }
            else if(command == 'P'){
                tree.printTree();
                cout << endl << endl;
            }
            else if(command == 'T'){
                break;
            }
            else{
                cout << "invalid command, program exiting!" << endl;
                break;
            }
        }
        inputFile.close();
    }
    else{
        cout << "Wrong file name, or command line argument not specified. Try agian." << endl;
    }

    /* SAMPLE DATA TO TEST INSERTIONS AND DELETIONS

    BTree<int, 5> f;
    ifstream in("sampleIn");
    
    int temp;
    while(in >> temp){
        cout << "insert: " << temp << endl;
        f.insert(temp);
        f.printTree();
        cout << endl << endl;
    }
    in.close()

    cout << endl << endl << "---------- FINISHED INSERTING ----------" << endl << endl;
   
    ifstream in2("sampleIn");
    int count = 0;
    while(in2 >> temp){
        ++count;
        cout << endl << endl<<"remove: " << temp << endl;
        f.remove(temp);
        f.printTree();
    }
    in2.close();

    cout << endl << "adding 1,2,3,4,5 to same tree, so checking that structure holds after removing" << endl;

    cout << endl;

    f.insert(1);
    f.insert(2);
    f.insert(3);
    f.insert(4);
    f.insert(5);
    
    f.printTree();

    cout << endl << endl;
    */

    return 0;
}