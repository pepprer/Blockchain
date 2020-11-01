//
// Created by Patryk on 2020-11-01.
//

#include "../hFiles/Hash.h"

void Hash::convertTo32() {
    int textLength = text.length();

    if (textLength == 0) string32 = "0123456789ABCDEFabcdefghijklmnpt";

    else if (textLength % 32 == 0) string32 = text;

    else {
        string saveText = text;

        while (textLength < 32) {
            text += saveText;
            textLength = text.length();
        }

        if (textLength % 32 == 0) {
            string32 = text;
        } else {
            int gcd;

            for (int i = 1; i <= 32 && i <= textLength; i++) {
                if (32 % i == 0 && textLength % i == 0) {
                    gcd = i;
                }
            }

            saveText = text;

            for (int i = 0; i < gcd; i++) {
                text += saveText;
            }
            string32 = text;
        }
    }

    int length = string32.length() / 32;


    if (length != 1) {
        int acv = size % length;

        string string32Return;

        for (int i = 0; i < 32; i++) {
            string32Return += string32.at(i * length + acv);
        }

        string32 = string32Return;
    }
}

Hash::Hash(string str) {
    text = str;
    countSize();
    convertTo32();
    cryptString();
}

void Hash::countSize() {
    for (char s: text) {
        size += (int) s;
    }
    size += text.size();
}

void Hash::cryptString() {
    static const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; //0123456789abcdef
    unsigned long long int crypt = 43;
    if (size == 0) size = 1;
    for (char c : string32) {
        crypt += (int) c * size * (int) c;
        if (crypt % 124 > 62) {
            hashValue += alphabet[crypt % 124 - 62];
        } else if (crypt % 92 > 32) {
            hashValue += alphabet[crypt % 92 - 32];
        } else if (crypt % 88 > 12) {
            hashValue += alphabet[crypt % 92 + 17];
        } else if (crypt % 62 > 32) {
            hashValue += alphabet[crypt % 62 - 20];
        } else {
            hashValue += alphabet[crypt % 62];
        }
    }

    crypt = 777;
    for (char c : string32) {
        crypt += (int) c * size * (int) c;
        if (crypt % 186 > 124) {
            hashValue += alphabet[crypt % 186 - 124];
        } else if (crypt % 77 > 32) {
            hashValue += alphabet[crypt % 77 - 32];
        } else if (crypt % 66 > 32) {
            hashValue += alphabet[crypt % 66 - 32];
        } else if (crypt % 34 > 12) {
            hashValue += alphabet[crypt % 34 - 12];
        } else {
            hashValue += alphabet[crypt % 62];
        }
    }

    stringstream ss;
    int value = 0;
    for(char s : hashValue) {
        ss << hex << (int)s;
        value += (int)s;
    }

    hashValue = ss.str();

    string string64;
    int which = value % 2;
    for(int i = 0; i < 64; i++) {
        string64 += hashValue.at(i * 2 + which);
    }

    hashValue = string64;
}

string Hash::getHashValue() {
    return hashValue;
}

void Hash::hashNew(string str) {
    text = str;
    string32 = "";
    hashValue = "";
    size = 0;
    countSize();
    convertTo32();
    cryptString();
}

string myHash(const string str)
{
    Hash hash = Hash(str);

    return hash.getHashValue();
}