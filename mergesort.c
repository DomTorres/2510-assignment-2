#include "a2.h"

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

    if(a.immigration == 'D' && b.immigration == 'I') return -1;
    else if(a.immigration == 'I' && b.immigration == 'D') return 1;
    else if(a.immigration == 'I' && b.immigration == 'I' && a.TOEFL != b.TOEFL) return a.TOEFL - b.TOEFL;

    return 0;
} 

// Positive if a comes alphabetically after b, negative if a alphabetically comes before b, 0 if a is the same as b
// TO DO: NAMES MUST BE CASE SENSITIVE (not yet handled)
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

