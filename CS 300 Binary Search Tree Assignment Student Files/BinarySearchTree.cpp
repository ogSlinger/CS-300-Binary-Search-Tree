//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Derek Spaulding
// Version     : 1.0
// Copyright   : Copyright © 2024 SNHU COCE
// Description : Binary Search Tree in C++
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold bid information
struct Course {
    string courseId; // unique identifier
    string courseTitle;
    vector<string> prereqs;
};

// Internal structure for tree node
struct Node {
    Course* course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Course* aCourse) :
        Node() {
        course = aCourse;
    }

    ~Node() {
        delete course;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course* course);
    void inOrder(Node* node);
    void recursiveDestructor(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void printNodeData(Node* node);
    void Insert(Course* course);
    Course* Search(string bidId);
    Node* findNode(Course* course);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    if (this->root != nullptr) {
        this->recursiveDestructor(this->root);
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    if (this->root != nullptr) {
        this->inOrder(this->root);
    }
}

void BinarySearchTree::printNodeData(Node* node) {
    if (node != nullptr) {
        std::cout << node->course->courseId << ", "
            << node->course->courseTitle;

        size_t size = node->course->prereqs.size();
        if (size != 0) {
            cout << endl << "\tPrerequisites: " << endl << "\t";

            for (size_t i = 0; i < size; i++) {
                cout << node->course->prereqs.at(i);
                if (i < size - 1) {
                    cout << ", ";
                }
            }
        }

        cout << endl;
    }
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course* course) {
    if (this->root == nullptr) {
        Node* newNode = new Node(course);
        this->root = newNode;
    }
    else {
        this->addNode(this->root, course);
    }
}

/**
 * Search for a bid
 */
Course* BinarySearchTree::Search(string courseId) {
    Node* currNode = this->root;

    while (currNode != nullptr) {
        if (currNode->course->courseId == courseId) {
            return currNode->course;
        }

        if (currNode->left != nullptr) {
            if (currNode->course->courseId > courseId) {
                currNode = currNode->left;
                continue;
            }
        }

        if (currNode->right != nullptr) {
            if (currNode->course->courseId < courseId) {
                currNode = currNode->right;
                continue;
            }
        }

        currNode = nullptr;
    }
    
    Course* emptyCourse = nullptr;
    return emptyCourse;
}

Node* BinarySearchTree::findNode(Course* course)
{
    Node* currNode = this->root;

    while (currNode != nullptr) {
        if (currNode->course->courseId == course->courseId) {
            return currNode;
        }

        if (currNode->left != nullptr) {
            if (currNode->course->courseId > course->courseId) {
                currNode = currNode->left;
                continue;
            }
        }

        if (currNode->right != nullptr) {
            if (currNode->course->courseId < course->courseId) {
                currNode = currNode->right;
                continue;
            }
        }

        currNode = nullptr;
    }

    currNode = nullptr;
    return currNode;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Course* course) {
    if (node->course->courseId > course->courseId) {
        if (node->left == nullptr) {
            Node* newNode = new Node(course);
            node->left = newNode;
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            Node* newNode = new Node(course);
            node->right = newNode;
        }
        else {
            this->addNode(node->right, course);
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {
      // FixMe (9): Pre order root
    if (node->left != nullptr) {
        this->inOrder(node->left);
    }

    this->printNodeData(node);

    if (node->right != nullptr) {
        this->inOrder(node->right);
    }
}

void BinarySearchTree::recursiveDestructor(Node* node) {
    if (node->left != nullptr) {
        this->recursiveDestructor(node->left);
    }
    else if (node->right != nullptr) {
        this->recursiveDestructor(node->right);
    }
    else {
        delete node;
        return;
    }
}

void displayCourse(Course* course) {
    std::cout << course->courseId << ", "
        << course->courseTitle;

    if (course->prereqs.size() != 0) {
        for (int i = 0; i < course->prereqs.size(); i++) {
            cout << ", " << course->prereqs.at(i);
        }
    }

    cout << endl;
}
//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading file " << csvPath << endl;
    ifstream file(csvPath);

    if (!file.is_open()) {
        cout << "Unable to open file" << endl;
        return;
    }

    else {
        string line;
        bool hasComma = false;
        string courseId = "";
        string courseTitle = "";
        string prereq = "";
        string* element = &courseId;
        char chr;
        Course* course = nullptr;
        size_t i = 0;

        while (std::getline(file, line)) {
            course = nullptr;
            courseId = "";
            courseTitle = "";
            prereq = "";
            hasComma = false;
            i = 0;
            element = &courseId;

            if (!line.empty() && line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') { // Was getting these characters at start of getline() calls
                line.erase(0, 3);
            }

            while (i < line.size()) {  //Loop to get Cours Id
                chr = line.at(i);

                while (element->empty() && (chr == ' ')) {
                    i++;
                    chr = line.at(i);
                }

                if (chr == '\n') {
                    i = 0;
                    break;
                }

                if (chr == ',') {
                    hasComma = true;
                    i++;
                    break;
                }

                else {
                    *element += chr;
                    i++;
                }
            }

            if ((hasComma == false) || (courseId.empty())) { //Reset if no course id or no comma found in line
                continue;
            }

            hasComma = false;

            element = &courseTitle;
            while (i < line.size()) { //Read in for course title
                chr = line.at(i);

                while (element->empty() && chr == ' ') {
                    i++;
                    chr = line.at(i);
                }

                if (chr == '\n') {
                    i = 0;
                    break;
                }

                if (chr == ',') {
                    hasComma = true;
                    i++;
                    break;
                }

                else {
                    *element += chr;
                    i++;
                }
            }

            if (courseTitle.empty()) {  //Reset if no course title
                continue;
            }

            course = new Course();
            course->courseId = courseId;
            course->courseTitle = courseTitle;

            if (hasComma) {
                while (i < line.size()) {     //Populate prereq vector
                    chr = line.at(i);

                    while ((prereq == "") && chr == ' ') {
                        i++;
                        chr = line.at(i);
                    }

                    if (chr == '\n') {
                        if (!prereq.empty()) {
                            course->prereqs.push_back(prereq);
                            prereq = "";
                        }
                        break;
                    }

                    if (chr == ',') {
                        i++;

                        if (!prereq.empty()) {
                            course->prereqs.push_back(prereq);
                            prereq = "";
                            continue;
                        }
                        else {
                            prereq = "";
                            continue;
                        }
                    }

                    else {
                        prereq += chr;
                        i++;
                    }

                    if (i == line.size() && !prereq.empty()) {
                        course->prereqs.push_back(prereq);
                        prereq = "";
                    }
                }
            }

            if (course != nullptr) {
                bst->Insert(course);
            }
        }

    }
    file.close();
}


/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseId;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseId = argv[2];
        break;
    default:
        csvPath = "ABCU_Advising_Program_Input.txt";
    }

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course* course = nullptr;

    cout << "Welcome to the course planner:" << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Complete the method call to load the bids
            loadCourses(csvPath, bst);
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            cout << "What course do you want to know about?" << endl;
            cin >> courseId;

            course = bst->Search(courseId);

            if (course != nullptr) {
                if (!(course->courseId.empty())) {
                    bst->printNodeData(bst->findNode(course));
                }
                else {
                    cout << "Course Id " << courseId << " not found." << endl;
                }
            }

            else {
                cout << "Course Id " << courseId << " not found." << endl;
            }

            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            return 0;

        default:
            cout << choice << " is not a valid option." << endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }


    }

	return 0;
}
