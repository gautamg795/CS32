#ifndef LEVEL_H_
#define LEVEL_H_

#include "GameConstants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <map>

const int INVALID_SETTING = (unsigned int)-1;

const std::string optionProbOfGoodieOverall = "probOfGoodieOverall";
const std::string optionProbOfExtraLifeGoodie = "probOfExtraLifeGoodie";
const std::string optionProbOfWalkThruGoodie = "probOfWalkThruGoodie";
const std::string optionProbOfMoreSprayersGoodie = "probOfMoreSprayersGoodie";
const std::string optionTicksPerSimpleZumiMove = "ticksPerSimpleZumiMove";
const std::string optionTicksPerComplexZumiMove = "ticksPerComplexZumiMove";
const std::string optionGoodieLifetimeInTicks = "goodieLifetimeInTicks";
const std::string optionLevelBonus = "levelBonus";
const std::string optionWalkThruLifetimeTicks = "walkThruLifetimeTicks";
const std::string optionBoostedSprayerLifetimeTicks = "boostedSprayerLifetimeTicks";
const std::string optionMaxBoostedSprayers = "maxBoostedSprayers";
const std::string optionComplexZumiSearchDistance = "complexZumiSearchDistance";

class Level
{
public:

	enum MazeEntry {
		empty, exit, player, simple_zumi, complex_zumi, perma_brick, destroyable_brick
	};
	enum LoadResult {
		load_success, load_fail_file_not_found, load_fail_bad_format
	};

	Level()
	{
		for (int i = 0; i < VIEW_HEIGHT; i++)
			for (int j = 0; j < VIEW_WIDTH; j++)
				m_maze[i][j] = empty;
	}

	LoadResult loadLevel(std::string filename, std::string dir = "")
	{
		if (dir != "")
			filename = dir + '/' + filename;
		std::ifstream levelFile(filename.c_str());
		if (!levelFile)
			return load_fail_file_not_found;

		std::string line;

		  // get options
		while (std::getline(levelFile, line))
		{
			std::string::size_type equalPos = line.find('=');
			if (equalPos == std::string::npos)
				break;
			line[equalPos] = ' ';

			std::string optionName;
			int optionValue;

			std::istringstream iss(line);
			if (! (iss >> optionName >> optionValue))
				return load_fail_bad_format;
			char dummy;
			if (iss >> dummy)
				return load_fail_bad_format;
			if (optionValue < 0)
				return load_fail_bad_format;

			toUpperStr(optionName);
			m_settingsMap[optionName] = static_cast<unsigned int>(optionValue);
		}

		if ( ! allOptionsValid())
			return load_fail_bad_format;

		  // empty line separates options from maze
		if (line.find_first_not_of(" \t\r") != std::string::npos)
			return load_fail_bad_format;

		  // get the maze

		bool foundExit = false;
		bool foundPlayer = false;

		for (int y = VIEW_HEIGHT-1; std::getline(levelFile, line); y--)
		{
			if (y < 0)  // too many maze lines?
			{
				if (line.find_first_not_of(" \t\r") != std::string::npos)
					return load_fail_bad_format;  // non-blank line
				char dummy;
				if (levelFile >> dummy)  // non-blank rest of file
			    	return load_fail_bad_format;
				break;
			}

			if (line.size() < VIEW_WIDTH  ||  line.find_first_not_of(" \t\r", VIEW_WIDTH) != std::string::npos)
				return load_fail_bad_format;
				
			for (int x = 0; x < VIEW_WIDTH; x++)
			{
				MazeEntry me;
				switch (tolower(line[x]))
				{
					default:   return load_fail_bad_format;
					case ' ':  me = empty;                      break;
					case 'e':  me = exit; foundExit = true;     break;
					case '@':  me = player; foundPlayer = true; break;
					case 's':  me = simple_zumi;                break;
					case 'c':  me = complex_zumi;               break;
					case '#':  me = perma_brick;                break;
					case '*':  me = destroyable_brick;          break;
				}
				m_maze[y][x] = me;
			}
		}

		if (!foundExit || !foundPlayer || !edgesValid())
			return load_fail_bad_format;

		return load_success;
	}

	MazeEntry getContentsOf(unsigned int x, unsigned int y) const
	{
		return (x < VIEW_WIDTH && y < VIEW_HEIGHT) ? m_maze[y][x] : empty;
	}

	unsigned int getOptionValue(std::string forSetting) const
	{
		toUpperStr(forSetting);
		SettingsMap::const_iterator it = m_settingsMap.find(forSetting);
		return (it == m_settingsMap.end()) ? INVALID_SETTING : it->second;
	}

private:

	bool optionValid(std::string option) const
	{
		return getOptionValue(option) != INVALID_SETTING;
	}

	bool allOptionsValid() const
	{
		return optionValid(optionProbOfGoodieOverall) &&
			   optionValid(optionProbOfExtraLifeGoodie) &&
			   optionValid(optionProbOfWalkThruGoodie) &&
			   optionValid(optionProbOfMoreSprayersGoodie) &&
			   optionValid(optionTicksPerSimpleZumiMove) &&
			   optionValid(optionTicksPerComplexZumiMove) &&
			   optionValid(optionGoodieLifetimeInTicks) &&
			   optionValid(optionLevelBonus) &&
			   optionValid(optionWalkThruLifetimeTicks) &&
			   optionValid(optionBoostedSprayerLifetimeTicks) &&
			   optionValid(optionMaxBoostedSprayers) &&
			   optionValid(optionComplexZumiSearchDistance);
	}

	bool edgesValid() const
	{
		for (int y = 0; y < VIEW_HEIGHT; y++)
			if (m_maze[y][0] != perma_brick || m_maze[y][VIEW_WIDTH-1] != perma_brick)
				return false;
		for (int x = 0; x < VIEW_WIDTH; x++)
			if (m_maze[0][x] != perma_brick || m_maze[VIEW_HEIGHT-1][x] != perma_brick)
				return false;

		return true;
	}

	static void toUpperStr(std::string& str)
	{
		for (std::string::size_type k = 0; k != str.size(); k++)
			str[k] = toupper(str[k]);
	}

private:
	typedef std::map<std::string, unsigned int> SettingsMap;

	MazeEntry   m_maze[VIEW_HEIGHT][VIEW_WIDTH];
	SettingsMap m_settingsMap;
};

#endif // LEVEL_H_
