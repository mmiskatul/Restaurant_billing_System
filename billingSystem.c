#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VERIFICATION_FILE "verify.txt"
#define BILLS_FILE "AllBill.txt"

struct item {
    char ItemName[20];
    float price;
    int qty;
};

struct Order {
    char CustomerName[30];
    char date[50];
    char time[50];
    int noOfItem;
    struct item ITM[100];
    int disPercentages;
    float paid;
};

int emailCheck(const char *email) {
    const char *at = strchr(email, '@');
    if (at == NULL || at == email) return 0;
    const char *dot = strchr(at + 1, '.');
    if (dot == NULL || dot == at + 1) return 0;
    if (strlen(dot + 1) < 2) return 0;
    return 1;
}

void generateBillHead(char name[20], char date[20], char time[]) {
    printf("\n\n\t\tOur Cafe");
    printf("\n===========================");
    printf("\nDate: %s\tTime: %s", date, time);
    printf("\nCustomer Name: %s", name);
    printf("\n------------------------------\n-");
    printf("\nItem\t\tQTY\t\tTotal");
    printf("\n-------------------------------\n");
}

void generateBillBody(char item[], int qty, float price) {
    float total = price * qty;
    printf("%-15s\t%d\t\t%.2f\n", item, qty, total);
}

void generateBillFooter(float total, int percentage, float paid) {
    float discount = ((float)percentage / 100) * total;
    float netTotal = total - discount;
    float gst = netTotal * 0.05;
    float grandTotal = netTotal + gst;

    printf("\n----------------------------");
    printf("\nSubTotal:\t\t\t%.2f", total);
    printf("\nDiscount (%d%%):\t\t%.2f", percentage, discount);
    printf("\nNet Total:\t\t\t%.2f", netTotal);
    printf("\nGST (5%%):\t\t\t%.2f", gst);
    printf("\nPayable:\t\t\t%.2f", grandTotal);
    printf("\nPaid:\t\t\t%.2f", paid);
    printf("\nChange:\t\t\t%.2f", paid - grandTotal);
    printf("\n------------------------------------");
}

void saveBill(struct Order ord, float total) {
    FILE *fp = fopen(BILLS_FILE, "a+");
    if (fp == NULL) {
        printf("\nError saving file!");
        return;
    }
    fprintf(fp, "\n==========================\n");
    fprintf(fp, "Customer: %s\nDate: %s\nTime: %s\n", ord.CustomerName, ord.date, ord.time);
    fprintf(fp, "-------------------------------\n");
    fprintf(fp, "Item\t\tQTY\t\tPrice\n");
    for (int i = 0; i < ord.noOfItem; i++) {
        fprintf(fp, "%-15s\t%d\t\t%.2f\n", ord.ITM[i].ItemName, ord.ITM[i].qty, ord.ITM[i].price);
    }
    fprintf(fp, "-------------------------------\n");
    fprintf(fp, "SubTotal: %.2f\n", total);
    float discount = ((float)ord.disPercentages / 100) * total;
    float netTotal = total - discount;
    float gst = netTotal * 0.05;
    fprintf(fp, "Discount: %.2f\n", discount);
    fprintf(fp, "GST: %.2f\n", gst);
    fprintf(fp, "Grand Total: %.2f\n", netTotal + gst);
    fprintf(fp, "==========================\n");
    fclose(fp);
    printf("\nBill Saved Successfully!\n");
    getchar();
}

void registerUser() {
    char username[30], password[30], confirmPass[30];
    FILE *fp = fopen(VERIFICATION_FILE, "a+");
    printf("\n=========== Register ===========");
    do {
        printf("\nEnter your Email: ");
        scanf("%s", username);
        if (!emailCheck(username)) {
            printf("\nInvalid Email Format! Please enter a valid email.");
        }
    } while (!emailCheck(username));
    
    while (1) {
        printf("Enter your Password: ");
        scanf("%s", password);
        printf("Confirm Password: ");
        scanf("%s", confirmPass);
        if (strcmp(password, confirmPass) == 0) {
            break;
        } else {
            printf("\nPasswords do not match! Try again.\n");
        }
    }
    fprintf(fp, "%s %s\n", username, password);
    fclose(fp);
    printf("\nUser Registered Successfully!");
     getchar();
    
}

