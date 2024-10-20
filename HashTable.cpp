#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int EMPTY = -1;        
const int DELETED = -2;     

class HashTable {
private:
    vector<int> table;
    int currentSize;
    int itemCount;
    double loadFactorThreshold = 0.8; 

    int nextPrime(int n) {
        while (!isPrime(n)) n++;
        return n;
    }

    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

    int hash(int key) {
        return key % currentSize;
    }

    int quadraticProbe(int key, int i) {
        return (hash(key) + i * i) % currentSize;
    }

    void resize() {
        int newSize = nextPrime(2 * currentSize); 
        vector<int> newTable(newSize, EMPTY);
        for (int i = 0; i < currentSize; ++i) {
            if (table[i] != EMPTY && table[i] != DELETED) { 
                int newHash = table[i] % newSize;
                int j = 0;
                while (newTable[(newHash + j * j) % newSize] != EMPTY) {
                    j++; 
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
        table = vector<int>(currentSize, EMPTY); 
        itemCount = 0;
    }

    void insert(int key) {
        if ((double)itemCount / currentSize >= loadFactorThreshold) {
            resize();
        }
        int hashValue = hash(key);
        int i = 0;
        while (table[(hashValue + i * i) % currentSize] != EMPTY && table[(hashValue + i * i) % currentSize] != DELETED) {
            i++; 
        }
        table[(hashValue + i * i) % currentSize] = key;
        itemCount++;
    }

    int search(int key) {
        int hashValue = hash(key);
        int i = 0;
        while (table[(hashValue + i * i) % currentSize] != key) {
            if (table[(hashValue + i * i) % currentSize] == EMPTY) return -1; 
            i++;
            if (i == currentSize) return -1; 
        }
        return (hashValue + i * i) % currentSize; 
    }

    void remove(int key) {
        int hashValue = hash(key);
        int i = 0;
        while (table[(hashValue + i * i) % currentSize] != key) {
            if (table[(hashValue + i * i) % currentSize] == EMPTY) return; 
            i++;
            if (i == currentSize) return; 
        }
        table[(hashValue + i * i) % currentSize] = DELETED; 
        itemCount--;
    }

    void printTable() {
        for (int i = 0; i < currentSize; ++i) {
            if (table[i] == EMPTY || table[i] == DELETED)
                cout << "- "; 
            else
                cout << table[i] << " "; 
        }
        cout << endl;
    }
};
