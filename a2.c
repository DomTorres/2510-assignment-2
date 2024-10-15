#include <stdio.h>
#include <stdlib.h>

#define MAX_STUDENTS 50
#define MAX_NAME_LEN 10

typedef struct 
{
    int year;
    int month; // 1 - 12
    int day; // 1 - 31
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    double GPA;
    char immigration;
    int TOEFL;
} student;

student *sortStudents(student *student, int numberOfStudents);
student *mergeStudents(student *leftStudents, int numberOfLeftStudents, student *rightStudents, int numberOfRightStudents);
student *studentsCopy(student *students, int numberOfStudents);
int compareStudents(student a, student b);
int compareNames(char *a, char *b);
void printStudent(student s);

int main()
{
    int numberOfStudents = 5;

    student *students = (student *) malloc(numberOfStudents * sizeof(student));

    student mary = {1990, 2, 2, "Mary", "Jackson", 4.0, 'I', 60};
    student jack = {1990, 2, 3, "Jack", "He", 2.45, 'D'};
    student mike = {1980, 9, 2, "Mike", "Johnson", 3.125, 'D'};
    student jane = {1970, 3, 2, "Jane", "Zhang", 3.8, 'I', 120};
    student bob = {1990, 2, 3, "Bob", "He", 2.45, 'D'};

    students[0] = mary;
    students[1] = jack;
    students[2] = mike;
    students[3] = jane;
    students[4] = bob;
    
    printf("\nUNSORTED:\n");
    for(int i = 0; i < numberOfStudents; i++)
    {
        printStudent(students[i]);
    }

    student *sortedStudents = sortStudents(students, numberOfStudents);

    printf("\nSORTED:\n");
    for(int i = 0; i < numberOfStudents; i++)
    {
        printStudent(sortedStudents[i]);
    }

    free(students);
    free(sortedStudents);

    return 0;
} 

student *sortStudents(student *students, int numberOfStudents)
{
    if(numberOfStudents <= 1) 
    {
        return studentsCopy(students, numberOfStudents);
    }

    int numberOfLeftStudents = numberOfStudents / 2;
    int numberOfRightStudents = numberOfStudents - numberOfLeftStudents;

    // copy left
    student *leftStudents = studentsCopy(students, numberOfLeftStudents);

    // sort left
    student *sortedLeftStudents = sortStudents(leftStudents, numberOfLeftStudents);

    // copy right
    student *rightStudents = studentsCopy(students + numberOfLeftStudents, numberOfRightStudents);

    // sort right
    student *sortedRightStudents = sortStudents(rightStudents, numberOfRightStudents);

    // merge both halves
    student *sortedStudents = mergeStudents(sortedLeftStudents, numberOfLeftStudents, sortedRightStudents, numberOfRightStudents);

    // free memory no longer needed
    free(leftStudents);
    free(rightStudents);
    free(sortedLeftStudents);
    free(sortedRightStudents);

    return sortedStudents;
}

student *mergeStudents(student *sortedLeftStudents, int numberOfLeftStudents, student *sortedRightStudents, int numberOfRightStudents)
{
    int totalNumberOfStudents = numberOfLeftStudents + numberOfRightStudents;

    student *sortedStudents = (student *) malloc(totalNumberOfStudents * sizeof(student));

    int l = 0;
    int r = 0;
    for(int i = 0; i < totalNumberOfStudents; i++)
    {
        if(r >= numberOfRightStudents || 
            (l < numberOfLeftStudents && 
            compareStudents(sortedLeftStudents[l], sortedRightStudents[r]) < 0)
        )
        {
            sortedStudents[i] = sortedLeftStudents[l];
            l++;
        } 
        else 
        {
            sortedStudents[i] = sortedRightStudents[r];
            r++;
        }
    }

    return sortedStudents;
}

student *studentsCopy(student *students, int numberOfStudents)
{
    student *studentsCopy = (student *) malloc(numberOfStudents * sizeof(student));

    for(int i = 0; i < numberOfStudents; i++)
    {
        *(studentsCopy + i) = *(students + i);
    }

    return studentsCopy;
}

// Positive if a comes after b, negative if a comes before b, 0 if a is the same as b
int compareStudents(student a, student b) 
{
    int yearDifference = a.year - b.year;
    if(yearDifference != 0) return yearDifference;

    int monthDifference = a.month - b.month;
    if(monthDifference != 0) return monthDifference; 

    int dayDifference = a.day - b.day;
    if(dayDifference != 0) return dayDifference;

    int lastNameDifference = compareNames(a.lastName, b.lastName);
    if(lastNameDifference != 0) return lastNameDifference;

    int firstNameDifference = compareNames(a.firstName, b.firstName);
    if(firstNameDifference != 0) return firstNameDifference;

    int GPADifference = a.GPA - b.GPA;
    if(GPADifference != 0) return GPADifference;

    if(a.immigration == 'D' && b.immigration == 'I')
    {
        // different immigration statuses
        return -1;
    }
    else if(a.immigration == 'I' && b.immigration == 'D')
    {
        return 1;
    }
    else if(a.immigration == 'I' && b.immigration == 'I' && a.TOEFL != b.TOEFL) 
    {
        return a.TOEFL - b.TOEFL;
    }

    return 0;
} 

// Positive if a comes alphabetically after b, negative if a alphabetically comes before b, 0 if a is the same as b
int compareNames(char *a, char *b)
{
    for(int i = 0; i < MAX_NAME_LEN; i++)
    {
        if(a[i] != b[i])
        {
            return a[i] - b[i];
        }
    }

    return 0;
}

void printStudent(student s)
{
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    printf("%s %s %s-%d-%d %f %c %d\n",
        s.firstName, s.lastName,
        months[s.month - 1], s.day, s.year,
        s.GPA,
        s.immigration,
        s.immigration == 'I' ? s.TOEFL : 0
    );
}
