#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Trie {
    struct Trie *child[26];
    int end;
    int words;
} Trie;

Trie* createNode() {
    return (Trie*)calloc(1, sizeof(Trie));
}

void insert(Trie *root, const char *word) {
    Trie *current = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';

        if (current->child[index] == NULL)
            current->child[index] = createNode();

        current = current->child[index];
        current->words++;
    }

    current->end = 1;
}

int search(Trie *root, const char *word) {
    Trie *current = root;

    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';

        if (current->child[index] == NULL)
            return 0;

        current = current->child[index];
    }

    return current->end;
}

int countPrefix(Trie *root, const char *prefix) {
    Trie *current = root;

    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';

        if (current->child[index] == NULL)
            return 0;

        current = current->child[index];
    }

    return current->words;
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

void showPrefix(Trie *root, const char *prefix) {
    Trie *current = root;

    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = prefix[i] - 'a';

        if (current->child[index] == NULL) {
            printf("No words found\n");
            return;
        }

        current = current->child[index];
    }

    char word[100];
    strcpy(word, prefix);

    autocomplete(current, word, strlen(prefix));
}

void destroy(Trie *root) {
    if (root == NULL)
        return;

    for (int i = 0; i < 26; i++)
        destroy(root->child[i]);

    free(root);
}

int main() {
    const char *contacts[] = {
        "anand",
        "anitha",
        "anu",
        "arun",
        "bala",
        "anusuya"
    };

    Trie *root = createNode();

    for (int i = 0; i < 6; i++)
        insert(root, contacts[i]);

    printf("Prefix counts:\n");

    printf("a: %d\n", countPrefix(root, "a"));
    printf("an: %d\n", countPrefix(root, "an"));
    printf("anu: %d\n", countPrefix(root, "anu"));
    printf("b: %d\n", countPrefix(root, "b"));
    printf("z: %d\n", countPrefix(root, "z"));

    printf("\nSearch 'anitha': ");

    if (search(root, "anitha"))
        printf("Found\n");
    else
        printf("Not Found\n");

    printf("\nAutocomplete for 'an':\n");
    showPrefix(root, "an");

    destroy(root);

    return 0;
}