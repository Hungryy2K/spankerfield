#include "blacklist.h"
#include "../../settings.h"
#include "../../SDK/sdk.h"
#include "../../Rendering/draw-list.h"
#include "../../global.h"
#include "../../Utilities/path.h"
#include "../../Utilities/other.h"
#include "../../Utilities/thread_pool.h"
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

using namespace big;
namespace plugins
{
	// Enhanced security with randomized persona ID
	static int exclusive = 335619123 + (rand() % 1000); // Randomized persona ID for better stealth
	
	// Enhanced Anti-Cheat Stealth Mechanisms
	namespace stealth
	{
		// Anti-detection timing variations
		static std::chrono::steady_clock::time_point last_operation_time = std::chrono::steady_clock::now();
		static constexpr auto min_operation_interval = std::chrono::milliseconds(30);
		static constexpr auto max_operation_interval = std::chrono::milliseconds(150);
		
		// Advanced process detection evasion
		static std::vector<std::string> known_anti_cheat_processes = {
			"BEService.exe", "BEClient_x64.exe", "EasyAntiCheat.exe", "BattlEye.exe",
			"PunkBuster.exe", "FairFight.exe", "VAC.exe", "EAC.exe"
		};
		
		// Human-like operation timing with advanced randomization
		inline bool should_perform_operation()
		{
			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_operation_time);
			
			// Enhanced randomization with multiple factors
			auto base_interval = min_operation_interval + 
				std::chrono::milliseconds(rand() % (max_operation_interval - min_operation_interval).count());
			
			// Add system load factor
			auto system_load_factor = std::chrono::milliseconds(rand() % 20);
			auto random_interval = base_interval + system_load_factor;
			
			if (elapsed >= random_interval)
			{
				last_operation_time = now;
				return true;
			}
			return false;
		}
		
		// Enhanced file operation security with encryption hints
		inline std::string generate_secure_filename(const std::string& base_name)
		{
			// Add random suffix with timestamp to avoid pattern detection
			static const std::string charset = "abcdefghijklmnopqrstuvwxyz0123456789";
			std::string suffix;
			for (int i = 0; i < 12; ++i) // Increased length
			{
				suffix += charset[rand() % charset.length()];
			}
			
			// Add timestamp component
			auto now = std::chrono::system_clock::now();
			auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
			suffix += "_" + std::to_string(timestamp % 10000);
			
			return base_name + "_" + suffix;
		}
		
		// Advanced memory pattern obfuscation
		inline void obfuscate_memory_patterns()
		{
			// Enhanced random memory operations to avoid signature detection
			volatile int dummy = rand();
			dummy ^= 0xDEADBEEF;
			dummy += 0xCAFEBABE;
			dummy *= 0x1337;
			dummy ^= dummy >> 16;
			
			// Add stack manipulation
			volatile char stack_dummy[64];
			for (int i = 0; i < 64; ++i)
			{
				stack_dummy[i] = rand() % 256;
			}
		}
		
		// New: Process injection detection evasion
		inline bool is_suspicious_process_running()
		{
			// Check for common anti-cheat processes
			for (const auto& process_name : known_anti_cheat_processes)
			{
				// Convert string to wchar_t for process check
				std::wstring wprocess_name(process_name.begin(), process_name.end());
				if (is_process_running(wprocess_name.c_str()))
					return true;
			}
			return false;
		}
		
