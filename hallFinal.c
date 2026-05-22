#include <stdio.h>
#include <string.h>

void handleResidential();
void handleNonResidential();
void saveToFile(char *type, char *id, int time, char *status);
void adminPanel();

int main() {
    int choice;
    printf("--- Welcome to Hall Management System ---\n");
    printf("Select Option:\n");
    printf("1. Residential Student Entry\n");
    printf("2. Non-residential Student Entry\n");
    printf("3. Admin Panel (View Database)\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            handleResidential();
            break;
        case 2:
            handleNonResidential();
            break;
        case 3:
            adminPanel();
            break;
        default:
            printf("\nInvalid choice! Please restart the program.\n");
    }
    return 0;
}

void handleResidential() {
    char hallID[20];
    int entryTime;
    char hasID[10], hasRegister[10], validReason[10];
    
    printf("\n--- Residential Student Entry ---\n");
    printf("Enter your Hall ID: ");
    scanf("%s", hallID);
    printf("Enter entry time (24-hour format): ");
    scanf("%d", &entryTime);

    if (entryTime <= 22) {
        printf("\nWelcome! Hall ID: %s\n", hallID);
        saveToFile("Residential", hallID, entryTime, "Normal Entry");
    } 
    else {
        printf("\nIt's late! Answer (yes/no):\n");
        printf("1. ID card? "); scanf("%s", hasID);
        printf("2. Valid reason? "); scanf("%s", validReason);
        printf("3. Signed Register? "); scanf("%s", hasRegister);

        if (strcmp(hasID, "yes") == 0 && strcmp(validReason, "yes") == 0 && strcmp(hasRegister, "yes") == 0) {
            
            FILE *fptr = fopen("database.txt", "r");
            int lateCount = 0;
            
            if (fptr != NULL) {
                char line[250];
                while (fgets(line, sizeof(line), fptr) != NULL) {
                    if (strstr(line, hallID) != NULL && strstr(line, "Late Entry Approved") != NULL) {
                        lateCount++;
                    }
                }
                fclose(fptr);
            }

            printf("\nAccess Granted (Late Entry).\n");
            
            lateCount++; 
            printf("-> This is your late entry number: %d\n", lateCount);

            if (lateCount > 3) {
                printf("PENALTY WARNING: You have crossed the 3-time late entry limit!\n");
                printf("Action: An automated Late Entry Notification has been sent to your Parent/Guardian.\n");
                
                saveToFile("Residential", hallID, entryTime, "Late Entry Approved (Guardian Warned)");
            } else {
                saveToFile("Residential", hallID, entryTime, "Late Entry Approved");
            }
        } 
        else {
            printf("\nAccess Denied!\n");
            saveToFile("Residential", hallID, entryTime, "Late Entry Denied");
        }
    }
}

void handleNonResidential() {
    char hasID[10];
    int subChoice;
    printf("\n--- Non-residential Student Entry ---\n");
    printf("Do you have ID card? (yes/no): ");
    scanf("%s", hasID);

    if (strcmp(hasID, "yes") == 0) {
        printf("1. Written Permission\n2. Guardian's Consent\nChoice: ");
        scanf("%d", &subChoice);
        if (subChoice == 1) {
            printf("Submit letter to guard.\n");
            saveToFile("Non-Residential", "N/A", 0, "Permission Letter Entry");
        } 
        else if (subChoice == 2) {
            printf("Guardian verification required.\n");
            saveToFile("Non-Residential", "N/A", 0, "Guardian Consent Entry");
        }
    } 
    else {
        printf("\nAccess Denied! No ID.\n");
        saveToFile("Non-Residential", "Guest", 0, "Denied - No ID");
    }
}

void saveToFile(char *type, char *id, int time, char *status) {
    FILE *fptr;
    fptr = fopen("database.txt", "a");
    if (fptr == NULL) {
        printf("\nError: Could not create or open the database!\n");
        return;
    }
    fprintf(fptr, "Type: %s | ID: %s | Time: %d | Status: %s\n", type, id, time, status);
    fclose(fptr);
    printf("[Data saved to system database]\n");
}

void adminPanel() {
    char password[20];
    
    printf("\n--- Admin Login ---\n");
    printf("Enter Admin Password: ");
    scanf("%s", password);

    if (strcmp(password, "admin123") == 0) {
        printf("\nLogin Successful!\n");
        
        FILE *fptr;
        char line[250];
        int count = 0;
        
        fptr = fopen("database.txt", "r");
        if (fptr == NULL) {
            printf("No records found in database.\n");
            return;
        }
        
        while (fgets(line, sizeof(line), fptr) != NULL) {
            count++;
        }
        
        rewind(fptr);
        
        printf("===========================================\n");
        printf("  Total Students Entered Today: %d\n", count);
        printf("===========================================\n");
        printf("\n--- Student Entry Database Records ---\n");
        
        while (fgets(line, sizeof(line), fptr) != NULL) {
            printf("%s", line);
        }
        
        fclose(fptr);
        printf("-------------------------------------------\n");
    } 
    else {
        printf("\nIncorrect Password! Access Denied.\n");
    }
}
