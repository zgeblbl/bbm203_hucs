#include <algorithm>
#include "KD_Tree.h"

// Default constructor implementation
KD_Tree::KD_Tree() : root(nullptr), split_threshold(0.1) {
}

// Parameterized constructor implementation
KD_Tree::KD_Tree(double threshold) : root(nullptr), split_threshold(threshold) {
}

// Destructor implementation
KD_Tree::~KD_Tree() {
    // Implementation for safely deleting the KD_Tree and its nodes
}
KDTreeNode* KD_Tree::buildTreeRecursive(const std::vector<Point>& points, int depth) {
    if (points.empty()) {
        return nullptr;
    }
    Point *point = new Point();

    int dimension = depth % point->getDimensionsCount();

    // Sort the points based on the chosen dimension
    std::vector<Point> sortedPoints(points);
    std::sort(sortedPoints.begin(), sortedPoints.end(), [dimension](const Point& a, const Point& b) {
        return a.getDimensionValue(dimension) < b.getDimensionValue(dimension);
    });

    // Calculate the split value as the median
    int medianIndex = sortedPoints.size() / 2;
    double splitValue = sortedPoints[medianIndex].getDimensionValue(dimension);

    // Create an internal node to store the split dimension and value
    kd_tree_inter_node* internalNode = new kd_tree_inter_node(dimension, splitValue);

    // Split the data into two groups
    std::vector<Point> leftGroup(sortedPoints.begin(), sortedPoints.begin() + medianIndex);
    std::vector<Point> rightGroup(sortedPoints.begin() + medianIndex, sortedPoints.end());

    // Recursively build the left and right subtrees
    internalNode->left=(buildTreeRecursive(leftGroup, depth + 1));
    internalNode->right=(buildTreeRecursive(rightGroup, depth + 1));

    return internalNode;
}
void KD_Tree::build(Dataset& data) {
    // Implementation for building the KD_Tree using the provided dataset
    root = buildTreeRecursive(data.points, 0);
}

KDTreeNode* KD_Tree::getRoot() {
    // Implementation to return the root of the KD_Tree
    return  root;
}

// Add any additional function as you wish
