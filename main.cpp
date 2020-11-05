#include "header.h"

vector<User> generateUsers(int n)
{
    vector<User> users;
    User user;
    for(int i = 0; i < n; i++) {
        user.name = "Vardas" + to_string(i) +" Pavarde" + to_string(i);
        user.public_key = myHash(user.name);
        user.balance = rand()%(1000000-100 + 1) + 100;
        users.push_back(user);
    }
    return users;
}

vector<Transaction> generateTransactions(vector<User> users, int n)
{
    vector<Transaction> transactions;
    Transaction transaction;
    for(int i = 0; i < n; i++) {
        transaction.sender = users.at(rand()%(100 + 1)).public_key;
        do {
            transaction.recipient = users.at(rand()%(100 + 1)).public_key;
        } while (transaction.sender == transaction.recipient);
        transaction.sum = rand()%(1000-1 + 1) + 1;
        transaction.id = myHash(transaction.sender + transaction.recipient + to_string(transaction.sum));
        transactions.push_back(transaction);
    }
    return transactions;
}

vector<Transaction> get100Transactions(vector<Transaction>& transactions) {
    vector<Transaction> returnTransactions;
    for(int i = 0; i < 100; i++) {
        returnTransactions.push_back(transactions.at(0));
        transactions.erase(transactions.begin());
    }
    return returnTransactions;
}

string merkleTreeHash(vector<Transaction> transactions) {
    int size = transactions.size();
    vector<string> hashes;
    for(int i = 0; i < size; i++) {
        if(i + 1 < size) {
            hashes.push_back(myHash(transactions.at(i).id + transactions.at(i+1).id));
            i++;
        } else {
            hashes.push_back(myHash(transactions.at(i).id));
        }
    }

    while(hashes.size() != 1) {
        vector<string> newHashes;
        size = hashes.size();

        for(int i = 0; i < size; i++) {
            if(i + 1 < size) {
                newHashes.push_back(myHash(hashes.at(i) + hashes.at(i+1)));
                i++;
            } else {
                newHashes.push_back(myHash(hashes.at(i)));
            }

        }
        hashes = newHashes;
    }

    return hashes.at(0);
}

int main() {
    vector<User> users = generateUsers(1000);
    vector<Transaction> transactions = generateTransactions(users, 10000);
    vector<Transaction> transactions100 = get100Transactions(transactions);
    cout << merkleTreeHash(transactions100) << endl;
    return 0;
}
