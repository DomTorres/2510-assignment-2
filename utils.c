#include "a2.h"

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

student *studentsCopy(student *students, int numberOfStudents)
{
    student *studentsCopy = (student *) malloc(numberOfStudents * sizeof(student));

    for(int i = 0; i < numberOfStudents; i++)
    {
        *(studentsCopy + i) = *(students + i);
    }

    return studentsCopy;
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