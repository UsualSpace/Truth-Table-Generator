/*
    Program: truth_table_generator.cpp
    Programmer: Abdurrahman Alyajouri
    Purpose: Generates a truth table based on a user entered
             propositional statement. It supports propositional variables,
             constant truth values, boolean algebra, and proper operator 
             precedence as well as usage of parentheses. 
             Available operations and their typed representations:
             - TRUTH_VALUE:     0, 1, F, T.
             - PROPOSITION:     p, q, r, etc. (any letter or other unused character you wish to use).
             - NEGATION:        !, ~.
             - CONJUNCTION:     ^, *.
             - DISJUNCTION:     v, +.
             - IMPLICATION:     ->.
             - BICONDITIONAL:   <->
             
             NOTE: Type "quit" to exit the program.
    
    Date: 7/11/2024
    Written in C++17.        
*/


#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <iomanip>

class Token {
    public:
        enum Type {
            TRUTH_VALUE = 0,
            PROPOSITION,
            NEGATION,
            CONJUNCTION,
            DISJUNCTION,
            IMPLICATION,
            BICONDITIONAL,
            LPAREN,
            RPAREN
        };
        enum Precedence {
            L1, L2, L3, L4, L5, NA
        };

        Token() {
            _type = TRUTH_VALUE;
            _precedence = NA;
            _value = false;
            _lexeme = "null";
        }

        Token(Type type, Precedence precedence, bool value, std::string lexeme) {
            _type = type;
            _precedence = precedence;
            _value = value;
            _lexeme = lexeme;
        }
        const Type type() const {return _type;}
        const int precedence() const {return _precedence;}
        const bool value() const {return _value;}
        const std::string lexeme() const {return _lexeme;}
        
        const bool isOperator() const {
            switch(_type) {
                case NEGATION:
                case CONJUNCTION:
                case DISJUNCTION:
                case IMPLICATION:
                case BICONDITIONAL: 
                    return true;
                default: return false;
            }
        }

        // Will not do anything if the token does not represent propositional variable.
        void setValue(bool value) {
            if(_type == PROPOSITION)
                _value = value;
        }
    private:
        Type _type;
        Precedence _precedence;
        bool _value;
        std::string _lexeme;
};

// inspired by https://craftinginterpreters.com/scanning.html
class Lexer {
    public:
        Lexer(std::string source) {
            _source = source;
            scanTokens();
        }

        const std::vector<Token> getTokens() const {
            return _tokens;
        }

        const std::map<std::string, Token> getPropositionTokens() const {
            return _proposition_tokens;
        }

        const void Print() const {
            for(int i{}; i < _tokens.size(); ++i) {
                std::cout << i + 1 << ". Type: " << _tokens[i].type() << ", Lexeme: " + _tokens[i].lexeme() << std::endl;
            }
        }

    private:

        void scanToken() {
            switch(_source[_current_position]) {
                // =========================================================
                // Ignore Whitespace
                case ' ':
                    break;
                // =========================================================
                // Truth values
                case '0':
                case 'F':
                    addToken(Token::Type::TRUTH_VALUE, false);
                    break;
                case '1':
                case 'T':
                    addToken(Token::Type::TRUTH_VALUE, true);
                    break;
                // =========================================================
                // Parentheses
                case '(':
                    addToken(Token::Type::LPAREN, Token::Precedence::NA);
                    break;
                case ')':
                    addToken(Token::Type::RPAREN, Token::Precedence::NA);
                    break;
                // =========================================================
                // Single-char operators
                case '^':
                case '*':
                    addToken(Token::Type::CONJUNCTION, Token::Precedence::L2);
                    break;
                case 'v':
                case '+':
                    addToken(Token::Type::DISJUNCTION, Token::Precedence::L3);
                    break;
                case '!':
                case '~':
                    addToken(Token::Type::NEGATION, Token::Precedence::L1);
                    break;
                // =========================================================
                // Multi-char operators
                case '-':
                    if(matchNext('>')) addToken(Token::Type::IMPLICATION, Token::Precedence::L4);
                    break;
                case '<':
                    if(matchNext('-') && matchNext('>')) addToken(Token::Type::BICONDITIONAL, Token::Precedence::L5);
                    break;
                // =========================================================
                // Propositional Variables
                default:
                    addToken(Token::Type::PROPOSITION);
                    _proposition_tokens.insert({getCurrentLexeme(), Token(Token::Type::PROPOSITION, Token::Precedence::NA, false, getCurrentLexeme())});
                    break;
                // =========================================================
            }
            ++_current_position;
        }

