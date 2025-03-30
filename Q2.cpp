#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define NUM_STUDENTS 10

int choosing[NUM_STUDENTS];
int ticket[NUM_STUDENTS];
bool question_pending = false;
bool answer_done = true;


int max_ticket()
{
    int max = 0;
    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        if (ticket[i] > max)
        {
            max = ticket[i];
        }
    }
    return max;
}


void lock(int id)
{
    choosing[id] = 1;
    ticket[id] = max_ticket() + 1;
    choosing[id] = 0;

    for (int j = 0; j < NUM_STUDENTS; j++)
    {
        if (j == id)
            continue;
        while (choosing[j])
            ;
        while (ticket[j] != 0 &&
               (ticket[j] < ticket[id] || (ticket[j] == ticket[id] && j < id)))
            ;
    }
}


void unlock(int id)
{
    ticket[id] = 0;
}


void QuestionStart(int id)
{
    lock(id);
    while (!answer_done)
        ; 
    question_pending = true;
    answer_done = false;
    printf("Student %d: Asking a question\n", id);
}


void QuestionDone(int id)
{
    printf("Student %d: Finished asking the question\n", id);
    unlock(id);
}


void AnswerStart()
{
    while (!question_pending)
        ;
    printf("Professor: Answering the question\n");
}


void AnswerDone()
{
    printf("Professor: Finished answering\n");
    question_pending = false;
    answer_done = true;
}


void *student_thread(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < 3; i++)
    {
        QuestionStart(id);
        sleep(1);
        QuestionDone(id);
        while (!answer_done)
            ; 
    }

    return NULL;
}


void *professor_thread(void *arg)
{
    for (int i = 0; i < NUM_STUDENTS * 3; i++)
    {
        AnswerStart();
        sleep(2);
        AnswerDone();
    }

    return NULL;
}

int main()
{
    pthread_t students[NUM_STUDENTS];
    pthread_t professor;
    int student_ids[NUM_STUDENTS];

    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        choosing[i] = 0;
        ticket[i] = 0;
    }

    pthread_create(&professor, NULL, professor_thread, NULL);

    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        student_ids[i] = i;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    for (int i = 0; i < NUM_STUDENTS; i++)
    {
        pthread_join(students[i], NULL);
    }

    pthread_join(professor, NULL);

    return 0;
}
