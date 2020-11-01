//
// Created by Patryk on 2020-11-01.
//

#ifndef BLOCKCHAIN_HASH_H
#define BLOCKCHAIN_HASH_H
#include "../header.h"

class Hash {
private:
    string text;
    string string32;
    string hashValue;
    unsigned long long int size = 0;

    void convertTo32();

    void countSize();

    void cryptString();

public:
    Hash(string str = "");

    string getHashValue();

    void hashNew(string str);
};

string myHash(const string str);

#endif //BLOCKCHAIN_HASH_H
