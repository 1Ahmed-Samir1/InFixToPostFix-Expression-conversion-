#include <iostream>
#include "stack.h"
#include "String"
#include "stdio.h"
#include "math.h"

using namespace std;

void stack_push(node *&s,char x);
void stack_pop(node *&s);
bool stack_empty(node *&s);
void stack_initialize(node *&s);
void inFixToPostFix(string infix,string &postfix);
int evaluatePostFix(string postfix);

int main()
{
    string inFix;
    string postFix="";
    cout<<"enter the expression"<<endl;
    getline(cin,inFix);
    inFixToPostFix(inFix,postFix);
    cout<<postFix<<endl;
    cout<<evaluatePostFix(postFix)<<endl;

    return 0;
}

//----------------- Stack of char functions------------------------------------------
void stack_push(node *&s,char x){
    node* temp = new node();
    temp->opt = x;
    temp->next = s;
    s = temp;
}
void stack_pop(node *&s){
    node *p = s;
    s = s->next;
    delete p;
}
bool stack_empty(node *&s){
    if(s == NULL)
        return true;
    else
        return false;
}
void stack_initialize(node *&s){
    s = NULL;
}
//----------------- Stack of Integers functions------------------------------------------
void intStack_push(nodeInt *&s,int x){
   nodeInt *temp = new nodeInt();
   temp->data = x;
   temp->next = s;
   s = temp;
}
void intStack_pop(nodeInt *&s,int &x){
    nodeInt *p = s;
    x = s->data;
    s = s->next;
    delete p;
}
bool intStack_empty(nodeInt *&s){
    if(s == NULL)
        return true;
    else
        return false;
}
void intStack_initialize(nodeInt *&s){
    s = NULL;
}
//-----------------------Expression Conversion supporting functions--------------------------------
 //Check if is operant
 bool isOperant(char c){
    if(c >='0' && c<='9')
        return true;
    else if(c>='a' && c<='z')
        return true;
    else if(c>='A' && c<='Z')
        return true;
    else
        return false;
 }
 //check if is operator
 bool isOperator(char c){
    if(c == '+' || c=='-' || c=='/' || c== '*' || c=='^')
        return true;
    else
        return false;
 }
 // getting the operator weight
 int getWeight(char c){
    switch(c){
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    default:
        return -1;
    }
 }
 // checking for stack higher precedence
bool isHigher(char s,char opt){
    if(getWeight(s)>= getWeight(opt))
        return true;
    else
        return false;
}
// checking if operator is opening parenthesis
bool isOpening(char c){
    if(c == '(' || c=='{' || c== '[')
        return true;
    else
        return false;
}
// checking if operator is closing parenthesis
bool isClosing(char c){
    if(c == ')' || c=='}' || c== ']')
        return true;
    else
        return false;
}
//-------------------------Converting inFix To PostFix function----------------------
 void inFixToPostFix(string infix,string &postfix){
    node *s = NULL; //initialize the stack

    for(unsigned int i=0;i<infix.length();i++){

        if(infix[i] == ' ')
            continue;
        else if(isOperant(infix[i])){
            postfix+=infix[i];
            if(i == infix.length()-1)
                postfix+=' ';
        }
        //if the operator is (
        else if(isOpening(infix[i]))
            stack_push(s,infix[i]);
        // normal operator
        else if(isOperator(infix[i])){
            postfix+=' ';
            while(!stack_empty(s) && isHigher(s->opt,infix[i]) && !isOpening(infix[i])){
                postfix+=s->opt;
                stack_pop(s);
            }
            stack_push(s,infix[i]);
        }
        else if(i == infix.length())
            postfix+=' ';
        // if the operator is )
        else if(isClosing(infix[i])){
            postfix+=' ';
            while(!stack_empty(s) && isHigher(s->opt,infix[i]) && !isOpening(s->opt)){
                postfix+=s->opt;
                stack_pop(s);
            }
            if(!stack_empty(s))
                stack_pop(s);
        }
    }//end of String

    // returning the rest of stack to postFix String
    while(!stack_empty(s)){
        if(!isOpening(s->opt)){
            postfix+=s->opt;
        }
        stack_pop(s);
    }
 }
//------------------- postFix Expression evaluation--------------------
// function to calculate the postFix expression
int calculate(int x2,int x1,char opt){
    if(opt == '+')
        return x1 + x2;
    else if(opt == '-')
        return x1 - x2;
    else if(opt == '*')
        return x1 * x2;
    else if(opt == '/')
        return x1 / x2;
    else if(opt == '^')
        return pow(x1,x2);
    else
        return 0;
}
int evaluatePostFix(string postfix){
    int x,x1,x2,m;
    nodeInt *s = NULL; //Creating an empty stack of integers
    //Parsing the PostFix
    for(unsigned int i=0;i<postfix.length();i++){
        if(isOperant(postfix[i])){
            m = 10;
            x=0;
            while(postfix[i]!=' '){
                x*=m;
                x+=postfix[i]-48;
                i++;
            }
            intStack_push(s,x);
        }
        else if(isOperator(postfix[i])){
            intStack_pop(s,x1);
            intStack_pop(s,x2);
            intStack_push(s,calculate(x1,x2,postfix[i]));
        }
    }
    intStack_pop(s,x1);
    return x1;
}
