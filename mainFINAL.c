#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CONTACTS 100
#define FILE_NAME "contacts.txt"

// Color codes
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

typedef struct {
    char phone[15];
    char name[50];
    char company[50];
    char email[50];
    int blocked;
    int favorite;
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

void saveToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        perror(RED "🚫 Failed to save contacts to file" RESET);
        return;
    }

    for (int i = 0; i < contactCount; i++) {
        fprintf(file, "%s,%s,%s,%s,%d,%d\n",
                contacts[i].phone,
                contacts[i].name,
                contacts[i].company,
                contacts[i].email,
                contacts[i].blocked,
                contacts[i].favorite);
    }
    fclose(file);
    printf(GREEN "✅ Contacts saved to file.\n" RESET);
}

void loadFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf(YELLOW "ℹ️ No existing contact file found. A new one will be created when saving.\n" RESET);
        return;
    }

    while (fscanf(file, "%14[^,],%49[^,],%49[^,],%49[^,],%d,%d\n",
                  contacts[contactCount].phone,
                  contacts[contactCount].name,
                  contacts[contactCount].company,
                  contacts[contactCount].email,
                  &contacts[contactCount].blocked,
                  &contacts[contactCount].favorite) == 6) {
        contactCount++;
    }
    fclose(file);
    printf(GREEN "✅ Loaded %d contacts from file.\n" RESET, contactCount);
}

void readString(char *prompt, char *target, int maxLength) {
    do {
        printf("%s", prompt);
        fgets(target, maxLength, stdin);
        target[strcspn(target, "\n")] = '\0';
    } while (strlen(target) == 0);
}

void addContact() {
    if (contactCount >= MAX_CONTACTS) {
        printf(RED "🚫 Contact list is full!\n" RESET);
        return;
    }

    printf(GREEN "\n✅ Add New Contact\n" RESET);
    getchar(); // Clear leftover newline

    readString("📱 Phone number (max 14 chars): ", contacts[contactCount].phone, 15);
    readString("👤 Name (max 49 chars): ", contacts[contactCount].name, 50);
    readString("🏢 Company (optional): ", contacts[contactCount].company, 50);
    readString("📧 Email (optional): ", contacts[contactCount].email, 50);

    contacts[contactCount].blocked = 0;
    contacts[contactCount].favorite = 0;
    contactCount++;

    saveToFile();
    printf(GREEN "✅ Contact added!\n" RESET);
}

void displayContacts() {
    if (contactCount == 0) {
        printf(YELLOW "ℹ️ No contacts available.\n" RESET);
        return;
    }

    printf(GREEN "\n📇 Contact List:\n" RESET);
    for (int i = 0; i < contactCount; i++) {
        printf("%d. " BLUE "%s" RESET " (" YELLOW "%s" RESET ") - %s - %s %s %s\n",
               i + 1,
               contacts[i].name,
               contacts[i].phone,
               contacts[i].company,
               contacts[i].email,
               contacts[i].blocked ? RED "[BLOCKED]" RESET : "",
               contacts[i].favorite ? GREEN "[FAVORITE]" RESET : "");
    }
}

void deleteContact() {
    int index;
    displayContacts();
    if (contactCount == 0) return;

    printf("\nEnter contact number to delete (1-%d): ", contactCount);
    scanf("%d", &index);
    getchar(); // Clear newline

    if (index < 1 || index > contactCount) {
        printf(RED "🚫 Invalid selection!\n" RESET);
        return;
    }

    printf("🗑️ Delete %s? (y/n): ", contacts[index-1].name);
    char confirm;
    scanf(" %c", &confirm);
    getchar(); // Clear newline

    if (confirm == 'y' || confirm == 'Y') {
        for (int i = index - 1; i < contactCount - 1; i++) {
            contacts[i] = contacts[i + 1];
        }
        contactCount--;
        saveToFile();
        printf(RED "✅ Contact deleted!\n" RESET);
    }
}

void toggleBlock() {
    int index;
    displayContacts();
    if (contactCount == 0) return;

    printf("\nEnter contact number to block/unblock (1-%d): ", contactCount);
    scanf("%d", &index);
    getchar(); // Clear newline

    if (index < 1 || index > contactCount) {
        printf(RED "🚫 Invalid selection!\n" RESET);
        return;
    }

    contacts[index-1].blocked = !contacts[index-1].blocked;
    saveToFile();
    printf("%s %s!\n",
           contacts[index-1].name,
           contacts[index-1].blocked ? RED "is now BLOCKED" RESET : GREEN "is now UNBLOCKED" RESET);
}

void toggleFavorite() {
    int index;
    displayContacts();
    if (contactCount == 0) return;

    printf("\nEnter contact number to favorite/unfavorite (1-%d): ", contactCount);
    scanf("%d", &index);
    getchar(); // Clear newline

    if (index < 1 || index > contactCount) {
        printf(RED "🚫 Invalid selection!\n" RESET);
        return;
    }

    contacts[index-1].favorite = !contacts[index-1].favorite;
    saveToFile();
    printf("%s %s!\n",
           contacts[index-1].name,
           contacts[index-1].favorite ? GREEN "is now FAVORITED" RESET : YELLOW "is now UNFAVORITED" RESET);
}

void menu() {
    int option;
    do {
        printf("\n" BLUE "📱 Contact Manager\n" RESET);
        printf("1. Add Contact\n");
        printf("2. View Contacts\n");
        printf("3. Delete Contact\n");
        printf("4. Block/Unblock\n");
        printf("5. Favorite/Unfavorite\n");
        printf("0. Exit\n");
        printf("Choose: ");

        scanf("%d", &option);
        getchar(); // Clear newline

        switch (option) {
            case 1: addContact(); break;
            case 2: displayContacts(); break;
            case 3: deleteContact(); break;
            case 4: toggleBlock(); break;
            case 5: toggleFavorite(); break;
            case 0: printf(GREEN "👋 Goodbye!\n" RESET); break;
            default: printf(RED "🚫 Invalid choice!\n" RESET);
        }
    } while (option != 0);
}

int main() {
    loadFromFile();
    menu();
    return 0;
}
