#ifndef BFS_H
#define BFS_H

#include <iostream>
#include <string>
using namespace std;

class Queue {
private:
    int arr[100];
    int front;
    int rear;

public:
    Queue() {
        front = -1;
        rear = -1;
    }

    bool isEmpty() {
        return front == -1;
    }

    void enqueue(int x) {
        if (front == -1) {
            front = 0;
        }
        rear++;
        arr[rear] = x;
    }

    int dequeue() {
        int item = arr[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        }
        else {
            front++;
        }
        return item;
    }
};

class Graph {
private:
    int numAirports;
    int adjMatrix[20][20];
    string airportNames[20];

public:
    Graph(int n) {
        numAirports = n;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                adjMatrix[i][j] = 0;
            }
        }
    }

    void SetAirportName(int index, string name) {
        if (index >= 0 && index < numAirports) {
            airportNames[index] = name;
        }
    }

    void AddRoute(int from, int to) {
        if (from >= 0 && from < numAirports && to >= 0 && to < numAirports) {
            adjMatrix[from][to] = 1;
            adjMatrix[to][from] = 1;
        }
    }

    void DisplayAirports() {
        cout << "\n--- Available Airports ---" << endl;
        for (int i = 0; i < numAirports; i++) {
            cout << i << ". " << airportNames[i] << endl;
        }
    }

    void BFS(int start) {
        if (start < 0 || start >= numAirports) {
            cout << "Invalid airport!" << endl;
            return;
        }

        int visited[20];
        int distance[20];
        int parent[20];

        for (int i = 0; i < numAirports; i++) {
            visited[i] = 0;
            distance[i] = -1;
            parent[i] = -1;
        }

        Queue q;
        visited[start] = 1;
        distance[start] = 0;
        q.enqueue(start);

        while (!q.isEmpty()) {
            int current = q.dequeue();

            for (int i = 0; i < numAirports; i++) {
                if (adjMatrix[current][i] == 1 && visited[i] == 0) {
                    visited[i] = 1;
                    distance[i] = distance[current] + 1;
                    parent[i] = current;
                    q.enqueue(i);
                }
            }
        }

        cout << "\n--- BFS Traversal ---" << endl;
        cout << "Airport\t\tDistance\tParent" << endl;
        cout << "----------------------------------------" << endl;
        for (int i = 0; i < numAirports; i++) {
            cout << airportNames[i] << "\t\t" << distance[i] << "\t\t";
            if (parent[i] == -1)
                cout << "None";
            else
                cout << airportNames[parent[i]];
            cout << endl;
        }
    }

    void MinimumStops(int start, int end) {
        if (start < 0 || start >= numAirports || end < 0 || end >= numAirports) {
            cout << "Invalid airports!" << endl;
            return;
        }

        int visited[20];
        int distance[20];
        int parent[20];

        for (int i = 0; i < numAirports; i++) {
            visited[i] = 0;
            distance[i] = -1;
            parent[i] = -1;
        }

        Queue q;
        visited[start] = 1;
        distance[start] = 0;
        q.enqueue(start);

        while (!q.isEmpty()) {
            int current = q.dequeue();

            for (int i = 0; i < numAirports; i++) {
                if (adjMatrix[current][i] == 1 && visited[i] == 0) {
                    visited[i] = 1;
                    distance[i] = distance[current] + 1;
                    parent[i] = current;
                    q.enqueue(i);
                }
            }
        }

        if (distance[end] == -1) {
            cout << "No route available from " << airportNames[start];
            cout << " to " << airportNames[end] << endl;
            return;
        }

        cout << "\nMinimum stops from " << airportNames[start];
        cout << " to " << airportNames[end] << ": " << distance[end] << endl;

        int path[20];
        int pathLength = 0;
        int current = end;

        while (current != -1) {
            path[pathLength] = current;
            pathLength++;
            current = parent[current];
        }

        cout << "Route: ";
        for (int i = pathLength - 1; i >= 0; i--) {
            cout << airportNames[path[i]];
            if (i != 0)
                cout << " -> ";
        }
        cout << endl;
    }
};

#endif
