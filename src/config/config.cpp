/*
 * Bomb-her-man
 * Copyright (C) Sardem FF7 2010 <sardemff7.pub@gmail.com>
 * 
 * Bomb-her-man is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Bomb-her-man is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bombherman.hpp"
#include "config.hpp"

using namespace bombherman;

/*
 * Instance space
 */

Config::Config() :
	iValue(0),
	sValue("")
{
}

Config &Config::operator=(std::string value)
{
	iValue = 0;
	sValue = value;
	
	return (*this);
}

Config &Config::operator=(int value)
{
	iValue = value;
	
	std::ostringstream s("");
	s << iValue;
	sValue = s.str();
	
	return (*this);
}

int
Config::intValue()
{
	if ( ( ! sValue.empty() ) && ( iValue == 0 ) )
	{
		std::istringstream s(sValue);
		s >> iValue;
	}
	return iValue;
}

/*
 * Class space
 */

std::map<std::string, Config> Config::config = std::map<std::string, Config>();

void
Config::init()
{
	if ( config.empty() )
	{
		/*
		 * Initializing defaults values
		 */
		Config::config["mapSize"] = 15;
		Config::config["defaultPlantableBombs"] =3;
		Config::config["defaultRange"] = 5;
		
		/*
		 * Then read the file
		 */
		read();
	}
}

void
Config::read()
{
	std::fstream file("/tmp/config.ini", std::ios::in); //TODO: dynamic file
	std::string line;
	std::string::size_type separator;
	
	do
	{
		file >> line;
		std::cout << line << std::endl;
		separator = line.find_first_of('=');
		if ( separator != std::string::npos )
		{
			std::cout << line.substr(0, separator) << '=' << line.substr(separator+1) << std::endl;
			Config::config[line.substr(0, separator)] = line.substr(separator+1);
		}
	} while ( ! file.eof() );
	
	file.close();
}

void
Config::write()
{
	std::fstream file("/tmp/config.ini", std::ios::out); //TODO: dynamic file
	
	for ( std::map<std::string, Config>::iterator i = Config::config.begin(), e = Config::config.end() ; i != e ; ++i )
	{
		if ( ! (i->second).sValue.empty() )
			file << i->first << '=' << (i->second).sValue << std::endl;
	}
	file.close();
}

std::string
Config::get(std::string key)
{
	Config::init();
	return Config::config[key].sValue;
}

int
Config::getInt (std::string key)
{
	Config::init();
	return (Config::config[key]).intValue();
}

void
Config::set(std::string key, int value)
{
	Config::init();
	Config::config[key] = value;
}

void
Config::set(std::string key, std::string value)
{
	Config::init();
	Config::config[key] = value;
}

