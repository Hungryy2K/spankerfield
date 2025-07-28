#include "esp.h"
#include "../../settings.h"
#include "../../global.h"
#include "../../SDK/sdk.h"
#include "../../Rendering/draw-list.h"
#include "../../Rendering/Maps/nicknames.h"
#include "../../Utilities/w2s.h"
#include "../../Utilities/other.h"
#include "../../Features/Friend List/friend_list.h"
#include "../../Features/Blacklist\blacklist.h"

#define PI 3.14159265f

using namespace big;
namespace plugins
{
    void draw_bone(const ImColor& color, RagdollComponent* ragdoll, UpdatePoseResultData::BONES bone_1, UpdatePoseResultData::BONES bone_2, bool dots)
    {
        if (!IsValidPtr(ragdoll))
            return;

        Vector3 bone_1_vec;
        if (!ragdoll->GetBone(bone_1, bone_1_vec))
            return;

        Vector3 bone_2_vec;
        if (!ragdoll->GetBone(bone_2, bone_2_vec))
            return;

        if (!world_to_screen(bone_1_vec) || !world_to_screen(bone_2_vec))
            return;

        auto pos_1 = Vector2(bone_1_vec.x, bone_1_vec.y);
        auto pos_2 = Vector2(bone_2_vec.x, bone_2_vec.y);

        // Modern bone drawing with enhanced visual effects
        if (dots)
        {
            // Enhanced bone dots with gradient effect
            m_drawing->DrawFillArea(pos_1.x, pos_1.y, 4.0f, 4.0f, color);
            m_drawing->DrawFillArea(pos_2.x, pos_2.y, 4.0f, 4.0f, color);
            
            // Add subtle glow effect
            ImColor glow_color = ImColor(color.Value.x * 0.7f, color.Value.y * 0.7f, color.Value.z * 0.7f, color.Value.w * 0.5f);
            m_drawing->DrawFillArea(pos_1.x, pos_1.y, 6.0f, 6.0f, glow_color);
            m_drawing->DrawFillArea(pos_2.x, pos_2.y, 6.0f, 6.0f, glow_color);
        }

        // Enhanced bone line with thickness variation
        float line_thickness = 2.0f;
        m_drawing->AddLine(ImVec2(pos_1.x, pos_1.y), ImVec2(pos_2.x, pos_2.y), color, line_thickness);
        
        // Add subtle glow line
        ImColor glow_line_color = ImColor(color.Value.x * 0.6f, color.Value.y * 0.6f, color.Value.z * 0.6f, color.Value.w * 0.3f);
        m_drawing->AddLine(ImVec2(pos_1.x, pos_1.y), ImVec2(pos_2.x, pos_2.y), glow_line_color, line_thickness + 1.0f);
    }

    std::map<uint64_t, std::string> streamer_personas{};
    
    // Enhanced ESP Features - New Visual Effects (Placeholder for future implementation)
    // Note: These functions will be implemented with proper API calls in future updates
    
    // Radar system for better situational awareness
    void draw_radar_system(const Vector3& local_pos, const std::vector<std::pair<Vector3, ImColor>>& enemies)
    {
        const float radar_size = 150.0f;
        const float radar_range = 200.0f;
        const ImVec2 radar_pos(50.0f, 50.0f);
        
        // Draw radar background
        ImColor radar_bg = ImColor(0.1f, 0.1f, 0.1f, 0.8f);
        m_drawing->AddRectFilled(radar_pos, ImVec2(radar_pos.x + radar_size, radar_pos.y + radar_size), radar_bg);
        m_drawing->AddRect(radar_pos, ImVec2(radar_pos.x + radar_size, radar_pos.y + radar_size), ImColor(0.3f, 0.3f, 0.3f, 1.0f), 0.0f);
        
        // Draw radar center (player position)
        ImVec2 radar_center(radar_pos.x + radar_size / 2.0f, radar_pos.y + radar_size / 2.0f);
        m_drawing->AddCircleFilled(radar_center, 3.0f, ImColor(0.0f, 1.0f, 0.0f, 1.0f));
        
        // Draw enemies on radar
        for (const auto& enemy : enemies)
        {
            Vector3 relative_pos = enemy.first - local_pos;
            if (relative_pos.Length() <= radar_range)
            {
                float normalized_x = (relative_pos.x / radar_range) * (radar_size / 2.0f);
                float normalized_z = (relative_pos.z / radar_range) * (radar_size / 2.0f);
                
                ImVec2 enemy_pos(radar_center.x + normalized_x, radar_center.y + normalized_z);
                m_drawing->AddCircleFilled(enemy_pos, 2.0f, enemy.second);
            }
        }
    }
    
