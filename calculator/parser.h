#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include "mynumber.h"
#include "mynumber_functions.h"

class Parser
{
public:
    Parser();

    ///////////////////////////////////////////////////////////////////////////////////////
    //Solve math expression with or without variables

    //solves math expressions, i.e: "2+3*cos(pi)"
    MyNumber SolveExpression(const QString &expression);

    //solves math expressions, i.e: "2+3*cos(pi)" and returns a list with size 'size' with the solution
    //this can be usefull for calculating graphs of math functions where the functions is constant
    // i.e "f(x)=4"
    QList<double> SolveExpression_List(const QString &expression, const int &size);


    //solves math expressions with one variable, i.e: expression_fx="2x+4" ; value_x="5" ; variable "x" is autodetected
    MyNumber SolveExpression_fx(const QString &expression_fx, const double &value_x);


    //solves math expressions with one variable, i.e: expression_fx="2x+4" ; value_x="5" ; variable_x ="x"
    MyNumber SolveExpression_fx(const QString &expression_fx, const QString &value_x, const QString &variable_x);

    //convenience function, where value_x is a double instead of a string
    MyNumber SolveExpression_fx(const QString &expression_fx, const double &value_x, const QString &variable_x);

    //convenience function, where value_x is a type Number instead of a string
    //(type Number can be a double, Complexo or Matrix)
    MyNumber SolveExpression_fx(const QString &expression_fx, const MyNumber &value_x, const QString &variable_x);

    //convenience function, where value_x is a type QList<Complexo> instead of a string
    //QList<Complexo> SolveExpression_fx(const QString &expression_fx, QList<Complexo> &value_x, const QString &variable_x);
    MyNumber SolveExpression_fx(const QString &expression_fx, QList<Complexo> &value_x, const QString &variable_x);

    //solves math expressions with one variable f(x), where x is a array
    //i.e: expression_fx="2x+4" ; value_x="5;4;7;8;..." ; variable_x ="x"
    QList<Complexo> SolveExpression_fx_ComplexList(const QString &expression_fx, QList<double> &value_x, const QString &variable_x);


    //solves math expressions with one variable f(x), where x is a array
    //i.e: expression_fx="2x+4" ; value_x="5;4;7;8;..." ; variable_x ="x"
    //if the result is a arrai of complex number, is truncated to array of doubles
    QList<double> SolveExpression_fx_List(const QString &expression_fx, QList<double> &value_x, const QString &variable_x);

    //convenience function, where the variable is autodetected
    QList<double> SolveExpression_fx_List(const QString &expression_fx, QList<double> &value_x);


    //solves math expressions with 2 variables, f(x,t), where x is a array and t is time variable
    //i.e: expression_fx="2x+4+t" ; value_x="5;4;7;8;..." ; t= 0.5
    //variable x is autodetected, can be "x" , "y", "my_var"
    //if the result is a arrai of complex number, is truncated to array of doubles
    QList<double> SolveExpression_fxt(const QString &expression_fx, QList<double> &value_x, const double &t);

    //convenience function where value_x is a double
    MyNumber SolveExpression_fxt(const QString &expression_fx, const double &value_x, const double &t);


    //solves math expressions with 2 variables, i.e:
    //expression_fxy = "2x+y" ; variable_x = "x" ; variable_y = "y" ; value_x = "5" ; value_y = "3"
    MyNumber SolveExpression_fxy(const QString &expression_fx,
                               const QString &value_x, const QString &variable_x,
                               const QString &value_y, const QString &variable_y);

    //convenience function, where value_x and value_y are a double's instead of a string's
    MyNumber SolveExpression_fxy(const QString &expression_fx,
                               const double &value_x, const QString &variable_x,
                               const double &value_y, const QString &variable_y);

    //convenience function, where value_x and value_y are type Number instead of a string's
    //(type Number can be a double, Complexo or Matrix)
    MyNumber SolveExpression_fxy(const QString &expression_fx,
                               const MyNumber &value_x, const QString &variable_x,
                               const MyNumber &value_y, const QString &variable_y);


    //solves math expressions with n variables, i.e:
    //expression_fn = "5+x+y+z+var1" ; variables_List = {x; y; z; var1} ; values_List = {2.3; 1; 5; 6.1}
    MyNumber SolveExpression_fn(const QString &expression_fn,
                              QStringList &values_List_, QStringList &variables_List_);

    //convenience method with double values instead of QString values
    MyNumber SolveExpression_fn(const QString &expression_fn,
                              QList<double> &values_List_, QStringList &variables_List_);

    //convenience method with class Number values instead of QString values
    MyNumber SolveExpression_fn(const QString &expression_fn, QList<MyNumber> &values_List_, QStringList &variables_List_);


    QString Expression_ReplaceVariables_WithValues(QString expression, QStringList &variables, QStringList &values);

    /////////////////////////////////////////////////////////////////////////////////////////

    //grabs the variables from a expression, i.e:
    // expression = "5+x+y+z+var1" ; stores the variables "x", "y", "z" and "var1" in "list_variables"
    // and returns the number of variables (4 in this example)
    int GrabVariables(const QString &expression, QStringList &list_variables);




    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // validate expressions

    //check if expression is a valid math expression, like i.e: "2+3*cos(pi)"
    bool isValidExpression(const QString &expression);
    //convenience method, save the result in value_
    bool isValidExpression(const QString &expression, MyNumber &value_);


    //check if expression is a valid math expression with one variable, like i.e: "2+x"
    bool isValidExpression_fx(const QString &expression);
    //convenience method, saves the one variable from expression in "variable"
    bool isValidExpression_fx(const QString &expression, QString &variable);

    //check if expression is a valid math expression f(t) with one 't' time variable, like i.e: "2+t"
    bool isValidExpression_ft(const QString &expression);


