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

	enum GRADE {
		NORMAL, GOLD, SILVER
	};
	class IGrade {
	public:
		virtual int GetGradePoint() = 0;
		virtual string GetGradeString() = 0;
		virtual int GetGrade() = 0;
		static const string GRADE_STR[3];
		static const int GRADE_POINT[3];
	};

	class NormalGrade : public IGrade {
	public:
		int GetGradePoint() override {
			return GRADE_POINT[GRADE::NORMAL];
		}
		string GetGradeString() override {
			return GRADE_STR[GRADE::NORMAL];
		}
		int GetGrade() {
			return GRADE::NORMAL;
		}
	};

	class SilverGrade : public IGrade {
	public:
		int GetGradePoint() override {
			return GRADE_POINT[GRADE::SILVER];
		}
		string GetGradeString() override {
			return GRADE_STR[GRADE::SILVER];
		}
		int GetGrade() {
			return GRADE::SILVER;
		}
	};

	class GoldGrade : public IGrade {
	public:
		int GetGradePoint() override {
			return GRADE_POINT[GRADE::GOLD];
		}
		string GetGradeString() override {
			return GRADE_STR[GRADE::GOLD];
		}
		int GetGrade() {
			return GRADE::GOLD;
		}
	};

	class GradeFactory {
	public:
		static IGrade* CreateGradeStrategy(int point) {
			if (point >= IGrade::GRADE_POINT[GRADE::GOLD]) {
				return new GoldGrade();
			}
			else if (point >= IGrade::GRADE_POINT[GRADE::SILVER]) {
				return new SilverGrade();
			}
			else {
				return new NormalGrade();
			}
		}
	private:
		GradeFactory() = default;
		~GradeFactory() = default;
	};

	class CalcAttendancePoint {
	public:
		CalcAttendancePoint() = default;
		int GetDayAttendancePoint(std::unordered_map<int, int> weekdayCount) {
			int point = 0;
			for (const auto& pair : weekdayCount) {
				int day = pair.first;
				int count = pair.second;
				point += DAY_ATTANDANCE_POINT[day] * count;
			}
			point += GetAdditiaonalPoint(weekdayCount);
			return point;
		}

	private:
		int GetAdditiaonalPoint(std::unordered_map<int, int> weekdayCount) {
			int additional_point = 0;
			additional_point += CheckAndGetWedBonus(weekdayCount);
			additional_point += CheckAndGetWeekendBonus(weekdayCount);
			return additional_point;
		}

		int CheckAndGetWedBonus(std::unordered_map<int, int> weekdayCount)
		{
			if (weekdayCount[WEEK::WED] >= ATTENDANCE_BONUS_MIN_COUNT) {
				return WED_ADDITIONAL_BOUUS_POINT;
			}
			return 0;
		}

		int CheckAndGetWeekendBonus(std::unordered_map<int, int> weekdayCount)
		{
			if (weekdayCount[WEEK::SAT] + weekdayCount[WEEK::SUN] > ATTENDANCE_BONUS_MIN_COUNT) {
				return WED_ADDITIONAL_BOUUS_POINT;
			}
			return 0;
		}

		const int DAY_ATTANDANCE_POINT[WEEK_COUNT] = { 1, 1, 3, 1, 1, 2, 2 };
		const int ATTENDANCE_BONUS_MIN_COUNT = 10;
		const int WED_ADDITIONAL_BOUUS_POINT = 10;
		const int WEEKEN_ADDITIONAL_BOUUS_POINT = 10;

		const int WED_ATTENDANCE_BOUUS_POINT = 2;
		const int WEEKEND_ATTENDANCE_BOUUS_POINT = 1;
	};


	class User {
	public:
		User() = default;
		User(string attendeeName, int userID)
		{
			name = attendeeName;
			id = userID;
			point = 0;
			for (int i = 0; i < WEEK_COUNT; i++) {
				weekdayCount[i] = 0;
			}
		}
		void UpdateUserGrade() {
			grade = GradeFactory::CreateGradeStrategy(point);
		}
		void AddAttendanceDay(string week) {
			int week_index = GetWeekIndex(week);

			if (week_index < 0 || week_index >= WEEK_COUNT) {
				throw std::exception("invalid week index");
			}
			weekdayCount[week_index]++;
		}
		void CalcUserPoint() {
			CalcAttendancePoint objCalcAttendancePoint;
			point = objCalcAttendancePoint.GetDayAttendancePoint(weekdayCount);
		}
		void PrintUserInfo() {
			cout << "NAME : " << name << ", ";
			cout << "POINT : " << point << ", ";
			cout << "GRADE : " << grade->GetGradeString() << "\n";
		}
		bool IsRemovedUser() {
			if ((grade->GetGrade() == GRADE::NORMAL) && (true == IsEmptyImportantDayAttendance())) {
				return true;
			}
			return false;
		}
		string GetName() {
			return name;
		}

	private:
		bool IsEmptyImportantDayAttendance()
		{
			if ((weekdayCount[WEEK::WED] +
				weekdayCount[WEEK::SAT] +
				weekdayCount[WEEK::SUN]) == 0) {
				return true;
			}
			return false;
		}
		int GetWeekIndex(string week) {
			auto it = weekIndexMap.find(week);
			if (it != weekIndexMap.end()) {
				return it->second;
			}
			return -1;
		}
		string name;
		int id;
		int point;
		IGrade* grade;
		std::unordered_map<int, int> weekdayCount;
	};

	class AttendanceRecord {
	public:
		AttendanceRecord(string attendeeName, string attendance_day)
			: name(attendeeName), day(attendance_day) {
		}
		string name;
		string day;
	};

	class AttendanceManager {
	public:
		AttendanceManager() {
			totalUserCnt = 0;
		};

		void Process(std::ifstream& attendaceData) {
			HandleRecordFile(attendaceData);
			ProcessRecords();
			PrintAllUsers();
			PrintRemovedUsers();
		}

	private:
		void PrintRemovedUsers() {
			cout << "\nRemoved player\n";
			cout << "==============\n";
			for (auto& name : userNames) {
				auto& user = users[name];
				if (user.IsRemovedUser()) {
					std::cout << user.GetName() << "\n";
				}
			}
		}
		void HandleRecordFile(std::ifstream& attendaceData)
		{
			for (int i = 0; i < MAX_ATTENDANCE_RECORD; i++) {
				string sName, attendance_day;
				attendaceData >> sName >> attendance_day;
				if (sName.empty() || attendance_day.empty()) {
					break;
				}
				AttendanceRecord objRecord(sName, attendance_day);
				AddRecord(objRecord);
			}
		}

		void PrintAllUsers() {
			for (auto& name : userNames) {
				auto& user = users[name];
				user.PrintUserInfo();
			}
		}
		void ProcessRecords() {
			for (const auto& record : records) {
				ProcessAttendanceRecord(record.name, record.day);
			}

			for (auto& pair : users) {
				auto& user = pair.second;
				user.CalcUserPoint();
				user.UpdateUserGrade();
			}
		}
		void ProcessAttendanceRecord(string attendeeName, string attendance_day) {

			if (users.find(attendeeName) == users.end()) {
				AddUser(attendeeName);
			}
			auto user = &users[attendeeName];
			user->AddAttendanceDay(attendance_day);
		}
		void AddRecord(AttendanceRecord& record) {
			records.push_back(record);
		}
		int AddUser(string attendeeName) {
			int newId = totalUserCnt;
			userNames.push_back(attendeeName);
			User newUser(attendeeName, newId);
			users.insert({ attendeeName, newUser });
			totalUserCnt++;
			return newId;
		}
		std::unordered_map<string, User> users;
		vector<AttendanceRecord> records;
		vector<string> userNames;	// for print order

		int totalUserCnt;
		const int MAX_ATTENDANCE_RECORD = 500;
	};

} // namespace RefAttendance

