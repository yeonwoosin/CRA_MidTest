#include "gmock/gmock.h"
#include "attendance.cpp"
#include "ref_attendance.h"
#include <sstream>
#include <iostream>

using std::cout;

static const string attendanceDataFile = "attendance_weekday_500.txt";

TEST(TS1, TC1)
{
	// Redirect cout to a stringstream to capture the output
	std::stringstream OriginalBuffer;
	std::stringstream RefactoringBuffer;

	// bakckup old buffer and redirect std::cout to our stringstream buffer
	std::streambuf* oldCout = cout.rdbuf(OriginalBuffer.rdbuf());


	// Call original version
	input();

	// switch to refactored version
	cout.rdbuf(RefactoringBuffer.rdbuf());

	// Call refactored version
	std::ifstream attendaceData{ attendanceDataFile }; //500개 데이터 입력
	RefAttendance::AttendanceManager objManager;
	objManager.Process(attendaceData);

	// restore old buffer
	cout.rdbuf(oldCout);

	// Get the output original and refactoring version
	string output = OriginalBuffer.str();
	cout << output << "\n";

	output = RefactoringBuffer.str();
	cout << output << "\n";

	// Compare the outputs
	EXPECT_EQ(OriginalBuffer.str(), RefactoringBuffer.str());
}