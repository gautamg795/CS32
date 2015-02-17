//
//  eval.cpp
//  Problem5
//
//  Created by Gautam Gupta on 2/1/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;
int opPrecedence(char c);
bool checkSyntax(string infix);
int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    int EVAL_SUCCESS = 0, EVAL_FAILURE = 1;
    if (!checkSyntax(infix))
        return EVAL_FAILURE;
    postfix = "";
    stack<char> operatorStack;
    for (size_t i = 0; i < infix.size(); i++)
    {
        char ch = infix[i];
        if (isdigit(ch))
            postfix += ch;
        else if (ch == '(' || ch == '!')
            operatorStack.push(ch);
        else if (ch == ')')
        {
            while (operatorStack.top() != '(')
            {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.pop();
        }
        else if (ch == '|' || ch == '&')
        {
            while (!operatorStack.empty() && operatorStack.top() != '(' && opPrecedence(ch) <= opPrecedence(operatorStack.top()))
            {
                postfix += operatorStack.top();
                operatorStack.pop();
            }
            operatorStack.push(ch);
        }
    }
    while (!operatorStack.empty())
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    stack<bool> operandStack;
    for (size_t i = 0; i < postfix.size(); i++)
    {
        char ch = postfix[i];
        if (isdigit(ch))
        {
            int pos = ch - '0';
            operandStack.push(values[pos]);
        }
        else
        {
            bool operand2 = operandStack.top();
            operandStack.pop();
            if (ch == '!')
            {
                operandStack.push(!operand2);
                continue;
            }
            bool operand1 = operandStack.top();
            operandStack.pop();
            if (ch == '|')
                operandStack.push(operand1 || operand2);
            else if (ch == '&')
                operandStack.push(operand1 && operand2);
        }
    }
    if (operandStack.size() != 1)
        return EVAL_FAILURE;
    result = operandStack.top();
    return EVAL_SUCCESS;
}
bool checkSyntax(string infix) // Returns true if the infix string is syntactically valid
{
    char prevChar = '!';
    int openP = 0;
    int closeP = 0;
    for (size_t i = 0; i < infix.size(); i++)
    {
        char curChar = infix[i];
        if (curChar == ' ')
            continue;
        else if (isdigit(curChar))
        {
            if (isdigit(prevChar) || prevChar == ')')
                return false;
        }
        else if (curChar == '|' || curChar == '&')
        {
            if (!isdigit(prevChar) && prevChar != ')')
                return false;
        }
        else if (curChar == '!')
        {
            if (isdigit(prevChar) || prevChar == ')')
                return false;
        }
        else if (curChar == '(')
            openP++;
        else if (curChar == ')')
            closeP++;
        
        prevChar = curChar;
    }
    if (prevChar != ')' && !isdigit(prevChar)) // Last char should be a digit or a ")"
        return false;
    if (openP != closeP) // Number of "(" should equal number of ")"
        return false;
    return true;
}

int opPrecedence(char c) // Assumes that only an operator will be passed to it
{
    string ops = "(|&!";
    return static_cast<int>(ops.find(c)); // Returns the position in the string—-which so happens to be sorted by precedence
}
int main()
{
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    assert(evaluate(" (0|1)|3", ba, pf, answer) == 0 && answer);
    assert(evaluate("0", ba, pf, answer) == 0 && pf == "0" && answer);
    assert(evaluate("0 & !3", ba, pf, answer) == 0 && pf == "03!&");
    assert(evaluate("((0))", ba, pf, answer) == 0 && pf == "0" && answer);
    // User-created test cases
    assert(evaluate("!(0|9)", ba, pf, answer) == 0 && pf == "09|!" && !answer);
    assert(evaluate("!9|0", ba, pf, answer) == 0 && pf == "9!0|" && answer);
    assert(evaluate("0|9&9", ba, pf, answer) == 0 && pf == "099&|" && answer);
    assert(evaluate("0&!(9|0&0|9)|!!!(9&0&9)", ba, pf, answer) == 0 && pf == "0900&|9|!&90&9&!!!|" && answer);
    assert(evaluate("(0&((9|0)&(0)))", ba, pf, answer) == 0 && pf == "090|0&&" && answer);
    assert(evaluate("!((0&((9|0)&(0))))", ba, pf, answer) == 0 && pf == "090|0&&!" && !answer);
    
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
    cout << "Passed all tests" << endl;
    
}
