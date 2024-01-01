#include "kNN.h"
#include <cmath>
#include <algorithm>
#include "KD_Tree.h"

// Constructor implementation
KNN::KNN(int neighbors, double threshold) : k(neighbors), split_threshold(threshold) {
    k = neighbors;
    split_threshold = threshold;
}

// Train function implementation
void KNN::train(Dataset& data) {
    // standardize the data
    for (int i = 0; i < data.points[0].features.size(); i++) {
        double sum = 0;
        double stddev = 0;
        for (const Point &point: data.points) {
            sum += point.features[i];
        }
        double mean = sum / data.points.size();
        double temp = 0;
        for (const Point &point: data.points) {
            temp += pow(point.features[i] - mean, 2);
        }
        stddev = sqrt(temp / data.points.size());
        for (Point &point: data.points) {
            point.features[i] = (point.features[i] - mean) / stddev;
        }
    }
    // build the KD_Tree
    KD_Tree kdTree;
    kdTree.build(data);

}
double KNN::calculateDistance(const Point& p1, const Point& p2) {
    double distance = 0.0;
    for (size_t i = 0; i < p1.features.size(); ++i) {
        distance += std::pow(p1.features[i] - p2.features[i], 2);
    }
    return std::sqrt(distance);
}
KDTreeNode* KNN::knnSearch(KDTreeNode* node, const Point& queryPoint) {
    if (!node) {
        return nullptr;
    }
    if (node->isLeaf()) {
        return node;
    }
    kd_tree_inter_node* internalNode = dynamic_cast<kd_tree_inter_node*>(node);
    if (queryPoint.features[internalNode->split_dimension] < internalNode->split_value) {
        return knnSearch(internalNode->left, queryPoint);
    } else {
        return knnSearch(internalNode->right, queryPoint);
    }
}
// Predict function implementation
int KNN::predict(const Point& queryPoint) {
    // returns if the given queryPoint is habitable (1) or not (0)
    std::vector<Point> neighbors;
    knnSearch(tree.getRoot(), queryPoint);

    // For simplicity, assume labels are binary (0 or 1)
    int countLabel0 = 0;
    for (const Point& neighbor : neighbors) {
        if (neighbor.label == "0") {
            countLabel0++;
        }
    }
    if (countLabel0 > k / 2){
        return 0;
    }else{
        return 1;
    }
    // Make a decision based on the majority label
}

// Additional methods or helper functions can be added as needed