    //check if expression is a valid math expression f(variable,t) i.e. with 2 variables,
    //one of them being 't', the time varible, like e.e: "cos(x+t)", "2*y+t"
    bool isValidExpression_fxt(const QString &expression);
    //convenience method, saves the variable other than 't', in "variable"
    bool isValidExpression_fxt(const QString &expression, QString &variable);




    //check if expression is a valid math expression with n variables (n > 0), like i.e: "5+x+y*z"
    bool isValidExpression_fn(const QString &expression);
    //convenience function where the it check the number of variables in "expression" against "n"
    //bool isValidExpression_fn(const QString &expression, const uint n);


    //check if expression is a valid equation with one variable, like i.e.: "2+x=4x-5" or "var1+2=3*var1^2"
    bool isValidEquation(const QString &expression);
    //convenience method, saves first and second members in "equation_First_Member" and "equation_Second_Member"
    bool isValidEquation(const QString &expression, QString &equation_First_Member, QString &equation_Second_Member, QString &variable);


    //check if expression is a valid equation, explicit from a constant expression
    //the first member must be only the variable, and second member must be only a expression without variables
    // like i.e.: "x=4-5" or "var1=2+cos(pi)"
    bool isValidEquation_Explicit_From_Constant(const QString &equation);
    //convenience method, saves variable in variable_ and value in value_
    bool isValidEquation_Explicit_From_Constant(const QString &equation, QString &variable_, MyNumber &value_);

    //check if expression is a valid equation, explicit from other variables
    //like i.e.:  "x=2y+z"
    bool isValidEquation_Explicit_From_Variables(const QString &equation);
    //convenience method, saves first member variable in "variable_first_member"
    //and the variables of second member in "variables_second_member"
    bool isValidEquation_Explicit_From_Variables(const QString &equation, QString &first_member, QString &second_member);



    //check for errors after solving expressions with SolveExpression_...() methods
    bool error();


private:
    int Degree_Rad_Grad;
    QStringList m_listFunctions;

public:
    void setDegreeRadGrad(const int &isDegreeRadGrad_);
    int getDegreeRadGrad();


private:
    //parse a math expression i.e. "2+3*5" to a token list, then calls ListSolver
    //this function is called from SolveExpression(...)
    MyNumber ParseExpressionToList(const QString &expression_);

    //convenience function, parse a math expression with variables, i.e. "2*x+y"
    //variables_List are "x" and "y", values_List are their values i.e. 4 and 3
    // replaces variables with their values in the token list i.e. "2x+y" -> "2*4+3"
    //then calls ListSolver
    MyNumber ParseExpressionToList(const QString &expression_, QStringList &variables_List, QList<MyNumber> &values_List);

    //Solves a list of token elements
    MyNumber ListSolver();





private:
    QVector<MyNumber> m_tokenNumberList;
    bool bError;

private:
    // check for a number inside the expression_ at given index and return true if it's a valid number
    //example: number_ = "2+3.45*7" ,  index = 2
    // return true,  stores 3.45 in number_  and set's index to 5 (ending index of the number)
    bool grabNumber(const QString &expression_, int &index, double &number_);

    // check for a matrix inside the expression_ at given index and return true if it's a valid matrix
    //example: number_ = "2+[2 3; 6 8]*7" ,  index = 2
    // return true,  stores [2 3; 6 8] in matrix_  and set's index to 11 (ending index of the matrix)
    bool grabMatrix(const QString &expression_, int &index, Matrix &matrix_);



public:
    //check inside the expression_, for functions pre-defined or user defined or variables
    //functions always start with a letter, can contain letters, numbers and underscores
    // examples: "cos", "sin", "x", "x1", "my_variable"
    bool grabFunction_aux(const QString &expression_, int &index, QString &function_);

    //check for pre-defined functions like "sin", "cos", ... inside an expression_ at given index
    //if found return true and store it in function_
    bool grabFunction_predefined(const QString &expression_, int &index, QString &function_);

    //check for user defined functions/variables like "x", "y1", "my_variable"... inside an expression_ at given index
    //if found return true and store it in function_
    bool grabFunction_or_Variable_userdefined(const QString &expression_, int &index, QString &function_);

private:
    bool grabOperatorOrParentheses(const QString expression_);


    //check for "pi", "e" (euler number) and "i" (imaginary number)
    bool grabConstants(const QString &expression_, int &index, QString &constants_);
    //check if it is "i" imaginary number
    bool isImaginaryNumber(const QString &expression_, int &index, QString &constants_);


    //check if it is "pi"
    bool isPiNumber(const QString &expression_, int &index, QString &constants_);

    //check if it is "e" Neper number
    bool isNeper(const QString &expression_, int &index, QString &constants_);



public:
    //replace in an expression, a list of varibles by a list of values
    //example: expression_ = "cos(x)+y*z" ; variables={x,y,z}; values={3,2,4}
    // will return "cos(3)+2*4"
    QString expression_replace_variables_with_values(const QString &expression_, QStringList &variables, QStringList &values);
    //convenience function
    QString expression_replace_variables_with_values(const QString &expression_, QPair<QStringList, QStringList> &variables_values);

private:
    // following methods solve functions/operators of the token list
    void SolveOperator(int &i);
    void SolveFunctions(int &i);
    void RemoveParentheses(int &i);
    void RemoveParentheses_abs(int &i);


    // following methods check if previous and next elements of the list exist
    bool bExist_Next_Element_List(int &i);
    bool bExist_Next_2_Elements_List(int &i);
    bool bExist_Previous_Element_List(int &i);
    bool bExist_Previous_2_Elements_List(int &i);

public:
    void unitTest();

};

#endif // PARSER_H
