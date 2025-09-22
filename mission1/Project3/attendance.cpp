#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Node {
	string w;
	string wk;
};

map<string, int> id1;
int id_cnt = 0;

//dat[�����ID][����]
int UserDayAttendanceData[100][100];        // ���� ����
int points[100];
int grade[100];
string names[100];

int wed[100];
int weeken[100];

void input2(string w, string wk) {
	//ID �ο�
	if (id1.count(w) == 0) {
		id1.insert({ w, ++id_cnt });

		if (w == "Daisy") {
			int debug = 1;
		}

		names[id_cnt] = w;
	}
	int id2 = id1[w];

	//������
	if (w == "Daisy") {
		int debug = 1;
	}


	int add_point = 0;
	int index = 0;
	if (wk == "monday") {
		index = 0;
		add_point++;
	}
	if (wk == "tuesday") {
		index = 1;
		add_point++;
	}
	if (wk == "wednesday") {
		index = 2;
		add_point += 3;
		wed[id2] += 1;
	}
	if (wk == "thursday") {
		index = 3;
		add_point++;
	}
	if (wk == "friday") {
		index = 4;
		add_point++;
	}
	if (wk == "saturday") {
		index = 5;
		add_point += 2;
		weeken[id2] += 1;
	}
	if (wk == "sunday") {
		index = 6;
		add_point += 2;
		weeken[id2] += 1;
	}

	//�����ID�� ���� �����Ϳ� 1�� ����
	UserDayAttendanceData[id2][index] += 1;
	points[id2] += add_point;
}

void input() {  //������ ����.
	ifstream fin{ "attendance_weekday_500.txt" }; //500�� ������ �Է�
	for (int i = 0; i < 500; i++) { // magic �ѹ�
		string t1, t2;
		fin >> t1 >> t2;
		input2(t1, t2);
	}

	for (int i = 1; i <= id_cnt; i++) {
		if (UserDayAttendanceData[i][2] > 9) {
			points[i] += 10;
		}

		if (UserDayAttendanceData[i][5] + UserDayAttendanceData[i][6] > 9) {
			points[i] += 10;
		}

		if (points[i] >= 50) {
			grade[i] = 1;
		}
		else if (points[i] >= 30) {
			grade[i] = 2;
		}
		else {
			grade[i] = 0;
		}

		cout << "NAME : " << names[i] << ", ";
		cout << "POINT : " << points[i] << ", ";
		cout << "GRADE : ";

		if (grade[i] == 1) {
			cout << "GOLD" << "\n";
		}
		else if (grade[i] == 2) {
			cout << "SILVER" << "\n";
		}
		else {
			cout << "NORMAL" << "\n";
		}
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int i = 1; i <= id_cnt; i++) {

		if (grade[i] != 1 && grade[i] != 2 && wed[i] == 0 && weeken[i] == 0) {
			std::cout << names[i] << "\n";
		}
	}
}