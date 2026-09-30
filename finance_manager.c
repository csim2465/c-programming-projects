#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TRANSACTIONS 500
#define DESC_LENGTH 80
#define DATE_LENGTH 11
#define DATA_FILE "transactions.dat"
#define SETTINGS_FILE "finance_settings.dat"

/* =========================================================
                         DATA STRUCTURE
   ========================================================= */

typedef struct
{
    int id;
    char description[DESC_LENGTH];
    double amount;
    int type;       /* 1 = Income, 2 = Expense */
    int category;   /* 1-6 for expenses */
    char date[DATE_LENGTH];
} Transaction;


/* =========================================================
                      FUNCTION PROTOTYPES
   ========================================================= */

void displayHeader(void);
void displayMenu(void);

void addIncome(Transaction transactions[], int *count, int *nextId);
void addExpense(Transaction transactions[], int *count, int *nextId);

void viewTransactions(const Transaction transactions[], int count);
void viewSummary(const Transaction transactions[],
                 int count,
                 double monthlyBudget);
void viewCategoryReport(const Transaction transactions[], int count);

void searchTransactions(const Transaction transactions[], int count);
void editTransaction(Transaction transactions[], int count);
void deleteTransaction(Transaction transactions[], int *count);
void sortTransactionsByAmount(const Transaction transactions[], int count);

void setBudget(double *monthlyBudget);

const char *getCategoryName(int category);
const char *getTypeName(int type);

void saveTransactions(const Transaction transactions[], int count);
void loadTransactions(Transaction transactions[], int *count, int *nextId);

void saveSettings(double monthlyBudget);
void loadSettings(double *monthlyBudget);

int getInt(const char *prompt, int min, int max);
double getDouble(const char *prompt, double min);
void getString(const char *prompt, char text[], int size);

void getDate(const char *prompt, char date[]);
int isValidDate(const char *date);

void pauseProgram(void);


/* =========================================================
                            MAIN
   ========================================================= */

int main(void)
{
    Transaction transactions[MAX_TRANSACTIONS];

    int transactionCount = 0;
    int nextId = 1;
    int choice;

    double monthlyBudget = 0.0;

    loadTransactions(transactions,
                     &transactionCount,
                     &nextId);

    loadSettings(&monthlyBudget);

    do
    {
        displayHeader();
        displayMenu();

        choice = getInt("Select an option: ", 1, 11);

        switch (choice)
        {
            case 1:
                addIncome(transactions,
                          &transactionCount,
                          &nextId);

                saveTransactions(transactions,
                                 transactionCount);
                break;

            case 2:
                addExpense(transactions,
                           &transactionCount,
                           &nextId);

                saveTransactions(transactions,
                                 transactionCount);
                break;

            case 3:
                viewTransactions(transactions,
                                 transactionCount);
                break;

            case 4:
                viewSummary(transactions,
                            transactionCount,
                            monthlyBudget);
                break;

            case 5:
                viewCategoryReport(transactions,
                                   transactionCount);
                break;

            case 6:
                searchTransactions(transactions,
                                   transactionCount);
                break;

            case 7:
                editTransaction(transactions,
                                transactionCount);

                saveTransactions(transactions,
                                 transactionCount);
                break;

            case 8:
                deleteTransaction(transactions,
                                  &transactionCount);

                saveTransactions(transactions,
                                 transactionCount);
                break;

            case 9:
                sortTransactionsByAmount(transactions,
                                         transactionCount);
                break;

            case 10:
                setBudget(&monthlyBudget);
                saveSettings(monthlyBudget);
                break;

            case 11:
                saveTransactions(transactions,
                                 transactionCount);

                saveSettings(monthlyBudget);

                printf("\n");
                printf("============================================================\n");
                printf("                  DATA SAVED SUCCESSFULLY\n");
                printf("============================================================\n");
                printf("Thank you for using Personal Finance Manager!\n");
                printf("============================================================\n");
                break;

            default:
                break;
        }

        if (choice != 11)
        {
            pauseProgram();
        }

    } while (choice != 11);

    return 0;
}


/* =========================================================
                         MAIN DISPLAY
   ========================================================= */

void displayHeader(void)
{
    printf("\n");
    printf("============================================================\n");
    printf("                 PERSONAL FINANCE MANAGER\n");
    printf("============================================================\n");
}

