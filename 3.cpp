#include <iostream>
#include<stdlib.h>
#include <stdio.h>
#include<unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
using namespace std;
// n = number of process, m = number of recources
const int maxP = 5;
const int maxR = 3;
void findNeed(int need[][maxR], int max[][maxR], int alloc[][maxR], int n, int m);
int safeSeq(int prcs[], int avail[], int max[][maxR], int alloc[][maxR], int n, int m);
int safeReq(int p, int req[], int prcs[], int avail[], int max[][maxR], int alloc[][maxR], int n, int m);
void findNeed(int need[][maxR], int max[][maxR], int alloc[][maxR], int n, int m){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    }
}
int safeSeq(int prcs[], int avail[], int max[][maxR], int alloc[][maxR], int n, int m){
    int need[maxP][maxR];
    findNeed(need, max, alloc, n, m);
    bool PrcsExecd[maxP] = { 0 };
    int safeSeq[maxP];
    int UpdatedAvail[maxR];
    int count = 0;
    int j, i, k, a;
    for (i = 0; i < m; i++)
        UpdatedAvail[i] = avail[i];
    for (a= 0; count < n; a++) {
        bool found = false;
        for (i = 0; i < n; i++) {
            if (PrcsExecd[i] == 0) {
                for (j = 0; j < m; j++) {
                    if (need[i][j] > UpdatedAvail[j])
                        break;
                }
                if (j == m) {
                    for (k = 0; k < m; k++)
                        UpdatedAvail[k] = UpdatedAvail[k] + alloc[i][k];
                    safeSeq[count++] = i;
                    PrcsExecd[i] = 1;
                    found = true;
                }
            }
        }
        if (found == false)
            return 0;
    }
    cout << "Safe Sequence: ";
    for (i = 0; i < n; i++) {
        cout << "P" << prcs[safeSeq[i]];
        cout << "  ";
    }
    cout << endl;
    return 1;
}
int safeReq(int p, int req[], int prcs[], int avail[], int max[][maxR], int alloc[][maxR], int n, int m){
    int need[maxP][maxR];
    findNeed(need, max, alloc, n, m);
    for (int i = 0; i < m; i++) {
        if (req[i] > need[p][i])
            return 0;
        if (req[i] > avail[i])
            return 0;
        avail[i] = avail[i] - req[i];
        alloc[p][i] = alloc[p][i] + req[i];
        need[p][i] = need[p][i] - req[i];
    }
    int s = safeSeq(prcs, avail, max, alloc, n, m);
    for (int i = 0; i < m; i++) {
        avail[i] = avail[i] + req[i];
        alloc[p][i] = alloc[p][i] - req[i];
        need[p][i] = need[p][i] - req[i];
    }
    return s;
}
int main() {
    const int n = 5;
    const int m = 3;
    int prcs[n] = { 0, 1, 2, 3, 4 };
    int alloc[n][m] = { { 0, 1, 0 }, { 2, 0, 0 }, { 3, 0, 2 }, { 2, 1, 1 }, { 0, 0, 2 } };
    int max[n][m] = { { 7, 5, 3 }, { 3, 2, 2 }, { 9, 0, 2 }, { 2, 2, 2 }, { 4, 3, 3 } };
    int avail[m] = { 3, 3, 2 };
    int req[m];
    int p;
    if (safeSeq(prcs, avail, max, alloc, n, m) == 1)
        cout << "System in safe state!\n";
    else
        cout << "System not in safe state!\n";
    cout << "Requesting Process #: ";
    cin >> p;
    cout << "Request vector:\n";
    for (int i = 0; i < m; i++)
        cin >> req[i];
    if (safeReq(p, req, prcs, avail, max, alloc, n, m) == 1)
        cout << "Request Accepted!\n";
    else
        cout << "Request Declined!\n";
    return 0;
}

