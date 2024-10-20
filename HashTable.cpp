#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class HashTable {
private:
    vector<int> table;
    int currentSize;
    int itemCount;
    const double loadFactorThreshold = 0.8;

    // Helper function to find the next prime number (used for resizing)
    int nextPrime(int n) {
        while (!isPrime(n)) n++;
        return n;
    }

    // Check if a number is prime
    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    // Hash function
    int hash(int key) {
        return key % currentSize;
    }

    // Quadratic probing to resolve collisions
    int quadraticProbe(int key, int i) {
        return (hash(key) + i * i) % currentSize;
    }

    // Resize the hash table when load factor exceeds threshold
    void resize() {
        int newSize = nextPrime(2 * currentSize); // Ensure new size is prime
        vector<int> newTable(newSize, -1); // New table with larger prime size
        for (int i = 0; i < currentSize; ++i) {
            if (table[i] != -1) { // Rehash existing keys
                int newHash = table[i] % newSize;
                int j = 0;
                while (newTable[(newHash + j * j) % newSize] != -1) {
                    j++; // Quadratic probing to find the next available slot
                }
                newTable[(newHash + j * j) % newSize] = table[i];
            }
        }
        table = newTable;
        currentSize = newSize;
    }

public:
    HashTable(int size) {
        currentSize = nextPrime(size);
        table = vector<int>(currentSize, -1); // Initialize table with -1 (empty slots)
        itemCount = 0;
    }

    // Insert a key into the hash table
    void insert(int key) {
        if ((double)itemCount / currentSize >= loadFactorThreshold) {
            resize();
        }
        int hashValue = hash(key);
        int i = 0;
        while (table[(hashValue + i * i) % currentSize] != -1) {
            i++; // Quadratic probing in case of collision
        }
        table[(hashValue + i * i) % currentSize] = key;
        itemCount++;
    }

    // Search for a key in the hash table
    bool search(int key) {
        int hashValue = hash(key);
        int i = 0;
        while (table[(hashValue + i * i) % currentSize] != key) {
            if (table[(hashValue + i * i) % currentSize] == -1) return false; // Key not found
            i++;
            if (i == currentSize) return false; // Prevent infinite loop
        }
        return true; // Key found
    }

    // Delete a key from the hash table
    void remove(int key) {
        int hashValue = hash(key);
        int i = 0;
        while (table[(hashValue + i * i) % currentSize] != key) {
            if (table[(hashValue + i * i) % currentSize] == -1) return; // Key not found
            i++;
            if (i == currentSize) return; // Prevent infinite loop
        }
        table[(hashValue + i * i) % currentSize] = -1; // Mark slot as empty
        itemCount--;
    }

    // Print the current state of the hash table
    void printTable() {
        for (int i = 0; i < currentSize; ++i) {
            if (table[i] != -1)
                cout << i << " --> " << table[i] << endl;
            else
                cout << i << " --> [empty]" << endl;
        }
    }
};
