//
// Created by Patryk on 2020-11-01.
//

#ifndef BLOCKCHAIN_HEADER_H
#define BLOCKCHAIN_HEADER_H

#include <bitcoin/bitcoin.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <algorithm>
#include <cstring>
#include <bits/stdc++.h>
#include <iterator>

using namespace std;

#include "hFiles/Hash.h"

struct User {
    string name;
    string public_key;
    double balance;
};

struct Transaction {
    string id;
    string sender;
    string recipient;
    double sum;
    bool bad = false;
};

struct TransactionBox {
    vector<Transaction> transactions;
};

struct Block {
    string hash;
    vector<Transaction> transactions;
};

#endif //BLOCKCHAIN_HEADER_H
