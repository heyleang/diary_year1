#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to remove newline character from a string
void removeNewline(char* str) {
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';  // Remove the newline character
    }
}

// Function to edit a word in a diary entry
void editWord(char* fileName, char* targetWord, char* replacementWord) {
    FILE* file = fopen(fileName, "r+");
    if (file == NULL) {
        printf("Error: Diary entry not found.\n");
        return;
    }

    char buffer[1000];
    long currentPosition;

    while (fgets(buffer, sizeof(buffer), file)) {
        char* found = buffer;
        size_t targetLen = strlen(targetWord);

        while ((found = strstr(found, targetWord)) != NULL) {
            // Check if the target word is a whole word
            if ((found == buffer || !isalpha(found[-1])) && (!isalpha(found[targetLen]) || found[targetLen] == '\0')) {
                // Store the current position in the file
                currentPosition = ftell(file);
                // Move the file pointer to the beginning of the found position
                fseek(file, currentPosition - (strlen(buffer) - (found - buffer)), SEEK_SET);
                // Write characters before the target word
                fwrite(buffer, 1, found - buffer, file);
                // Write the replacement word
                fprintf(file, "%s", replacementWord);
                // Move the file pointer back to the original position
                fseek(file, currentPosition, SEEK_SET);
                // Move the pointer past the target word in the original buffer
                found += targetLen;
            } else {
                // If the target word is not a whole word, simply move to the next character
                found++;
            }
        }
    }

    fclose(file);

    // Remove newline characters from file names
    removeNewline(fileName);

    printf("Word edited successfully.\n");
}


// Function to create a new diary entry
void createDiary(char* fileName, char* content) {
    // Check if the file already exists
    FILE* fileCheck = fopen(fileName, "r");
    if (fileCheck != NULL) {
        fclose(fileCheck);
        printf("Error: Diary entry already exists. Choose a different file name.\n");
        return;
    }

    // File doesn't exist, create a new one
    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error: Unable to create the diary entry.\n");
        return;
    }

    fprintf(file, "%s\n", content);
    fclose(file);
    printf("Diary entry created successfully.\n");
}

// Function to read a diary entry
void readDiary(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error: Diary entry not found.\n");
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

// Function to update an existing diary entry
void updateDiary(char* fileName, char* content) {
    // Check if the file exists
    FILE* fileCheck = fopen(fileName, "r");
    if (fileCheck == NULL) {
        printf("Error: Diary entry not found. Cannot update.\n");
        return;
    }
    fclose(fileCheck);

    // File exists, open in write mode to overwrite the content
    FILE* file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error: Unable to update the diary entry.\n");
        return;
    }

    fprintf(file, "%s\n", content);
    fclose(file);
    printf("Diary entry updated successfully.\n");
}

// Function to delete a diary entry
void deleteDiary(char* fileName) {
    if (remove(fileName) == 0) {
        printf("Diary entry deleted successfully.\n");
    } else {
        printf("Error: Unable to delete the diary entry.\n");
    }
}


int main() {
    char fileName[100];
    int choice;

    while (1) {
        printf("\nDiary Management System\n");
        printf("1. Create Diary Entry\n");
        printf("2. Read Diary Entry\n");
        printf("3. Update Diary Entry\n");
        printf("4. Delete Diary Entry\n");
        printf("5. Edit Word in Diary Entry\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter file name: ");
                getchar();  // Consume newline character
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strlen(fileName) - 1] = '\0'; // Remove the newline character

                printf("Enter diary content. Enter 'END' on a new line to finish:\n");
                char content[1000];
                strcpy(content, "");  // Initialize content with an empty string

                while (1) {
                    char paragraph[1000];
                    fgets(paragraph, sizeof(paragraph), stdin);
                    if (strcmp(paragraph, "END\n") == 0) {
                        break;  // Stop entering paragraphs when "END" is entered
                    }
                    strcat(content, paragraph);  // Append the paragraph to content
                }   
                createDiary(fileName, content);
                break;

            case 2:
                printf("Enter file name to read: ");
                getchar();  // Consume newline character
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strlen(fileName) - 1] = '\0'; // Remove the newline character
                readDiary(fileName);
                break;

            case 3:
                printf("Enter file name to update: ");
                getchar();  // Consume newline character
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strlen(fileName) - 1] = '\0'; // Remove the newline character

                printf("Enter new diary content. Enter 'END' on a new line to finish:\n");
                char newContent[1000];
                strcpy(newContent, "");  // Initialize newContent with an empty string

                while (1) {
                    char paragraph[1000];
                    fgets(paragraph, sizeof(paragraph), stdin);
                    if (strcmp(paragraph, "END\n") == 0) {
                        break;  // Stop entering paragraphs when "END" is entered
                    }
                    strcat(newContent, paragraph);  // Append the paragraph to newContent
                }
                updateDiary(fileName, newContent);
                break;

            case 4:
                printf("Enter file name to delete: ");
                getchar();  // Consume newline character
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strlen(fileName) - 1] = '\0'; // Remove the newline character
                deleteDiary(fileName);
                break;
            case 5:
                printf("Enter file name to edit: ");
                getchar();  // Consume newline character
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strlen(fileName) - 1] = '\0'; // Remove the newline character

                char targetWord[50], replacementWord[50];
                printf("Enter the word to be replaced: ");
                scanf("%s", targetWord);
                printf("Enter the replacement word: ");
                scanf("%s", replacementWord);

                editWord(fileName, targetWord, replacementWord);
                break;

            case 6:
                printf("Exiting the Diary Management System.\n");
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