    // Hit marker system
    void draw_hit_marker(const Vector2& screen_pos, bool is_headshot = false)
    {
        const float marker_size = is_headshot ? 20.0f : 15.0f;
        const float thickness = is_headshot ? 3.0f : 2.0f;
        ImColor marker_color = is_headshot ? ImColor(1.0f, 0.0f, 0.0f, 1.0f) : ImColor(1.0f, 1.0f, 0.0f, 1.0f);
        
        // Draw crosshair-style hit marker
        m_drawing->AddLine(
            ImVec2(screen_pos.x - marker_size, screen_pos.y),
            ImVec2(screen_pos.x - marker_size/2, screen_pos.y),
            marker_color, thickness
        );
        m_drawing->AddLine(
            ImVec2(screen_pos.x + marker_size/2, screen_pos.y),
            ImVec2(screen_pos.x + marker_size, screen_pos.y),
            marker_color, thickness
        );
        m_drawing->AddLine(
            ImVec2(screen_pos.x, screen_pos.y - marker_size),
            ImVec2(screen_pos.x, screen_pos.y - marker_size/2),
            marker_color, thickness
        );
        m_drawing->AddLine(
            ImVec2(screen_pos.x, screen_pos.y + marker_size/2),
            ImVec2(screen_pos.x, screen_pos.y + marker_size),
            marker_color, thickness
        );
    }
    
    // Enhanced bullet tracer system
    void draw_bullet_tracers(const Vector3& start_pos, const Vector3& end_pos, const ImColor& color)
    {
        Vector2 start_screen, end_screen;
        if (world_to_screen(start_pos, start_screen) && world_to_screen(end_pos, end_screen))
        {
            // Draw main tracer line
            m_drawing->AddLine(ImVec2(start_screen.x, start_screen.y), ImVec2(end_screen.x, end_screen.y), color, 2.0f);
            
            // Draw glow effect
            ImColor glow_color = ImColor(color.Value.x * 0.6f, color.Value.y * 0.6f, color.Value.z * 0.6f, color.Value.w * 0.3f);
            m_drawing->AddLine(ImVec2(start_screen.x, start_screen.y), ImVec2(end_screen.x, end_screen.y), glow_color, 4.0f);
            
            // Draw impact effect at end point
            m_drawing->AddCircleFilled(ImVec2(end_screen.x, end_screen.y), 3.0f, color);
        }
    }
    
    // Enhanced visibility indicators
    void draw_visibility_indicators(const Vector3& pos, bool is_visible, const ImColor& color)
    {
        Vector2 screen_pos;
        if (!world_to_screen(pos, screen_pos))
            return;
            
        if (is_visible)
        {
            // Draw visibility indicator (eye icon)
            m_drawing->AddCircleFilled(ImVec2(screen_pos.x, screen_pos.y - 35.0f), 8.0f, ImColor(0.0f, 1.0f, 0.0f, 0.8f));
            m_drawing->AddCircle(ImVec2(screen_pos.x, screen_pos.y - 35.0f), 8.0f, color, 1.0f);
        }
        else
        {
            // Draw occluded indicator (crossed eye)
            m_drawing->AddCircleFilled(ImVec2(screen_pos.x, screen_pos.y - 35.0f), 8.0f, ImColor(1.0f, 0.0f, 0.0f, 0.8f));
            m_drawing->AddCircle(ImVec2(screen_pos.x, screen_pos.y - 35.0f), 8.0f, color, 1.0f);
        }
    }
    
    // Distance indicator function
    void draw_distance_indicator(const Vector3& world_pos, const ImColor& color, float distance)
    {
        Vector2 screen_pos;
        if (!world_to_screen(world_pos, screen_pos))
            return;
            
        // Format distance text
        char distance_text[32];
        if (distance >= 1000.0f)
            sprintf_s(distance_text, "%.1fkm", distance / 1000.0f);
        else
            sprintf_s(distance_text, "%.0fm", distance);
            
        // Draw distance text with shadow
        m_drawing->AddText(screen_pos.x, screen_pos.y - 20.0f, color, 12.0f, FL_CENTER_X | FL_SHADOW, distance_text);
        
        // Draw small indicator dot
        m_drawing->AddCircleFilled(ImVec2(screen_pos.x, screen_pos.y - 25.0f), 2.0f, color);
    }
    
