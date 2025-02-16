#ifndef PAGES_H
#define PAGES_H
int login_page_code();
int deposit_withdraw_check();
int transfer(long long fromacc);
double accountbalance(long long accno);
int amounttwithdraw(long long accno, double amount);
int amounttdeposit(long long acc, double amount);
#include <stdio.h>
#include "menus.h"
#include "Admin.h"
#include <stdlib.h>
#include <stdbool.h>
long long accno; 
int login_page_code()
{
	loginpage:
	welcome();
	loginPage();
	int choice;
	int a;
	char name[100],dob[10],pass[10];
	long long aadhaar;
	long mobileno;
	printf("Enter your Choice : ");
	scanf("%d",&choice);
	if (choice-1){

		signup:
		if (addaccount())
		{
			printf("Account Created Successfully!!!\n");
		}
		else
		{
			printf("File the Details Properly!!!");
			goto signup;
		}

	}else{
		login:
		printf("Enter your Account No : ");
		scanf("%lld",&accno);
        char pass[100];
		printf("Enter your Password : ");
		scanf("%s",&pass);
        int a = verifyaccount(accno,pass); 
		if (a){
            clear_screen();
			return accno;
		}else{
			printf("Invalid Credentials!!!\n");
			goto login;
		}
	}
	return 0;
}
int deposit_withdraw_check(long long accno)
{
	while (1)
	{
	welcome();
	block3();
	int choice;
	double amount;
	printf("Enter your Choice : ");
	scanf("%d",&choice);
	switch(choice){
    		case 1:
            system("clear");
			// show account balance
			printf("##########Account No : %lld##########\n",accno);
			double balance = accountbalance(accno);
			printf("##########Account Balance : %.2lf##########\n",balance);
			break;
		case 2:
            system("clear");
			// Withdraw the amount
            printf("----------Withdraw----------\n");
			printf("Account No : %.2ld\n",accno);
			printf("Enter the Amount to Withdraw : ");
			scanf("%ld",&amount);
			if (amounttwithdraw(accno,amount)){
                system("clear");
				printf("###########Amount Withdrawn Successfully###########\n");
			}else{
                system("clear");
				printf("###########Invalid Credentials###########\n");
			}
			break;
		case 3:
			printf("Enter the Amount to Deposit : ");
			scanf("%lf",&amount);
			if (amounttdeposit(accno,amount)){
                system("clear");
				printf("###########Amount Deposited Successfully###########\n");
			}else{
				printf("Invalid Credentials!!!\n");
			}
			break;
		case 4:
            system("clear");
			transfer(accno);
			break;
		case 5:
				exit(0);
		default:
            printf("Invalid choice.\n");
	}
}
    clear_screen();
	return 0;
}
int transfer(long long fromacc)
{
    printf("----------TRANSFER----------\n");
    char line[100];
    long long toacc;
    long long csvacc1, csvacc2;
    double tobal, amt;
    bool x = 0, y = 0;
    double frombal;
    FILE *file, *temp;
    file = fopen("balance.csv", "r");
    temp = fopen("temp.csv", "w");
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%lld,%lf", &csvacc1, &frombal);
        if (csvacc1 == fromacc)
        {
            break;
        }
    }
    printf("Enter the acc no:\n");
    scanf("%lld", &toacc);
    while (y == 0 && x == 0)
    {
        rewind(file); 
        while (fgets(line, sizeof(line), file))
        {
            sscanf(line, "%lld,%lf", &csvacc2, &tobal);
            if (csvacc2 == toacc)
            {
                printf("Enter the amount you want to transfer: ");
                scanf("%lf", &amt);
                if (frombal >= amt)
                {
                    frombal -= amt;  
                    tobal += amt;    
                }
                x = 1;  
            }
            if(csvacc2 != fromacc)
            {
            fprintf(temp, "%lld,%.2f\n", csvacc2, tobal);  
            }
        }
        if (x == 0)
        {
            printf("Account not found.\n");
            char c;
            printf("The account number was not found, would you like to retry again? [Y]/[N]: ");
            scanf(" %c", &c);
            while (c != 'N' && c != 'Y')
            {
                printf("Try again (use caps): ");
                scanf(" %c", &c);
            }
            if (c == 'N')
            {
                y = 1;
                printf("Exiting program.\n");
                break;
            }
        printf("Enter the account number:");
        scanf("%lld",&toacc);
        }
    }
    if (x == 1)
    {
        fprintf(temp, "%lld,%.2f\n", fromacc, frombal); 
    }
    fclose(file);
    fclose(temp);
    remove("balance.csv");
    rename("temp.csv", "balance.csv");
    system("clear");
    if(x == 1)
    {
        printf("###########Transferred Successfully###########\n");
    }
    return 0;
}
double accountbalance(long long accno){
    FILE *file = fopen("balance.csv","r");
    char line[256];
    long long b;
    double balance;
    while(fgets(line,sizeof(line),file)){
        if(sscanf(line,"%lld,%lf",&b,&balance)==2){
            if(accno==b){
                return balance;
                break;
            }
        }
    }
    fclose(file);
}

