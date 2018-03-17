#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct hash_item_struct {
    int key;
    int val;
    struct hash_item_struct* next;
} hash_item_t;

typedef hash_item_t** hash_table_t;

#define TABLE_SIZE 32
#define HASH(k) k % TABLE_SIZE

int contains(hash_table_t table, int key) {
    size_t index = (size_t) HASH(key);
    hash_table_t table_ptr = table + index;
    hash_item_t* item_ptr = *table_ptr;
    while (item_ptr != NULL) {
        if (key == item_ptr->key) return 1;
        item_ptr = item_ptr->next;
    }
    return 0;
}

int get(hash_table_t table, int key) {
    size_t index = (size_t) HASH(key);
    hash_table_t table_ptr = table + index;
    hash_item_t* item_ptr = *table_ptr;
    while (item_ptr != NULL) {
        if (key == item_ptr->key) return item_ptr->val;
        item_ptr = item_ptr->next;
    }
    return -1;
}

void put(hash_table_t table, int key, int val) {
    /* create the new item for the hash table */
    hash_item_t* item_ptr = (hash_item_t*) malloc(sizeof(hash_item_t));
    item_ptr->key = key;
    item_ptr->val = val;
    item_ptr->next = NULL;
    /* insert the item into the hash table */
    size_t index = (size_t) HASH(key);
    hash_table_t table_ptr = table + index;
    if (*table_ptr != NULL) {
        item_ptr->next = *table_ptr;
    }
    *table_ptr = item_ptr;
}

void dump(hash_table_t table) {
    if (table == NULL) return;
    for (int index = 0; index < TABLE_SIZE; ++index) {
        printf("%p:[%d] ->", &table[index], index);
        hash_item_t* item_ptr = table[index];
        while (item_ptr != NULL) {
            printf(" %p:[%d %d %p]", item_ptr, item_ptr->key, item_ptr->val, item_ptr->next);
            item_ptr = item_ptr->next;
        }
        printf(" %p\n", item_ptr);
    }
}

void destroy(hash_table_t table) {
    for (int index = 0; index < TABLE_SIZE; index++) {
        hash_table_t table_ptr = table + index;
        hash_item_t* item_ptr = *table_ptr;
        while (item_ptr != NULL) {
            hash_item_t* next_ptr = item_ptr->next;
            free(item_ptr);
            item_ptr = next_ptr;
        }
        *table_ptr = NULL;
    }
    free(table);
}

int* twoSum(int* nums, int numsSize, int target) {
    hash_table_t table = (hash_table_t) calloc(TABLE_SIZE, sizeof(hash_item_t*));

    for (int op_one = 0; op_one < numsSize; op_one++) {
        if (contains(table, target - nums[op_one])) {
            int* operands = (int*) malloc(2 * sizeof(int));
            operands[0] = get(table, target - nums[op_one]);
            operands[1] = op_one;
            return operands;
        } else {
            put(table, nums[op_one], op_one);
        }
    }
    destroy(table);
    table = NULL;

    return NULL;
}

int* twoSum_nsquared(int* nums, int numsSize, int target) {
    for (int op_one = 0; op_one < numsSize; op_one++) {
        for (int op_two = op_one + 1; op_two < numsSize; op_two++) {
            if (nums[op_one] + nums[op_two] == target) {
                int* operands = (int*) malloc(2 * sizeof(int));
                operands[0] = op_one;
                operands[1] = op_two;
                return operands;
            }
        }
    }
    return NULL;
}

#define START_TIMER \
    struct timespec time_started; \
    struct timespec time_finished; \
    clock_gettime(CLOCK_MONOTONIC, &time_started); \
    for (long test_iter = 0; test_iter < 1000000; ++test_iter) {

#define END_TIMER \
    } \
    clock_gettime(CLOCK_MONOTONIC, &time_finished);

#define TIMER_RESULT \
    long test_time = (time_finished.tv_sec * 1000000000 + time_finished.tv_nsec) - \
                     (time_started.tv_sec * 1000000000 + time_started.tv_nsec); \
    printf("Solution runtime = %ld ns", test_time / 1000000);

int main(int argc, char** argv) {
    int op;

    if (argc <= 1) {
        printf("invalid input\n");
        return 0;
    }
    /* acquire space for operands */
    int op_count = argc - 2;
    int* nums = (int*) malloc(op_count * sizeof(int));
    /* fill number buffer and save target */
    for (int arg = 1; arg < argc - 1; arg++) {
        nums[arg - 1] = atoi(argv[arg]);
    }
    int target = atoi(argv[argc - 1]);
    /* get the locations in the number buffer that contains values whose */
    /* sum is equal to the target */
    printf("Given nums = [");
    for (op = 0; op < op_count - 1; op++) {
        printf("%d, ", nums[op]);
    }
    printf("%d], target = %d\n", nums[op], target);
    int* operands;
    operands = twoSum(nums, op_count, target);
    if (operands) {
        printf("Because nums[%d] + nums[%d] = %d + %d = %d,\n", operands[0], operands[1],
                nums[operands[0]], nums[operands[1]], target);
        printf("return [%d, %d].\n", operands[0], operands[1]);
    } else {
        printf("No two sum solution.\n");
    }
    /* cleanup and exit */
    free(nums);
    if (operands) free(operands);
    return 1;
}
