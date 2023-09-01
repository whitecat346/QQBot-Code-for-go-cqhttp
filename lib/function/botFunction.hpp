#pragma execution_character_set("utf-8")

#include <string>
#include <fstream>
#include <random>
#include <Windows.h>
#include "json.hpp"
#include "cq.hpp"

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

//������Ϣ������  2023.8.20 ����INT��STRING��BOOL���ж����⣬��������
/*

{"action":"send_group_msg","params":{"group_id":114514,"message":"Hello World!"}}

{"action":"send_group_msg","params":{"group_id":114514,"message":"Hello World!"},"echo":"Test"}

*/
//std::string requestMsg(std::string action, std::string params, std::string echo)
//{
//	std::string out = "\{\"action\":\"" + action + "\"" , temp;
//	if (params.size() != 0)
//	{
//		out.append(",\"params\":\{");
//		for (int i = 0; i < params.size(); i++)
//		{
//			if (params[i] != ':' || params[i] != ' ')
//				temp[i] = params[i];
//			else if (params[i] == ':')
//			{
//				out.append("\"" + temp + "\":");
//				temp.clear();
//			}
//			else if (params[i] == ' ')
//			{
//				for (int a = 0; a < temp.size(); a++)
//				{
//					if (48 <= temp[a] <= 57) out.append(temp + ",");
//					else out.append("\"" + temp + "\",");
//				}
//				temp.clear();
//			}
//		}
//		out.append("\"" + temp + "\"\}");
//	}
//	if (echo.size() != 0) out.append(",\"echo\":\"" + echo + "\"}");
//	return out;
//}

void Uecho(std::string* msg)
{
	nlohmann::json jmsg = nlohmann::json::parse(msg);
	std::string UEtemp, funStr = jmsg.at("message");
	for (int i = 0; i < funStr.size(); i++)
	{
		if (funStr.at(i) == ' ')
		{
			for (int a = i++; a < funStr.size(); a++)
				UEtemp += funStr.at(a);
			break;
		}
	}
	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
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

void cave(std::string* msg)
{
	nlohmann::json jmsg = nlohmann::json::parse(msg);

	if (openCave == false)
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] ��ȴʱ����10�룡����"
			+ "\"}}";
		return;
	}

	std::ifstream iCave("cave.json");
	/*nlohmann::json jCave = nlohmann::json::parse(iCave);*/
	nlohmann::json jCaveCN = nlohmann::json::parse(iCave);
	iCave.close();
	int max = jCaveCN.size();

	std::random_device seed;//Ӳ���������������
	std::ranlux48 engine(seed());//���������������������
	std::uniform_int_distribution<> distrib(0, max);//�����������Χ����Ϊ���ȷֲ�
	int random = distrib(engine);

	std::string CAtemp = "��������� ������"
		+ std::to_string(random)
		+ "��\n"
		+ BotfunOut(jCaveCN.at(random).at("content"))
		+ "\n����  "
		+ BotfunOut(jCaveCN.at(random).at("author"));

	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":" + BotfunOut(jmsg.at("group_id")) + ",\"message\":\"" + CAtemp + "\"}}";
	timeO();
}

void enable(std::string* msg)
{
	nlohmann::json jmsg = nlohmann::json::parse(msg);
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
	if (MFtemp == "enable" || MFtemp == "disable")
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] �޷����û�ر�������ܣ�"
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
			if (jBF.at("enable").at(MFtemp) == true) *msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ BotfunOut(jmsg.at("group_id"))
				+ ",\"message\":\"[ERROR] �ù��������ã�"
				+ "\"}}";
			else jBF.at("enable").at(MFtemp) = true;
		}
		else *msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] δ�ҵ��˹��ܣ�"
			+ "\"}}";
	}
	else *msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"[ERROR] ��û��Ȩ��ʹ�øù��ܣ�"
		+ "\"}}";
}

void disable(std::string* msg)
{
	nlohmann::json jmsg = nlohmann::json::parse(msg);
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
	if (MFtemp == "enable" || MFtemp == "disable")
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] �޷����û�ر�������ܣ�"
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
			if (jBF.at("enable").at(MFtemp) = false) *msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ BotfunOut(jmsg.at("group_id"))
				+ ",\"message\":\"[ERROR] �ù����ѱ����ã�"
				+ "\"}}";
			else
			{
				jBF.at("enable").at(MFtemp) = false;
				*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
					+ BotfunOut(jmsg.at("group_id"))
					+ ",\"message\":\"[ERROR] ���óɹ���"
					+ "\"}}";
			}
		}
		else *msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] δ�ҵ��˹��ܣ�"
			+ "\"}}";
	}
	else *msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"[ERROR] ��û��Ȩ��ʹ�øù��ܣ�"
		+ "\"}}";
}

void op(std::string* msg)
{
	nlohmann::json jmsg = nlohmann::json::parse(msg);
	unsigned int user_id = jmsg.at("user_id");
	if (user_id != 2710458198 || user_id != 256626981)
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] ��û��Ȩ��ʹ�øù��ܣ�"
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
					*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
						+ BotfunOut(jmsg.at("group_id"))
						+ ",\"message\":\"[ERROR] �����������ַ���"
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

	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"[INFO] ��ӳɹ���"
		+ "\"}}";
}

