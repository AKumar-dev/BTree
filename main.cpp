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
            typename BTree<T,M>::CODE test;
            if((test = tree.insert(value)) == BTree<T,M>::CODE::DUPLICATE){
                cout << "DUPLICATE found, " << value << " not inserted." << endl;
            };
        }
        file.close();
    }
    else{
        cout << "invalid file passed for loading -- skipping to next instruction" << endl;
    }
}

int main(int argc, char *argv[]){
    cout << endl;
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
                cout << "inserting: " << value << endl;
                if(tree.insert(value) == BTree<int,5>::CODE::DUPLICATE)
                    cout << "duplicate found, " << value << " not inserted." << endl;
            }
            else if(command == 'D'){
                int value;
                inputFile >> value;
                cout << "removing: " << value << endl;
                if(tree.remove(value) == BTree<int,5>::CODE::NOT_FOUND)
                    cout << " * " << value << " not found" << endl << endl;
            }
            else if(command == 'S'){
                int value;
                inputFile >> value;
                cout << "searching for " << value << ": " << endl;
                if(tree.search(value) == BTree<int,5>::CODE::NOT_FOUND)
                    cout << "VALUE NOT FOUND" << endl;
                cout << endl;
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

    return 0;
}