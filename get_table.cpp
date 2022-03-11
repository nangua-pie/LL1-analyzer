#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;

void prog_in(string prog_file);
void get_select(void);
bool is_ll1(void);
void out_table(string pre_table);

typedef struct production {
    char prod_left;
    string prod_right;
}production;

vector<production> programmer;//文法
vector<char> vn;//非终结符
vector<char> vt;//终结符
map<string,set<char>> first_set;
map<char,set<char>> follow_set;
map<int,set<char>> select_set;

int main(void)
{
    string prog_file;
    string pre_table;
    ifstream fin_test;
    do
    {
        cout << "请输入语法文件名：";
        cin >> prog_file;
        fin_test.open(prog_file);
    }
    while(!fin_test.good());
    fin_test.close();
    prog_in(prog_file);
    get_select();
    if(!is_ll1())
    {
        cout << "该文法不是LL(1)文法！" << endl;
        return 0;
    }
    pre_table=prog_file;
    pre_table.replace(pre_table.length()-3,3,"csv");
    out_table(pre_table);
    cout << "预测分析表生成完毕！";
    return 0;
}

void prog_in(string prog_file)
{
    ifstream prog_fin(prog_file);
    string line;
    getline(prog_fin,line);
    for(int index=0;index<line.length();index++)
    {
        vn.push_back(line[index]);
    }
    getline(prog_fin,line);
    for(int index=0;index<line.length();index++)
    {
        vt.push_back(line[index]);
    }
    while(true)
    {
        getline(prog_fin,line);
        if(line[0]=='$')
            break;
        programmer.push_back({line[0],line.substr(3,line.length()-3)});
    }
    for(int prod_num=0;prod_num<programmer.size();prod_num++)
    {
        getline(prog_fin,line);
        for(int index=programmer[prod_num].prod_right.length()+9;index<line.length()-1;index++)
        {
            first_set[programmer[prod_num].prod_right].insert(line[index]);
        }
    }
    for(int vn_num=0;vn_num<vn.size();vn_num++)
    {
        getline(prog_fin,line);
        for(int index =11;index<line.length()-1;index++)
        {
            if(line[index]==',')
                continue;
            follow_set[vn[vn_num]].insert(line[index]);
        }
    }
    prog_fin.close();
    return;
}

void get_select(void)
{
    for(int prod_num=0;prod_num<programmer.size();prod_num++)
    {
        production prod_pointer = programmer[prod_num];
        select_set[prod_num].insert(first_set[programmer[prod_num].prod_right].begin(),first_set[programmer[prod_num].prod_right].end());
        if(first_set[prod_pointer.prod_right].find('_')==first_set[prod_pointer.prod_right].end())
        {
            continue;
        }
        else
        {
            select_set[prod_num].erase('_');
            select_set[prod_num].insert(follow_set[prod_pointer.prod_left].begin(),follow_set[prod_pointer.prod_left].end());
        }
    }
    return;
}

bool is_ll1(void)
{
    for(int prod_index=0;prod_index<programmer.size();prod_index++)
    {
        if(programmer[prod_index].prod_left == programmer[prod_index].prod_right[0])
            return false;
    }
    for(int vn_index=0;vn_index<vn.size();vn_index++)
    {
        vector<int> vt_num(vt.size()+1,0);//终结符在相同左部产生式select集中出现次数
        for(int vt_index=0;vt_index<vt.size()+1;vt_index++)
        {
            for(int prod_index=0;prod_index<programmer.size();prod_index++)
            {
                if(programmer[prod_index].prod_left==vn[vn_index])
                {
                    if(vt_index==vt.size())
                    {
                         if(select_set[prod_index].find('#')!=select_set[prod_index].end())
                         vt_num[vt_index]++;
                    }
                    else
                    {
                         if(select_set[prod_index].find(vt[vt_index])!=select_set[prod_index].end())
                         vt_num[vt_index]++;
                    }
                }
            }
        }
        for(int index=0;index<vt_num.size();index++)
        {
            if(vt_num[index]>1)
                return false;
        }
    }
    return true;
}

void out_table(string pre_table)
{
    ofstream pre_file(pre_table);
    pre_file << '@' << vn[0];
    for(int vt_index=0;vt_index<vt.size()+1;vt_index++)
    {
        if(vt_index==vt.size())
        {
            pre_file << ',' << '#';
        }
        else
        {
            pre_file << ',' << vt[vt_index];
        }
    }
    pre_file << endl;
    for(int vn_index=0;vn_index<vn.size();vn_index++)
    {
        pre_file << vn[vn_index];
        vector<int> vt_temp(vt.size()+1,0);
        vector<int> vt_prod(vt.size()+1,-1);
        for(int prod_index=0;prod_index<programmer.size();prod_index++)
        {
            if(programmer[prod_index].prod_left==vn[vn_index])
            {
                for(int vt_index=0;vt_index<vt.size()+1;vt_index++)
                {
                    if(vt_index==vt.size())
                    {
                        if(select_set[prod_index].find('#')!=select_set[prod_index].end())
                        {
                            vt_temp[vt_index]=1;
                            vt_prod[vt_index]=prod_index;
                        }
                    }
                    else
                    {
                        if(select_set[prod_index].find(vt[vt_index])!=select_set[prod_index].end())
                        {
                            vt_temp[vt_index]=1;
                            vt_prod[vt_index]=prod_index;
                        }
                    }
                }
            }
        }
        for(int vt_index=0;vt_index<vt.size()+1;vt_index++)
        {
            if(vt_temp[vt_index])
            {
                pre_file << ',' << programmer[vt_prod[vt_index]].prod_right;
            }
            else
            {
                pre_file << ',' << ' ';
            }
        }
        pre_file << endl;
    }
    pre_file << ' ';
    for(int index=0;index<vt.size()+1;index++)
    {
        pre_file << ',';
    }
    pre_file << endl;
    pre_file.close();
    return;
}
