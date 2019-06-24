// This file is a part of "CoconutCNC" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#pragma once

#include "GLWidget.h"
#include <cmath>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using glm::vec3;
using glm::vec4;

namespace  Coconut
{
    class Cylinder;
	class ToolSettings;
	class ToolDrawer : public GLWidget
	{
	public:
		ToolDrawer(AppState* state);
		virtual ~ToolDrawer() override;

        bool Init() override;
		void Update() override;

		float ToolDiameter() const;
		void SetToolDiameter(float toolDiameter);

		float ToolLength() const;
		void SetToolLength(float toolLength);

		vec3 ToolPosition() const;
		void SetToolPosition(const vec3 &toolPosition);

		float ToolAngle() const;
		void SetToolAngle(float toolAngle);

		vec4 ToolColor() const;
		void SetToolColor(const vec4& color);

		vec4 ToolHolderColor() const;
		void SetToolHolderColor(const vec4& color);

		ToolSettings* GetTool() const;
		void SetTool(ToolSettings* toolHandle);

		void SetSpindleRotating(bool);
		void SetSpindleSpeed(float);

		bool NeedsUpdateGeometry() const;

	protected:
		float NormalizeAngle(float angle);
        void GenerateToolGeometry();
        void GenerateToolHolderGeometry();
		vector<GLWidgetVertex> GenerateCylinderGeometry
        (Cylinder* cylinder, const vec3& origin, const vec4& color);
		void Rotate();

	private:
		ToolSettings* mTool;
		vec3 mToolPosition;
		float mRotationAngle;
		vec4 mToolColor;
		vec4 mToolHolderColor;
		bool mSpindleRotating;
		float mSpindleSpeed;
		bool mNeedsGeometryUpdate;
	};

}