void displayMenu(void)
{
    printf("  1. Add Income\n");
    printf("  2. Add Expense\n");
    printf("  3. View All Transactions\n");
    printf("  4. Financial Dashboard\n");
    printf("  5. Category Spending Report\n");
    printf("  6. Search Transactions\n");
    printf("  7. Edit Transaction\n");
    printf("  8. Delete Transaction\n");
    printf("  9. Sort Transactions by Amount\n");
    printf(" 10. Set Monthly Budget\n");
    printf(" 11. Save & Exit\n");

    printf("------------------------------------------------------------\n");
}


/* =========================================================
                         ADD INCOME
   ========================================================= */

void addIncome(Transaction transactions[],
               int *count,
               int *nextId)
{
    Transaction newTransaction;

    if (*count >= MAX_TRANSACTIONS)
    {
        printf("\nTransaction storage is full.\n");
        return;
    }

    printf("\n");
    printf("============================================================\n");
    printf("                         ADD INCOME\n");
    printf("============================================================\n");

    newTransaction.id = *nextId;

    getString("Description: ",
              newTransaction.description,
              DESC_LENGTH);

    newTransaction.amount =
        getDouble("Amount: $", 0.01);

    getDate("Date (MM/DD/YYYY): ",
            newTransaction.date);

    newTransaction.type = 1;
    newTransaction.category = 0;

    transactions[*count] = newTransaction;

    (*count)++;
    (*nextId)++;

    printf("\nIncome added successfully!\n");
    printf("Transaction ID: %d\n",
           newTransaction.id);
}


/* =========================================================
                         ADD EXPENSE
   ========================================================= */

void addExpense(Transaction transactions[],
                int *count,
                int *nextId)
{
    Transaction newTransaction;

    if (*count >= MAX_TRANSACTIONS)
    {
        printf("\nTransaction storage is full.\n");
        return;
    }

    printf("\n");
    printf("============================================================\n");
    printf("                         ADD EXPENSE\n");
    printf("============================================================\n");

    newTransaction.id = *nextId;

    getString("Description: ",
              newTransaction.description,
              DESC_LENGTH);

    newTransaction.amount =
        getDouble("Amount: $", 0.01);

    getDate("Date (MM/DD/YYYY): ",
            newTransaction.date);

    printf("\n");
    printf("Expense Categories\n");
    printf("------------------------------\n");
    printf("1. Food\n");
    printf("2. Transportation\n");
    printf("3. Entertainment\n");
    printf("4. Shopping\n");
    printf("5. Bills\n");
    printf("6. Other\n");
    printf("------------------------------\n");

    newTransaction.category =
        getInt("Select category: ", 1, 6);

    newTransaction.type = 2;

    transactions[*count] = newTransaction;

    (*count)++;
    (*nextId)++;

    printf("\nExpense added successfully!\n");
    printf("Transaction ID: %d\n",
           newTransaction.id);
    printf("Category: %s\n",
           getCategoryName(newTransaction.category));
}


/* =========================================================
                      VIEW TRANSACTIONS
   ========================================================= */

void viewTransactions(const Transaction transactions[],
                      int count)
{
    if (count == 0)
    {
        printf("\nNo transactions have been recorded yet.\n");
        return;
    }

    printf("\n");
    printf("========================================================================================\n");
    printf("                                  TRANSACTION HISTORY\n");
    printf("========================================================================================\n");

    printf("%-5s %-12s %-10s %-25s %-17s %12s\n",
           "ID",
           "DATE",
           "TYPE",
           "DESCRIPTION",
           "CATEGORY",
           "AMOUNT");

    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        const char *category;

        if (transactions[i].type == 1)
        {
            category = "-";
        }
        else
        {
            category =
                getCategoryName(transactions[i].category);
        }

        printf("%-5d %-12s %-10s %-25.25s %-17s $%11.2f\n",
               transactions[i].id,
               transactions[i].date,
               getTypeName(transactions[i].type),
               transactions[i].description,
               category,
               transactions[i].amount);
    }

    printf("========================================================================================\n");
}


/* =========================================================
                      FINANCIAL DASHBOARD
   ========================================================= */

