#include "SpaceSectorBST.h"
#include <unordered_set>
#include <algorithm>

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    destroyTree(root);
}
void SpaceSectorBST::destroyTree(Sector* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
bool SpaceSectorBST::compareSectors(Sector* root, Sector* sector){
    if(root->x==sector->x){
        if(root->y==sector->y){
            if(root->z < sector->z){
                return true;
            }else{
                return false;
            }
        }else if (root->y < sector->y){
            return true;
        }else{
            return false;
        }
    }else if(root->x < sector->x) {
        return true;
    }else{
        return false;
    }
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        while (std::getline(file, line)) {
            int x, y, z;
            sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
            insertSectorByCoordinates(x,y,z);
        }
        file.close();
    }
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison criteria based on the sector coordinates.
}
void SpaceSectorBST::insertSectorRecursiveHelper(Sector* currentNode, Sector* newSector) {
    if (compareSectors(currentNode, newSector)) {
        if (currentNode->right == nullptr) {
            currentNode->right = newSector;
            newSector->parent = currentNode;
        } else {
            insertSectorRecursiveHelper(currentNode->right, newSector);
        }
    } else {
        if (currentNode->left == nullptr) {
            currentNode->left = newSector;
            newSector->parent = currentNode;
        } else {
            insertSectorRecursiveHelper(currentNode->left, newSector);
        }
    }
}
void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    Sector *sector = new Sector(x, y, z);
    sector->color=false;
    sector->parent=nullptr;
    sector->left=nullptr;
    sector->right=nullptr;
    if (root== nullptr){
        root = sector;
    }else if (root!=sector){
        insertSectorRecursiveHelper(root, sector);
    }
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
}

Sector* SpaceSectorBST::findSectorByCodeHelper(Sector* root, const std::string& sector_code) {
    if (root != nullptr && root->sector_code == sector_code) {
        return root;
    }
    if (root != nullptr) {
        Sector* leftResult = findSectorByCodeHelper(root->left, sector_code);
        if (leftResult != nullptr) {
            return leftResult;
        }
        Sector* rightResult = findSectorByCodeHelper(root->right, sector_code);
        if (rightResult != nullptr) {
            return rightResult;
        }
    }
    return nullptr;
}
void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    Sector* dangerous = findSectorByCodeHelper(root, sector_code);
    if (dangerous != nullptr) {
        if (dangerous->left == nullptr && dangerous->right == nullptr) {
            if(dangerous==root) {
                root= nullptr;
                delete root;
            }else{
                if(compareSectors(dangerous->parent, dangerous)){
                    dangerous->parent->right= nullptr;
                }else{
                    dangerous->parent->left= nullptr;
                }
                dangerous= nullptr;
                delete dangerous;
            }
        }
        else if (dangerous->left == nullptr){
            if(dangerous==root) {
                root = root->right;
                if (root != nullptr) {
                    root->parent = nullptr;
                }
            }else{
                if(compareSectors(dangerous->parent, dangerous)){
                    dangerous->parent->right= dangerous->right;
                }else{
                    dangerous->parent->left= dangerous->right;
                }
                dangerous->right->parent = dangerous->parent;
                dangerous=nullptr;
                delete dangerous;
            }
        }
        else if (dangerous->right == nullptr){
            if(dangerous==root) {
                root = root->left;
                if (root != nullptr) {
                    root->parent = nullptr;
                }
            }else{
                if(compareSectors(dangerous->parent, dangerous)){
                    dangerous->parent->right= dangerous->left;
                }else{
                    dangerous->parent->left= dangerous->left;
                }
                dangerous->left->parent = dangerous->parent;
            }
            dangerous= nullptr;
            delete dangerous;
        }else if (dangerous->left != nullptr && dangerous->right != nullptr){
            Sector* rightRoot = dangerous->right;
            while (rightRoot->left != nullptr) {
                rightRoot = rightRoot->left;
            }
            if(compareSectors(rightRoot->parent, rightRoot)){
                rightRoot->parent->right= nullptr;
            }else{
                rightRoot->parent->left= nullptr;
            }
            if (dangerous==root) {
                root = rightRoot;
                root->parent = nullptr;
            }else{
                if(compareSectors(dangerous->parent, dangerous)){
                    dangerous->parent->right= rightRoot;
                }else{
                    dangerous->parent->left= rightRoot;
                }
                rightRoot->parent = dangerous->parent;
            }
            if (dangerous->left!=nullptr){
                if (rightRoot->left == nullptr) {
                    rightRoot->left = dangerous->left;
                    dangerous->left->parent = rightRoot;
                }
            }
            if (dangerous->right != nullptr){
                if (rightRoot->right!=nullptr){
                    Sector* temp = rightRoot->right;
                    while (temp->right != nullptr) {
                        temp = temp->right;
                    }
                    temp->right = dangerous->right;
                    dangerous->right->parent = temp;
                }else{
                    rightRoot->right = dangerous->right;
                    dangerous->right->parent = rightRoot;
                }
            }
            dangerous= nullptr;
            delete dangerous;
        }
    }
    // TODO: Delete the sector given by its sector_code from the BST.
}

void SpaceSectorBST::displayInOrder(Sector* sector) {
    if (sector != nullptr) {
        displayInOrder(sector->left);
        std::cout<<sector->sector_code << std::endl;
        displayInOrder(sector->right);
    }
}
void SpaceSectorBST::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    displayInOrder(root);
    std::cout<<std::endl;
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
}
void SpaceSectorBST::displayPreOrder(Sector* sector) {
    if (sector != nullptr) {
        std::cout << sector->sector_code << std::endl;
        displayPreOrder(sector->left);
        displayPreOrder(sector->right);
    }
}
void SpaceSectorBST::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:\n";
    displayPreOrder(root);
    std::cout<<std::endl;
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}
void SpaceSectorBST::displayPostOrder(Sector* sector) {
    if (sector != nullptr) {
        displayPostOrder(sector->left);
        displayPostOrder(sector->right);
        std::cout << sector->sector_code << std::endl;
    }
}
void SpaceSectorBST::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    displayPostOrder(root);
    std::cout<<std::endl;
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    Sector * destination = findSectorByCodeHelper(root, sector_code);
    if (destination == root){
        path.push_back(destination);
    }else if (destination== nullptr){
        return path;
    }else{
        std::unordered_set<Sector*> visited;
        while(destination != nullptr && visited.find(destination) == visited.end()) {
            path.push_back(destination);
            visited.insert(destination);
            destination = destination->parent;
        }
    }
    std::reverse(path.begin(), path.end());
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    return path;
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()){
        std::cout<<"A path to Dr. Elara could not be found."<<std::endl;
    }else{
        std::cout<<"The stellar path to Dr. Elara: ";
        for (size_t i=0;i<path.size();i++){
            std::cout<<path[i]->sector_code;
            if (i<path.size()-1){
                std::cout<<"->";
            }else{
                std::cout<<"\n";
            }
        }
    }
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}