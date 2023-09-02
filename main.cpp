#pragma comment(lib, "hv_static.lib")
#pragma execution_character_set("utf-8")

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include "lib/hv/WebSocketClient.h"
#include "lib/hv/json.hpp"
#include "lib/function/botFunction.hpp"
#include "lib/function/log.hpp"

typedef void(*fun) (std::string* msg);
fun botFunction[] = { Uecho, cave, enable, disable, talkBan, op, caveAdd, caveList, caveEnable, FigurBed, BotInfo };
std::map<std::string, int>  mBotFun;
std::map<std::string, bool> mBotOn;
hv::WebSocketClient ws;
LogPrint log_print;
bool debug = false;

void openSet()
{
	std::ifstream iBotFun("function.json");
	nlohmann::json jBotFun = nlohmann::json::parse(iBotFun);
	iBotFun.close();
	int jSize = jBotFun.at("function").size();
	for (int i = 0; i < jSize; i++)
	{
		mBotFun.insert(std::pair<std::string, int>(jBotFun.at("function").at(i), jBotFun.at("index").at(BotfunOut(jBotFun.at("function").at(i)))));
		mBotOn.insert(std::pair<std::string, bool>(jBotFun.at("function").at(i), jBotFun.at("enable").at(BotfunOut(jBotFun.at("function").at(i)))));
	}
	std::cout << log_print.MSG_INFO("初始化完成!") << std::endl;
}

void OnMessage(const std::string& msg)
{
	nlohmann::json jMsg = nlohmann::json::parse(msg);
	std::string toMsg = msg;

	if (debug == true) std::cout << log_print.MSG_INFO(msg) << std::endl;

	// Message
	if (jMsg.find("post_type") != jMsg.end())
	{
		std::cout << log_print.MSG_OINFO();
		std::cout << "消息类型：" << jMsg.at("post_type") << " 消息时间：" << jMsg.at("time") << std::endl;
		std::string type = jMsg.at("post_type");
		if (type == "message")
		{
			std::cout << log_print.MSG_OINFO();
			int group_id = jMsg.at("group_id");
			unsigned int user_id = jMsg.at("user_id");
			std::cout << "消息ID：" << jMsg.at("message_id") << " 消息发送者ID：" << user_id << " 消息内容：" << jMsg.at("message") << std::endl;
			std::string info = jMsg.at("message");
			if (info[0] == '#')
			{
				std::map<std::string, int>::iterator it = mBotFun.find(msgFun(info));
				if (it == mBotFun.end())
				{
					ws.send("{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
						+ std::to_string(group_id)
						+ ",\"message\":\"[ERROR] 功能未找到！"
						+ "\"}}");
					std::cout << log_print.MSG_INFO("消息已发送！") << std::endl;
					jMsg.clear();
				}
				else
				{
					if (mBotOn[msgFun(info)] == true)
					{
						std::cout << log_print.MSG_INFO("执行函数：") << log_print.BK_GREEN(msgFun(info)) << std::endl;
						botFunction[mBotFun[msgFun(info)]](&toMsg);
						ws.send(toMsg);
						std::cout << log_print.MSG_INFO("消息已发送！") << std::endl;
						jMsg.clear();
					}
					else
					{
						std::cout << log_print.MSG_INFO("函数：" + msgFun(info) + "未启用!") << std::endl;
						ws.send("{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
							+ std::to_string(group_id)
							+ ",\"message\":\"[ERROR] 函数未启用！"
							+ "\"}}");
						std::cout << log_print.MSG_INFO("消息已发送！") << std::endl;
					}
				}
			}
		}
	}
	else if (jMsg.find("status") != jMsg.end()) /*log.MSG_ERROR(BotFunction::jsonS(jMsg.at("status")));*/
	{
		std::string temp = jMsg.at("status");
		if (temp == "ok") std::cout << log_print.MSG_OINFO() << log_print.FC_GREEN("API 调用成功\n");
		else if (temp == "async") std::cout << log_print.MSG_OWARN() << log_print.FC_YELLOW("API 调用已经提交异步处理\n");
		else if (temp == "failed")
		{
			std::string wording = jMsg.at("wording");
			std::cout << log_print.MSG_OFATA() << log_print.FC_READ("API 调用失败! 原因：" + wording + "\n");
		}
		jMsg.clear();
	}
}

void OnOpen()
{
	std::cout << log_print.MSG_INFO("连接成功！") << std::endl;
}

int main()
{
	system("chcp 65001");
	system("cls");

	//Ws Connection Set
	ws.onopen = OnOpen;
	ws.onmessage = OnMessage;
	ws.onclose = [&]()
		{
			std::cout << log_print.MSG_FATA("连接关闭或者未找到！");
			ws.close();
			exit(0);
		};
	reconn_setting_t reconn;
	reconn.min_delay = 1000;
	reconn.max_delay = 10000;
	reconn.delay_policy = 2;
	ws.setReconnect(&reconn);

	std::cout << log_print.MSG_INFO("Please Input Port: ");
	std::string temp;
	std::cin >> temp;
	std::cout << std::endl;
	int itemp = 0;
	try
	{
		itemp = std::stoi(temp);
		if (itemp <= 0 || itemp >= 65536)
		{
			//std::cout << "Illegal scope!" << std::endl;
			std::cout << log_print.MSG_WARN("Illegal scope!");
			return -1;
		}
	}
	catch (std::invalid_argument const& ex)
	{
		ex;
		//std::cout << "Number please!" << std::endl;
		std::cout << log_print.MSG_WARN("Number please!");
		return -1;
	}
	catch (std::out_of_range const& ex)
	{
		ex;
		//std::cout << "Out Of Range!" << std::endl;
		std::cout << log_print.MSG_WARN("Out Of Range!");
		return -1;
	}

	openSet();

	ws.open(std::string("ws://127.0.0.1:" + std::to_string(itemp)).c_str());
	
	while (true);
	return 0;
}