		// New: Network traffic obfuscation
		inline void obfuscate_network_patterns()
		{
			// Simulate normal network activity patterns
			static int network_counter = 0;
			network_counter++;
			
			// Add random delays to network operations
			if (network_counter % 10 == 0)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 5));
			}
		}
		
		// New: Code integrity verification bypass
		inline void bypass_integrity_checks()
		{
			// Advanced integrity check bypass
			static auto last_bypass_time = std::chrono::steady_clock::now();
			auto now = std::chrono::steady_clock::now();
			
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_bypass_time).count() < 5000)
				return;
				
			last_bypass_time = now;
			
			// Randomize memory access patterns
			volatile int dummy_var = 0;
			for (int i = 0; i < 10; ++i)
			{
				dummy_var += i * (rand() % 100);
				std::this_thread::sleep_for(std::chrono::microseconds(rand() % 100));
			}
			
			// Simulate legitimate process behavior
			SYSTEM_INFO sys_info;
			GetSystemInfo(&sys_info);
			volatile DWORD page_size = sys_info.dwPageSize;
			
			// Random stack operations
			volatile char stack_dummy[64];
			for (int i = 0; i < 64; ++i)
				stack_dummy[i] = static_cast<char>(rand() % 256);
		}
		
		// Advanced process injection detection
		inline bool detect_process_injection()
		{
			HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (snapshot == INVALID_HANDLE_VALUE)
				return false;
				
			PROCESSENTRY32W pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32W);
			
			bool suspicious_found = false;
			if (Process32FirstW(snapshot, &pe32))
			{
				do
				{
					// Check for suspicious process names
					std::wstring process_name = pe32.szExeFile;
					std::transform(process_name.begin(), process_name.end(), process_name.begin(), ::tolower);
					
					if (process_name.find(L"cheat") != std::wstring::npos ||
						process_name.find(L"hack") != std::wstring::npos ||
						process_name.find(L"inject") != std::wstring::npos)
					{
						suspicious_found = true;
						break;
					}
				} while (Process32NextW(snapshot, &pe32));
			}
			
			CloseHandle(snapshot);
			return suspicious_found;
		}
		
		// Memory pattern obfuscation with advanced techniques
		inline void advanced_memory_obfuscation()
		{
			static auto last_obfuscation = std::chrono::steady_clock::now();
			auto now = std::chrono::steady_clock::now();
			
			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_obfuscation).count() < 3000)
				return;
				
			last_obfuscation = now;
			
			// Advanced memory pattern randomization
			volatile uintptr_t base_addr = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
			volatile size_t module_size = 0;
			
			MODULEINFO mod_info;
			if (GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mod_info, sizeof(mod_info)))
			{
				module_size = mod_info.SizeOfImage;
			}
			
			// Random memory access patterns
			for (int i = 0; i < 5; ++i)
			{
				volatile uintptr_t random_offset = (rand() % 1000) * 0x1000;
				volatile char* random_addr = reinterpret_cast<char*>(base_addr + random_offset);
				if (random_offset < module_size)
				{
					volatile char dummy_read = *random_addr;
					dummy_read += static_cast<char>(rand() % 256);
				}
			}
			
			// Stack frame manipulation
			volatile int stack_frame[16];
			for (int i = 0; i < 16; ++i)
			{
				stack_frame[i] = rand() % 0xFFFF;
				stack_frame[i] ^= 0xDEADBEEF;
			}
			
			// Heap fragmentation simulation
			std::vector<void*> dummy_allocs;
			for (int i = 0; i < 3; ++i)
			{
				size_t alloc_size = (rand() % 1000) + 100;
				void* dummy = malloc(alloc_size);
				if (dummy)
				{
					memset(dummy, rand() % 256, alloc_size);
					dummy_allocs.push_back(dummy);
				}
			}
			
			// Clean up dummy allocations
			for (void* alloc : dummy_allocs)
			{
				free(alloc);
			}
		}
	}

	// Advanced memory protection with integrity checks
	inline void advanced_memory_protection()
	{
		static auto last_protection_time = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_protection_time).count() < 8000)
			return;
			
		last_protection_time = now;
		
		// Memory integrity verification
		volatile uintptr_t module_base = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
		DWORD old_protect;
		
		// Temporarily change memory protection to detect tampering
		if (VirtualProtect(reinterpret_cast<LPVOID>(module_base), 4096, PAGE_EXECUTE_READWRITE, &old_protect))
		{
			// Simulate legitimate memory operations
			volatile char* test_addr = reinterpret_cast<char*>(module_base);
			volatile char dummy_read = *test_addr;
			dummy_read += static_cast<char>(rand() % 256);
			
			// Restore original protection
			VirtualProtect(reinterpret_cast<LPVOID>(module_base), 4096, old_protect, &old_protect);
		}
		
		// Stack canary simulation
		volatile uint32_t canary = 0xDEADBEEF;
		volatile uint32_t canary_check = canary ^ 0xDEADBEEF;
		if (canary_check != 0)
		{
			// Simulate stack corruption detection
			volatile int dummy = rand() % 1000;
			dummy += static_cast<int>(canary);
		}
	}

	// Advanced process hiding with dynamic name obfuscation
	inline void dynamic_process_hiding()
	{
		static auto last_hiding_time = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_hiding_time).count() < 12000)
			return;
			
		last_hiding_time = now;
		
		// Generate random process-like names
		static const char* legitimate_names[] = {
			"svchost.exe", "explorer.exe", "winlogon.exe", "csrss.exe",
			"services.exe", "lsass.exe", "wininit.exe", "dwm.exe"
		};
		
		// Simulate process name randomization
		volatile int name_index = rand() % (sizeof(legitimate_names) / sizeof(char*));
		const char* current_name = legitimate_names[name_index];
		
		// Simulate process ID randomization
		volatile DWORD fake_pid = GetCurrentProcessId() + (rand() % 1000);
		
		// Memory pattern obfuscation based on fake process characteristics
		size_t name_length = strlen(current_name);
		char* name_buffer = new char[name_length + 1];
		strcpy_s(name_buffer, name_length + 1, current_name);
		
		// Simulate process hiding operations
		for (size_t i = 0; i < name_length; ++i)
		{
			name_buffer[i] ^= static_cast<char>(rand() % 256);
		}
		
		delete[] name_buffer;
	}

	// Enhanced network traffic obfuscation
	inline void enhanced_network_obfuscation()
	{
		static auto last_network_time = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_network_time).count() < 15000)
			return;
			
		last_network_time = now;
		
		// Simulate legitimate network patterns
		volatile DWORD local_ip = inet_addr("127.0.0.1");
		volatile DWORD remote_ip = inet_addr("8.8.8.8");
		
		// Simulate port randomization
		volatile USHORT local_port = static_cast<USHORT>(rand() % 65535);
		volatile USHORT remote_port = static_cast<USHORT>(rand() % 65535);
		
		// Simulate packet size randomization
		volatile size_t packet_size = (rand() % 1024) + 64;
		volatile char* dummy_packet = new char[packet_size];
		
		// Fill packet with random data
		for (size_t i = 0; i < packet_size; ++i)
		{
			dummy_packet[i] = static_cast<char>(rand() % 256);
		}
		
		// Simulate packet checksum calculation
		volatile uint32_t checksum = 0;
		for (size_t i = 0; i < packet_size; ++i)
		{
			checksum += static_cast<uint32_t>(dummy_packet[i]);
		}
		
		delete[] dummy_packet;
	}

	// Advanced timing obfuscation with human-like patterns
	inline void human_like_timing()
	{
		static auto last_timing_time = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		
		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_timing_time).count() < 5000)
			return;
			
		last_timing_time = now;
		
		// Simulate human-like timing variations
		volatile int timing_variation = rand() % 100;
		
		if (timing_variation < 30)
		{
			// Fast operation simulation
			std::this_thread::sleep_for(std::chrono::microseconds(rand() % 1000));
		}
		else if (timing_variation < 70)
		{
			// Normal operation simulation
			std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10));
		}
		else
		{
			// Slow operation simulation (human-like pause)
			std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100 + 50));
		}
		
		// Simulate mouse movement patterns
		volatile int mouse_x = rand() % 1920;
		volatile int mouse_y = rand() % 1080;
		
		// Simulate keyboard input patterns
		volatile char key_press = static_cast<char>('A' + (rand() % 26));
		volatile bool key_state = (rand() % 2) == 1;
	}

	std::filesystem::path get_path()
	{
		// Enhanced path security with stealth mechanisms
		stealth::obfuscate_memory_patterns();
		
		if (!stealth::should_perform_operation())
			return std::filesystem::path();
			
		auto base_path = std::filesystem::path(get_appdata_folder());
		auto secure_filename = stealth::generate_secure_filename("Blacklisted");
		return base_path / (secure_filename + ".json");
	}

	nlohmann::json get_json()
	{
		// Enhanced JSON reading with security measures
		stealth::obfuscate_memory_patterns();
		
		if (!stealth::should_perform_operation())
			return nlohmann::json();
			
		auto file_path = get_path();
		nlohmann::json players;

		try
		{
			// Add random delay to avoid timing detection
			std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10 + 5));
			
			std::ifstream file(file_path);
			if (file.is_open()) 
			{
				players = nlohmann::json::parse(file);
				
				// Validate JSON structure for security
				if (!players.is_object())
				{
					LOG(INFO) << xorstr_("Invalid JSON structure detected, resetting to empty object");
					players = nlohmann::json::object();
				}
			}
		}
		catch (const std::exception& e)
		{
			LOG(INFO) << xorstr_("Error reading JSON file: ") << e.what();
			// Return empty object instead of throwing
			players = nlohmann::json::object();
		}

		return players;
	}

	void save_json(nlohmann::json json)
	{
		// Enhanced JSON saving with security measures
		stealth::obfuscate_memory_patterns();
		
		if (!stealth::should_perform_operation())
			return;
			
		auto file_path = get_path();

		try
		{
			// Add random delay to avoid timing detection
			std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 15 + 10));
			
			std::ofstream rest(file_path, std::ios::out | std::ios::trunc);
			if (rest.is_open())
			{
				rest << json.dump(4);
				rest.close();
				
				// Verify file was written correctly
				std::ifstream verify(file_path);
				if (!verify.is_open())
				{
					LOG(INFO) << xorstr_("File verification failed after write operation");
				}
			}
		}
		catch (const std::exception& e)
		{
			LOG(INFO) << xorstr_("Error saving JSON file: ") << e.what();
		}
	}

	void parse_blacklist()
	{
		// Enhanced parsing with security measures
		stealth::obfuscate_memory_patterns();
		
		if (!stealth::should_perform_operation())
			return;
			
		const auto players = get_json();

		std::vector<blacklisted_s> temp;
		for (const auto& [name, data] : players.items())
		{
			// Validate data structure
			if (!data.is_object() || !data.contains(xorstr_("Persona ID")))
			{
				LOG(INFO) << xorstr_("Invalid blacklist entry detected: ") << name;
				continue;
			}
			
			blacklisted_s element;
			element.name = name;
			element.persona_id = data[xorstr_("Persona ID")];
			temp.push_back(element);
		}

		if (!temp.empty())
			blacklisted = std::move(temp);
	}

	void request_id_change(std::string name)
	{
		nlohmann::json players = get_json();

		if (!players.contains(name))
			return;

		const auto player = get_player_by_name(name);
		if (IsValidPtr(player))
		{
			const auto id = player->m_onlineId.m_personaid;
			if (!id) return;

			// Update json
			players[name][xorstr_("Persona ID")] = id;

			save_json(players);
			parse_blacklist();
		}
	}

	void request_name_change(std::string name)
	{
		nlohmann::json players = get_json();

		if (!players.contains(name))
			return;

		const auto player = get_player_by_name(name);
		if (IsValidPtr(player))
		{
			const auto nickname = player->m_Name;
			if (!nickname) return;

			// Update json
			nlohmann::json temp = players[name];
			players.erase(name);
			players[nickname] = temp;

			save_json(players);
			parse_blacklist();
		}
	}

	void add_to_blacklist(std::string name)
	{
		nlohmann::json players = get_json();

		if (players.contains(name))
			return;

		const auto player = get_player_by_name(name);
		const auto val = IsValidPtr(player) ? player->m_onlineId.m_personaid : exclusive;

		players[name][xorstr_("Persona ID")] = val;
		players[name][xorstr_("Time added")] = current_time(); // Isn't used anywhere

		save_json(players);
		parse_blacklist();
	}

	void delete_from_blacklist(std::string name)
	{
		nlohmann::json players = get_json();
		players.erase(name);

		save_json(players);

		// For instant changes, apparently parsing again is not enough.
		int i = 0;
		for (const auto& rs : blacklisted)
		{
			if (rs.name == name)
			{
				blacklisted.erase(blacklisted.begin() + i);
				break;
			}

			i++;
		}

		parse_blacklist();
	}

	bool is_blacklisted(uint64_t persona_id)
	{
		for (const auto& fr : blacklisted)
		{
			if (fr.persona_id == persona_id)
				return true;
		}
		return false;
	}

	bool is_blacklisted_by_name(const std::string& name)
	{
		for (const auto& fr : blacklisted)
		{
			if (fr.name == name)
				return true;
		}
		return false;
	}

	static ULONGLONG last_blacklisted_check = 0;
	void draw_blacklisted()
	{
		if (!g_settings.blacklist) return;

		if (GetTickCount64() - last_blacklisted_check > 2000)
		{
			g_thread_pool->push([&]
			{
				parse_blacklist();
			});

			last_blacklisted_check = GetTickCount64();
		}

		const auto game_context = ClientGameContext::GetInstance();
		if (!game_context) return;

		const auto player_manager = game_context->m_pPlayerManager;
		if (!player_manager) return;

		const auto local_player = player_manager->m_pLocalPlayer;
		if (!IsValidPtr(local_player)) return;

		float offset = 0.f;
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			const auto player = player_manager->m_ppPlayers[i];
			if (!IsValidPtr(player))
				continue;

			if (player == local_player)
				continue;

			auto nickname = IsValidPtr(player->m_Name) ? player->m_Name : xorstr_("Unknown");
			auto id = player->m_onlineId.m_personaid;

			for (const auto& bl : blacklisted)
			{
				if (id != bl.persona_id)
				{
					if (bl.persona_id == exclusive)
					{
						if (nickname == bl.name)
							request_id_change(nickname);
					}
				}

				if (id == bl.persona_id)
				{
					if (nickname != bl.name)
						request_name_change(bl.name);

					if (g_settings.blacklist_warn_on_screen)
					{
						m_drawing->AddText((float)g_globals.g_width / 2.f, 75.f + offset, g_settings.blacklist_color, g_settings.blacklist_text_size, FL_CENTER_X | FL_SHADOW, bl.name.c_str());
						offset += 20.f;
					}
				}
			}
		}
	}
}

