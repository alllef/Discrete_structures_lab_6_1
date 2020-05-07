#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include<Windows.h>

using namespace std;

struct Rib {
public:
    int start;
    int end;
};

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs);

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs);

bool hasEilerCycle(int &picks, int &ribs, vector<Rib> &structRibs);

vector<int> findEilerPath(int &picks, int &ribs, vector<Rib> structRibs, int startPick);

bool hasEilerPath(int &picks, int &ribs, vector<Rib> &structRibs);

vector<int> findEilerCycle(int &picks, int &ribs, vector<Rib> &structRibs);

bool hasHamiltonianCycle(int &picks, int &ribs, vector<Rib> &structRibs);

vector<int> findHamiltonPath(int &picks, int &ribs, vector<Rib> &structRibs);

vector<int> findHamiltonCycle(int &picks, int &ribs, vector<Rib> &structRibs);

void printHamiltonPath(vector<int> path, int picks);

void printEilerPath(vector<int> path, int ribs);

void makeMenu(int &picks, int &ribs, vector<Rib> &structRibs);

int main() {
    SetConsoleOutputCP(CP_UTF8);
    vector<Rib> ribsList;
    int n = 0, m = 0;
    initializeGraph(n, m, ribsList);
    sortRibs(n, m, ribsList);
    makeMenu(n, m, ribsList);
    return 0;
}

void initializeGraph(int &picks, int &ribs, vector<Rib> &structRibs) {
    Rib myRib{};
    ifstream inFile;
    inFile.open("myGraph.txt");

    if (!inFile.is_open()) cout << "It is not open" << endl;
    inFile >> picks >> ribs;

    for (int i = 0; i < ribs; i++) {
        inFile >> myRib.start >> myRib.end;

        structRibs.push_back(myRib);
    }
    inFile.close();
}

void sortRibs(int &picks, int &ribs, vector<Rib> &structRibs) {

    Rib tmp{};
    for (int i = 0; i < ribs - 1; i++) {
        for (int j = 0; j < ribs - 1; j++) {
            if ((structRibs[j].start + structRibs[j].end) > (structRibs[j + 1].start + structRibs[j + 1].end)) {

                tmp = structRibs[j];
                structRibs[j] = structRibs[j + 1];
                structRibs[j + 1] = tmp;

            }
        }
    }
}

bool hasEilerCycle(int &picks, int &ribs, vector<Rib> &structRibs) {
    int incidenceCounter = 0;
    for (int i = 1; i <= picks; i++) {
        for (auto &structRib : structRibs) {
            if (structRib.start == i || structRib.end == i) incidenceCounter++;
        }
        if (incidenceCounter % 2 != 0) return false;
    }

    return true;
}

bool hasEilerPath(int &picks, int &ribs, vector<Rib> &structRibs) {
    vector<int> tmpRib;
    int incidenceCounter = 0;
    int unDoubleDegree = 0;
    for (int i = 1; i <= picks; i++) {
        for (int j=0; i<structRibs.size();i++) {
            if (structRibs[j].start == i || structRibs[j].end == i) incidenceCounter++;
        }
        if (incidenceCounter % 2 == 1) {
            unDoubleDegree++;
            if (unDoubleDegree > 2) return false;
            tmpRib.push_back(i);
        }
        incidenceCounter = 0;
    }
    Rib myRib{};
    myRib.start = tmpRib[0];
    myRib.end = tmpRib[1];
    structRibs.push_back(myRib);
    return true;
}

vector<int> findEilerPath(int &picks, int &ribs, vector<Rib> structRibs, int startPick) {

    stack<int> myStack;
    vector<int> path;
    vector<Rib> tmpRibs = structRibs;

    myStack.push(startPick);


    while (!myStack.empty()) {
        int currPick = myStack.top();
        int currSize = tmpRibs.size();
        for (int i = 0; i < currSize; i++) {

            if (tmpRibs[i].start == myStack.top()) {
                myStack.push(tmpRibs[i].end);
                tmpRibs.erase(tmpRibs.begin() + i);

                break;
            }

            if (tmpRibs[i].end == myStack.top()) {
                myStack.push(tmpRibs[i].start);
                tmpRibs.erase(tmpRibs.begin() + i);


                break;
            }

        }
        if (currPick == myStack.top()) {
            path.push_back(myStack.top());
            myStack.pop();

        }
    }
    return path;
}

