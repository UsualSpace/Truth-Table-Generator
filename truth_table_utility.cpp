/*
 * Filename: truth_table_utility.cpp
 * Programmer: Abdurrahman Alyajouri
 * Initial Creation Date: 1/12/2025
 * Purpose: The purpose of this file is to implement the functions
 *          and structures declared in truth_table_utility.h
 */

#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <stack>

using std::cout;
using std::endl;
using std::stack;
using std::unordered_map;

#include "truth_table_utility.h"
#include "chronological_map.h"

Token::Token() : type(TokenType::NULLTOKEN), lexeme("") {}
Token::Token(TokenType a_type, char a_lexeme) : type(a_type), lexeme(1, a_lexeme) {}
Token::Token(TokenType a_type, const string& a_lexeme) : type(a_type), lexeme(a_lexeme) {}

ostream& operator<<(ostream& my_out, TokenType a_type) {
    switch(a_type) {
        case TokenType::TRUE:
            my_out << "TRUE";
            break;
        case TokenType::FALSE: 
            my_out << "FALSE";
            break;
        case TokenType::LPAREN: 
            my_out << "LPAREN";
            break;
        case TokenType::RPAREN: 
            my_out << "RPAREN";
            break;
        case TokenType::VARIABLE:  
            my_out << "VARIABLE";
            break;
        case TokenType::NEGATION: 
            my_out << "NEGATION";
            break;
        case TokenType::NULLTOKEN:
            my_out << "NULL";
            break;
        case TokenType::CONJUNCTION:
            my_out << "CONJUNCTION";
            break;
        case TokenType::DISJUNCTION: 
            my_out << "DISJUNCTION";
            break;
        case TokenType::IMPLICATION:
            my_out << "IMPLICATION";
            break;
        case TokenType::BICONDITIONAL:
            my_out << "BICONDITIONAL";
            break;
        default:
            break;
    }
    
    return my_out;
}

bool IsOperator(const Token& a_token) {
    switch(a_token.type) {
        case TokenType::NEGATION:
        case TokenType::CONJUNCTION:
        case TokenType::DISJUNCTION:
        case TokenType::IMPLICATION:
        case TokenType::BICONDITIONAL:
            return true;
        default:
            return false;
    }
}

void Tokenize(const string& the_expression, vector<vector<Token>>& the_tokens, chrono_map& variable_tracker) {
    if(the_expression.empty()) return;
    
    //Number of expressions we have encountered (separated by '|').
    size_t expression_counter = 1;
    //The current position in the expression string we are at.
    size_t current_position = 0; 
    
    //Create a default token, to be modified by below loop.
    Token new_token;
    
    //Step through the expression string, searching for tokens.
    while(current_position < the_expression.size()) {
        char current_char = the_expression.at(current_position);
        switch(current_char) {
            //Whitespaces result in null tokens.
            case ' ':
                new_token = Token(); //Re-init to a null token via default ctor.
                break;
            //Truth constants.
            case 'T':
            case '1':
                new_token = Token(TokenType::TRUE, current_char);
                break;
            case 'F':
            case '0':
                new_token = Token(TokenType::FALSE, current_char);
                break;
            //Negation operator.
            case '~':
            case '!':
                new_token = Token(TokenType::NEGATION, current_char);
                break;
            //Conjunction operator.
            case '^':
            case '*':
                new_token = Token(TokenType::CONJUNCTION, current_char);
                break;
            //Disjunction operator.
            case 'v':
            case '+':
                new_token = Token(TokenType::DISJUNCTION, current_char);
                break;
            //Implication operator.
            case '-':
                if(the_expression.at(current_position + 1) == '>') {
                    new_token = Token(TokenType::IMPLICATION, "->");
                    current_position += 1;
                }
                break;
            //Biconditional operator (<-> | <=>).
            case '<': 
                {
                    char second = the_expression.at(current_position + 1);
                    char third = the_expression.at(current_position + 2);
                    if(second == '-' || second == '=')
                        if(third == '>') {
                            //A bit messy.
                            new_token = Token(TokenType::BICONDITIONAL, "<" + string(1, second) + ">");
                            current_position += 2;
                        }
                }
                break;
            //Parentheses.
            case '(':
                new_token = Token(TokenType::LPAREN, current_char);
                break;
            case ')':
                new_token = Token(TokenType::RPAREN, current_char);
                break;
            //Alternation check.
            case '|':
                expression_counter += 1;
                break;
            //Anything else is recognized as a variable.
            default:
                new_token = Token(TokenType::VARIABLE, current_char);
                
                //By default, variables will be initialized to false.
                variable_tracker[current_char] = false; 
                break;
        }  
        
        //Finally push the token, if not null, into the list of token
        //expressions (I feel like there is an edge case here I am not seeing).
        if(new_token.type != TokenType::NULLTOKEN) {
            if(expression_counter > the_tokens.size())
                the_tokens.push_back({new_token});
            else
                the_tokens.at(expression_counter - 1).push_back(new_token);
        }
        
        //Move on.
        current_position += 1;
    }
}

