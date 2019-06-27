/*
 * SerialPortModel.h
 *
 * Created: 24 2018 by Ashley
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

#pragma once

#include <map>
#include <thread>
#include <sstream>

#include <glm/vec3.hpp>

#include "GrblMachineState.h"
#include "GrblConfigurationModel.h"
#include "GCodeFileModel.h"
#include "GCodeCommand.h"
#include "GrblResponse.h"

using std::stringstream;
using std::thread;
using std::deque;
using std::map;
using std::string;
using glm::vec3;

namespace Coconut
{
    class AppState;
	class GrblMachineModel
	{
	public:
		GrblMachineModel(AppState* state);
		~GrblMachineModel();

		void ClearState();
        void StartWorkThread();
        void JoinWorkThread();
        void WorkFunction();

		int  BufferLengthInUse();
		bool SendNextCommandFromQueue();
		int  CommandsQueueLength();

		void RestoreOffsets(GrblMachineState& state);
		void StoreOffsets(GrblMachineState& state);
		void RestoreParserState();
		void ClearCommandQueue();
		void ClearCommandBuffer();

		vec3  GetMachinePosition();
		float GetMachinePositionX();
		float GetMachinePositionY();
		float GetMachinePositionZ();

		vec3  GetWorkPosition();
		float GetWorkPositionX();
		float GetWorkPositionY();
		float GetWorkPositionZ();

		void QueueCommand(const GCodeCommand& command);

		bool GetProgramRunning() const;
		void SetProgramRunning(bool programRunning);


		void SendProgram();
		void SendProgramFromLine(long fromId);
		void GCodeCommandManualSend(const GCodeCommand& cmd);

		void UpdateSpindleOverride(float speed);
		void UpdateFeedOverride(float rate);
		void UpdateRapidOverride(float rate);
		void ToolChangeCompleted();

		void BytesWritten(int bytes);

        GrblConfigurationModel& GetConfigurationModel();
        bool IsWorkThreadRunning();

	protected: // Member Functions
		GCodeCommand FeedOverride(const GCodeCommand& command, double overridePercent);
		GCodeCommand GetNextCommand(GCodeFileModel& gcodeFile);

		bool IsSpaceInBuffer(const GCodeCommand& cmd);
		int  GetProcessedPercent();

		void UpdateWorkPosition();
		void UpdateStatus(GrblResponse response);
		void UpdateOverrides(const GrblResponse& data);
		void UpdateWorkCoordinateOffset(const GrblResponse& data);
		void UpdateMachinePosition(const GrblResponse& data);
		void UpdateFeedRateAndSpindleSpeed(const GrblResponse& response);

        void SendNextPacket();
		void ProcessResponse(const GrblResponse& data);
		void ParseError(const GrblResponse& error);
		void ParseGrblVersion(const GrblResponse& response);
		void ParseConfigurationResponse(GrblResponse response);
		void ParseAlarmResponse(const GrblResponse& response);
        void RequestStatus();

        long GetTimeDelta();

		static string StateToString(GrblMachineState state);
		const static map<int,string> ERROR_STRINGS;
		const static map<int,string> ALARM_STRINGS;

	private: // Members
		const static int BUFFER_LENGTH_LIMIT;
        AppState* mAppState;
		deque<GCodeCommand> mCommandBuffer;
		deque<GCodeCommand> mCommandQueue;
        vector<string> mLinesRead;
        stringstream mCurrentLine;
        GrblConfigurationModel mConfigurationModel;
		GrblMachineState mState;
		GrblMachineState mLastState;

		vec3 mMachinePosition;
		vec3 mWorkPosition;
		vec3 mWorkCoordinateOffset;

		int mProgramSendInterval;
		int mStatusInterval;
		int mCountProcessedCommands;
		int mCommandQueueInitialSize;
		float mFeedOverride;
		float mSpindleOverride;
		float mRapidOverride;
		bool mError;
		int mErrorCode;
		float mBufferUsedPercentage;
		string mErrorString;
		string mGrblVersion;
		int mBytesWaiting;

		bool mStatusRequested;
		bool mWaitingForStatus;

		bool mProgramRunning;
		bool mToolChangeWaiting;
		int mFeedRate;
		int mSpindleSpeed;

        thread mWorkThread;
        bool mWorkThreadRunning;
        long mCurrentTime;
        long mLastTime;
	};
}
