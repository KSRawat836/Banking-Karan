#ifndef HOMEPAGE_H
#define HOMEPAGE_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <transfer.h>
#include <deposit.h>
#include <withdraw.h>
#include <log.h>
#include <chckbal.h>

// Constants
int loging(long fromAccountNumber, long toAccountNumber, double amount);
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800

int homepage(long long acc_no) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Crest Banking");
    SetTargetFPS(60);

    Texture2D Logo = LoadTexture("image/logo.png");

    Font customFont = LoadFontEx("resources/Roboto-Regular.ttf", 64, 0, 0);
    GenTextureMipmaps(&customFont.texture);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);

    if (customFont.texture.id == 0) {
        printf("Failed to load font!\n");
        CloseWindow();
        return -1;
    }

    // Boolean Values
    bool button1Active = false;
    bool button2Active = false;
    bool button3Active = false;
    bool button4Active = false;
    bool button5Active = false; // New button for Log
    bool logoutActive = false;

    while (!WindowShouldClose()) {
        Color peach = { 255, 218, 185, 255 };  // Peach color (Soft warm tone)

        BeginDrawing();
        ClearBackground(peach);

        Vector2 mouse = GetMousePosition();

        const char* intro = "Home Page";
        Vector2 intro_textsize = MeasureTextEx(customFont, intro, 64, 4);
        Vector2 intro_position = {(SCREEN_WIDTH - Logo.width - intro_textsize.x) / 2 + Logo.width - 20, (Logo.height - intro_textsize.y) / 2};

        // Text of intro
        DrawTextEx(customFont, intro, intro_position, 64, 4, BLACK);

        // Logo of the app
        DrawTexture(Logo, 10, 0, WHITE);

        int buttonsheight = 100;
        int buttonswidth = 500;

        // Button 1: Check Balance
        Rectangle button1 = { (SCREEN_WIDTH / 2 - buttonswidth) / 2 + 20, Logo.height + 10, buttonswidth, buttonsheight };
        DrawRectangleRounded(button1, 0.3, 10, button1Active ? GRAY : LIGHTGRAY);

        const char* text1 = "Check Balance";
        Vector2 text1_size = MeasureTextEx(customFont, text1, 30, 1);
        Vector2 text1_position = { button1.x + (button1.width - text1_size.x) / 2, button1.y + (button1.height - text1_size.y) / 2 };
        DrawTextEx(customFont, text1, text1_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, button1)) {
            button1Active = true;
        } else {
            button1Active = false;
        }
        if (CheckCollisionPointRec(mouse, button1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            chckbal(acc_no);
            return homepage(acc_no);
        }

        // Button 2: Deposit
        Rectangle button2 = { (SCREEN_WIDTH / 2 - buttonswidth) / 2 + 20, Logo.height + button1.height + 10 * 2, buttonswidth, buttonsheight };
        DrawRectangleRounded(button2, 0.3, 10, button2Active ? GRAY : LIGHTGRAY);

        const char* text2 = "Deposit";
        Vector2 text2_size = MeasureTextEx(customFont, text2, 30, 1);
        Vector2 text2_position = { button2.x + (button2.width - text2_size.x) / 2, button2.y + (button2.height - text2_size.y) / 2 };
        DrawTextEx(customFont, text2, text2_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, button2)) {
            button2Active = true;
        } else {
            button2Active = false;
        }
        if (CheckCollisionPointRec(mouse, button2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            deposit(acc_no);
            return homepage(acc_no);
        }

        // Button 3: Withdraw
        Rectangle button3 = { SCREEN_WIDTH / 2 + (SCREEN_WIDTH / 2 - buttonswidth) / 2 - 20, Logo.height + 10, buttonswidth, buttonsheight };
        DrawRectangleRounded(button3, 0.3, 10, button3Active ? GRAY : LIGHTGRAY);

        const char* text3 = "Withdraw";
        Vector2 text3_size = MeasureTextEx(customFont, text3, 30, 1);
        Vector2 text3_position = { button3.x + (button3.width - text3_size.x) / 2, button3.y + (button3.height - text3_size.y) / 2 };
        DrawTextEx(customFont, text3, text3_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, button3)) {
            button3Active = true;
        } else {
            button3Active = false;
        }
        if (CheckCollisionPointRec(mouse, button3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            withdraw(acc_no);
            return homepage(acc_no);
        }

        // Button 4: Transfer
        Rectangle button4 = { SCREEN_WIDTH / 2 + (SCREEN_WIDTH / 2 - buttonswidth) / 2 - 20, Logo.height + button1.height + 10 * 2, buttonswidth, buttonsheight };
        DrawRectangleRounded(button4, 0.3, 10, button4Active ? GRAY : LIGHTGRAY);

        const char* text4 = "Transfer";
        Vector2 text4_size = MeasureTextEx(customFont, text4, 30, 1);
        Vector2 text4_position = { button4.x + (button4.width - text4_size.x) / 2, button4.y + (button4.height - text4_size.y) / 2 };
        DrawTextEx(customFont, text4, text4_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, button4)) {
            button4Active = true;
        } else {
            button4Active = false;
        }
        if (CheckCollisionPointRec(mouse, button4) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            transfer(acc_no);
            return homepage(acc_no);
        }

        // Button 5: Log (New Button)
        Rectangle button5 = { (SCREEN_WIDTH / 2 - buttonswidth) / 2 + 20, Logo.height + button2.height + button1.height + 10 * 4, buttonswidth, buttonsheight };
        DrawRectangleRounded(button5, 0.3, 10, button5Active ? GRAY : LIGHTGRAY);

        const char* text5 = "Log";
        Vector2 text5_size = MeasureTextEx(customFont, text5, 30, 1);
        Vector2 text5_position = { button5.x + (button5.width - text5_size.x) / 2, button5.y + (button5.height - text5_size.y) / 2 };
        DrawTextEx(customFont, text5, text5_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, button5)) {
            button5Active = true;
        } else {
            button5Active = false;
        }
        if (CheckCollisionPointRec(mouse, button5) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            checklog(acc_no);
            return homepage(acc_no);
        }

        // Logout Button (Centered Horizontally)
        Rectangle button6 = { (SCREEN_WIDTH - buttonswidth) / 2 + SCREEN_WIDTH/4 -20, Logo.height + button2.height + button1.height + 10 * 4, buttonswidth, buttonsheight };
        DrawRectangleRounded(button6, 0.3, 10, logoutActive ? GRAY : LIGHTGRAY);

        const char* text6 = "Logout";
        Vector2 text6_size = MeasureTextEx(customFont, text6, 30, 1);
        Vector2 text6_position = { button6.x + (button6.width - text6_size.x) / 2, button6.y + (button6.height - text6_size.y) / 2 };
        DrawTextEx(customFont, text6, text6_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, button6)) {
            logoutActive = true;
        } else {
            logoutActive = false;
        }
        if (CheckCollisionPointRec(mouse, button6) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            system("Exit");
        }

        EndDrawing();
    }

    UnloadTexture(Logo);
    UnloadFont(customFont);

    CloseWindow();
    return 0;
}
#endif //HOMEPAGE_H