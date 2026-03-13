/**********************************************************************************************
*   imgui_impl_raylib.h - Header for raylib ImGui backend
*   Part of rlImGui from raylib-extras
**********************************************************************************************/
#pragma once

struct ImDrawData;
struct ImTextureData;

#ifdef __cplusplus
extern "C" {
#endif

bool ImGui_ImplRaylib_Init(void);
void ImGui_ImplRaylib_Shutdown(void);
void ImGui_ImplRaylib_NewFrame(void);
void ImGui_ImplRaylib_RenderDrawData(ImDrawData* drawData);
bool ImGui_ImplRaylib_ProcessEvents(void);
void ImGui_ImplRaylib_UpdateTexture(ImTextureData* tex);

#ifdef __cplusplus
}
#endif