void talkBan(std::string* msg)
{
	nlohmann::json jmsg = nlohmann::json::parse(msg);
	std::string TBmsg = jmsg.at("message"), role = jmsg.at("sender").at("role");

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

		*msg = "{\"action\":\"set_group_ban\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"user_id\":" + std::to_string(id)
			+ ",\"duration\":" + std::to_string(itime)
			+ "}}";
	}
	else
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] ��û��Ȩ��ʹ�øù��ܣ�\""
			+ "}}";
		return;
	}
}

void caveAdd(std::string* msg)
{
	// example:
	/*{
		"author": "-",
		"content" : "��������PCL2������!",
		"from" : "������"
	},
	{
		"author": "-",
		"content" : "���շŸ����ݾƣ���è����û���",
		"from" : "������"
	},*/

	std::fstream ifun("function.json");
	nlohmann::json jfun = nlohmann::json::parse(ifun), jmsg = nlohmann::json::parse(msg);
	ifun.close();

	bool white_list = false;
	for (int g = 0; g < jfun.at("white_list").size(); ++g)
		if (jfun.at("user_id") == jfun.at("white_list").at(g))
			white_list = true;
	if (white_list == false)
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"[ERROR] ��û��Ȩ��ʹ�øù��ܣ�\""
		+ "}}";
		return;
	}

	jmsg.clear();
	ifun.open("wc.json");
	std::string swc;
	ifun >> swc;

	std::string temp;
	for (int i = msg->find(' ') + 1; i < msg->size(); ++i)
		temp += msg->at(i);
	std::string msginfo = "{\"author\": \"" + BotfunOut(jmsg.at("sender").at("nickname")) + "\",\"content\": \"" + temp + "\"}";

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
	jmsg = nlohmann::json::parse(ifun);
	ifun.close();
	int num = jmsg.size();

	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"[INFO] ��ӳɹ���(ID: " + std::to_string(num) + ")\""
		+ "}}";
}

void caveList(std::string* msg)
{
	std::fstream ifun("function.json");
	nlohmann::json jfun = nlohmann::json::parse(ifun), jmsg = nlohmann::json::parse(msg);
	ifun.close();

	bool white_list = false;
	for (int g = 0; g < jfun.at("white_list").size(); ++g)
		if (jfun.at("user_id") == jfun.at("white_list").at(g))
			white_list = true;
	if (white_list == false)
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] ��û��Ȩ��ʹ�øù��ܣ�\""
			+ "}}";
		return;
	}

	/*
	 *[ID] 0
	 *Hello World!
	 *���� Somebody
	 *
	 *[ID] 1
	 *Test
	 *���� Somebody
	 */

	// for:  temp [ID] + i & \n + ����  + author \n\n

	ifun.open("wc.json");
	nlohmann::json jwc = nlohmann::json::parse(ifun);
	ifun.close();

	std::string temp;
	for (int i = 0; i > jwc.size(); ++i)
		temp.append("[ID] " + std::to_string(i) + "\n" + "���� " + BotfunOut(jwc.at(i).at("author")) + "\n\n");

	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"" + temp + "\""
		+ "}}";
}

void caveEnable(std::string* msg)
{
	std::fstream ifun("function.json");
	nlohmann::json jfun = nlohmann::json::parse(ifun), jmsg = nlohmann::json::parse(msg);
	ifun.close();

	bool white_list = false;
	for (int g = 0; g < jfun.at("white_list").size(); ++g)
		if (jfun.at("user_id") == jfun.at("white_list").at(g))
			white_list = true;
	if (white_list == false)
	{
		*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
			+ BotfunOut(jmsg.at("group_id"))
			+ ",\"message\":\"[ERROR] ��û��Ȩ��ʹ�øù��ܣ�\""
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
			*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
				+ BotfunOut(jmsg.at("group_id"))
				+ ",\"message\":\"[ERROR] �����������ַ���\""
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

	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"[INFO] �����ɣ�\"}}";
}

void FigureBed(std::string* msg)
{
	// Image CQ Code:
	// [CQ:image,file=7963d97b656bafd6e3687514b576b035.image,subType=0,
	// url=https://gchat.qpic.cn/gchatpic_new/2710458198/620988360-2387791336-7963D97B656BAFD6E3687514B576B035/0?term=2&amp;is_origin=1]

	// get cq JSON

	std::string msginfo;
	for (int i = msg->find(' ') + 1; i < msg->size(); i++)
		msginfo += msg->at(i);

	nlohmann::json jcq = nlohmann::json::parse(cq::to_json(cq::getCQ(msginfo))), jmsg = nlohmann::json::parse(msg);

	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"" + BotfunOut(jcq.at("url")) + "\"}}";
}

void BotInfo(std::string* msg)
{
	// ifstream function.json >> json
	// temp += for get info of function
	// *msg

	std::ifstream ifun("fucntion.json");
	nlohmann::json jfun = nlohmann::json::parse(ifun), jmsg = nlohmann::json::parse(msg);
	ifun.close();

	/*
	 *Function List:
	 *Uecho
	 *cave .etc
	 */
	std::string temp = "Function List:";
	for (int i = 0; i < jfun.at("function").size(); i++)
		temp.append("\n" + BotfunOut(jfun.at("function").at(i)));

	*msg = "{\"action\":\"send_group_msg\",\"params\":{\"group_id\":"
		+ BotfunOut(jmsg.at("group_id"))
		+ ",\"message\":\"" + temp + "\"}}";
}
