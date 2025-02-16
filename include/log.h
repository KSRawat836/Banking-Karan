#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>



// Constants
#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 800
#define MAX_TRANSACTIONS 1000
#define ROWS_PER_PAGE 10

int parseTransactions(const char *filename, long long target_acc, char *transactions[MAX_TRANSACTIONS], int *total) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    *total = 0;
    while (fgets(line, sizeof(line), file)) {
        long long to_acc = 0, from_acc = 0;
        double amount = 0;
        char date[11] = "", time[9] = "";

        // Improved sscanf parsing
        if (sscanf(line, " %lld , %lld , %lf , %10s %8s", &to_acc, &from_acc, &amount, date, time) == 5) {
            // Debug print to check parsed values
            printf("Parsed: to_acc=%lld, from_acc=%lld, amount=%.2f, date=%s, time=%s\n", to_acc, from_acc, amount, date, time);

            // Check if either account matches
            if (from_acc == target_acc || to_acc == target_acc) {
                transactions[*total] = strdup(line);
                (*total)++;
                if (*total >= MAX_TRANSACTIONS) break;
            }
        } else {
            printf("Error parsing line: %s\n", line);
        }
    }

    fclose(file);
    return 0;
}


int checklog(long long acc_no) {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Transaction Log");
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

    // Parse transactions
    char *transactions[MAX_TRANSACTIONS];
    int total_transactions = 0;
    if (parseTransactions("transfer_log.csv", acc_no, transactions, &total_transactions) == -1) {
        UnloadTexture(Logo);
        UnloadFont(customFont);
        CloseWindow();
        return -1;
    }

    // Main loop
    int page = 0;
    bool exitbuttonActive = false;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){255, 218, 185, 255}); // Peach color

        Vector2 mouse = GetMousePosition();
        DrawTexture(Logo, 10, 0, WHITE);

        // Back button
        Rectangle exitbutton = {SCREEN_WIDTH - 120, 20, 100, 50};
        DrawRectangleRounded(exitbutton, 0.3, 10, exitbuttonActive ? GRAY : LIGHTGRAY);

        const char *exitlabel = "Back";
        Vector2 exitlabel_size = MeasureTextEx(customFont, exitlabel, 30, 1);
        Vector2 exitlabel_position = {exitbutton.x + (exitbutton.width - exitlabel_size.x) / 2, exitbutton.y + (exitbutton.height - exitlabel_size.y) / 2};

        DrawTextEx(customFont, exitlabel, exitlabel_position, 30, 1, BLACK);

        if (CheckCollisionPointRec(mouse, exitbutton)) {
            exitbuttonActive = true;
        } else {
            exitbuttonActive = false;
        }

        // Handle back button click
        if (CheckCollisionPointRec(mouse, exitbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            break; // Exit the loop and return to the previous screen
        }

        // Pagination buttons (next and previous)
        Rectangle nextbutton = {SCREEN_WIDTH - 70, 120, 50, 50};
        Rectangle previousbutton = {SCREEN_WIDTH - 140, 120, 50, 50};

        if (CheckCollisionPointRec(mouse, nextbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (page + 1) * ROWS_PER_PAGE < total_transactions) {
            page++;
        }
        if (CheckCollisionPointRec(mouse, previousbutton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && page > 0) {
            page--;
        }

        DrawRectangleRounded(nextbutton, 0.3, 10, LIGHTGRAY);
        DrawRectangleRounded(previousbutton, 0.3, 10, LIGHTGRAY);
        DrawTextEx(customFont, ">", (Vector2){nextbutton.x + 18, nextbutton.y + 10}, 30, 1, BLACK);
        DrawTextEx(customFont, "<", (Vector2){previousbutton.x + 18, previousbutton.y + 10}, 30, 1, BLACK);

        // Draw transaction table
        Rectangle table = {25, 200, SCREEN_WIDTH - 50, SCREEN_HEIGHT - 230};
        DrawRectangleRounded(table, 0.02, 10, LIGHTGRAY);
        DrawRectangleRoundedLinesEx(table, 0.02, 10, 2, BLACK);

        const char *header_labels[5] = {"To Acc-No", "Date", "Time", "Type", "Amount"};
        for (int i = 0; i < 5; i++) {
            Vector2 text_position = {table.x + i * (table.width / 5) + 10, table.y + 10};
            DrawTextEx(customFont, header_labels[i], text_position, 25, 1, BLACK);
        }

        for (int i = 0; i < ROWS_PER_PAGE; i++) {
            int index = page * ROWS_PER_PAGE + i;
            if (index >= total_transactions) break;

            long long to_acc, from_acc;
            double amount;
            char date[11], time[9];
            sscanf(transactions[index], "%lld,%lld,%lf,%10s %8s", &to_acc, &from_acc, &amount, date, time);

            char amount_str[20];
            snprintf(amount_str, sizeof(amount_str), "%.2f", amount);

            // Correctly assign to_acc and from_acc
            char to_acc_str[20];
            char from_acc_str[20];
            snprintf(to_acc_str, sizeof(to_acc_str), "%lld", to_acc);
            snprintf(from_acc_str, sizeof(from_acc_str), "%lld", from_acc);

            // Determine transaction type
            const char *type = (amount<0) ? "Debit" : "Credit";

            // Populate row_data
            const char *row_data[5] = {to_acc_str, date, time, type, amount_str};

            for (int j = 0; j < 5; j++) {
                Vector2 text_position = {table.x + j * (table.width / 5) + 10, table.y + (i + 1) * 50 + 10};
                DrawTextEx(customFont, row_data[j], text_position, 20, 1, BLACK);
            }
        }
        EndDrawing();
    }

    // Cleanup
    for (int i = 0; i < total_transactions; i++) {
        free(transactions[i]);
    }
    UnloadTexture(Logo);
    UnloadFont(customFont);
    CloseWindow();

    return 0; // Return to the previous screen
}
#endif