#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <string>
using namespace std;

class FlightAVL {
private:
    string FlightID;
    string Destination;
    string DepartureTime;
    float FlightDuration_hrs;
    string FlightStatus;

public:
    FlightAVL() {
        FlightID = "";
        Destination = "";
        DepartureTime = "";
        FlightDuration_hrs = 0.0;
        FlightStatus = "";
    }

    void FlightInput() {
        cout << "Enter Flight ID (e.g. PK101): ";
        cin >> FlightID;
        cout << "Enter Destination: ";
        cin >> Destination;
        cout << "Enter Departure Time (HH:MM): ";
        cin >> DepartureTime;
        cout << "Enter Flight Duration (hours): ";
        cin >> FlightDuration_hrs;
        cout << "Enter Status (On-time/Delayed/Cancelled): ";
        cin >> FlightStatus;
    }

    void DisplayFlight() {
        cout << "----------------------------" << endl;
        cout << "Flight ID: " << FlightID << endl;
        cout << "Destination: " << Destination << endl;
        cout << "Departure: " << DepartureTime << endl;
        cout << "Duration: " << FlightDuration_hrs << " hrs" << endl;
        cout << "Status: " << FlightStatus << endl;
    }

    string getFlightID() { return FlightID; }
    string getDestination() { return Destination; }
    string getDepartureTime() { return DepartureTime; }
    float getFlightDuration_hrs() { return FlightDuration_hrs; }
    string getFlightStatus() { return FlightStatus; }

    void setFlightID(string id) { FlightID = id; }
    void setDestination(string dest) { Destination = dest; }
    void setDepartureTime(string time) { DepartureTime = time; }
    void setFlightDuration_hrs(float hrs) { FlightDuration_hrs = hrs; }
    void setFlightStatus(string status) { FlightStatus = status; }
};

class AVLNode {
private:
    FlightAVL data; 
    AVLNode* left;
    AVLNode* right;
    int height;

public:
    AVLNode() {
        left = NULL;
        right = NULL;
        height = 1;
    }

    AVLNode(FlightAVL f) {
        data = f;
        left = NULL;
        right = NULL;
        height = 1;
    }

    void setData(FlightAVL f) { data = f; }
    void setLeft(AVLNode* l) { left = l; }
    void setRight(AVLNode* r) { right = r; }
    void setHeight(int h) { height = h; }

    FlightAVL getData() { return data; }
    AVLNode* getLeft() { return left; }
    AVLNode* getRight() { return right; }
    int getHeight() { return height; }
};

class AVL {
private:
    AVLNode* root;
    int rotationCount;

    int GetHeight(AVLNode* node) {
        if (node == NULL)
            return 0;
        return node->getHeight();
    }

    int GetBalance(AVLNode* node) {
        if (node == NULL)
            return 0;
        return GetHeight(node->getLeft()) - GetHeight(node->getRight());
    }

    int Max(int a, int b) {
        if (a > b)
            return a;
        return b;
    }

    AVLNode* RightRotate(AVLNode* y) {
        AVLNode* x = y->getLeft();
        AVLNode* T2 = x->getRight();

        x->setRight(y);
        y->setLeft(T2);

        y->setHeight(Max(GetHeight(y->getLeft()), GetHeight(y->getRight())) + 1);
        x->setHeight(Max(GetHeight(x->getLeft()), GetHeight(x->getRight())) + 1);

        rotationCount++;
        return x;
    }

    AVLNode* LeftRotate(AVLNode* x) {
        AVLNode* y = x->getRight();
        AVLNode* T2 = y->getLeft();

        y->setLeft(x);
        x->setRight(T2);

        x->setHeight(Max(GetHeight(x->getLeft()), GetHeight(x->getRight())) + 1);
        y->setHeight(Max(GetHeight(y->getLeft()), GetHeight(y->getRight())) + 1);

        rotationCount++;
        return y;
    }

    AVLNode* Insert(AVLNode* node, FlightAVL f) {
        if (node == NULL) {
            return new AVLNode(f);
        }

        if (f.getFlightID() < node->getData().getFlightID()) {
            node->setLeft(Insert(node->getLeft(), f));
        }
        else if (f.getFlightID() > node->getData().getFlightID()) {
            node->setRight(Insert(node->getRight(), f));
        }
        else {
            return node;
        }

        node->setHeight(1 + Max(GetHeight(node->getLeft()), GetHeight(node->getRight())));

        int balance = GetBalance(node);

        if (balance > 1 && f.getFlightID() < node->getLeft()->getData().getFlightID()) {
            return RightRotate(node);
        }

        if (balance < -1 && f.getFlightID() > node->getRight()->getData().getFlightID()) {
            return LeftRotate(node);
        }

        if (balance > 1 && f.getFlightID() > node->getLeft()->getData().getFlightID()) {
            node->setLeft(LeftRotate(node->getLeft()));
            return RightRotate(node);
        }

        if (balance < -1 && f.getFlightID() < node->getRight()->getData().getFlightID()) {
            node->setRight(RightRotate(node->getRight()));
            return LeftRotate(node);
        }

        return node;
    }

    AVLNode* Search(AVLNode* node, string id) {
        if (node == NULL) {
            return NULL;
        }

        if (id == node->getData().getFlightID()) {
            return node;
        }
        else if (id < node->getData().getFlightID()) {
            return Search(node->getLeft(), id);
        }
        else {
            return Search(node->getRight(), id);
        }
    }

    void Inorder(AVLNode* node) {
        if (node != NULL) {
            Inorder(node->getLeft());
            node->getData().DisplayFlight();
            Inorder(node->getRight());
        }
    }

public:
    AVL() {
        root = NULL;
        rotationCount = 0;
    }

    void Clear(AVLNode* node) {
        if (node != NULL) {
            Clear(node->getLeft());
            Clear(node->getRight());
            delete node;
        }
    }

    void Reset() {
        Clear(root);
        root = NULL;
        rotationCount = 0;
    }

    void InsertFlight() {
        FlightAVL f;
        f.FlightInput();
        root = Insert(root, f);
        cout << "Flight inserted in AVL!" << endl;
    }

    void InsertFlightDirect(FlightAVL f) {
        root = Insert(root, f);
    }

    bool SearchFlightQuiet(string id) {
        return Search(root, id) != NULL;
    }

    void DisplayAll() {
        if (root == NULL) {
            cout << "No flights in AVL tree!" << endl;
            return;
        }
        cout << "\n--- AVL Tree Flights (Sorted by ID) ---" << endl;
        Inorder(root);
        cout << "\nTotal Rotations: " << rotationCount << endl;
    }

    int getRotationCount() {
        return rotationCount;
    }
};

#endif
