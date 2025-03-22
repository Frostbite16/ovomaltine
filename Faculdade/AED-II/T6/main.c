#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_NODES 256
#define MAX_BIT_LENGTH 256

// Node structure for Huffman tree
typedef struct Node {
    unsigned char character;
    unsigned long frequency;
    struct Node *left;
    struct Node *right;
} Node;

// Priority queue structure
typedef struct {
    Node *nodes[MAX_TREE_NODES];
    int size;
} PriorityQueue;

// Create a new node
Node *createNode(unsigned char character, unsigned long frequency) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) {
        perror("Failed to allocate memory for node");
        exit(EXIT_FAILURE);
    }
    node->character = character;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Create an empty priority queue
PriorityQueue *createPriorityQueue() {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if (!pq) {
        perror("Failed to allocate memory for priority queue");
        exit(EXIT_FAILURE);
    }
    pq->size = 0;
    return pq;
}

// Insert a node into the priority queue
void insertPriorityQueue(PriorityQueue *pq, Node *node) {
    int i = pq->size++;
    while (i && node->frequency < pq->nodes[(i - 1) / 2]->frequency) {
        pq->nodes[i] = pq->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->nodes[i] = node;
}

// Remove the node with the smallest frequency from the queue
Node *removeMin(PriorityQueue *pq) {
    if (pq->size == 0) return NULL; // Check if the queue is empty

    Node *minNode = pq->nodes[0];
    Node *lastNode = pq->nodes[--pq->size];

    int i = 0, child;
    while (i * 2 + 1 < pq->size) {
        child = i * 2 + 1;
        if (child + 1 < pq->size && pq->nodes[child + 1]->frequency < pq->nodes[child]->frequency) {
            child++;
        }
        if (lastNode->frequency <= pq->nodes[child]->frequency) {
            break;
        }
        pq->nodes[i] = pq->nodes[child];
        i = child;
    }
    pq->nodes[i] = lastNode;
    return minNode;
}

// Build the Huffman tree
Node *buildHuffmanTree(unsigned long *frequencies) {
    PriorityQueue *pq = createPriorityQueue();
    for (int i = 0; i < MAX_TREE_NODES; i++) {
        if (frequencies[i] > 0) {
            insertPriorityQueue(pq, createNode(i, frequencies[i]));
        }
    }
    while (pq->size > 1) {
        Node *left = removeMin(pq);
        Node *right = removeMin(pq);
        Node *parent = createNode(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        insertPriorityQueue(pq, parent);
    }
    Node *root = removeMin(pq);
    free(pq);
    return root;
}

// Generate Huffman codes
void generateHuffmanCodes(Node *node, char *code, int depth, char codes[MAX_TREE_NODES][MAX_BIT_LENGTH]) {
    if (!node) return;

    if (node->left == NULL && node->right == NULL) {
        code[depth] = '\0';
        strcpy(codes[node->character], code);
    } else {
        code[depth] = '0';
        generateHuffmanCodes(node->left, code, depth + 1, codes);
        code[depth] = '1';
        generateHuffmanCodes(node->right, code, depth + 1, codes);
    }
}

// Serialize the Huffman tree
void serializeTree(Node *node, FILE *file) {
    if (!node) {
        fputc('#', file); // Represents a null node
        return;
    }
    fputc(node->character, file);
    serializeTree(node->left, file);
    serializeTree(node->right, file);
}

// Save compressed data
void saveCompressedData(const char *inputFileName, Node *root, unsigned long *frequencies) {
    char codes[MAX_TREE_NODES][MAX_BIT_LENGTH] = {0};
    char code[MAX_BIT_LENGTH];
    generateHuffmanCodes(root, code, 0, codes);

    // Create output file
    char outputFileName[256];
    sprintf(outputFileName, "%s.cmp", inputFileName);
    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        perror("Error opening output file");
        return;
    }

    // Write the Huffman tree to the file
    serializeTree(root, outputFile);

    // Write compressed data
    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        fclose(outputFile);
        return;
    }

    unsigned char buffer;
    unsigned long bitBuffer = 0;
    int bitCount = 0;

    while (fread(&buffer, sizeof(unsigned char), 1, inputFile)) {
        char *huffmanCode = codes[buffer];
        for (int i = 0; huffmanCode[i] != '\0'; i++) {
            bitBuffer <<= 1;
            if (huffmanCode[i] == '1') {
                bitBuffer |= 1;
            }
            bitCount++;
            if (bitCount == 8) {
                fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
                bitBuffer = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount); // Fill with zeros
        fwrite(&bitBuffer, sizeof(unsigned char), 1, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

// Deserialize the Huffman tree
Node *deserializeTree(FILE *file) {
    int ch = fgetc(file);
    if (ch == '#') return NULL; // Null node
    Node *node = createNode(ch, 0);
    node->left = deserializeTree(file);
    node->right = deserializeTree(file);
    return node;
}

// Decompress the data
void decompressFile(const char *inputFileName) {
    // Create output file
    char outputFileName[256];
    sprintf(outputFileName, "%s.dcmp", inputFileName);
    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile) {
        perror("Error opening output file");
        return;
    }

    // Open input file
    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        fclose(outputFile);
        return;
    }

    // Deserialize the Huffman tree
    Node *root = deserializeTree(inputFile);
    if (!root) {
        fprintf(stderr, "Error: Failed to deserialize Huffman tree.\n");
        fclose(inputFile);
        fclose(outputFile);
        return;
    }

    // Read compressed data and decompress
    unsigned char byte;
    Node *current = root;

    // Read byte and process bits
    while (fread(&byte, sizeof(unsigned char), 1, inputFile)) {
        for (int i = 0; i < 8; i++) {
            if (byte & (1 << (7 - i))) { // Check bit
                current = current->right;
            } else {
                current = current->left;
            }

            // If we reached a leaf, write the character
            if (current->left == NULL && current->right == NULL) {
                fputc(current->character, outputFile);
                current = root; // Restart to the top of the tree
            }
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

// Free the memory of the tree
void freeTree(Node *node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

// Main function
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <option> <input_file>\n", argv[0]);
        return 1;
    }

    const char *option = argv[1];
    const char *inputFileName = argv[2];
    unsigned long frequencies[MAX_TREE_NODES] = {0};

    FILE *inputFile = fopen(inputFileName, "rb");
    if (!inputFile) {
        perror("Error opening input file");
        return 1;
    }

    unsigned char buffer;
    while (fread(&buffer, sizeof(unsigned char), 1, inputFile)) {
        frequencies[buffer]++;
    }
    fclose(inputFile);

    if (strcmp(option, "c") == 0) {
        // Build the Huffman tree
        Node *root = buildHuffmanTree(frequencies);
        // Save the compressed data
        saveCompressedData(inputFileName, root, frequencies);
        freeTree(root);
    } else if (strcmp(option, "d") == 0) {
        // Decompress the data
        decompressFile(inputFileName);
    } else {
        printf("Invalid option. Use 'c' to compress or 'd' to decompress.\n");
        return 1;
    }

    return 0;
}
