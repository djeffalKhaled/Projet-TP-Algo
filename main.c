#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int val;
    struct node* prev;
    struct node* next;
}node ;
struct node* createNode(int val) {
    node* newNode = (struct node*)malloc(sizeof(node));
    newNode->val = val;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}
void add_element(node** head, int val) {
    node* newNode = createNode(val);
    
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }
}

// Message au wassim: Ajouter le trie par l'insertion la.
// Vous pouvez remplacer le code "add_element" et "createNode" par votre propre code équivalent si vous le souhaitez. 
// Ils sont là pour que mon interface fonctionne correctement.


node *head = NULL;
void build_linked_list()
{
    // Elements of the linked list
    add_element(&head, 10);
    add_element(&head, 5);
    add_element(&head, 7);
    add_element(&head, 9);
    add_element(&head, 1);
    add_element(&head, 4);
}
typedef struct listGUI {
    Rectangle value;
    Color valColor;
    Rectangle next;
    Color nextColor;
    Rectangle last;
    Color lastColor;
    Rectangle nextPointer;
    Rectangle lastPointer;
    Color pointerColor;
    int x;
    int y;
    char* text;
} listGUI;
listGUI new_ListGUI(int x, int y, char* text, Color valColor, Color nextColor, Color lastColor, Color pointerColor) {
    listGUI list = {
        .value = {x, y, 100, 80},
        .next = {x, y + 80, 100, 80},
        .nextPointer = {x + 100, y + 110, 100, 10},
        .last = {x, y - 80, 100, 80},
        .lastPointer = {x + 100, y - 50, 100, 10},
        .x = x,
        .y = y,
        .text = strdup(text),
        .valColor = valColor,
        .nextColor = nextColor,
        .lastColor = lastColor,
        .pointerColor = pointerColor,
    };
    return list;
}
void draw_ListGUI(listGUI *list) {
    DrawRectangleRec(list->value, list->valColor);
    DrawRectangleRec(list->next, list->nextColor);
    DrawRectangleRec(list->nextPointer, list->pointerColor);
    DrawRectangleRec(list->last, list->lastColor);
    DrawRectangleRec(list->lastPointer, list->pointerColor);

    // Gives the pointers an arrow look
    Rectangle nextArrow = {list->x + 180, list->y + 100, 10, 30};
    DrawRectangleRec(nextArrow, list->pointerColor);
    Rectangle lastArrow = {list->x + 110, list->y - 60, 10, 30};
    DrawRectangleRec(lastArrow, list->pointerColor);
    
    
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
    lists[j] = new_ListGUI(x1, y1, lists[j].text, lists[j].valColor, lists[j].nextColor, lists[j].lastColor, LIGHTGRAY);
    lists[i] = new_ListGUI(x2, y2, lists[i].text, lists[i].valColor, lists[i].nextColor, lists[j].lastColor, LIGHTGRAY);
}
void recolor(int i, Color valueColor, Color nextColor, Color lastColor) {
    lists[i] = new_ListGUI(lists[i].x, lists[i].y, lists[i].text, valueColor, nextColor, lastColor, LIGHTGRAY);
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
typedef struct Button {
    Rectangle bounds;
    const char *text;
    Color color;
    Color textColor;
    bool clicked;
} Button;

bool IsMouseOverButton(Button button) {
    Vector2 mouse = GetMousePosition();
    bool isOver = CheckCollisionPointRec(mouse, button.bounds);

    if (isOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        button.clicked = true;
    } else {
        button.clicked = false;
    }

    return button.clicked; 
}

Button new_button(Rectangle bounds, char* text, Color color, Color textColor) {
    Button button = {
        .bounds = bounds,
        .text = strdup(text),
        .color = color,
        .textColor = textColor,
    };
    return button;
}

void DrawButton(Button button) {
    DrawRectangleRec(button.bounds, button.color);
    DrawText(button.text, (int)(button.bounds.x + button.bounds.width / 2 - MeasureText(button.text, 20) / 2),
             (int)(button.bounds.y + button.bounds.height / 2 - 10), 20, button.textColor);
}

int main(void)
{
    int screenWidth = 1300;
    int screenHeight = 600;
    Color valColors = BLUE;
    Color nextColors = DARKBLUE;
    Color lastColors = DARKBLUE;
    bool canSort = false;
    
    build_linked_list();
    repeat:
    node* p = head;
    int i = 0; int j = 0;
    while (p != NULL)
    {
        char str[10];
        sprintf(str, "%d", p->val);
        lists[i] = new_ListGUI(i * 100 + j, 130, str, valColors, nextColors, lastColors, LIGHTGRAY);
        j = j + 100;
        i++;
        p = p->next;
    }

    // Buttons
    Rectangle sortBounds = {800, 450, 300, 100};
    Button sortButton = new_button(sortBounds, "Insertion Sort", BLUE, WHITE);
    Rectangle addBounds = {450, 450, 300, 100};
    Button addButton = new_button(addBounds, "Add Element", BLUE, WHITE);
    Rectangle deleteBounds = {100, 450, 300, 100};
    Button deleteButton = new_button(deleteBounds, "Delete Element", BLUE, WHITE);
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
        if (IsMouseOverButton(sortButton)) {
            canSort = true;
        }
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
             
            DrawText("-> Created in Raylib", 20, 10, 10, LIGHTGRAY);
            DrawButton(sortButton);
            DrawButton(addButton);
            DrawButton(deleteButton);
            for (int i = 0; i < sizeof(lists) / sizeof(lists[0]); i++)
            {
                draw_ListGUI(&lists[i]);
            }
            
            if (canSort)
            {
                node *p1 = head;
                node *p2 = head->next;
                int i = 0;
                Timer timer;
                StartTimer(&timer, 2);
                while (p1->next != NULL && p2 != NULL)
                {
                    
                    if (p1->val > p2->val) {
                        recolor(i, GREEN, DARKGREEN, DARKGREEN);
                        recolor(i+1, GREEN, DARKGREEN, DARKGREEN);
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
