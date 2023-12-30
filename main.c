#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Message au wassim: Ajouter ton code des liste chainné la. Le nom d'addition des element pour la list doivent etre add_element()
void build_linked_list()
{
    add_element(&head, 10);
    add_element(&head, 5);
    add_element(&head, 7);
    add_element(&head, 9);
    add_element(&head, 1);
}
typedef struct listGUI {
    Rectangle value;
    Color valColor;
    Rectangle next;
    Color nextColor;
    Rectangle pointer;
    Color pointerColor;
    int x;
    int y;
    char* text;
} listGUI;
listGUI new_ListGUI(int x, int y, char* text, Color valColor, Color nextColor, Color pointerColor) {
    listGUI list = {
        .value = {x, y, 100, 80},
        .next = {x + 100, y, 100, 80},
        .pointer = {x + 200, y + 35, 100, 10},
        .x = x,
        .y = y,
        .text = strdup(text),
        .valColor = valColor,
        .nextColor = nextColor,
        .pointerColor = pointerColor,
    };
    return list;
}
void draw_ListGUI(listGUI *list) {
    DrawRectangleRec(list->value, list->valColor);
    DrawRectangleRec(list->next, list->nextColor);
    DrawRectangleRec(list->pointer, list->pointerColor);

    // Draws the value's text
    float centerX = list->value.x + list->value.width / 2;
    float centerY = list->value.y + list->value.height / 2;
    // Measure the text width and height
    Vector2 textSize = MeasureTextEx(GetFontDefault(), list->text, 20, 1);
    DrawText(list->text, (int)(centerX - textSize.x / 2), (int)(centerY - textSize.y / 2), 20, WHITE);
}
listGUI lists[10];
void clear_Node(int i) {
    Rectangle rec = { lists[i].x, lists[i].y, 200, 80};
    DrawRectangleRec(rec, RAYWHITE);
}
void swap_Position(int i, int j) {
    int x1 = lists[i].x;
    int y1 = lists[i].y;
    int x2 = lists[j].x;
    int y2 = lists[j].y;
    lists[j] = new_ListGUI(x1, y1, lists[j].text, lists[j].valColor, lists[j].nextColor, LIGHTGRAY);
    lists[i] = new_ListGUI(x2, y2, lists[i].text, lists[i].valColor, lists[i].nextColor, LIGHTGRAY);
}
void recolor(int i, Color valueColor, Color nextColor) {
    lists[i] = new_ListGUI(lists[i].x, lists[i].y, lists[i].text, valueColor, nextColor, LIGHTGRAY);
    draw_ListGUI(&lists[i]);
}
typedef struct Timer {
    double startTime;   // Start time (seconds)
    double lifeTime;    // Lifetime (seconds)
} Timer;
void StartTimer(Timer *timer, double lifetime)
{
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}
bool TimerDone(Timer timer)
{
    return GetTime() - timer.startTime >= timer.lifeTime;
}
double GetElapsed(Timer timer)
{
    return GetTime() - timer.startTime;
}

int main(void)
{
    int screenWidth = 1500;
    int screenHeight = 450;
    Color valColors = BLUE;
    Color nextColors = DARKBLUE;
    bool canClear = false;
    
    build_linked_list();
    repeat:
    node* p = head;
    int i = 0; int j = 0;
    while (p != NULL)
    {
        char str[10];
        sprintf(str, "%d", p->val);
        lists[i] = new_ListGUI(i * 100 + j, 130, str, valColors, nextColors, LIGHTGRAY);
        j = j + 200;
        i++;
        p = p->next;
    }
    InitWindow(screenWidth, screenHeight, "Linked List Representation"); 
    SetTargetFPS(24);
    while (!WindowShouldClose())
    {
        
        if (IsKeyDown(KEY_SPACE)) {
            add_element(&head, 20);
            screenWidth += 200;
            CloseWindow();
            goto repeat;
        }
        if (IsKeyDown(KEY_X)) {
            canClear = true;
        }
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
             
            DrawText("-> Created in Raylib", screenWidth / 2 - 200, screenHeight / 2, 20, LIGHTGRAY);
            for (int i = 0; i < sizeof(lists) / sizeof(lists[0]); i++)
            {
                draw_ListGUI(&lists[i]);
            }
            
            if (canClear)
            {
                node *p1 = head;
                node *p2 = head->next;
                int i = 0;
                Timer timer;
                StartTimer(&timer, 2);
                while (p1->next != NULL && p2 != NULL)
                {
                    
                    if (p1->val > p2->val) {
                        recolor(i, GREEN, DARKGREEN);
                        recolor(i+1, GREEN, DARKGREEN);
                        swap_Position(i, i+1);
                        int temp = p1->val;
                        p1->val = p2->val;
                        p2->val = temp;
                    }
                    p1 = p1->next;
                    p2 = p2->next;
                    i++;
                }
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