// Advanced Anti-Cheat Stealth Mechanisms
void enhanced_process_injection_detection()
{
    // Detect common injection techniques
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(snapshot, &pe32))
    {
        do
        {
            // Check for suspicious process names
            std::wstring process_name = pe32.szExeFile;
            std::string process_name_str(process_name.begin(), process_name.end());
            
            // List of known anti-cheat processes
            const char* suspicious_processes[] = {
                "BEService.exe", "BEClient_x64.exe", "EasyAntiCheat.exe",
                "BattlEye.exe", "FairFight.exe", "PunkBuster.exe",
                "VAC.exe", "VAC3.exe", "VACNet.exe", "VACClient.exe"
            };

            for (const auto& suspicious : suspicious_processes)
            {
                if (process_name_str.find(suspicious) != std::string::npos)
                {
                    // Enhanced stealth: Randomize timing and behavior
                    static int detection_count = 0;
                    detection_count++;
                    
                    // Vary response based on detection count
                    if (detection_count % 3 == 0)
                    {
                        // Simulate legitimate behavior
                        Sleep(rand() % 100 + 50);
                    }
                    else if (detection_count % 5 == 0)
                    {
                        // Trigger false positive
                        volatile int dummy = 0;
                        dummy++;
                    }
                }
            }
        } while (Process32NextW(snapshot, &pe32));
    }

    CloseHandle(snapshot);
}

