#pragma execution_character_set("utf-8")

#include <string>
#include <fstream>
#include <random>
#include "json.hpp"
#include "cq.hpp"

// Set
std::ifstream iCave("cave.json");
/*nlohmann::json jCave = nlohmann::json::parse(iCave);*/
nlohmann::json jCaveCN = nlohmann::json::parse(iCave);

std::string msgFun(std::string MFmsg)
{
	MFmsg.erase(MFmsg.begin());
	std::string MFtemp;
	for (int i = 0; i < MFmsg.size(); i++)
	{
		if (MFmsg.at(i) == ' ') break;
		MFtemp += MFmsg[i];
	}
	return MFtemp;
}

std::string jsonS(std::string in)
{
	in.erase(in.begin());
	in.erase(in.end() - 1);
	return in;
}

std::string BotfunOut(std::string in) { return in; }

void Uecho(std::string* Botmsg)
{
	nlohmann::json jmsg = nlohmann::json::parse(*Botmsg);

	std::string UEtemp, funStr = jmsg.at("message");
	for (int i = funStr.find(' ') + 1; i < funStr.size(); i++)
		UEtemp += funStr.at(i);

	int group_id = jmsg.at("group_id");
	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[ECHO] "
		+ UEtemp
		+ "\"}}";
}

bool openCave = true;
void timeO()
{
	openCave = false;
	Sleep(10000);
	openCave = true;
}

void cave(std::string* Botmsg)
{
	nlohmann::json jmsg = nlohmann::json::parse(*Botmsg);

	if (openCave == false)
	{
		int group_id = jmsg.at("group_id");
		*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 冷却时间是10秒！！！"
			+ "\"}}";
		return;
	}

	int max = jCaveCN.size();
	std::random_device seed;//硬件生成随机数种子
	std::ranlux48 engine(seed());//利用种子生成随机数引擎
	std::uniform_int_distribution<> distrib(0, max);//设置随机数范围，并为均匀分布
	int random = distrib(engine);

	std::string CAtemp = "盗版回声洞 ——（"
		+ std::to_string(random)
		+ "）\n"
		+ BotfunOut(jCaveCN.at(random).at("content"))
		+ "\n——  "
		+ BotfunOut(jCaveCN.at(random).at("author"));

	int group_id = jmsg.at("group_id");
	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":" + std::to_string(group_id) + ",\"message\":\"" + CAtemp + "\"}}";
	timeO();
}

void enable(std::string* Botmsg)
{
	nlohmann::json jmsg = nlohmann::json::parse(*Botmsg);
	std::string MFtemp, EAmsg = jmsg.at("message");
	for (int i = 0; i < EAmsg.size(); i++)
	{
		if (EAmsg.at(i) == ' ')
		{
			for (int a = i++; a < EAmsg.size(); a++)
				MFtemp += EAmsg.at(a);
			MFtemp.erase(MFtemp.begin());
			break;
		}
	}
	int group_id = jmsg.at("group_id");
	if (MFtemp == "enable" || MFtemp == "disable")
	{
		*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 无法启用或关闭这个功能！"
			+ "\"}}";
		return;
	}

	std::ifstream iBF("botFun.json");
	nlohmann::json jBF = nlohmann::json::parse(iBF);
	unsigned int user_id = jmsg.at("user_id");
	bool wFind = false;

	for (int i = 0; i < jBF.at("white_list").size(); i++)
		if (jBF.at("white_list").at(i) == user_id) wFind = true;

	if (wFind == true)
	{
		if (jBF.at("enable").contains(MFtemp))
		{
			if (jBF.at("enable").at(MFtemp) == true) *Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ std::to_string(group_id)
				+ ",\"message\":\"[ERROR] 该功能已启用！"
				+ "\"}}";
			else jBF.at("enable").at(MFtemp) = true;
		}
		else *Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 未找到此功能！"
			+ "\"}}";
	}
	else *Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[ERROR] 你没有权限使用该功能！"
		+ "\"}}";
}

