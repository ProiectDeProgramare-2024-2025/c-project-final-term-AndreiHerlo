#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_CONTACTS 100
#define FILE_NAME "contacts.txt"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

typedef struct {
    char phone[15];
    char label[50];
    char company[50];
    char email[50];
    int blocked;
    int favorite;
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

void saveToFile();
void loadFromFile();
void addContact();
void displayContacts();
void deleteContact();
void blockContact();
void favoriteContact();
void menu();
int isValidPhone(const char *phone);
int isValidEmail(const char *email);
void clearInputBuffer();
void printHeader(const char *text);

int isValidPhone(const char *phone) {
    if (strlen(phone) < 10 || strlen(phone) > 14) return 0;
    for (int i = 0; phone[i]; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-') {
            return 0;
        }
    }
    return 1;
}

int isValidEmail(const char *email) {
    if (!email || !*email) return 1;
    int atCount = 0, dotCount = 0;
    for (int i = 0; email[i]; i++) {
        if (email[i] == '@') atCount++;
        if (email[i] == '.') dotCount++;
    }
    return (atCount == 1 && dotCount >= 1);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printHeader(const char *text) {
    printf("\n" COLOR_CYAN "=== %s ===\n" COLOR_RESET, text);
}

void saveToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf(COLOR_RED "Error opening file for writing!\n" COLOR_RESET);
        return;
    }
    for (int i = 0; i < contactCount; i++) {
        fprintf(file, "%s,%s,%s,%s,%d,%d\n", 
                contacts[i].phone, 
                contacts[i].label, 
                contacts[i].company, 
                contacts[i].email, 
                contacts[i].blocked, 
                contacts[i].favorite);
    }
    fclose(file);
}

void loadFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;
    while (fscanf(file, "%14[^,],%49[^,],%49[^,],%49[^,],%d,%d\n", 
                  contacts[contactCount].phone, 
                  contacts[contactCount].label, 
                  contacts[contactCount].company, 
                  contacts[contactCount].email, 
                  &contacts[contactCount].blocked, 
                  &contacts[contactCount].favorite) == 6) {
        contactCount++;
    }
    fclose(file);
}

void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf(COLOR_RED "Contact list is full!\n" COLOR_RESET);
        return;
    }
    
    printHeader("Add New Contact");
    
    do {
        printf("Phone number (10-14 digits, can include + or -): ");
        scanf("%14s", contacts[contactCount].phone);
        clearInputBuffer();
        if (!isValidPhone(contacts[contactCount].phone)) {
            printf(COLOR_RED "Invalid phone number format!\n" COLOR_RESET);
        }
    } while (!isValidPhone(contacts[contactCount].phone));
    
    printf("Contact name (max 49 characters): ");
    scanf("%49s", contacts[contactCount].label);
    clearInputBuffer();
    
    printf("Company (optional, max 49 characters): ");
    scanf("%49[^\n]", contacts[contactCount].company);
    clearInputBuffer();
    
    do {
        printf("Email (optional, must contain @ and .): ");
        scanf("%49s", contacts[contactCount].email);
        clearInputBuffer();
        if (!isValidEmail(contacts[contactCount].email)) {
            printf(COLOR_RED "Invalid email format!\n" COLOR_RESET);
        }
    } while (!isValidEmail(contacts[contactCount].email));
    
    contacts[contactCount].blocked = 0;
    contacts[contactCount].favorite = 0;
    contactCount++;
    saveToFile();
    printf(COLOR_GREEN "\nContact added successfully!\n" COLOR_RESET);
}

void displayContacts() {
    if (contactCount == 0) {
        printf(COLOR_YELLOW "No contacts available.\n" COLOR_RESET);
        return;
    }
    
    printHeader("Contact List");
    for (int i = 0; i < contactCount; i++) {
        printf("%d. " COLOR_MAGENTA "%s" COLOR_RESET " (" COLOR_BLUE "%s" COLOR_RESET ") - %s - %s %s %s\n", 
               i + 1,
               contacts[i].label, 
               contacts[i].phone,
               contacts[i].company, 
               contacts[i].email,
               contacts[i].blocked ? COLOR_RED "[BLOCKED]" COLOR_RESET : "",
               contacts[i].favorite ? COLOR_YELLOW "[FAVORITE]" COLOR_RESET : "");
    }
}

