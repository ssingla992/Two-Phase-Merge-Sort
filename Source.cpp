#include<iostream>
#include<string>
#include<cstring>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include <sstream>
#include<queue>

using namespace std;

vector<int> col, flg;
int asc = 0;
class StringListCompare
{
public:
  bool operator()(const vector<string>& lhs, const vector<string>& rhs)
  {
    int i;
	  for(i=0;i<col.size();i++){
		if(lhs[col[i]-1] == rhs[col[i]-1])
			continue;
		if(asc)
			return lhs[col[i]-1] < rhs[col[i]-1];
		else
			return rhs[col[i]-1] < lhs[col[i]-1];
	  }
	  if(asc)
			return lhs[col[i-1]-1] < rhs[col[i-1]-1];
	  else
			return rhs[col[i-1]-1] < lhs[col[i-1]-1];
  }
};

class StringListCompare1
{
public:
  bool operator()(const pair<vector<string>, int>& lhs, const pair<vector<string>, int>& rhs)
  {
	  int i;
	  for(i=0;i<col.size();i++){
		if(lhs.first[col[i]-1] == rhs.first[col[i]-1])
			continue;
		if(asc)
			return lhs.first[col[i]-1] > rhs.first[col[i]-1];
		else
			return rhs.first[col[i]-1] > lhs.first[col[i]-1];
	  }
	  if(asc)
			return lhs.first[col[i-1]-1] > rhs.first[col[i-1]-1];
	  else
			return rhs.first[col[i-1]-1] > lhs.first[col[i-1]-1];
  }
};

void write(ofstream& tmp, vector<vector<string> > record){
	for(int i=0;i<record.size();i++){
		for(int j=0;j<3;j++)
			tmp << record[i][j] << " ";
		tmp << endl;
	}
}

void read(istream& temp, vector<vector<string> >& block, int n, int index){
	int i, flag = 0;
	string s;
	vector<string> tmp;
	for(i=0;i<n;i++){
		getline(temp, s);
			if(!temp.eof()){
				tmp.clear();
			    istringstream stream1;
		        string word;
				stream1.str(s);
				while(stream1 >> word)
					tmp.push_back(word);
				block.push_back(tmp);
			}
			else{
				flg.push_back(index);
				break;
			}
		}
}

void final_sort(int sorted_files, int n, int mem){
	int buffers, i, flag = 0, count = 0, j, length = n;
	static int a = sorted_files;
	buffers = sorted_files+1;
	mem = 4096;
	n = mem/n;
	vector<vector<string> > *block;
	block = new vector<vector<string> >[buffers];
	priority_queue<vector<string>, vector<pair<vector<string>,int> >, StringListCompare1 > heap;
	pair<vector<string>,int> dalo;
	ifstream *temp;
	ofstream writ;
	string s;
	stringstream out;
	out << sorted_files;
	s = out.str();
	writ.open(&s[0]);
	temp = new ifstream[sorted_files];
	for(i=0;i<sorted_files;i++){
		string s;
		stringstream out;
		out << i;
		s = out.str();
		temp[i].open(&s[0]);
		read(temp[i], block[i], n, i);
		dalo.first = block[i][0];
		dalo.second = i;
		heap.push(dalo);
		block[i].erase(block[i].begin());
	}
	while(1){
		flag = 1;
		if(n == count){
			write(writ, block[buffers-1]);
			count = 0;
			block[buffers-1].clear();
		}
		for(i=0;i<sorted_files;i++)
			if(find(flg.begin(), flg.end(), i) == flg.end() || !block[i].empty()){
				flag = 0;
				break;
			}	
		if(flag){
			while(!heap.empty()){
				block[buffers-1].push_back(heap.top().first);
				heap.pop();
			}
			write(writ, block[buffers-1]);
			break;
		}
		count++;
		if(!heap.empty()){
			block[buffers-1].push_back(heap.top().first);
			j = heap.top().second;
			heap.pop();
		}
		if(find(flg.begin(), flg.end(), j) != flg.end() && block[j].empty())
			continue;
		else if(block[j].empty())
			read(temp[j], block[j], n, j);
		dalo.first = block[j][0];
		dalo.second = j;
		heap.push(dalo);
		block[j].erase(block[j].begin());
	}
	for(i=0;i<sorted_files;i++)
		temp[i].close();
	writ.close();
}

int main(int argc, char **argv){
	if(strcmp(argv[3], "desc"))
		asc = 1;
	for(int i=4; i<argc;i++)
		col.push_back(atoi(&argv[i][1]));
	int mem = atoi(argv[2])*1000000;
	int n, file = 0, i, flag=0, sorted_files = 0;
	char s[1000];
	FILE *f1;
	f1 = fopen (argv[1],"r");
	fscanf(f1, "%[^\n]", s);
	fclose(f1);
	n = mem/strlen(s);
	vector<vector<string> > record;
	vector<string> tmp;
	string word;
	const char* p;
	ifstream f (argv[1]);
	while(1){
		ofstream f2;
		sorted_files++;
		record.clear();
		read(f, record, n, -1);
		sort(record.begin(), record.end(), StringListCompare());
		string s;
		stringstream out;
		out << file++;
		s = out.str();
		f2.open(&s[0]);	
		write(f2, record);
		f2.close();
		if(find(flg.begin(), flg.end(), -1) != flg.end())
			break;
	}
	int tmp = ((sorted_files+1)*4096);
	if(tmp > mem){
		printf(" Sorting Not Feasible\n");
		return 0;
	}
	f.close();
	final_sort(sorted_files, strlen(s), mem);
	return 0;
}
