#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

const int MAX_MSG_SIZE = 256;
const int NUM_PROCESSES = 4;
const int NUM_CYCLES = 5; // Number of message cycles to complete

// Function to log messages to a file
void log_message(const char *filename, const char *message)
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        printf("Error Opening File Descriptor");

    }

    write(fd, message, strlen(message));
    write(fd, "\n", 1);
    close(fd);
}

int main()
{
    // Create pipes for communication between processes
    int pipes[NUM_PROCESSES][2];
    pid_t pids[NUM_PROCESSES];
    char process_name = 'A';

    // Initialize all pipes
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            printf("Error Creating Pipe %s", i);
    
        }
    }

    // Create child processes
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        pids[i] = fork();

        if (pids[i] < 0)
        {
            printf("Error Creating Fork %s", i);
    
        }
        else if (pids[i] == 0)
        {
            // Child process
            process_name = 'A' + i; // A, B, C, or D
            break;
        }
    }

    // Close unused pipe ends and handle process-specific logic
    if (process_name == 'A')
    {
        // Process A
        // Close all unused pipes
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 0)
                close(pipes[i][1]); // Close all write ends except A's
            if (i != 3)
                close(pipes[i][0]); // Close all read ends except D's output
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        // Initialize the message cycle
        for (int cycle = 1; cycle <= NUM_CYCLES; cycle++)
        {
            char message[MAX_MSG_SIZE];
            sprintf(message, "Message cycle %d from Process A", cycle);

            // Log the message
            log_message(log_filename, message);
            printf("Process A sending: %s\n", message);

            // Send to the next process (B)
            if (write(pipes[0][1], message, strlen(message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }

            // Wait for the message to complete the full circle
            char received_message[MAX_MSG_SIZE];
            if (read(pipes[3][0], received_message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            printf("Process A received back: %s\n", received_message);

            // Sleep briefly between cycles to make output more readable
            sleep(1);
        }

        // Signal other processes to terminate
        char end_msg[MAX_MSG_SIZE] = "END";
        write(pipes[0][1], end_msg, strlen(end_msg) + 1);

        // Close remaining pipe ends
        close(pipes[0][1]);
        close(pipes[3][0]);

        // Wait for all child processes to terminate
        for (int i = 0; i < NUM_PROCESSES - 1; i++)
        {
            wait(NULL);
        }
    }
    else if (process_name == 'B')
    {
        // Process B
        // Close all unused pipes
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 1)
                close(pipes[i][1]); // Close all write ends except B's
            if (i != 0)
                close(pipes[i][0]); // Close all read ends except A's output
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        while (1)
        {
            char message[MAX_MSG_SIZE];
            // Receive message from Process A
            if (read(pipes[0][0], message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            // Check for termination signal
            if (strcmp(message, "END") == 0)
            {
                // Pass the end signal to the next process
                write(pipes[1][1], message, strlen(message) + 1);
                break;
            }

            // Modify message
            char modified_message[MAX_MSG_SIZE];
            sprintf(modified_message, "%s - Processed by B", message);

            // Log the modified message
            log_message(log_filename, modified_message);

            // Send to the next process (C)
            if (write(pipes[1][1], modified_message, strlen(modified_message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }
        }

        // Close remaining pipe ends
        close(pipes[0][0]);
        close(pipes[1][1]);
        exit(EXIT_SUCCESS);
    }
    else if (process_name == 'C')
    {
        // Process C
        // Close all unused pipes
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 2)
                close(pipes[i][1]); // Close all write ends except C's
            if (i != 1)
                close(pipes[i][0]); // Close all read ends except B's output
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        while (1)
        {
            char message[MAX_MSG_SIZE];
            // Receive message from Process B
            if (read(pipes[1][0], message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            // Check for termination signal
            if (strcmp(message, "END") == 0)
            {
                // Pass the end signal to the next process
                write(pipes[2][1], message, strlen(message) + 1);
                break;
            }

            // Modify message
            char modified_message[MAX_MSG_SIZE];
            sprintf(modified_message, "%s - Processed by C", message);

            // Log the modified message
            log_message(log_filename, modified_message);

            // Send to the next process (D)
            if (write(pipes[2][1], modified_message, strlen(modified_message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }
        }

        // Close remaining pipe ends
        close(pipes[1][0]);
        close(pipes[2][1]);
        exit(EXIT_SUCCESS);
    }
    else if (process_name == 'D')
    {
        // Process D
        // Close all unused pipes
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 3)
                close(pipes[i][1]); // Close all write ends except D's
            if (i != 2)
                close(pipes[i][0]); // Close all read ends except C's output
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        while (1)
        {
            char message[MAX_MSG_SIZE];
            // Receive message from Process C
            if (read(pipes[2][0], message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            // Check for termination signal
            if (strcmp(message, "END") == 0)
            {
                // Pass the end signal back to Process A
                write(pipes[3][1], message, strlen(message) + 1);
                break;
            }

            // Modify message
            char modified_message[MAX_MSG_SIZE];
            sprintf(modified_message, "%s - Processed by D", message);

            // Log the modified message
            log_message(log_filename, modified_message);

            // Send back to Process A
            if (write(pipes[3][1], modified_message, strlen(modified_message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }
        }

        // Close remaining pipe ends
        close(pipes[2][0]);
        close(pipes[3][1]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process (waiting for children)
        // Close all pipes in parent as they're not needed
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        // Wait for all child processes to terminate
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            wait(NULL);
        }
    }

    return 0;
}