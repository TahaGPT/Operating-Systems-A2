#include <bits/stdc++.h>
using namespace std;

struct Process{
    int id;
    int arrival;
    int burst;

};

int main()
{
    int n;
    cin >> n;
    Process *p = new Process[n];
    for (int i = 0; i < n; i++)
    {
        cin >> p[i].id >> p[i].arrival >> p[i].burst;
    }
}

