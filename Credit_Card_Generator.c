#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int get_length(const char *prefix) {
    if (strncmp(prefix, "34", 2) == 0 || strncmp(prefix, "37", 2) == 0)
        return 15;
    if (prefix[0] == '4')
        return 16;
    if (prefix[0] == '5' && prefix[1] >= '1' && prefix[1] <= '5')
        return 16;
    return 16;
}

const char* get_card_type(const char *prefix) {
    if (strncmp(prefix, "34", 2) == 0 || strncmp(prefix, "37", 2) == 0)
        return "American Express";
    if (prefix[0] == '4')
        return "Visa";
    if (prefix[0] == '5' && prefix[1] >= '1' && prefix[1] <= '5')
        return "MasterCard";
    return "Unknown";
}

int calculate_luhn(int digits[], int length) {
    int sum = 0, alternate = 1;

    for (int i = length - 2; i >= 0; i--) {
        int n = digits[i];
        if (alternate) {
            n *= 2;
            if (n > 9)
                n -= 9;
        }
        sum += n;
        alternate = !alternate;
    }
    return (10 - (sum % 10)) % 10;
}

void generate_cvv(const char *type, char *cvv) {
    int length = (strcmp(type, "American Express") == 0) ? 4 : 3;
    for (int i = 0; i < length; i++)
        cvv[i] = (rand() % 10) + '0';
    cvv[length] = '\0';
}

void generate_expiry(char *expiry) {
    int month = rand() % 12 + 1;
    int add_year = rand() % 5 + 1;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    int year = (tm_info->tm_year + 1900 + add_year) % 100;
    sprintf(expiry, "%02d/%02d", month, year);
}

void show_previous_cards() {
    FILE *file = fopen("cards.txt", "r");
    char ch;

    if (file == NULL) {
        printf("\nNo card history found.\n");
        return;
    }

    printf("\n--- Card History ---\n");
    while ((ch = fgetc(file)) != EOF)
        putchar(ch);

    fclose(file);
}

void clear_history() {
    FILE *file = fopen("cards.txt", "w");
    if (file == NULL) {
        printf("Error clearing history.\n");
        return;
    }
    fclose(file);
    printf("\nCard history cleared successfully.\n");
}

void generate_new_card() {
    char prefix[5];
    int digits[20], length;
    char cvv[5], expiry[6];
    FILE *file;

    printf("Enter first 4 digits of card: ");
    scanf("%4s", prefix);

    length = get_length(prefix);

    for (int i = 0; i < 4; i++)
        digits[i] = prefix[i] - '0';

    for (int i = 4; i < length - 1; i++)
        digits[i] = rand() % 10;

    digits[length - 1] = calculate_luhn(digits, length);

    const char *type = get_card_type(prefix);
    generate_cvv(type, cvv);
    generate_expiry(expiry);

    printf("\nCard Type: %s\n", type);
    printf("Card Number: ");
    for (int i = 0; i < length; i++)
        printf("%d", digits[i]);
    printf("\nCVV: %s\n", cvv);
    printf("Expiry Date: %s\n", expiry);

    file = fopen("cards.txt", "a");
    if (file == NULL) {
        printf("Error saving card.\n");
        return;
    }

    fprintf(file, "Card Type: %s\n", type);
    fprintf(file, "Card Number: ");
    for (int i = 0; i < length; i++)
        fprintf(file, "%d", digits[i]);
    fprintf(file, "\nCVV: %s\n", cvv);
    fprintf(file, "\nExpiry Date: %s\n", expiry);
    fprintf(file, "\n-------------------------\n");

    fclose(file);
}

int main() {
    int choice;
    srand(time(NULL));

    do {
        printf("\nCredit Card Generator Menu\n");
        printf("1. Show previous cards\n");
        printf("2. Generate new card\n");
        printf("3. Clear card history\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                show_previous_cards();
                break;
            case 2:
                generate_new_card();
                break;
            case 3:
                clear_history();
                break;
            case 4:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 4);

    return 0;
}
