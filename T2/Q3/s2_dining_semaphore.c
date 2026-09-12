#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5

pthread_mutex_t chopsticks[N];
sem_t waiter;

void* philosopher(void* num)
{
    int id = *(int*)num;

    for (int i = 0; i < 3; i++)
    {
        printf("Philosopher %d is thinking...\n", id);
        sleep(1);

        printf("Philosopher %d is hungry\n", id);

        sem_wait(&waiter);

        pthread_mutex_lock(&chopsticks[id]);
        printf("Philosopher %d picked up left chopstick\n", id);

        pthread_mutex_lock(&chopsticks[(id + 1) % N]);
        printf("Philosopher %d picked up right chopstick\n", id);

        printf("Philosopher %d is eating...\n", id);
        sleep(1);

        pthread_mutex_unlock(&chopsticks[id]);
        pthread_mutex_unlock(&chopsticks[(id + 1) % N]);

        printf("Philosopher %d finished eating\n", id);

        sem_post(&waiter);
    }

    return NULL;
}

int main()
{
    pthread_t philo[N];
    int ids[N];

    sem_init(&waiter, 0, 4);

    for (int i = 0; i < N; i++)
        pthread_mutex_init(&chopsticks[i], NULL);

    for (int i = 0; i < N; i++)
    {
        ids[i] = i;
        pthread_create(&philo[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(philo[i], NULL);

    for (int i = 0; i < N; i++)
        pthread_mutex_destroy(&chopsticks[i]);

    sem_destroy(&waiter);

    printf("\nAll philosophers finished eating.\n");

    return 0;
}