void viewSummary(const Transaction transactions[],
                 int count,
                 double monthlyBudget)
{
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

    double largestExpense = 0.0;

    char largestExpenseName[DESC_LENGTH] = "None";

    int incomeCount = 0;
    int expenseCount = 0;

    for (int i = 0; i < count; i++)
    {
        if (transactions[i].type == 1)
        {
            totalIncome += transactions[i].amount;
            incomeCount++;
        }
        else if (transactions[i].type == 2)
        {
            totalExpenses += transactions[i].amount;
            expenseCount++;

            if (transactions[i].amount > largestExpense)
            {
                largestExpense =
                    transactions[i].amount;

                strcpy(largestExpenseName,
                       transactions[i].description);
            }
        }
    }

    double balance =
        totalIncome - totalExpenses;

    double savingsRate = 0.0;

    if (totalIncome > 0.0)
    {
        savingsRate =
            (balance / totalIncome) * 100.0;
    }

    printf("\n");
    printf("============================================================\n");
    printf("                    FINANCIAL DASHBOARD\n");
    printf("============================================================\n");

    printf("Total Income:                         $%10.2f\n",
           totalIncome);

    printf("Total Expenses:                       $%10.2f\n",
           totalExpenses);

    printf("Current Balance:                      $%10.2f\n",
           balance);

    printf("------------------------------------------------------------\n");

    printf("Income Transactions:                  %10d\n",
           incomeCount);

    printf("Expense Transactions:                 %10d\n",
           expenseCount);

    printf("Savings Rate:                         %9.1f%%\n",
           savingsRate);

    printf("------------------------------------------------------------\n");

    if (largestExpense > 0.0)
    {
        printf("Largest Expense: %s ($%.2f)\n",
               largestExpenseName,
               largestExpense);
    }
    else
    {
        printf("Largest Expense: None\n");
    }

    if (monthlyBudget > 0.0)
    {
        double remaining =
            monthlyBudget - totalExpenses;

        double percentageUsed =
            (totalExpenses / monthlyBudget) * 100.0;

        printf("\n");
        printf("MONTHLY BUDGET\n");
        printf("------------------------------------------------------------\n");

        printf("Budget:                               $%10.2f\n",
               monthlyBudget);

        printf("Spent:                                $%10.2f\n",
               totalExpenses);

        printf("Remaining:                            $%10.2f\n",
               remaining);

        printf("Budget Used:                          %9.1f%%\n",
               percentageUsed);

        printf("\nBudget Status: ");

        if (percentageUsed > 100.0)
        {
            printf("OVER BUDGET\n");
        }
        else if (percentageUsed >= 90.0)
        {
            printf("WARNING - Budget almost reached\n");
        }
        else if (percentageUsed >= 75.0)
        {
            printf("CAUTION - Monitor spending\n");
        }
        else
        {
            printf("WITHIN BUDGET\n");
        }
    }
    else
    {
        printf("\nMonthly Budget: Not configured\n");
    }

    printf("============================================================\n");
}


/* =========================================================
                     CATEGORY REPORT
   ========================================================= */

void viewCategoryReport(const Transaction transactions[],
                        int count)
{
    double categoryTotals[7] = {0.0};
    double totalExpenses = 0.0;

    for (int i = 0; i < count; i++)
    {
        if (transactions[i].type == 2)
        {
            int category =
                transactions[i].category;

            if (category >= 1 && category <= 6)
            {
                categoryTotals[category] +=
                    transactions[i].amount;

                totalExpenses +=
                    transactions[i].amount;
            }
        }
    }

    printf("\n");
    printf("============================================================\n");
    printf("                  CATEGORY SPENDING REPORT\n");
    printf("============================================================\n");

    if (totalExpenses <= 0.0)
    {
        printf("No expense data available.\n");
        printf("============================================================\n");
        return;
    }

    int largestCategory = 1;

    for (int i = 1; i <= 6; i++)
    {
        double percentage =
            (categoryTotals[i] / totalExpenses) * 100.0;

        printf("%-20s $%10.2f     %6.1f%%\n",
               getCategoryName(i),
               categoryTotals[i],
               percentage);

        if (categoryTotals[i] >
            categoryTotals[largestCategory])
        {
            largestCategory = i;
        }
    }

    printf("------------------------------------------------------------\n");

    printf("Total Expenses:       $%10.2f\n",
           totalExpenses);

    printf("Highest Category:     %s\n",
           getCategoryName(largestCategory));

    printf("============================================================\n");
}


/* =========================================================
                       SEARCH
   ========================================================= */

