#include <utility>

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
        transaction.sender = users.at(rand()%(1000)).public_key;
        do {
            transaction.recipient = users.at(rand()%(1000)).public_key;
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

string findHash(const string& prevBlockHash, vector<Transaction> transactions) {
    string newBlockHash;
    time_t timestamp = std::time(nullptr);
    string version = "version1";
    string merkleHash = merkleTreeHash(move(transactions));
    string difficultyTarget = "0";
    long long int nonce = 0;
    do {
        newBlockHash = myHash(prevBlockHash + to_string(timestamp) + version + merkleHash + to_string(nonce) + difficultyTarget);
        nonce++;
    } while (newBlockHash.substr(0, difficultyTarget.size()) != difficultyTarget);
    return newBlockHash;
}

int main() {
    vector<User> users = generateUsers(1000);
    vector<Transaction> transactions = generateTransactions(users, 10000);
    vector<Block> blocks;

    while(!transactions.empty()) {
        vector<Transaction> transactions100 = get100Transactions(transactions);
        Block block;
        string hash;
        if(blocks.empty()) {
            hash = findHash("0", transactions100);
        } else {
            hash = findHash(blocks.back().hash, transactions100);
        }

        for(int i = 0; i < transactions100.size(); i++) {
            Transaction transaction = transactions100.at(i);
            int sender = find_if(users.begin(), users.end(), [transaction, i](const User& user){return user.public_key == transaction.sender; }) - users.begin();
            int recipient = find_if(users.begin(), users.end(), [transaction, i](const User& user){return user.public_key == transaction.recipient; }) - users.begin();
            if(users.at(sender).balance >= transaction.sum) {
                users.at(sender).balance -= transaction.sum;
                users.at(recipient).balance += transaction.sum;
            } else {
                transactions100.at(i).bad = true;
            }
        }
        block.hash = hash;
        block.transactions = transactions100;
        blocks.push_back(block);
    }

    return 0;
}
