#pragma execution_character_set("utf-8")

#include <string>

namespace cq
{
	std::string to_json(std::string msg)
	{
		/* data example
		 *	CQ CODE:
		 *	[CQ:at,qq=2710458198,name=Null]
		 *
		 *	JSON:
		 *	{
		 *		"type": "at",
		 *		"data": {
		 *			"qq": "2710458198",
		 *			"name": "Null“
		 *		}
		 *	}
		 */

		// temp get type
		// temp get data info (all are string)
		// return temp

		std::string temp = "{\"type\":\"";

		int g = 0;
		for (int i = 0; i < msg.size(); i++)
		{
			if (msg[i] == ':')
			{
				while (true)
				{
					i++;
					if (msg[i] == ',') break;
					temp += msg[i];
				}
				temp.append("\",\"data\":{\"");

				while (true)
				{
					i++;
					if (msg[i] == '=')
					{
						temp.append("\":\"");
						while (true)
						{
							i++;
							if (msg[i] == ',')
							{
								i++;
								temp.append("\",\"");
								break;
							}
							else if (msg[i] == ']')
							{
								temp.append("\"}}");
								return temp;
							}
							temp += msg[i];
						}
					}
					temp += msg[i];
				}
			}
		}
	}

	std::string getCQ(std::string msg)
	{
		std::string temp;
		for (int i = msg.find("[CQ:"); i < msg.size(); i++)
		{
			if (msg[i] == ']')
			{
				temp.append("]");
				break;
			}
			temp += msg[i];
		}
		return temp;
	}
}