void deleteContact() {
    int index;
    
    displayContacts();
    if (contactCount == 0) return;
    
    printf("\nEnter contact number to delete (1-%d): ", contactCount);
    scanf("%d", &index);
    clearInputBuffer();
    
    if (index < 1 || index > contactCount) {
        printf(COLOR_RED "Invalid selection!\n" COLOR_RESET);
        return;
    }
    
    printf(COLOR_YELLOW "Are you sure you want to delete " COLOR_MAGENTA "%s" COLOR_YELLOW "? (y/n): " COLOR_RESET, contacts[index-1].label);
    char confirm;
    scanf("%c", &confirm);
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        for (int i = index - 1; i < contactCount - 1; i++) {
            contacts[i] = contacts[i + 1];
        }
        contactCount--;
        saveToFile();
        printf(COLOR_GREEN "Contact deleted successfully!\n" COLOR_RESET);
    } else {
        printf(COLOR_BLUE "Deletion canceled.\n" COLOR_RESET);
    }
}

void blockContact() {
    int index;
    
    displayContacts();
    if (contactCount == 0) return;
    
    printf("\nEnter contact number to block (1-%d): ", contactCount);
    scanf("%d", &index);
    clearInputBuffer();
    
    if (index < 1 || index > contactCount) {
        printf(COLOR_RED "Invalid selection!\n" COLOR_RESET);
        return;
    }
    
    if (contacts[index - 1].blocked) {
        printf(COLOR_YELLOW "Contact is already blocked!\n" COLOR_RESET);
        return;
    }
    
    contacts[index - 1].blocked = 1;
    saveToFile();
    printf(COLOR_GREEN "Contact blocked successfully!\n" COLOR_RESET);
}

void favoriteContact() {
    int index;
    
    displayContacts();
    if (contactCount == 0) return;
    
    printf("\nEnter contact number to mark as favorite (1-%d): ", contactCount);
    scanf("%d", &index);
    clearInputBuffer();
    
    if (index < 1 || index > contactCount) {
        printf(COLOR_RED "Invalid selection!\n" COLOR_RESET);
        return;
    }
    
    if (contacts[index - 1].favorite) {
        printf(COLOR_YELLOW "Contact is already marked as favorite!\n" COLOR_RESET);
        return;
    }
    
    contacts[index - 1].favorite = 1;
    saveToFile();
    printf(COLOR_GREEN "Contact marked as favorite!\n" COLOR_RESET);
}

void menu() {
    int option;
    do {
        printHeader("Contact Management System");
        printf(COLOR_CYAN "1." COLOR_RESET " Add Contact\n");
        printf(COLOR_CYAN "2." COLOR_RESET " Display Contacts\n");
        printf(COLOR_CYAN "3." COLOR_RESET " Delete Contact\n");
        printf(COLOR_CYAN "4." COLOR_RESET " Block Contact\n");
        printf(COLOR_CYAN "5." COLOR_RESET " Mark as Favorite\n");
        printf(COLOR_CYAN "0." COLOR_RESET " Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &option) != 1) {
            printf(COLOR_RED "\nInvalid input! Please enter a number.\n" COLOR_RESET);
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        switch (option) {
            case 1: addContact(); break;
            case 2: displayContacts(); break;
            case 3: deleteContact(); break;
            case 4: blockContact(); break;
            case 5: favoriteContact(); break;
            case 0: printf(COLOR_GREEN "\nExiting...\n" COLOR_RESET); break;
            default: printf(COLOR_RED "\nInvalid option!\n" COLOR_RESET);
        }
    } while (option != 0);
}

int main() {
    loadFromFile();
    menu();
    return 0;
}