/*
 * Based on code by SHARAD VERMA
 * Implementation of 128-bit AES encryption and decryption.
 * Source: https://coderspacket.com/128-bit-aes-encryptor-and-decryptor-in-c#google_vignette
 *
 * Key is stored in file key.txt
 * Encryption reads plain text from input.txt
 * and stores encrypted data in encryption.aes
 * Decryption reads encrypted data from encryption.text
 * and stores decrypted data in outputtext.txt file
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "key_expand.h"
#include "encoding.h"
#include "decoding.h"
#include <windows.h>
#include <chrono>
using namespace std;

int main(){
    while (TRUE) {
        int choice;
        cout << "\n\nWelcome to 128 bits AES encryption \n\n";
        cout << "Enter you choice\n";
        cout << "1 - Encoding\n";
        cout << "2 - Decoding\n";
        cout << "3 - Exit\n";
        cin >> choice;
        cout << "\n\n";

        switch (choice) {
            case 1: {
                ifstream File;
                string filepath = "../encryption.aes";
                File.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc);
                if (!File.is_open() || File.fail()) {
                    File.close();
                    printf("\nError : failed to erase file content !");
                    continue;
                }
                File.close();
                //cout << " \"encryption.aes\" successfully erased. \n";
                long long int count = 0;
                long long int lines = 0;

                ifstream newFile("../input.txt");
                if (!(newFile.is_open())) {
                    cout << "ERROR: file could not be opened /n";
                    continue;
                }
                //cout << "\"input.txt\" successfully opened .........\n";
                string tp;
                string k;
                ifstream infile;
                infile.open("../key.txt");
                if (!(infile.is_open())) {
                    cout << "Unable to open file";
                    continue;
                }
                getline(infile, k);
                infile.close();
                // cout << "Key retrieved!\n";

                unsigned long int start_time = std::chrono::duration_cast<std::chrono::microseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();

                while (getline(newFile, tp)) {
                    unsigned long long messLength = tp.length();
                    unsigned long long extendedLength;
                    if ((messLength % 16) != 0) {
                        extendedLength = messLength + (16 - (messLength % 16));
                    } else {
                        extendedLength = messLength;
                    }
                    unsigned char *encryptedText = new unsigned char[extendedLength];
                    for (int i = 0; i < extendedLength; i++) {
                        if (i < messLength) {
                            encryptedText[i] = tp[i];
                        } else {
                            encryptedText[i] = 0;
                        }
                    }
                    istringstream tempKey(k);
                    unsigned char key[16];
                    unsigned int x;
                    for (unsigned char &i: key) {
                        tempKey >> hex >> x;
                        i = x;
                    }
                    //extending key
                    unsigned char extendedKeys[176];
                    Key_extenxion(key, extendedKeys);

                    //encrypting our plain text
                    for (int i = 0; i < extendedLength; i += 16) {
                        auto *temp = new unsigned char[16];
                        memset(temp, 0, sizeof(*temp));
                        for (int j = 0; j < 16; j++) {
                            temp[j] = encryptedText[i + j];
                            count += 1;
                        }
                        encryption(temp, extendedKeys);
                        for (int j = 0; j < 16; j++) {
                            encryptedText[i + j] = temp[j];
                        }
                    }

                    ofstream fout;
                    ifstream fin;
                    fin.open("../encryption.aes");
                    fout.open("../encryption.aes", ios::app); // Append mode
                    if (fin.is_open()) {
                        for (int i = 0; i < extendedLength; i++){
                            fout << encryptedText[i];
                        }
                        fout << "\n"; // Writing data to file
                        lines += 1;
                    } else {
                        cout << "ERROR : could not open encryption.aes";
                        continue;
                    }
                    fin.close();
                    fout.close();
                }

                unsigned long int end_time = std::chrono::duration_cast<std::chrono::microseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                cout << "128-bit AES encryption is done successfully in " << float(end_time - start_time)
                     << " microseconds \n";
                cout << "Data has been appended to file encryption.aes \n";
                cout << "Character Count : " << count << "\n";
                cout << "Lines : " << lines << "\n";
                newFile.close();
                break;
            }
            case 2: {
                string tp;
                ifstream File;
                string filepath = "../outputtext.txt";
                File.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc);
                if (!File.is_open() || File.fail()) {
                    File.close();
                    printf("\nError : failed to erase file content !");
                    continue;
                }
                File.close();

                cout << "Decrypted text:\n";
                ifstream MyReadFile;
                string myText;
                unsigned long long int count = 0;
                MyReadFile.open("../encryption.aes", ios::in | ios::binary);
                if (not MyReadFile.is_open()) {
                    cout << "ERROR: Can not open .aes file\n ";
                    continue;
                }

                string k;
                ifstream infile;
                infile.open("../key.txt");
                if (!(infile.is_open())) {
                    cout << "Error: Unable to open key.txt";
                    break;
                }

                getline(infile, k); // The first line of file should be the key
                infile.close();

                istringstream tempkey(k);
                unsigned char key[16];
                memset(key,0,sizeof(key));
                unsigned int x1;
                for (unsigned char & i : key) {
                    tempkey >> hex >> x1;
                    i = x1;
                }

                //extending key
                unsigned char extendedkeys[176];
                memset(extendedkeys,0,sizeof(extendedkeys));
                Key_extenxion(key, extendedkeys);

                unsigned long int start_time = std::chrono::duration_cast<std::chrono::microseconds>(
                                            std::chrono::system_clock::now().time_since_epoch()).count();

                while (TRUE){
                    string oldText = myText;
                    getline(MyReadFile, myText);
                    if (myText[0] == 0){
                        break;
                    }
                    if (oldText == myText){
                        break;
                    }
                    count += 1;
                    char *x;
                    x = &myText[0];
                    int messlength = strlen(x);
                    cout << "MESSLENGTH :::" << messlength ;
                    char *msg = new char[myText.size() + 1];

                    strcpy(msg, myText.c_str());

                    int n = strlen(msg);
                    unsigned char *decryptedtext = new unsigned char[n];
                    memset(decryptedtext,0,sizeof(*decryptedtext));
                    //decrypting our encrypted data
                    for (int i = 0; i < n; i++) {
                        decryptedtext[i] = (unsigned char) msg[i];
                    }

                    //decrypting our data

                    for (int i = 0; i < messlength; i += 16) {
                        auto *temp = new unsigned char[16];
                        memset(temp,0,sizeof(*temp));
                        for (int j = 0; j < 16; j++) {
                            temp[j] = decryptedtext[i + j];
                        }
                        decryption(temp, extendedkeys);
                        for (int j = 0; j < 16; j++) {
                            decryptedtext[i + j] = temp[j];
                        }
                    }

                    ofstream fout;  // Create Object of Ofstream
                    ifstream fin;
                    fin.open("../outputtext.txt");
                    fout.open("../outputtext.txt", ios::app); // Append mode
                    if (fin.is_open()) {
                        fout << decryptedtext << "\n"; // Writing data to file
                        cout << decryptedtext << "\n";
                        fin.close();
                        fout.close(); // Closing the file
                    }
                }
                unsigned long int end_time = std::chrono::duration_cast<std::chrono::microseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();

                cout << "\nData has been saved to file outputtext.txt \n";
                cout << "Time: " << end_time - start_time << " microseconds\n";
                cout << "Count : " << count << "\n";
                MyReadFile.close();
                break;
            }
            case 3: {
                return 69;
            }
            default: {
                cout << "Please enter a Valid Menu Option \n\n\n ";
            }
        }
    }
}
