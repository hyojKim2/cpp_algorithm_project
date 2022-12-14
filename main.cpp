#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

#define BASE_YEAR 2000
#define BUCKETS 10
#define DIGITS 5
#define MAX_NUMBER 10000
#define MAX_QUEUE_SIZE 1000

int compareCount = 0;
int visitCount = 0;

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

// quick ??????
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int list[], int left, int right) {
    int pivot;
    int low, high;

    low = left;
    high = right + 1;
    pivot = list[left];

    do {
        do {
            low++;
            ::compareCount++;
        } while (low <= right && list[low] < pivot);

        do {
            high--;
            ::compareCount++;
        } while (high >= left && list[high] > pivot);

        if (low < high) {
            swap(&list[low], &list[high]);
        }
    } while (low < high);

    swap(&list[left], &list[high]);

    return high;
}

void quick_sort(int list[], int left, int right) {
    if (left < right) {
        int q = partition(list, left, right); // q: ????????? ??????

        quick_sort(list, left, q - 1);
        quick_sort(list, q + 1, right);
    }
}

void insertion_sort(int list[], int start, int end);
void CountSort(int data[], int start, int end, int result[], int k, char mode = 'y') {
    // data: ????????? ??????
    // result: ????????? ?????? ??????
    // start: ?????? index
    // end: ????????? index
    // k: ?????? ?????? ??????

    if (start == end) {
        return;
    }

    int n = end - start + 1;
    int i, j;
    // N: ???????????? ??? ????????? ???????????? ????????? ?????? ??????
    int* N = new int[k];

    for (i = 0; i < k; i++)
        N[i] = 0;

    // year ????????? ??????
    if (mode == 'y') {
        // year ?????? ?????? ?????? year_N?????? ??????
        int* year_N = new int[k];
        for (int i = 0; i < k; i++) {
            year_N[i] = 0;
        }

        // ??? ?????? ?????? ??????
        for (j = 0; j < n; j++) {
            // data[j] = 2022112313 ??????
            int year = data[j] / 1000000;
            N[year - BASE_YEAR] += 1;
            ::visitCount++;
        }

        // ??? ?????? ?????? ??? ??????
        for (i = 1; i < k; i++) {
            N[i] += N[i - 1];
            year_N[i] = N[i];
            ::visitCount++;
        }

        // ?????? ????????? result????????? ??????
        // result??? year????????? ????????? ?????????
        for (j = n - 1; j >= 0; j--) {
            int year = data[j] / 1000000;
            result[N[year - BASE_YEAR] - 1] = data[j];
            N[year - BASE_YEAR] -= 1;
            ::visitCount++;
        }

        // year ???????????? ????????? ?????? ?????? ????????? ??????
        int* temp = new int[n];
        for (int i = 0; i < n; i++) {
            temp[i] = result[i];
        }

        // ?????? year??? ????????? ??????????????? month??? ?????? ?????? ??????
        int count = 0;
        for (i = 0; i < k; i++) {
            if (year_N[i] != count) {
                CountSort(temp, count, year_N[i] - 1, result, 12 + 1, 'm');
                count = year_N[i];
                ::visitCount++;
            }
        }
        delete[] year_N;
        delete[] temp;
    }
    else if (mode == 'm') {
        // N[0]??? ?????? x, N[1]??? 1??? ??????
        // ??? ?????? ?????? ??????
        for (j = start; j <= end; j++) {
            // data[j] = 2022112313 ??????
            int month = (data[j] % 1000000) / 10000;
            N[month] += 1;
            ::visitCount++;
        }

        // ??? ?????? ?????? ??? ??????
        for (i = 1; i < k; i++) {
            N[i] += N[i - 1];
            ::visitCount++;
        }

        // ?????? ????????? ?????? B??? ??????
        for (j = end; j >= start; j--) {
            int month = (data[j] % 1000000) / 10000;
            result[start + N[month] - 1] = data[j];
            N[month] -= 1;
            ::visitCount++;
        }
    }
    delete[](N);
}