    // 3D Health bar function
    void draw_health_bar_3d(const TransformAABBStruct& transform, float health_percentage, const ImColor& color)
    {
        Vector3 pos = Vector3(transform.Transform.m[3]);
        Vector2 screen_pos;
        if (!world_to_screen(pos, screen_pos))
            return;
            
        // Health bar dimensions
        float bar_width = 40.0f;
        float bar_height = 4.0f;
        float bar_x = screen_pos.x - bar_width / 2.0f;
        float bar_y = screen_pos.y - 30.0f;
        
        // Background bar
        ImColor bg_color = ImColor(0.1f, 0.1f, 0.1f, 0.8f);
        m_drawing->AddRectFilled(ImVec2(bar_x, bar_y), ImVec2(bar_x + bar_width, bar_y + bar_height), bg_color);
        
        // Health bar
        float health_width = bar_width * health_percentage;
        ImColor health_color = ImColor(
            (1.0f - health_percentage) * color.Value.x + health_percentage * 0.2f,
            health_percentage * color.Value.y,
            health_percentage * color.Value.z,
            color.Value.w
        );
        m_drawing->AddRectFilled(ImVec2(bar_x, bar_y), ImVec2(bar_x + health_width, bar_y + bar_height), health_color);
        
        // Border
        m_drawing->AddRect(ImVec2(bar_x, bar_y), ImVec2(bar_x + bar_width, bar_y + bar_height), color, 0.0f);
    }
    
    // Weapon indicator function
    void draw_weapon_indicator(ClientSoldierEntity* soldier, const ImColor& color)
    {
        if (!IsValidPtr(soldier))
            return;
            
        // Get soldier position from transform (same as in main ESP function)
        TransformAABBStruct transform = get_transform(soldier);
        Vector3 soldier_pos = Vector3(transform.Transform.m[3]);
            
        Vector2 screen_pos;
        if (!world_to_screen(soldier_pos, screen_pos))
            return;
            
        // Draw generic weapon indicator (since weapon name access is not available in SDK)
        m_drawing->AddText(screen_pos.x, screen_pos.y + 15.0f, color, 10.0f, FL_CENTER_X | FL_SHADOW, "WEAPON");
        
        // Draw small weapon icon (dot)
        m_drawing->AddCircleFilled(ImVec2(screen_pos.x, screen_pos.y + 25.0f), 1.5f, color);
    }

