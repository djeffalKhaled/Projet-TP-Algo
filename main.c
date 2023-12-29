#include "raylib.h"
typedef struct listGUI {
    Rectangle value;
    Color valColor;
    Rectangle next;
    Color nextColor;
    Rectangle pointer;
    Color pointerColor;
} listGUI;
void new_ListGUI(listGUI *list, int x, int y, Color valColor, Color nextColor, Color pointerColor) {
    Rectangle value = { x, y, 100, 80 };
    Rectangle next = { x + 100, y, 100, 80 };
    Rectangle pointer = { x + 200, y + 35, 100, 10};
    list->value = value;
    list->valColor = valColor;
    list->next = next;
    list->nextColor = nextColor;
    list->pointer = pointer;
    list->pointerColor = pointerColor;
}
void draw_ListGUI(listGUI *list, char* text) {
    DrawRectangleRec(list->value, list->valColor);
    DrawRectangleRec(list->next, list->nextColor);
    DrawRectangleRec(list->pointer, list->pointerColor);

    // Draws the value's text
    float centerX = list->value.x + list->value.width / 2;
    float centerY = list->value.y + list->value.height / 2;
    // Measure the text width and height
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
    DrawText(text, (int)(centerX - textSize.x / 2), (int)(centerY - textSize.y / 2), 20, WHITE);
}


int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Linked List Representation");

    

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("-> Created in Raylib", screenWidth / 2 - 200, screenHeight / 2, 20, LIGHTGRAY);
            int i = 0;
            int j = 0;
            listGUI list;
            for (i = 0; i < 4; i++) {
                new_ListGUI(&list, i * 100 + j, 130, DARKBLUE, BLUE, LIGHTGRAY);
                char str[10];
                sprintf(str, "%d", i);
                draw_ListGUI(&list, str);
                j = j + 200;
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}