void searchTransactions(const Transaction transactions[],
                        int count)
{
    if (count == 0)
    {
        printf("\nNo transactions available to search.\n");
        return;
    }

    char searchTerm[DESC_LENGTH];
    char searchLower[DESC_LENGTH];

    int found = 0;

    getString("\nEnter description to search: ",
              searchTerm,
              DESC_LENGTH);

    strcpy(searchLower, searchTerm);

    for (int i = 0; searchLower[i] != '\0'; i++)
    {
        searchLower[i] =
            (char)tolower((unsigned char)searchLower[i]);
    }

    printf("\n");
    printf("============================================================\n");
    printf("                       SEARCH RESULTS\n");
    printf("============================================================\n");

    for (int i = 0; i < count; i++)
    {
        char descriptionLower[DESC_LENGTH];

        strcpy(descriptionLower,
               transactions[i].description);

        for (int j = 0;
             descriptionLower[j] != '\0';
             j++)
        {
            descriptionLower[j] =
                (char)tolower(
                    (unsigned char)descriptionLower[j]);
        }

        if (strstr(descriptionLower,
                   searchLower) != NULL)
        {
            printf("ID:          %d\n",
                   transactions[i].id);

            printf("Date:        %s\n",
                   transactions[i].date);

            printf("Description: %s\n",
                   transactions[i].description);

            printf("Type:        %s\n",
                   getTypeName(transactions[i].type));

            printf("Amount:      $%.2f\n",
                   transactions[i].amount);

            if (transactions[i].type == 2)
            {
                printf("Category:    %s\n",
                       getCategoryName(
                           transactions[i].category));
            }

            printf("------------------------------------------------------------\n");

            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching transactions found.\n");
    }

    printf("============================================================\n");
}


/* =========================================================
                         EDIT
   ========================================================= */

void editTransaction(Transaction transactions[],
                     int count)
{
    if (count == 0)
    {
        printf("\nNo transactions available to edit.\n");
        return;
    }

    int id =
        getInt("\nEnter transaction ID to edit: ",
               1,
               1000000);

    for (int i = 0; i < count; i++)
    {
        if (transactions[i].id == id)
        {
            printf("\n");
            printf("Editing Transaction #%d\n",
                   transactions[i].id);

            printf("Current Description: %s\n",
                   transactions[i].description);

            printf("Current Amount: $%.2f\n",
                   transactions[i].amount);

            printf("Current Date: %s\n",
                   transactions[i].date);

            if (transactions[i].type == 2)
            {
                printf("Current Category: %s\n",
                       getCategoryName(
                           transactions[i].category));
            }

            printf("\nEnter the new information.\n");

            getString("New description: ",
                      transactions[i].description,
                      DESC_LENGTH);

            transactions[i].amount =
                getDouble("New amount: $", 0.01);

            getDate("New date (MM/DD/YYYY): ",
                    transactions[i].date);

            if (transactions[i].type == 2)
            {
                printf("\n");
                printf("1. Food\n");
                printf("2. Transportation\n");
                printf("3. Entertainment\n");
                printf("4. Shopping\n");
                printf("5. Bills\n");
                printf("6. Other\n");

                transactions[i].category =
                    getInt("New category: ", 1, 6);
            }

            printf("\nTransaction updated successfully!\n");

            return;
        }
    }

    printf("\nTransaction ID not found.\n");
}


/* =========================================================
                         DELETE
   ========================================================= */

void deleteTransaction(Transaction transactions[],
                       int *count)
{
    if (*count == 0)
    {
        printf("\nNo transactions available to delete.\n");
        return;
    }

    int id =
        getInt("\nEnter transaction ID to delete: ",
               1,
               1000000);

    for (int i = 0; i < *count; i++)
    {
        if (transactions[i].id == id)
        {
            printf("\nTransaction Found\n");
            printf("------------------------------\n");

            printf("Description: %s\n",
                   transactions[i].description);

            printf("Amount: $%.2f\n",
                   transactions[i].amount);

            printf("Date: %s\n",
                   transactions[i].date);

            char confirmation[10];

            getString("\nType YES to delete this transaction: ",
                      confirmation,
                      sizeof(confirmation));

            if (strcmp(confirmation, "YES") != 0)
            {
                printf("\nDeletion cancelled.\n");
                return;
            }

            for (int j = i;
                 j < *count - 1;
                 j++)
            {
                transactions[j] =
                    transactions[j + 1];
            }

            (*count)--;

            printf("\nTransaction deleted successfully!\n");

            return;
        }
    }

    printf("\nTransaction ID not found.\n");
}


/* =========================================================
                          SORT
   ========================================================= */

void sortTransactionsByAmount(
    const Transaction transactions[],
    int count)
{
    if (count == 0)
    {
        printf("\nNo transactions available to sort.\n");
        return;
    }

    Transaction sorted[MAX_TRANSACTIONS];

    for (int i = 0; i < count; i++)
    {
        sorted[i] = transactions[i];
    }

    /*
       Bubble sort:
       Sort from highest amount to lowest amount.
    */

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0;
             j < count - i - 1;
             j++)
        {
            if (sorted[j].amount <
                sorted[j + 1].amount)
            {
                Transaction temp =
                    sorted[j];

                sorted[j] =
                    sorted[j + 1];

                sorted[j + 1] =
                    temp;
            }
        }
    }

    printf("\n");
    printf("======================================================================\n");
    printf("                    TRANSACTIONS BY AMOUNT\n");
    printf("======================================================================\n");

    printf("%-5s %-25s %-12s %12s\n",
           "ID",
           "DESCRIPTION",
           "TYPE",
           "AMOUNT");

    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-5d %-25.25s %-12s $%11.2f\n",
               sorted[i].id,
               sorted[i].description,
               getTypeName(sorted[i].type),
               sorted[i].amount);
    }

    printf("======================================================================\n");
}


