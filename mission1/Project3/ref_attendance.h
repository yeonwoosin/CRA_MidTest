#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

using std::string;
using std::vector;
using std::map;
using std::cout;

namespace RefAttendance {

	void AddNewMember(string attendeeName);
	bool IsNewMember(string attendeeName);
	int GetWeekIndex(string attDay);
	bool IsWedDay(int week_index);
	bool IsWeekendDay(int week_index);
	void CalcAttendancePoint(int id2, int week_index);
	int CheckAndGetWedBonus(int userWeekPoints[]);
	int CheckAndGetWeekendBonus(int userWeekPoints[]);
	int GetUserGrade(int userPoint);
	void PrintUserInfo(int userId);
	void ProcessAttendanceRecord(string attendeeName, string attendance_day);
	void ProcessDailyAttendancePoint(std::ifstream& attendaceData);

	void CalcBonusPoint();
	void CalcUserGrade();
	void PrintAllUserInfo();
	void PrintRemovedPlayer();
	void ProcessAttendanceData(std::ifstream& attendaceData);
} // namespace RefAttendance

