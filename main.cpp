#include <iostream>
#include <string.h>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <stdlib.h>

/************************************************** CONSTANTS *******************************************/
const float G_R = 1.61803398875;
const int MSIZE = 200000;                          
int64_t QMatrix[4] = {1,1,1,0};
const char DELIM = '$';

int64_t resultMatrix[MSIZE];
int ERR_COUNT = 0;
int ERR_FIXED = 0;
/************************************************ FRUNCTIONS ********************************************/

std::string Menu();                                                                        //returns menu choice (i is for sneaky reasons)
void stringToInts();                                                                             //2 way conversion
void encryption();                                                                                  // encryption
void decryption();
void displayStoredData();                                                                     // displays data in resultMatrix

void QMpower(int exp, int64_t returnMatrix[4]);                                                   //raises Q-Matrix to the exp^th power
void matrixMult(const int64_t Matrix0[4],const int64_t matrix1[4], int64_t returnMatrix[4]);      //multiplies Matrix0 with Matrix1
void printM(int64_t Matrix[4]);                                                                   //prints Matrix0 nicely
void genCipTxt(int64_t Plain[4], int64_t key[4], int64_t returnMatrix[5]);                        //hmph
void genPlainTxt(int64_t Cipher[5], int64_t key[4], int64_t returnMatrix[4],int check);
void matrixInv(int64_t Matrix0[4],int64_t returnMatrix[4]);                                     //finds inverse matrix of Matrix0
int64_t det(int64_t Matrix0[4]);                                                                //finds determinant
void copyintoMatrix(std::string text, int64_t returnMatrix[MSIZE]);
bool errorcheck(int64_t Matrix0[5]);                                                              //returns 1 if there is an error
void errorCorrect(int64_t Matrix0[5],int64_t returnMatrix[4]);                                  //if possible, returns a fixed matrix
void readFile();
void writeFile();

/************************************************ MAIN ***************************************************/
int main() {
    memset (resultMatrix, 0, sizeof(resultMatrix));

    std::cout << "\n\n\nWelcome to Golden Encryption!! \n";
    std::cout << "Version:0.8.34\n";
    std::cout << "DELIM character is:" << DELIM << "\n";
    std::cout << "File directory for this program is: \n";
    std::cout << std::filesystem::current_path() << "\n";
    std::string OPT;

    while (1) {
        OPT = Menu();
        ERR_COUNT = 0;
        ERR_FIXED = 0;
        if (OPT == "E") {
            encryption();
        } else if (OPT == "D") {
            decryption();
            std::cout << "Decryption Over!";
        } else if (OPT == "C") {
            std::cout << "HIIIIIII";
            stringToInts();
        } else if (OPT == "S") {
            displayStoredData();
        } else if (OPT == "R") {
            readFile();
        } else if (OPT == "W") {
            writeFile();
        } else if (OPT == "X") {
            break;
            return 0;
        } else {
            std::cout << "That wasnt a valid option. Try again. \n";
        }
    }
    std::cout << "Bye.";
    return 0;
}

/***************************************************** ACtual FUNctionS ***********************************************/
std::string Menu(){
    std::string option;
    std::cout << "\n\n\nMenu: \n";
    std::cout << "  C - string-ASCII Conversion\n";
    std::cout << "  E - Encryption \n";
    std::cout << "  D - Decryption \n";
    std::cout << "  S - display Stored data\n";
    std::cout << "  R - Read data from a file\n";
    std::cout << "  W - Write data to a file\n";
    std::cout << "  X - eXit \n";
    std::getline(std::cin, option);
    return option;
}

