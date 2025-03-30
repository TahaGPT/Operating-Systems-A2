// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <fstream>
// #include <sstream>
// #include <queue>

// using namespace std;

// struct Player
// {
//     string name;
//     double strikeRate;  // Used for batsmen
//     double economyRate; // Used for bowlers
//     int form;           // Current form of the player (1-10)
//     string role;
// };

// // Sorting batsmen in descending order of Strike Rate, adjusting dynamically based on form
// bool compareBatsmen(Player a, Player b)
// {
//     return (a.strikeRate * a.form) > (b.strikeRate * b.form);
// }

// // Sorting bowlers in ascending order of Economy Rate, adjusting dynamically based on form
// bool compareBowlers(Player a, Player b)
// {
//     return (a.economyRate / a.form) < (b.economyRate / b.form);
// }

// // Function to read players from CSV file
// vector<Player> readPlayersFromCSV(const string &filename)
// {
//     vector<Player> players;
//     ifstream file(filename);
//     string line, name, role;
//     double strikeRate, economyRate;
//     int form;

//     if (file.is_open())
//     {
//         getline(file, line); // Skip header
//         while (getline(file, line))
//         {
//             stringstream ss(line);
//             getline(ss, name, ',');
//             ss >> strikeRate;
//             ss.ignore();
//             ss >> economyRate;
//             ss.ignore();
//             ss >> form;
//             ss.ignore();
//             getline(ss, role, ',');

//             players.push_back({name, strikeRate, economyRate, form, role});
//         }
//         file.close();
//     }
//     return players;
// }

// // Function to simulate Round Robin rotation for bowlers
// void roundRobinBowlingRotation(vector<Player> &bowlers, int overs)
// {
//     queue<Player> rotationQueue;
//     for (auto &bowler : bowlers)
//     {
//         rotationQueue.push(bowler);
//     }

//     cout << "\nBowling Rotation using Round Robin:" << endl;
//     for (int i = 1; i <= overs; i++)
//     {
//         Player currentBowler = rotationQueue.front();
//         rotationQueue.pop();
//         cout << "Over " << i << " - " << currentBowler.name << " (Economy Rate: " << currentBowler.economyRate << ")" << endl;
//         rotationQueue.push(currentBowler);
//     }
// }

// // Function to predict match outcome based on batting and bowling performances
// void predictMatchOutcome(vector<Player> &batsmen, vector<Player> &bowlers)
// {
//     double battingScore = 0, bowlingScore = 0;

//     for (const auto &batsman : batsmen)
//     {
//         battingScore += batsman.strikeRate * batsman.form;
//     }

//     for (const auto &bowler : bowlers)
//     {
//         bowlingScore += 100 / (bowler.economyRate * bowler.form); // Lower economy is better
//     }

//     cout << "\nPredicted Match Outcome:" << endl;
//     cout << "Total Batting Strength: " << battingScore << endl;
//     cout << "Total Bowling Strength: " << bowlingScore << endl;

//     if (battingScore > bowlingScore)
//     {
//         cout << "Prediction: Stronger Batting Performance Expected." << endl;
//     }
//     else
//     {
//         cout << "Prediction: Stronger Bowling Performance Expected." << endl;
//     }
// }

// int main()
// {
//     vector<Player> players = readPlayersFromCSV("cricketers_with_form.csv");
//     vector<Player> batsmen, bowlers;

//     for (const auto &player : players)
//     {
//         if (player.role == "Batsman")
//             batsmen.push_back(player);
//         else if (player.role == "Bowler")
//             bowlers.push_back(player);
//     }

//     // Apply SRTF logic by dynamically adjusting order based on ongoing form
//     sort(batsmen.begin(), batsmen.end(), compareBatsmen);
//     sort(bowlers.begin(), bowlers.end(), compareBowlers);

//     cout << "Optimal Batsmen Order (Based on Strike Rate & Form):" << endl;
//     for (auto &batsman : batsmen)
//     {
//         cout << batsman.name << " - Adjusted Score: " << batsman.strikeRate * batsman.form << endl;
//     }

//     cout << "\nOptimal Bowlers Order (Based on Economy Rate & Form):" << endl;
//     for (auto &bowler : bowlers)
//     {
//         cout << bowler.name << " - Adjusted Score: " << bowler.economyRate / bowler.form << endl;
//     }

//     // Simulate Round Robin rotation for bowlers over a given number of overs
//     int overs = 10; // Example: Rotate bowlers over 10 overs
//     roundRobinBowlingRotation(bowlers, overs);

//     // Predict match outcome based on performances
//     predictMatchOutcome(batsmen, bowlers);

//     return 0;
// }

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

