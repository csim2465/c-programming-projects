#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TRANSACTIONS 500
#define DESC_LENGTH 80
#define DATE_LENGTH 11
#define DATA_FILE "transactions.dat"
#define SETTINGS_FILE "finance_settings.dat"

typedef struct
{
    int id;
    char description[DESC_LENGTH];
    double amount;
    int type;       // 1 = Income, 2 = Expense
    int category;   // 1-6 for expenses
    char date[DATE_LENGTH];
} Transaction;

/* ---------- FUNCTION PROTOTYPES ---------- */

void displayHeader(void);
void displayMenu(void);

void addIncome(Transaction transactions[], int *count, int *nextId);
void addExpense(Transaction transactions[], int *count, int *nextId);

void viewTransactions(Transaction transactions[], int count);
void viewSummary(Transaction transactions[], int count, double monthlyBudget);
void viewCategoryReport(Transaction transactions[], int count);

void searchTransactions(Transaction transactions[], int count);
void deleteTransaction(Transaction transactions[], int *count);
void editTransaction(Transaction transactions[], int count);

void sortTransactionsByAmount(Transaction transactions[], int count);

void setBudget(double *monthlyBudget);

const char *getCategoryName(int category);
const char *getTypeName(int type);

void saveTransactions(Transaction transactions[], int count);
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

    loadTransactions(transactions, &transactionCount, &nextId);
    loadSettings(&monthlyBudget);

    do
    {
        displayHeader();
        displayMenu();

        choice = getInt("Select an option: ", 1, 11);

        switch (choice)
        {
            case 1:
                addIncome(transactions, &transactionCount, &nextId);
                saveTransactions(transactions, transactionCount);
                break;

            case 2:
                addExpense(transactions, &transactionCount, &nextId);
                saveTransactions(transactions, transactionCount);
                break;

            case 3:
                viewTransactions(transactions, transactionCount);
                break;

            case 4:
                viewSummary(transactions, transactionCount, monthlyBudget);
                break;

            case 5:
                viewCategoryReport(transactions, transactionCount);
                break;

            case 6:
                searchTransactions(transactions, transactionCount);
                break;

            case 7:
                editTransaction(transactions, transactionCount);
                saveTransactions(transactions, transactionCount);
                break;

            case 8:
                deleteTransaction(transactions, &transactionCount);
                saveTransactions(transactions, transactionCount);
                break;

            case 9:
                sortTransactionsByAmount(transactions, transactionCount);
                break;

            case 10:
                setBudget(&monthlyBudget);
                saveSettings(monthlyBudget);
                break;

            case 11:
                saveTransactions(transactions, transactionCount);
                saveSettings(monthlyBudget);

                printf("\n========================================\n");
                printf("       DATA SAVED SUCCESSFULLY\n");
                printf("========================================\n");
                printf("Thank you for using Finance Manager!\n");
                printf("========================================\n");
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
                         DISPLAY
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
                     ADD TRANSACTIONS
   ========================================================= */

void addIncome(Transaction transactions[], int *count, int *nextId)
{
    Transaction newTransaction;

    if (*count >= MAX_TRANSACTIONS)
    {
        printf("\nTransaction storage is full.\n");
        return;
    }

    printf("\n================ ADD INCOME ================\n");

    newTransaction.id = (*nextId)++;

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

    printf("\nIncome added successfully!\n");
    printf("Transaction ID: %d\n", newTransaction.id);
}

void addExpense(Transaction transactions[], int *count, int *nextId)
{
    Transaction newTransaction;

    if (*count >= MAX_TRANSACTIONS)
    {
        printf("\nTransaction storage is full.\n");
        return;
    }

    printf("\n================ ADD EXPENSE ===============\n");

    newTransaction.id = (*nextId)++;

    getString("Description: ",
              newTransaction.description,
              DESC_LENGTH);

    newTransaction.amount =
        getDouble("Amount: $", 0.01);

    getDate("Date (MM/DD/YYYY): ",
            newTransaction.date);

    printf("\nExpense Categories\n");
    printf("------------------------------\n");
    printf("1. Food\n");
    printf("2. Transportation\n");
    printf("3. Entertainment\n");
    printf("4. Shopping\n");
    printf("5. Bills\n");
    printf("6. Other\n");

    newTransaction.category =
        getInt("Select category: ", 1, 6);

    newTransaction.type = 2;

    transactions[*count] = newTransaction;
    (*count)++;

    printf("\nExpense added successfully!\n");
    printf("Transaction ID: %d\n", newTransaction.id);
}


/* =========================================================
                    VIEW TRANSACTIONS
   ========================================================= */

void viewTransactions(Transaction transactions[], int count)
{
    if (count == 0)
    {
        printf("\nNo transactions have been recorded yet.\n");
        return;
    }

    printf("\n");
    printf("================================================================================\n");
    printf("                              TRANSACTION HISTORY\n");
    printf("================================================================================\n");
    printf("%-5s %-12s %-10s %-22s %-15s %12s\n",
           "ID",
           "DATE",
           "TYPE",
           "DESCRIPTION",
           "CATEGORY",
           "AMOUNT");

    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-5d %-12s %-10s %-22.22s %-15s $%11.2f\n",
               transactions[i].id,
               transactions[i].date,
               getTypeName(transactions[i].type),
               transactions[i].description,
               transactions[i].type == 1
                   ? "-"
                   : getCategoryName(transactions[i].category),
               transactions[i].amount);
    }

    printf("================================================================================\n");
}


