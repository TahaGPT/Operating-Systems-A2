
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

struct Player {
    string name;
    double batting_avg;
    double strike_rate;
    double bowling_avg;
    double economy_rate;
    double current_form;
};

// SJF for batting order (higher strike rate first)
bool compareBatsmen(const Player &a, const Player &b) {
    return a.strike_rate > b.strike_rate;
}

// SJF for bowling order (lower economy rate first)
bool compareBowlers(const Player &a, const Player &b) {
    return a.economy_rate < b.economy_rate;
}

// SRTF (considering current form dynamically)
bool compareSRTF(const Player &a, const Player &b) {
    return a.current_form > b.current_form;
}

vector<Player> readCSV(const string &filename) {
    vector<Player> players;
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        Player p;
        string temp;

        getline(ss, p.name, ',');
        getline(ss, temp, ','); p.batting_avg = temp.empty() ? 0 : stod(temp);
        getline(ss, temp, ','); p.strike_rate = temp.empty() ? 0 : stod(temp);
        getline(ss, temp, ','); p.bowling_avg = temp.empty() ? 0 : stod(temp);
        getline(ss, temp, ','); p.economy_rate = temp.empty() ? 0 : stod(temp);
        getline(ss, temp, ','); p.current_form = temp.empty() ? 0 : stod(temp);
        
        players.push_back(p);
    }
    file.close();
    return players;
}

void simulateMatch(vector<Player> players) {
    vector<Player> batsmen, bowlers;
    
    for (auto &p : players) {
        if (p.batting_avg > 0) batsmen.push_back(p);
        if (p.bowling_avg > 0) bowlers.push_back(p);
    }

    sort(batsmen.begin(), batsmen.end(), compareBatsmen);
    sort(bowlers.begin(), bowlers.end(), compareBowlers);

    cout << "\nOptimal Batting Order (SJF - Higher Strike Rate First):\n";
    for (auto &b : batsmen)
        cout << b.name << " (SR: " << b.strike_rate << ")\n";

    cout << "\nOptimal Bowling Order (SJF - Lower Economy Rate First):\n";
    for (auto &b : bowlers)
        cout << b.name << " (Eco: " << b.economy_rate << ")\n";
    
    sort(players.begin(), players.end(), compareSRTF);
    cout << "\nDynamic Order Based on Current Form (SRTF):\n";
    for (auto &p : players)
        cout << p.name << " (Form: " << p.current_form << ")\n";
}

int main() {
    string filename = "cricketers.csv";
    vector<Player> players = readCSV(filename);
    simulateMatch(players);
    return 0;
}
