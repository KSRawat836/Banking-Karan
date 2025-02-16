#ifndef TRANSFER_H
#define TRANSFER_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>

int logt(long fromAccountNumber, long toAccountNumber, double amount) {
    // Get the current date and time
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);

    // Open the transaction log file in append mode
    FILE *logFile = fopen("transfer_log.csv", "a");
    if (logFile == NULL) {
        printf("Error opening log file.\n");
        return 0;
    }

    // Write the transaction details to the CSV file
    fprintf(logFile, "%lld,%lld,%lf,%s\n", fromAccountNumber, toAccountNumber, amount, timeStr);

    fclose(logFile);
    return 0;
}




double tran_balance(long long account_number) {
    FILE *file = fopen("balance.csv", "r");
    if (!file) {
        printf("Error opening balance.csv\n");
        return -1.0;  // Return an invalid balance to indicate failure
    }
    char line[100];
    long long acc_no;
    double balance;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%lld,%lf", &acc_no, &balance);
        if (acc_no == account_number) {
            fclose(file);
            return balance;
        }
    }
    printf("Account %lld not found in balance.csv\n", account_number);
    fclose(file);
    return -1.0;  // Return -1 if account is not found
}


int tnsf(long long fromacc, long long toacc, long long amt) {
    FILE *file = fopen("balance.csv", "r");
    FILE *temp = fopen("temp.csv", "w");
    
    if (!file || !temp) {
        if (file) fclose(file);
        if (temp) fclose(temp);
        return -1;  // Return error if files can't be opened
    }

    char line[100];
    long long csvacc;
    double balance;
    double frombal = -1, tobal = -1;
    bool from_found = false, to_found = false;

    // Read balances
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%lld,%lf", &csvacc, &balance);
        if (csvacc == fromacc) {
            frombal = balance;
            from_found = true;
        } 
        if (csvacc == toacc) {
            tobal = balance;
            to_found = true;
        }
    }
    rewind(file);

    // If either account is missing, or insufficient funds, abort
    if (!from_found || !to_found || frombal < amt) {
        fclose(file);
        fclose(temp);
        remove("temp.csv");
        return -1;
    }

    // Update balances
    frombal -= amt;
    tobal += amt;

    // Write updated balances to temp file
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%lld,%lf", &csvacc, &balance);
        if (csvacc == fromacc) {
            fprintf(temp, "%lld,%.2f\n", csvacc, frombal);
        } else if (csvacc == toacc) {
            fprintf(temp, "%lld,%.2f\n", csvacc, tobal);
        } else {
            fprintf(temp, "%lld,%.2f\n", csvacc, balance);
        }
    }

    fclose(file);
    fclose(temp);

    // Replace balance.csv with updated data
    remove("balance.csv");
    rename("temp.csv", "balance.csv");

    return 0;  // Success
}
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800



// Function to convert input_amount to double
double getAmntValue(const char *input_amount) {
    if (input_amount[0] == '\0'){
        return 0.0;
    }  
    return strtod(input_amount, NULL);  
}

