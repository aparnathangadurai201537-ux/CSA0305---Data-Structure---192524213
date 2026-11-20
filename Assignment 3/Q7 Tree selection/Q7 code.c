#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORDS 100
#define MAX_LEN 50

typedef struct Trie {
    struct Trie *child[26];
    int end;
} Trie;

Trie* createNode() {
    return (Trie*)calloc(1, sizeof(Trie));
}

void insertTrie(Trie *root, char word[]) {
    Trie *current = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';

        if (current->child[index] == NULL)
            current->child[index] = createNode();

        current = current->child[index];
    }

    current->end = 1;
}

int searchTrie(Trie *root, char word[]) {
    Trie *current = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';

        if (current->child[index] == NULL)
            return 0;

        current = current->child[index];
    }

    return current->end;
}

void autocomplete(Trie *root, char word[], int depth) {
    if (root->end)
        printf("%s\n", word);

    for (int i = 0; i < 26; i++) {
        if (root->child[i]) {
            word[depth] = 'a' + i;
            word[depth + 1] = '\0';

            autocomplete(root->child[i], word, depth + 1);
        }
    }
}

void showAutocomplete(Trie *root, char prefix[]) {
    Trie *current = root;

    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';

        if (current->child[index] == NULL) {
            printf("No suggestions\n");
            return;
        }

        current = current->child[index];
    }

    char word[MAX_LEN];
    strcpy(word, prefix);

    autocomplete(current, word, strlen(prefix));
}

void rangeList(char words[][MAX_LEN],
               int n,
               char start[],
               char end[]) {

    printf("\nWords from %s to %s:\n", start, end);

    for (int i = 0; i < n; i++) {
        if (strcmp(words[i], start) >= 0 &&
            strcmp(words[i], end) <= 0) {

            printf("%s\n", words[i]);
        }
    }
}

void destroy(Trie *root) {
    if (root == NULL)
        return;

    for (int i = 0; i < 26; i++)
        destroy(root->child[i]);

    free(root);
}

int main() {

    char words[][MAX_LEN] = {
        "apple",
        "application",
        "apply",
        "banana",
        "cat",
        "car",
        "cart",
        "dog",
        "door",
        "data"
    };

    int n = 10;

    Trie *root = createNode();

    for (int i = 0; i < n; i++)
        insertTrie(root, words[i]);

    // Exact lookup
    char key[] = "application";

    if (searchTrie(root, key))
        printf("Exact Search: %s found\n", key);
    else
        printf("Exact Search: %s not found\n", key);

    // Autocomplete
    printf("\nAutocomplete for 'app':\n");
    showAutocomplete(root, "app");

    // Alphabetical range
    rangeList(words, n, "banana", "dog");

    destroy(root);

    return 0;
}