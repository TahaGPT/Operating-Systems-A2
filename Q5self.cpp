#include <bits/stdc++.h>
using namespace std;

struct Process
{
    int id;
    int arrival;
    int burst;
    int TAT = 0;
    int WAT = 0;
    int remaining;
};

int main()
{
    int n;
    cin >> n;
    Process *p = new Process[n];
    int time = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        cin >> p[i].id >> p[i].arrival >> p[i].burst;
        p[i].remaining = p[i].burst;
        if (time > p[i].arrival)
            time = p[i].arrival;
    }

    queue<int> rr1, rr2, fcfs, complete;
    const int q1 = 4, q2 = 8;
    while (complete.size() == n || !(rr1.empty() || rr2.empty() || fcfs.empty()))
    {
        for (int i = 0; i < n && p[i].arrival <= time; i++)
        {
            rr1.push(i);
        }   

        if(!rr1.empty())
        {
            int i = rr1.front();
            rr1.pop();
            time += q1;
            p[i].remaining -= q1;
            if(p[i].remaining <= q1)
            {
                rr1.push(i);
            }
            else{
                rr2.push(i);
            }
        }
        else if(!rr2.empty())
        {
            int i = rr2.front();
            rr2.pop();
            p[i].remaining--;
            time++;
            
        }
    }
}




