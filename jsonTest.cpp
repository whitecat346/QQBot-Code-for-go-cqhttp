#include "lib/hv/json.hpp"
#include <iostream>

int main()
{
    nlohmann::json j = "{\"type\":\"image\",\"data\":{\"file\":\"b46e621a8c5f2ccad9f62fe980c8196e.image\",\"subType\":\"0\",\"url\":\"https://gchat.qpic.cn/gchatpic_new/2073646536/620988360-3154370070-B46E621A8C5F2CCAD9F62FE980C8196E/0?term=2&amp;is_origin=0\"}}";
    std::cout << j.at("type") << std::endl;
    return 0;
}