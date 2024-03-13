#include <stdio.h>

#include <stdlib.h>

#include <string.h>

struct date
{

    int mm, dd, yy;
};
FILE *fout;

FILE *fin;

struct Item

{
    int itemNo;

    char name[25];

    struct date d;
};
struct Amount
{
    struct Item item;

    float price, qty, tax, gross, dis, netAmt;
};
struct User
{
    char username[20];
    char password[20];
};

struct User users[] = {
    {"admin", "admin123"},
    {"user", "123"}};

int authenticateUser(const char *username, const char *password)
{
    int numUsers = sizeof(users) / sizeof(users[0]);
    for (int i = 0; i < numUsers; i++)
    {
        if (strcmp(username, users[i].username) == 0 && strcmp(password, users[i].password) == 0)
        {
            return 1; // Authentication successful
        }
    }
    return 0; // Authentication failed
}
void add(struct Amount *a)
{
    printf("\n\n\tItem No: ");

    scanf("%d", &a->item.itemNo);

    printf("\n\n\tName of the item: ");

    scanf("%s", a->item.name);

    printf("\n\n\tManufacturing Date (dd-mm-yy): ");

    scanf("%d %d %d", &a->item.d.mm, &a->item.d.dd, &a->item.d.yy);

    printf("\n\n\tPrice: ");

    scanf("%f", &a->price);

    printf("\n\n\tQuantity: ");

    scanf("%f", &a->qty);

    printf("\n\n\tTax percent: ");

    scanf(" %f", &a->tax);

    printf("\n\n\tDiscount percent: ");

    scanf("%f", &a->dis);

    a->gross = a->price + (a->price * (a->tax / 100));

    a->netAmt = a->qty * (a->gross - (a->gross * (a->dis / 100)));

    fout = fopen("itemstore.dat", "ab");

    fwrite(a, sizeof(struct Amount), 1, fout);

    fclose(fout);
}

void show(struct Amount *a)
{
    fin = fopen("itemstore.dat", "rb");

    while (fread(a, sizeof(struct Amount), 1, fin))
    {
        printf("\n%d", a->item.itemNo);
        printf("\t\t%s", a->item.name);

        printf("\t\t%d-%d-%d", a->item.d.mm, a->item.d.dd, a->item.d.yy);
        printf("\t\t %.2f\n\n", a->netAmt);
    }
    fclose(fin);
}

