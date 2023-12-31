#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour les nœuds de la liste doublement chaînée
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

int totalElements;

// Fonction pour créer  la liste
void ajout_element(Node** tete_ref, int new_data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = NULL;

    if (*tete_ref == NULL) {
        new_node->prev = NULL;
        *tete_ref = new_node;
    } else {
        Node* last = *tete_ref;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = new_node;
        new_node->prev = last;
    }
    totalElements++;
}

void tri_par_insertion(Node* node) {
    Node* Q = node->next;
    Node* p = node;
    Node* R = p;
    int temp;

    while (Q != NULL) {
        temp = Q->data;
        p = Q->prev;
        while (p != NULL && temp < p->data) {
            (p->next)->data = p->data;
            R = p;
            p = p->prev;
        }
        if (R->data > temp) {
            R->data = temp;
        }
        Q = Q->next;
    }
}

//  afficher la liste
void printList(Node* node) {
    while (node != NULL) {
        printf("%d ", node->data);
        node = node->next;
    }
    printf("\n");
}

//libérer la mémoire
void freeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

Node *head = NULL;

void build_linked_list() {
    // Elements of the linked list
    ajout_element(&head, 10);
    ajout_element(&head, 5);
    ajout_element(&head, 7);
    ajout_element(&head, 9);
    ajout_element(&head, 1);
    ajout_element(&head, 4);
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

void draw_last_ListGUI(listGUI *list) {
    DrawRectangleRec(list->value, list->valColor);
    DrawRectangleRec(list->next, list->nextColor);
    DrawRectangleRec(list->last, list->lastColor);

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

int main(void)
{
    int screenWidth = 1100;
    int screenHeight = 600;
    Color valColors = BLUE;
    Color nextColors = DARKBLUE;
    Color lastColors = DARKBLUE;
    bool canSort = false;

    build_linked_list();
    
    Node* p = head;
    int i = 0; int j = 0;
    while (p != NULL)
    {
        char str[10];
        sprintf(str, "%d", p->data);
        lists[i] = new_ListGUI(i * 100 + j, 130, str, valColors, nextColors, lastColors, LIGHTGRAY);
        j = j + 100;
        i++;
        p = p->next;
    }

    // Buttons
    Rectangle sortButton = {800, 450, 300, 100};
    Rectangle addButton = {450, 450, 300, 100};
    Rectangle deleteButton = {100, 450, 300, 100};
    
    // Value Boxes
    Rectangle addButtonValue = {450, 400, 300, 50};

    InitWindow(screenWidth, screenHeight, "Linked List Representation"); 
    GuiSetStyle(DEFAULT, TEXT_SIZE, 25);
    SetTargetFPS(24);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
            
        ClearBackground(RAYWHITE);
        DrawText("-> Created in Raylib", 20, 10, 10, LIGHTGRAY);

        if (GuiButton(sortButton, "Insertion Sort")) {
            canSort = true;
        }

        // Add Element Button code
        int elementValue;
        GuiSpinner(addButtonValue, "", &elementValue, -100, 100, true);
        
        if (GuiButton(addButton, "Add Element") && totalElements < 8) {
            char str[10];
            sprintf(str, "%d", elementValue);
            DrawText(str, 30, 20, 20, LIGHTGRAY);
            ajout_element(&head, elementValue);

            // Update the linked list data
            p = head;
            i = 0; j = 0;
            while (p != NULL)
            {
                char str[10];
                sprintf(str, "%d", p->data);
                lists[i] = new_ListGUI(i * 100 + j, 130, str, valColors, nextColors, lastColors, LIGHTGRAY);
                j = j + 100;
                i++;
                p = p->next;
            }
        }

        // Delete Element Button code
        if (GuiButton(deleteButton, "Delete Element") && totalElements > 0) {
            // Delete the last element
            Node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            if (temp->prev != NULL) {
                temp->prev->next = NULL;
                free(temp);
                totalElements--;

                // Update the linked list data
                p = head;
                i = 0; j = 0;
                while (p != NULL)
                {
                    char str[10];
                    sprintf(str, "%d", p->data);
                    lists[i] = new_ListGUI(i * 100 + j, 130, str, valColors, nextColors, lastColors, LIGHTGRAY);
                    j = j + 100;
                    i++;
                    p = p->next;
                }
            }
        }

        // Draw the linked list visualization
        for (int i = 0; i < totalElements - 1; i++)
        {
            draw_ListGUI(&lists[i]);
        }
        draw_last_ListGUI(&lists[totalElements - 1]);

        if (canSort)
        {
            // Sort the linked list
            tri_par_insertion(head);

            // Draw the sorted linked list visualization
            Rectangle deletion = {0, 0, 1300, 300};
            DrawRectangleRec(deletion, RAYWHITE);
            Node* p = head;
            int i = 0; int j = 0;
            while (p != NULL)
            {
                char str[10];
                sprintf(str, "%d", p->data);
                lists[i] = new_ListGUI(i * 100 + j, 130, str, GREEN, DARKGREEN, DARKGREEN, LIGHTGRAY);
                j = j + 100;
                i++;
                p = p->next;
            }
            for (i = 0; i < sizeof(lists) / sizeof(lists[0]); i++)
            {
                draw_ListGUI(&lists[i]);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    // Free the linked list memory
    freeList(head);

    return 0;
}
