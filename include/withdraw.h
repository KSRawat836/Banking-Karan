#ifndef WITHDRAW_H
#define WITHDRAW_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <homepage.h>

// Constants

int logw(long fromAccountNumber, long toAccountNumber, double amount) {
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


double with_balance(long long account_number) {
    FILE *file = fopen("balance.csv", "r");
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

int withBalance(long long account_number, double amount) {
    FILE *file = fopen("balance.csv", "r");  
    char line[256];
    long long acc;
    double balance;
    int found = 0;
    FILE *temp = fopen("temp.csv", "w");   
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%lld,%lf", &acc, &balance);
        if (acc == account_number) {
            balance -= amount;
            found = 1;
        }
        fprintf(temp, "%lld,%.2f\n", acc, balance);
    }
    
    fclose(file);
    fclose(temp);
    
    if (found) {
        remove("balance.csv");
        rename("temp.csv", "balance.csv");
        return 1; // Successful withdrawal
    } else {
        remove("temp.csv");
        return -1; // Account not found
    }
}
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800

// Function to convert input_amount to double

double getAmtValue(const char *input_amount) 
{
    if (input_amount[0] == '\0')
    {
        return 0.0;
    }  
    return strtod(input_amount, NULL);  
}

int withdraw(long long account_number)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Creast Banking");
    SetTargetFPS(60);
    Texture2D Logo = LoadTexture("image/logosignup.png");
    Logo.width /= 1.1;
    Logo.height /= 1.1;
    Font customFont = LoadFontEx("resources/Roboto-Regular.ttf", 64, 0, 0);
    GenTextureMipmaps(&customFont.texture);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);
    if (customFont.texture.id == 0) 
    {
        printf("Failed to load font!\n");
        CloseWindow();
        return -1;
    }
    bool amountActive = false;
    bool amountActive_ph = true;
    bool withbuttonActive = false;
    bool statusmessageActive = false;
    bool status = false; 
    bool exitbuttonActive = false;
    char input_amount[100] = "";
    int count = 0; 
    char *statusmessage = "";   

    while (!WindowShouldClose()) 
    {
        Color peach = { 255, 218, 185, 255 };  // Peach color (Soft warm tone)
        BeginDrawing();
        ClearBackground(peach);
        Vector2 mouse = GetMousePosition();
        const char* intro = "Withdraw Page";
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

        // text box for name
        Rectangle amountbox = { SCREEN_WIDTH/3, Logo.height + 150, txtboxwidth, txtboxheight };
        DrawRectangleRounded(amountbox, 0.3, 10, LIGHTGRAY);
        DrawRectangleRoundedLinesEx(amountbox, 0.3, 10, 0.2, amountActive ? BLACK : LIGHTGRAY);
        const char *amountlabel = "Amount:";
        Vector2 amountlabel_textsize = MeasureTextEx(customFont, amountlabel, 30, 1);
        Vector2 amountlabel_position = {amountbox.x - amountlabel_textsize.x - 20, amountbox.y + (amountbox.height - amountlabel_textsize.y)/2};
        DrawTextEx(customFont, amountlabel, amountlabel_position, 30, 1, BLACK);

        const char *amount_ph = "Enter Amount to Withdraw";
        Vector2 amountph_textsize = MeasureTextEx(customFont, amount_ph, 25, 1);
        Vector2 amountph_position = {amountbox.x + (amountbox.width - amountph_textsize.x)/2,amountbox.y + (amountbox.height - amountph_textsize.y)/2};
        DrawTextEx(customFont, amount_ph, amountph_position, 25, 1, amountActive_ph ? BLACK : LIGHTGRAY);

        if (CheckCollisionPointRec(mouse, amountbox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            amountActive = true;
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
        Rectangle withbutton = { SCREEN_WIDTH/2 - buttonswidth-20    , amountbox.y + amountbox.height + 10*8, buttonswidth, buttonsheight};
        DrawRectangleRounded(withbutton, 0.3, 10, withbuttonActive ? GRAY : LIGHTGRAY);

        const char *buttonlabel = "Withdraw";

        Vector2 buttonlabel_size = MeasureTextEx(customFont, buttonlabel, 30, 1);
        Vector2 buttonlabel_position = {withbutton.x + (withbutton.width - buttonlabel_size.x)/2, withbutton.y + (withbutton.height - buttonlabel_size.y)/2};

        DrawTextEx(customFont, buttonlabel, buttonlabel_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, withbutton)){
            withbuttonActive = true;
        }else{
            withbuttonActive = false;
        }
        

         //Back button
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


        // amount in double

        double amount = getAmtValue(input_amount);
        double balance = with_balance(account_number);

        if (CheckCollisionPointRec(mouse,withbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            if (amount > balance) {
                statusmessage = "Insufficient funds!";
                statusmessageActive = false;
                status = true;

            } else {
                withBalance(account_number,amount);
                logw(account_number,account_number,-amount);
                statusmessage = "Withdrawal successful!";
                statusmessageActive = true;
                status = true;
            }
        }

       // status showing

        Vector2 statusmessage_textsize = MeasureTextEx(customFont, statusmessage, 25, 1);
        Vector2 statusmessage_position = {(SCREEN_WIDTH - statusmessage_textsize.x)/2, Logo.height + 50};

        if (CheckCollisionPointRec(mouse, exitbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            break;
        }


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

#endif //WITHDRAW_H