        void addToken(Token::Type type) {
            _tokens.push_back(Token(type, Token::Precedence::NA, false, getCurrentLexeme()));
        }

        void addToken(Token::Type type, Token::Precedence precedence) {
            _tokens.push_back(Token(type, precedence, false, getCurrentLexeme()));
        }

        void addToken(Token::Type type, bool value) {
            _tokens.push_back(Token(type, Token::Precedence::NA, value, getCurrentLexeme()));
        }

        void scanTokens() {
            while(!endReached()) {
                _start_position = _current_position;
                scanToken();
            }
        }

        std::string getCurrentLexeme() {
            std::string result = "";
            for(int i{_start_position}; i <= _current_position; ++i) {result += _source[i];}
            return result;
        }

        bool matchNext(char c) {
            ++_current_position;
            if(endReached()) return false;
            if(_source[_current_position] != c) return false;
            return true;
        }

        const bool endReached() const {
            return _current_position >= _source.size();
        }

        std::vector<Token> _tokens;
        std::map<std::string, Token> _proposition_tokens;
        std::string _source;
        int _start_position{};
        int _current_position{};
};

// I wish this was more elegant. Maybe regular expressions could work here?
bool validateTokenString(const std::vector<Token>& tokens) {
    bool flag = true;
    for(int i{}; i < tokens.size(); ++i) {
        switch(tokens[i].type()) {
            case Token::TRUTH_VALUE:
            case Token::PROPOSITION:
                if(!(   tokens.size() == 1 ||
                        (tokens[i + 1].isOperator() && !(tokens[i + 1].type() == Token::NEGATION)) || 
                        tokens[i - 1].isOperator() || 
                        tokens[i - 1].type() == Token::LPAREN)) flag = false;   
                break;
            case Token::NEGATION:
                if(!(   tokens[i + 1].type() == Token::TRUTH_VALUE || 
                        tokens[i + 1].type() == Token::PROPOSITION ||
                        tokens[i + 1].type() == Token::LPAREN || 
                        tokens[i + 1].type() == Token::NEGATION ||
                        tokens[i - 1].type() == Token::LPAREN ||
                        tokens[i - 1].isOperator() && !(tokens[i + 1].type() == Token::NEGATION))) flag = false;
                break;
            case Token::CONJUNCTION:
            case Token::DISJUNCTION:
            case Token::IMPLICATION:
            case Token::BICONDITIONAL:
                if(!(   tokens[i + 1].type() == Token::TRUTH_VALUE || 
                        tokens[i + 1].type() == Token::PROPOSITION ||
                        tokens[i + 1].type() == Token::LPAREN ||  
                        tokens[i + 1].type() == Token::NEGATION ||
                        tokens[i - 1].type() == Token::TRUTH_VALUE || 
                        tokens[i - 1].type() == Token::PROPOSITION ||
                        tokens[i - 1].type() == Token::RPAREN
                        )) flag = false;
                break;
            default:break;
        }
    }
    return flag;
}

