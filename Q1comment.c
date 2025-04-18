// #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_PLAYERS 2  // Number of players
#define MAX_TURNS 10   // Maximum game turns

// Function to calculate zombie count based on roll number
int getZombieCount(int rollNumber) {
    return (rollNumber * 3) % 20 + 1; // Ensure at least 1 zombie
}

int main() {
    int rollNumber = 1234;  // Example roll number (Replace with actual input)
    int numZombies = getZombieCount(rollNumber);
    
    printf("Starting Zombie Survival Game!\n");
    printf("Players: %d | Zombies: %d\n", MAX_PLAYERS, numZombies);
    
    int player_pipes[MAX_PLAYERS][2];
    int zombie_pipes[numZombies][2];
    pid_t players[MAX_PLAYERS], zombies[numZombies];
    
    // Create player processes
    for (int i = 0; i < MAX_PLAYERS; i++) {
        pipe(player_pipes[i]);
        players[i] = fork();
        
        if (players[i] == 0) { // Child process (Player)
            close(player_pipes[i][0]);
            srand(time(NULL) + i);
            while (1) {
                int action;
                printf("Player %d: Choose action (0-Move, 1-Attack, 2-Hide): ", i + 1);
                scanf("%d", &action);
                write(player_pipes[i][1], &action, sizeof(action));
            }
            exit(0);
        }
    }
    
    // Create zombie processes
    for (int i = 0; i < numZombies; i++) {
        pipe(zombie_pipes[i]);
        zombies[i] = fork();
        
        if (zombies[i] == 0) { // Child process (Zombie)
            close(zombie_pipes[i][0]);
            srand(time(NULL) + i + 100);
            while (1) {
                int attack = rand() % 2;  // 0: Move closer, 1: Attack
                write(zombie_pipes[i][1], &attack, sizeof(attack));
                sleep(2);
            }
            exit(0);
        }
    }
    
    // Parent process (Game Manager)
    int action, attack;
    
    for (int turn = 0; turn < MAX_TURNS; turn++) {
        printf("\n--- Turn %d ---\n", turn + 1);
        
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (read(player_pipes[i][0], &action, sizeof(action)) > 0) {
                printf("Player %d action: %s\n", i + 1, action == 0 ? "Move" : action == 1 ? "Attack" : "Hide");
            }
        }
        
        for (int i = 0; i < numZombies; i++) {
            if (read(zombie_pipes[i][0], &attack, sizeof(attack)) > 0) {
                printf("Zombie %d action: %s\n", i + 1, attack == 0 ? "Move Closer" : "Attack");
            }
        }
        sleep(1);
    }
    
    printf("Game Over!\n");
    return 0;
}
