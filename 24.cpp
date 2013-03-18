#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <stack>
#include <set>

using namespace std;

typedef vector<int>     vi;
typedef vector<string>   vs;

#define pb			push_back
#define mp			make_pair

//Define global variable here
set<string> postfix; //The postfix set to filter repeats
int ans; //The answer to reach

//The shunting yard algorithm, converts infix to postfix
string shuntingYard(string infix){

     stringstream in(infix+"\n"), out;

     stack<char> ophold;

     while(in.peek()!='\n'){
          char token;

          in >> token;

          //Funnels numbers directly
          if(isdigit(token))
               out << token;
          else
               switch(token){
                    //+ and - has lower precedence, pop the stack for other operators
                    case '+':
                    case '-':
                         while(ophold.top()=='*' || ophold.top()=='/' || ophold.top()=='+' || ophold.top()=='-'){
                              out << ophold.top();
                              ophold.pop();
                         }
                         ophold.push(token);
                         break;
                    //* and / has higher precedence, pop the stack for other * and / only
                    case '*':
                    case '/':
                         while(ophold.top()=='*' || ophold.top()=='/'){
                              out << ophold.top();
                              ophold.pop();
                         }
                         ophold.push(token);
                         break;
                    //Push open parenthesis
                    case '(':
                         ophold.push(token);
                         break;
                    //Pop till close parenthesis
                    case ')':
                         while(ophold.top()!='('){
                              out << ophold.top();
                              ophold.pop();
                         }
                         ophold.pop();
                         break;
               }

     }

     return out.str();

}

void recurCompute(vi numbers, vs method);

//A helper to format data for the recursion
void recurStep(vi numbers, vs method,int combinedVal, string combineMethod){
     if(combinedVal>=0){
          //Adds the new cases to the list
          numbers.pb(combinedVal);
          method.pb(combineMethod);
          recurCompute(numbers, method);
     }
}

//The recursion step to generate the sub cases for recursion
void recurCompute(vi numbers, vs method){

     //Empty case
     if(numbers.size()==0)
          return;

     //Base/return case
     if(numbers.size()==1 && numbers[0]==ans){
          //Partial repeat filtering using shunting yard
          string pf = shuntingYard(method[0]);
          if(postfix.find(pf)==postfix.end()){
               postfix.insert(pf);
               cout << method[0]<<endl;
          }
     }

     //n^2 loop to find all combinations of numbers in the current list
     for(int i=0;i<numbers.size();i++){
          for(int j=i+1;j<numbers.size();j++){

               //List without 2 numbers to combine
               vi filter;
               vs filterS;
               for(int x=0;x<numbers.size();x++){
                    if(x!=i && x!= j){
                         filter.pb(numbers[x]);
                         filterS.pb(method[x]);
                    }
               }

               //Numbers to combine
               int first = numbers[i], second = numbers[j], combined=0;
               string firstS = method[i], secondS = method[j];

               //Addition
               if(firstS.length()<secondS.length() ||(firstS.length()==secondS.length() && first<=second))
                    recurStep(filter,filterS,first+second, "("+firstS+" + "+secondS+")");
               else
                    recurStep(filter,filterS,first+second, "("+secondS+" + "+firstS+")");

               //Subtraction
               recurStep(filter,filterS,first-second,"("+firstS+" - "+secondS+")");
               recurStep(filter,filterS,second-first,"("+secondS+" - "+firstS+")");


               //Multiplication
               if(firstS.length()<secondS.length() ||(firstS.length()==secondS.length() && first<=second))
                    recurStep(filter,filterS,first*second,"("+firstS+" * "+secondS+")");
               else
                    recurStep(filter,filterS,first*second,"("+secondS+" * "+firstS+")");

               //Division
               if(second!=0 && first%second==0)
                    recurStep(filter,filterS,first/second,"("+firstS+" / "+secondS+")");
               if(first!=0 && second%first==0)
                    recurStep(filter,filterS,second/first,"("+secondS+" / "+firstS+")");
          }
     }

}

int main() {
     bool cont;

     do{
          cout << "Enter numbers (with spacing, all 0s to end): ";
          cont = false;

          vi numbers;
          vs numberString;

          string line;
          getline(cin,line);
          stringstream stream(line);
          stringstream intparse;

          //Read in the line of numbers availible
          while(!stream.eof() && line!=""){
               int x;
               stream >> x;

               cont |= x;

               numbers.pb(x);

               intparse << x;
               numberString.pb(intparse.str());
               intparse.str(string());

          }

          //If not all 0, get the number to compute and do the recursion
          if(cont){
               cout << "Enter number to reach: ";
               cin >> ans;
               getline(cin, line);

               recurCompute(numbers, numberString);
               postfix.clear();
          }
     }while(cont);
}