/* =========================================================
                    FINANCIAL DASHBOARD
   ========================================================= */

void viewSummary(Transaction transactions[],
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
        else
        {
            totalExpenses += transactions[i].amount;
            expenseCount++;

            if (transactions[i].amount > largestExpense)
            {
                largestExpense = transactions[i].amount;

                strcpy(largestExpenseName,
                       transactions[i].description);
            }
        }
    }

    double balance = totalIncome - totalExpenses;

    double savingsRate = 0.0;

    if (totalIncome > 0)
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

    if (largestExpense > 0)
    {
        printf("Largest Expense: %s ($%.2f)\n",
               largestExpenseName,
               largestExpense);
    }
    else
    {
        printf("Largest Expense: None\n");
    }

    printf("\n");

    if (monthlyBudget > 0)
    {
        double remaining =
            monthlyBudget - totalExpenses;

        double percentageUsed =
            (totalExpenses / monthlyBudget) * 100.0;

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

        printf("\nStatus: ");

        if (percentageUsed > 100)
        {
            printf("OVER BUDGET\n");
        }
        else if (percentageUsed >= 90)
        {
            printf("WARNING - Budget almost reached\n");
        }
        else if (percentageUsed >= 75)
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
        printf("Monthly Budget: Not configured\n");
    }

    printf("============================================================\n");
}


/* =========================================================
                    CATEGORY ANALYTICS
   ========================================================= */

