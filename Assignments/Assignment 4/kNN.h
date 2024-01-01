// kNN.h
#ifndef KNN_H
#define KNN_H

#include "KD_Tree.h"
#include "kNN_Data.h"
#include <vector>

class KNN {
private:


public:
    KD_Tree tree;
    int k; // Number of neighbors for kNN
    double split_threshold; // Threshold for the kd_tree

    KNN(int k, double threshold);
    void train(Dataset& data); // Need to initialize the tree here 
    int predict(const Point& queryPoint);


    double calculateDistance(const Point &p1, const Point &p2);

    KDTreeNode *knnSearch(KDTreeNode *node, const Point &queryPoint);
};

#endif // KNN_H