// ?????? ??????
void insertion_sort(int list[], int start, int end) {
    int i, j, key;

    for (i = start + 1; i <= end; i++) {
        key = list[i];

        for (j = i - 1; j >= start && list[j] > key; j--) {
            ::compareCount++;
            list[j + 1] = list[j];
        }

        list[j + 1] = key;
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

void fileWrite(string name, int arr[], int n) {
    ofstream file(name);

    if (file.is_open()) {
        for (int i = 0; i < n; i++) {
            string str = to_string(arr[i]);
            str.insert(4, "/").insert(7, "/").insert(10, "/");

            file << str << endl;
        }
        file.close();
    }
    else {
        cout << "Unable to open file";
    }
}

int main() {
    string arr[MAX_NUMBER];
    // int n = fileRead("..\\data\\data.txt", arr); // 100??? ?????????
    int n = fileRead("..\\data\\data2.txt", arr); // 1000??? ?????????

    radix_sort(arr, n, 'h');
    radix_sort(arr, n, 'd');
    radix_sort(arr, n, 'm');
    radix_sort(arr, n, 'y');

    for (int i = 0; i < n; i++)
        cout << arr[i] << endl;
    cout << endl;

    fileWrite("..\\result\\RadixSortResult.txt", arr, n);

    // quick sort
    int data[10000];
    // int n = fileRead("..\\data\\data.txt", arr); // 100??? ?????????
    int dataSize = fileRead("..\\data\\data2.txt", arr); // 1000??? ?????????

    // yyyy/mm/dd/hh -> ?????????
    for (int i = 0; i < dataSize; i++) {
        data[i] = stoi(regex_replace(arr[i], regex("/"), ""));
    }

    cout << "Start quick sort..." << endl;
    int quickSortData[MAX_NUMBER]; // ?????? ???????????? ??????????????? data ????????? ?????? ???????????? ?????? ????????? ?????? ???????????? ??????
    for (int i = 0; i < dataSize; i++) {
        quickSortData[i] = data[i];
    }
    quick_sort(quickSortData, 0, dataSize - 1);

    fileWrite("..\\result\\QuickSortResult.txt", quickSortData, dataSize);

    cout << "End quick sort" << endl;
    cout << "Compare count: " << ::compareCount << endl << endl;
    ::compareCount = 0;

    // counting sort
    cout << "Start counting sort..." << endl;
    int countingSortData[MAX_NUMBER];

    /*
        ?????? ?????? ??? ?????? ??????
        ??????, ?????? ????????? ??????????????? ??????, ??? ???????????? ?????? ?????? -> ??? ?????? ?????? ?????? ??????
        ??????????????? ?????? ????????? ??????????????? ?????? ?????? ?????? ??????
        (?????? ????????? ?????? N^2 ?????????, ????????? ?????? ????????? ??????????????? ??????????????? N???)
        (?????? ????????? ??????, ???, ???, ?????? ?????? ?????? ???????????? ?????? ?????? ??????????????? ????????? ???????????? ?????? ???????????? ??????????????? ??? ?????????)
        ?????? ????????? ?????? ????????? ?????? ???????????? ??? ???????????? ??? ?????? ?????? ????????? ????????? ?????? ??? ??? ??????
    */
    CountSort(data, 0, dataSize - 1, countingSortData, 100, 'y'); // data ????????? ???????????? countingSortData????????? ??????
    insertion_sort(countingSortData, 0, dataSize - 1); // ??????, ??? ??????????????? ????????? countingSortData????????? ?????? ??????

    fileWrite("..\\result\\CountingSortResult.txt", countingSortData, dataSize);

    cout << "End counting sort" << endl;
    // ?????? ??????????????? ?????? ??????(compareCount) + ?????? ??????????????? ????????? ????????? ????????? ??? ???????????? ????????? ??????(visitCount)
    cout << "Compare count: " << ::compareCount + ::visitCount << endl << endl;
    ::compareCount = 0;

    return 0;
}
