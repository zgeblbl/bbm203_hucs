#include "SpaceSectorLLRBT.h"
#include <math.h>
#include <algorithm>

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

bool SpaceSectorLLRBT::compareSectors(Sector* root, Sector* sector){
    if (root->x != sector->x) {
        return root->x < sector->x;
    }
    if (root->y != sector->y) {
        return root->y < sector->y;
    }
    return root->z < sector->z;
}
void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        while (std::getline(file, line)) {
            int x, y, z;
            sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
            insertSectorByCoordinates(x, y, z);
        }
        file.close();
    }
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison criteria based on the sector coordinates.
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    destroyTree(root);
    // TODO: Free any dynamically allocated memory in this class.
}
void SpaceSectorLLRBT::destroyTree(Sector* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
void SpaceSectorLLRBT::insertSectorRecursiveHelper(Sector* currentNode, Sector* newSector) {
    if (compareSectors(currentNode, newSector)) {
        if (currentNode->right == nullptr) {
            newSector->color=true;
            currentNode->right = newSector;
            newSector->parent = currentNode;
        } else {
            insertSectorRecursiveHelper(currentNode->right, newSector);
        }
    } else {
        if (currentNode->left == nullptr) {
            newSector->color=true;
            currentNode->left = newSector;
            newSector->parent = currentNode;
        } else {
            insertSectorRecursiveHelper(currentNode->left, newSector);
        }
    }
}
void SpaceSectorLLRBT::balanceMaker(Sector* sector){
    if (sector->parent->right!= nullptr && sector->parent->left!= nullptr&&sector->parent->right->color&&sector->parent->left->color){
        sector->parent->left->color=false;
        sector->parent->right->color=false;
        if (sector->parent !=root){
            sector->parent->color=true;
            balanceMaker(sector->parent);
        }
    }
    else if(compareSectors(sector->parent, sector)){
        if (sector->color){
            if (sector->left!=nullptr){
                sector->parent->right = sector->left;
                sector->left->parent=sector->parent;
                sector->left=nullptr;
            }else{
                sector->parent->right=nullptr;
            }
            //turn left
            if (sector->parent==root){
                root->color=true;
                sector->parent->parent = sector;
                sector->left = sector->parent;
                sector->parent= nullptr;
                sector->color=false;
                root = sector;
            }else{
                Sector * parent = sector->parent;
                Sector * grandp = parent->parent;
                sector->color=false;
                if (parent->color){
                    sector->color=true;
                }
                parent->color=true;
                if (compareSectors(grandp, parent)){
                    grandp->right = sector;
                }else{
                    grandp->left = sector;
                }
                parent->parent = sector;
                sector->left = parent;
                sector->parent = grandp;
            }
            balanceMaker(sector->left);
        }
    }else{
        if (sector->color&&sector->parent->color){
            //right rot
            Sector * parent = sector->parent;
            Sector * grandp = sector->parent->parent;
            if (grandp==root){
                grandp->color=true;
                parent->color=false;
                root = parent;
                parent->parent=nullptr;
                if (parent->right== nullptr){
                    grandp->left= nullptr;
                }else{
                    grandp->left = parent->right;
                    parent->right->parent = grandp;
                }
                parent->right = grandp;
                grandp->parent = parent;
            }else{
                if (grandp->color){
                    parent->color = true;
                }else{
                    parent->color = false;
                }
                grandp->color=true;
                parent->parent = grandp->parent;
                if (compareSectors(grandp->parent,grandp)){
                    grandp->parent->right = parent;
                }else{
                    grandp->parent->left = parent;
                }
                if (parent->right == nullptr){
                    grandp->left= nullptr;
                }else{
                    grandp->left= parent->right;
                    parent->right->parent = grandp;
                }
                parent->right = grandp;
                grandp->parent = parent;
            }
            balanceMaker(grandp);
        }
    }
}
void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    Sector *sector = new Sector(x, y, z);
    sector->parent=nullptr;
    sector->left=nullptr;
    sector->right=nullptr;
    if (root== nullptr){
        root = sector;
        root->color=false;
    }else{
        insertSectorRecursiveHelper(root, sector);
        balanceMaker(sector);
    }
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.
}
void SpaceSectorLLRBT::displayInOrder(Sector* sector) {
    if (sector != nullptr) {
        displayInOrder(sector->left);
        if (sector->color){
            std::cout<<"RED"<<" sector: "<<sector->sector_code << std::endl;
        }else{
            std::cout<<"BLACK"<<" sector: "<<sector->sector_code << std::endl;
        }
        displayInOrder(sector->right);
    }
}
void SpaceSectorLLRBT::displaySectorsInOrder() {
    std::cout << "Space sectors inorder traversal:\n";
    displayInOrder(root);
    std::cout<<std::endl;
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
}
void SpaceSectorLLRBT::displayPreOrder(Sector* sector) {
    if (sector != nullptr) {
        if (sector->color){
            std::cout<<"RED"<<" sector: "<<sector->sector_code << std::endl;
        }else{
            std::cout<<"BLACK"<<" sector: "<<sector->sector_code << std::endl;
        }
        displayPreOrder(sector->left);
        displayPreOrder(sector->right);
    }
}
void SpaceSectorLLRBT::displaySectorsPreOrder() {
    std::cout << "Space sectors preorder traversal:\n";
    displayPreOrder(root);
    std::cout<<std::endl;
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
}
void SpaceSectorLLRBT::displayPostOrder(Sector* sector) {
    if (sector != nullptr) {
        displayPostOrder(sector->left);
        displayPostOrder(sector->right);
        if (sector->color){
            std::cout<<"RED"<<" sector: "<<sector->sector_code << std::endl;
        }else{
            std::cout<<"BLACK"<<" sector: "<<sector->sector_code << std::endl;
        }
    }
}
void SpaceSectorLLRBT::displaySectorsPostOrder() {
    std::cout << "Space sectors postorder traversal:\n";
    displayPostOrder(root);
    std::cout<<std::endl;
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
}

