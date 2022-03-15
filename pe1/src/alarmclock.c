#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

bool turnOff = false;

struct Alarm
{
    time_t time;
    int pid;
    bool exists;
    int id;
};

struct Alarm alarms[8];

void catch_the_zombies()
{
    pid_t pid;
    pid = waitpid(-1, NULL, WNOHANG);

    if (pid == 0)
    {
        return;
    }

    unsigned int alarm_number = -100;

    for (int i = 0; i < 8; i++)
    {
        if (alarms[i].pid == pid)
        {
            alarm_number = i;
            break;
        }
    }

    if (alarm_number == -100)
    {
        return;
    }
    else
    {
        alarms[alarm_number].exists = false;
        alarms[alarm_number].time = 0;
        alarms[alarm_number].pid = 0;
        alarms[alarm_number].id = -1;
    }
}

int fork_alarm(int s)
{
    unsigned int pid = fork();
    if (pid != 0)
    {
        return pid;
    }

    else
    {

        time_t rtime;
        time(&rtime);
        int wait = (int)difftime(s, rtime);

        if (wait < 0)
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            sleep(wait);
            printf("\nAlarm has finished\n");
            printf("RING RING%c", 7);
            exit(EXIT_SUCCESS);
        }
    }
}

void schedule()
{
    catch_the_zombies();
    // Source: https://stackoverflow.com/questions/21651904/trying-to-set-values-in-a-struct-tm
    struct tm t_var = {0};
    time_t time_of_day;

    int day;
    int mon;
    int year;
    int hour;
    int min;
    int sec;

    printf("Insert a time of the format DD:MM:YYYY HH:MM:SS\n");
    scanf("%d:%d:%d %d:%d:%d", &day, &mon, &year, &hour, &min, &sec);

    time_t time_now;

    t_var.tm_mday = day;
    t_var.tm_mon = mon - 1;
    t_var.tm_year = year - 1900;
    t_var.tm_hour = hour;
    t_var.tm_min = min;
    t_var.tm_sec = sec;
    t_var.tm_isdst = -1;
    time_of_day = mktime(&t_var);

    unsigned int pid = fork_alarm(time_of_day);
    for (int i = 0; i < 8; i++)
    {
        if (!alarms[i].exists)
        {
            alarms[i].exists = true;
            alarms[i].time = time_of_day;
            alarms[i].pid = pid;
            alarms[i].id = i;
            time_t rtime;
            time(&rtime);
            int wait = (int)difftime(time_of_day, rtime);
            printf("Scheduled an alarm for %s(%d seconds)\n", ctime(&time_of_day), wait);
            break;
        }
    }

    // makeAlarmRing(time_of_alarm);
}

void list()
{
    catch_the_zombies();
    printf("List of all alarms: \n");
    for (int i = 0; i < 8; i++)
    {
        if (alarms[i].exists)
        {
            printf("%d: %s \n", i, ctime(&alarms[i].time));
        }
        else
        {
            printf("%d: Empty alarm slot\n", i);
            printf("\n");
        }
    }
}

void cancel()
{
    catch_the_zombies();
    int cancelChoice;
    printf("Which alarm do you want to cancel? (INSERT A NUMBER) \nPress \"9\" to go back.\n");
    list();
    scanf(" %d", &cancelChoice);

    if (cancelChoice == 9)
    {
        return;
    }
    if (cancelChoice == 0, 1, 2, 3, 4, 5, 6, 7)
    {
        if (alarms[cancelChoice].exists) 
        {

            kill(alarms[cancelChoice].pid, SIGKILL);

            alarms[cancelChoice].exists = false;
            printf("Alarm %d was cancelled\n", cancelChoice);
        }
        else{
            printf("This alarm doesn't exist\n");
            return;
        }
    }
    else
    {
        printf("Something went wrong\n");
        return;
    }
}

void shutDown()
{
    turnOff = true;
}

void printMenu()
{

    if (turnOff == true)
    {
        return;
    }

    catch_the_zombies();

    char choice;
    printf("Menu - Enter a character:\n");
    printf("---------------\n");
    printf("\"s\" for schedule\n");
    printf("\"l\" for list\n");
    printf("\"c\" for cancel\n");
    printf("\"x\" for exit\n");
    printf("----------------\n");

    scanf(" %s", &choice);

    if (choice == 's')
    {
        schedule();
        printMenu();
    }

    else if (choice == 'l')
    {
        list();
        printMenu();
    }

    else if (choice == 'c')
    {
        cancel();
        printMenu();
    }

    else if (choice == 'x')
    {
        shutDown();
    }

    else
    {
        printf("Something went wrong.\n");
        printMenu();
    }
};

int main()
{
    printMenu();
    return 1;
}
