/* Dillon Cole Mabry UNC Charlotte ID: 800854402 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <err.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

void menu(long, long);
int evaluateMenu(int, long);
void displayTime();
void displayCalendar();
int get_week_day(int, int, int);
void displayFiles();

int main() {

    long ppid = getppid();
    long pid = getpid();
    printf("\nParent Process ID: %ld, Parent's parent process ID: %ld \n",pid,ppid);
	while(getppid() == ppid){
		menu(ppid, pid);
	}
    return 0;
}

/* Function to print menu options */
void menu(long ppid, long pid) {
    
    	pid = getpid();
	int choice = 0;
	
	printf("Choose the function to be performed by the child:\n"
	"(1) Display the current date and time.\n"
	"(2) Display the calendar of the current month.\n"
	"(3) List the files in the current directory.\n"
	"(4) Exit the program.\n\n"
	"Enter your choice: ");

	scanf("%d", &choice);
	if(choice == 4) {
		printf("Exiting Program....\n");
		exit(0);
	}
	evaluateMenu(choice, pid);
    	printf("Current process ID %ld\n", pid);
}

/* Function to evaluate the choice the user makes when 
selecting the case to perform. Includes creation of Child
processes as well as process ID identification/management */
int evaluateMenu(int choice, long pid){
	
	long ppid = getpid();
	
	switch (choice) {
		case 1:
			pid = fork();
			if (pid > 0) {
            printf("Child process ID %ld, Parent ID %ld \n\n", pid,ppid);
			}
			displayTime();
			kill(pid, SIGKILL);
			break;
		case 2:
			pid = fork();
			if (pid > 0) {
			printf("Child process ID %ld, Parent ID %ld \n\n", pid,ppid);
			}
			displayCalendar();
			kill(pid, SIGKILL);
			break;
		case 3:
			pid = fork();
			if(pid > 0) {
            printf("Child process ID %ld, Parent ID %ld \n\n", pid,ppid);
			}
			displayFiles();
			kill(pid, SIGKILL);
			break;
		default:
			printf("Error, please select a choice between 1 and 4!\n\n");
			break;	
	}
}

/* Function to display the current date and time
in US Eastern Standard Time (GMT -5) format */
void displayTime() {
    setenv("TZ", "EST5EDT", 1);
	long current, time();
	char *ctime();
	time(&current);
	printf("Current time: %s\n", ctime (&current));
}


/* Function to display the files in the current directory
along with read/write permissions.
Source: http://stackoverflow.com/questions/20775762/how-to-
walk-through-a-directory-in-c-and-print-all-files-name-and-
permissions */
void displayFiles() {
	
    DIR *dir;
    struct dirent* archive;
    struct stat fileStat;
    char fullpath[256];

    if ((dir=opendir("."))== NULL) {
		perror("Error in opening the directory for reading.\n");
        exit(1);
    }

    while ((archive = readdir(dir)) != NULL)
    {
        printf("%s ", archive->d_name);
        strcpy(fullpath, ".");
        strcat(fullpath, "/");
        strcat(fullpath, archive->d_name);
        if (!stat(fullpath, &fileStat))
        {
            printf((S_ISDIR(fileStat.st_mode))  ? "d" : "-");
            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
        } else {
            perror("Error full path\n");
        }
        printf("\n");
    }
	printf("\n");
    closedir(dir);
}

/* Function to display the current calendar of the
current month/year. The printout includes correct dates 
as well as a simple console GUI string representation. */
void displayCalendar() {
   
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    char *months[] = {" ","January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"};

    int numDays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    int month = aTime->tm_mon + 1; 
    int year = aTime->tm_year + 1900; 
    int daysInMonth = numDays[month]; 
    int dayOfWeek, date;
    
    int startingDay = get_week_day(1,month, year);

    /* determine if February has leap year or not */
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
       numDays[2] = 29;

    /* print out console GUI */
    printf("\n%21s\n\n", months[month]);  
    printf("  Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");         
    
    /* print new lines for each week including days */
    for (dayOfWeek = 0; dayOfWeek < startingDay; ++dayOfWeek) {
        // printf("     ");
        printf("\n");
    }
    
    for (date = 1; date <= daysInMonth; ++date) {
        printf("%5d", date);
        if (++dayOfWeek > 6) {
            printf("\n");
            dayOfWeek = 0;
        }
    } 
    printf("\n\n");
	
}

/* Algorithm to get the correct days in each week for 
each month. Source: http://stackoverflow.com/questions
/18664548/print-a-12-month-calendar-with-only-input-being-
the-year-using-c */
int get_week_day(int day, int month, int year) {
    
  int a, m, y;
  a = (14 - month) / 12;
  y = year - a;
  m = month + 12*a - 2;

  return (day + y + y/4 - y/100 + y/400 + (31*m)/12) % 7; 
}