void stringToInts(){
    std::string choice ="0";
    int i,j,count;
    count = 0;
    j=0;
    std::string entry_par;
    while ((choice != "1") and (choice != "2")) {
        std::cout << "Which way do you want to translate? \n"
                     "  1 - String to ASCII\n"
                     "  2 - ASCII to string\n";
        std::cin >> choice;
    }

    if (choice == "2") {
        std::string entry,temp_value;
        int length;
        std::cout << "Enter your numbers (ending with a "<<DELIM<<") or type 's"<<DELIM<<"' to used saved data: \n";
        std::getline(std::cin,entry,DELIM);
        if(entry.find("s") != std::string::npos) {
            std::cout << "Fetching stored data...\n";
            i = 0;
            while(resultMatrix[i] != 0){
                std::cout << char(int(resultMatrix[i]));
                i++;
            }
        } else {
            length = entry.length();
            for (i = 0; i <= length; i++) {
                if (isdigit(entry[i])) {
                    temp_value += entry[i];
                } else if (temp_value != "" and temp_value != "\n") {
                    std::cout << char(std::stoi(temp_value));
                    temp_value = "";
                }
            }
        }
        return;
    } else if (choice != "1"){
        std::cout << "Invalid input.\n";
        return;
    }
    std::cout << "Enter your text, ending with the character "<<DELIM<<": \n";
    std::string story;
    std::getline(std::cin,story,DELIM);
    std::cout << "Length:" << story.length() << "\n";
    for( i =0; i< story.length(); i++) {
        if(float(i)>(float(MSIZE)*0.8)-2){
            std::cout << "Too many characters, only first 160k taken. \n";
            break;
        }
        resultMatrix[i]=int(story[i]);
    }
    std::cout << "\nThis data has been saved and can be access again.";
}

void encryption() {
    std::cout << "Encryption\n";
    int keyPower,i,j,length;
    int64_t key[4], plainText[4], cipherText[5];
    int64_t plain[MSIZE];
    memset (plain, 0, sizeof(plain));             //sets all of plain[] to 0

    std::cout << "Enter plaintext or type 's"<<DELIM<<"' to access saved data.   [MUST END IN '"<<DELIM<<"']: \n";
    std::string entry;

    std::getline(std::cin,entry,DELIM);
    if(entry.find("s") == std::string::npos) {
        copyintoMatrix(entry, plain);
    } else {
        std::cout << "Fetching stored data...\n";
        i = 0;
        while (not(resultMatrix[i] == 0 and resultMatrix[i+1] == 0) and i < MSIZE){
            plain[i]=resultMatrix[i];
            i++;
        }
    }
    memset (resultMatrix, 0, MSIZE);
    std::cout << "Enter the key: \n";
    std::cin >> keyPower;
    QMpower(keyPower, key);

    i = 0;
    uint64_t start_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    while(i*1000 < MSIZE and plain[1000*i] != 0) {
        j = 0;
        while(plain[1000*i+4*j] != 0 and j < 250) {
            plainText[0] = plain[1000 * i + 4 * j];
            plainText[1] = plain[1000 * i + 4 * j + 1];
            plainText[2] = plain[1000 * i + 4 * j + 2];
            plainText[3] = plain[1000 * i + 4 * j + 3];
            genCipTxt(plainText, key, cipherText);
            for (int k = 0; k < 5; k++) {
                resultMatrix[1250 * i + 5 * j + k] = cipherText[k];
            }
            j++;
        }
        i++;
    }
    uint64_t end_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock ::now().time_since_epoch()).count();
    i = 0;
    std::cout << "This data has been stored.\n";
    std::cout << " \n\n Finished in " << float(end_time - start_time) << " microseconds. \n";  //do I need a comment here?
}