/* =========================================================
                         BUDGET
   ========================================================= */

void setBudget(double *monthlyBudget)
{
    printf("\n");
    printf("============================================================\n");
    printf("                       MONTHLY BUDGET\n");
    printf("============================================================\n");

    *monthlyBudget =
        getDouble("Enter monthly spending budget: $",
                  0.01);

    printf("\nMonthly budget successfully set to $%.2f\n",
           *monthlyBudget);
}


/* =========================================================
                     CATEGORY HELPERS
   ========================================================= */

const char *getCategoryName(int category)
{
    switch (category)
    {
        case 1:
            return "Food";

        case 2:
            return "Transportation";

        case 3:
            return "Entertainment";

        case 4:
            return "Shopping";

        case 5:
            return "Bills";

        case 6:
            return "Other";

        default:
            return "Unknown";
    }
}

const char *getTypeName(int type)
{
    if (type == 1)
    {
        return "Income";
    }

    if (type == 2)
    {
        return "Expense";
    }

    return "Unknown";
}


/* =========================================================
                       SAVE TRANSACTIONS
   ========================================================= */

void saveTransactions(const Transaction transactions[],
                      int count)
{
    FILE *file =
        fopen(DATA_FILE, "wb");

    if (file == NULL)
    {
        printf("\nWARNING: Transaction data could not be saved.\n");
        return;
    }

    fwrite(&count,
           sizeof(int),
           1,
           file);

    if (count > 0)
    {
        fwrite(transactions,
               sizeof(Transaction),
               (size_t)count,
               file);
    }

    fclose(file);
}


/* =========================================================
                       LOAD TRANSACTIONS
   ========================================================= */

void loadTransactions(Transaction transactions[],
                      int *count,
                      int *nextId)
{
    FILE *file =
        fopen(DATA_FILE, "rb");

    if (file == NULL)
    {
        *count = 0;
        *nextId = 1;
        return;
    }

    int storedCount = 0;

    if (fread(&storedCount,
              sizeof(int),
              1,
              file) != 1)
    {
        fclose(file);

        *count = 0;
        *nextId = 1;

        return;
    }

    if (storedCount < 0 ||
        storedCount > MAX_TRANSACTIONS)
    {
        fclose(file);

        *count = 0;
        *nextId = 1;

        return;
    }

    size_t loaded = 0;

    if (storedCount > 0)
    {
        loaded =
            fread(transactions,
                  sizeof(Transaction),
                  (size_t)storedCount,
                  file);
    }

    fclose(file);

    *count = (int)loaded;

    int highestId = 0;

    for (int i = 0; i < *count; i++)
    {
        if (transactions[i].id > highestId)
        {
            highestId =
                transactions[i].id;
        }
    }

    *nextId =
        highestId + 1;
}


/* =========================================================
                       SAVE SETTINGS
   ========================================================= */

void saveSettings(double monthlyBudget)
{
    FILE *file =
        fopen(SETTINGS_FILE, "wb");

    if (file == NULL)
    {
        printf("\nWARNING: Budget settings could not be saved.\n");
        return;
    }

    fwrite(&monthlyBudget,
           sizeof(double),
           1,
           file);

    fclose(file);
}


/* =========================================================
                       LOAD SETTINGS
   ========================================================= */

