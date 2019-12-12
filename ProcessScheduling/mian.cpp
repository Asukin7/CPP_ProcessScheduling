#pragma warning(disable : 4996)
#include <iostream>
#include <iomanip>
#include <ctime>
#include <queue>
#include <windows.h>
using namespace std;

const int N = 6;//����������
int PCBn = 0;//���̱����ñ���
queue<int> Q;//���̶���
time_t now;//��ǰʱ��

class PCB
{
public:
	PCB();

	time_t get_at();
	double get_st();
	int get_sc();
	double get_R();//��ȡResponse_ratio��Ӧ��

	void set_lrt();
	void set_at_st_rst(int t);
	void set_rst_sc_ct(double n);

	void display_current();
	void display_all();
private:
	int pn;//process name��������
	time_t at;//arrival time����ʱ��
	time_t ct;//completion time���ʱ��
	double st;//service time����ʱ��
	time_t lrt;//last run time�������ʱ��
	double rst;//remaining service timeʣ�����ʱ�� 
	int sc;//sign completion��־�Ƿ����
}process[N];

void FCFS_SJF(int n);//�����ȷ���&����ҵ��������̶���
void HRN(int n);//�����Ӧ����������̶���
void gotoxy(int x, int y);//��궨λ

int main()
{
	srand(time(NULL));

	int n;
	cout << "�������������1~6)��" << endl;
	do
	{
		cin >> n;
	} while (n < 1 || n > 6);
	cout << endl;
	for (int i = 0; i < n; i++)
	{
		cout << "���ڴ�������" << i << "..." << endl;
		process[i].set_at_st_rst(rand() % 5 - 2);
		process[i].set_lrt();
	}
	system("cls");

	HRN(n);
	cout << "����\t����";
	gotoxy(0, 9);
	cout << "����  ����ʱ��  ����ʱ��  ʣ�����ʱ��  ���ʱ��  ���״̬";
	for (int i = 0; i < n; i++)
	{
		process[i].display_current();
		process[i].display_all();
	}

	while (!Q.empty())
	{
		double t = 10;
		for (int i = 0; i < 1000 / Q.size(); i += t)
		{
			if (process[Q.front()].get_sc() == 1) break;
			Sleep(t);
			process[Q.front()].set_rst_sc_ct(t / 1000);
			process[Q.front()].display_current();
		}
		process[Q.front()].set_lrt();
		if (process[Q.front()].get_sc() == 0)
		{
			Q.push(Q.front());
			Q.pop();
		}
		else
		{
			Q.pop();
		}
	}

	gotoxy(0, 20);
	system("pause");
	return 0;
}

PCB::PCB()
{
	pn = PCBn;
	sc = 0;
	PCBn++;
}

time_t PCB::get_at()
{
	return at;
}

double PCB::get_st()
{
	return st;
}

int PCB::get_sc()
{
	return sc;
}

double PCB::get_R()
{
	return 1 + (now - lrt) / rst;
}

void PCB::set_lrt()
{
	lrt = time(NULL);
}

void PCB::set_at_st_rst(int t)
{
	at = time(NULL) + t;
	st = rand() % 7 + 2;
	rst = st;
}

void PCB::set_rst_sc_ct(double n)
{
	rst -= n;
	if (rst <= 0)
	{
		rst = 0;
		sc = 1;
		ct = time(NULL);
	}
}

void PCB::display_current()
{
	cout.setf(ios::fixed);
	cout << fixed << setprecision(2);
	double n = (1 - rst / st) * 100;
	if (sc == 1)
	{
		n = 100;
		gotoxy(0, pn + 1);
		cout << pn << "\t" << setw(5) << n << "%";
		display_all();
		return;
	}
	gotoxy(0, pn + 1);
	cout << pn << "\t" << setw(5) << n << "%";
	gotoxy(26, pn + 10);
	cout << fixed << setprecision(0) << setw(2) << rst;
}

void PCB::display_all()
{
	cout.setf(ios::fixed);
	cout << fixed << setprecision(0);
	tm* t;
	gotoxy(0, pn + 10);
	cout << pn;
	gotoxy(6, pn + 10);
	t = localtime(&at);
	cout << t->tm_min << " : " << t->tm_sec;
	gotoxy(16, pn + 10);
	cout << st;
	gotoxy(26, pn + 10);
	cout << setw(2) << rst;
	if (sc == 1)
	{
		gotoxy(40, pn + 10);
		t = localtime(&ct);
		cout << t->tm_min << " : " << t->tm_sec;
		gotoxy(50, pn + 10);
		cout << "���  ";
	}
	else
	{
		gotoxy(40, pn + 10);
		cout << "δ���";
		gotoxy(50, pn + 10);
		cout << "δ���";
	}
	cout << endl;
}

void FCFS_SJF(int n)
{
	int q[N] = { 0,1,2,3,4,5 };
	int t;
	for (int i = 0; i < n; i++)//���򣬰�����ʱ�������������dt��ȣ���st��������
	{
		for (int j = 0; j < n - 1; j++)
		{
			if (process[q[j]].get_at() > process[q[j + 1]].get_at())
			{
				t = q[j];
				q[j] = q[j + 1];
				q[j + 1] = t;
			}
			else if (process[q[j]].get_at() == process[q[j + 1]].get_at())
			{
				if (process[q[j]].get_st() > process[q[j + 1]].get_st())
				{
					t = q[j];
					q[j] = q[j + 1];
					q[j + 1] = t;
				}
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		Q.push(q[i]);
	}
}

void HRN(int n)
{
	int q[N] = { 0,1,2,3,4,5 };
	int t;
	now = time(NULL);
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			if (process[q[i]].get_R() < process[q[j]].get_R())
			{
				t = q[i];
				q[i] = q[j];
				q[j] = t;
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		Q.push(q[i]);
	}
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
};