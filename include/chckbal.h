#ifndef CHCKBAL_H
#define CHCKBAL_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>

// Constants
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800

int chckbal(long long accno) {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Creast Banking");
    SetTargetFPS(60);

    // Load logo texture
    Texture2D Logo = LoadTexture("image/logosignup.png");
    if (Logo.id == 0) {
        printf("Failed to load logo texture!\n");
        CloseWindow();
        return -1;
    }
    Logo.width /= 1.25;
    Logo.height /= 1.25;
    GenTextureMipmaps(&Logo);
    SetTextureFilter(Logo, TEXTURE_FILTER_BILINEAR);

    // Load custom font
    Font customFont = LoadFontEx("resources/Roboto-Regular.ttf", 64, 0, 0);
    if (customFont.texture.id == 0) {
        printf("Failed to load font!\n");
        UnloadTexture(Logo);
        CloseWindow();
        return -1;
    }
    GenTextureMipmaps(&customFont.texture);
    SetTextureFilter(customFont.texture, TEXTURE_FILTER_BILINEAR);

    // Read account balance
    double balance = 0.0; // Declare balance variable
    FILE *file = fopen("balance.csv", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        UnloadTexture(Logo);
        UnloadFont(customFont);
        CloseWindow();
        return -1;
    }

    long long accbuffer;
    double balancebuffer;
    while (fscanf(file, "%lld,%lf\n", &accbuffer, &balancebuffer) != EOF) {
        if (accbuffer == accno) {
            balance = balancebuffer;
            break;
        }
    }
    fclose(file);

    // Convert balance to string
    char accno_str[13];
    char balance_str[50];
    snprintf(accno_str, sizeof(accno_str), "%lld", accno);
    snprintf(balance_str, sizeof(balance_str), "%.2lf", balance);

    // Buttons
    bool tranbuttonActive = false;
    bool backbuttonActive = false;

    while (!WindowShouldClose()) {
        Color peach = { 255, 218, 185, 255 };  // Peach color (Soft warm tone)

        BeginDrawing();
        ClearBackground(peach);

        Vector2 mouse = GetMousePosition();

        // Draw intro text
        const char* intro = "Check Balance & Transaction History";
        Vector2 intro_textsize = MeasureTextEx(customFont, intro, 40, 1);
        Vector2 intro_position = { (SCREEN_WIDTH - Logo.width - intro_textsize.x) / 2 + Logo.width - 20, (Logo.height - intro_textsize.y) / 2 };
        DrawTextEx(customFont, intro, intro_position, 40, 1, BLACK);

        // Draw logo
        DrawTexture(Logo, 10, 0, WHITE);

        // Text box for account number
        int txtboxheight = 60;
        int txtboxwidth = 600;

        Rectangle accnobox = { SCREEN_WIDTH / 3, Logo.height + 100, txtboxwidth, txtboxheight };
        DrawRectangleRounded(accnobox, 0.3, 10, LIGHTGRAY);

        const char* accnolabel = "Account No.:";
        Vector2 accnolabel_textsize = MeasureTextEx(customFont, accnolabel, 30, 1);
        Vector2 accnolabel_position = { accnobox.x - accnolabel_textsize.x - 20, accnobox.y + (accnobox.height - accnolabel_textsize.y) / 2 };
        DrawTextEx(customFont, accnolabel, accnolabel_position, 30, 1, BLACK);

        Vector2 accno_textsize = MeasureTextEx(customFont, accno_str, 30, 2);
        Vector2 accno_position = { accnobox.x + (accnobox.width - accno_textsize.x) / 2, accnobox.y + (accnobox.height - accnolabel_textsize.y) / 2 };
        DrawTextEx(customFont, accno_str, accno_position, 30, 2, BLACK);

        // Text box for balance
        Rectangle balancebox = { SCREEN_WIDTH / 3, accnobox.y + 100, txtboxwidth, txtboxheight };
        DrawRectangleRounded(balancebox, 0.3, 10, LIGHTGRAY);

        const char* balancelabel = "Balance:";
        Vector2 balancelabel_textsize = MeasureTextEx(customFont, balancelabel, 30, 1);
        Vector2 balancelabel_position = { balancebox.x - balancelabel_textsize.x - 20, balancebox.y + (balancebox.height - balancelabel_textsize.y) / 2 };
        DrawTextEx(customFont, balancelabel, balancelabel_position, 30, 1, BLACK);

        Vector2 balance_textsize = MeasureTextEx(customFont, balance_str, 30, 2);
        Vector2 balance_position = { balancebox.x + (balancebox.width - balance_textsize.x) / 2, balancebox.y + (balancebox.height - balancelabel_textsize.y) / 2 };
        DrawTextEx(customFont, balance_str, balance_position, 30, 2, BLACK);

        // Back and transaction history buttons
        Rectangle backbutton = { SCREEN_WIDTH / 2 - 320, balancebox.y + 100, 300, balancebox.height - 10 };
        DrawRectangleRounded(backbutton, 0.3, 10, backbuttonActive ? GRAY : LIGHTGRAY);

        const char* backtext = "Back";
        Vector2 back_textsize = MeasureTextEx(customFont, backtext, 30, 1);
        Vector2 backtext_position = { backbutton.x + (backbutton.width - back_textsize.x) / 2, backbutton.y + (backbutton.height - back_textsize.y) / 2 };
        DrawTextEx(customFont, backtext, backtext_position, 30, 1, !backbuttonActive ? BLACK : WHITE);

        Rectangle transactionbutton = { SCREEN_WIDTH / 2 + 20, backbutton.y, 300, balancebox.height - 10 };
        DrawRectangleRounded(transactionbutton, 0.3, 10, tranbuttonActive ? GRAY : LIGHTGRAY);

        const char* transactiontext = "Transaction History";
        Vector2 transaction_textsize = MeasureTextEx(customFont, transactiontext, 30, 1);
        Vector2 transactiontext_position = { transactionbutton.x + (transactionbutton.width - transaction_textsize.x) / 2, transactionbutton.y + (transactionbutton.height - transaction_textsize.y) / 2 };
        DrawTextEx(customFont, transactiontext, transactiontext_position, 30, 1, !tranbuttonActive ? BLACK : WHITE);

        // Handle button hover
        if (CheckCollisionPointRec(mouse, backbutton)) {
            backbuttonActive = true;
        } else {
            backbuttonActive = false;
        }

        if (CheckCollisionPointRec(mouse, transactionbutton)) {
            tranbuttonActive = true;
        } else {
            tranbuttonActive = false;
        }

        // Handle button clicks
        if (CheckCollisionPointRec(mouse, backbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            return 0; // Return to the homepage
        }

        if (CheckCollisionPointRec(mouse, transactionbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
            checklog(accno); // Open transaction history
            return 0;
        }

        // Draw line between buttons
        Vector2 startline = { SCREEN_WIDTH / 2, balancebox.y + balancebox.height + 35 };
        Vector2 endline = { SCREEN_WIDTH / 2, balancebox.y + balancebox.height * 2 + 35 };
        DrawLineEx(startline, endline, 2, GRAY);

        EndDrawing();
    }

    // Cleanup
    UnloadTexture(Logo);
    UnloadFont(customFont);
    CloseWindow();
    return 0;
}

#endif // CHCKBAL_H