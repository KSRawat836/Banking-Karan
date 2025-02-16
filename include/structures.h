#ifndef STRUCTURES_H
#define STRUCTURES_H

struct LoginCSV {
    long long accno;
    char username[10];
    char password[10];
};

struct AccountCSV {
	long long accno;
	char name[100];
	long long AadhaarNo;
	// DOB
	long long Mobile;
};

struct BalanceCSV {
	long long accno;
	double balance;
};

struct TransactionCSV {
	long long accno;
	// date
	// time
	bool type;
	double amount;
}


#endif