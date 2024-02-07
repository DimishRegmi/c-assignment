#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maximum legth
#define MAX_LEN 50

// Function prototypes
void welcome();
int authenticate(char *accNum, char *pin);
float checkBalance(char *accNum);
void deposit(char *accNum, float amount);
void withdraw(char *accNum, float amount);
void updateFile(char *accNum, float newBalance);

int main() {
    welcome();

    char accNum[MAX_LEN];
    char pin[MAX_LEN];
    float amount;
    int choice;

    printf("\nEnter your acc num: ");
    scanf("%s", accNum);

    if (authenticate(accNum, pin)) {
        do {
            printf("\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Your current balance is: %.2f\n", checkBalance(accNum));
                    break;
                case 2:
                    printf("Enter amount to deposit: ");
                    scanf("%f", &amount);
                    deposit(accNum, amount);
                    printf("Deposit successful.\n");
                    break;
                case 3:
                    printf("Enter amount to withdraw: ");
                    scanf("%f", &amount);
                    withdraw(accNum, amount);
                    printf("Withdrawal successful.\n");
                    break;
                case 4:
                    printf("Exiting...\n");
                    exit(0);
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (1);
    } else {
        printf("Account number or PIN incorrect. Exiting...\n");
    }

    return 0;
}

void welcome() {
    printf("Welcome to the ATM Machine!\n");
}

int authenticate(char *accNum, char *pin) {
    char fileAccNum[MAX_LEN];
    char filePin[MAX_LEN];
    float balance;
    FILE *file = fopen("accounts.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fscanf(file, "%s %s %f", fileAccNum, filePin, &balance) != EOF) {
        if (strcmp(accNum, fileAccNum) == 0) {
            printf("Enter your PIN: ");
            scanf("%s", pin);
            if (strcmp(pin, filePin) == 0) {
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

float checkBalance(char *accNum) {
    char fileAccNum[MAX_LEN];
    char pin[MAX_LEN];
    float balance;
    FILE *file = fopen("accounts.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fscanf(file, "%s %s %f", fileAccNum, pin, &balance) != EOF) {
        if (strcmp(accNum, fileAccNum) == 0) {
            fclose(file);
            return balance;
        }
    }

    fclose(file);
    return -1;
}

void deposit(char *accNum, float amount) {
    char fileAccNum[MAX_LEN];
    char pin[MAX_LEN];
    float balance;
    FILE *file = fopen("accounts.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fscanf(file, "%s %s %f", fileAccNum, pin, &balance) != EOF) {
        if (strcmp(accNum, fileAccNum) == 0) {
            balance += amount;
        }
        fprintf(tempFile, "%s %s %.2f\n", fileAccNum, pin, balance);
    }

    fclose(file);
    fclose(tempFile);
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

void withdraw(char *accNum, float amount) {
    char fileAccNum[MAX_LEN];
    char pin[MAX_LEN];
    float balance;
    FILE *file = fopen("accounts.txt", "r+");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fscanf(file, "%s %s %f", fileAccNum, pin, &balance) != EOF) {
        if (strcmp(accNum, fileAccNum) == 0) {
            if (balance >= amount) {
                balance -= amount;
            } else {
                printf("Insufficient balance.\n");
                return;
            }
        }
        fprintf(tempFile, "%s %s %.2f\n", fileAccNum, pin, balance);
    }

    fclose(file);
    fclose(tempFile);
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}