int transfer(long long acc_no){
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Crest Banking");
    SetTargetFPS(60);

    Texture2D Logo = LoadTexture("image/logosignup.png");

    Logo.width /= 1.1;
    Logo.height /= 1.1;

    Font customFont = LoadFontEx("resources/Roboto-Regular.ttf", 64, 0, 0);
    GenTextureMipmaps(&customFont.texture);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);

    if (customFont.texture.id == 0) {
        printf("Failed to load font!\n");
        CloseWindow();
        return -1;
    }

    // data of user 
    // balance user_info;

    // input fields
    char input_accno[12] = "";
    char input_amount[100] = "";
    int count = 0; 
    char *statusmessage = "";   

    // double balance = user_info.amount; // Assuming balance is stored in `user_info`

    // Boolean Values
    bool accnoActive = false;
    bool accnoActive_ph = true;
    bool amountActive = false;
    bool amountActive_ph = true;
    bool withbuttonActive = false;
    bool statusmessageActive = false;
    bool status = false; 
    bool exitbuttonActive = false;

    while (!WindowShouldClose()) {

        Color peach = { 255, 218, 185, 255 };  // Peach color (Soft warm tone)

        BeginDrawing();
        ClearBackground(peach);

        Vector2 mouse = GetMousePosition();

        const char* intro = "Transfer Page";
        Vector2 intro_textsize = MeasureTextEx(customFont, intro, 64, 4);
        Vector2 intro_position = {(SCREEN_WIDTH - Logo.width - intro_textsize.x)/2 + Logo.width - 20,(Logo.height - intro_textsize.y)/2};

        // text of intro

        DrawTextEx(customFont, intro, intro_position, 64, 4, BLACK);

        // logo of the app

        DrawTexture(Logo, 100, 0, WHITE);

        int txtboxheight = 60;
        int txtboxwidth = 600;
        int buttonsheight = 70;
        int buttonswidth = 200;

        // text box for Account Number

        Rectangle accnobox = { SCREEN_WIDTH/3, Logo.height + 150, txtboxwidth, txtboxheight };
        DrawRectangleRounded(accnobox, 0.3, 10, LIGHTGRAY);

        DrawRectangleRoundedLinesEx(accnobox, 0.3, 10, 0.2, accnoActive ? BLACK : LIGHTGRAY);

        const char *accnolabel = "Account Number:";
        Vector2 accnolabel_textsize = MeasureTextEx(customFont, accnolabel, 30, 1);
        Vector2 accnolabel_position = {accnobox.x - accnolabel_textsize.x - 20, accnobox.y + (accnobox.height - accnolabel_textsize.y)/2};

        DrawTextEx(customFont, accnolabel, accnolabel_position, 30, 1, BLACK);

        const char *accno_ph = "Enter Account Number to Tranfer";
        Vector2 accnoph_textsize = MeasureTextEx(customFont, accno_ph, 25, 1);
        Vector2 accnoph_position = {accnobox.x + (accnobox.width - accnoph_textsize.x)/2,accnobox.y + (accnobox.height - accnoph_textsize.y)/2};

        DrawTextEx(customFont, accno_ph, accnoph_position, 25, 1, accnoActive_ph ? BLACK : LIGHTGRAY);

        if (CheckCollisionPointRec(mouse, accnobox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            accnoActive = true;
            amountActive = false;
        }

        if (strlen(input_accno) != 0 ) {
            accnoActive_ph = false;
        }

        if ( !accnoActive && strlen(input_accno) == 0 ){
            accnoActive_ph = true;
        }

        Vector2 accno_textsize = MeasureTextEx(customFont, input_accno, 25, 1);
        Vector2 accno_position = {accnobox.x + (accnobox.width - accno_textsize.x)/2,accnobox.y + (accnobox.height - accno_textsize.y)/2};

        if (strlen(input_accno)!=0){
            DrawTextEx(customFont, input_accno, accno_position, 25, 1, BLACK );
        }


        // text box for Amount

        Rectangle amountbox = { SCREEN_WIDTH/3, accnobox.y + txtboxheight + 20, txtboxwidth, txtboxheight };
        DrawRectangleRounded(amountbox, 0.3, 10, LIGHTGRAY);

        DrawRectangleRoundedLinesEx(amountbox, 0.3, 10, 0.2, amountActive ? BLACK : LIGHTGRAY);

        const char *amountlabel = "Amount:";
        Vector2 amountlabel_textsize = MeasureTextEx(customFont, amountlabel, 30, 1);
        Vector2 amountlabel_position = {amountbox.x - amountlabel_textsize.x - 20, amountbox.y + (amountbox.height - amountlabel_textsize.y)/2};

        DrawTextEx(customFont, amountlabel, amountlabel_position, 30, 1, BLACK);

        const char *amount_ph = "Enter Amount to Transfer";
        Vector2 amountph_textsize = MeasureTextEx(customFont, amount_ph, 25, 1);
        Vector2 amountph_position = {amountbox.x + (amountbox.width - amountph_textsize.x)/2,amountbox.y + (amountbox.height - amountph_textsize.y)/2};

        DrawTextEx(customFont, amount_ph, amountph_position, 25, 1, amountActive_ph ? BLACK : LIGHTGRAY);

        if (CheckCollisionPointRec(mouse, amountbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            amountActive = true;
            accnoActive = false;
        }

        if (strlen(input_amount) != 0 ) {
            amountActive_ph = false;
        }

        if ( !amountActive && strlen(input_amount) == 0 ){
            amountActive_ph = true;
        }

        Vector2 amount_textsize = MeasureTextEx(customFont, input_amount, 25, 1);
        Vector2 amount_position = {amountbox.x + (amountbox.width - amount_textsize.x)/2,amountbox.y + (amountbox.height - amount_textsize.y)/2};

        if (strlen(input_amount)!=0){
            DrawTextEx(customFont, input_amount, amount_position, 25, 1, BLACK );
        }

        // Withdraw Button
        Rectangle withbutton = { SCREEN_WIDTH/2 - buttonswidth-20 , amountbox.y + amountbox.height + 10*8, buttonswidth, buttonsheight};
        DrawRectangleRounded(withbutton, 0.3, 10, withbuttonActive ? GRAY : LIGHTGRAY);

        const char *buttonlabel = "Transfer";

        Vector2 buttonlabel_size = MeasureTextEx(customFont, buttonlabel, 30, 1);
        Vector2 buttonlabel_position = {withbutton.x + (withbutton.width - buttonlabel_size.x)/2, withbutton.y + (withbutton.height - buttonlabel_size.y)/2};

        DrawTextEx(customFont, buttonlabel, buttonlabel_position, 30, 1, !withbuttonActive ? BLACK : WHITE);

        if (CheckCollisionPointRec(mouse, withbutton)){
            withbuttonActive = true;
        }else{
            withbuttonActive = false;
        }
        

        Rectangle exitbutton = { SCREEN_WIDTH/2 + 20, amountbox.y + amountbox.height + 10*8, buttonswidth, buttonsheight};
        DrawRectangleRounded(exitbutton, 0.3, 10, exitbuttonActive ? GRAY : LIGHTGRAY);
        const char *exitlabel = "Back";

        Vector2 exitlabel_size = MeasureTextEx(customFont, exitlabel, 30, 1);
        Vector2 exitlabel_position = {exitbutton.x + (exitbutton.width - exitlabel_size.x)/2, exitbutton.y + (exitbutton.height - exitlabel_size.y)/2};

        DrawTextEx(customFont, exitlabel, exitlabel_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, exitbutton)){
            exitbuttonActive = true;
        }else{
            exitbuttonActive = false;
        }

                if (CheckCollisionPointRec(mouse, exitbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            break;
        }


        // Keyboard Interaction

        int key = GetCharPressed();

        if (amountActive) {
            if (IsKeyPressed(KEY_BACKSPACE) && strlen(input_amount) > 0) {
                if (input_amount[strlen(input_amount) - 1] == '.') {
                    count = 0; // Reset decimal flag if '.' is deleted
                }
                input_amount[strlen(input_amount) - 1] = '\0';
            }

            if (key >= 48 && key <= 57 && strlen(input_amount) < 100) { // Digits 0-9
                input_amount[strlen(input_amount)] = (char)key;
                input_amount[strlen(input_amount) + 1] = '\0';
            }

            if (key == 46 && count == 0 && strlen(input_amount) > 0) { // Allow one '.'
                input_amount[strlen(input_amount)] = '.';
                input_amount[strlen(input_amount) + 1] = '\0';
                count = 1; // Decimal point added
            }

            // Restrict decimal places to 2 digits after '.'
            char *dotPos = strchr(input_amount, '.');
            if (dotPos != NULL && strlen(dotPos + 1) > 2) { 
                input_amount[strlen(input_amount) - 1] = '\0'; // Remove extra digits
            }
        }
        // Keyboard Interaction for Account Number Field
        if (accnoActive) {
            if (IsKeyPressed(KEY_BACKSPACE) && strlen(input_accno) > 0) {
                input_accno[strlen(input_accno) - 1] = '\0';
                 }

             if (key >= 48 && key <= 57 && strlen(input_accno) < 11) { // Digits 0-9, limit to 11 chars
                 input_accno[strlen(input_accno)] = (char)key;
            input_accno[strlen(input_accno) + 1] = '\0';
    }
}


        // amount in double

        double amount = getAmntValue(input_amount);
        double balance = tran_balance(acc_no);


        if (CheckCollisionPointRec(mouse,withbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if (amount > balance) {
                statusmessage = "Insufficient funds!";
                statusmessageActive = false;
                status = true;

            } else {
                long long to_acc = strtoll(input_accno,NULL,10);
                tnsf(acc_no,to_acc,amount);
                logt(acc_no,to_acc,-amount);
                statusmessage = "Transfer Successfull!";
                statusmessageActive = true;
                status = true;
            }
        }

       // status showing

        Vector2 statusmessage_textsize = MeasureTextEx(customFont, statusmessage, 25, 1);
        Vector2 statusmessage_position = {(SCREEN_WIDTH - statusmessage_textsize.x)/2, Logo.height + 50};


        if (status){
            DrawTextEx(customFont, statusmessage, statusmessage_position, 25, 1, statusmessageActive ? GREEN : RED );
        }


        EndDrawing();
    }


    UnloadTexture(Logo);
    UnloadFont(customFont);


    CloseWindow();
    return 0;
}





#endif //TRANSFER.H
