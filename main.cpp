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

// quick 정렬
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
        int q = partition(list, left, right); // q: 피벗의 위치

        quick_sort(list, left, q - 1);
        quick_sort(list, q + 1, right);
    }
}

void insertion_sort(int list[], int start, int end);
void CountSort(int data[], int start, int end, int result[], int k, char mode = 'y') {
    // data: 정렬할 배열
    // result: 정렬된 결과 배열
    // start: 시작 index
    // end: 마지막 index
    // k: 값의 변화 범위

    if (start == end) {
        return;
    }

    int n = end - start + 1;
    int i, j;
    // N: 범위내의 각 숫자가 나타나는 횟수를 위한 배열
    int* N = new int[k];

    for (i = 0; i < k; i++)
        N[i] = 0;

    // year 순으로 정렬
    if (mode == 'y') {
        // year 개수 저장 위해 year_N배열 선언
        int* year_N = new int[k];
        for (int i = 0; i < k; i++) {
            year_N[i] = 0;
        }

        // 각 키의 개수 저장
        for (j = 0; j < n; j++) {
            // A[j] = 2022112313 형식
            int year = data[j] / 1000000;
            N[year - BASE_YEAR] += 1;
            ::visitCount++;
        }

        // 각 키의 누적 합 저장
        for (i = 1; i < k; i++) {
            N[i] += N[i - 1];
            year_N[i] = N[i];
            ::visitCount++;
        }

        // 정렬 결과를 배열 B에 저장
        // B가 year순으로 정렬된 배열임
        for (j = n - 1; j >= 0; j--) {
            int year = data[j] / 1000000;
            result[N[year - BASE_YEAR] - 1] = data[j];
            N[year - BASE_YEAR] -= 1;
            ::visitCount++;
        }

        // year 단위로만 정렬된 결과 잠시 저장할 배열
        int* temp = new int[n];
        for (int i = 0; i < n; i++) {
            temp[i] = result[i];
        }

        // month에 대해 계수 정렬
        int count = 0;
        for (i = 0; i < k; i++) {
            if (year_N[i] != count) {
                // A, B 배열 저장 위치?
                CountSort(temp, count, year_N[i] - 1, result, 12 + 1, 'm');
                count = year_N[i];
                ::visitCount++;
            }
        }
        delete[] year_N;
        delete[] temp;
    }
    else if (mode == 'm') {
        // 각 키의 개수 저장
        for (j = start; j <= end; j++) {
            // A[j] = 2022112313 형식
            int month = (data[j] % 1000000) / 10000;
            N[month] += 1;
            ::visitCount++;
        }

        // 각 키의 누적 합 저장
        for (i = 1; i < k; i++) {
            N[i] += N[i - 1];
            ::visitCount++;
        }

        // 정렬 결과를 배열 B에 저장
        for (j = end; j >= start; j--) {
            int month = (data[j] % 1000000) / 10000;
            result[start + N[month] - 1] = data[j];
            N[month] -= 1;
            ::visitCount++;
        }
    }
    delete[](N);
}

// 삽입 정렬
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
    // int n = fileRead("..\\data\\data.txt", arr); // 100개 데이터
    int n = fileRead("..\\data\\data2.txt", arr); // 1000개 데이터

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
    // int n = fileRead("..\\data\\data.txt", arr); // 100개 데이터
    int dataSize = fileRead("..\\data\\data2.txt", arr); // 1000개 데이터

    // yyyy/mm/dd/hh -> 숫자로
    for (int i = 0; i < dataSize; i++) {
        data[i] = stoi(regex_replace(arr[i], regex("/"), ""));
    }

    cout << "Start quick sort..." << endl;
    int quickSortData[MAX_NUMBER]; // 다른 정렬에도 사용하므로 data 배열을 직접 사용하지 않고 새로운 배열 선언하여 사용
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
        계수 정렬 후 삽입 정렬
        먼저, 계수 정렬로 데이터들을 연도, 월 기준으로 거의 정리 -> 그 다음 삽입 정렬 사용
        데이터들이 거의 정렬된 상태에서는 삽입 정렬 사용 유리
        (삽입 정렬은 최악 N^2 이지만, 자료가 이미 정렬된 상태에서는 시간복잡도 N임)
        (계수 정렬로 연도, 월, 일, 시간 각각 전부 정렬하면 매우 많은 비교횟수와 공간이 필요하고 이는 데이터가 많아질수록 더 많아짐)
        계수 정렬과 삽입 정렬을 같이 사용하면 퀵 정렬보다 더 적은 비교 횟수가 나오는 것을 볼 수 있음
    */
    CountSort(data, 0, dataSize - 1, countingSortData, 100, 'y'); // data 배열을 정렬하여 countingSortData배열에 저장
    insertion_sort(countingSortData, 0, dataSize - 1); // 연도, 월 기준으로만 정렬된 countingSortData배열을 다시 정렬

    fileWrite("..\\result\\CountingSortResult.txt", countingSortData, dataSize);

    cout << "End counting sort" << endl;
    // 삽입 정렬에서의 비교 횟수(compareCount) + 계수 정렬에서의 새로운 배열을 생성할 때 데이터를 순회한 횟수(visitCount)
    cout << "Compare count: " << ::compareCount + ::visitCount << endl << endl;
    ::compareCount = 0;

    return 0;
}
