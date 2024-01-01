#include "Sector.h"
#include <cmath>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
    double distance =  sqrt((pow(x,2) + pow(y,2) + pow(z,2)));
    distance_from_earth = distance;
    std::string code = std::to_string(static_cast<int>(distance_from_earth));
    if (x==0){
        code.append("S");
    }else if (x>0){
        code.append("R");
    }else if (x<0){
        code.append("L");
    }if (y==0){
        code.append("S");
    }else if (y>0){
        code.append("U");
    }else if (y<0){
        code.append("D");
    }if (z==0){
        code.append("S");
    }else if (z>0){
        code.append("F");
    }else if (z<0){
        code.append("B");
    }
    sector_code = code;
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->distance_from_earth = other.distance_from_earth;
    this->sector_code = other.sector_code;
    this->color = other.color;
    this->left = other.left;
    this->right = other.right;
    this->parent = other.parent;
    // TODO: Overload the assignment operator
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}