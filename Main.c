#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_SIZE 20

int ARRAY_SIZE = 0;

typedef struct node
{
    char data[MAX_WORD_SIZE];
    struct node *next;
} node;


node *create_node(char *data)
{
    node *new_node = (node *)malloc(sizeof(node));
    strcpy(new_node->data, data);
    new_node->next = NULL;
    return new_node;
}

node *insert_node(node *head, char *data)
{
    node *new_node = create_node(data);
    new_node->next = head;
    head = new_node;
    return head;
}

node *search_node(node *head, char *data)
{
    node *current = head;
    while (current != NULL)
    {
        if (strcmp(current->data, data) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

node *build_list(node *head, FILE *fptr)
{
    char word[MAX_WORD_SIZE];
    while (fscanf(fptr, "%s", word) != EOF)
    {
        head = insert_node(head, word);
    }
    return head;
}


void print_list(node *head)
{
    node *current = head;
    while (current != NULL)
    {
        printf("%s\n", current->data);
        current = current->next;
    }
}


void execSAM(FILE *fptr, char *search_word)
{
    node *head = NULL;
    head = build_list(head, fptr);

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    node* found = search_node(head, search_word);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    if (found != NULL)
    {
        printf("Word found: %s\n", found->data);
    }
    else
    {
        printf("Word not found\n");
    }
    printf("Time taken by SAM search: %f seconds\n", cpu_time_used);
}

void search_word_in_array(char words[ARRAY_SIZE][MAX_WORD_SIZE], int length, char* search_word){
    int visited[250000] = {0};

    int count = 0;
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    while (count < length) {
        int index = rand() % length;
        if (!visited[index]) {
            visited[index] = 1;
            count++;
            if (strcmp(words[index], search_word) == 0) {
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Word found: %s\n", words[index]);
                printf("Time taken by RAM search: %f seconds\n", cpu_time_used);
                return;
            }
        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Word not found\n");
    printf("Time taken by RAM search: %f seconds\n", cpu_time_used);
}

void execRAM(FILE* fptr, char* search_word){
    char words[ARRAY_SIZE][MAX_WORD_SIZE];
    int i = 0;
    while (fscanf(fptr, "%s", words[i]) != EOF) i++;

    search_word_in_array(words, i, search_word);
}

char* get_random_word(FILE* fptr){
    char words[ARRAY_SIZE][MAX_WORD_SIZE];
    int i = 0;
    while (fscanf(fptr, "%s", words[i]) != EOF) i++;

    int index = rand() % i;
    char* word = malloc(sizeof(char) * (strlen(words[index]) + 1));
    strcpy(word, words[index]);

    return word;
}

int main(int argc, char** argv)
{

    //Nomes: Felipe T. Malacarne e Mateus R. Zanella

    char filename[50];
    int file;
    printf("1. 10 words\n");
    printf("2. 50 words\n");
    printf("3. 100 words\n");
    printf("4. 500 words\n");
    printf("5. 100000 words\n");
    printf("6. 250000 words\n");
    printf("Enter the number of the file to open for reading: ");
    scanf("%d", &file);

    switch (file)
    {
    case 1:
        strcpy(filename, "./files/10p.txt");
        ARRAY_SIZE = 10;
        break;
    case 2:
        strcpy(filename, "./files/50p.txt");
        ARRAY_SIZE = 50;
        break;
    case 3:
        strcpy(filename, "./files/100p.txt");
        ARRAY_SIZE = 100;
        break;
    case 4:
        strcpy(filename, "./files/500p.txt");
        ARRAY_SIZE = 500;
        break;
    case 5:
        strcpy(filename, "./files/100000p.txt");
        ARRAY_SIZE = 100000;
        break;
    case 6:
        strcpy(filename, "./files/250000p.txt");
        ARRAY_SIZE = 250000;
        break;
    default:
        printf("Invalid file number\n");
        return 0;
    }

    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        return 0;
    }

    srand(time(NULL));

    char* search_word = get_random_word(fptr);
    printf("Searching for word: %s\n", search_word);

    fclose(fptr);
    fptr = fopen(filename, "r");

    execRAM(fptr, search_word);

    fclose(fptr);
    fptr = fopen(filename, "r");

    execSAM(fptr, search_word);

    fclose(fptr); 
    return 0;
}