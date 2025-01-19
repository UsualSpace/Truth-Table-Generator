/*
 * Filename: main.cpp
 * Programmer: Abdurrahman Alyajouri
 * Initial Creation Date: 1/12/2025
 * Purpose: The purpose of this file is to showoff the result of the 
 *          truth table Evaluate() function.
 */

#include <iostream>
#include "truth_table_utility.h"

using namespace std;

int main() {
    
    string answer = "";
    while(answer != "quit") {
        cout << "Enter a propositional expression:\n";
        getline(cin, answer);
        cout << endl;
        try {
            Evaluate(answer);   
        } catch(const invalid_argument& exception) {
            cout << exception.what() << ", please try again.\n";
        }
        cout << endl;
    }

    return 0;
}
