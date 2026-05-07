#ifndef BST_H
#define BST_H

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

enum RelationType { LESS, GREATER, EQUAL };

class ItemType {
private:
    string value;

public:
    ItemType() {
        value = "";
    }

    void setValue(string v) {
        value = v;
    }

    string getValue() {
        return value;
    }

    RelationType ComparedTo(ItemType other) {
        if (value < other.getValue())
            return LESS;
        else if (value > other.getValue())
            return GREATER;
        else
            return EQUAL;
    }
};

class Flight {
private:
    string FlightID;
    string Destination;
    string DepartureTime;
    float FlightDuration_hrs;
    string FlightStatus;

public:
    Flight() {
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

    void DisplayWithETA() {
        string arrivalTime = "Invalid";

        if (DepartureTime.length() >= 5) {
            int hour = (DepartureTime[0] - '0') * 10 + (DepartureTime[1] - '0');
            int min = (DepartureTime[3] - '0') * 10 + (DepartureTime[4] - '0');

            int totalMinutes = hour * 60 + min + (FlightDuration_hrs * 60);
            int arrivalHour = (totalMinutes / 60) % 24;
            int arrivalMin = totalMinutes % 60;

            stringstream ss;
            if (arrivalHour < 10) ss << "0";
            ss << arrivalHour << ":";
            if (arrivalMin < 10) ss << "0";
            ss << arrivalMin;
            arrivalTime = ss.str();
        }

        string flightCondition;
        if (FlightStatus == "Delayed" || FlightStatus == "delayed")
            flightCondition = "DELAYED";
        else if (FlightStatus == "Departed" || FlightStatus == "departed")
            flightCondition = "ON TIME";
        else if (FlightStatus == "Landed" || FlightStatus == "landed")
            flightCondition = "ARRIVED";
        else
            flightCondition = "UNKNOWN";

        cout << "\n----- ETA DETAILS -----";
        cout << "\nFlight ID: " << FlightID;
        cout << "\nDestination: " << Destination;
        cout << "\nDeparture Time: " << DepartureTime;
        cout << "\nFlight Duration: " << FlightDuration_hrs << " hrs";
        cout << "\nExpected Arrival Time: " << arrivalTime;
        cout << "\nCurrent Status: " << flightCondition << endl;
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

class Node {
private:
    Flight data;
    ItemType key;
    Node* left;
    Node* right;

public:
    Node() {
        left = NULL;
        right = NULL;
    }

    Node(Flight f) {
        data = f;
        key.setValue(f.getFlightID());
        left = NULL;
        right = NULL;
    }

    void setData(Flight f) {
        data = f;
        key.setValue(f.getFlightID());
    }
    void setLeft(Node* l) { left = l; }
    void setRight(Node* r) { right = r; }

    Flight getData() { return data; }
    ItemType getKey() { return key; }
    Node* getLeft() { return left; }
    Node* getRight() { return right; }
};

class BST {
private:
    Node* root;

    Node* Insert(Node* node, Flight f) {
        if (node == NULL) {
            return new Node(f);
        }

        ItemType newKey;
        newKey.setValue(f.getFlightID());

        if (newKey.ComparedTo(node->getKey()) == LESS) {
            node->setLeft(Insert(node->getLeft(), f));
        }
        else if (newKey.ComparedTo(node->getKey()) == GREATER) {
            node->setRight(Insert(node->getRight(), f));
        }

        return node;
    }

    Node* FindMin(Node* node) {
        while (node != NULL && node->getLeft() != NULL) {
            node = node->getLeft();
        }
        return node;
    }

    Node* Delete(Node* node, string id) {
        if (node == NULL) {
            return NULL;
        }

        ItemType searchKey;
        searchKey.setValue(id);

        if (searchKey.ComparedTo(node->getKey()) == LESS) {
            node->setLeft(Delete(node->getLeft(), id));
        }
        else if (searchKey.ComparedTo(node->getKey()) == GREATER) {
            node->setRight(Delete(node->getRight(), id));
        }
        else {
            if (node->getLeft() == NULL) {
                Node* temp = node->getRight();
                delete node;
                return temp;
            }
            else if (node->getRight() == NULL) {
                Node* temp = node->getLeft();
                delete node;
                return temp;
            }
            else {
                Node* temp = FindMin(node->getRight());
                node->setData(temp->getData());
                node->setRight(Delete(node->getRight(), temp->getData().getFlightID()));
            }
        }
        return node;
    }

    Node* Search(Node* node, string id) {
        if (node == NULL) {
            return NULL;
        }

        ItemType searchKey;
        searchKey.setValue(id);

        if (searchKey.ComparedTo(node->getKey()) == EQUAL) {
            return node;
        }
        else if (searchKey.ComparedTo(node->getKey()) == LESS) {
            return Search(node->getLeft(), id);
        }
        else {
            return Search(node->getRight(), id);
        }
    }

    void Inorder(Node* node) {
        if (node != NULL) {
            Inorder(node->getLeft());
            node->getData().DisplayFlight();
            Inorder(node->getRight());
        }
    }

    void InorderByDestination(Node* node, string dest) {
        if (node != NULL) {
            InorderByDestination(node->getLeft(), dest);
            if (node->getData().getDestination() == dest) {
                node->getData().DisplayFlight();
            }
            InorderByDestination(node->getRight(), dest);
        }
    }

    void InorderByStatus(Node* node, string status) {
        if (node != NULL) {
            InorderByStatus(node->getLeft(), status);
            if (node->getData().getFlightStatus() == status) {
                node->getData().DisplayFlight();
            }
            InorderByStatus(node->getRight(), status);
        }
    }

    void InorderETA(Node* node) {
        if (node != NULL) {
            InorderETA(node->getLeft());
            node->getData().DisplayWithETA();
            InorderETA(node->getRight());
        }
    }

    int CountDelayed(Node* node) {
        if (node == NULL) {
            return 0;
        }

        int count = 0;
        if (node->getData().getFlightStatus() == "Delayed" ||
            node->getData().getFlightStatus() == "delayed") {
            count = 1;
        }

        return count + CountDelayed(node->getLeft()) + CountDelayed(node->getRight());
    }

public:
    BST() {
        root = NULL;
    }

    Node* getRoot() {
        return root;
    }

    void InsertFlight() {
        Flight f;
        f.FlightInput();

        if (Search(root, f.getFlightID()) != NULL) {
            cout << "Flight with this ID already exists!" << endl;
            return;
        }

        root = Insert(root, f);
        cout << "Flight inserted successfully!" << endl;
    }

    void InsertFlightDirect(Flight f) {
        root = Insert(root, f);
    }

    void DeleteFlight() {
        string id;
        cout << "Enter Flight ID to delete (e.g. PK101): ";
        cin >> id;

        if (Search(root, id) == NULL) {
            cout << "Flight not found!" << endl;
            return;
        }

        root = Delete(root, id);
        cout << "Flight deleted successfully!" << endl;
    }

    void SearchFlight() {
        string id;
        cout << "Enter Flight ID to search (e.g. PK101): ";
        cin >> id;

        Node* result = Search(root, id);
        if (result != NULL) {
            cout << "\n--- Flight Found ---" << endl;
            result->getData().DisplayFlight();
        }
        else {
            cout << "Flight not found!" << endl;
        }
    }

    bool SearchFlightQuiet(string id) {
        return Search(root, id) != NULL;
    }

    void DisplayAll() {
        if (root == NULL) {
            cout << "No flights in database!" << endl;
            return;
        }
        cout << "\n--- All Flights (Sorted by ID) ---" << endl;
        Inorder(root);
    }

    void DisplayByDestination() {
        if (root == NULL) {
            cout << "No flights in database!" << endl;
            return;
        }
        string dest;
        cout << "Enter Destination: ";
        cin >> dest;
        cout << "\n--- Flights to " << dest << " ---" << endl;
        InorderByDestination(root, dest);
    }

    void DisplayByStatus() {
        if (root == NULL) {
            cout << "No flights in database!" << endl;
            return;
        }
        string status;
        cout << "Enter Status (On-time/Delayed/Cancelled): ";
        cin >> status;
        cout << "\n--- Flights with status: " << status << " ---" << endl;
        InorderByStatus(root, status);
    }

    void DisplayETA() {
        if (root == NULL) {
            cout << "No flights in database!" << endl;
            return;
        }
        cout << "\n--- ETA for All Flights ---" << endl;
        InorderETA(root);
    }

    void SurpriseFeature() {
        int count = CountDelayed(root);
        cout << "\n--- Surprise Feature ---" << endl;
        cout << "Total Delayed Flights: " << count << endl;
    }
};

#endif
