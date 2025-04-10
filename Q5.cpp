// #include <bits/stdc++.h>
// using namespace std;

// struct Process
// {
//     int id;
//     int arrival;
//     int burst;
//     int TAT = 0;
//     int WAT = 0;
//     int completion;
//     int remaining;
// };

// struct Running
// {
//     int p = -1;
//     int qN;
// };

// int main()
// {
//     int n;
//     cin >> n;
//     Process *p = new Process[n];
//     int time = INT_MAX;
//     for (int i = 0; i < n; i++)
//     {
//         cin >> p[i].id >> p[i].arrival >> p[i].burst;
//         p[i].remaining = p[i].burst;
//         if (time > p[i].arrival)
//             time = p[i].arrival;
//     }
//     queue<int> rr1, rr2, fcfs, complete;
//     Running r;
//     int total = 0;
//     const int q1 = 4, q2 = 8;
//     while (complete.size() != n || !(rr1.empty() || rr2.empty() || fcfs.empty()))
//     {
//         cout << "Loop : \n";
//         for (int i = total; i < n && p[i].arrival <= time; i++)
//         {
//             cout << "Process " << i << " entered at Time " << time << " seconds\n";
//             rr1.push(i);
//             total++;
//         }

//         if(!rr1.empty())
//         {
//             int i = rr1.front();
//             rr1.pop();
//             time += q1;
//             p[i].remaining -= q1;
//             if(p[i].remaining <= 0)
//             {
//                 p[i].completion = time + p[i].remaining;
//                 cout << "Process " << i << " -> " << p[i].completion << endl;
//                 complete.push(i);
//             }
//             else if(p[i].remaining <= q1)
//             {
//                 rr1.push(i);
//             }
//             else if (p[i].remaining > q1)
//             {
//                 rr2.push(i);
//             }

//         }
//         else if(!rr2.empty())
//         {
//             int i = rr2.front();
//             rr2.pop();
//             if(total < n && time + q2 > p[total].arrival)
//             {
//                 p[i].remaining -= (p[total].arrival - time);
//                 rr2.push(i);
//                 time += (p[total].arrival - time);
//             }
//             else
//             {
//                 p[i].remaining -= q2;
//                 time += q2;
//             }
//             if (p[i].remaining <= 0)
//             {
//                 p[i].completion = time + p[i].remaining;
//                 cout << "Process " << i << " -> " << p[i].completion << endl;
//                 complete.push(i);
//             }
//             else if (p[i].remaining <= q2)
//             {
//                 rr2.push(i);
//             }
//             else if (p[i].remaining > q2)
//             {
//                 fcfs.push(i);
//             }

//         }
//         else if(!fcfs.empty())
//         {
//             int i = fcfs.front();
//             if(total < n)
//             {
//                 p[i].remaining -= p[total].arrival - time;
//                 time += p[total].arrival - time;
//             }
//             else
//             {
//                 time += p[i].remaining;
//                 p[i].remaining = 0;
//                 p[i].completion = time;
//                 cout << "Process " << i << " -> " << p[i].completion << endl;
//                 complete.push(i);
//             }
//         }
//         else{
//             time++;
//         }
//     }
// }

#include <bits/stdc++.h>
using namespace std;

struct Process
{
    int id;
    int arrival;
    int burst;
    int TAT = 0;    
    int WAT = 0;    
    int completion; 
    int remaining;  
};

