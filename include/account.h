#ifndef ACCOUNT_H
#define ACCOUNT_H

typedef struct {
    long long acc_num;
    char name[50];
    long long aadhar_no;
    char DOB[11];
    long long mob_no;
    char passwd[100];
} account;

typedef struct {
    long long int acc_num;
    double balance;
} balance;

#endif