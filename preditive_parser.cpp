#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <stack>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

void init_pre_analysis(string table_name);//Ԥ��������ʼ��
int  pre_analysis(string input_str);//��������
void print_analy_proc(stack<char> analysis_stack,string input_str,int curr_str);//���������������
void error(int error_num);//���������

map<char,map<char,vector<char>>> pre_table;//Ԥ�������
char start_sym;//�ķ���ʼ��

int main(void)
{
	string input_str;
	string table_name="pre_table.csv";
	ifstream fin_test;
	do
    {
        	cout << "������Ԥ������ļ�����";
        	cin >> table_name;
        	fin_test.open(table_name);
    }
	while(!fin_test.good());
	fin_test.close();
	do
	{
        cout << "������һ���ַ�����";
	}
    while(!(cin >> input_str));
	input_str+='#';
	init_pre_analysis(table_name);
	error(pre_analysis(input_str));
	return 0;
}

void init_pre_analysis(string table_name)
{
	ifstream fin(table_name);
	string line;
	vector<char> vn;//�ս��
	while(true)
    {
        getline(fin,line);
        if(line[0]==' ')
            break;
        else if(line[0]=='@')
        {
            start_sym=line[1];
            for(int index=3;index<line.length();index++)
            {
                if(line[index]==',')
                    continue;
                vn.push_back(line[index]);
            }
        }
        else
        {
            int vn_index=0;
            for(int index=2;index<line.length();index++)
            {
                if(line[index]==',')
                {
                    vn_index++;
                    continue;
                }
                pre_table[line[0]][vn[vn_index]].push_back(line[index]);
            }
        }
    }
    fin.close();
	return;
}

int  pre_analysis(string input_str)
{
    stack<char> analysis_stack;
    analysis_stack.push('#');//����ջ��ʼ��
    analysis_stack.push(start_sym);
    int curr_char=0;//��ǰ�ַ�ָ��
    while(true)
    {
        //����
        if(analysis_stack.top()=='#' && input_str[curr_char]=='#')
        {
            print_analy_proc(analysis_stack,input_str,curr_char);
            break;
        }
        //����
        if(!pre_table[analysis_stack.top()][input_str[curr_char]].size() || pre_table[analysis_stack.top()][input_str[curr_char]][0]==' ')
        {
            return 1;
        }
        //����ʽ�Ҳ�Ϊ��
        else if(pre_table[analysis_stack.top()][input_str[curr_char]][0]=='_')
        {
            print_analy_proc(analysis_stack,input_str,curr_char);
            analysis_stack.pop();
        }
        //�Ƶ�
        else
        {
            print_analy_proc(analysis_stack,input_str,curr_char);
            char prod_left=analysis_stack.top();
            analysis_stack.pop();
            for(int index=pre_table[prod_left][input_str[curr_char]].size()-1;index>=0;index--)
            {
                analysis_stack.push(pre_table[prod_left][input_str[curr_char]][index]);
            }
        }
        //ƥ��
        if(analysis_stack.top()==input_str[curr_char] && analysis_stack.top()!='#')
        {
            print_analy_proc(analysis_stack,input_str,curr_char);
            analysis_stack.pop();
            curr_char++;
        }
    }
	return 0;
}

void print_analy_proc(stack<char> analysis_stack,string input_str,int curr_char)
{
    static int step_num=1;
    if(step_num==1)//����������̱���
    {
        cout << left << setw(5) << "����";
        cout << left << setw(15) << "����ջ";
        cout << right << setw(15) << "ʣ���ַ���";
        cout << "    �Ƶ����ò���ʽ��ƥ��";
        cout << endl;
    }
    cout << left << setw(5) << step_num;//�������
    string temp_str;
    while(!analysis_stack.empty())
    {
        temp_str+=analysis_stack.top();
        analysis_stack.pop();
    }
    reverse(temp_str.begin(),temp_str.end());
    cout << left << setw(15) << temp_str;//�������ջ
    cout << right << setw(15) << input_str.substr(curr_char,input_str.length()-curr_char);
    if(temp_str[temp_str.length()-1] == input_str[curr_char])
    {
        if(input_str[curr_char] == '#')
        {
            cout << "    ����";
        }
        else
        {
            cout << "    " << '\"' << input_str[curr_char] << '\"' << " ƥ��";
        }
    }
    else
    {
        cout << "    " << temp_str[temp_str.length()-1] << "->";
        for(int index=0;index<pre_table[temp_str[temp_str.length()-1]][input_str[curr_char]].size();index++)
        {
            cout << pre_table[temp_str[temp_str.length()-1]][input_str[curr_char]][index];
        }
    }
    cout << endl;
    step_num++;
    return;
}

void error(int error_num)
{
	if(error_num)
        cout << "�˴������ķ��ľ��ӣ�" << endl;
	return;
}