void loadSettings(double *monthlyBudget)
{
    FILE *file =
        fopen(SETTINGS_FILE, "rb");

    if (file == NULL)
    {
        *monthlyBudget = 0.0;
        return;
    }

    if (fread(monthlyBudget,
              sizeof(double),
              1,
              file) != 1)
    {
        *monthlyBudget = 0.0;
    }

    fclose(file);

    if (*monthlyBudget < 0.0)
    {
        *monthlyBudget = 0.0;
    }
}


/* =========================================================
                      INTEGER INPUT
   ========================================================= */

int getInt(const char *prompt,
           int min,
           int max)
{
    char buffer[100];

    int value;
    char extra;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer,
                  sizeof(buffer),
                  stdin) == NULL)
        {
            clearerr(stdin);
            continue;
        }

        /*
           "%d %c" checks whether anything other
           than whitespace follows the integer.

           Example:
           "2"     = valid
           "2abc"  = invalid
        */

        int result =
            sscanf(buffer,
                   " %d %c",
                   &value,
                   &extra);

        if (result == 1)
        {
            if (value >= min &&
                value <= max)
            {
                return value;
            }
        }

        printf("Invalid input. Enter a number from %d to %d.\n",
               min,
               max);
    }
}


/* =========================================================
                       DECIMAL INPUT
   ========================================================= */

double getDouble(const char *prompt,
                 double min)
{
    char buffer[100];

    double value;
    char extra;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer,
                  sizeof(buffer),
                  stdin) == NULL)
        {
            clearerr(stdin);
            continue;
        }

        int result =
            sscanf(buffer,
                   " %lf %c",
                   &value,
                   &extra);

        if (result == 1)
        {
            if (value >= min)
            {
                return value;
            }
        }

        printf("Invalid amount. Enter a number of at least %.2f.\n",
               min);
    }
}


/* =========================================================
                        STRING INPUT
   ========================================================= */

void getString(const char *prompt,
               char text[],
               int size)
{
    while (1)
    {
        printf("%s", prompt);

        if (fgets(text,
                  size,
                  stdin) == NULL)
        {
            clearerr(stdin);
            continue;
        }

        /*
           Check whether the entire line fit inside
           the destination array.
        */

        if (strchr(text, '\n') == NULL)
        {
            int character;

            while ((character = getchar()) != '\n' &&
                   character != EOF)
            {
                /* Discard remaining characters. */
            }
        }

        text[strcspn(text, "\n")] =
            '\0';

        /*
           Reject strings containing only whitespace.
        */

        int containsText = 0;

        for (int i = 0;
             text[i] != '\0';
             i++)
        {
            if (!isspace((unsigned char)text[i]))
            {
                containsText = 1;
                break;
            }
        }

        if (containsText)
        {
            return;
        }

        printf("Input cannot be empty.\n");
    }
}


/* =========================================================
                        DATE INPUT
   ========================================================= */

void getDate(const char *prompt,
             char date[])
{
    while (1)
    {
        getString(prompt,
                  date,
                  DATE_LENGTH);

        if (isValidDate(date))
        {
            return;
        }

        printf("Invalid date. Please use MM/DD/YYYY.\n");
    }
}


/* =========================================================
                      DATE VALIDATION
   ========================================================= */

int isValidDate(const char *date)
{
    if (strlen(date) != 10)
    {
        return 0;
    }

    if (date[2] != '/' ||
        date[5] != '/')
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 ||
            i == 5)
        {
            continue;
        }

        if (!isdigit((unsigned char)date[i]))
        {
            return 0;
        }
    }

    int month =
        (date[0] - '0') * 10 +
        (date[1] - '0');

    int day =
        (date[3] - '0') * 10 +
        (date[4] - '0');

    int year =
        (date[6] - '0') * 1000 +
        (date[7] - '0') * 100 +
        (date[8] - '0') * 10 +
        (date[9] - '0');

    if (month < 1 ||
        month > 12)
    {
        return 0;
    }

    if (year < 1900 ||
        year > 2200)
    {
        return 0;
    }

    int daysInMonth[12] =
    {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31
    };

    int leapYear =
        (year % 400 == 0) ||
        (year % 4 == 0 &&
         year % 100 != 0);

    if (leapYear)
    {
        daysInMonth[1] = 29;
    }

    if (day < 1 ||
        day > daysInMonth[month - 1])
    {
        return 0;
    }

    return 1;
}


/* =========================================================
                          PAUSE
   ========================================================= */

void pauseProgram(void)
{
    char buffer[10];

    printf("\nPress ENTER to return to the main menu...");

    fgets(buffer,
          sizeof(buffer),
          stdin);
}