// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.
#include <stdio.h>
#include <stdlib.h>

// structure
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchRecord(FILE *fPtr);
void displayAll(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;
    unsigned int choice;

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File not found, creating new file...\n");

        // create file if not exists
        cfPtr = fopen("credit.dat", "wb+");

        struct clientData blank = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blank, sizeof(struct clientData), 1, cfPtr);
        }
    }

    // menu loop
        while (1){
               
             choice = enterChoice();

             switch (choice)
            {
                case 1:
                    textFile(cfPtr);
                    break;

                case 2:
                    updateRecord(cfPtr);
                    break;

                case 3:
                    newRecord(cfPtr);
                    break;

                case 4:
                    deleteRecord(cfPtr);
                    break;

                case 5:
                    searchRecord(cfPtr);
                    break;

                case 6:
                    displayAll(cfPtr);
                    break;

                case 7:
                    printf("Exiting program...\n");
                    fclose(cfPtr); return 0;

                default:
                printf("Invalid choice! Try again.\n");
            }
        }
    }
// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1 - Save accounts to text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Search account\n");
    printf("6 - Display all accounts\n");
    printf("7 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}

// create text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("Cannot create file.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    printf("accounts.txt created successfully!\n");
}

// update record
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("Current Balance: %.2f\n", client.balance);

        printf("Enter amount (+deposit / -withdraw): ");
        scanf("%lf", &transaction);

        if (client.balance + transaction < 0)
        {
            printf("Insufficient balance!\n");
            return;
        }

        client.balance += transaction;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Updated Balance: %.2f\n", client.balance);
    }
}

// add new record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter new account number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists!\n");
    }
    else
    {
        printf("Enter last name, first name, balance:\n");
        scanf("%14s %9s %lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = account;

        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Account created successfully!\n");
    }
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter account number to delete: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);

        printf("Account deleted successfully.\n");
    }
}

// search record
void searchRecord(FILE *fPtr)
{
    unsigned int account;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Details:\n");
        printf("Account No: %d\n", client.acctNum);
        printf("Name: %s %s\n", client.firstName, client.lastName);
        printf("Balance: %.2f\n", client.balance);
    }
}

// display all
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\nAll Accounts:\n");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-5d %-10s %-10s %.2f\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance);
        }
    }
}