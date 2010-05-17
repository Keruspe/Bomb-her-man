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

#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>
#include <map>
#include <fstream>
#include <sstream>

namespace bombherman
{
	class Config
	{
	public:
		///Constructor : item
		Config();
		
		/// To get a parameter
		static std::string get(std::string key);
		static int getInt(std::string key);
		/// To set a parameter
		static void set(std::string key, int value);
		static void set(std::string key, std::string value);
	
	private:
		Config &operator=(std::string);
		Config &operator=(int);
		int intValue();
		
		std::string sValue;
		int iValue;
		
		/// To initialize the map
		static void init();
		/// To read the file
		static void read();
		
		/// The confguration map
		static std::map<std::string, Config> config;
	
	};
}



#endif // _CONFIG_HPP_