void pay(struct Amount *a)
{
    float sum = 0;
    printf("\n\n\n\t\t*********************************************");
    printf("\n\t\t----------------DETAILS---------------");
    printf("\n\t\t*********************************************");
    fin = fopen("itemstore.dat", "rb");

    while (fread(a, sizeof(struct Amount), 1, fin))
    {
        printf("\n\n\t\tNAME                      : %s", a->item.name);
        printf("\n\t\tPRICE                     : %.2f", a->price);
        printf("\n\t\tQUANTITY                  : %.2f", a->qty);
        printf("\n\t\tTAX PERCENTAGE            : %.2f", a->tax);
        printf("\n\t\tDISCOUNT PERCENTAGE       : %.2f", a->dis);
        printf("\n\n\t\tNET AMOUNT                : Rs. %.2f", a->netAmt);

        sum += a->netAmt;
        printf("\n\t\t*********************************************\n");
    }

    printf("\n\n\t\tTOTAL PRICE                :Rs. %.2f\n", sum);
    printf("\n\t\t*********************************************\n\n");
}
void edit(struct Amount *a)
{
    int flag = 0;
    int itemNo;
    printf("\n\n\tEnter the Item No to edit: ");
    scanf("%d", &itemNo);

    FILE *file = fopen("itemstore.dat", "rb+");
    if (file == NULL)
    {
        printf("\n\n\tError opening file!");
        return;
    }

    while (fread(a, sizeof(struct Amount), 1, file))
    {
        if (a->item.itemNo == itemNo)
        {
            printf("\n\n\tItem No: %d", a->item.itemNo);

            printf("\n\n\tName of the item: %s", a->item.name);

            printf("\n\n\tDate: %d-%d-%d", a->item.d.mm, a->item.d.dd, a->item.d.yy);

            printf("\n\n\tEnter new details:\n");

            printf("\n\n\tName of the item: ");

            scanf("%s", a->item.name);

            printf("\n\n\tManufacturing Date (dd-mm-yy): ");

            scanf("%d %d %d", &a->item.d.mm, &a->item.d.dd, &a->item.d.yy);

            printf("\n\n\tPrice: ");

            scanf("%f", &a->price);

            printf("\n\n\tQuantity: ");

            scanf("%f", &a->qty);

            printf("\n\n\tTax percent: ");

            scanf("%f", &a->tax);

            printf("\n\n\tDiscount percent: ");

            scanf("%f", &a->dis);

            a->gross = a->price + (a->price * (a->tax / 100));

            a->netAmt = a->qty * (a->gross - (a->gross * (a->dis / 100)));

            fseek(file, -sizeof(struct Amount), SEEK_CUR);

            fwrite(a, sizeof(struct Amount), 1, file);

            printf("\n\n\tItem updated successfully!\n\n");

            break;
        }
        else
        {
            flag = 1;
        }
    }
    if (flag == 1)
    {
        printf("\n\n\tItem does not Exist!!!!!!!!!!!!!\n\n");
    }
    fclose(file);
}
void remove(struct Amount *a)
{
    int itemNo;

    printf("\n\n\tEnter the Item No to remove: ");

    scanf("%d", &itemNo);

    FILE *file = fopen("itemstore.dat", "rb+");

    if (file == NULL)
    {

        printf("\n\n\tError opening file!");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");

    if (tempFile == NULL)
    {
        printf("\n\n\tError creating temporary file!\n\n");

        fclose(file);

        return;
    }

    int found = 0;

    while (fread(a, sizeof(struct Amount), 1, file))
    {
        if (a->item.itemNo != itemNo)
        {
            fwrite(a, sizeof(struct Amount), 1, tempFile);
        }
        else
        {
            found = 1;
        }
    }

    fclose(file);

    fclose(tempFile);

    if (found)
    {
        remove("itemstore.dat");

        rename("temp.dat", "itemstore.dat");

        printf("\n\n\tItem removed successfully!\n\n");
    }
    else
    {
        remove("temp.dat");

        printf("\n\n\tItem not found!\n\n");
    }
}
int main()
{
    struct Amount a;

    int ch;
    printf("\n\n\t\t---------------Super Market Billing-----------------\n");
    printf("\t\t====================================================\n\n");
    char username[20];
    char password[20];

    while (1)
    {
        printf("\n\t\tPlease enter your username: ");
        scanf("%s", username);
        printf("\t\tPlease enter your password: ");
        scanf("%s", password);

        if (authenticateUser(username, password))
        {
            printf("\n\n\t\t-----Login Successful!-----\n");
            break;
        }
        else
        {
            printf("\n\n\t\tInvalid username or password! Please try again.\n");
        }
    }

    do
    {
        printf("\n\t\t1. Add Item");

        printf("\n\t\t2. Show Item");

        printf("\n\t\t3. Generate Bill");

        printf("\n\t\t6. Exit");

        printf("\n\t\t4. edit");

        printf("\n\t\t5. remove");

        printf("\n\n\t\tPlease enter your choice (1-6): ");

        scanf("%d", &ch);

        switch (ch) // Switch Case......
        {
        case 1:
            add(&a);
            printf("\n\n-----Item added successfully!------\n\n");
            break;

        case 2:
            printf("\n\n\t\t\t******ITEM LIST*******\n\n");
            printf("=====================================================================\n");
            printf("Item No.       Name                Date                  Net Amount\n");
            printf("=====================================================================\n");
            show(&a);
            break;

        case 3:

            printf("\n\n\t\t\t\tBILL\n\n");
            pay(&a);

            break;

        case 6:
            printf("\n\n\t\t*****Thank you for using the Super Market Billing system!******\n\n");
            exit(0);

        case 4:
            edit(&a);
            break;

        case 5:
            remove(&a);
            break;

        default:

            printf("\n\n\t\tInvalid choice! Please try again.");
        }

    } while (1);

    return 0;
}