vector<Token> ToPostFix(const vector<Token>& the_tokens) {
    vector<Token> output; //Final postfix token expression is stored here.
    stack<Token> operators; //To hold the operators in the expression.
    
    //Do Shunting yard algorithm.
    for(const Token& token : the_tokens) {
        if(IsOperator(token)) {
            while(!operators.empty() && operators.top().type != TokenType::LPAREN && PRECEDENCE_MAP.at(operators.top().type) >= PRECEDENCE_MAP.at(token.type)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        } else if(token.type == TokenType::LPAREN) {
            operators.push(token);
        } else if(token.type == TokenType::RPAREN) {
            while(operators.top().type != TokenType::LPAREN) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
        } else {
            output.push_back(token);
        }
    }
    
    while(!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }
    
    return output;
}

void Evaluate(const string& the_expression) {
    //Stores tokens after tokenizing.
    vector<vector<Token>> the_tokens; 
    //Tracks variables during tokenization.
    chrono_map variable_tracker; 
    
    //Tokenize the expression.
    Tokenize(the_expression, the_tokens, variable_tracker);
    
    //Convert to post-fix.
    for(auto& expression : the_tokens)
        expression = ToPostFix(expression);
    
    //Stores operands we encounter during evaluation.
    stack<bool> operands;
    
    //Quick compact functions for evaluating binary/unary expressions,
    //to prevent writing alot of code.
    auto SolveBinary = [&operands](bool(*Operation)(bool left, bool right)) {
        bool right = operands.top();
        operands.pop();
        bool left = operands.top();
        operands.pop();
        return Operation(left, right);
    };
    
    auto SolveUnary = [&operands](bool(*Operation)(bool operand)) {
        bool operand = operands.top();
        operands.pop();
        return Operation(operand);
    };
    
    //First print the truth table header - Variables, then expressions.
    //Variables:
    for(const auto& variable : variable_tracker) 
        cout << variable.first << ' ';
    
    //Then expressions:
    cout << '\t' << the_expression << endl << endl;
    
    //True and false character representations, to be indexed with booleans.
    char TV[] = {'F', 'T'};
    
    //For every row of the truth table (2^num_vars amount of rows)...
    for(size_t rows = 0; rows < (1 << variable_tracker.size()); ++rows) {
        //Set the values of every variable before evaluation. We start at
        //all true and go down to all false.
        size_t j = variable_tracker.size() - 1;
        for(auto& variable : variable_tracker) {
            variable.second = !((rows >> j--) & 1);
            cout << TV[variable.second] << ' ';
        }
        
        //Since all variable values are set, evaluate every sub-expression.
        for(const auto& expression : the_tokens) {
            for(const Token& token : expression) {
                switch(token.type) {
                    case TokenType::TRUE:
                        operands.push(true); 
                        break;
                    case TokenType::FALSE:
                        operands.push(false);
                        break;
                    case TokenType::VARIABLE:
                        //Variables are supposed to be 1 character, so check 
                        //index 0 of the lexeme string.
                        operands.push(variable_tracker[token.lexeme.at(0)]); 
                        break;
                    case TokenType::NEGATION:
                        operands.push(SolveUnary([](bool right){return !right;}));
                        break;
                    case TokenType::CONJUNCTION:
                        operands.push(SolveBinary([](bool left, bool right){return left && right;}));
                        break;
                    case TokenType::DISJUNCTION:
                        operands.push(SolveBinary([](bool left, bool right){return left || right;}));
                        break;
                    case TokenType::IMPLICATION:
                        operands.push(SolveBinary([](bool left, bool right){return left <= right;}));
                        break;
                    case TokenType::BICONDITIONAL:
                        operands.push(SolveBinary([](bool left, bool right){return left == right;}));
                        break;
                    default:break;
                }
            }
            
            //If the operands stack is a size other than 1, something went wrong.
            if(operands.size() != 1) throw std::invalid_argument("Invalid expression");
            
            //Need to work on formatting.
            cout << '\t' << std::setw((expression.size() + 6)/2) << TV[operands.top()] << ' ';
            operands.pop();
        }
        cout << endl;
    }
    
    return;
}
