#ifndef A2_H
#define A2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 1010
#define MAX_LINE_LEN 100
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

int loadStudents(FILE *inputFilePointer, FILE *outputFilePointer, int option, student *students, int *studentCount);
student *sortStudents(student *student, int numberOfStudents);
student *mergeStudents(student *leftStudents, int numberOfLeftStudents, student *rightStudents, int numberOfRightStudents);
student *studentsCopy(student *students, int numberOfStudents);
int compareStudents(student a, student b);
int compareNames(char *a, char *b);
void printStudent(student s);
int monthStringToInt(char *month);
void writeToOutput(FILE *outputFilePointer, student *students, int studentCount);

#endif
