#include "ImGuiInterface.h"

void SetStyle()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(ImColor(56, 56, 56)));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBg, ImVec4(ImColor(33, 25, 46)));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_TitleBgActive, ImVec4(ImColor(63, 48, 87)));
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(ImColor(77, 77, 77)));
}

void CreateAssetMenu()
{

}

void CreateTopBarMenu()
{
	ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Create")) {
        }
        if (ImGui::MenuItem("Open", "Ctrl+O")) {
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {
        }
        if (ImGui::MenuItem("Save as..")) {
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Preferences", "Ctrl+P")) {
        }
        ImGui::EndMenu();
    }
    
	ImGui::EndMainMenuBar();
}