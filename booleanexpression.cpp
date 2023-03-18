#include <bits/stdc++.h>
using namespace std;

//vector of uncovered primes 
//map of vectors for prime implicant 
void finalExpression(map <string, vector<string>>prime, vector<string>uncoveredMins, vector<string>primeImplicats ){
         int mx = 0;
         string maximumPrime;
         vector<string> answer = primeImplicats;
         
         while (uncoveredMins.size()){
           for (auto it: prime){
            int value = 0;
             for (auto minT: it.second){
                if (count(uncoveredMins.begin(), uncoveredMins.end(),minT)){
                      value++;
                } 
            }
            if(value>mx){
                mx = value;
                maximumPrime = it.first;
            }
          }
          for(auto srt: prime[maximumPrime]){
            remove(uncoveredMins.begin(), uncoveredMins.end(), srt);
          }
           
        answer.push_back (maximumPrime);
    }

         //print answer and all the EPIs       
}