void decryption() {
    std::cout << "Decryption\n";
    int keyPower,i,length,check;
    int64_t key[4],IKey[4], plainText[5], cipherText[4], Ciph[MSIZE];
    memset (Ciph, 0, sizeof(Ciph));

    std::cout << "Enter your Cipher code or type's"<<DELIM<<"' to access saved data [MUST END WITH A '"<<DELIM<<"': \n";
    std::string entry;
    std::getline(std::cin,entry,DELIM);
    if(entry.find("s") == std::string::npos) {
        copyintoMatrix(entry,Ciph);
    } else {
        i = 0;
        while(not(resultMatrix[i] == 0 and resultMatrix[i+1] == 0)){
            Ciph[i]=resultMatrix[i];
            i++;
        }
    }
    memset (resultMatrix, 0, sizeof(resultMatrix));
    std::cout << "Enter the key: \n";
    std::cin >> keyPower;
    QMpower(keyPower, key);
    matrixInv(key, IKey);
    std::cout << "Do you want to check for errors (will add time). Enter 1 for yes:\n";
    std::cin >> check;
    i=0;

    unsigned long int start_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while(Ciph[5*i]!=0){
        cipherText[0] = Ciph[5*i];
        cipherText[1] = Ciph[5*i+1];
        cipherText[2] = Ciph[5*i+2];
        cipherText[3] = Ciph[5*i+3];
        cipherText[4] = Ciph[5*i+4];
        genPlainTxt(cipherText, IKey, plainText,check);
        for (int k=0; k < 4; k++){
            resultMatrix[4*i+k] = plainText[k];
        }
        i++;
    }
    unsigned long int end_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "This data has been stored.\n";
    std::cout << DELIM <<" \n\n Finished in " << float(end_time - start_time) << " microseconds with ";
    std::cout << ERR_COUNT << " errors (" << ERR_FIXED << " errors fixed). \n";
    std::cout << "WTF is happening???";
}

void displayStoredData(){
    int i = 0;
    std::cout << "Displaying info about resultMatrix: \n";
    while (not(resultMatrix[i] == 0 and resultMatrix[i+1]==0)){
        //std::cout << resultMatrix[i] << ",";
        i++;
    }
    std::cout << "Length: " << i << " \n First 100 terms:";
    i = 0;
    while (not(resultMatrix[i] == 0 and resultMatrix[i+1]==0) and i<100){
        std::cout << resultMatrix[i] << ",";
        i++;
    }
}

void printM(int64_t Matrix0[4]){
    std::cout << Matrix0[0] << "  " << Matrix0[1] << "\n";
    std::cout << Matrix0[2] << "  " << Matrix0[3] << "\n\n";
}

void matrixMult(const int64_t Matrix0[4],const int64_t Matrix1[4], int64_t returnMatrix[4]) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            returnMatrix[2 * i + j] = Matrix0[2 * i] * Matrix1[j] + Matrix0[2 * i + 1] * Matrix1[2 + j];
        }
    }
}

int64_t det(int64_t Matrix0[4]){
    return Matrix0[0]*Matrix0[3]-Matrix0[1]*Matrix0[2];
}

void matrixInv( int64_t Matrix0[4], int64_t returnMatrix[4]){
    int64_t determinant = det(Matrix0);
    returnMatrix[0] =  Matrix0[3] * determinant;
    returnMatrix[1] = - Matrix0[1] * determinant;
    returnMatrix[2] = - Matrix0[2] * determinant;
    returnMatrix[3] =  Matrix0[0] * determinant;
}

void QMpower(int exp, int64_t returnMatrix[4]) {
    int64_t M1[4] = {1, 1, 1, 0};
    for (int a = 0; a < exp-1; a++) {
        matrixMult(M1, QMatrix,returnMatrix);
        M1[0] = returnMatrix[0];
        M1[1] = returnMatrix[1];
        M1[2] = returnMatrix[2];
        M1[3] = returnMatrix[3];
    }
}

void genCipTxt(int64_t Plain[4], int64_t key[4], int64_t returnMatrix[5]){
    int64_t tempList[4];
    matrixMult(key,Plain,tempList);
    returnMatrix[0] = tempList[0];
    returnMatrix[1] = tempList[1];
    returnMatrix[2] = tempList[2];
    returnMatrix[3] = tempList[3];
    returnMatrix[4] = det(tempList);
}