// Shunting yard algorithm is used here.
std::vector<Token> toPostFix(const std::vector<Token>& tokens) {
    std::stack<Token> operators;
    std::vector<Token> output;

    for(const Token& t : tokens) {
        if(t.isOperator()) {
            while(!operators.empty() && t.precedence() > operators.top().precedence()) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(t);
        } else if(t.type() == Token::LPAREN) {
            operators.push(t);
        } else if(t.type() == Token::RPAREN) {
            while(operators.top().type() != Token::LPAREN) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
        } else {
            output.push_back(t);
        }
    }
    while(!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }
    return output;
}

void evaluate(std::string expression) {
    // No need to do anything if the expression is empty.
    if(expression.empty()) return;
    
    // Lexer lexes expression upon construction.
    Lexer lexer(expression);
    
    // Tracks all expression tokens, regardless of type.
    std::vector<Token> tokens = lexer.getTokens();

    // Messy validation, unsure if it covers all cases, to be improved in the
    // future.
    if(!validateTokenString(tokens)) {
        std::cout << "Invalid expression!\n";
        return;
    }

    // Tracks propositional variables so we can set each appearance of the same 
    // variable with its respective value.
    std::map<std::string, Token> propositions = lexer.getPropositionTokens();
    
    std::stack<bool> operands; // Stores operands during post-fix evaluation.
    
    // Just a container for True(T) or False(F) labels, 'F' is stored at index 0
    // and 'T' at index 1 for convenient use with a boolean.
    char TV[] = {'F', 'T'};

    // Solves a binary expression using the operands stack.
    auto solveBinary = [&operands](bool(*func)(bool left, bool right)) {
        bool right = operands.top();
        operands.pop();
        bool left = operands.top();
        operands.pop();
        return func(left, right);
    };
    
    // Solves a unary expression using the operands stack, 
    // for now only used for the right side of a NOT operation.
    auto solveUnary = [&operands](bool(*func)(bool right)) {
        bool right = operands.top();
        operands.pop();
        return func(right);
    };

    // Printing table headers.
    for(auto const& x : propositions) {
        std::cout << x.first << ' ';
    }
    std::cout << '\t' << expression << std::endl << std::endl;

    // Evaluation loop, runs 2 ^ (number of propositions) times in order to 
    // calculate every possible set of truth values in a given expression.
    for(int i{}; i < (1 << propositions.size()); ++i) {
        // This next loop takes every propositional variable if any, and gives
        // it a truth value.
        int j = propositions.size() - 1;
        for(auto & x : propositions) {
            x.second.setValue(!(1 == ((i >> j--) & 1)));
            std::cout << TV[x.second.value()] << ' ';
        }
        // This next loop evaluates a post-fix expression using another stack.
        for(const Token& t : toPostFix(tokens)) {
            switch(t.type()) {
                case Token::TRUTH_VALUE:
                    operands.push(t.value()); 
                    break;
                case Token::PROPOSITION:
                    operands.push(propositions[t.lexeme()].value()); 
                    break;
                case Token::NEGATION:
                    operands.push(solveUnary([](bool right){return !right;}));
                    break;
                case Token::CONJUNCTION:
                    operands.push(solveBinary([](bool left, bool right){return left && right;}));
                    break;
                case Token::DISJUNCTION:
                    operands.push(solveBinary([](bool left, bool right){return left || right;}));
                    break;
                case Token::IMPLICATION:
                    operands.push(solveBinary([](bool left, bool right){return left <= right;}));
                    break;
                case Token::BICONDITIONAL:
                    operands.push(solveBinary([](bool left, bool right){return left == right;}));
                    break;
                default:break;
            }
        }
        // Formatting.
        std::cout << '\t' << std::setw((expression.size() + 1)/2) << TV[operands.top()] << std::endl;
        operands.pop();
    }
    std::cout << std::endl;
} 

int main() {
    std::string answer = "";
    
    //Run loop, entering "quit" will stop the loop.
    while(answer != "quit") {
        evaluate(answer);
        std::cout << "Enter proposition: ";
        getline(std::cin, answer);
    }
    return 0;
}
