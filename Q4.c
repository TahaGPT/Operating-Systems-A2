#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

const int MAX_MSG_SIZE = 256;
const int NUM_PROCESSES = 4;
const int NUM_CYCLES = 5; 


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
    
    int pipes[NUM_PROCESSES][2];
    pid_t pids[NUM_PROCESSES];
    char process_name = 'A';

    
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            printf("Error Creating Pipe %s", i);
    
        }
    }

    
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        pids[i] = fork();

        if (pids[i] < 0)
        {
            printf("Error Creating Fork %s", i);
    
        }
        else if (pids[i] == 0)
        {
            
            process_name = 'A' + i; 
            break;
        }
    }

    
    if (process_name == 'A')
    {
        
        
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 0)
                close(pipes[i][1]); 
            if (i != 3)
                close(pipes[i][0]); 
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        
        for (int cycle = 1; cycle <= NUM_CYCLES; cycle++)
        {
            char message[MAX_MSG_SIZE];
            sprintf(message, "Message cycle %d from Process A", cycle);

            
            log_message(log_filename, message);
            printf("Process A sending: %s\n", message);

            
            if (write(pipes[0][1], message, strlen(message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }

            
            char received_message[MAX_MSG_SIZE];
            if (read(pipes[3][0], received_message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            printf("Process A received back: %s\n", received_message);

            
            sleep(1);
        }

        
        char end_msg[MAX_MSG_SIZE] = "END";
        write(pipes[0][1], end_msg, strlen(end_msg) + 1);

        
        close(pipes[0][1]);
        close(pipes[3][0]);

        
        for (int i = 0; i < NUM_PROCESSES - 1; i++)
        {
            wait(NULL);
        }
    }
    else if (process_name == 'B')
    {
        
        
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 1)
                close(pipes[i][1]); 
            if (i != 0)
                close(pipes[i][0]); 
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        while (1)
        {
            char message[MAX_MSG_SIZE];
            
            if (read(pipes[0][0], message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            
            if (strcmp(message, "END") == 0)
            {
                
                write(pipes[1][1], message, strlen(message) + 1);
                break;
            }

            
            char modified_message[MAX_MSG_SIZE];
            sprintf(modified_message, "%s - Processed by B", message);

            
            log_message(log_filename, modified_message);

            
            if (write(pipes[1][1], modified_message, strlen(modified_message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }
        }

        
        close(pipes[0][0]);
        close(pipes[1][1]);
        exit(EXIT_SUCCESS);
    }
    else if (process_name == 'C')
    {
        
        
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 2)
                close(pipes[i][1]); 
            if (i != 1)
                close(pipes[i][0]); 
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        while (1)
        {
            char message[MAX_MSG_SIZE];
            
            if (read(pipes[1][0], message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            
            if (strcmp(message, "END") == 0)
            {
                
                write(pipes[2][1], message, strlen(message) + 1);
                break;
            }

            
            char modified_message[MAX_MSG_SIZE];
            sprintf(modified_message, "%s - Processed by C", message);

            
            log_message(log_filename, modified_message);

            
            if (write(pipes[2][1], modified_message, strlen(modified_message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }
        }

        
        close(pipes[1][0]);
        close(pipes[2][1]);
        exit(EXIT_SUCCESS);
    }
    else if (process_name == 'D')
    {
        
        
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            if (i != 3)
                close(pipes[i][1]); 
            if (i != 2)
                close(pipes[i][0]); 
        }

        char log_filename[15];
        sprintf(log_filename, "log_%c.txt", process_name);

        while (1)
        {
            char message[MAX_MSG_SIZE];
            
            if (read(pipes[2][0], message, MAX_MSG_SIZE) == -1)
            {
                printf("Error reading from pipe");
                
            }

            
            if (strcmp(message, "END") == 0)
            {
                
                write(pipes[3][1], message, strlen(message) + 1);
                break;
            }

            
            char modified_message[MAX_MSG_SIZE];
            sprintf(modified_message, "%s - Processed by D", message);

            
            log_message(log_filename, modified_message);

            
            if (write(pipes[3][1], modified_message, strlen(modified_message) + 1) == -1)
            {
                printf("Error writing to pipe");
                
            }
        }

        
        close(pipes[2][0]);
        close(pipes[3][1]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        
        
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        
        for (int i = 0; i < NUM_PROCESSES; i++)
        {
            wait(NULL);
        }
    }

    return 0;
}