void disable(std::string* Botmsg)
{
	
	nlohmann::json jmsg = nlohmann::json::parse(*Botmsg);
	std::string MFtemp, DAmsg = jmsg.at("message");
	for (int i = 0; i < DAmsg.size(); i++)
	{
		if (DAmsg.at(i) == ' ')
		{
			for (int a = i++; a < DAmsg.size(); a++)
				MFtemp += DAmsg.at(a);
			MFtemp.erase(MFtemp.begin());
			break;
		}
	}
	int group_id = jmsg.at("group_id");
	if (MFtemp == "enable" || MFtemp == "disable")
	{
		*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 无法启用或关闭这个功能！"
			+ "\"}}";
		return;
	}

	std::ifstream iBF("botFun.json");
	nlohmann::json jBF = nlohmann::json::parse(iBF);
	unsigned int user_id = jmsg.at("user_id");
	bool wFind = false;

	for (int i = 0; i < jBF.at("white_list").size(); i++)
		if (jBF.at("white_list").at(i) == user_id) wFind = true;
	if (wFind == true)
	{
		if (jBF.at("enable").contains(MFtemp))
		{
			if (jBF.at("enable").at(MFtemp) = false) *Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ std::to_string(group_id)
				+ ",\"message\":\"[ERROR] 该功能已被禁用！"
				+ "\"}}";
			else
			{
				jBF.at("enable").at(MFtemp) = false;
				*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
					+ std::to_string(group_id)
					+ ",\"message\":\"[ERROR] 禁用成功！"
					+ "\"}}";
			}
		}
		else *Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 未找到此功能！"
			+ "\"}}";
	}
	else *Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[ERROR] 你没有权限使用该功能！"
		+ "\"}}";
}

void op(std::string* Botmsg)
{
	
	nlohmann::json jmsg = nlohmann::json::parse(*Botmsg);
	unsigned int user_id = jmsg.at("user_id");
	int group_id = jmsg.at("group_id");
	if (user_id != 2710458198 || user_id != 256626981)
	{
		*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 你没有权限使用该功能！"
			+ "\"}}";
		return;
	}

	std::fstream iOP("botFun.json");
	nlohmann::json jOP = nlohmann::json::parse(iOP);

	std::string str, OPmsg = jmsg.at("message");

	/*for (int i = 0; i < OPmsg.size(); i++)
	{
		if (OPmsg.at(i) == ' ')
		{
			for (int a = 0; a < OPmsg.size(); a++)
			{
				if (isdigit(OPmsg.at(a))) str += OPmsg.at(a);
				else
				{
					**Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
						+ std::to_string(group_id)
						+ ",\"message\":\"[ERROR] 包含非数字字符！"
						+ "\"}}";
					break;
				}
			}
		}
	}*/

	std::string cq, qqId, time;
	for (int i = OPmsg.find(' ') + 1; i < OPmsg.size(); ++i)
	{
		if (OPmsg.at(i) == ' ')
		{
			while (true)
			{
				++i;
				if (i >= OPmsg.size()) break;
				time += OPmsg.at(i);
			}
			break;
		}
		cq += OPmsg.at(i);
	}

	for (int i = cq.find('qq=') + 1; i < cq.size(); ++i) qqId += cq[i];
	qqId.erase(qqId.end() - 1);

	std::istringstream sstr(qqId);
	unsigned id = 0;
	sstr >> id;

	jOP.at("white_list") += id;
	iOP << jOP;
	iOP.close();

	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[INFO] 添加成功！"
		+ "\"}}";
}