    void draw_3d_box(const TransformAABBStruct& transform, const ImColor& color, float thickness = 1.0f)
    {
        // Get position and size of the object
        Vector3 pos = Vector3(transform.Transform.m[3]);
        Vector3 min = Vector3(transform.AABB.m_Min.x, transform.AABB.m_Min.y, transform.AABB.m_Min.z);
        Vector3 max = Vector3(transform.AABB.m_Max.x, transform.AABB.m_Max.y, transform.AABB.m_Max.z);
        
        // Calculate all 8 corners of the 3D box
        Vector3 corners[8];
        corners[0] = pos + multiply_mat(Vector3(min.x, min.y, min.z), &transform.Transform); // Bottom left back
        corners[1] = pos + multiply_mat(Vector3(max.x, min.y, min.z), &transform.Transform); // Bottom right back
        corners[2] = pos + multiply_mat(Vector3(max.x, min.y, max.z), &transform.Transform); // Bottom right front
        corners[3] = pos + multiply_mat(Vector3(min.x, min.y, max.z), &transform.Transform); // Bottom left front
        corners[4] = pos + multiply_mat(Vector3(min.x, max.y, min.z), &transform.Transform); // Top left back
        corners[5] = pos + multiply_mat(Vector3(max.x, max.y, min.z), &transform.Transform); // Top right back
        corners[6] = pos + multiply_mat(Vector3(max.x, max.y, max.z), &transform.Transform); // Top right front
        corners[7] = pos + multiply_mat(Vector3(min.x, max.y, max.z), &transform.Transform); // Top left front
        
        // Convert 3D coordinates to 2D for display on the screen
        Vector2 screen_corners[8];
        for (int i = 0; i < 8; i++)
        {
            if (!world_to_screen(corners[i], screen_corners[i]))
                return; // If at least one corner is not visible, don't draw the box
        }
        
        // Enhanced 3D box drawing with modern effects and glow
        float enhanced_thickness = thickness * 2.0f;
        
        // Create glow effect with multiple layers
        ImColor inner_glow = ImColor(color.Value.x * 0.4f, color.Value.y * 0.4f, color.Value.z * 0.4f, color.Value.w * 0.2f);
        ImColor outer_glow = ImColor(color.Value.x * 0.2f, color.Value.y * 0.2f, color.Value.z * 0.2f, color.Value.w * 0.1f);
        
        // Draw glow layers first (background)
        float glow_layer_thickness = enhanced_thickness + 3.0f;
        m_drawing->AddLine(ImVec2(screen_corners[0].x, screen_corners[0].y), ImVec2(screen_corners[1].x, screen_corners[1].y), outer_glow, glow_layer_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[1].x, screen_corners[1].y), ImVec2(screen_corners[2].x, screen_corners[2].y), outer_glow, glow_layer_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[2].x, screen_corners[2].y), ImVec2(screen_corners[3].x, screen_corners[3].y), outer_glow, glow_layer_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[3].x, screen_corners[3].y), ImVec2(screen_corners[0].x, screen_corners[0].y), outer_glow, glow_layer_thickness);
        
        // Draw main lines with enhanced visibility
        m_drawing->AddLine(ImVec2(screen_corners[0].x, screen_corners[0].y), ImVec2(screen_corners[1].x, screen_corners[1].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[1].x, screen_corners[1].y), ImVec2(screen_corners[2].x, screen_corners[2].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[2].x, screen_corners[2].y), ImVec2(screen_corners[3].x, screen_corners[3].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[3].x, screen_corners[3].y), ImVec2(screen_corners[0].x, screen_corners[0].y), color, enhanced_thickness);
        
        // Draw top face (4-5-6-7) with enhanced visibility
        m_drawing->AddLine(ImVec2(screen_corners[4].x, screen_corners[4].y), ImVec2(screen_corners[5].x, screen_corners[5].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[5].x, screen_corners[5].y), ImVec2(screen_corners[6].x, screen_corners[6].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[6].x, screen_corners[6].y), ImVec2(screen_corners[7].x, screen_corners[7].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[7].x, screen_corners[7].y), ImVec2(screen_corners[4].x, screen_corners[4].y), color, enhanced_thickness);
        
        // Draw vertical lines connecting top and bottom faces with enhanced visibility
        m_drawing->AddLine(ImVec2(screen_corners[0].x, screen_corners[0].y), ImVec2(screen_corners[4].x, screen_corners[4].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[1].x, screen_corners[1].y), ImVec2(screen_corners[5].x, screen_corners[5].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[2].x, screen_corners[2].y), ImVec2(screen_corners[6].x, screen_corners[6].y), color, enhanced_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[3].x, screen_corners[3].y), ImVec2(screen_corners[7].x, screen_corners[7].y), color, enhanced_thickness);
        
        // Add subtle glow effect for better visibility
        ImColor glow_color = ImColor(color.Value.x * 0.4f, color.Value.y * 0.4f, color.Value.z * 0.4f, color.Value.w * 0.2f);
        float glow_thickness = enhanced_thickness + 2.0f;
        
        // Draw glow lines for better visibility
        m_drawing->AddLine(ImVec2(screen_corners[0].x, screen_corners[0].y), ImVec2(screen_corners[1].x, screen_corners[1].y), glow_color, glow_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[1].x, screen_corners[1].y), ImVec2(screen_corners[2].x, screen_corners[2].y), glow_color, glow_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[2].x, screen_corners[2].y), ImVec2(screen_corners[3].x, screen_corners[3].y), glow_color, glow_thickness);
        m_drawing->AddLine(ImVec2(screen_corners[3].x, screen_corners[3].y), ImVec2(screen_corners[0].x, screen_corners[0].y), glow_color, glow_thickness);
    }

    void draw_eye_tracer(ClientSoldierEntity* soldier, const ImColor& color, float distance, float thickness = 1.0f)
    {
        if (!IsValidPtr(soldier))
            return;
            
        // Get position of the head
        Vector3 head_pos;
        if (IsValidPtr(soldier->m_pRagdollComponent))
        {
            if (!soldier->m_pRagdollComponent->GetBone(UpdatePoseResultData::BONES::Head, head_pos))
                return;
        }
        else
            return;
            
        // Get transformation matrix of the soldier
        Matrix transform;
        soldier->GetTransform(&transform);
        
        // Direction of view is the forward vector from the transformation matrix
        Vector3 forward = Vector3(transform.m[2][0], transform.m[2][1], transform.m[2][2]);
        
        // Calculate the end point of the ray
        Vector3 end_pos = head_pos + (forward * distance);
        
        // Convert 3D coordinates to 2D for display on the screen
        Vector2 head_screen, end_screen;
        if (!world_to_screen(head_pos, head_screen) || !world_to_screen(end_pos, end_screen))
            return;
            
        // Draw line from head to the direction of view
        m_drawing->AddLine(ImVec2(head_screen.x, head_screen.y), ImVec2(end_screen.x, end_screen.y), color, thickness);
    }

    // For aim point
    Vector3 last_aim_point = Vector3(0, 0, 0);
    ULONGLONG last_aim_point_change_time = 0;
    bool aim_point_changed = false;

    void draw_aim_point()
    {
        // Aim Point, credit VincentVega, upgraded by nloginov
        if (g_settings.esp_draw_aim_point)
        {
            if (g_globals.g_has_pred_aim_point)
            {
                ULONGLONG current_time = GetTickCount64();

                // Check if aim point has changed
                if (g_globals.g_pred_aim_point.x != last_aim_point.x ||
                    g_globals.g_pred_aim_point.y != last_aim_point.y ||
                    g_globals.g_pred_aim_point.z != last_aim_point.z)
                {
                    // Aim point has changed, update last position and time
                    last_aim_point = g_globals.g_pred_aim_point;
                    last_aim_point_change_time = current_time;
                    aim_point_changed = true;
                }

                // Only draw if less than 500ms has passed since last change
                if (aim_point_changed && (current_time - last_aim_point_change_time <= 500))
                {
                    Vector2 aimpoint_screen_coords;
                    if (world_to_screen(g_globals.g_pred_aim_point, aimpoint_screen_coords))
                    {
                        // Calculate smooth alpha fade using cosine interpolation for smoother transitions
                        float time_elapsed = static_cast<float>(current_time - last_aim_point_change_time);
                        float alpha_multiplier = 1.0f;

                        if (time_elapsed > 300.0f)
                        {
                            // Smooth fade out using cosine interpolation
                            float fade_progress = (time_elapsed - 300.0f) / 200.0f;
                            fade_progress = std::clamp(fade_progress, 0.0f, 1.0f);
                            alpha_multiplier = 0.5f + 0.5f * cosf(fade_progress * PI); // Cosine curve
                        }

                        // Apply alpha to color with smooth transition
                        ImColor aim_point_color = g_settings.esp_aim_point_color;
                        aim_point_color.Value.w *= alpha_multiplier * alpha_multiplier; // Quadratic fade for more natural look

                        m_drawing->AddCircleFilled(
                            ImVec2(aimpoint_screen_coords.x, aimpoint_screen_coords.y),
                            g_settings.esp_aim_point_size,
                            aim_point_color);
                    }
                }
            }
            else
            {
                // Reset changed flag if we don't have a prediction point
                aim_point_changed = false;
            }
        }
    }

    // Advanced ESP Features - Placeholder for future implementation