int main()
{
    int n;
    cin >> n;
    Process *p = new Process[n];

    
    int earliest_arrival = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        cin >> p[i].id >> p[i].arrival >> p[i].burst;
        p[i].remaining = p[i].burst;
        earliest_arrival = min(earliest_arrival, p[i].arrival);
    }

    
    sort(p, p + n, [](const Process &a, const Process &b)
         { return a.arrival < b.arrival; });

    queue<int> rr1, rr2, fcfs;
    bool processed[n] = {false}; 
    int completed_count = 0;

    
    int current_time = earliest_arrival;
    const int Q1 = 4, Q2 = 8;

    
    while (completed_count < n)
    {
        
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival <= current_time && p[i].remaining > 0 && !processed[i])
            {
                rr1.push(i);
                processed[i] = true;
                cout << "Process " << p[i].id << " entered RR1 queue at time " << current_time << endl;
            }
        }

        if (!rr1.empty())
        {
            
            int idx = rr1.front();
            rr1.pop();
            processed[idx] = false; 

            cout << "Executing Process " << p[idx].id << " from RR1 at time " << current_time << endl;

            if (p[idx].remaining <= Q1)
            {
                
                current_time += p[idx].remaining;
                p[idx].remaining = 0;
                p[idx].completion = current_time;
                completed_count++;

                
                p[idx].TAT = p[idx].completion - p[idx].arrival;
                p[idx].WAT = p[idx].TAT - p[idx].burst;

                cout << "Process " << p[idx].id << " completed at time " << current_time << endl;
            }
            else
            {
                
                current_time += Q1;
                p[idx].remaining -= Q1;

                
                rr2.push(idx);
                processed[idx] = true; 
                cout << "Process " << p[idx].id << " moved to RR2 queue at time " << current_time << endl;
            }
        }
        else if (!rr2.empty())
        {
            
            int idx = rr2.front();
            rr2.pop();
            processed[idx] = false; 

            cout << "Executing Process " << p[idx].id << " from RR2 at time " << current_time << endl;

            if (p[idx].remaining <= Q2)
            {
                
                current_time += p[idx].remaining;
                p[idx].remaining = 0;
                p[idx].completion = current_time;
                completed_count++;

                
                p[idx].TAT = p[idx].completion - p[idx].arrival;
                p[idx].WAT = p[idx].TAT - p[idx].burst;

                cout << "Process " << p[idx].id << " completed at time " << current_time << endl;
            }
            else
            {
                
                current_time += Q2;
                p[idx].remaining -= Q2;

                
                fcfs.push(idx);
                processed[idx] = true; 
                cout << "Process " << p[idx].id << " moved to FCFS queue at time " << current_time << endl;
            }
        }
        else if (!fcfs.empty())
        {
            
            int idx = fcfs.front();
            fcfs.pop();
            processed[idx] = false; 

            cout << "Executing Process " << p[idx].id << " from FCFS at time " << current_time << endl;

            
            current_time += p[idx].remaining;
            p[idx].remaining = 0;
            p[idx].completion = current_time;
            completed_count++;

            
            p[idx].TAT = p[idx].completion - p[idx].arrival;
            p[idx].WAT = p[idx].TAT - p[idx].burst;

            cout << "Process " << p[idx].id << " completed at time " << current_time << endl;
        }
        else
        {
            
            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++)
            {
                if (p[i].arrival > current_time && p[i].remaining > 0 && p[i].arrival < next_arrival)
                {
                    next_arrival = p[i].arrival;
                }
            }

            if (next_arrival != INT_MAX)
            {
                cout << "Idle until time " << next_arrival << endl;
                current_time = next_arrival;
            }
            else
            {
                
                cout << "Error: No processes in queues and none arriving, but not all completed" << endl;
                break;
            }
        }

        
        for (int i = 0; i < n; i++)
        {
            if (p[i].arrival <= current_time && p[i].remaining > 0 && !processed[i])
            {
                rr1.push(i);
                processed[i] = true;
                cout << "Process " << p[i].id << " entered RR1 queue at time " << current_time << endl;
            }
        }
    }

    
    cout << "\nProcess Results:\n";
    cout << "ID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";

    float avg_tat = 0, avg_wat = 0;
    for (int i = 0; i < n; i++)
    {
        cout << p[i].id << "\t" << p[i].arrival << "\t" << p[i].burst << "\t"
             << p[i].completion << "\t\t" << p[i].TAT << "\t\t" << p[i].WAT << endl;
        avg_tat += p[i].TAT;
        avg_wat += p[i].WAT;
    }

    avg_tat /= n;
    avg_wat /= n;
    cout << "\nAverage Turnaround Time: " << avg_tat << endl;
    cout << "Average Waiting Time: " << avg_wat << endl;

    delete[] p;
    return 0;
}