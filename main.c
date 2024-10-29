#include "a2.h"

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
