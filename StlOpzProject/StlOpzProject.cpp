#include <iostream>
#include "MathExpression.h"

int main()
{
    MathExpression expr("(5 + 6) * 7 + sin(1 + 1)");
    expr["x"] = 2;
    //int p = expr.CheckBrackets();

    expr.OpzCreate();
    cout << expr.ExpressionOpz() << "\n";
    cout << expr.Expression() << " = " << expr.OpzCalculate() << "\n";
    
    
}
