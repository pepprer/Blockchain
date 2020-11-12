#include "header.h"

vector<User> generateUsers(int n) {
    vector<User> users;
    User user;
    for (int i = 0; i < n; i++) {
        user.name = "Vardas" + to_string(i) + " Pavarde" + to_string(i);
        user.public_key = myHash(user.name);
        user.balance = rand() % (1000000 - 100 + 1) + 100;
        users.push_back(user);
    }
    return users;
}

vector<Transaction> generateTransactions(vector<User> users, int n) {
    vector<Transaction> transactions;
    Transaction transaction;
    for (int i = 0; i < n; i++) {
        transaction.sender = users.at(rand() % (1000)).public_key;
        do {
            transaction.recipient = users.at(rand() % (1000)).public_key;
        } while (transaction.sender == transaction.recipient);
        transaction.sum = rand() % (1000 - 1 + 1) + 1;
        transaction.id = myHash(transaction.sender + transaction.recipient + to_string(transaction.sum));
        transactions.push_back(transaction);
    }
    return transactions;
}

vector<Transaction> get100Transactions(vector<Transaction> transactions) {
    vector<Transaction> returnTransactions;
    if (transactions.size() > 100) {
        int nums[transactions.size()];
        for (int i = 0; i < transactions.size(); i++) nums[i] = i;
        shuffle(nums, nums + transactions.size(), mt19937(random_device()()));
        for (int i = 0; i < 100; i++) {
            returnTransactions.push_back(transactions.at(i));
        }
    } else {
        returnTransactions = transactions;
    }

    return returnTransactions;
}

string merkleTreeHash(vector<Transaction> transactions) {
    int size = transactions.size();
    vector<string> hashes;
    for (int i = 0; i < size; i++) {
        if (i + 1 < size) {
            hashes.push_back(myHash(transactions.at(i).id + transactions.at(i + 1).id));
            i++;
        } else {
            hashes.push_back(myHash(transactions.at(i).id));
        }
    }

    while (hashes.size() != 1) {
        vector<string> newHashes;
        size = hashes.size();

        for (int i = 0; i < size; i++) {
            if (i + 1 < size) {
                newHashes.push_back(myHash(hashes.at(i) + hashes.at(i + 1)));
                i++;
            } else {
                newHashes.push_back(myHash(hashes.at(i)));
            }

        }
        hashes = newHashes;
    }

    return hashes.at(0);
}

string findHash(const string &prevBlockHash, vector<Transaction> transactions, long long int nonceTo) {
    string newBlockHash;
    time_t timestamp = std::time(nullptr);
    string version = "version1";
    string merkleHash = merkleTreeHash(move(transactions));
    string difficultyTarget = "0";
    long long int nonce = 0;
    for (long long int i = nonce; i <= nonceTo; i++) {
        newBlockHash = myHash(
                prevBlockHash + to_string(timestamp) + version + merkleHash + to_string(i) + difficultyTarget);
        if (newBlockHash.substr(0, difficultyTarget.size()) == difficultyTarget) {
            return newBlockHash;
        }
    }
    return "";
}

int main() {
    vector<User> users = generateUsers(1000);
    vector<Transaction> transactions = generateTransactions(users, 10000);
    vector<Block> blocks;
    TransactionBox transactionBox;
    Block block;
    int index;
    int nums[5] = {0, 1, 2, 3, 4};

    while (!transactions.empty()) {
        vector<TransactionBox> transactionBoxes;
        for (int i = 0; i < 5; i++) {
            transactionBox.transactions = get100Transactions(transactions);
            transactionBoxes.push_back(transactionBox);
        }

        shuffle(nums, nums + 5, mt19937(random_device()()));

        string hash;
        long long int nonceTo = 100000;

        while (hash.empty()) {
            int i = 0;

            while (i < 5 && hash.empty()) {
                if (blocks.empty()) {
                    hash = findHash("0", transactionBoxes.at(i).transactions, nonceTo);
                } else {
                    hash = findHash(blocks.back().hash, transactionBoxes.at(i).transactions, nonceTo);
                }
                i++;
            }

            if (!hash.empty()) {
                index = i;
            } else {
                nonceTo += 100000;
            }
        }

        for (auto & transaction : transactionBoxes.at(index).transactions) {

            int sender = find_if(users.begin(), users.end(),
                                 [transaction](const User &user) { return user.public_key == transaction.sender; }) -
                         users.begin();

            int recipient = find_if(users.begin(), users.end(), [transaction](const User &user) {
                return user.public_key == transaction.recipient;
            }) - users.begin();

            int transactionNumber = find_if(transactions.begin(), transactions.end(),
                                            [transaction](const Transaction &transactionHere) {
                                                return transactionHere.id == transaction.id;
                                            }) - transactions.begin();

            string transactionId = myHash(transaction.sender + transaction.recipient + to_string(transaction.sum));

            if (users.at(sender).balance >= transaction.sum && transaction.id == transactionId) {
                users.at(sender).balance -= transaction.sum;
                users.at(recipient).balance += transaction.sum;
            } else {
                transaction.bad = true;
            }

            transactions.erase(transactions.begin() + transactionNumber);
        }
        cout << "Iskastas blokas: " << hash << endl;
        block.hash = hash;
        block.transactions = transactionBoxes.at(index).transactions;
        blocks.push_back(block);
    }

    return 0;
}