void talkBan(std::string* Botmsg)
{
	
	nlohmann::json jmsg = nlohmann::json::parse(*Botmsg);
	std::string TBmsg = jmsg.at("message"), role = jmsg.at("sender").at("role");
	int group_id = jmsg.at("group_id");

	if (role == "owner" || role == "admin")
	{
		// #talkBan [CQ:at,qq=1332960146] 1
		// size 32  at 31

		std::string cq, qqId, time;
		for (int i = TBmsg.find(' ') + 1; i < TBmsg.size(); ++i)
		{
			if (TBmsg.at(i) == ' ')
			{
				while (true)
				{
					++i;
					if (i >= TBmsg.size()) break;
					time += TBmsg.at(i);
				}
				break;
			}
			cq += TBmsg.at(i);
		}

		for (int i = cq.find('qq=') + 1; i < cq.size(); ++i) qqId += cq[i];
		qqId.erase(qqId.end() - 1);

		std::istringstream sstr(qqId);
		unsigned id = 0, itime = 0;

		sstr >> id;
		sstr.clear();

		std::istringstream sstri(time);
		sstri >> itime;
		itime *= 60;

		*Botmsg = "{\"action\":\"set_group_ban\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"user_id\":" + std::to_string(id)
			+ ",\"duration\":" + std::to_string(itime)
			+ "}}";
	}
	else
	{
		*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 你没有权限使用该功能！\""
			+ "}}";
		return;
	}
}

void caveAdd(std::string* Botmsg)
{
	// example:
	/*
	{
		"author": "-",
		"content" : "快来试试PCL2下载器!",
		"from" : "回声洞"
	},
	{
		"author": "-",
		"content" : "白日放鸽须纵酒，龙猫作伴好还乡",
		"from" : "回声洞"
	}
	*/

	nlohmann::json jmsg = nlohmann::json::parse(*Botmsg);
	std::string message = jmsg.at("message");
	int group_id = jmsg.at("group_id");

	std::fstream ifun("wc.json");
	std::string swc, temp;
	ifun >> swc;

	for (int i = message.find(' ') + 1; i < message.size(); i++)
		temp += message.at(i);
	std::string msginfo = "{\"author\": \""
						+ BotfunOut(jmsg.at("sender").at("nickname"))
						+ "\",\n\"content\": \""
						+ temp + "\"\n}";

	// [] size 2 at 1
	// size -1 same string.end  |  size -2 same lase_second char

	// when swc at(size -2) is [ :  swc eraser end & add msginfo then add ]
	// when swc at(size -2) is } :  swc.end change to , & add msginfo then add ]
	if (swc.at(swc.size() - 2) == '[')
	{
		swc.erase(swc.end() - 1);
		swc.append(msginfo + "]");
	}
	else
	{
		swc.at(swc.size() - 1) = ',';
		swc.append(msginfo + "]");
	}

	ifun << swc;
	jmsg.clear();
	jmsg = nlohmann::json::parse(ifun);
	ifun.close();
	int num = jmsg.size();

	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[INFO] 添加成功！(ID: " + std::to_string(num) + ")\""
		+ "}}";
}

void caveList(std::string* Botmsg)
{
	std::fstream ifun("function.json");
	
	nlohmann::json jfun = nlohmann::json::parse(ifun), jmsg = nlohmann::json::parse(*Botmsg);
	ifun.close();

	int group_id = jmsg.at("group_id");
	unsigned int user_id = jmsg.at("user_id");
	for (int i = 0; i < jfun.at("white_list").size(); ++i)
		if (user_id == jfun.at("white_list").at(i))
		{
			/*
			 *[ID] 0
			 *Hello World!
			 *—— Somebody
			 *
			 *[ID] 1
			 *Test
			 *—— Somebody
			 */

			 // for:  temp [ID] + i & \n + ——  + author \n\n

			jfun.clear();
			ifun.open("wc.json");
			jfun = nlohmann::json::parse(ifun);
			ifun.close();

			std::string temp;
			int jsonSize = jfun.size();
			for (int u = 0; u < jsonSize; u++)
				temp.append("[ID] " + std::to_string(u) 
					+ "\n" + BotfunOut(jfun.at(u).at("content")) 
					+ "\n—— " + BotfunOut(jfun.at(u).at("author")) 
					+ "\n");

			if (temp.size() == 0)
			{
				*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ std::to_string(group_id)
				+ ",\"message\":\"[ERROR] 等待列表内无消息！\""
				+ "}}";
				return;
			}
				
			*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ std::to_string(group_id)
				+ ",\"message\":\"" + temp + "\""
				+ "}}";
			return;
		}
	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[ERROR] 你没有权限使用该功能！\""
		+ "}}";
}