vector<int> findEilerCycle(int &picks, int &ribs, vector<Rib> &structRibs) {
    if (hasEilerCycle(picks, ribs, structRibs))return findEilerPath(picks, ribs, structRibs, rand() % picks + 1);
    if (hasEilerPath(picks, ribs, structRibs)) {
        vector<int> tmp = findEilerPath(picks, ribs, structRibs, structRibs.back().start);
        tmp.erase(tmp.end() - 1);
        structRibs.erase(structRibs.end() - 1);
        return tmp;
    }
    return {};
}

bool hasHamiltonianCycle(int &picks, int &ribs, vector<Rib> &structRibs) {
    if (picks < 3)return false;
    int incidenceCounter = 0;
    for (int i = 1; i <= picks; i++) {
        for (auto &structRib : structRibs) {
            if (structRib.start == i || structRib.end == i) incidenceCounter++;
        }
        if (incidenceCounter < picks / 2) return false;
        incidenceCounter = 0;
    }
    return true;
}

vector<int> findHamiltonPath(int &picks, int &ribs, vector<Rib> &structRibs) {
    stack<int> myStack;
    int startPick = rand() % picks + 1;
    vector<bool> isVisited(5);
    int deadEndPick = picks + 1;
    vector<int> path;

    myStack.push(startPick);
    isVisited[startPick - 1] = true;
    path.push_back(startPick);
    while (path.size() < picks + 1) {
        int prev = myStack.top();

        for (int i = 0; i < structRibs.size(); i++) {
            if (myStack.top() == structRibs[i].start && structRibs[i].end != deadEndPick &&
                isVisited[structRibs[i].end - 1] == false) {
                myStack.push(structRibs[i].end);
                isVisited[structRibs[i].end - 1] = true;
                path.push_back(structRibs[i].end);
                break;
            }

            if (myStack.top() == structRibs[i].end && structRibs[i].start != prev &&
                isVisited[structRibs[i].start - 1] == false) {
                myStack.push(structRibs[i].start);
                isVisited[structRibs[i].start - 1] = true;
                path.push_back(structRibs[i].start);
                break;
            }

        }

        deadEndPick = picks + 1;

        if (myStack.size() == picks) isVisited[startPick - 1] = false;

        if (myStack.top() == prev) {
            deadEndPick = myStack.top();
            isVisited[deadEndPick - 1] = false;
            myStack.pop();
        }
    }
    return path;
}

vector<int> findHamiltonCycle(int &picks, int &ribs, vector<Rib> &structRibs) {
    if (hasHamiltonianCycle(picks, ribs, structRibs)) return findHamiltonPath(picks, ribs, structRibs);
    else {
        Rib tmpRib{};
        vector<Rib> tmpRibs = structRibs;
        tmpRib.start = picks + 1;
        for (int i = 1; i <= picks; i++) {
            tmpRib.end = i;
            tmpRibs.push_back(tmpRib);
        }
        int fictPicks = picks + 1;
        if (hasHamiltonianCycle(fictPicks, ribs, tmpRibs)) {
            vector<int> path = findHamiltonPath(fictPicks, ribs, tmpRibs);
            path.erase(path.end() - 1);
            return path;
        } else return {};
    }
}

void printHamiltonPath(vector<int> path, int picks) {

    if (path.empty())cout << "немає гамільтонова цикла та маршруту";
    else {
        if (path.size() == picks)cout << "Є гамільтонів маршрут\n";
        if (path.size() == picks + 1)cout << "Є гамільтонів цикл\n";

        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << " ";
        }
    }
}

void printEilerPath(vector<int> path, int ribs) {

    if (path.empty())cout << "немає ейлерового цикла та маршруту";
    else {
        if (path.size() == ribs + 1)cout << " Є ейлерів маршрут\n";
        if (path.size() == ribs + 2)cout << "Є ейлерів цикл\n";
        for (int i : path) {
            cout << i << " ";
        }
    }
}

void makeMenu(int &picks, int &ribs, vector<Rib> &structRibs) {
    bool isEiler;
    cout << "Введіть 0 для знаходження гамільтонового циклу та 1 для знаходження ейлерового циклу";
    cin >> isEiler;
    if (isEiler) {
        printEilerPath(findEilerCycle(picks, ribs, structRibs), ribs);
    } else printHamiltonPath(findHamiltonCycle(picks, ribs, structRibs), picks);
}