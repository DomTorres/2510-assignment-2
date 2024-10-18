#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 50
#define MAX_LINE_LEN 50
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

int main(int argc, char *argv[])
{
    // validate arg counter
    if(argc != 4)
    {
        printf("Error: Usage must be ./a2 <input file> <output file> <option>\n");
        return 1;
    }

    // validate option
    int option = atoi(argv[3]);
    if(!(option == 1 || option == 2 || option == 3))
    {
        printf("Error: Invalid option.\n");
        return 1;
    }

    // open input file
    FILE *inputFilePointer = fopen(argv[1], "r");
    if(!inputFilePointer)
    {
        printf("Error: Input file not found.\n");
        return 1;
    }

    // open output file
    FILE *outputFilePointer = fopen(argv[2], "w");
    if(!outputFilePointer)
    {
        printf("Error: Output file cannot be created.\n");
        return 1;
    }

    // load students from input file
    student *students = (student *) malloc(MAX_STUDENTS * sizeof(student));
    if(!students)
    {
        printf("Error: Memory allocation error.\n");
        return 1;
    }

    int studentCount = 0;

    if(loadStudents(inputFilePointer, outputFilePointer, option, students, &studentCount) != 0)
    {
        return 1;
    }
    
    // printf("\nUNSORTED:\n");
    // for(int i = 0; i < studentCount; i++)
    // {
    //     printStudent(students[i]);
    // }

    // sort students
    student *sortedStudents = sortStudents(students, studentCount);

    // printf("\nSORTED:\n");
    // for(int i = 0; i < studentCount; i++)
    // {
    //     printStudent(sortedStudents[i]);
    // }

    writeToOutput(outputFilePointer, sortedStudents, studentCount);

    free(students);
    free(sortedStudents);

    fclose(inputFilePointer);

    return 0;
} 

int loadStudents(FILE *inputFilePointer, FILE *outputFilePointer, int option, student *students, int *studentCount)
{
    // printf("STUDENT COUNT: %d\n", *studentCount);

    char line[MAX_LINE_LEN];

    while(fgets(line, MAX_LINE_LEN, inputFilePointer))
    {
        // parse line, store in variables
        char firstName[MAX_NAME_LEN];
        char lastName[MAX_NAME_LEN];

        char month[10]; // extra buffer
        int day = -1;
        int year = -1;
        
        double GPA = -1;
        char immigration = ' ';
        int TOEFL = -1;

        sscanf(line, "%s %s %3s-%d-%d %lf %c %d",
            firstName,
            lastName,
            month,
            &day,
            &year,
            &GPA,
            &immigration,
            &TOEFL);

        // if we don't need to save this student, then don't bother creating a student
        int saveDomestic = immigration == 'D' && (option == 1 || option == 3);
        int saveInternational = immigration == 'I' && (option == 2 || option == 3);

        if(!(saveDomestic || saveInternational))
        {
            continue;
        }

        int monthInt = monthStringToInt(month);

        // validate fields
        if(immigration == 'I' && !(0 <= TOEFL && TOEFL <= 120))
        {
            fprintf(outputFilePointer, "Error: Invalid TOEFL.\n");
            return 1;
        }

        if(immigration == 'D' && TOEFL != -1)
        {
            fprintf(outputFilePointer, "Error: Domestic student must not have a TOEFL field.\n");
            return 1;
        }

        if(!(immigration == 'I' || immigration == 'D'))
        {
            fprintf(outputFilePointer, "Error: Invalid immigration status.\n");
            return 1;
        }

        if(!(0.0 <= GPA && GPA <= 4.3))
        {
            fprintf(outputFilePointer, "Error: Invalid GPA.\n");
            return 1;
        }

        if(monthInt == -1)
        {
            fprintf(outputFilePointer, "Invalid month.\n");
            return 1;
        }

        if(!(1950 <= year && year <= 2010))
        {
            fprintf(outputFilePointer, "Error: Invalid year.\n");
            return 1;
        }

        // TO DO (?): validate day 

        // create student and populate fields
        student this;

        this.year = year;
        this.month = monthInt;
        this.day = day;
        strcpy(this.firstName, firstName);
        strcpy(this.lastName, lastName);
        this.GPA = GPA;
        this.immigration = immigration;
        if(immigration == 'I') { this.TOEFL = TOEFL; }

        // save student
        students[(*studentCount)++] = this;

        if(*studentCount >= MAX_STUDENTS)
        {
            return 1;
        }
    }

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
        // different immigration statuses
        return 1;
    }
    else if(a.immigration == 'I' && b.immigration == 'I' && a.TOEFL != b.TOEFL) 
    {
        return a.TOEFL - b.TOEFL;
    }

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

void printStudent(student s)
{
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    printf("%s %s %s-%d-%d %f %c",
        s.firstName, s.lastName,
        months[s.month - 1], s.day, s.year,
        s.GPA,
        s.immigration
    );

    if(s.immigration == 'I') { printf(" %d", s.TOEFL); }

    printf("\n");
}

int monthStringToInt(char *month)
{
    if(strcmp(month, "Jan") == 0) { return 1; }
    if(strcmp(month, "Feb") == 0) { return 2; }
    if(strcmp(month, "Mar") == 0) { return 3; }
    if(strcmp(month, "Apr") == 0) { return 4; }
    if(strcmp(month, "May") == 0) { return 5; }
    if(strcmp(month, "Jun") == 0) { return 6; }
    if(strcmp(month, "Jul") == 0) { return 7; }
    if(strcmp(month, "Aug") == 0) { return 8; }
    if(strcmp(month, "Sep") == 0) { return 9; }
    if(strcmp(month, "Oct") == 0) { return 10; }
    if(strcmp(month, "Nov") == 0) { return 11; }
    if(strcmp(month, "Dec") == 0) { return 12; }
    return -1;
}

void writeToOutput(FILE *outputFilePointer, student *students, int studentCount)
{
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    for(int i = 0; i < studentCount; i++)
    {
        student s = students[i];

        fprintf(outputFilePointer, "%s %s %s-%d-%d %f %c",
            s.firstName, s.lastName,
            months[s.month - 1], s.day, s.year,
            s.GPA,
            s.immigration
        );

        if(s.immigration == 'I') { fprintf(outputFilePointer ," %d", s.TOEFL); }

        fprintf(outputFilePointer, "\n");
    }
}

