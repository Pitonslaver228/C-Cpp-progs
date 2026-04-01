#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define ALPHABET_SIZE 26

// структура узла Trie
typedef struct TrieNode {
    int isEnd;
    struct TrieNode* children[ALPHABET_SIZE];
} TrieNode;

// создание нового узла
TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEnd = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;
    return node;
}

// вставка слова
void insert(TrieNode* root, const char* str) {
    TrieNode* current = root;
    while (*str) {
        int index = *str - 'a';
        if (!current->children[index])
            current->children[index] = createNode();
        current = current->children[index];
        str++;
    }
    current->isEnd = 1;
}

// проверка, идут ли буквы по алфавиту
int is_alphabetical(const char* str, int start, int end) {
    for (int i = start; i < end - 1; i++) {
        if (str[i] > str[i + 1])
            return 0;
    }
    return 1;
}

// обработка слова — добавление всех подстрок, идущих по алфавиту
void process_word(TrieNode* root, const char* word) {
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j <= len; j++) {
            if (is_alphabetical(word, i, j)) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, &word[i], j - i);
                temp[j - i] = '\0';
                insert(root, temp);
            }
        }
    }
}

// прямой обход (в строку)
void preorder(TrieNode* node, char* buffer, int depth, char* result) {
    if (!node) return;
    if (node->isEnd) {
        buffer[depth] = '\0';
        strcat(result, buffer);
        strcat(result, " ");
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[depth] = i + 'a';
            preorder(node->children[i], buffer, depth + 1, result);
        }
    }
}

// центральный обход (в строку)
void inorder(TrieNode* node, char* buffer, int depth, char* result) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[depth] = i + 'a';
            inorder(node->children[i], buffer, depth + 1, result);
        }
    }
    if (node->isEnd) {
        buffer[depth] = '\0';
        strcat(result, buffer);
        strcat(result, " ");
    }
}

// обратный обход (в строку)
void postorder(TrieNode* node, char* buffer, int depth, char* result) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[depth] = i + 'a';
            postorder(node->children[i], buffer, depth + 1, result);
        }
    }
    if (node->isEnd) {
        buffer[depth] = '\0';
        strcat(result, buffer);
        strcat(result, " ");
    }
}

// поиск слова
int search(TrieNode* root, const char* str) {
    TrieNode* current = root;
    while (*str) {
        int index = *str - 'a';
        if (!current->children[index])
            return 0;
        current = current->children[index];
        str++;
    }
    return current && current->isEnd;
}

// освобождение памяти
void freeTrie(TrieNode* node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        freeTrie(node->children[i]);
    free(node);
}

// линейный поиск
int linear_search(char* arr[], int n, const char* target) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i], target) == 0)
            return i;
    }
    return -1;
}

int main() {
    TrieNode* root = createNode();

    // тестовые слова
    char* words[] = {"abc", "acb", "xyz", "ace", "bac", "aeg", "bdf"};
    int n = sizeof(words) / sizeof(words[0]);

    printf("Words:\n");
    for (int i = 0; i < n; i++) {
        printf("%s ", words[i]);
        process_word(root, words[i]);
    }

    printf("\n\n---------- Obhodbl Trie ----------\n");

    char buffer[MAX_WORD_LEN];
    char result[1000];

    // preorder
    result[0] = '\0';
    preorder(root, buffer, 0, result);
    printf("Preorder: %s\n", result);

    // inorder
    result[0] = '\0';
    inorder(root, buffer, 0, result);
    printf("Inorder: %s\n", result);

    // postorder
    result[0] = '\0';
    postorder(root, buffer, 0, result);
    printf("Postorder: %s\n", result);

    // последовательный поиск
    printf("\n---------- Posledovatelny searching 'ace' ----------\n");
    int position = linear_search(words, n, "ace");
    if (position >= 0)
        printf("Found on position %d\n", position);
    else
        printf("Не найдено\n");

    // поиск в Trie
    printf("\n---------- Searching in Trie ----------\n");
    printf("Searching 'abc': %s\n", search(root, "abc") ? "found" : "not found");
    printf("Searching 'ac': %s\n", search(root, "ac") ? "found" : "not found");

    // очистка памяти
    freeTrie(root);
    return 0;
}
