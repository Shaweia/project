#include<iostream>
#include<map>
#include<fstream>
#include<string>
#include<utility>
using namespace std;

int main()
{
    map<string,string> my_map;
    string word;
    string instr;
    int count = 0;
    while(count != 4)
    {
        cin>>instr;
        cin>>word;
        my_map.insert(make_pair(instr,word));
        count++;
    }
    ifstream ins("instruction.txt");
    ofstream ous("instruction.txt");
    map<string,string>::iterator it = my_map.begin();
    for(;it != my_map.end();it++)
    {
        ous<<it->first<<" "<<it->second<<endl;
    }
    return 0;
}