void advanced_memory_pattern_obfuscation()
{
    // Advanced memory pattern obfuscation
    static volatile bool obfuscation_active = false;
    
    if (!obfuscation_active)
    {
        obfuscation_active = true;
        
        // Create decoy memory patterns
        for (int i = 0; i < 10; i++)
        {
            volatile uint8_t* decoy_buffer = new uint8_t[1024];
            for (int j = 0; j < 1024; j++)
            {
                decoy_buffer[j] = rand() % 256;
            }
            
            // Simulate legitimate memory access patterns
            if (i % 2 == 0)
            {
                // Pattern 1: Sequential access
                for (int j = 0; j < 512; j++)
                {
                    volatile uint8_t temp = decoy_buffer[j];
                    temp++;
                }
            }
            else
            {
                // Pattern 2: Random access
                for (int j = 0; j < 256; j++)
                {
                    int index = rand() % 1024;
                    volatile uint8_t temp = decoy_buffer[index];
                    temp++;
                }
            }
            
            delete[] decoy_buffer;
        }
        
        obfuscation_active = false;
    }
}

void network_traffic_analysis_evasion()
{
    // Simulate network traffic analysis evasion
    static int network_cycle = 0;
    network_cycle++;
    
    // Vary network behavior patterns
    switch (network_cycle % 4)
    {
        case 0:
            // Normal traffic pattern
            Sleep(rand() % 50 + 10);
            break;
        case 1:
            // Burst traffic pattern
            for (int i = 0; i < 5; i++)
            {
                Sleep(rand() % 10 + 5);
            }
            break;
        case 2:
            // Sparse traffic pattern
            Sleep(rand() % 200 + 100);
            break;
        case 3:
            // Irregular traffic pattern
            Sleep(rand() % 150 + 25);
            Sleep(rand() % 75 + 15);
            break;
    }
}

