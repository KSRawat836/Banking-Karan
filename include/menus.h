#ifndef MENUS_H
#define MENUS_H

// Function declarations
#include <stdio.h>

void welcome();
void loginPage();
void block3();


void welcome(){
	printf("--------------------Banking-------------------\n");
}

void loginPage(){
	printf("1.Login.\n");
	printf("2.SignUp.\n");
}

void block3(){
	printf("1.Show Account Balance.\n");
	printf("2.Withdraw the Amount.\n");
	printf("3.Deposit a Amount.\n");
	printf("4.Transfer amount.\n");
	printf("5.Log out.\n");
}
#endif // MENUS_H
