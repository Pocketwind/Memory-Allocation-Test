#include<stdio.h>
#include<iostream>
#include<vector>
#include<string>

#pragma warning(disable:4996)

using namespace std;

#define MAX 100

int Max[MAX][MAX];
int Allocation[MAX][MAX];
int Available[MAX];
int Need[MAX][MAX];
int h, w;
bool p_end[MAX];

int main(int argc, char** argv);
int readfile(char*);
bool is_Available(int[],int[]);
bool is_End();

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "사용법 - safe.exe input.txt";
		return 0;
	}
	if (readfile(argv[1]))
	{
		cout << "파일이 없습니다";
		return 0;
	}

	cout << w << endl << h << endl;
	cout << "Allocation\t";
	for (int i = 0; i < w; ++i)
	{
		cout << (char)('A' + i) << '\t';
	}
	cout << endl;
	for (int i = 0; i < h; ++i)
	{
		printf("P_%02d\t\t", i);
		for (int k = 0; k < w; ++k)
		{
			cout << Allocation[i][k] << '\t';
		}
		cout << endl;
	}
	cout << endl;

	cout << "Max\t\t";
	for (int i = 0; i < w; ++i)
	{
		cout << (char)('A' + i) << '\t';
	}
	cout << endl;
	for (int i = 0; i < h; ++i)
	{
		printf("P_%02d\t\t", i);
		for (int k = 0; k < w; ++k)
		{
			cout << Max[i][k] << '\t';
		}
		cout << endl;
	}
	cout << endl;

	cout << "Available\t";
	for (int i = 0; i < w; ++i)
	{
		cout << (char)('A' + i) << '\t';
	}
	cout << endl << "\t\t";
	for (int k = 0; k < w; ++k)
	{
		cout << Available[k] << '\t';
	}
	cout << endl << endl;

	int count = 0;
	int max_try = (h + 1) * (h + 1);
	bool deadlock = false;
	vector<int> safe, unsafe;
	while (!is_End())
	{
		for (int i = 0; i < h; ++i)
		{
			if (max_try < count)
			{
				deadlock = true;
				break;
			}
			if (is_Available(Need[i], Available) && p_end[i] == false)
			{
				p_end[i] = true;
				for (int k = 0; k < w; ++k)
					Available[k] += Allocation[i][k];
				safe.push_back(i);
				break;
			}
		}
		if (deadlock)
			break;
		++count;
	}


	if (deadlock)
	{
		count = 0;
		memset(p_end, 0, sizeof(bool) * MAX);
		while (!is_End())
		{
			for (int i = 0; i < h; ++i)
			{
				if (count > max_try)
					break;
				if (!is_Available(Need[i], Available) && p_end[i] == false)
				{
					p_end[i] = true;
					unsafe.push_back(i);
					break;
				}
			}
			count++;
			if (count > max_try)
				break;
		}
	}
	
	cout << "Result : ";


	if (deadlock)
	{
		cout << "Unsafe - ";
		for (int k = 0; k < unsafe.size(); ++k)
			printf("P_%02d ", unsafe[k]);
	}
	else
	{
		cout << "Safe - ";
		for (int k = 0; k < safe.size(); ++k)
			printf("P_%02d ", safe[k]);
	}
	return 0;
}

int readfile(char* filename)
{
	FILE* file;
	char line[1000];
	char* tok;
	string str;
	vector<int> data;
	fopen_s(&file, filename, "r");
	if (file == NULL)
		return 1;

	fgets(line, 1000, file);
	tok = strtok(line, " ");
	w = atoi(tok);
	tok = strtok(NULL, " ");
	h = atoi(tok);

	while (fgets(line, 1000, file))
	{
		tok = strtok(line, " \r\t");
		do
		{
			str = tok;
			if (strlen(tok) == 1 && isdigit(tok[0]))
			{
				data.push_back(atoi(tok));
			}
		} while (tok = strtok(NULL, " \r\t\n"));
	}


	int i = 0;
	for (int k = 0; k < h; ++k)
	{
		for (int j = 0; j < w; ++j)
			Allocation[k][j] = data[i++];
	}

	for (int k = 0; k < h; ++k)
	{
		for (int j = 0; j < w; ++j)
		{
			Max[k][j] = data[i++];
			Need[k][j] = Max[k][j] - Allocation[k][j];
		}
	}

	for (int k = 0; k < w; ++k)
		Available[k] = data[i++];

	
	return 0;
}

bool is_Available(int need[], int available[])
{
	for (int i = 0; i < w; ++i)
	{
		if (need[i] > available[i])
			return false;

	}
	return true;
}

bool is_End()
{
	for (int i = 0; i < h; ++i)
	{
		if (p_end[i] == false)
			return false;
	}

	return true;
}