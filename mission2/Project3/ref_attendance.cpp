#include "ref_attendance.h"

namespace RefAttendance {

using std::string;
using std::vector;
using std::map;
using std::cout;

map<string, int> s_mapUserId;
int TotalIdCnt = 0;

static const int MAX_USER = 100;
static const int MAX_DAY = 7;
static const int ATTENDANCE_BONUS_MIN_COUNT = 10;
static const int WED_ADDITIONAL_BOUUS_POINT = 10;
static const int WED_ATTENDANCE_BOUUS_POINT = 2;
static const int WEEKEND_ATTENDANCE_BOUUS_POINT = 1;

static const int MAX_ATTENDANCE_RECORD = 500;
static const int DAY_ATTANDANCE_POINT[7] = { 1, 1, 3, 1, 1, 2, 2 };

static const string WEEK_STR[7] = { "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday" };
static const string GRADE_STR[3] = { "NORMAL", "GOLD", "SILVER" };
static const int GRADE_POINT[3] = { 0, 50, 30 };

//UserDayAttendanceData[사용자ID][요일]
int UserDayAttendanceData[MAX_USER][MAX_DAY];
int points[MAX_USER];
int grade[MAX_USER];
string names[MAX_USER];
int wed[MAX_USER];
int weeken[MAX_USER];

enum GRADE {
	NORMAL, GOLD, SILVER
};
enum WEEK {
	MON, TUE, WED, THU, FRI, SAT, SUN,
	WEEK_COUNT 
};

static const std::unordered_map<string, int> weekIndexMap = {
	{ "monday", WEEK::MON },
	{ "tuesday", WEEK::TUE },
	{ "wednesday", WEEK::WED },
	{ "thursday", WEEK::THU },
	{ "friday", WEEK::FRI },
	{ "saturday", WEEK::SAT },
	{ "sunday", WEEK::SUN }
};

int GetWeekIndex(string attDay)
{
	auto it = weekIndexMap.find(attDay);
	if (it != weekIndexMap.end())
		return it->second;
	return -1; // 없는 경우 -1 반환
}

bool IsWedDay(int week_index)
{
	if (week_index == WEEK::WED) {
		return true;
	}
	return false;
}
bool IsWeekendDay(int week_index)
{
	if ((week_index == WEEK::SAT) || (week_index == WEEK::SUN)) {
		return true;
	}
	return false;
}
void CalcAttendancePoint(int UserID, int week_index)
{
	int add_point = 0;

	if (week_index == -1)
	{
		std::exception("invalid attendance day string");
	}

	if (IsWedDay(week_index))
	{
		add_point += WED_ATTENDANCE_BOUUS_POINT;
		wed[UserID]++;
	}

	if (IsWeekendDay(week_index)) {
		add_point += WEEKEND_ATTENDANCE_BOUUS_POINT;
		weeken[UserID]++;
	}

	add_point++;

	//사용자ID별 요일 데이터에 1씩 증가
	UserDayAttendanceData[UserID][week_index] += 1;
	points[UserID] += add_point;
}

void CheckMaximumUserID(int userID)
{
	if (userID >= MAX_USER) {
		std::exception("exceed max user count");
	}
}

bool IsNewMember(string attendeeName) {
	if (s_mapUserId.count(attendeeName) == 0) {
		return true;
	}
	return false;
}


int GetUserID(string attendeeName)
{
	return s_mapUserId[attendeeName];
}

void ProcessAttendanceRecord(string attendeeName, string attendance_day) {

	int allocatedUserID = 0;
	int week_index = 0;

	//ID 부여
	if (IsNewMember(attendeeName)) {
		AddNewMember(attendeeName);
	}
	try	{
		allocatedUserID = GetUserID(attendeeName);
		CheckMaximumUserID(allocatedUserID);
		week_index = GetWeekIndex(attendance_day);
		CalcAttendancePoint(allocatedUserID, week_index);
	}
	catch (...)	{
		std::exception("ProcessAttendanceRecord failed");
	}
}

int CheckAndGetWedBonus(int userWeekPoints[])
{
	if (userWeekPoints[WEEK::WED] >= ATTENDANCE_BONUS_MIN_COUNT) {
		return WED_ADDITIONAL_BOUUS_POINT;
	}
	return 0;
}

int CheckAndGetWeekendBonus(int userWeekPoints[])
{
	if (userWeekPoints[WEEK::SAT] + userWeekPoints[WEEK::SUN] > ATTENDANCE_BONUS_MIN_COUNT) {
		return WED_ADDITIONAL_BOUUS_POINT;
	}
	return 0;
}

int GetUserGrade(int userPoint)
{
	if (userPoint >= GRADE_POINT[GRADE::GOLD]) {
		return GRADE::GOLD;
	}
	else if (userPoint >= GRADE_POINT[GRADE::SILVER]) {
		return GRADE::SILVER;
	}
	else {
		return GRADE::NORMAL;
	}
}

void PrintUserInfo(int userId)
{
	cout << "NAME : " << names[userId] << ", ";
	cout << "POINT : " << points[userId] << ", ";
	cout << "GRADE : " << GRADE_STR[grade[userId]] << "\n";
}

void ProcessDailyAttendancePoint(std::ifstream& attendaceData) {
	for (int i = 0; i < MAX_ATTENDANCE_RECORD; i++) {
		string sName, attendance_day;
		attendaceData >> sName >> attendance_day;
		try
		{
			ProcessAttendanceRecord(sName, attendance_day);
		}
		catch (...)
		{
			cout << "ProcessDailyAttendancePoint failed with " << sName << " : " << attendance_day << "\n";
			return;
		}
	}
}


void CalcBonusPoint()
{
	for (int i = 1; i <= TotalIdCnt; i++) {
		points[i] += CheckAndGetWedBonus(UserDayAttendanceData[i]);
		points[i] += CheckAndGetWeekendBonus(UserDayAttendanceData[i]);
	}
}

void CalcUserGrade()
{
	for (int i = 1; i <= TotalIdCnt; i++)
	{
		grade[i] = GetUserGrade(points[i]);
	}
}

void PrintAllUserInfo()
{
	for (int i = 1; i <= TotalIdCnt; i++) {
		PrintUserInfo(i);
	}
}

void PrintRemovedPlayer()
{
	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int i = 1; i <= TotalIdCnt; i++) {

		if (grade[i] != 1 && grade[i] != 2 && wed[i] == 0 && weeken[i] == 0) {
			std::cout << names[i] << "\n";
		}
	}
}

void ProcessAttendanceData(std::ifstream& attendaceData) {

	ProcessDailyAttendancePoint(attendaceData);
	CalcBonusPoint();
	CalcUserGrade();
	PrintAllUserInfo();
	PrintRemovedPlayer();
}

void AddNewMember(string attendeeName)
{
	s_mapUserId.insert({ attendeeName, ++TotalIdCnt });
	names[TotalIdCnt] = attendeeName;
}

} // namespace RefAttendance