void genPlainTxt(int64_t Cipher[5], int64_t key[4], int64_t returnMatrix[4], int check){
    int64_t correctedCipher[5];
    if(check==1 and errorcheck(Cipher)) {
        errorCorrect(Cipher, correctedCipher);
    }

    int64_t C[4] = {Cipher[0],Cipher[1],Cipher[2],Cipher[3]};
    int64_t tempList[4];
    matrixMult(key,C,tempList);
    returnMatrix[0] = tempList[0];
    returnMatrix[1] = tempList[1];
    returnMatrix[2] = tempList[2];
    returnMatrix[3] = tempList[3];
}

void copyintoMatrix(std::string text, int64_t returnMatrix[MSIZE]){
    int length = text.length(), i=0,j=0;
    std::string temp_value = "";

    for(i=0;i<=length;i++){
        if(i>MSIZE){
            std::cout << "ERROR: MAX LENGTH EXCEEDED [copyintoMatrix()]";
            return;
        }
        if(isdigit(text[i]) or text[i]=='-'){
            temp_value += text[i];
        } else if (text[i] == '\n') {
            temp_value=temp_value;
        }else if( temp_value != ""){
            returnMatrix[j] = std::stoi(temp_value);
            temp_value = "";
            j+= 1;
        }
    }
}

bool errorcheck(int64_t Matrix0[5]){
    int64_t Matrix1[4] = {Matrix0[0],Matrix0[1],Matrix0[2],Matrix0[3]};                                //det() only takes matrix[4] not matrix[5]
    if(det(Matrix1) == Matrix0[4]) {
        return 0;                                                                                                     //will have to be a very (un)lucky error if det matches!
    }
    std::cout << " \n Error detected! actual determinant is " << det(Matrix1) << " compared to " << Matrix0[4] << ". \n";
    std::cout << "Matrix with error: \n";
    printM(Matrix1);
    ERR_COUNT ++;
    return 1;
}

void errorCorrect(int64_t Matrix0[5],int64_t returnMatrix[4]){
    ERR_FIXED ++;
    int64_t determinant = Matrix0[4];
    returnMatrix[0] =  Matrix0[0]; returnMatrix[1] =  Matrix0[1];
    returnMatrix[2] =  Matrix0[2]; returnMatrix[3] =  Matrix0[3];
    float val1 = float(Matrix0[0])/float(Matrix0[1]),       val2 = float(Matrix0[2])/float(Matrix0[3]);                   //ratio of top and bottom rows
    float diff[2] = {std::abs(val1-G_R),std::abs(val2-G_R)};
    float acc = 0.01;                                                                                                     //sensitivity of error checking
    std::cout << "\n\nError checking... val 1: " << val1 << ", val 2:" << val2<< ".\n";

    if ((diff[0]>acc) and (diff[1]<acc)){                                        //row 1
        std::cout << "Error in row 1.\n";
        returnMatrix[0] = (Matrix0[4] + Matrix0[1]*Matrix0[2])/Matrix0[3];
        val1 = float(returnMatrix[0])/float(returnMatrix[1]);
        diff[0] = std::abs(val1-G_R);

        if ( diff[0]<acc) {
            return;
        }
        returnMatrix[0] = Matrix0[0];
        returnMatrix[1] = -(Matrix0[4] - Matrix0[0]*Matrix0[3])/Matrix0[2];
        val1 = float(returnMatrix[0])/float(returnMatrix[1]);
        diff[0] = std::abs(val1-G_R);

        if ( diff[0]>acc) {
            std::cout << "Error in determinant too. Can not be corrected.\n";
            ERR_FIXED --;
        }
    } else if ((diff[0]<acc) and (diff[1]>acc)) {                                      //row 2
        std::cout << "\n Error in row 2.\n";
        returnMatrix[2] = -(Matrix0[4] - Matrix0[0]*Matrix0[3])/Matrix0[1];
        val2 = float(returnMatrix[2])/float(returnMatrix[3]);
        diff[1] = std::abs(val2-G_R);

        if ( diff[1]<acc) {
            return;
        }
        returnMatrix[2] = Matrix0[2];
        returnMatrix[3] = (Matrix0[4] + Matrix0[1]*Matrix0[2])/Matrix0[0];
        val2 = float(returnMatrix[2])/float(returnMatrix[3]);
        diff[1] = std::abs(val2-G_R);
        if ( diff[1]>acc){
            std::cout << "Error in determinant too. Can not be corrected.\n";
            ERR_FIXED --;
        }
    } else if ((diff[0]>acc) and (diff[1]>acc)) {
        std::cout << "Errors in both columns. (Can not be reliably fixed). \n";
        ERR_FIXED --;
    } else {
        std::cout << "Error appears to be in determinant. No correction needed. \n";
    }
}