int amounttwithdraw(long long accno,double amount)
{
    FILE *file = fopen("balance.csv", "r+");
    if (file == NULL) {
        printf("Error opening balance file.\n");
        return 0;
    }
    FILE *tempFile = fopen("temp.csv", "w+");
    if (tempFile == NULL) {
        printf("Error opening temporary file.\n");
        fclose(file); // Clean up and close file if temp file can't be opened
        return 0;
    }
    char line[100]; // Sufficient for "1234567890,10000.00\n"
    int found = 0;
    // Process the original file line by line
    while (fgets(line, sizeof(line), file)) {
        long long file_acc_num;
        double balance;
        sscanf(line, "%lld,%lf", &file_acc_num, &balance);
        if (file_acc_num == accno) {
            // Account found; update the balance
            balance -= amount;
            found = 1;
        }
        // Write back the line (updated if matched)
        fprintf(tempFile, "%lld,%.2lf\n", file_acc_num, balance);
    }
    // Append new account if not found
    if (!found) {
        fprintf(tempFile, "%lld,%.2lf\n", accno, amount);
    }
    fclose(file);
    fclose(tempFile);
    // Replace the original file with the updated file
    if (remove("balance.csv") != 0) {
        printf("Error removing original file.\n");
        return 0;
    }
    if (rename("temp.csv", "balance.csv") != 0) {
        printf("Error renaming temp file.\n");
        return 0;
    }
    printf("Transaction Successful!\n");
    return 1;
}
#include <stdio.h>
#include <stdlib.h>
int amounttdeposit(long long accno, double amt)
{
    FILE *file = fopen("balance.csv", "r+");
    if (file == NULL) {
        printf("Error opening balance file.\n");
        return 0;
    }
    FILE *tempFile = fopen("temp.csv", "w+");
    if (tempFile == NULL) {
        printf("Error opening temporary file.\n");
        fclose(file); // Clean up and close file if temp file can't be opened
        return 0;
    }
    char line[100]; // Sufficient for "1234567890,10000.00\n"
    int found = 0;
    // Process the original file line by line
    while (fgets(line, sizeof(line), file)) {
        long long file_acc_num;
        double balance;
        sscanf(line, "%lld,%lf", &file_acc_num, &balance);
        if (file_acc_num == accno) {
            // Account found; update the balance
            balance += amt;
            found = 1;
        }
        // Write back the line (updated if matched)
        fprintf(tempFile, "%lld,%.2lf\n", file_acc_num, balance);
    }
    // Append new account if not found
    if (!found) {
        fprintf(tempFile, "%lld,%.2lf\n", accno, amt);
    }
    fclose(file);
    fclose(tempFile);
    // Replace the original file with the updated file
    if (remove("balance.csv") != 0) {
        printf("Error removing original file.\n");
        return 0;
    }
    if (rename("temp.csv", "balance.csv") != 0) {
        printf("Error renaming temp file.\n");
        return 0;
    }
    printf("Transaction Successful!\n");
    return 1;
}
#endif // PAGES_H