/*
 * GcodeCommand.cpp
 *
 * Created: 02 2018 by Ashley
 *
 * Copyright 2018 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "GCodeCommand.h"
#include "../../Common/Util.h"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <regex>

using std::stringstream;
using std::fixed;
using std::setprecision;
using std::regex;

namespace Coconut
{
	GCodeCommand::GCodeCommand
	(string cmd,
		int tableIndex,
		bool showInConsole,
		int consoleIndex,
		GcodeCommandState state
	)
		: mRawCommand(0),
		  mCommand(cmd),
		  mResponse(GrblResponse()),
		  mState(state),
		  mArgs(vector<string>()),
		  mID(nextID()),
		  mLine(-1),
		  mTableIndex(tableIndex),
		  mConsoleIndex(consoleIndex),
		  mShowInConsole(showInConsole) {}

	GCodeCommand::GCodeCommand(const GCodeCommand& other)
		: mRawCommand(other.mRawCommand),
		  mCommand(other.mCommand),
		  mResponse(other.mResponse),
		  mState(other.mState),
		  mArgs(other.mArgs),
		  mID(other.mID),
		  mLine(other.mLine),
		  mTableIndex(other.mTableIndex),
		  mConsoleIndex(other.mConsoleIndex),
		  mShowInConsole(other.mShowInConsole) {}

	GCodeCommand::GCodeCommand(const GCodeCommand* other)
		: mRawCommand(other->mRawCommand),
		  mCommand(other->mCommand),
		  mResponse(other->mResponse),
		  mState(other->mState),
		  mArgs(other->mArgs),
		  mID(other->mID),
		  mLine(other->mLine),
		  mTableIndex(other->mTableIndex),
		  mConsoleIndex(other->mConsoleIndex),
		  mShowInConsole(other->mShowInConsole) {}


	GCodeCommand::GCodeCommand(unsigned char rawCmd) : mRawCommand(rawCmd) {}

	GCodeCommand* GCodeCommand::AbsoluteXCommand(float x)
	{
		static GCodeCommand gc;
        stringstream s;
        s << "G90X" << fixed << setprecision(3) << x << "\r";
		gc = GCodeCommand(s.str());
		return &gc;
	}

	GCodeCommand* GCodeCommand::AbsoluteYCommand(float y)
	{
		static GCodeCommand gc;
        stringstream s;
		s << "G90Y"<< fixed << setprecision(3) << y << "\r";
		gc = GCodeCommand(s.str());
		return &gc;
	}

	GCodeCommand* GCodeCommand::AbsoluteZCommand(float z)
	{
		static GCodeCommand gc;
        stringstream s;
        s << "G90Z" << fixed << setprecision(3) << z << "\r";
		gc = GCodeCommand(s.str());
		return &gc;
	}

	GCodeCommand* GCodeCommand::ControlXCommand()
	{
		static GCodeCommand gc("[CTRL+X]");
		return &gc;
	}

	GCodeCommand* GCodeCommand::GetFirmwareConfigurationCommand()
	{
		static GCodeCommand doubleBuck("$$\r");
		return &doubleBuck;
	}

	GCodeCommand* GCodeCommand::SetFirmwareConfigurationCommand(int param, string value)
	{
		static GCodeCommand doubleBuck;
        stringstream s;
        s << "$" << param << "=" << value << "\r";
		doubleBuck = (s.str());
		return &doubleBuck;
	}

	GCodeCommand* GCodeCommand::UnlockCommand()
	{
		static GCodeCommand gc("$X\r");
		return &gc;
	}

	GCodeCommand* GCodeCommand::ResetCommand()
	{
		static GCodeCommand gc(0x18);
		return &gc;
	}

	GCodeCommand* GCodeCommand::JogCancelCommand()
	{
		static GCodeCommand gc(0x85);
		return &gc;
	}

	GCodeCommand* GCodeCommand::StatusUpdateCommand()
	{
		static GCodeCommand gc("?");
		return &gc;
	}

	GCodeCommand* GCodeCommand::SpindleCounterClockwiseCommand()
	{
		static GCodeCommand gc("M4\r");
		return &gc;
	}

	GCodeCommand* GCodeCommand::SpindleClockwiseCommand()
	{
		static GCodeCommand gc("M3\r");
		return &gc;
	}

	GCodeCommand* GCodeCommand::SpindleStopCommand()
	{
		static GCodeCommand gc("M5\r");
		return &gc;
	}

	GCodeCommand* GCodeCommand::HomingCommand()
	{
	   static GCodeCommand gc("$H\r");
	   return &gc;
	}

	GCodeCommand* GCodeCommand::ZeroXYCommand()
	{
	   static GCodeCommand gc("G92X0Y0\r");
	   return &gc;
	}

	GCodeCommand*GCodeCommand::GetGcodeParserParamsCommand()
	{
	   static GCodeCommand gc("$#\r");
	   return &gc;
	}

	GCodeCommand*GCodeCommand::ZeroZCommand()
	{
	   static GCodeCommand gc("G92Z0\r");
	   return &gc;
	}

	GCodeCommand* GCodeCommand::CyclePauseResumeCommand()
	{
		static GCodeCommand gc("~");
		return &gc;
	}

	GCodeCommand* GCodeCommand::FeedHoldCommand()
	{
	   static GCodeCommand gc("!");
	   return &gc;
	}

	GCodeCommand* GCodeCommand::JogCommand
	(
		double x, double y, double z,
		int feedRate,
		bool inches,
		bool machineCoordinates
	)
	{
		static string jogString = "$J=%1 %2 X%3 Y%4 Z%5 F%6\r";
        stringstream s;
        s << "$J=" << (inches?"G20 ":"G21 ");
	    s << (machineCoordinates?"G53 ":"G91 ");
		s << "X" << fixed << setprecision(3) << x;
		s << "Y" << fixed << setprecision(3) << y;
		s << "Z" << fixed << setprecision(3) << z;
        s << feedRate;

		static GCodeCommand gc;
		gc = GCodeCommand(s.str());
		return &gc;
	}

	GCodeCommand* GCodeCommand::SetSafePositionCommand()
	{
		static GCodeCommand gc("G28.1\r");
		return &gc;
	}

	GCodeCommand* GCodeCommand::GoToSafePositionCommand()
	{
		static GCodeCommand gc("G28 G91 Z0\rG28 G91 X0 Y0\r");
		return &gc;
	}

	GCodeCommand* GCodeCommand::GoToXYOriginCommand()
	{
		static GCodeCommand gc("G90 X0Y0\r");
		return &gc;
	}

	GCodeCommand* GCodeCommand::GoToZOriginCommand()
	{
		static GCodeCommand gc("G90 Z0\r");
		return &gc;
	}

	GCodeCommand *GCodeCommand::FeedOvDefault()
	{
		static GCodeCommand gc(0x90);
		return &gc;
	}

	GCodeCommand *GCodeCommand::FeedOvPlusOne()
	{
		static GCodeCommand gc(0x93);
		return &gc;
	}

	GCodeCommand *GCodeCommand::FeedOvPlusTen()
	{
		static GCodeCommand gc(0x91);
		return &gc;
	}

	GCodeCommand *GCodeCommand::FeedOvMinusOne()
	{
		static GCodeCommand gc(0x94);
		return &gc;
	}

	GCodeCommand *GCodeCommand::FeedOvMinusTen()
	{
		static GCodeCommand gc(0x92);
		return &gc;
	}

	GCodeCommand *GCodeCommand::RapidOvDefault()
	{
		static GCodeCommand gc(0x95);
		return &gc;
	}

	GCodeCommand *GCodeCommand::RapidOv50Percent()
	{
		static GCodeCommand gc(0x96);
		return &gc;
	}

	GCodeCommand *GCodeCommand::RapidOv25Percent()
	{
		static GCodeCommand gc(0x97);
		return &gc;
	}

	GCodeCommand *GCodeCommand::SpindleOvDefault()
	{
		static GCodeCommand gc(0x99);
		return &gc;
	}

	GCodeCommand *GCodeCommand::SpindleOvPlusOne()
	{
		static GCodeCommand gc(0x9C);
		return &gc;
	}

	GCodeCommand *GCodeCommand::SpindleOvPlusTen()
	{
		static GCodeCommand gc(0x9A);
		return &gc;
	}

	GCodeCommand *GCodeCommand::SpindleOvMinusOne()
	{
		static GCodeCommand gc(0x9D);
		return &gc;
	}

	GCodeCommand *GCodeCommand::SpindleOvMinusTen()
	{
		static GCodeCommand gc(0x9B);
		return &gc;
	}

	bool GCodeCommand::operator==(const GCodeCommand& other)
	{
		return mID == other.mID;
	}

	bool GCodeCommand::isEmpty()
	{
		return mCommand.empty();
	}

	int GCodeCommand::getCommandLength() const
	{
		return mCommand.length();
	}

	string GCodeCommand::getCommand() const
	{
		return mCommand;
	}

	bool GCodeCommand::isM30Command()
	{
		return mCommand.find("M30") != string::npos;
	}

	void GCodeCommand::setCommand(const string& command)
	{
		mCommand = Util::to_upper(command);
	}

	int GCodeCommand::getTableIndex() const
	{
		return mTableIndex;
	}

	void GCodeCommand::setTableIndex(int tableIndex)
	{
		mTableIndex = tableIndex;
	}

	bool GCodeCommand::getShowInConsole() const
	{
		return mShowInConsole;
	}

	void GCodeCommand::setShowInConsole(bool showInConsole)
	{
		mShowInConsole = showInConsole;
	}

	int GCodeCommand::getConsoleIndex() const
	{
		return mConsoleIndex;
	}

	void GCodeCommand::setConsoleIndex(int consoleIndex)
	{
		mConsoleIndex = consoleIndex;
	}

	long GCodeCommand::getID()
	{
		return mID;
	}

	bool GCodeCommand::hasID(long id)
	{
		return mID == id;
	}

	long GCodeCommand::nextID()
	{
		return ID++;
	}

	string GCodeCommand::getMarker() const
	{
		return mMarker;
	}

	bool GCodeCommand::isMarker() const
	{
	   return !mMarker.empty();
	}

	void GCodeCommand::setMarker(const string marker)
	{
		mMarker = marker;
	}

	bool GCodeCommand::isToolChangeCommand()
	{
	   return mCommand.find("M6") != string::npos;
	}

	string GCodeCommand::removeM6()
	{
		string noM6 = mCommand;
        noM6 = regex_replace(noM6, regex("M6"), "  ");
		return noM6;
	}

	int GCodeCommand::getToolNumber()
	{
		static regex toolNumRegex("T(\\d+)");
		std::smatch base_match;

		if (std::regex_match(mCommand, base_match, toolNumRegex))
        {
			// The first sub_match is the whole string; the next
			// sub_match is the first parenthesized expression.
			if (base_match.size() == 2)
            {
				std::ssub_match base_sub_match = base_match[1];
				std::string base = base_sub_match.str();
                return std::stoi(base);
			}
		}
		return -1;
	}

	GrblResponse GCodeCommand::getResponse() const
	{
		return mResponse;
	}

	void GCodeCommand::setResponse(const GrblResponse& response)
	{
		mResponse = response;
	}

	int GCodeCommand::getLine() const
	{
		return mLine;
	}

	void GCodeCommand::setLine(int line)
	{
		mLine = line;
	}

	bool GCodeCommand::isRawCommand()
	{
		return mRawCommand > 0;
	}

	vector<string> GCodeCommand::getArgs() const
	{
		return mArgs;
	}

	void GCodeCommand::setArgs(const vector<string>& args)
	{
		mArgs= args;
	}

	GcodeCommandState GCodeCommand::getState() const
	{
		return mState;
	}

	void GCodeCommand::setState(const GcodeCommandState state)
	{
		mState = state;
	}

	unsigned char GCodeCommand::getRawCommand()
	{
		return mRawCommand;
	}

	long GCodeCommand::ID = 0;

}
