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

int main() {
    vector<User> users = generateUsers(1000);

    return 0;
}