Sector* SpaceSectorLLRBT::findSectorByCodeHelper(Sector* root, const std::string& sector_code) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->sector_code == sector_code) {
        return root;
    }
    Sector* leftResult = findSectorByCodeHelper(root->left, sector_code);
    if (leftResult != nullptr) {
        return leftResult;
    }
    return findSectorByCodeHelper(root->right, sector_code);
}

Sector* SpaceSectorLLRBT::findLCA(Sector* root, Sector* earth, Sector* destination) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root==earth || root==destination){
        return  root;
    }
    if (!compareSectors(root, earth) && !compareSectors(root, destination)) {
        return findLCA(root->left, earth, destination);
    }
    else if (compareSectors(root, earth) && compareSectors(root, destination)) {
        return findLCA(root->right, earth, destination);
    }else{
        return root;
    }
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    Sector* earth = findSectorByCodeHelper(root, "0SSS");
    Sector* destination = findSectorByCodeHelper(root, sector_code);
    if (destination != nullptr) {
        Sector* lca = findLCA(root, earth, destination);
        if (earth != lca) {
            while (earth != lca) {
                path.push_back(earth);
                earth = earth->parent;
            }
        }
        path.push_back(lca);
        std::vector<Sector*> halfPath;
        while (destination != lca) {
            halfPath.push_back(destination);
            destination = destination->parent;
        }
        std::reverse(halfPath.begin(), halfPath.end());
        path.insert(path.end(), halfPath.begin(), halfPath.end());
    }
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    if (path.empty()) {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
    } else {
        std::cout << "The stellar path to Dr. Elara: ";
        for (size_t i = 0; i < path.size(); i++) {
            std::cout << path[i]->sector_code;
            if (i < path.size() - 1) {
                std::cout << "->";
            } else {
                std::cout << "\n";
            }
        }
    }
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}