#include<iostream>
#include<map>
#include<utility>
#include<string>
#include<fstream>
using namespace std;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>

void search(char* buff,map<string,string>& my_map)
{

    char buf[1024] = {0};
    string value_str = "not found";
    sscanf(buff,"instruction=%s",&buf);
    string key_str(buf);
    auto it = my_map.find(key_str);
    if(it != my_map.end())
    {
        value_str = it->second;
    }

   // fprintf(stderr, "%s\n", key_str.c_str());
   // fprintf(stderr, "%s\n", value_str.c_str());
   // fprintf(stderr, "%s\n", it->second.c_str());

    printf("<html>\n");
    printf("<body>\n");

    cout<<"<h3>"<<key_str<<"</h3>"<<endl;
    cout<<"<h3>"<<value_str<<"</h3>"<<endl;

    printf("</body>\n");
    printf("</html>\n");

}

int main()
{
    ifstream ins("wwwroot/cgi/instruction.txt");
    map<string,string> my_map;
    while(!ins.eof())
    {
        string key;
        string value;
        ins >> key>>value;
        my_map.insert(make_pair(key,value));
    }

    //map<string,string>::iterator it = my_map.begin();
    /*
    for(;it != my_map.end();it++)
    {
        cout<<"key: "<<it->first<<" "<<" value: "<<it->second<<endl;
    }
    */

    char buff[1024] = {0};
	if(getenv("METHOD")){
		if(strcasecmp(getenv("METHOD"), "GET") == 0){
			strcpy(buff, getenv("QUERY_STRING"));
		}else{
			int content_length = atoi(getenv("CONTENT_LENGTH"));
			int i = 0;
			char c;
			for(; i < content_length; i++){
				read(0, &c, 1);
				buff[i] = c;
			}
			buff[i] = '\0';
		}
	}

    search(buff,my_map);
    return 0;
}

