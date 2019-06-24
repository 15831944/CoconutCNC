#include "JogWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    JogWindow::JogWindow(AppState* project) : ImGuiWidget(project, "Jog"),
		mJogStepValue(10.f),
		mJogFeedRate(1000.0f),
        mJogAbsoluteX(0.0f),
        mJogAbsoluteY(0.0f),
        mJogAbsoluteZ(0.0f)
	{}

    JogWindow::~JogWindow ()
	{}

    void
    JogWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);

        ImVec2 space = ImGui::GetContentRegionAvail();
        ImVec2 buttonSize(
    		(space.x -= ImGui::GetItemsLineHeightWithSpacing()) / 4,
        	(space.y -= ImGui::GetItemsLineHeightWithSpacing()) / 3
        );

        // Row 1
        if (ImGui::Button("-X / +Y"))
        {

        }
        ImGui::SameLine();

        if (ImGui::Button("+Y"))
        {

        }
        ImGui::SameLine();

        if (ImGui::Button("+X / +Y"))
        {

        }
        ImGui::SameLine();

        if (ImGui::Button("Z+"))
        {

        }

        // Row 2
		if (ImGui::Button("-X"))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("Home XY"))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("+X"))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("Home Z"))
        {

        }

		// Row 3
		if (ImGui::Button("-X / -Y"))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("-Y"))
        {

        }

        ImGui::SameLine();

		if (ImGui::Button("+X / -Y"))
        {

        }

        ImGui::SameLine();

        if (ImGui::Button("-Z"))
        {

        }

        ImGui::Separator();

        ImGui::Text("Step");

        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat("##Step", &mJogStepValue))
        {
			if (mJogStepValue < 0.0f)
            {
                mJogStepValue = 0.01f;
            }
            if (mJogStepValue > 100.0f)
            {
               mJogStepValue = 100.0f;
            }
        }

        if (ImGui::Button("0.01"))
        {
            mJogStepValue = 0.01f;
        }
        ImGui::SameLine();

        if (ImGui::Button("0.1"))
        {
            mJogStepValue = 0.1f;
        }
        ImGui::SameLine();

        if (ImGui::Button("1"))
        {
            mJogStepValue = 1.f;
        }
        ImGui::SameLine();

        if (ImGui::Button("10"))
        {
            mJogStepValue = 10.f;
        }
        ImGui::SameLine();

        if (ImGui::Button("100"))
        {
            mJogStepValue = 100.f;
        }

        ImGui::Separator();

        ImGui::Text("Feed Rate");

        ImGui::PushItemWidth(-1);
        if(ImGui::DragFloat("##FeedRate",&mJogFeedRate))
        {

        }

        ImGui::Text("Jog Absolute");

		if (ImGui::Button("Go To X"))
        {

    	}
        ImGui::SameLine();
		ImGui::InputFloat("##JogAbsoluteX",&mJogAbsoluteX);

        if (ImGui::Button("Go To Y"))
        {

    	}
        ImGui::SameLine();
        ImGui::InputFloat("##JogAbsoluteY",&mJogAbsoluteY);

        if (ImGui::Button("Go To Z"))
        {

    	}
        ImGui::SameLine();
        ImGui::InputFloat("##JogAbsoluteZ",&mJogAbsoluteZ);

		ImGui::End();
    }
}