// Note: These functions will be implemented with proper API calls in future updates


    void draw_esp()
    {
        if (!g_settings.esp) return;

        const auto game_context = ClientGameContext::GetInstance();
        if (!game_context) return;

        const auto player_manager = game_context->m_pPlayerManager;
        if (!player_manager) return;

        const auto local_player = player_manager->m_pLocalPlayer;
        if (!IsValidPtr(local_player)) return;

        const auto local_soldier = local_player->GetSoldier();
        if (!IsValidPtr(local_soldier)) return;

        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            const auto player = player_manager->m_ppPlayers[i];
            if (!IsValidPtr(player) || player == local_player)
                continue;

            // Player status
            uint64_t persona_id = player->m_onlineId.m_personaid;
            bool is_friend = plugins::is_friend(persona_id);
            bool is_blacklisted = plugins::is_blacklisted(persona_id);
            bool teammate = player->m_TeamId == local_player->m_TeamId;

            // Don't question such logic
            if (teammate && !g_settings.esp_draw_teammates && !(is_friend && g_settings.esp_draw_friends))
                continue;

            if (is_friend && !g_settings.esp_draw_friends && !(teammate && g_settings.esp_draw_teammates))
                continue;

            const auto soldier = player->GetSoldier();
            if (!IsValidPtr(soldier) || !soldier->IsAlive()) continue;

            TransformAABBStruct transform = get_transform(player);
            TransformAABBStruct local_transform = get_transform(local_player);

            Vector3 local_pos = Vector3(local_transform.Transform.m[3]);
            Vector3 pos = Vector3(transform.Transform.m[3]);
            Vector2 box_coords[2];

            const char* nickname = IsValidPtr(player->m_Name) ? player->m_Name : xorstr_("Unknown");

            // Streamer mode
            if (g_settings.streamer_mode)
            {
                if (persona_id != 0)
                {
                    if (streamer_personas.count(persona_id) <= 0)
                        streamer_personas[persona_id] = get_random_string(weird_nicknames);

                    nickname = streamer_personas[persona_id].empty() ? xorstr_("Unknown") : streamer_personas[persona_id].c_str();
                }
            }

            // Data
            float distance = get_distance(pos, local_pos);

            ClientVehicleEntity* vehicle = player->GetVehicle();
            if (vehicle && !g_settings.esp_draw_vehicles)
                continue;

            // https://www.unknowncheats.me/forum/battlefield-4-a/296540-vehicle-visibility-checks.html
            // You can do vehicle occlusion yourself, I will not be implementing it
            bool occluded = vehicle || (IsValidPtr(soldier) && soldier->m_Occluded);

            float health_player = 0.f, max_health_player = 0.f, health_vehicle = 0.f, max_health_vehicle = 0.f;
            if (IsValidPtr(vehicle))
            {
                if (IsValidPtr(vehicle->m_pHealthComp) && vehicle->m_pHealthComp->m_VehicleHealth)
                    health_vehicle = vehicle->m_pHealthComp->m_VehicleHealth;

                const auto data = get_vehicle_data(vehicle);
                if (IsValidPtrWithVTable(data))
                {
                    if (data->m_MaxHealth > 0.f)
                        max_health_vehicle = data->m_MaxHealth;
                }
            }
            else if (IsValidPtr(soldier->m_pHealthComp))
            {
                health_player = soldier->m_pHealthComp->m_Health;
                max_health_player = soldier->m_pHealthComp->m_MaxHealth;
            }

            if (get_box_coords(transform, &box_coords[0]) && distance <= g_settings.esp_distance)
            {
                float box_width = box_coords[1].x - box_coords[0].x;
                float box_height = box_coords[1].y - box_coords[0].y;
                float health = IsValidPtr(vehicle) ? health_vehicle : health_player;
                float max_health = IsValidPtr(vehicle) ? max_health_vehicle : max_health_player;

                if (g_settings.esp_draw_3d_box)
                {
                    ImColor box_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        box_color = g_settings.esp_friend_color;
                    else if (teammate)
                        box_color = g_settings.esp_teammate_color;
                    else
                        box_color = occluded ? g_settings.esp_3d_box_color_occluded : g_settings.esp_3d_box_color;
                    
                    draw_3d_box(transform, box_color, g_settings.esp_3d_box_thickness);
                }
                
                if (g_settings.esp_draw_eye_tracer && IsValidPtr(soldier))
                {
                    ImColor tracer_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        tracer_color = g_settings.esp_friend_color;
                    else if (teammate)
                        tracer_color = g_settings.esp_teammate_color;
                    else
                        tracer_color = occluded ? g_settings.esp_eye_tracer_color_occluded : g_settings.esp_eye_tracer_color;

                    draw_eye_tracer(soldier, tracer_color, g_settings.esp_eye_tracer_distance, g_settings.esp_eye_tracer_thickness);
                }
                
                // Enhanced ESP Features - Distance Indicator
                if (g_settings.esp_draw_distance)
                {
                    ImColor distance_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        distance_color = g_settings.esp_friend_color;
                    else if (teammate)
                        distance_color = g_settings.esp_teammate_color;
                    else
                        distance_color = occluded ? g_settings.esp_3d_box_color_occluded : g_settings.esp_3d_box_color;
                    
                    draw_distance_indicator(pos, distance_color, distance);
                }
                
                // Enhanced ESP Features - 3D Health Bar
                if (g_settings.esp_draw_health_bar_3d && max_health > 0.0f)
                {
                    float health_percentage = health / max_health;
                    ImColor health_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        health_color = g_settings.esp_friend_color;
                    else if (teammate)
                        health_color = g_settings.esp_teammate_color;
                    else
                        health_color = occluded ? g_settings.esp_3d_box_color_occluded : g_settings.esp_3d_box_color;
                    
                    draw_health_bar_3d(transform, health_percentage, health_color);
                }
                
                // Enhanced ESP Features - Weapon Indicator
                if (g_settings.esp_draw_weapon_indicator && IsValidPtr(soldier))
                {
                    ImColor weapon_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        weapon_color = g_settings.esp_friend_color;
                    else if (teammate)
                        weapon_color = g_settings.esp_teammate_color;
                    else
                        weapon_color = occluded ? g_settings.esp_3d_box_color_occluded : g_settings.esp_3d_box_color;
                    
                    draw_weapon_indicator(soldier, weapon_color);
                }
                
                // Enhanced ESP Features - Visibility Indicators
                if (g_settings.esp_draw_visibility_indicators)
                {
                    ImColor visibility_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        visibility_color = g_settings.esp_friend_color;
                    else if (teammate)
                        visibility_color = g_settings.esp_teammate_color;
                    else
                        visibility_color = occluded ? g_settings.esp_3d_box_color_occluded : g_settings.esp_3d_box_color;
                    
                    draw_visibility_indicators(pos, !occluded, visibility_color);
                }

                if (g_settings.esp_draw_box)
                {
                    if (g_settings.esp_box_fill)
                        m_drawing->DrawFillArea(box_coords[0].x, box_coords[0].y, box_coords[1].x - box_coords[0].x, box_coords[1].y - box_coords[0].y, g_settings.esp_box_fill_color, 0.0f);

                    ImColor box_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        box_color = g_settings.esp_friend_color;
                    else if (teammate)
                        box_color = g_settings.esp_teammate_color;
                    else
                        box_color = occluded ? g_settings.esp_box_color_occluded : g_settings.esp_box_color;

                    m_drawing->DrawEspBox(g_settings.esp_box_style, box_coords[0].x, box_coords[0].y, box_coords[1].x - box_coords[0].x, box_coords[1].y - box_coords[0].y, box_color.Value.x, box_color.Value.y, box_color.Value.z, box_color.Value.w);
                }

                if (g_settings.esp_draw_line)
                {
                    ImColor line_color = occluded ? g_settings.esp_line_color_occluded : g_settings.esp_line_color;
                    ImVec2 box_center = ImVec2(box_coords[0].x + box_width / 2.0f, box_coords[0].y + box_height / 2.0f);
                    ImVec2 drawing_from = ImVec2((float)g_globals.g_width / 2.0f, (float)g_globals.g_height / 2.0f);

                    switch (g_settings.esp_draw_line_from)
                    {
                    case 1:
                        drawing_from = ImVec2((float)g_globals.g_width / 2.0f, (float)g_globals.g_height);
                        break;
                    case 2:
                        drawing_from = ImVec2((float)g_globals.g_width / 2.0f, 0.f);
                        break;
                    case 3:
                        drawing_from = ImVec2(0, (float)g_globals.g_height / 2.0f);
                        break;
                    case 4:
                        drawing_from = ImVec2((float)g_globals.g_width, (float)g_globals.g_height / 2.0f);
                        break;
                    case 5:
                        drawing_from = ImVec2(0.f, 0.f);
                        break;
                    case 6:
                        drawing_from = ImVec2((float)g_globals.g_width, 0.f);
                        break;
                    case 7:
                        drawing_from = ImVec2(0, (float)g_globals.g_height);
                        break;
                    case 8:
                        drawing_from = ImVec2((float)g_globals.g_width, (float)g_globals.g_height);
                        break;
                    }

                    m_drawing->AddLine(drawing_from, box_center, line_color, g_settings.esp_line_thickness);
                }

                if (g_settings.esp_draw_health && max_health > 0.f)
                {
                    float health_ratio = health / max_health;
                    ImColor hb_color(BYTE(255 - max(health_ratio - 0.5f, 0.f) * 510), BYTE(255 - max(0.5f - health_ratio, 0.f) * 510), 0, 255);
                    float hb_width = max(box_width, 5.0f);
                    float hb_width_vertical = max(box_width / 50.0f, 3.0f);
                    float hb_height = max(box_width / 50.0f, 3.0f);
                    float hb_width_offset = max((hb_width - box_width) / 2, 0.f);
                    float hb_height_offset = 5.0f;
                    float hb_perc_width = hb_width * health_ratio;
                    float hb_perc_height = box_height * health_ratio;

                    switch (g_settings.esp_health_location)
                    {
                    case 0: // Bottom
                        m_drawing->DrawBoxOutline(box_coords[0].x - hb_width_offset, box_coords[1].y + hb_height_offset, hb_width, hb_height, ImColor(0, 0, 0, 255));
                        m_drawing->DrawFillArea(box_coords[0].x - hb_width_offset, box_coords[1].y + hb_height_offset, hb_perc_width, hb_height, hb_color);
                        
                        break;
                    case 1: // Top
                        m_drawing->DrawBoxOutline(box_coords[0].x - hb_width_offset, box_coords[0].y - hb_height_offset, hb_width, hb_height, ImColor(0, 0, 0, 255));
                        m_drawing->DrawFillArea(box_coords[0].x - hb_width_offset, box_coords[0].y - hb_height_offset, hb_perc_width, hb_height, hb_color);
                        
                        break;
                    case 2: // Left
                        m_drawing->DrawBoxOutline(box_coords[0].x - hb_height_offset - hb_width_vertical, box_coords[0].y, hb_width_vertical, box_height, ImColor(0, 0, 0, 255));
                        m_drawing->DrawFillArea(box_coords[0].x - hb_height_offset - hb_width_vertical, box_coords[0].y + box_height - hb_perc_height, hb_width_vertical, hb_perc_height, hb_color);
                        
                        break;
                    case 3: // Right
                        m_drawing->DrawBoxOutline(box_coords[1].x + hb_height_offset, box_coords[0].y, hb_width_vertical, box_height, ImColor(0, 0, 0, 255));
                        m_drawing->DrawFillArea(box_coords[1].x + hb_height_offset, box_coords[0].y + box_height - hb_perc_height, hb_width_vertical, hb_perc_height, hb_color);
                        break;
                    default:
                        break;
                    }
                }

                bool allow_text = !IsValidPtr(vehicle) || player->m_AttachedEntryId == 0;
                if (allow_text)
                {
                    ImColor text_color;
                    if (is_friend && !g_settings.esp_friend_color_to_tag)
                        text_color = g_settings.esp_friend_color;
                    else if (teammate)
                        text_color = g_settings.esp_teammate_color;
                    else
                        text_color = occluded ? g_settings.text_color_occluded : g_settings.text_color;

                    // Calculate position for text elements
                    float x, y;
                    int text_flags = FL_SHADOW;

                    // Count total text elements
                    const std::vector<bool> text_elements =
                    {
                        g_settings.esp_draw_name,
                        g_settings.esp_draw_distance,
                        is_friend && g_settings.esp_friend_color_to_tag,
                        is_blacklisted && g_settings.blacklist_tag_in_esp,
                        g_settings.esp_draw_vehicle_tag && IsValidPtr(vehicle)
                    };

                    __int64 text_count = std::count(text_elements.begin(), text_elements.end(), true);
                    float total_height = text_count * g_settings.esp_text_spacing;

                    // Set position based on configuration (0 = right, 1 = top, 2 = bottom)
                    switch (g_settings.esp_text_position)
                    {
                    case 1: // Top
                        x = box_coords[0].x + (box_coords[1].x - box_coords[0].x) / 2.f;
                        y = box_coords[0].y - total_height - 9.5f;
                        text_flags = FL_CENTER_X | FL_SHADOW;

                        break;
                    case 2: // Bottom
                        x = box_coords[0].x + (box_coords[1].x - box_coords[0].x) / 2.f;
                        y = box_coords[1].y + 2.5f;
                        text_flags = FL_CENTER_X | FL_SHADOW;

                        break;
                    default: // Right (0)
                        x = box_coords[1].x + 3.5f;
                        y = box_coords[0].y - 3.f;

                        if (g_settings.esp_draw_health && g_settings.esp_health_location == 3)
                            x += 9.0f;

                        break;
                    }

                    // Helper function to draw text if condition is met
                    auto draw_text_if = [&](bool condition, const char* text, const ImColor& color)
                    {
                        if (condition)
                        {
                            m_drawing->AddText(x, y, color, 14.f, text_flags, text);
                            y += g_settings.esp_text_spacing;
                        }
                    };

                    // Draw text elements using the helper function
                    draw_text_if(g_settings.esp_draw_name, nickname, text_color);
                    draw_text_if(g_settings.esp_draw_distance, fmt::format(xorstr_("{}m"), abs(ceil(distance))).c_str(), text_color);
                    draw_text_if(is_friend && g_settings.esp_friend_color_to_tag, xorstr_("FRND"), g_settings.esp_friend_color);
                    draw_text_if(is_blacklisted && g_settings.blacklist_tag_in_esp, xorstr_("BLKLST"), g_settings.blacklist_color);
                    draw_text_if(g_settings.esp_draw_vehicle_tag && IsValidPtr(vehicle), xorstr_("VEH"), g_settings.esp_additional_tags_color);
                }

                if (g_settings.skeleton)
                {
                    bool dots = g_settings.skeleton_use_dots && distance <= g_settings.skeleton_dots_distance;

                    RagdollComponent* ragdoll_component = soldier->m_pRagdollComponent;
                    if (IsValidPtr(ragdoll_component))
                    {
                        ImColor skeleton_color;
                        if (is_friend && !g_settings.esp_friend_color_to_tag)
                            skeleton_color = g_settings.esp_friend_color;
                        else if (teammate)
                            skeleton_color = g_settings.esp_teammate_color;
                        else
                            skeleton_color = occluded ? g_settings.skeleton_color_occluded : g_settings.skeleton_color;

                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::Head, UpdatePoseResultData::Neck, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::Neck, UpdatePoseResultData::Spine2, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::Spine2, UpdatePoseResultData::Spine1, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::Spine1, UpdatePoseResultData::Spine, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::Neck, UpdatePoseResultData::LeftShoulder, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::LeftShoulder, UpdatePoseResultData::LeftElbowRoll, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::RightShoulder, UpdatePoseResultData::RightElbowRoll, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::LeftElbowRoll, UpdatePoseResultData::LeftHand, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::RightElbowRoll, UpdatePoseResultData::RightHand, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::Spine, UpdatePoseResultData::RightKneeRoll, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::Spine, UpdatePoseResultData::LeftKneeRoll, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::RightKneeRoll, UpdatePoseResultData::RightFoot, dots);
                        draw_bone(skeleton_color, ragdoll_component, UpdatePoseResultData::LeftKneeRoll, UpdatePoseResultData::LeftFoot, dots);
                    }
                }
            }
        }
    }
}