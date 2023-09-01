#pragma execution_character_set("utf-8")

#include <string>
#include <chrono>

std::string SysTime()
{
#pragma warning(disable:4996)
	auto now = std::chrono::system_clock::now();
	uint64_t dis_millseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
		- std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
	time_t tt = std::chrono::system_clock::to_time_t(now);
	tm* time_tm = localtime(&tt);
	char strTime[35] = { 0 };
	const char temp1[] = "[%d-%02d-%02d %02d:%02d:%02d] ";
	sprintf_s(strTime, temp1, time_tm->tm_year + 1900, time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour, time_tm->tm_min, time_tm->tm_sec);
	return strTime;
}

class LogPrint
{
public:
	std::string BK_BLACK(std::string log_msg) { return "\033[40m" + log_msg + "\033[0m"; }

	std::string BK_DREAD(std::string log_msg) { return "\033[41m" + log_msg + "\033[0m"; }

	std::string BK_GREEN(std::string log_msg) { return "\033[42m" + log_msg + "\033[0m"; }

	std::string BK_YELLOW(std::string log_msg) { return "\033[43m" + log_msg + "\033[0m"; }

	std::string BK_BLUE(std::string log_msg) { return "\033[44m" + log_msg + "\033[0m"; }

	std::string BK_PURPLE(std::string log_msg) { return "\033[45m" + log_msg + "\033[0m"; }

	std::string BK_DGREEN(std::string log_msg) { return "\033[46m" + log_msg + "\033[0m"; }

	std::string BK_WHITE(std::string log_msg) { return "\033[47m" + log_msg + "\033[0m"; }

	std::string FC_BLACK(std::string log_msg) { return "\033[30m" + log_msg + "\033[0m"; }

	std::string FC_READ(std::string log_msg) { return "\033[31m" + log_msg + "\033[0m"; }

	std::string FC_GREEN(std::string log_msg) { return "\033[32m" + log_msg + "\033[0m"; }

	std::string FC_YELLOW(std::string log_msg) { return "\033[33m" + log_msg + "\033[0m"; }

	std::string FC_BLUE(std::string log_msg) { return "\033[34m" + log_msg + "\033[0m"; }

	std::string FC_PURPLE(std::string log_msg) { return "\033[35m" + log_msg + "\033[0m"; }

	std::string FC_DGREEN(std::string log_msg) { return "\033[36m" + log_msg + "\033[0m"; }

	std::string FC_WHITE(std::string log_msg) { return "\033[37m" + log_msg + "\033[0m"; }

	std::string MSG_ERROR(std::string log_msg) { return (SysTime() + "\033[31m" + "[ERRO]\033[0m " + log_msg); }

	std::string MSG_INFO(std::string log_msg) { return (SysTime() + "\033[32m" + +"[INFO]\033[0m " + log_msg); }

	std::string MSG_WARN(std::string log_msg) { return (SysTime() + "\033[33m" + "[WARN]\033[0m " + log_msg); }

	std::string MSG_FATA(std::string log_msg) { return (SysTime() + "\033[41;37m" + "[FATA]\033[0m " + log_msg); }

	std::string MSG_OERROR() { return  SysTime() + "\033[31m[ERRO]\033[0m "; }

	std::string MSG_OINFO() { return  SysTime() + "\033[32m[INFO]\033[0m "; }

	std::string MSG_OWARN() { return  SysTime() + "\033[33m[WARN]\033[0m "; }

	std::string MSG_OFATA() { return  SysTime() + "\033[41;37m[FATA]\033[0m "; }
};
