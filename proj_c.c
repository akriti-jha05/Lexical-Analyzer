#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student
{
    int rollNo;
    int admission_number;
    char name[50];
    int age;
    char grade[5];
};

void addStudent();
void displayStudents();
void searchStudent();
void deleteStudent();
void updateStudent();

int main()
{
    int choice;

    while (1)
    {
        printf("\n School Management System \n");
        printf("1. Add student\n");
        printf("2. Display all students\n");
        printf("3. Search student by roll number\n");
        printf("4. Delete student by roll number\n");
        printf("5. Update student by roll number\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            displayStudents();
            break;
        case 3:
            searchStudent_and_deletestudent();
            break;
        
        case 4:
            updateStudent();
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

void addStudent()
{
    FILE *fp;
    struct Student s;

    fp = fopen("students.dat", "ab");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    printf("Enter Admission Number");
    scanf("%d" , &s.admission_number);
    printf("Enter Roll Number: ");
    scanf("%d", &s.rollNo);
    printf("Enter Name: ");
    getchar();
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';
    printf("Enter Age: ");
    scanf("%d", &s.age);
    printf("Enter Grade: ");
    scanf("%s", s.grade);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Student added successfully\n");
}

void displayStudents()
{
    FILE *fp;
    struct Student s;

    fp = fopen("students.dat", "rb");
    if (fp == NULL)
    {
        printf("No records found\n");
        return;
    }

    printf("\nStudent Records \n");
    while (fread(&s, sizeof(s), 1, fp))
    {
        printf("Roll No: %d\n", s.rollNo);
        printf("Name   : %s\n", s.name);
        printf("Age    : %d\n", s.age);
        printf("Grade  : %s\n", s.grade);
        printf("----------------------\n");
    }
    fclose(fp);
}

void searchStudent_and_deletestudent()
{
    FILE *fp;
    struct Student s;
    int roll, admission_number,  found = 0;

    fp = fopen("students.dat", "rb");
    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Roll Number to Search: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.rollNo == roll)
        {
            printf("Record Found!\n");
            printf("Roll No: %d\n", s.rollNo);
            printf("Name   : %s\n", s.name);
            printf("Age    : %d\n", s.age);
            printf("Grade  : %s\n", s.grade);
            found = 1;
            break;
        }
    }
    char ans;
    printf("If you want to delete a student. Enter Y or N");
    scanf("%c", &ans);

    FILE *temp;

    temp = fopen("temp.dat", "wb");

    if (temp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Admission Number  to delete: ");
    scanf("%d", &admission_number);

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.rollNo != admission_number)
        {
            fwrite(&s, sizeof(s), 1, temp);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        printf("Student deleted successfully!\n");
    else
        printf("Student with Roll No %d not found!\n", roll);

}

    

void deleteStudent()
{
    FILE *fp, *temp;
    struct Student s;
    int roll, found = 0;

    fp = fopen("students.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // printf("Enter Roll Number to Search: ");
    // scanf("%d", &roll);
    searchStudent();

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.rollNo != roll)
        {
            fwrite(&s, sizeof(s), 1, temp);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        printf("Student deleted successfully!\n");
    else
        printf("Student with Roll No %d not found!\n", roll);
}

void updateStudent()
{
    FILE *fp;
    struct Student s;
    int roll, found = 0;

    fp = fopen("students.dat", "rb+");
    if (fp == NULL)
    {
        printf("No records found!\n");
        return;
    }

    printf("Enter Roll Number to Update: ");
    scanf("%d", &roll);

    while (fread(&s, sizeof(s), 1, fp))
    {
        if (s.rollNo == roll)
        {
            printf("Enter New Name: ");
            getchar();
            fgets(s.name, sizeof(s.name), stdin);
            s.name[strcspn(s.name, "\n")] = '\0';
            printf("Enter New Age: ");
            scanf("%d", &s.age);
            printf("Enter New Grade: ");
            scanf("%s", s.grade);

            fseek(fp, -sizeof(s), SEEK_CUR);
            fwrite(&s, sizeof(s), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (found)
        printf("Student updated successfully\n");
    else
        printf("Student with Roll No %d not found\n", roll);
}
