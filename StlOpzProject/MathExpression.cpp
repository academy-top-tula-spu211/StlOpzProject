#include "MathExpression.h"

double& MathExpression::operator[](string varName)
{
    if (!variables.contains(varName))
        variables.emplace(varName, 0.0);
    return variables[varName];
}

double& MathExpression::operator[](const char* varName)
{
    if (!variables.contains(varName))
        variables.emplace(varName, 0.0);
    return variables[varName];
}

int MathExpression::CheckBrackets()
{
    string brackets{ bracketsOpen + bracketsClose };
    stack<char> bracketsStack;

    for (int position{}; position < expression.length(); position++)
    {
        // not brackets
        if (brackets.find(expression[position]) == string::npos)
            continue;

        // if open bracket
        if (bracketsOpen.find(expression[position]) != string::npos)
        {
            bracketsStack.push(expression[position]);
            continue;
        }

        // if close bracket
        if (bracketsStack.empty())
            return position;

        int typeClose = bracketsClose.find(expression[position]);
        int typeOpen = bracketsOpen.find(bracketsStack.top());
        if (typeClose != typeOpen)
            return position;

        bracketsStack.pop();
    }

    return ((bracketsStack.empty()) ? -1 : expression.length());
}

void MathExpression::OpzCreate()
{
    stack<char> operatorsStack;
    
    string fvals{ "" };
    string fkeys{ "" };
    for (auto f : functions)
    {
        fkeys += f.first + "#";
        fvals += f.second + "#";
    }

    for (int position{}; position < expression.length(); position++)
    {
        char symbol = expression[position];
        
        // white spaces
        if (symbol == ' ' || symbol == '\t')
            continue;

        // opening bracket
        if (bracketsOpen.find(symbol) != string::npos)
        {
            operatorsStack.push(symbol);
            continue;
        }

        // closing brackets
        if (bracketsClose.find(symbol) != string::npos)
        {
            while (!operatorsStack.empty() &&
                bracketsOpen.find(operatorsStack.top()) == string::npos)
            {
                expressionOpz += operatorsStack.top();
                operatorsStack.pop();
            }
            operatorsStack.pop();
            continue;
        }

        // multiplex operators
        if (operatorsMultiplex.find(symbol) != string::npos)
        {
            while (!operatorsStack.empty() &&
                operatorsMultiplex.find(operatorsStack.top()) != string::npos)
            {
                expressionOpz += operatorsStack.top();
                operatorsStack.pop();
            }
            operatorsStack.push(symbol);
        }

        float x = .5e2;

        // additive operators
        if (operatorsAdditive.find(symbol) != string::npos)
        {
            while (!operatorsStack.empty()
                &&
                (operatorsMultiplex.find(operatorsStack.top()) != string::npos
                || operatorsAdditive.find(operatorsStack.top()) != string::npos))
            {
                expressionOpz += operatorsStack.top();
                operatorsStack.pop();
            }
            operatorsStack.push(symbol);
        }

        // numbers
        if (isdigit(symbol) || symbol == '.')
        {
            string number;
            while ((isdigit(symbol) ||
                symbol == '.' ||
                tolower(symbol) == 'e') &&
                position < expression.length())
            {
                if (tolower(symbol) == 'e' &&
                    (expression[position + 1] == '-' ||
                     expression[position + 1] == '+'))
                {
                    number += "e-";
                    position += 2;
                    symbol = expression[position];
                }
                else
                {
                    number += symbol;
                    symbol = expression[++position];
                }
            }
            number += "#";
            expressionOpz += number;
            position--;
            continue;
        }

        // variables and functions
        if (isalpha(symbol) || symbol == '_')
        {
            string name{ "" };
            while ((isalnum(symbol) ||
                symbol == '_') &&
                position < expression.length())
            {
                name += symbol;
                symbol = expression[++position];
            }

            // if name is function
            if (fkeys.find(name) != string::npos)
                operatorsStack.push(functions[name]);
            else
                expressionOpz += to_string(variables[name]) + "#";
            position--;
            continue;
        }
    }

    while (!operatorsStack.empty())
    {
        expressionOpz += operatorsStack.top();
        operatorsStack.pop();
    }
}

double MathExpression::OpzCalculate()
{
    stack<double> operandsStack;

    string fvals{ "" };
    for (auto f : functions)
        fvals += f.second;

    int position{};
    while (position < expressionOpz.length())
    {
        char symbol{ expressionOpz[position] };
        
        // numbers
        if (isdigit(symbol) || symbol == '.')
        {
            string operand{ "" };
            while (symbol != '#')
            {
                operand += symbol;
                symbol = expressionOpz[++position];
            }
            operandsStack.push(stod(operand));
            position++;
            continue;
        }

        // operators
        if (operatorsMultiplex.find(symbol) != string::npos ||
            operatorsAdditive.find(symbol) != string::npos)
        {
            double operandTwo = operandsStack.top();
            operandsStack.pop();
            double operandOne = operandsStack.top();
            operandsStack.pop();

            double result;
            switch (symbol)
            {
            case '+': result = operandOne + operandTwo; break;
            case '-': result = operandOne - operandTwo; break;
            case '*': result = operandOne * operandTwo; break;
            case '/': result = operandOne / operandTwo; break;
            default:
                break;
            }
            operandsStack.push(result);
            position++;
            continue;
        }

        //functions
        if (fvals.find(symbol) != string::npos)
        {
            double operand = operandsStack.top();
            operandsStack.pop();

            switch (symbol)
            {
            case 's': operandsStack.push(sin(operand)); break;
            case 'c': operandsStack.push(cos(operand)); break;
            case 't': operandsStack.push(tan(operand)); break;
            case 'g': operandsStack.push(1 / tan(operand)); break;
            default:
                break;
            }
            position++;
            continue;
        }
    }
    return operandsStack.top();
}