//************************************************************* EXPERIMENTAL ********************************//
void readFile() {
    std::string filename, entry, entry_par;
    std::string filelist[100];
    memset(filelist,0,sizeof(filelist));
    entry = "";
    filename = ".";
    std::cout << "Here is a list of files in the directory:\n";
    std::string path = ".";

    int i = 0;
    for (const auto& file: std::filesystem::directory_iterator(path)){
        std::filesystem::path outfilename = file.path();
        std::string outfilename_str = outfilename.string();
        std::string dirfilename = outfilename_str.substr(2, outfilename_str.size() - 6);
        if ((outfilename_str.substr(outfilename_str.size()-4) == ".txt") and (dirfilename != "CMakeCache")) {
            std::cout << "   " << dirfilename << "\n";
            filelist[i] = dirfilename;
        }
        i++;
    }
    i=0;
    int j=0;
    while (j==0) {
        std::cout << "Enter your filename from the directory: \n";
        std::cin >> filename;
        for(i=0;i<100;i++){
            if(filelist[i]==filename){
                std::cout << "File found! \n";
                j=1;
                break;
            }
        }
        i=0;
    }

    try {
        std::ifstream MyReadFile(filename + ".txt");
        i = 0;
        while (getline(MyReadFile, entry_par,'$')) {
            if (i == 0 and entry_par != "1618") {
                std::cout << "Incompatible file! Please choose another file. \n";
                return;
            } else if ( i != 0){
                entry += entry_par + DELIM;
            } else {
                std::cout << "Valid file.\n";
            }
            i++;
        }
        copyintoMatrix(entry,resultMatrix);
        MyReadFile.close();
        std::cout << "Data successfully saved into program!\n";
        return;
    }
    catch(...) {
        std::cout << "Error with file. i = " << i <<"\n";
    }
}

void writeFile(){
    std::string filename = ".";
    std::string path = ".";
    std::string filelist[100];
    memset(filelist,0,sizeof(filelist));

    std::cout << "Here are the text files that aleady exist: \n";
    int i = 0;
    for (const auto& file: std::filesystem::directory_iterator(path)){
        std::filesystem::path outfilename = file.path();
        std::string outfilename_str = outfilename.string();
        std::string dirfilename = outfilename_str.substr(2, outfilename_str.size() - 6);
        if ((outfilename_str.substr(outfilename_str.size()-4) == ".txt") and (dirfilename != "CMakeCache")) {
            std::cout << "   " << dirfilename << "\n";
            filelist[i] = dirfilename;
        }
        i++;
    }
    i=0;
    int j=0;
    while (j==0) {
        std::cout << "Enter your filename (it cannot be a name that already exist): \n";
        std::cin >> filename;
        j=1;
        for(i=0;i<100;i++){
            if(filelist[i]==filename){
                std::cout << "File already exists! Try another name. \n";
                j=0;
                break;
            }
        }
        i=0;
    }

    std::ofstream MyFile(filename +".txt");
    MyFile << "1618$";
    while (not(resultMatrix[i]==0 and resultMatrix[i+1]==0)){
        MyFile << resultMatrix[i] << DELIM;
        i++;
    }
    MyFile << "$";
    MyFile.close();
    std::cout << "File successfully created!";
}


