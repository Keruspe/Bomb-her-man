/*
 * Bomb-her-man
 * Copyright (C) Sardem FF7 2010 <sardemff7.pub@gmail.com>
 * Copyright (C) Marc-Antoine Perennou 2010 <Marc-Antoine@Perennou.com>
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

// TODO DYNAMIC FILE FOR WINDOWS
#ifdef __MINGW32__
	#define CONFIG_FILE (std::string(getenv("AppData")) + std::string("/bomb-her-man/config.init")).c_str()
#else
	#include <cstdlib>
	#define CONFIG_FILE (std::string(getenv("HOME")) + std::string("/.config/bomb-her-man/config")).c_str()
#endif // __MINGW32__

using namespace bombherman;

/*
 * Instance space
 */

Config::Config() :
	iValue(0),
	sValue("")
{
}

// Both those operator= return a reference to stock the Config in the map
// Note that sValue and intValue are always synchronised

Config & Config::operator=(std::string value)
{
	// This is used for a string configuration
	this->sValue = value;
	this->intValue();
	
	return (*this);
}

Config &Config::operator=(int value)
{
	// This is used for an int configuration
	this->iValue = value;
	this->stringValue();
	
	return (*this);
}

void
Config::intValue()
{
	// Try to transform the stringValue to an int
	std::istringstream s(this->sValue);
	s >> this->iValue;
}

void
Config::stringValue()
{
	// Transform the int to a string
	std::ostringstream s("");
	s << this->iValue;
	this->sValue = s.str();
}

/*
 * Class space
 */

// Initialize statics
std::map<std::string, Config> Config::config;
bool Config::isInit = false;

void
Config::init()
{
	if ( Config::isInit )
		// Don't initialize twice
		return;
	
	/*
	 * Initializing defaults values
	 */
	config["mapSize"] = 15;
	
	config["maxPlayers"] = 2;
	config["maxMaps"] = 10;
	
	config["defaultPlantableBombs"] = 3;
	config["defaultRange"] = 2;
	
	config["mgInsertionProbabilityBase"] = 100;
	config["mgInsertionProbabilityBaseHorizontal"] = 50;
	config["mgInsertionProbabilityBaseVertical"] = 50;
	config["mgInsertionProbabilityRegressionHorizontal"] = 10;
	config["mgInsertionProbabilityRegressionVertical"] = 10;
	config["mgInsertionProbabilityBarrel"] = 42 * 2;
	config["mgInsertionElementSizeMaxHorizontal"] = 3;
	config["mgInsertionElementSizeMaxVertical"] = 3;
	
	config["nbAIs"] = 0;
	
	config["bonusApparitionProbability"] = 100 - config["mgInsertionProbabilityBarrel"].iValue;
	config["rangeVariation"] = config["mapSize"].iValue / 10;
	config["maxRange"] = config["mapSize"].iValue / 3;
	config["minRange"] = 1;
	config["capacityVariation"] = 1;
	config["maxCapacity"] = 5;
	config["minCapacity"] = 1;
	
	config["suicideMalus"] = -2;
	config["killBonus"] = 1;
	config["minimumScore"] = -9;
	config["maximumScore"] = 99;

	config["timeBeforeExplosion"] = 5;
	
	/*
	 * Then read the file
	 */
	read();
	
	/*
	 * And force some minimals
	 */
	if ( config["nbPlayers"].iValue < 1 )
	{
		config["nbPlayers"] = config["maxPlayers"];
		config["nbAIs"] = 0;
	}
	if ( config["nbMaps"].iValue < 1 )
		config["nbMaps"] = config["maxMaps"];
	if ( config["mapSize"].iValue < 15 )
		config["mapSize"] = 15;
	
	Config::isInit = true;
}

void
Config::read()
{
	// Do open the file
	std::ifstream file(CONFIG_FILE, std::ios::in);
	std::string line;
	std::string::size_type separator;
	
	if ( file.is_open() )
	{
		while ( ! file.eof() )
		{
			// If the line is correct, set the corresponding configurations
			file >> line;
			separator = line.find_first_of('=');
			if ( separator != std::string::npos )
			{
				config[line.substr(0, separator)] = line.substr(separator+1);
			}
		}
		file.close();
	}
}

void
Config::write()
{
	// Open the file
	std::ofstream file(CONFIG_FILE, std::ios::out | std::ios::trunc);
	
	if ( file.is_open() )
	{
		for ( std::map<std::string, Config>::iterator i = Config::config.begin(), e = Config::config.end() ; i != e ; ++i )
		{
			// Write the configurations to the file
			if ( ! (i->second).sValue.empty() )
				file << i->first << '=' << (i->second).sValue << std::endl;
		}
		file.close();
	}
}

std::string
Config::get(std::string key)
{
	Config::init();
	return config[key].sValue;
}

int
Config::getInt (std::string key)
{
	Config::init();
	return (config[key]).iValue;
}

void
Config::set(std::string key, int value)
{
	Config::init();
	config[key] = value;
}

void
Config::set(std::string key, std::string value)
{
	Config::init();
	config[key] = value;
}

