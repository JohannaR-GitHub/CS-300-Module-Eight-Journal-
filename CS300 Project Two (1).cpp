#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Define a Course structure
struct Course {
    string courseId; // Course number (e.g., CSCI400)
    string courseName; // Course title (e.g., Large Software Development)
    vector<string> prerequisites; // List of prerequisite course numbers
};

// Define a Node for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Define the BST class
class CourseBST {
private:
    Node* root;

    void addNode(Node* node, Course course) {
        if (course.courseId < node->course.courseId) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseId << ", " << node->course.courseName << endl;
            inOrder(node->right);
        }
    }

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    CourseBST() {
        root = nullptr;
    }

    ~CourseBST() {
        destroyTree(root);
    }

    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    void PrintCourseList() {
        inOrder(root);
    }

    Course* Search(string courseId) {
        Node* current = root;
        while (current != nullptr) {
            if (current->course.courseId == courseId) {
                return &current->course;
            } else if (courseId < current->course.courseId) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return nullptr;
    }
};

// Convert a string to uppercase
string toUpper(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Function to load courses from file
void loadCourses(string fileName, CourseBST& bst) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        Course course;
        getline(ss, course.courseId, ',');
        getline(ss, course.courseName, ',');

        string prereq;
        while (getline(ss, prereq, ',')) {
            course.prerequisites.push_back(prereq);
        }

        bst.Insert(course);
    }

    file.close();
}

int main() {
    CourseBST bst;
    string userChoice;

    cout << "Welcome to the course planner.\n" << endl;

    while (true) {
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit.\n" << endl;

        cout << "What would you like to do? ";
        getline(cin, userChoice);

        if (userChoice == "1") {
            cout << "Enter the file name to load: ";
            string fileName;
            getline(cin, fileName);
            loadCourses(fileName, bst);
            cout << "Courses loaded successfully.\n" << endl;
        } else if (userChoice == "2") {
            cout << "\nHere is a sample schedule:\n" << endl;
            bst.PrintCourseList();
            cout << endl;
        } else if (userChoice == "3") {
            cout << "\nWhat course do you want to know about? ";
            string courseId;
            getline(cin, courseId);
            courseId = toUpper(courseId);

            Course* course = bst.Search(courseId);
            if (course != nullptr) {
                cout << endl;
                cout << course->courseId << ", " << course->courseName << endl;
                if (!course->prerequisites.empty()) {
                    cout << "Prerequisites: ";
                    for (size_t i = 0; i < course->prerequisites.size(); ++i) {
                        cout << course->prerequisites[i];
                        if (i < course->prerequisites.size() - 1) {
                            cout << ", ";
                        }
                    }
                    cout << endl;
                } else {
                    cout << "Prerequisites: None" << endl;
                }
                cout << endl;
            } else {
                cout << endl;
                cout << "Course not found.\n" << endl;
            }
        } else if (userChoice == "9") {
            cout << "\nThank you for using the course planner!\n" << endl;
            break;
        } else {
            cout << endl;
            cout << userChoice << " is not a valid option.\n" << endl;
        }
    }

    return 0;
}
