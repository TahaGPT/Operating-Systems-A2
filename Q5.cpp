#include <bits/stdc++.h>
using namespace std;

struct Process{
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
        if(time > p[i].arrival)
            time = p[i].arrival;
    }

    queue<int> rr1, rr2, fcfs;
    int q1 = 4, q2 = 8;
    for(int i = 0; i < n;i++)
    {
        if(p[i].arrival == time)
            rr1.push(i);
    }
    while(!(rr1.empty() || rr2.empty() || fcfs.empty()))
    {
        int i = rr1.front();
        rr1.pop();
        if(p[i].remaining > q1)
        {
            p[i].remaining -= q1;
            time += q1;
            for(int j = 0; j < n;j++)
            {
                if(p[j].arrival <= time && j != i)
                    rr1.push(j);
            }
            rr2.push(i);
        }
        else
        {
            time += p[i].remaining;
            p[i].remaining = 0;
            p[i].TAT = time - p[i].arrival;
            p[i].WAT = p[i].TAT - p[i].burst;
            for(int j = 0; j < n;j++)
            {
                if(p[j].arrival <= time && j != i)
                    rr1.push(j);
            }
        }
        if(rr1.empty())
        {
            while(!rr2.empty())
            {
                int i = rr2.front();
                rr2.pop();
                if(p[i].remaining > q2)
                {
                    p[i].remaining -= q2;
                    time += q2;
                    for(int j = 0; j < n;j++)
                    {
                        if(p[j].arrival <= time && j != i)
                            rr2.push(j);
                    }
                    fcfs.push(i);
                }
                else
                {
                    time += p[i].remaining;
                    p[i].remaining = 0;
                    p[i].TAT = time - p[i].arrival;
                    p[i].WAT = p[i].TAT - p[i].burst;
                    for(int j = 0; j < n;j++)
                    {
                        if(p[j].arrival <= time && j != i)
                            rr2.push(j);
                    }
                }
            }
            if(rr2.empty())
            {
                while(!fcfs.empty())
                {
                    int i = fcfs.front();
                    fcfs.pop();
                    time += p[i].remaining;
                    p[i].remaining = 0;
                    p[i].TAT = time - p[i].arrival;
                    p[i].WAT = p[i].TAT - p[i].burst;
                    for(int j = 0; j < n;j++)
                    {
                        if(p[j].arrival <= time && j != i)
                            fcfs.push(j);
                    }
                }
            }
        }
    }
    

}

