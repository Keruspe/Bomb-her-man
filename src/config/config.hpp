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
		/// Constructor (for item)
		Config();
		
		/// To write the file
		static void write();
		
		/// To get a parameter
		/**
		 * @param key The name of the value you want to get
		 * 
		 * @return The value of the key (a string)
		 */
		static std::string get(std::string key);
		/// To get a parameter
		/**
		 * @param key The name of the value you want to get
		 * 
		 * @return The value of the key (an int)
		 */
		static int getInt(std::string key);
		/// To set a parameter
		/**
		 * @param key The name of the value you want to set
		 * @param value The value of the key (an int)
		 */
		static void set(std::string key, int value);
		/// To set a parameter
		/**
		 * @param key The name of the value you want to set
		 * @param value The value of the key (a string)
		 */
		static void set(std::string key, std::string value);
	
	private:
		/// To set the new value (string)
		Config &operator=(std::string);
		/// To set the new value (int)
		Config &operator=(int);
		
		/// Get the int value a of parameter
		/**
		 * This function convert the string to an int
		 * if necessary
		 * 
		 * @return The int value
		 */
		int intValue();
		
		/// The value of an int parameter
		int iValue;
		/// The value of a string parameter
		std::string sValue;
		
		/// To initialize the map
		static void init();
		/// To read the file
		static void read();
		
		/// The confguration map
		static std::map<std::string, Config> config;
	
	};
}



#endif // _CONFIG_HPP_

