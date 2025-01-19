#pragma once

/*
 * Filename: truth_table_utility.h
 * Programmer: Abdurrahman Alyajouri
 * Initial Creation Date: 1/12/2025
 * Purpose: The purpose of this file is to define multiple structures
 *          and declare function prototypes meant to assist in generating
 *          a truth table from a propositional expression.
 */

#include <unordered_map>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::ostream;
using std::unordered_map;

#include "chronological_map.h"

enum class TokenType {
    TRUE,           //'T', '1'
    FALSE,          //'F', '0'
    LPAREN,         //'('
    RPAREN,         //')'
    VARIABLE,       //'p', 'q', etc.
    NEGATION,       //'~', '!' (Logical NOT)
    NULLTOKEN,      //Null, empty token.
    CONJUNCTION,    //'^', '*' (Logical AND)
    DISJUNCTION,    //'v', '+' (Logical OR)
    IMPLICATION,    //"->"  
    BICONDITIONAL   //"<->", "<=>"
};

const unordered_map<TokenType, unsigned int> PRECEDENCE_MAP = {
    {TokenType::NEGATION, 5},
    {TokenType::CONJUNCTION, 4},
    {TokenType::DISJUNCTION, 3},
    {TokenType::IMPLICATION, 2},
    {TokenType::BICONDITIONAL, 1}
};

struct Token {
    //Constructors for convenience.
    Token();
    Token(TokenType a_type, char a_lexeme);
    Token(TokenType a_type, const string& a_lexeme);
    
    TokenType type;
    string lexeme; //String representation of token.
};

//Overload for printing token types to stdout.
ostream& operator<<(ostream& my_out, TokenType a_type);

//Checks if a token represents an operator.
bool IsOperator(const Token& a_token);

//Identifies tokens from "the_expression" and pushes them
//into "the_tokens", a vector allowing multiple token expressions.
//Also keeps tracks of variables for later evaluation purposes.
void Tokenize(const string& the_expression, vector<vector<Token>>& the_tokens, chrono_map& variable_tracker);

//Creates and returns a post-fix version of the input string of tokens.
vector<Token> ToPostFix(const vector<Token>& the_tokens);

//Evaluates a propositional expression, or a boolean algebra expression.
//Expressions may consist of multiple sub-expressions, split via the 
//'|' character. Will throw an exception if the expression is invalid.
void Evaluate(const string& the_expression);
