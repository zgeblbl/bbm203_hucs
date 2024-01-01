#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    bool compareSectors(Sector *root, Sector *sector);

    void insertSectorRecursiveHelper(Sector *currentNode, Sector *newSector);

    void displayInOrder(Sector *sector);

    void displayPreOrder(Sector *sector);

    void displayPostOrder(Sector *sector);

    Sector *findSectorByCodeHelper(Sector *root, const std::string &sector_code);

    void balanceMaker(Sector *sector);

    Sector *findSectorByCodeHelper(Sector *root, const std::string &sector_code, std::vector<std::string> &path);

    Sector *findLCA(Sector *root, int a, int b);

    Sector *findLCA(Sector *root, Sector *earth, Sector *destination);

    void destroyTree(Sector *node);
};

#endif // SPACESECTORLLRBT_H
