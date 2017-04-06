/* File:        serial.c
 *
 * Authors:     130164H H.A. Galappaththi
 *              130650U M.S. Wickramarathne
 *
 * Compile:     gcc -g -Wall -o serial serial.c -lm
 *
 * Run:         ./serial <sample size> <n> <m> <mMember> <mInsert> <mDelete>
 *                  n is the number of initial unique values in the Link List.
 *                  m is number of random Member, Insert, and Delete operations on the link list.
 *                  mMember is the fractions of operations of Member operation.
 *                  mInsert is the fractions of operations of Insert operation.
 *                  mDelete is the fractions of operations of Delete operation.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// linked list node struct
struct list_node_s {
    int data;
    struct list_node_s *next;
};

// program control variables
#define MAX_OPERATIONS_SIZE 10000
int sample_size, n, m;
double m_member, m_insert, m_delete;
int operations_array[MAX_OPERATIONS_SIZE];


// linked list operator definitions
int member(int value, struct list_node_s *head_p);

int insert(int value, struct list_node_s **head_pp);

int delete(int value, struct list_node_s **head_pp);

// helper function definitions
void generate_operations();

void shuffle(int *array, int n);

int rand_int(int n);

double run_experiment();

void initialize(int argc, char *argv[]);

void program_help(char *program_name);


// linked list member function
int member(int value, struct list_node_s *head_p) {
    struct list_node_s *curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value) {
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        return 0;
    } else {
        return 1;
    }
}

// linked list insert function
int insert(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;
    struct list_node_s *temp_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;

        if (pred_p == NULL) {
            *head_pp = temp_p;
        } else {
            pred_p->next = temp_p;
        }
        return 1;

    } else {
        return 0;
    }
}

// linked list delete function
int delete(int value, struct list_node_s **head_pp) {
    struct list_node_s *curr_p = *head_pp;
    struct list_node_s *pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
            free(curr_p);
        } else {
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;

    } else {
        return 0;
    }
}


// Generate operations
void generate_operations() {
    int ops_member = (int) (m * m_member);
    int ops_insert = (int) (m * m_insert);
    int ops_delete = (int) (m * m_delete);

    int start = 0;
    int end = ops_member;

    for (int i = start; i < end; i++) {
        operations_array[i] = 1; // member operation
    }

    start = ops_member;
    end = ops_member + ops_insert;

    for (int i = start; i < end; i++) {
        operations_array[i] = 2; // insert operation
    }

    start = ops_member + ops_insert;
    end = ops_member + ops_insert + ops_delete;

    for (int i = start; i < end; i++) {
        operations_array[i] = 3; // delete operation
    }

    shuffle(operations_array, m); // shuffle the array
}

// shuffle array - Fisher-Yates Algorithm
void shuffle(int *array, int n) {
    int i, j, tmp;

    for (i = n - 1; i > 0; i--) {
        j = rand_int(i + 1);

        tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

int rand_int(int n) {
    int limit = RAND_MAX - RAND_MAX % n;

    int rnd;
    do {
        rnd = rand();
    } while (rnd >= limit);

    return rnd % n;
}

// execute operations on linked list
double run_experiment() {
    generate_operations(); // randomize operations

    struct list_node_s *head = NULL;
    double start, finish, elapsed;

    // initially populating the link list
    for (int i = 0; i < n; i++) {
        int value = rand() % 65536;
        if (!insert(value, &head)) {
            i--;
        }
    }

    // start operations
    start = clock();

    for (int i = 0; i < m; i++) {
        int value = rand() % 65536;

        switch (operations_array[i]) {
            case 1: // member operation
                member(value, head);
                break;
            case 2: // insert operation
                insert(value, &head);
                break;
            case 3: // delete operation
                delete(value, &head);
                break;
            default:
                break;
        }
    }

    finish = clock();
    elapsed = (finish - start) / CLOCKS_PER_SEC;

    return elapsed;
}

// initialize program variables using arguments received
void initialize(int argc, char *argv[]) {
    if (argc != 7) {
        program_help(argv[0]);
    }

    sscanf(argv[1], "%d", &sample_size);
    sscanf(argv[2], "%d", &n);
    sscanf(argv[3], "%d", &m);

    sscanf(argv[4], "%lf", &m_member);
    sscanf(argv[5], "%lf", &m_insert);
    sscanf(argv[6], "%lf", &m_delete);

    if (sample_size <= 0 || n <= 0 || m <= 0 || m_member + m_insert + m_delete != 1.0) {
        program_help(argv[0]);
    }
}

// program usage instructions
void program_help(char *program_name) {
    fprintf(stderr, "usage: %s <sample size> <n> <m> <mMember> <mInsert> <mDelete>\n", program_name);
    fprintf(stderr, "\tn is the number of initial unique values in the Link List.\n");
    fprintf(stderr, "\tm is number of random Member, Insert, and Delete operations on the link list.\n");
    fprintf(stderr, "\tmMember is the fractions of operations of Member operation.\n");
    fprintf(stderr, "\tmInsert is the fractions of operations of Insert operation.\n");
    fprintf(stderr, "\tmDelete is the fractions of operations of Delete operation.\n");

    exit(0);
}


int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));
    initialize(argc, argv);

    double total_time = 0.0;
    double execution_times[sample_size];

    // calculate average execution time
    for (int i = 0; i < sample_size; i++) {
        double elapsed_time = run_experiment();
        execution_times[i] = elapsed_time;
        total_time += elapsed_time;
    }

    double average_time = total_time / sample_size;
    printf("Average elapsed time = %.10f seconds\n", average_time);

    if (sample_size > 1) {
        double variance = 0.0;

        // calculate standard deviation
        for (int i = 0; i < sample_size; i++) {
            variance += pow(execution_times[i] - average_time, 2);
        }

        double standard_deviation = sqrt(variance / (sample_size - 1));
        printf("Standard deviation = %.10f seconds\n", standard_deviation);

        double sample_size_n = pow(((100 * 1.96 * standard_deviation) / (5 * average_time)), 2);
        printf("Required samples size = %.10f \n", sample_size_n);
    }

    return 0;
}
