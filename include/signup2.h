#ifndef SIGNUP2_H
#define SIGNUP2_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include "account.h"
#include "login_page.h"
#include "admin.h"

long long signupgui();
long long generateUniqueId();


// Constants
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800
#define MAX_LINE_LENGTH 256
#define MAX_LENGTH 100

long long generateUniqueId() {
    srand(time(NULL));  

    long long newId;
    int exists;

    do {
        newId = (rand() % 9000000000LL) + 1000000000LL;  // Generate a 10-digit ID
        exists = 0;

        FILE *file = fopen("Signup.csv", "r");
        if (file == NULL) {
            perror("Error opening file");
            break;  // If file doesn't exist, consider ID unique
        }

        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file)) {
            long long fileId;
            sscanf(line, "%lld", &fileId);  // Read the ID from the line
            if (fileId == newId) {
                exists = 1;  // ID exists
                break;
            }
        }

        fclose(file);
        if (exists) {
            return generateUniqueId();
        }
    } while (exists);

    return newId;
}

    

long long signupgui() {
    bool nameActive = false, aadharActive = false, dobActive = false;
bool mobActive = false, passwordActive = false;

    char nameBuffer[50] = "";
    char aadharBuffer[20] = "";
    char dobBuffer[11] = "";
    char mobBuffer[20] = "";
    char passwordBuffer[100] = "";

    account acc = {0};
    acc.acc_num = generateUniqueId();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Crest Banking");
    SetTargetFPS(60);

    Texture2D Logo = LoadTexture("image/logo.png");
    Font customFont = LoadFontEx("resources/Roboto-Regular.ttf", 64, 0, 0);
    GenTextureMipmaps(&customFont.texture);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);

    if (customFont.texture.id == 0) {
        printf("Failed to load font!\n");
        CloseWindow();
        return 0;
    }

    bool submitted = false;

    while (!WindowShouldClose()) {
        Color peach = {255, 218, 185, 255};

        BeginDrawing();
        ClearBackground(peach);
        Vector2 mousePos = GetMousePosition();

        DrawTexture(Logo, (SCREEN_WIDTH - Logo.width) / 2, 90, WHITE);
        int yinc = 400;

        DrawText(TextFormat("Your account number assigned is: %lld", acc.acc_num), 300, yinc, 20, DARKGRAY);
        yinc += 50;

        Rectangle Name = {485, yinc - 5, 300, 30};
        DrawText("Name:", 300, yinc, 20, BLACK);
        GuiTextBox(Name, nameBuffer, sizeof(nameBuffer), nameActive);
        yinc += 50;

        Rectangle Aadhar = {485, yinc - 5, 300, 30};
        DrawText("Aadhar number:", 300, yinc, 20, BLACK);
        GuiTextBox(Aadhar, aadharBuffer, sizeof(aadharBuffer), aadharActive);
        yinc += 50;

        Rectangle DOB = {485, yinc - 5, 300, 30};
        DrawText("DOB [yyyy-mm-dd]:", 300, yinc, 20, BLACK);
        GuiTextBox(DOB, dobBuffer, sizeof(dobBuffer), dobActive);
        yinc += 50;

        Rectangle Mobile = {485, yinc - 5, 300, 30};
        DrawText("Mobile number:", 300, yinc, 20, BLACK);
        GuiTextBox(Mobile, mobBuffer, sizeof(mobBuffer), mobActive);
        yinc += 50;

        Rectangle pass = {485, yinc - 5, 300, 30};
        DrawText("Password:", 300, yinc, 20, BLACK);
        GuiTextBox(pass, passwordBuffer, sizeof(passwordBuffer), passwordActive);
        yinc += 50;

        Rectangle submit = {525, yinc, 150, 60};
        GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

        if (GuiButton(submit, "Submit")) {
            // Copy buffer values into struct
            strncpy(acc.name, nameBuffer, sizeof(acc.name) - 1);
            acc.name[sizeof(acc.name) - 1] = '\0';

            acc.aadhar_no = atoll(aadharBuffer);

            strncpy(acc.DOB, dobBuffer, sizeof(acc.DOB) - 1);
            acc.DOB[sizeof(acc.DOB) - 1] = '\0';

            acc.mob_no = atoll(mobBuffer);

            strncpy(acc.passwd, passwordBuffer, sizeof(acc.passwd) - 1);
            acc.passwd[sizeof(acc.passwd) - 1] = '\0';

            // Open files and write data
            FILE *file = fopen("Signup.csv", "a+");
            if (!file) {
                printf("Error: Couldn't open Signup.csv\n");
                return 0;
            }
            fprintf(file, "%lld,%s,%s,%lld,%lld,%s\n", acc.acc_num, acc.name, acc.DOB, acc.aadhar_no, acc.mob_no, acc.passwd);
            fclose(file);


            submitted = true;
        }

        if (submitted) break;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            nameActive = CheckCollisionPointRec(mousePos, Name);
            aadharActive = CheckCollisionPointRec(mousePos, Aadhar);
            dobActive = CheckCollisionPointRec(mousePos, DOB);
            mobActive = CheckCollisionPointRec(mousePos, Mobile);
            passwordActive = CheckCollisionPointRec(mousePos, pass);
        }

        EndDrawing();
    }

                FILE *bal = fopen("balance.csv", "a");
            if (!bal) {
                printf("Error: Couldn't open balance.csv\n");
                return 0;
            }
            fprintf(bal, "%lld,%lf\n", acc.acc_num, 0.00); // add newline to separate records
            fclose(bal);


    UnloadTexture(Logo);
    UnloadFont(customFont);
    return acc.acc_num; // success
}



#endif // SIGNUP2_H
