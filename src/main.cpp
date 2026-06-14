#include "auth/AuthManager.h"
#include "vault/VaultManager.h"
#include "storage/StorageManager.h"
#include "models/Credential.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <filesystem>

enum ViewState
{
    VIEW_SETUP,
    VIEW_LOGIN,
    VIEW_DASHBOARD
};

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// 🔐 Secure Memory Wiper for char buffers
static void secureWipe(char* buf, size_t size)
{
    volatile char* p = const_cast<volatile char*>(buf);
    while (size--)
    {
        *p++ = 0;
    }
}

// 🔐 Secure Memory Wiper for std::string
static void secureWipeString(std::string& s)
{
    if (s.empty()) return;
    volatile char* p = const_cast<volatile char*>(s.data());
    size_t size = s.size();
    while (size--)
    {
        *p++ = 0;
    }
    s.clear();
}

int main(int, char**)
{
    // Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Create window with OpenGL 2.0 context
    GLFWwindow* window = glfwCreateWindow(1024, 720, "Secure Password Manager", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Load settings (font scale)
    float fontScale = 1.0f;
    {
        std::ifstream settingsFile("data/settings.dat");
        if (settingsFile)
        {
            settingsFile >> fontScale;
        }
    }
    if (fontScale < 0.8f || fontScale > 2.5f) fontScale = 1.00f;
    io.FontGlobalScale = fontScale;

    // Load premium clean TrueType Font
    ImFontConfig fontConfig;
    fontConfig.OversampleH = 3;
    fontConfig.OversampleV = 3;
    
    ImFont* mainFont = nullptr;
    const char* fontPaths[] = {
        "C:\\Windows\\Fonts\\segoeui.ttf",
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\tahoma.ttf"
    };

    for (const char* path : fontPaths)
    {
        if (std::filesystem::exists(path))
        {
            mainFont = io.Fonts->AddFontFromFileTTF(path, 16.0f, &fontConfig);
            break;
        }
    }

    if (mainFont == nullptr)
    {
        io.Fonts->AddFontDefault();
    }

    // Setup Premium Dark Style
    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.WindowBorderSize = 0.0f; // Disable root window borders for a clean flat app look
    style.FrameBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.FramePadding = ImVec2(10.0f, 8.0f);
    style.ItemSpacing = ImVec2(10.0f, 12.0f);
    style.ScrollbarRounding = 12.0f;

    // Deep Charcoal / Soft Blue-Gray color palette
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.11f, 1.00f);      // Dark Charcoal App Background
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);       // Card background
    style.Colors[ImGuiCol_Border] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);        // Muted gray-blue outline
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);       // Inputs background
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.28f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.32f, 1.00f);
    
    style.Colors[ImGuiCol_Button] = ImVec4(0.18f, 0.42f, 0.70f, 1.00f);        // Muted Royal Blue Button
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.52f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.14f, 0.35f, 0.60f, 1.00f);
    
    style.Colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.35f, 1.00f);        // Table Headers
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.32f, 0.45f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.16f, 0.20f, 0.30f, 1.00f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.95f, 1.00f);          // Clear off-white text

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Instantiate Managers
    AuthManager authManager;
    VaultManager vaultManager;
    StorageManager storageManager;

    // View State Routing
    ViewState viewState = VIEW_LOGIN;
    if (!authManager.vaultExists())
    {
        viewState = VIEW_SETUP;
    }

    std::string masterPassword = "";

    // UI Input Buffers
    char setupPass1[128] = "";
    char setupPass2[128] = "";
    char loginPass[128] = "";
    char searchFilter[128] = "";

    char addSite[128] = "";
    char addUser[128] = "";
    char addPass[128] = "";

    std::string errorMessage = "";
    std::string successMessage = "";

    // Status message timer state
    double messageTimer = 0.0;
    std::string prevSuccess = "";
    std::string prevError = "";

    // Set to track visible passwords in the table (by site + "||" + user)
    std::set<std::string> visiblePasswords;

    // Main event loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // 🔹 Manage status message durations (5-second timeout)
        if (successMessage != prevSuccess || errorMessage != prevError)
        {
            messageTimer = glfwGetTime();
            prevSuccess = successMessage;
            prevError = errorMessage;
        }

        if (!successMessage.empty() || !errorMessage.empty())
        {
            if (glfwGetTime() - messageTimer > 5.0)
            {
                successMessage = "";
                errorMessage = "";
                prevSuccess = "";
                prevError = "";
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        
        // 🔹 ONE SINGLE Full screen app window covering the entire viewport
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)display_w, (float)display_h));
        ImGuiWindowFlags app_flags = ImGuiWindowFlags_NoTitleBar | 
                                     ImGuiWindowFlags_NoResize | 
                                     ImGuiWindowFlags_NoMove | 
                                     ImGuiWindowFlags_NoCollapse | 
                                     ImGuiWindowFlags_NoBringToFrontOnFocus | 
                                     ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("AppWindow", NULL, app_flags);

        // Helper Lambda to draw Font Size Control
        auto DrawFontSettings = [&]() {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "DISPLAY SETTINGS");
            ImGui::Spacing();
            
            ImGui::PushItemWidth(-1);
            if (ImGui::SliderFloat("##FontScaleSlider", &fontScale, 0.8f, 2.0f, "Font Size: %.1fx"))
            {
                io.FontGlobalScale = fontScale;
                std::ofstream outFile("data/settings.dat");
                if (outFile)
                {
                    outFile << fontScale;
                }
            }
            ImGui::PopItemWidth();
        };

        if (viewState == VIEW_SETUP)
        {
            // Center the setup card inside AppWindow
            float card_w = 460.0f * fontScale;
            float card_h = 390.0f * fontScale;
            
            ImGui::SetCursorPos(ImVec2(((float)display_w - card_w) * 0.5f, ((float)display_h - card_h) * 0.5f));
            
            // Draw card as a flat borderless child window
            ImGui::BeginChild("SetupCard", ImVec2(card_w, card_h), true, ImGuiWindowFlags_None);
            
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.00f, 1.00f), "FIRST TIME SETUP");
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::TextWrapped("Create a master password to encrypt and secure your password vault database.");
            ImGui::Spacing();

            ImGui::InputText("Master Password", setupPass1, IM_ARRAYSIZE(setupPass1), ImGuiInputTextFlags_Password);
            ImGui::InputText("Confirm Password", setupPass2, IM_ARRAYSIZE(setupPass2), ImGuiInputTextFlags_Password);

            ImGui::Spacing();
            if (ImGui::Button("Create Vault", ImVec2(-1, 35.0f * fontScale)))
            {
                std::string p1(setupPass1);
                std::string p2(setupPass2);

                if (p1.empty())
                {
                    errorMessage = "Password cannot be empty.";
                    successMessage = "";
                }
                else if (p1 != p2)
                {
                    errorMessage = "Passwords do not match.";
                    successMessage = "";
                }
                else
                {
                    if (authManager.createVaultPassword(p1))
                    {
                        masterPassword = p1;
                        storageManager.saveVault(std::vector<Credential>(), masterPassword);
                        viewState = VIEW_LOGIN;
                        errorMessage = "";
                        successMessage = "Vault created successfully! Please login.";
                        secureWipe(setupPass1, sizeof(setupPass1));
                        secureWipe(setupPass2, sizeof(setupPass2));
                    }
                    else
                    {
                        errorMessage = "Failed to write vault configuration files.";
                        successMessage = "";
                    }
                }
            }

            if (!errorMessage.empty())
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.9f, 0.35f, 0.35f, 1.0f), errorMessage.c_str());
            }

            DrawFontSettings();

            ImGui::EndChild();
        }
        else if (viewState == VIEW_LOGIN)
        {
            // Center the login card inside AppWindow
            float card_w = 460.0f * fontScale;
            float card_h = 370.0f * fontScale;

            ImGui::SetCursorPos(ImVec2(((float)display_w - card_w) * 0.5f, ((float)display_h - card_h) * 0.5f));
            
            // Draw card as a flat child window
            ImGui::BeginChild("LoginCard", ImVec2(card_w, card_h), true, ImGuiWindowFlags_None);

            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.00f, 1.00f), "SECURE LOGIN");
            ImGui::Separator();
            ImGui::Spacing();

            if (!successMessage.empty())
            {
                ImGui::TextColored(ImVec4(0.40f, 0.85f, 0.40f, 1.0f), successMessage.c_str());
                ImGui::Spacing();
            }

            if (authManager.isLocked())
            {
                ImGui::TextColored(ImVec4(0.90f, 0.35f, 0.35f, 1.0f), "Too many failed attempts. Lockout active.");
                long long remainingTime = authManager.getLockoutRemainingTime();
                ImGui::Text("Please wait %lld seconds and try again.", remainingTime);
            }
            else
            {
                ImGui::Text("Enter Master Password:");
                ImGui::Spacing();

                bool inputSubmitted = ImGui::InputText("##Password", loginPass, IM_ARRAYSIZE(loginPass), ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue);

                ImGui::Spacing();

                if (ImGui::Button("Unlock", ImVec2(-1, 35.0f * fontScale)) || inputSubmitted)
                {
                    std::string enteredPassword(loginPass);
                    if (authManager.verifyVaultPassword(enteredPassword))
                    {
                        masterPassword = enteredPassword;
                        errorMessage = "";
                        successMessage = "";

                        auto loaded = storageManager.loadVault(masterPassword);
                        vaultManager.clearVault();
                        if (loaded)
                        {
                            for (const auto& c : *loaded)
                            {
                                vaultManager.addCredential(c);
                            }
                        }
                        viewState = VIEW_DASHBOARD;
                        secureWipe(loginPass, sizeof(loginPass));
                    }
                    else
                    {
                        errorMessage = "Invalid password.";
                        successMessage = "";
                    }
                }
            }

            if (!errorMessage.empty() && !authManager.isLocked())
            {
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.90f, 0.35f, 0.35f, 1.0f), errorMessage.c_str());
            }

            DrawFontSettings();

            ImGui::EndChild();
        }
        else if (viewState == VIEW_DASHBOARD)
        {
            // Title line and lock button (Rendered directly in the main full screen container)
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.00f, 1.00f), "PASSWORD MANAGER VAULT");
            
            // Align the lock button to the right, calculating size dynamically
            float lock_btn_width = ImGui::CalcTextSize("Lock Vault").x + style.FramePadding.x * 2.0f;
            ImGui::SameLine(ImGui::GetContentRegionMax().x - lock_btn_width - style.WindowPadding.x);
            if (ImGui::Button("Lock Vault", ImVec2(0.0f, 26.0f)))
            {
                // Auto save, wipe keys, lock
                storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword);
                vaultManager.clearVault();
                secureWipeString(masterPassword);
                viewState = VIEW_LOGIN;
                successMessage = "Vault saved and locked successfully.";
                errorMessage = "";
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // 🔹 Split Layout: Left side for viewing/searching, Right side for adding entries and settings
            float right_pane_w = 320.0f * fontScale;
            float left_pane_w = ImGui::GetContentRegionAvail().x - right_pane_w - 15.0f;
            
            // --- LEFT PANEL ---
            ImGui::BeginChild("LeftPane", ImVec2(left_pane_w, 0.0f), true, ImGuiWindowFlags_None);
            
            // Search Bar & Save button (Aligned dynamically to prevent overlapping)
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Filter:");
            ImGui::SameLine();
            
            float save_btn_width = ImGui::CalcTextSize("Save Changes").x + style.FramePadding.x * 2.0f;
            float avail_width = ImGui::GetContentRegionAvail().x;
            
            // Set search input width to take up all remaining width except the Save button and spacing
            ImGui::PushItemWidth(avail_width - save_btn_width - 15.0f);
            ImGui::InputText("##Search", searchFilter, IM_ARRAYSIZE(searchFilter));
            ImGui::PopItemWidth();

            ImGui::SameLine();
            if (ImGui::Button("Save Changes", ImVec2(0.0f, 25.0f)))
            {
                if (storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword))
                {
                    successMessage = "Vault saved successfully.";
                    errorMessage = "";
                }
                else
                {
                    errorMessage = "Failed to save vault.";
                    successMessage = "";
                }
            }

            ImGui::Spacing();

            // Messages inside dashboard
            if (!successMessage.empty())
            {
                ImGui::TextColored(ImVec4(0.40f, 0.85f, 0.40f, 1.0f), successMessage.c_str());
                ImGui::Spacing();
            }
            if (!errorMessage.empty())
            {
                ImGui::TextColored(ImVec4(0.90f, 0.35f, 0.35f, 1.0f), errorMessage.c_str());
                ImGui::Spacing();
            }

            // Table of Credentials (column actions has a safer size scaling with fontScale)
            if (ImGui::BeginTable("CredentialsTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0.0f, -10.0f)))
            {
                ImGui::TableSetupColumn("Website", ImGuiTableColumnFlags_WidthStretch, 2.0f);
                ImGui::TableSetupColumn("Username", ImGuiTableColumnFlags_WidthStretch, 2.0f);
                ImGui::TableSetupColumn("Password", ImGuiTableColumnFlags_WidthStretch, 3.0f);
                ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 155.0f * fontScale);
                ImGui::TableHeadersRow();

                std::string filterKey(searchFilter);
                std::transform(filterKey.begin(), filterKey.end(), filterKey.begin(), ::tolower);

                const auto& credentials = vaultManager.getAllCredentials();
                for (size_t i = 0; i < credentials.size(); i++)
                {
                    const auto& c = credentials[i];

                    std::string site = c.getWebsite();
                    std::string user = c.getUsername();
                    std::string siteLower = site;
                    std::string userLower = user;
                    std::transform(siteLower.begin(), siteLower.end(), siteLower.begin(), ::tolower);
                    std::transform(userLower.begin(), userLower.end(), userLower.begin(), ::tolower);

                    // Search matching
                    if (!filterKey.empty() && 
                        siteLower.find(filterKey) == std::string::npos && 
                        userLower.find(filterKey) == std::string::npos)
                    {
                        continue;
                    }

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", site.c_str());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", user.c_str());

                    ImGui::TableSetColumnIndex(2);
                    std::string key = site + "||" + user;
                    bool visible = (visiblePasswords.find(key) != visiblePasswords.end());

                    if (visible)
                    {
                        ImGui::Text("%s", c.getPassword().c_str());
                    }
                    else
                    {
                        std::string masked(c.getPassword().length(), '*');
                        if (masked.empty()) masked = "********";
                        ImGui::Text("%s", masked.c_str());
                    }

                    ImGui::TableSetColumnIndex(3);
                    
                    // Show/Hide Toggle Button (Auto-sized)
                    std::string showLabel = visible ? "Hide##" + std::to_string(i) : "Show##" + std::to_string(i);
                    if (ImGui::Button(showLabel.c_str(), ImVec2(0.0f, 20.0f)))
                    {
                        if (visible)
                            visiblePasswords.erase(key);
                        else
                            visiblePasswords.insert(key);
                    }

                    ImGui::SameLine();

                    // Delete Button (Auto-sized)
                    std::string delLabel = "Delete##" + std::to_string(i);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.25f, 0.25f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.60f, 0.10f, 0.10f, 1.0f));
                    
                    if (ImGui::Button(delLabel.c_str(), ImVec2(0.0f, 20.0f)))
                    {
                        vaultManager.removeCredential(site, user);
                        // Save immediately on deletion for safety
                        storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword);
                    }
                    ImGui::PopStyleColor(3);
                }

                ImGui::EndTable();
            }

            ImGui::EndChild();

            ImGui::SameLine();

            // --- RIGHT PANEL (ADD ENTRY FORM & DISPLAY SETTINGS) ---
            ImGui::BeginChild("RightPane", ImVec2(right_pane_w, 0.0f), true, ImGuiWindowFlags_None);
            
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.35f, 0.75f, 1.00f, 1.00f), "ADD NEW CREDENTIAL");
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("Website Name:");
            ImGui::InputText("##WebsiteField", addSite, IM_ARRAYSIZE(addSite));

            ImGui::Spacing();
            ImGui::Text("Username / Email:");
            ImGui::InputText("##UsernameField", addUser, IM_ARRAYSIZE(addUser));

            ImGui::Spacing();
            ImGui::Text("Password:");
            ImGui::InputText("##PasswordField", addPass, IM_ARRAYSIZE(addPass), ImGuiInputTextFlags_Password);

            ImGui::Spacing();
            ImGui::Spacing();
            
            if (ImGui::Button("Add Credential", ImVec2(-1, 35.0f * fontScale)))
            {
                std::string site(addSite);
                std::string user(addUser);
                std::string pass(addPass);

                if (site.empty() || user.empty() || pass.empty())
                {
                    errorMessage = "All fields are required.";
                    successMessage = "";
                }
                else
                {
                    Credential credential(site, user, pass);
                    if (vaultManager.addCredential(credential))
                    {
                        // Auto save
                        storageManager.saveVault(vaultManager.getAllCredentials(), masterPassword);
                        successMessage = "Credential added & vault saved!";
                        errorMessage = "";
                        
                        // Clear add buffers securely
                        memset(addSite, 0, sizeof(addSite));
                        memset(addUser, 0, sizeof(addUser));
                        secureWipe(addPass, sizeof(addPass));
                    }
                    else
                    {
                        errorMessage = "Credential already exists for this site/username.";
                        successMessage = "";
                    }
                }
            }

            DrawFontSettings();

            ImGui::EndChild();
        }

        ImGui::End(); // AppWindow

        // Rendering
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.09f, 0.09f, 0.11f, 1.0f); // Clean flat color matching app background
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup and Shutdown
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}