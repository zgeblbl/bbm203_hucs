#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    bool compareSectors(Sector *root, Sector *sector);

    void displayInOrder(Sector *node);

    void displayPreOrder(Sector *node);

    void displayPostOrder(Sector *sector);

    void insertSectorRecursiveHelper(Sector *currentNode, Sector *newSector);

    Sector *findSectorByCodeHelper(Sector *root, const std::string &sector_code);

    void destroyTree(Sector *node);
};

#endif // SPACESECTORBST_H