int loginUser() {
    char username[30], password[30];
    char storedUsername[30], storedPassword[30];
    FILE *fp = fopen(VERIFICATION_FILE, "r");
    if (fp == NULL) {
        printf("\nNo registered account found! Please register first.");
        return 0;
    }
    printf("\n=========== Login ===========");
    printf("\nEnter your Email: ");
    scanf("%s", username);
    printf("Enter your Password: ");
    scanf("%s", password);

    while (fscanf(fp, "%s %s", storedUsername, storedPassword) != EOF) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            fclose(fp);
            printf("\nLogin Successful!\n");
            getchar();
            return 1;
        }
    }
    fclose(fp);
    printf("Invalid Email or Password");
    getchar();
    return 0;
}
void SerchOrderHistroy(){
    char serchname[30],line[200];
    char startDate[20],endDate[20];
    int searchOpt,found=0;
    printf("\n\n============ Search Order History ============\n\n\n");
    printf("\n1. Search By The Customer Name  ");
    printf("\n2. Search By Date Range ");
    printf("\n\nEnter Your choice : ");
    scanf("%d",&searchOpt);
    fgetc(stdin);
    FILE *fp;
    fp=fopen(BILLS_FILE,"r");
    if(fp==NULL){
        printf("\nNo sales report found ");
        return;
    }
    switch(searchOpt){
        case 1:
            printf("\nEnter Customer Name to Search :");
            scanf("%[^\n]s",serchname);
            fgetc(stdin);
            printf("\nSearching for records of customer: %s\n", serchname);
            printf("------------------------------------\n");
            while(fgets(line,sizeof(line),fp)){
                if(strstr(line,serchname)){
                    printf("%s",line);
                    found=-1;
                    printf("\nBill for the customer : %s\n\n",serchname);
                    do{
                        printf("%s",line);
                        if(strstr(line,"===================="))break;

                    getchar();
                    }while(fgets(line,sizeof(line),fp));
                }
            }
            fclose(fp);
            if(!found){
                printf("\nNo record found ");
            }
            break;
        case 2:
            printf("\nIncomplete");
            getchar();
            break;
            default :
            printf("\nInvalid Choice");

    }
    fclose(fp);
    if(!found){
        printf("\nNo matching records found ");
    }
    
}

void calculatesales(){
    char startDate[20],EndDate[20],line[200];
    float totalSales=0.0;
    printf("\n=================Calculate Total Sales=================\n");
    printf("");
    
}
int main() {
    int choice, opt;
    float total;
    struct Order ord;
    char saveBillChoice;

    do {
        getchar();
        system("cls");
        printf("\n=========== Welcome ===========");
        printf("\n=========== Our Cafe ===========");
        printf("\n\n1. Register");
        printf("\n2. Login");
        printf("\n3. Exit");
        printf("\n\nYour choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                registerUser();
              
                break;
            case 2:
                if (loginUser()) {
                  
                    goto main_menu;
                }
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid input.");
        }
    } while (1);

main_menu:
    do {
        total = 0.0;
        getchar();
        system("cls");
        printf("\n=========== Our Cafe ===========");
        printf("\n\n1. Generate Bill");
        printf("\n2. Search Order History");
        printf("\n3. Calculate the Total Sales");
        printf("\n4. Logout");
        printf("\n5. Exit");
        printf("\n\nYour Choice: ");
        scanf("%d", &opt);
        switch (opt) {
            case 1:
                printf("\nEnter Customer Name: ");
                scanf(" %[^\n]", ord.CustomerName);
                strcpy(ord.date, __DATE__);
                strcpy(ord.time, __TIME__);
                printf("\nEnter Number of Items: ");
                scanf("%d", &ord.noOfItem);

                for (int i = 0; i < ord.noOfItem; i++) {
                    printf("\nEnter Item %d Name: ", i + 1);
                    scanf(" %[^\n]", ord.ITM[i].ItemName);
                    printf("Enter Quantity: ");
                    scanf("%d", &ord.ITM[i].qty);
                    printf("Enter Unit Price: ");
                    scanf("%f", &ord.ITM[i].price);
                    total += ord.ITM[i].qty * ord.ITM[i].price;
                }

                ord.disPercentages = (total >= 500) ? 10 : 0;
                printf("\nPayable: %.2f", total + (total * 0.05));
                printf("\nEnter Amount Paid: ");
                scanf("%f", &ord.paid);

                generateBillHead(ord.CustomerName, ord.date, ord.time);
                for (int i = 0; i < ord.noOfItem; i++) {
                    generateBillBody(ord.ITM[i].ItemName, ord.ITM[i].qty, ord.ITM[i].price);
                }
                generateBillFooter(total, ord.disPercentages, ord.paid);

                printf("\n\nSave the Bill 'y'||'Y': ");
                scanf(" %c", &saveBillChoice);
                if (saveBillChoice == 'Y' || saveBillChoice == 'y') {
                    saveBill(ord, total);
                }
               
                break;
            case 2:
            SerchOrderHistroy();
                break;
            case 3:
                calculatesales();
                break;
            case 4:
                main();
              
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid input.");
        }
    } while (1);

    return 0;
}
