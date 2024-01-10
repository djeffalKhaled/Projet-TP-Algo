#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
// Structure for the nodes of the doubly linked list
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
}   Node;

 int totalElements;


// Function to add an element to the linked list
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

 // Function to perform insertion sort on the linked list
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

 // Function to print the linked list
 void printList(Node* node) {
     while (node != NULL) {
         printf("%d ", node->data);
         node = node->next;
     }
     printf("\n");
 }
 
 // Function to free the memory allocated for the linked list
 void freeList(Node* head) {
    while (head != NULL) {
         Node* temp = head;
         head = head->next;
         free(temp);
     }
 }
 Node* head = NULL;
 
 // Function that adds an element in given position
void ajout_element_pos(Node** head, int element, int position) {
    if (position < 0 || position > totalElements) {
        printf("Invalid position\n");
        return;
    }

    Node* new_node = malloc(sizeof(Node));
    new_node->data = element;

    if (position == 0) {
        new_node->next = *head;
        new_node->prev = NULL;

        if (*head != NULL) {
            (*head)->prev = new_node;
        }

        *head = new_node;
    } else {
        Node* current = *head;

        for (int i = 0; i < position - 1; i++) {
            if (current == NULL) {
                printf("Invalid position\n");
                free(new_node);
                return;
            }
            current = current->next;
        }

        new_node->next = current->next;
        new_node->prev = current;

        if (current->next != NULL) {
            current->next->prev = new_node;
        }

        current->next = new_node;
    }

    totalElements++;
}



// Function to build the linked list
void build_linked_list() {
    ajout_element(&head, 10);
    ajout_element(&head, 5);
    ajout_element(&head, 7);
    ajout_element(&head, 9);
    ajout_element(&head, 1);
    ajout_element(&head, 4);
    ajout_element(&head, -5);
    ajout_element(&head, -8);
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

void draw_ListGUI(listGUI* list) {
    DrawRectangleRec(list->value, list->valColor);
    DrawRectangleRec(list->next, list->nextColor);
    DrawRectangleRec(list->nextPointer, list->pointerColor);
    DrawRectangleRec(list->last, list->lastColor);
    DrawRectangleRec(list->lastPointer, list->pointerColor);

    Rectangle nextArrow = {list->x + 180, list->y + 100, 10, 30};
    DrawRectangleRec(nextArrow, list->pointerColor);
    Rectangle lastArrow = {list->x + 110, list->y - 60, 10, 30};
    DrawRectangleRec(lastArrow, list->pointerColor);

    float centerX = list->value.x + list->value.width / 2;
    float centerY = list->value.y + list->value.height / 2;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), list->text, 20, 1);
    DrawText(list->text, (int)(centerX - textSize.x / 2), (int)(centerY - textSize.y / 2), 20, WHITE);
}

void draw_last_ListGUI(listGUI* list) {
    DrawRectangleRec(list->value, list->valColor);
    DrawRectangleRec(list->next, list->nextColor);
    DrawRectangleRec(list->last, list->lastColor);

    float centerX = list->value.x + list->value.width / 2;
    float centerY = list->value.y + list->value.height / 2;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), list->text, 20, 1);
    DrawText(list->text, (int)(centerX - textSize.x / 2), (int)(centerY - textSize.y / 2), 20, WHITE);
}

listGUI lists[10];

void clear_Node(int i) {
    Rectangle rec = {lists[i].x, lists[i].y, 200, 80};
    DrawRectangleRec(rec, RAYWHITE);
}

void recolor(int i, Color valueColor, Color nextColor, Color lastColor) {
    lists[i] = new_ListGUI(lists[i].x, lists[i].y, lists[i].text, valueColor, nextColor, lastColor, LIGHTGRAY);
}

// Function to delete the last element in the linked list
void deleteLastElement() {
    if (head != NULL) {
        Node* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }

        if (temp->prev != NULL) {
            // If not the first element, update the previous node's next pointer
            temp->prev->next = NULL;
        } else {
            // If it's the first element, update the head pointer
            head = NULL;
        }

        free(temp);
        totalElements--;
    }
}

void search_list_element(Node *head, int element) {
    Node* p = head;
    int pos = 0;
    while (p != NULL) {
        if (p->data == element) {
            recolor(pos, GREEN, DARKGREEN, DARKGREEN);
        }
        pos++;
        p = p->next;
    }
}

// Function to update the linked list data for visualization
void updateListData(Color valColors, Color nextColors, Color lastColors) {
    Node* p = head;
    int i = 0; int j = 0;
    while (p != NULL) {
        char str[10];
        sprintf(str, "%d", p->data);
        lists[i] = new_ListGUI(i * 100 + j, 130, str, valColors, nextColors, lastColors, LIGHTGRAY);
        j = j + 100;
        i++;
        p = p->next;
    }
}
void deleteElementAtPosition(int position) {
    if (head != NULL && position >= 0 && position < totalElements) {
        Node* temp = head;
        for (int i = 0; i < position; i++) {
            temp = temp->next;
        }

        if (temp->prev != NULL) {
            // If not the first element, update the previous node's next pointer
            temp->prev->next = temp->next;
        } else {
            // If it's the first element, update the head pointer
            head = temp->next;
        }

        if (temp->next != NULL) {
            // If not the last element, update the next node's prev pointer
            temp->next->prev = temp->prev;
        }

        free(temp);
        totalElements--;
    }
}