void caveEnable(std::string* Botmsg)
{
	std::fstream ifun("function.json");
	
	nlohmann::json jfun = nlohmann::json::parse(ifun), jmsg = nlohmann::json::parse(*Botmsg);
	ifun.close();

	bool white_list = false;
	for (int g = 0; g < jfun.at("white_list").size(); ++g)
		if (jfun.at("user_id") == jfun.at("white_list").at(g))
			white_list = true;
	int group_id = jmsg.at("group_id");
	if (white_list == false)
	{
		*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ std::to_string(group_id)
			+ ",\"message\":\"[ERROR] 你没有权限使用该功能！\""
			+ "}}";
		return;
	}

	// get id
	// temp = jwc.at(id)
	// jwc.at(id) = ' '
	// cave.json add temp
	// wc.json eraser temp/id

	std::string msginfo = jmsg.at("message"), temp;
	for (int i = msginfo.find(' ') + 1; i < msginfo.size(); ++i)
	{
		if (48 <= (int)msginfo.at(i) <= 57)
		{
			*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ std::to_string(group_id)
				+ ",\"message\":\"[ERROR] 包含非数字字符！\""
				+ "}}";
			return;
		}
		temp += msginfo[i];
	}

	std::istringstream isstr(temp);
	int id = 0;
	isstr >> id;
	isstr.clear();

	std::fstream fwc("wc.json");
	nlohmann::json jwc = nlohmann::json::parse(fwc);
	temp = jwc.at(id);
	jwc.erase(id);
	fwc << jwc;
	fwc.close();
	jwc.clear();

	fwc.open("cave.json");
	msginfo.clear();
	fwc >> msginfo;
	msginfo.at(msginfo.size() - 1) = ',';
	msginfo.append(temp + "]");

	fwc << msginfo;
	fwc.close();

	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[INFO] 添加完成！\"}}";
}

void FigurBed(std::string* Botmsg)
{
	// Image CQ Code:
	// [CQ:image,file=7963d97b656bafd6e3687514b576b035.image,subType=0,
	// url=https://gchat.qpic.cn/gchatpic_new/2710458198/620988360-2387791336-7963D97B656BAFD6E3687514B576B035/0?term=2&amp;is_origin=1]

	// get cq JSON

	std::string msginfo;
	for (int i = Botmsg->find(' ') + 1; i < Botmsg->size(); i++)
		msginfo += Botmsg->at(i);

	nlohmann::json jcq = nlohmann::json::parse(cq::to_json(cq::getCQ(msginfo))), jmsg = nlohmann::json::parse(*Botmsg);

	int group_id = jmsg.at("group_id");
	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"[QQ图床] 图床URL：" + BotfunOut(jcq.at("data").at("url")) + "\"}}";
}

void BotInfo(std::string* Botmsg)
{
	// ifstream function.json >> json
	// temp += for get info of function
	// *Botmsg

	std::ifstream ifun("function.json");
	nlohmann::json jfun = nlohmann::json::parse(ifun),jmsg = nlohmann::json::parse(*Botmsg);
	ifun.close();

	/*
	 *Function List:
	 *Uecho
	 *cave .etc
	 */
	std::string temp = "Function List:";
	for (int i = 0; i < jfun.at("function").size(); i++)
		temp.append("\n" + BotfunOut(jfun.at("function").at(i)));

	int group_id = jmsg.at("group_id");
	*Botmsg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ std::to_string(group_id)
		+ ",\"message\":\"" + temp + "\"}}";
}
