#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define BUCKETS 10
#define DIGITS 5
#define MAX_NUMBER 10
#define MAX_QUEUE_SIZE 1000

typedef string element;

typedef struct QueueType {
    element queue[MAX_QUEUE_SIZE];
    int front, rear;
};

void error(char* message) {
    printf("%s\n", message);
}

void init(QueueType* q) {
    q->front = q->rear = 0;
}

int isEmpty(QueueType* q) {
    return (q->front == q->rear);
}

int isFull(QueueType* q) {
    return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType* q, element item) {
    if (isFull(q))
        exit(0);
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->queue[q->rear] = item;
}

element dequeue(QueueType* q) {
    if (isEmpty(q))
        exit(0);
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->queue[q->front];
}

void radix_sort(string list[], int n, char dType) {
    int i, b, d, factor = 1;

    QueueType queues[BUCKETS];
    for (b = 0; b < BUCKETS; b++)
        init(&queues[b]);

    switch (dType) {
    case 'y':
        for (d = 3; d >= 2; d--) {
            for (i = 0; i < n; i++)
                enqueue(&queues[list[i][d] - '0'], list[i]);


            for (b = i = 0; b < BUCKETS; b++)
                while (!isEmpty(&queues[b]))
                    list[i++] = dequeue(&queues[b]);
        }
        break;
    case 'm':
        for (d = 6; d >= 5; d--) {
            for (i = 0; i < n; i++)
                enqueue(&queues[(list[i][d] - '0') % 10], list[i]);


            for (b = i = 0; b < BUCKETS; b++)
                while (!isEmpty(&queues[b]))
                    list[i++] = dequeue(&queues[b]);
        }
        break;
    case 'd':
        for (d = 9; d >= 8; d--) {
            for (i = 0; i < n; i++)
                enqueue(&queues[(list[i][d] - '0') % 10], list[i]);


            for (b = i = 0; b < BUCKETS; b++)
                while (!isEmpty(&queues[b]))
                    list[i++] = dequeue(&queues[b]);
        }
        break;
    case 'h':
        for (d = 12; d >= 11; d--) {
            for (i = 0; i < n; i++)
                enqueue(&queues[(list[i][d] - '0') % 10], list[i]);


            for (b = i = 0; b < BUCKETS; b++)
                while (!isEmpty(&queues[b]))
                    list[i++] = dequeue(&queues[b]);
        }
        break;
    }
}

int fileRead(string name, string arr[]) {
    string line;
    ifstream file(name);
    int i = 0;

    if (file.is_open()) {
        while (getline(file, line)) {
            arr[i] = line;
            if (line != "") {
                i++;
            }
        }
        file.close();
    }
    else {
        cout << "Unable to open file";
    }
    return i;
}

void fileWrite(string name, string arr[], int n) {
    ofstream file(name);

    if (file.is_open()) {
        for (int i = 0; i < n; i++) {
            file << arr[i] << endl;
        }
        file.close();
    }
    else {
        cout << "Unable to open file";
    }
}

int main() {
    string arr[10000];
    int n = fileRead("data.txt", arr);

    radix_sort(arr, n, 'h');
    radix_sort(arr, n, 'd');
    radix_sort(arr, n, 'm');
    radix_sort(arr, n, 'y');

    for (int i = 0; i < n; i++)
        cout << arr[i] << endl;
    cout << endl;

    fileWrite("result.txt", arr, n);

    return 0;
}