int main(void) {
    int screenWidth = 1500;
    int screenHeight = 600;
    int addElementValue = 0;
    int searchElementValue = 0;
    bool ignoreTimer = false;
    Color valColors = BLUE;
    Color nextColors = DARKBLUE;
    Color lastColors = DARKBLUE;
    Timer timer;
    int deleteButtonValue = 1;
    int addPosition = 0;
    
    
    build_linked_list();

    Node* p = head;
    int i = 0; int j = 0;
    while (p != NULL) {
        char str[10];
        sprintf(str, "%d", p->data);
        lists[i] = new_ListGUI(i * 100 + j, 130, str, valColors, nextColors, lastColors, LIGHTGRAY);
        j = j + 100;
        i++;
        p = p->next;
    }

    // Buttons
    Rectangle searchButton = {70, 450, 300, 100};
    Rectangle deleteButton = {420, 450, 300, 100};
    Rectangle addButton = {770, 450, 300, 100};
    Rectangle sortButton = {1120, 450, 300, 100};

    Rectangle addButtonValue = {770, 400, 150, 50};
    Rectangle searchButtonValue = {70, 400, 300, 50};
    Rectangle addPositionInput = {920, 400, 150, 50};

    InitWindow(screenWidth, screenHeight, "Representation Liste Doublement Chainee");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 25);
    SetTargetFPS(24);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("-> Projet TP Algo || Créé dans Raylib", 20, 10, 15, LIGHTGRAY);

        // Sort Button
        if (GuiButton(sortButton, "Tri par Insertion") && totalElements > 0) {
            tri_par_insertion(head);

            Rectangle deletion = {0, 0, 1300, 300};
            DrawRectangleRec(deletion, RAYWHITE);

            updateListData(GREEN, DARKGREEN, DARKGREEN);
            StartTimer(&timer, 1);
        }

        GuiSpinner(addButtonValue, "", &addElementValue, -100, 100, true);
        GuiSpinner(searchButtonValue, "", &searchElementValue, -100, 100, false);

        // Search Element Button
        if(GuiButton(searchButton, "Recherche Element")) {
            search_list_element(head, searchElementValue); 
            StartTimer(&timer, 1);
            ignoreTimer = true;
        }
        
        // Delete Element at position Button
        GuiSpinner((Rectangle){420, 400, 300, 50}, "", &deleteButtonValue, 1, totalElements, false); 
        //DrawText("Position", deleteButton.x + 125, deleteButton.y - 70, 15, GRAY); 
          if (GuiButton(deleteButton, "Supprimer Position") && totalElements > 0) {
        // Delete the element at the specified position
        deleteElementAtPosition(deleteButtonValue-1);
        updateListData(valColors, nextColors, lastColors);
        }

        // Add Element at position Button
        GuiSpinner(addPositionInput, "", &addPosition, 0, totalElements, false);
        DrawText("Position", addPositionInput.x + 45, addPositionInput.y - 20, 20, GRAY);
        DrawText("Element", addButton.x + 45, addButton.y - 70, 20, GRAY);

            ajout_element_pos(&head, addElementValue, addPosition);
            updateListData(valColors, nextColors, lastColors);
            recolor(addPosition, GREEN, DARKGREEN, DARKGREEN);
            addPosition++;
            StartTimer(&timer, 1);
            ignoreTimer = true;
        }

        // Code for Timer, it resets colors after 1 second
        UpdateTimer(&timer); 
        if (TimerDone(&timer) && ignoreTimer) {
            updateListData(BLUE, DARKBLUE, DARKBLUE);
            ignoreTimer = false;
        }

        // Main code for drawing
        for (i = 0; i < totalElements - 1; i++) {
            if (i == 0) {
                DrawText("Tête", lists[i].x + 25, lists[i].y + 180, 20, GRAY);
            } 
            draw_ListGUI(&lists[i]);
        }
        // Deals with drawing the last element
        draw_last_ListGUI(&lists[totalElements - 1]);
        if (totalElements > 1) {
            DrawText("Queue", lists[totalElements - 1].x + 25, lists[totalElements - 1].y + 180, 20, GRAY);
        }
        else if (totalElements == 1) {
            DrawText("Tête + Queue", lists[totalElements - 1].x + 25, lists[totalElements - 1].y + 180, 20, GRAY);
        }
        EndDrawing();
    }

    CloseWindow();

    freeList(head);

    return 0;
}
