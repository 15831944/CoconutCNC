#include "ConsoleWindow.h"
#include "../../AppState.h"

using std::stringstream;

namespace Coconut
{
    ConsoleWindow::ConsoleWindow(AppState* project)
        : ImGuiWidget(project, "Console")
	{}

    ConsoleWindow::~ConsoleWindow ()
	{}

    void
    ConsoleWindow::Draw
    ()
    {
        ImGui::Begin(mName.c_str(), &mVisible);

        ImVec2 space = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("CommandHistory",ImVec2(-1,space.y - 30));

        ImGui::EndChild();
        ImGui::InputText("##MdiCommand", &mMdiBuffer[0], MDI_BUFFER_SIZE);
        ImGui::SameLine();
        ImGui::Button("Send");
		ImGui::End();
    }
}

