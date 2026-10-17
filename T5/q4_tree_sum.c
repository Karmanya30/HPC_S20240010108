// Compile: gcc -fopenmp q4_tree_sum.c -o q4
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct Node {
    long value;
    struct Node *left;
    struct Node *right;
};

long counter = 0;   // used only while building the tree (single thread)

// builds a complete tree of the given depth, values are 1, 2, 3, ...
struct Node *build(int depth) {
    if (depth == 0) return NULL;
    struct Node *node = malloc(sizeof(struct Node));
    counter++;
    node->value = counter;
    node->left = build(depth - 1);
    node->right = build(depth - 1);
    return node;
}

// normal sequential sum
long sum_seq(struct Node *node) {
    if (node == NULL) return 0;
    return node->value + sum_seq(node->left) + sum_seq(node->right);
}

// parallel sum: every child is a task
long sum_par(struct Node *node, int level) {
    if (node == NULL) return 0;

    // small subtrees: tasks would cost more than they save
    if (level >= 8) return sum_seq(node);

    long left_sum = 0, right_sum = 0;

    #pragma omp task shared(left_sum)
    left_sum = sum_par(node->left, level + 1);

    #pragma omp task shared(right_sum)
    right_sum = sum_par(node->right, level + 1);

    #pragma omp taskwait          // wait until both tasks are done
    return node->value + left_sum + right_sum;
}

int main() {
    struct Node *root = build(20);               // about 1 million nodes
    long expected = counter * (counter + 1) / 2; // 1+2+...+counter

    long total = 0;
    #pragma omp parallel
    {
        #pragma omp single                       // only one thread starts the recursion
        total = sum_par(root, 0);
    }

    printf("nodes    = %ld\n", counter);
    printf("parallel = %ld\n", total);
    printf("expected = %ld\n", expected);
    printf("sequential check = %ld\n", sum_seq(root));
    return 0;
}
