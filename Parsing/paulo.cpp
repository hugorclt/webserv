ConfigParser::Server    selectServ(std::string ip, std::string port, std::string hostName, configParser::data_type vecServs)
{
	//hostName = ip:port -> first occurence find 
	//else check first occurence with all 
   //else check only ip and port 
   //portset = listen.find(ip);
   //portset.find(port);
   //hostName == it->server_name
   configParser::data_type::iterator   firstOccu;
   for (configParser::data_type::iterator it = vecServs.begin(); it != vecServs.end(); it++)
   {
		if (hostName == it->server_name || hostName == ip + ":" + port)
		{
			if (std::set<std::string> portset = listen.find(ip)->second && listen.find(ip) != listen.end())
			{
				if (portset.find(port) != portset.end())
					return (*it);
			}
		}
		else
   	{
			if (std::set<std::string> portset = listen.find(ip)->second && listen.find(ip) != listen.end())
			{
				if (portset.find(port) != portset.end())
				{
					if (!firstOccu)
						firstOccu = it;
				}
			}
	}
   return (*firstOccu);
}