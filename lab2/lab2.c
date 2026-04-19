#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node* createNode(int value) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (n == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }

    n->data = value;
    n->next = NULL;
    return n;
}

void append(Node **head, Node **tail, int value) {
    Node *n = createNode(value);

    if (*head == NULL) {
        *head = n;
        *tail = n;
    } else {
        (*tail)->next = n;
        *tail = n;
    }
}

void printList(Node *head) {
    while (head) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

void freeList(Node *head) {
    Node *tmp;

    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

Node* mergeLists(Node *posH, Node *posT, Node *zeroH, Node *zeroT, Node *negH) {
    Node *head = NULL;

    if (posH) {
        head = posH;
        posT->next = zeroH ? zeroH : negH;
    } else if (zeroH) {
        head = zeroH;
    } else {
        head = negH;
    }

    if (zeroH)
        zeroT->next = negH;

    return head;
}

void rearrange(Node **head) {
    Node *posH = NULL, *posT = NULL;
    Node *zeroH = NULL, *zeroT = NULL;
    Node *negH = NULL, *negT = NULL;

    Node *curr = *head;

    while (curr) {
        Node *next = curr->next;
        curr->next = NULL;

        if (curr->data > 0) {
            if (posH == NULL)
                posH = posT = curr;
            else {
                posT->next = curr;
                posT = curr;
            }
        } else if (curr->data == 0) {
            if (zeroH == NULL)
                zeroH = zeroT = curr;
            else {
                zeroT->next = curr;
                zeroT = curr;
            }
        } else {
            if (negH == NULL)
                negH = negT = curr;
            else {
                negT->next = curr;
                negT = curr;
            }
        }

        curr = next;
    }

    *head = mergeLists(posH, posT, zeroH, zeroT, negH);
}

int main() {
    Node *head = NULL, *tail = NULL;
    int n, x;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid number of elements.\n");
        return 0;
    }

    printf("Enter %d integers:\n", n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &x);
        append(&head, &tail, x);
    }

    printf("Original list:\n");
    printList(head);

    rearrange(&head);

    printf("Rearranged list:\n");
    printList(head);

    freeList(head);

    return 0;
}