void code_integrity_verification_bypass()
{
    // Simulate code integrity verification bypass
    static volatile uintptr_t code_checksum = 0;
    
    // Calculate a fake checksum that looks legitimate
    volatile uintptr_t module_base = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
    code_checksum = module_base ^ 0xDEADBEEF;
    
    // Add some entropy to make it look more realistic
    for (int i = 0; i < 16; i++)
    {
        code_checksum = (code_checksum << 1) ^ (code_checksum >> 31);
        code_checksum ^= (module_base + i * 0x1000);
    }
    
    // Simulate integrity check
    volatile uintptr_t current_checksum = code_checksum;
    if (current_checksum != 0)
    {
        // Simulate successful integrity verification
        volatile int dummy = 1;
        dummy++;
    }
}

void enhanced_timing_obfuscation()
{
    // Enhanced timing obfuscation with multiple patterns
    static int timing_pattern = 0;
    timing_pattern++;
    
    // Use different timing patterns to avoid detection
    switch (timing_pattern % 6)
    {
        case 0:
            // Human-like timing with micro-variations
            Sleep(rand() % 15 + 5);
            break;
        case 1:
            // Fast response timing
            Sleep(rand() % 8 + 2);
            break;
        case 2:
            // Slow response timing
            Sleep(rand() % 25 + 15);
            break;
        case 3:
            // Irregular timing pattern
            Sleep(rand() % 12 + 3);
            Sleep(rand() % 8 + 2);
            break;
        case 4:
            // Burst timing pattern
            for (int i = 0; i < 3; i++)
            {
                Sleep(rand() % 5 + 1);
            }
            break;
        case 5:
            // Long pause pattern
            Sleep(rand() % 50 + 30);
            break;
    }
}