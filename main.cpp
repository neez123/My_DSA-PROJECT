#include <iostream>
#include <fstream>
#include <ctime>
#include "bst.h"
#include "avl.h"
#include "bfs.h"

using namespace std;

void LoadFlightsToBST(BST &bst) {
    ifstream file("flights.txt");
    if (!file) {
        cout << "No flights.txt found. Starting fresh." << endl;
        return;
    }

    string id;
    string dest, time, status;
    float duration;

    while (file >> id >> dest >> time >> duration >> status) {
        Flight f;
        f.setFlightID(id);
        f.setDestination(dest);
        f.setDepartureTime(time);
        f.setFlightDuration_hrs(duration);
        f.setFlightStatus(status);
        bst.InsertFlightDirect(f);
    }
    file.close();
    cout << "Flights loaded from file." << endl;
}

void LoadFlightsToAVL(AVL &avl) {
    avl.Reset();

    ifstream file("flights.txt");
    if (!file) {
        return;
    }

    string id;
    string dest, time, status;
    float duration;

    while (file >> id >> dest >> time >> duration >> status) {
        FlightAVL f;
        f.setFlightID(id);
        f.setDestination(dest);
        f.setDepartureTime(time);
        f.setFlightDuration_hrs(duration);
        f.setFlightStatus(status);
        avl.InsertFlightDirect(f);
    }
    file.close();
}

void SaveFlightsToFile(Node* node, ofstream &file) {
    if (node != NULL) {
        SaveFlightsToFile(node->getLeft(), file);
        Flight f = node->getData();
        file << f.getFlightID() << " "
             << f.getDestination() << " "
             << f.getDepartureTime() << " "
             << f.getFlightDuration_hrs() << " "
             << f.getFlightStatus() << endl;
        SaveFlightsToFile(node->getRight(), file);
    }
}

void SaveAllFlights(BST &bst) {
    ofstream file("flights.txt");
    SaveFlightsToFile(bst.getRoot(), file);
    file.close();
}

Graph* LoadGraph() {
    ifstream airportFile("airports.txt");
    if (!airportFile) {
        cout << "Creating sample airports and routes..." << endl;

        ofstream out("airports.txt");
        out << "5" << endl;
        out << "Karachi" << endl;
        out << "Lahore" << endl;
        out << "Islamabad" << endl;
        out << "Peshawar" << endl;
        out << "Skardu" << endl;
        out.close();

        ofstream routeOut("routes.txt");
        routeOut << "0 1" << endl;
        routeOut << "1 2" << endl;
        routeOut << "2 3" << endl;
        routeOut << "1 4" << endl;
        routeOut.close();

        airportFile.open("airports.txt");
    }

    int numAirports;
    airportFile >> numAirports;

    Graph* g = new Graph(numAirports);

    string name;
    getline(airportFile, name);
    for (int i = 0; i < numAirports; i++) {
        getline(airportFile, name);
        g->SetAirportName(i, name);
    }
    airportFile.close();

    ifstream routeFile("routes.txt");
    if (routeFile) {
        int from, to;
        while (routeFile >> from >> to) {
            g->AddRoute(from, to);
        }
        routeFile.close();
    }

    return g;
}

void CompareSearchTime(BST &bst, AVL &avl) {
    string id;
    cout << "Enter Flight ID to search (e.g. PK101): ";
    cin >> id;

    clock_t start, end;

    start = clock();
    for (int i = 0; i < 10000; i++) {
        bst.SearchFlightQuiet(id);
    }
    end = clock();
    double bstTime = double(end - start) / CLOCKS_PER_SEC * 1000;

    start = clock();
    for (int i = 0; i < 10000; i++) {
        avl.SearchFlightQuiet(id);
    }
    end = clock();
    double avlTime = double(end - start) / CLOCKS_PER_SEC * 1000;

    cout << "\n--- Search Time Comparison (10000 searches) ---" << endl;
    cout << "BST Time: " << bstTime << " ms" << endl;
    cout << "AVL Time: " << avlTime << " ms" << endl;

    if (bstTime < avlTime)
        cout << "BST was faster!" << endl;
    else if (avlTime < bstTime)
        cout << "AVL was faster!" << endl;
    else
        cout << "Both took same time!" << endl;
}

int main() {
    BST bst;
    AVL avl;

    LoadFlightsToBST(bst);
    LoadFlightsToAVL(avl);

    Graph* graph = LoadGraph();

    int choice;

    do {
        cout << "\n==============================" << endl;
        cout << "   Flight Management System" << endl;
        cout << "==============================" << endl;
        cout << "1. Add New Flight (BST)" << endl;
        cout << "2. Search Flight (BST)" << endl;
        cout << "3. Delete Flight (BST)" << endl;
        cout << "4. Display All Flights (BST)" << endl;
        cout << "5. Display Flights by Destination" << endl;
        cout << "6. Display Flights by Status" << endl;
        cout << "7. Display ETA for All Flights" << endl;
        cout << "8. Surprise Feature (Count Delayed Flights)" << endl;
        cout << "9. Display Flights (AVL) + Rotation Info" << endl;
        cout << "10. Compare Search Time (BST vs AVL)" << endl;
        cout << "11. Plan Trip from Airport A to B" << endl;
        cout << "12. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bst.InsertFlight();
                SaveAllFlights(bst);
                LoadFlightsToAVL(avl);
                break;

            case 2:
                bst.SearchFlight();
                break;

            case 3:
                bst.DeleteFlight();
                SaveAllFlights(bst);
                LoadFlightsToAVL(avl);
                break;

            case 4:
                bst.DisplayAll();
                break;

            case 5:
                bst.DisplayByDestination();
                break;

            case 6:
                bst.DisplayByStatus();
                break;

            case 7:
                bst.DisplayETA();
                break;

            case 8:
                bst.SurpriseFeature();
                break;

            case 9:
                avl.DisplayAll();
                break;

            case 10:
                CompareSearchTime(bst, avl);
                break;

            case 11: {
                graph->DisplayAirports();
                int start, end;
                cout << "Enter source airport number: ";
                cin >> start;
                cout << "Enter destination airport number: ";
                cin >> end;
                graph->BFS(start);
                graph->MinimumStops(start, end);
                break;
            }

            case 12:
                cout << "Saving data and exiting..." << endl;
                SaveAllFlights(bst);
                delete graph;
                break;

            default:
                cout << "Invalid choice!" << endl;
        }

    } while (choice != 12);

    return 0;
}