void viewCategoryReport(Transaction transactions[], int count)
{
    double categoryTotals[7] = {0};
    double totalExpenses = 0.0;

    for (int i = 0; i < count; i++)
    {
        if (transactions[i].type == 2)
        {
            int category = transactions[i].category;

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

    if (totalExpenses == 0)
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

        printf("%-20s $%10.2f   %6.1f%%\n",
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

void searchTransactions(Transaction transactions[], int count)
{
    if (count == 0)
    {
        printf("\nNo transactions available to search.\n");
        return;
    }

    char searchTerm[DESC_LENGTH];
    char descriptionLower[DESC_LENGTH];
    char searchLower[DESC_LENGTH];

    int found = 0;

    getString("\nEnter description to search: ",
              searchTerm,
              DESC_LENGTH);

    strcpy(searchLower, searchTerm);

    for (int i = 0; searchLower[i]; i++)
    {
        searchLower[i] =
            (char)tolower((unsigned char)searchLower[i]);
    }

    printf("\n================ SEARCH RESULTS ================\n");

    for (int i = 0; i < count; i++)
    {
        strcpy(descriptionLower,
               transactions[i].description);

        for (int j = 0; descriptionLower[j]; j++)
        {
            descriptionLower[j] =
                (char)tolower(
                    (unsigned char)descriptionLower[j]);
        }

        if (strstr(descriptionLower, searchLower) != NULL)
        {
            printf("\nID:          %d\n",
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

            found = 1;
        }
    }

    if (!found)
    {
        printf("No matching transactions found.\n");
    }

    printf("================================================\n");
}


/* =========================================================
                           EDIT
   ========================================================= */

void editTransaction(Transaction transactions[], int count)
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
            printf("\nEditing: %s\n",
                   transactions[i].description);

            getString("New description: ",
                      transactions[i].description,
                      DESC_LENGTH);

            transactions[i].amount =
                getDouble("New amount: $", 0.01);

            getDate("New date (MM/DD/YYYY): ",
                    transactions[i].date);

            if (transactions[i].type == 2)
            {
                printf("\n1. Food\n");
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

void deleteTransaction(Transaction transactions[], int *count)
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
            printf("\nDeleting: %s ($%.2f)\n",
                   transactions[i].description,
                   transactions[i].amount);

            char confirmation[10];

            getString("Type YES to confirm: ",
                      confirmation,
                      sizeof(confirmation));

            if (strcmp(confirmation, "YES") != 0)
            {
                printf("Deletion cancelled.\n");
                return;
            }

            for (int j = i; j < *count - 1; j++)
            {
                transactions[j] =
                    transactions[j + 1];
            }

            (*count)--;

            printf("Transaction deleted successfully!\n");
            return;
        }
    }

    printf("\nTransaction ID not found.\n");
}


/* =========================================================
                           SORT
   ========================================================= */

void sortTransactionsByAmount(Transaction transactions[],
                              int count)
{
    if (count < 2)
    {
        printf("\nNot enough transactions to sort.\n");
        return;
    }

    Transaction sorted[MAX_TRANSACTIONS];

    for (int i = 0; i < count; i++)
    {
        sorted[i] = transactions[i];
    }

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (sorted[j].amount < sorted[j + 1].amount)
            {
                Transaction temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    printf("\n");
    printf("============================================================\n");
    printf("             TRANSACTIONS SORTED BY AMOUNT\n");
    printf("============================================================\n");

    for (int i = 0; i < count; i++)
    {
        printf("%2d. %-25.25s $%10.2f\n",
               i + 1,
               sorted[i].description,
               sorted[i].amount);
    }

    printf("============================================================\n");
}


/* =========================================================
                           BUDGET
   ========================================================= */

void setBudget(double *monthlyBudget)
{
    printf("\n================ MONTHLY BUDGET ================\n");

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

    return "Expense";
}


/* =========================================================
                       FILE STORAGE
   ========================================================= */

void saveTransactions(Transaction transactions[], int count)
{
    FILE *file = fopen(DATA_FILE, "wb");

    if (file == NULL)
    {
        printf("\nWarning: Could not save transaction data.\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, file);

    fwrite(transactions,
           sizeof(Transaction),
           count,
           file);

    fclose(file);
}

void loadTransactions(Transaction transactions[],
                      int *count,
                      int *nextId)
{
    FILE *file = fopen(DATA_FILE, "rb");

    if (file == NULL)
    {
        *count = 0;
        *nextId = 1;
        return;
    }

    if (fread(count, sizeof(int), 1, file) != 1)
    {
        *count = 0;
        *nextId = 1;
        fclose(file);
        return;
    }

    if (*count < 0 || *count > MAX_TRANSACTIONS)
    {
        *count = 0;
        *nextId = 1;
        fclose(file);
        return;
    }

    size_t loaded =
        fread(transactions,
              sizeof(Transaction),
              *count,
              file);

    *count = (int)loaded;

    fclose(file);

    int highestId = 0;

    for (int i = 0; i < *count; i++)
    {
        if (transactions[i].id > highestId)
        {
            highestId = transactions[i].id;
        }
    }

    *nextId = highestId + 1;
}

void saveSettings(double monthlyBudget)
{
    FILE *file = fopen(SETTINGS_FILE, "wb");

    if (file == NULL)
    {
        return;
    }

    fwrite(&monthlyBudget,
           sizeof(double),
           1,
           file);

    fclose(file);
}

void loadSettings(double *monthlyBudget)
{
    FILE *file = fopen(SETTINGS_FILE, "rb");

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
}


/* =========================================================
                     INPUT VALIDATION
   ========================================================= */

int getInt(const char *prompt, int min, int max)
{
    char buffer[100];
    int value;
    char extra;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            continue;
        }

        if (sscanf(buffer, "%d %c", &value, &extra) == 1)
        {
            if (value >= min && value <= max)
            {
                return value;
            }
        }

        printf("Invalid input. Enter a number from %d to %d.\n",
               min,
               max);
    }
}

double getDouble(const char *prompt, double min)
{
    char buffer[100];
    double value;
    char extra;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            continue;
        }

        if (sscanf(buffer, "%lf %c", &value, &extra) == 1)
        {
            if (value >= min)
            {
                return value;
            }
        }

        printf("Invalid amount. Enter a value of at least %.2f.\n",
               min);
    }
}

void getString(const char *prompt, char text[], int size)
{
    while (1)
    {
        printf("%s", prompt);

        if (fgets(text, size, stdin) == NULL)
        {
            continue;
        }

        text[strcspn(text, "\n")] = '\0';

        if (strlen(text) > 0)
        {
            return;
        }

        printf("Input cannot be empty.\n");
    }
}


/* =========================================================
                       DATE VALIDATION
   ========================================================= */

void getDate(const char *prompt, char date[])
{
    while (1)
    {
        getString(prompt, date, DATE_LENGTH);

        if (isValidDate(date))
        {
            return;
        }

        printf("Invalid date. Use MM/DD/YYYY.\n");
    }
}

int isValidDate(const char *date)
{
    if (strlen(date) != 10)
    {
        return 0;
    }

    if (date[2] != '/' || date[5] != '/')
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
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

    if (month < 1 || month > 12)
    {
        return 0;
    }

    if (year < 1900 || year > 2200)
    {
        return 0;
    }

    int daysInMonth[] =
        {31, 28, 31, 30, 31, 30,
         31, 31, 30, 31, 30, 31};

    int leapYear =
        (year % 400 == 0) ||
        (year % 4 == 0 && year % 100 != 0);

    if (leapYear)
    {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1])
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
    fgets(buffer, sizeof(buffer), stdin);
}