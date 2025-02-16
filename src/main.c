#define RAYGUI_IMPLEMENTATION
#include <raygui.h>	
#include <stdio.h>
#include <stdlib.h>
#include "../include/login_page.h"
#include "../include/homepage.h"
#include "../include/withdraw.h"
#include "../include/deposit.h"
#include "../include/transfer.h"


int main() {
    long long acc_no=1;
    while(acc_no == 1 || acc_no == 0)
    {
        acc_no = login_page();
    }
        homepage(acc_no);

    return 0;
}
