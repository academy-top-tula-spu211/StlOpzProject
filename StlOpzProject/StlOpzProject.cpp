#include <iostream>
#include "MathExpression.h"

int main()
{
    MathExpression expr("5 + 6 * 7");
    expr["d"] = 23.5;
    //int p = expr.CheckBrackets();

    expr.OpzCreate();
    cout << expr.ExpressionOpz() << "\n";
    
    
}