struct Player
{
    string name;
    double strikeRate;  // Used for batsmen
    double economyRate; // Used for bowlers
    int form;           // Current form of the player (1-10)
    string role;
};

// Sorting batsmen in descending order of Strike Rate, adjusting dynamically based on form
bool compareBatsmen(Player a, Player b)
{
    return (a.strikeRate * a.form) > (b.strikeRate * b.form);
}

// Sorting bowlers in ascending order of Economy Rate, adjusting dynamically based on form
bool compareBowlers(Player a, Player b)
{
    return (a.economyRate / a.form) < (b.economyRate / b.form);
}

// Function to read players from CSV file
vector<Player> readPlayersFromCSV(const string &filename)
{
    vector<Player> players;
    ifstream file(filename);
    string line, name, role;
    double strikeRate, economyRate;
    int form;

    if (file.is_open())
    {
        getline(file, line); // Skip header
        while (getline(file, line))
        {
            stringstream ss(line);
            string token;

            // Parse each column properly
            getline(ss, name, ',');

            getline(ss, token, ',');
            strikeRate = token.empty() ? 0.0 : stod(token);

            getline(ss, token, ',');
            economyRate = token.empty() ? 0.0 : stod(token);

            getline(ss, token, ',');
            form = token.empty() ? 1 : stoi(token);

            getline(ss, role);

            players.push_back({name, strikeRate, economyRate, form, role});
        }
        file.close();
    }
    else
    {
        cerr << "Error: Unable to open file " << filename << endl;
    }
    return players;
}

// Function to simulate Round Robin rotation for bowlers
void roundRobinBowlingRotation(vector<Player> &bowlers, int overs)
{
    if (bowlers.empty())
    {
        cout << "\nNo bowlers available for rotation." << endl;
        return;
    }

    queue<Player> rotationQueue;
    for (auto &bowler : bowlers)
    {
        rotationQueue.push(bowler);
    }

    cout << "\nBowling Rotation using Round Robin:" << endl;
    for (int i = 1; i <= overs; i++)
    {
        Player currentBowler = rotationQueue.front();
        rotationQueue.pop();
        cout << "Over " << i << " - " << currentBowler.name << " (Economy Rate: " << currentBowler.economyRate << ")" << endl;
        rotationQueue.push(currentBowler);
    }
}

// Function to predict match outcome based on batting and bowling performances
void predictMatchOutcome(vector<Player> &batsmen, vector<Player> &bowlers)
{
    double battingScore = 0, bowlingScore = 0;

    for (const auto &batsman : batsmen)
    {
        battingScore += batsman.strikeRate * batsman.form;
    }

    for (const auto &bowler : bowlers)
    {
        // Avoid division by zero
        double adjustedEconomy = max(0.1, bowler.economyRate);
        bowlingScore += 100 / (adjustedEconomy * bowler.form);
    }

    cout << "\nPredicted Match Outcome:" << endl;
    cout << "Total Batting Strength: " << battingScore << endl;
    cout << "Total Bowling Strength: " << bowlingScore << endl;

    if (battingScore > bowlingScore)
    {
        cout << "Prediction: Stronger Batting Performance Expected." << endl;
    }
    else
    {
        cout << "Prediction: Stronger Bowling Performance Expected." << endl;
    }
}

int main()
{
    vector<Player> players = readPlayersFromCSV("cricketers_with_form.csv");

    if (players.empty())
    {
        cerr << "No players loaded. Check if the CSV file exists and has valid data." << endl;
        return 1;
    }

    vector<Player> batsmen, bowlers;

    for (const auto &player : players)
    {
        if (player.role == "Batsman")
            batsmen.push_back(player);
        else if (player.role == "Bowler")
            bowlers.push_back(player);
    }

    // Apply SRTF logic by dynamically adjusting order based on ongoing form
    sort(batsmen.begin(), batsmen.end(), compareBatsmen);
    sort(bowlers.begin(), bowlers.end(), compareBowlers);

    cout << "Optimal Batsmen Order (Based on Strike Rate & Form):" << endl;
    for (auto &batsman : batsmen)
    {
        cout << batsman.name << " - Adjusted Score: " << batsman.strikeRate * batsman.form << endl;
    }

    cout << "\nOptimal Bowlers Order (Based on Economy Rate & Form):" << endl;
    for (auto &bowler : bowlers)
    {
        cout << bowler.name << " - Adjusted Score: " << bowler.economyRate / bowler.form << endl;
    }

    // Simulate Round Robin rotation for bowlers over a given number of overs
    int overs = 10; // Example: Rotate bowlers over 10 overs
    roundRobinBowlingRotation(bowlers, overs);

    // Predict match outcome based on performances
    predictMatchOutcome(batsmen, bowlers);

    return 0;
}