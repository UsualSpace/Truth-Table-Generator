# Truth-Table-Generator
You can try it out here:
https://onlinegdb.com/lJG5Ig-8j

This program parses a propositional expression and generates a truth table of all possible input combinations and outputs. I made this originally to help me in my discrete structures course in college, however I only completed it after the course was done. 

It works by first performing a lexical analysis (lexing) on a raw user entered string. What that basically means is it breaks up the entered string into meaningful blocks of data that the program will be able to understand in a later step. These blocks of data - called "tokens" - store the raw characters that were identified during the lexing process, a token type (PROPOSITION, TRUTH_VALUE, CONJUNCTION, etc.), and a precedence level (because we will likely be dealing with logical operators, which have to be evaluated in a certain order). 

After all the tokens have been identified and gathered in a list, they are then sorted into a post-fix form (using the shunting-yard algorithm), which essentially moves operators to the right of its operands. For example, take the in-fix expression "1 + 2". The '+' operator is in between the '1' and '2' operands, hence the "in". The post-fix form of this expression is "1 2 +". The reason I do this is to make the overall expression simple to evaluate using the stack data structure, which happens to be the next step. 

For any given post-fix expression, to evaluate it, you first step through the expression, pushing any operands you come across onto the stack. Once you come across an operator however, depending on if it is a unary or binary operator, you pop off 1 or 2 operands from the stack, evaluate them using said operator, then push the result of the evaluation back onto the stack. When you repeat this process until the end of the expression, you should be left with only one operand in the stack (assuming the expression was valid in the first place). This lone operand will be the final answer of your evaluated expression.

In my case, depending on how many propositional variables are present, I need to run the evaluation step multiple times, each time with a different set of values, in order to generate all the possible outcomes of a given propositional expression. I've added comments explaining a few things, I hope to add more features and possibly refine the code.

Currently I support these basic operations:
NEGATION (logical NOT)
CONJUNCTION (logical AND)
DISJUNCTION (logical OR)
IMPLICATION (<=)
BICONDITIONAL (==)

Some example propositional expressions you can try entering:

p ^ q

~(p ^ q)

p -> q

(p -> q) ^ ~q -> ~p

p ^ T

(m ^ k ^ o ^ F) v i

I also added support for boolean algebra:

(1 * 0) + 1

~0 + ~(1 * 1 * 0)


     
