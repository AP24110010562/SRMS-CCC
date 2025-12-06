#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE      "student.txt"
#define CREDENTIALS_FILE  "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

/* Prototypes */
int loginSystem(void);
void mainMenu(void);

void adminMenu(void);
void staffMenu(void);
void userMenu(void);
void guestMenu(void);

void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
int  isDuplicateRoll(int);

/* ================= MAIN ================= */
int main(void) {
    if (loginSystem())
        mainMenu();
    else
        printf("\nLogin Failed!\n");

    return 0;
}

/* ================= LOGIN ================= */
int loginSystem(void) {
    char username[20], password[20];
    char fu[20], fp[20], fr[10];

    printf("\n=== LOGIN ===\n");
    printf("Username: ");
    scanf("%19s", username);
    printf("Password: ");
    scanf("%19s", password);

    FILE *file = fopen(CREDENTIALS_FILE, "r");
    if (!file) {
        printf("credentials.txt missing!\n");
        return 0;
    }

    while (fscanf(file, "%19s %19s %9s", fu, fp, fr) == 3) {
        if (strcmp(username, fu) == 0 && strcmp(password, fp) == 0) {
            strcpy(currentUser, fu);
            strcpy(currentRole, fr);
            fclose(file);
            printf("\nWelcome %s (%s)\n", currentUser, currentRole);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

/* ================= MAIN MENU ================= */
void mainMenu(void) {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else if (strcmp(currentRole, "USER") == 0)
        userMenu();
    else
        guestMenu();
}

/* ================= ADMIN ================= */
void adminMenu(void) {
    int ch;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Student\n2. Display Students\n3. Search\n4. Update\n5. Delete\n6. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
        }
    } while (ch != 6);
}

/* ================= STAFF ================= */
void staffMenu(void) {
    int ch;
    do {
        printf("\n--- STAFF MENU ---\n");
        printf("1. Display\n2. Search\n3. Update\n4. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
        }
    } while (ch != 4);
}

/* ================= USER ================= */
void userMenu(void) {
    int ch, roll, found;
    struct Student st;
    FILE *fp;

    do {
        printf("\n--- USER MENU ---\n");
        printf("1. View My Record\n2. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        if (ch == 1) {
            printf("Enter your Roll Number: ");
            scanf("%d", &roll);

            fp = fopen(STUDENT_FILE, "r");
            if (!fp) {
                printf("No records found!\n");
                continue;
            }

            found = 0;
            while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
                if (st.roll == roll) {
                    printf("\nROLL : %d\nNAME : %s\nMARKS: %.2f\n",
                           st.roll, st.name, st.marks);
                    found = 1;
                    break;
                }
            }
            if (!found) printf("Record not found!\n");
            fclose(fp);
        }

    } while (ch != 2);
}

/* ================= GUEST ================= */
void guestMenu(void) {
    printf("\n--- GUEST VIEW (READ ONLY) ---\n");
    displayStudents();
    printf("\nGuest session ended.\n");
}

/* ================= UTILS ================= */
int isDuplicateRoll(int roll) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return 0;
    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ================= ADD ================= */
void addStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("File error!\n");
        return;
    }

    printf("Roll: ");
    scanf("%d", &st.roll);
    if (isDuplicateRoll(st.roll)) {
        printf("Roll exists!\n");
        fclose(fp);
        return;
    }

    printf("Name: ");
    scanf("%49s", st.name);
    printf("Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);
    printf("Added successfully!\n");
}

/* ================= DISPLAY ================= */
void displayStudents(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("No data available!\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("-------------------\n");

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3)
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);

    fclose(fp);
}

/* ================= SEARCH ================= */
void searchStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    int roll, found = 0;

    if (!fp) {
        printf("No records!\n");
        return;
    }

    printf("Search Roll: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Found => %d %s %.2f\n",
                   st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found) printf("Not found!\n");
    fclose(fp);
}

/* ================= UPDATE ================= */
void updateStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    int roll, found = 0;

    if (!fp || !tmp) return;

    printf("Roll to Update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("New Name: ");
            scanf("%49s", st.name);
            printf("New Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp); fclose(tmp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Updated!\n");
    else printf("Not found!\n");
}

/* ================= DELETE ================= */
void deleteStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    int roll, found = 0;

    if (!fp || !tmp) return;

    printf("Roll to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp); fclose(tmp);
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Deleted.\n");
    else printf("Not found.\n");
}