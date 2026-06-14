#ifndef WEB_PORTAL_H
#define WEB_PORTAL_H

const char* const INDEX_HTML_CONTENT = R"rawhtml(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Secure Password Manager</title>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600&family=Outfit:wght@500;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --app-scale: 1.0;
            --bg-dark: #09090b;
            --card-bg: rgba(24, 24, 27, 0.65);
            --card-border: rgba(255, 255, 255, 0.08);
            --text-primary: #fafafa;
            --text-secondary: #a1a1aa;
            --accent-blue: #3b82f6;
            --accent-violet: #6d28d9;
            --accent-red: #ef4444;
            --success-green: #22c55e;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            transition: background-color 0.2s, border-color 0.2s, transform 0.1s;
        }

        body {
            background: radial-gradient(circle at 15% 20%, rgba(59, 130, 246, 0.12), transparent 45%),
                        radial-gradient(circle at 85% 80%, rgba(109, 40, 217, 0.12), transparent 45%),
                        var(--bg-dark);
            background-attachment: fixed;
            color: var(--text-primary);
            font-family: 'Inter', sans-serif;
            font-size: calc(15px * var(--app-scale));
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            overflow-x: hidden;
        }

        /* Containers */
        .app-container {
            width: 100%;
            max-width: 1100px;
            padding: 24px;
            display: flex;
            justify-content: center;
            flex-direction: column;
            align-items: center;
        }

        .glass-card {
            background: var(--card-bg);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 1px solid var(--card-border);
            border-radius: 14px;
            box-shadow: 0 12px 40px 0 rgba(0, 0, 0, 0.4);
            padding: 32px;
            width: 100%;
            max-width: 480px;
            animation: fadeIn 0.4s ease-out;
            text-align: center;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(12px); }
            to { opacity: 1; transform: translateY(0); }
        }

        /* Typography */
        h1, h2, h3 {
            font-family: 'Outfit', sans-serif;
            font-weight: 600;
            letter-spacing: -0.02em;
        }

        .title-accent {
            background: linear-gradient(135deg, #60a5fa, #c084fc);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }

        p.subtitle {
            color: var(--text-secondary);
            margin-top: 8px;
            margin-bottom: 24px;
            line-height: 1.5;
        }

        /* Forms & Inputs */
        .form-group {
            margin-bottom: 20px;
            display: flex;
            flex-direction: column;
            gap: 8px;
            text-align: left;
        }

        label {
            font-weight: 500;
            color: var(--text-secondary);
            font-size: 0.9em;
        }

        input {
            background: rgba(15, 15, 20, 0.6);
            border: 1px solid var(--card-border);
            border-radius: 8px;
            padding: 12px 16px;
            color: var(--text-primary);
            font-family: 'Inter', sans-serif;
            font-size: 1em;
            width: 100%;
        }

        input:focus {
            outline: none;
            border-color: var(--accent-blue);
            box-shadow: 0 0 0 4px rgba(59, 130, 246, 0.15);
            background: rgba(15, 15, 20, 0.8);
        }

        /* Buttons */
        .btn {
            display: inline-flex;
            align-items: center;
            justify-content: center;
            font-family: 'Outfit', sans-serif;
            font-weight: 600;
            border-radius: 8px;
            padding: 12px 24px;
            cursor: pointer;
            border: none;
            text-align: center;
            width: 100%;
            font-size: 1.05em;
            gap: 8px;
        }

        .btn-primary {
            background: linear-gradient(135deg, var(--accent-blue), var(--accent-violet));
            color: white;
            box-shadow: 0 4px 20px rgba(99, 102, 241, 0.15);
        }

        .btn-primary:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 24px rgba(99, 102, 241, 0.3);
        }

        .btn-secondary {
            background: rgba(255, 255, 255, 0.06);
            color: var(--text-primary);
            border: 1px solid var(--card-border);
            font-size: 0.9em;
            padding: 8px 16px;
            width: auto;
        }

        .btn-secondary:hover {
            background: rgba(255, 255, 255, 0.12);
        }

        .btn-danger {
            background: rgba(239, 68, 68, 0.15);
            color: #fca5a5;
            border: 1px solid rgba(239, 68, 68, 0.3);
            font-size: 0.85em;
            padding: 6px 12px;
            width: auto;
        }

        .btn-danger:hover {
            background: rgba(239, 68, 68, 0.30);
        }

        /* Notifications & Banner */
        .banner {
            border-radius: 8px;
            padding: 12px 16px;
            margin-bottom: 20px;
            font-size: 0.95em;
            display: none;
            text-align: left;
            width: 100%;
            max-width: 480px;
            animation: slideDown 0.2s ease-out;
        }

        @keyframes slideDown {
            from { transform: translateY(-10px); opacity: 0; }
            to { transform: translateY(0); opacity: 1; }
        }

        .banner-success {
            background: rgba(34, 197, 94, 0.15);
            color: #86efac;
            border: 1px solid rgba(34, 197, 94, 0.3);
            display: block;
        }

        .banner-error {
            background: rgba(239, 68, 68, 0.15);
            color: #fca5a5;
            border: 1px solid rgba(239, 68, 68, 0.3);
            display: block;
        }

        .hidden {
            display: none !important;
        }

        /* Lockout counter style */
        .lockout-box {
            text-align: center;
            padding: 16px;
            background: rgba(239, 68, 68, 0.08);
            border-radius: 8px;
            border: 1px solid rgba(239, 68, 68, 0.15);
            margin-bottom: 20px;
        }

        .lockout-timer {
            font-size: 2em;
            font-family: 'Outfit', sans-serif;
            font-weight: 700;
            color: var(--accent-red);
            margin-top: 8px;
        }

        /* Settings Panel */
        .settings-section {
            margin-top: 24px;
            border-top: 1px solid var(--card-border);
            padding-top: 20px;
        }

        .settings-title {
            font-size: 0.8em;
            text-transform: uppercase;
            letter-spacing: 0.08em;
            color: var(--text-secondary);
            margin-bottom: 12px;
            font-weight: 600;
            text-align: left;
        }

        .scale-slider-container {
            display: flex;
            align-items: center;
            gap: 12px;
        }

        .scale-slider {
            -webkit-appearance: none;
            appearance: none;
            height: 6px;
            border-radius: 3px;
            background: rgba(255, 255, 255, 0.12);
            outline: none;
            flex-grow: 1;
            cursor: pointer;
        }

        .scale-slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 16px;
            height: 16px;
            border-radius: 50%;
            background: var(--accent-blue);
            cursor: pointer;
            box-shadow: 0 0 8px rgba(59, 130, 246, 0.5);
        }

        .scale-label {
            font-size: 0.9em;
            color: var(--text-secondary);
            min-width: 45px;
            text-align: right;
        }

        /* ==================== DASHBOARD VIEW ==================== */
        .dashboard-container {
            width: 100%;
            max-width: 1200px;
            animation: fadeIn 0.4s ease-out;
            display: flex;
            flex-direction: column;
            gap: 20px;
        }

        .dashboard-header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            background: rgba(18, 18, 24, 0.8);
            border: 1px solid var(--card-border);
            border-radius: 12px;
            padding: 16px 28px;
            backdrop-filter: blur(12px);
        }

        .dashboard-layout {
            display: grid;
            grid-template-columns: 1fr 340px;
            gap: 20px;
        }

        @media (max-width: 900px) {
            .dashboard-layout {
                grid-template-columns: 1fr;
            }
        }

        .left-pane, .right-pane {
            background: rgba(18, 18, 24, 0.55);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 1px solid var(--card-border);
            border-radius: 12px;
            padding: 24px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);
        }

        .left-pane {
            display: flex;
            flex-direction: column;
            gap: 16px;
        }

        .controls-row {
            display: flex;
            gap: 12px;
            align-items: center;
            justify-content: space-between;
        }

        .search-container {
            display: flex;
            align-items: center;
            gap: 8px;
            flex-grow: 1;
            max-width: 320px;
        }

        .search-container input {
            padding: 8px 12px;
            font-size: 0.9em;
        }

        /* Custom Scrollable Table */
        .table-wrapper {
            max-height: 480px;
            overflow-y: auto;
            border: 1px solid var(--card-border);
            border-radius: 8px;
            background: rgba(10, 10, 12, 0.3);
        }

        table {
            width: 100%;
            border-collapse: collapse;
            text-align: left;
        }

        th, td {
            padding: 12px 16px;
            font-size: 0.95em;
        }

        th {
            background: rgba(20, 20, 25, 0.8);
            border-bottom: 1px solid var(--card-border);
            color: var(--text-secondary);
            font-weight: 600;
            position: sticky;
            top: 0;
            z-index: 10;
        }

        tr {
            border-bottom: 1px solid rgba(255, 255, 255, 0.03);
        }

        tr:last-child {
            border-bottom: none;
        }

        tr:hover {
            background: rgba(255, 255, 255, 0.02);
        }

        .cell-mono {
            font-family: 'Courier New', Courier, monospace;
            font-weight: bold;
            letter-spacing: 0.08em;
        }

        .cell-actions {
            display: flex;
            gap: 8px;
            justify-content: flex-end;
        }

        .table-btn {
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid var(--card-border);
            color: var(--text-primary);
            border-radius: 6px;
            padding: 5px 10px;
            font-size: 0.82em;
            cursor: pointer;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            min-width: 48px;
        }

        .table-btn:hover {
            background: rgba(255, 255, 255, 0.12);
        }

        .table-btn-danger {
            background: rgba(239, 68, 68, 0.1);
            border: 1px solid rgba(239, 68, 68, 0.25);
            color: #fca5a5;
        }

        .table-btn-danger:hover {
            background: rgba(239, 68, 68, 0.25);
        }

        /* Modal Overlay */
        .modal-overlay {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.6);
            backdrop-filter: blur(8px);
            z-index: 100;
            display: flex;
            align-items: center;
            justify-content: center;
            animation: fadeIn 0.2s ease-out;
        }

        .modal-content {
            max-width: 400px !important;
            padding: 24px !important;
            box-shadow: 0 20px 50px rgba(0, 0, 0, 0.5) !important;
        }

        /* 📋 Clipboard Progress Bar */
        .clipboard-bar-container {
            width: 100%;
            max-width: 1200px;
            height: 4px;
            background: rgba(255, 255, 255, 0.05);
            border-radius: 2px;
            overflow: hidden;
            margin-bottom: 12px;
            display: none;
        }

        .clipboard-bar-fill {
            height: 100%;
            width: 100%;
            background: linear-gradient(90deg, var(--accent-blue), var(--accent-violet));
            transition: width 0.1s linear;
        }

        /* 🔎 Credential row hover cursor */
        #credentials-table tbody tr {
            cursor: pointer;
        }

        #credentials-table tbody tr td:last-child {
            cursor: default; /* actions cell shouldn't show row pointer */
        }

        /* 🛡️ TOTP progress indicator */
        .totp-progress-container {
            display: flex;
            align-items: center;
            gap: 8px;
            margin-top: 6px;
        }

        .totp-progress-bar {
            height: 6px;
            background: rgba(255, 255, 255, 0.1);
            border-radius: 3px;
            flex-grow: 1;
            overflow: hidden;
        }

        .totp-progress-fill {
            height: 100%;
            width: 100%;
            background: var(--success-green);
            transition: width 1s linear, background-color 0.5s;
        }

        /* 📝 Textarea for notes */
        textarea {
            background: rgba(15, 15, 20, 0.6);
            border: 1px solid var(--card-border);
            border-radius: 8px;
            padding: 12px 16px;
            color: var(--text-primary);
            font-family: 'Inter', sans-serif;
            font-size: 1em;
            width: 100%;
            resize: vertical;
            min-height: 80px;
        }

        textarea:focus {
            outline: none;
            border-color: var(--accent-blue);
            box-shadow: 0 0 0 4px rgba(59, 130, 246, 0.15);
            background: rgba(15, 15, 20, 0.8);
        }

        /* 📎 Attachment file tag */
        .file-tag {
            background: rgba(59, 130, 246, 0.1);
            border: 1px solid rgba(59, 130, 246, 0.25);
            color: #93c5fd;
            border-radius: 6px;
            padding: 4px 10px;
            font-size: 0.85em;
            display: inline-flex;
            align-items: center;
            gap: 6px;
            margin-top: 6px;
        }
    </style>
</head>
<body>

    <div class="app-container">

        <!-- Banner Alert -->
        <div id="alert-banner" class="banner"></div>

        <!-- 🔒 Setup Card -->
        <div id="setup-view" class="glass-card hidden">
            <h1>Create <span class="title-accent">Secure Vault</span></h1>
            <p class="subtitle">Set up a strong master password to initialize your personal encrypted database.</p>
            
            <form id="setup-form" onsubmit="handleSetup(event)">
                <div class="form-group">
                    <label for="setup-pass">Master Password</label>
                    <input type="password" id="setup-pass" required placeholder="Choose a secure password..." oninput="checkMasterPasswordStrength(this.value)">
                    <div id="strength-meter-container" style="margin-top: 6px; display: none;">
                        <div style="height: 4px; width: 100%; background: rgba(255, 255, 255, 0.1); border-radius: 2px; overflow: hidden;">
                            <div id="strength-bar" style="height: 100%; width: 0%; background: var(--accent-red); transition: width 0.3s, background-color 0.3s;"></div>
                        </div>
                        <div id="strength-text" style="font-size: 0.75em; text-align: left; margin-top: 4px; color: var(--text-secondary);">Weak</div>
                    </div>
                </div>
                <div class="form-group">
                    <label for="setup-confirm">Confirm Password</label>
                    <input type="password" id="setup-confirm" required placeholder="Re-enter password...">
                </div>
                
                <button type="submit" class="btn btn-primary">Initialize Vault</button>
            </form>


        </div>

        <!-- 🔒 Login Card -->
        <div id="login-view" class="glass-card hidden">
            <h1>Unlock <span class="title-accent">Vault</span></h1>
            <p class="subtitle">Enter your master password to decrypt and open the database.</p>

            <div id="lockout-timer-box" class="lockout-box hidden">
                <label>Security Lockout Active</label>
                <div id="lockout-timer" class="lockout-timer">00</div>
            </div>

            <form id="login-form" onsubmit="handleLogin(event)">
                <div class="form-group">
                    <label for="login-pass">Master Password</label>
                    <input type="password" id="login-pass" required placeholder="Type password..." autofocus>
                </div>
                
                <button type="submit" id="unlock-btn" class="btn btn-primary">Unlock Database</button>
            </form>


        </div>

        <!-- 💻 Dashboard Workspace -->
        <div id="dashboard-view" class="dashboard-container hidden">
            
            <header class="dashboard-header">
                <h2>SECURE <span class="title-accent">PASSWORD VAULT</span></h2>
                <div style="display: flex; gap: 10px; align-items: center;">
                    <button id="nav-vault-btn" onclick="switchTab('vault')" class="btn btn-secondary" style="background: rgba(59, 130, 246, 0.2); border-color: var(--accent-blue);">Vault</button>
                    <button id="nav-audit-btn" onclick="switchTab('audit')" class="btn btn-secondary">Security Audit</button>
                    <button onclick="handleLock()" class="btn btn-secondary">Lock Vault</button>
                    <button onclick="handleShutdown()" class="btn btn-danger" style="font-size: 0.9em; padding: 8px 16px;">Exit App</button>
                </div>
            </header>

            <!-- 📋 Clipboard Progress Bar -->
            <div id="clipboard-progress-container" class="clipboard-bar-container">
                <div id="clipboard-progress-bar" class="clipboard-bar-fill"></div>
            </div>

            <div id="tab-vault" class="dashboard-layout">
                
                <!-- Left Pane: Credentials table -->
                <main class="left-pane">
                    <div class="controls-row">
                        <div class="search-container">
                            <label>Filter:</label>
                            <input type="text" id="search-filter" oninput="renderTable()" placeholder="Filter by site or username...">
                        </div>
                        <button onclick="saveVaultChanges()" class="btn btn-secondary">Save Changes</button>
                    </div>

                    <div class="table-wrapper">
                        <table id="credentials-table">
                            <thead>
                                <tr>
                                    <th>Website</th>
                                    <th>Username / Email</th>
                                    <th>Password</th>
                                    <th style="text-align: right; width: 190px;">Actions</th>
                                </tr>
                            </thead>
                            <tbody id="table-body">
                                <!-- Dynamic Rows -->
                            </tbody>
                        </table>
                    </div>
                </main>

                <!-- Right Pane: Add form & display configurations -->
                <aside class="right-pane">
                    <h3>ADD CREDENTIAL</h3>
                    <p class="subtitle" style="margin-bottom: 16px;">Save a new password record to the local database.</p>

                    <form id="add-form" onsubmit="handleAddCredential(event)">
                        <div class="form-group">
                            <label for="add-site">Website</label>
                            <input type="text" id="add-site" required placeholder="e.g. google.com">
                        </div>
                        <div class="form-group">
                            <label for="add-user">Username / Email</label>
                            <input type="text" id="add-user" required placeholder="e.g. user@email.com">
                        </div>
                        <div class="form-group">
                            <label for="add-pass">Password</label>
                            <input type="password" id="add-pass" required placeholder="Type password...">
                        </div>
                        <div class="form-group">
                            <label for="add-totp">TOTP Secret (2FA Key) - Optional</label>
                            <input type="text" id="add-totp" placeholder="e.g. JBSWY3DPEHPK3PXP">
                        </div>
                        <div class="form-group">
                            <label for="add-notes">Secure Notes - Optional</label>
                            <textarea id="add-notes" placeholder="e.g. Recovery keys, pin codes, security questions..."></textarea>
                        </div>
                        <div class="form-group">
                            <label>File Attachment - Optional (Max 2MB)</label>
                            <div style="display: flex; gap: 10px; align-items: center;">
                                <button type="button" onclick="attachFile()" class="btn btn-secondary" style="font-size: 0.85em; padding: 8px 12px; margin: 0; width: auto;">Select File...</button>
                                <span id="attached-file-info" style="font-size: 0.85em; color: var(--text-secondary);">No file attached</span>
                                <button type="button" id="btn-clear-attachment" onclick="clearAttachment()" class="table-btn table-btn-danger hidden" style="min-width: auto; height: 32px;">Clear</button>
                            </div>
                        </div>

                        <button type="submit" class="btn btn-primary" style="margin-top: 10px;">Add Record</button>
                    </form>

                    <!-- Secure Password Generator Section -->
                    <div class="settings-section" style="margin-top: 20px;">
                        <div class="settings-title">Secure Password Generator</div>
                        <div class="form-group" style="margin-bottom: 10px;">
                            <label for="gen-length">Length: <span id="gen-length-val">16</span></label>
                            <input type="range" id="gen-length" min="8" max="64" value="16" class="scale-slider" oninput="document.getElementById('gen-length-val').textContent=this.value; handleGeneratePassword();">
                        </div>
                        <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 8px; margin-bottom: 12px; font-size: 0.85em; text-align: left;">
                            <label style="display: flex; align-items: center; gap: 6px; cursor: pointer; color: var(--text-secondary);">
                                <input type="checkbox" id="gen-upper" checked style="width: auto; margin: 0;" onchange="handleGeneratePassword()"> A-Z
                            </label>
                            <label style="display: flex; align-items: center; gap: 6px; cursor: pointer; color: var(--text-secondary);">
                                <input type="checkbox" id="gen-lower" checked style="width: auto; margin: 0;" onchange="handleGeneratePassword()"> a-z
                            </label>
                            <label style="display: flex; align-items: center; gap: 6px; cursor: pointer; color: var(--text-secondary);">
                                <input type="checkbox" id="gen-digit" checked style="width: auto; margin: 0;" onchange="handleGeneratePassword()"> 0-9
                            </label>
                            <label style="display: flex; align-items: center; gap: 6px; cursor: pointer; color: var(--text-secondary);">
                                <input type="checkbox" id="gen-symbol" checked style="width: auto; margin: 0;" onchange="handleGeneratePassword()"> Special
                            </label>
                        </div>
                        <div style="display: flex; gap: 8px; align-items: center; margin-bottom: 10px;">
                            <input type="text" id="gen-result" readonly placeholder="Click Gen..." style="font-family: monospace; font-size: 0.9em; padding: 8px 12px; background: rgba(10, 10, 15, 0.7); cursor: default; border: 1px solid var(--card-border); border-radius: 8px; color: var(--text-primary); flex-grow: 1;">
                            <button onclick="copyGeneratedPassword()" class="table-btn" style="min-width: 50px; padding: 8px 12px;">Copy</button>
                        </div>
                    </div>

                    <div class="settings-section">
                        <div class="settings-title">Backup & Portability</div>
                        <div style="display: flex; gap: 10px; margin-top: 8px;">
                            <button onclick="importCSV()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">Import CSV</button>
                            <button onclick="openExportModal()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">Export CSV</button>
                        </div>
                    </div>


                </aside>

            </div>

            <div id="tab-audit" class="dashboard-layout hidden">
                <main class="left-pane" style="grid-column: 1 / -1;">
                    <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px;">
                        <h3>Security Audit Dashboard</h3>
                        <p class="subtitle" style="margin: 0;">Audit and fix weak or reused credentials completely offline.</p>
                    </div>
                    
                    <!-- Stats Grid -->
                    <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(220px, 1fr)); gap: 16px; margin-bottom: 24px;">
                        <div class="glass-card" style="padding: 20px; text-align: left; max-width: none;">
                            <div style="font-size: 0.85em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Total Passwords</div>
                            <div id="audit-total-count" style="font-size: 2.2em; font-weight: 700; margin-top: 8px; color: var(--text-primary);">0</div>
                        </div>
                        <div class="glass-card" style="padding: 20px; text-align: left; max-width: none; border-color: rgba(239, 68, 68, 0.25);">
                            <div style="font-size: 0.85em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Weak Passwords</div>
                            <div id="audit-weak-count" style="font-size: 2.2em; font-weight: 700; margin-top: 8px; color: #fca5a5;">0</div>
                            <div style="font-size: 0.75em; color: var(--text-secondary); margin-top: 4px;">Length &lt; 12 characters</div>
                        </div>
                        <div class="glass-card" style="padding: 20px; text-align: left; max-width: none; border-color: rgba(245, 158, 11, 0.25);">
                            <div style="font-size: 0.85em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Reused Passwords</div>
                            <div id="audit-reused-count" style="font-size: 2.2em; font-weight: 700; margin-top: 8px; color: #fde047;">0</div>
                            <div style="font-size: 0.75em; color: var(--text-secondary); margin-top: 4px;">Shared across multiple sites</div>
                        </div>
                    </div>

                    <!-- Weak/Reused lists -->
                    <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 20px;">
                        <div>
                            <h4 style="margin-bottom: 12px; color: #fca5a5;">Weak Credentials</h4>
                            <div class="table-wrapper" style="max-height: 300px;">
                                <table>
                                    <thead>
                                        <tr>
                                            <th>Website</th>
                                            <th>Username</th>
                                            <th>Length</th>
                                        </tr>
                                    </thead>
                                    <tbody id="audit-weak-list">
                                        <!-- Dynamic rows -->
                                    </tbody>
                                </table>
                            </div>
                        </div>
                        <div>
                            <h4 style="margin-bottom: 12px; color: #fde047;">Reused Credentials</h4>
                            <div class="table-wrapper" style="max-height: 300px;">
                                <table>
                                    <thead>
                                        <tr>
                                            <th>Password (Truncated)</th>
                                            <th>Sites / Users Reusing It</th>
                                        </tr>
                                    </thead>
                                    <tbody id="audit-reused-list">
                                        <!-- Dynamic rows -->
                                    </tbody>
                                </table>
                            </div>
                        </div>
                    </div>
                </main>
            </div>

        </div>

    </div>

    <script>
        // Global variables
        let credentials = [];
        let visiblePasswords = new Set();
        let scale = 1.0;
        let bannerTimer = null;
        let lockoutInterval = null;
        let clipboardTimer = null;
        let inactivityTimer = null;
        const INACTIVITY_LIMIT = 5 * 60 * 1000; // 5 minutes auto-lock timeout

        // Premium variables
        let clipboardPass = "";
        let clipboardProgressInterval = null;
        let currentAttachedFile = { name: "", data: "" };
        let totpTimer = null;
        let systemIdleInterval = null;

        // 🔹 Password strength checker (12-char strict complexity rules)
        function checkMasterPasswordStrength(pass) {
            const container = document.getElementById("strength-meter-container");
            const bar = document.getElementById("strength-bar");
            const text = document.getElementById("strength-text");
            
            if (!pass) {
                container.style.display = "none";
                return;
            }
            container.style.display = "block";
            
            let score = 0;
            if (pass.length >= 8) score++;
            if (pass.length >= 12) score++;
            if (/[A-Z]/.test(pass)) score++;
            if (/[0-9]/.test(pass)) score++;
            if (/[^A-Za-z0-9]/.test(pass)) score++;
            
            let percentage = (score / 5) * 100;
            bar.style.width = percentage + "%";
            
            if (score <= 2) {
                bar.style.backgroundColor = "var(--accent-red)";
                text.textContent = "Weak (Minimum 12 chars + mixed case/numbers/symbols required)";
                text.style.color = "#fca5a5";
            } else if (score <= 4) {
                bar.style.backgroundColor = "#fbbf24";
                text.textContent = "Medium (Try adding more symbols or length)";
                text.style.color = "#fde047";
            } else {
                bar.style.backgroundColor = "var(--success-green)";
                text.textContent = "Strong Master Password";
                text.style.color = "#86efac";
            }
        }

        // 🔹 Secure password generator handlers
        function handleGeneratePassword() {
            const length = parseInt(document.getElementById("gen-length").value);
            const upper = document.getElementById("gen-upper").checked;
            const lower = document.getElementById("gen-lower").checked;
            const digit = document.getElementById("gen-digit").checked;
            const symbol = document.getElementById("gen-symbol").checked;

            window.api_generate_password(length.toString(), upper.toString(), lower.toString(), digit.toString(), symbol.toString())
                .then(data => {
                    if (data && data.password) {
                        document.getElementById("gen-result").value = data.password;
                    }
                })
                .catch(() => showAlert("Failed to generate password."));
        }

        function copyGeneratedPassword() {
            const text = document.getElementById("gen-result").value;
            if (!text) return;
            copyPassword(text);
        }

        // 🔹 Inactivity tracking
        function resetInactivityTimer() {
            if (inactivityTimer) clearTimeout(inactivityTimer);
            const dashboard = document.getElementById("dashboard-view");
            if (dashboard && !dashboard.classList.contains("hidden")) {
                inactivityTimer = setTimeout(() => {
                    handleLock();
                    showAlert("Vault locked automatically due to inactivity.");
                }, INACTIVITY_LIMIT);
            }
        }

        function stopInactivityTimer() {
            if (inactivityTimer) clearTimeout(inactivityTimer);
        }

        // Event listeners to detect user activity
        ["mousemove", "mousedown", "keydown", "click", "scroll"].forEach(event => {
            window.addEventListener(event, resetInactivityTimer);
        });

        // 🔹 Clipboard safe copy with visual progress bar and conditional clear
        function copyPassword(text) {
            navigator.clipboard.writeText(text)
                .then(() => {
                    clipboardPass = text;
                    showAlert("Password copied to clipboard! (Auto-clears in 20s)", true);
                    
                    const container = document.getElementById("clipboard-progress-container");
                    const fill = document.getElementById("clipboard-progress-bar");
                    container.style.display = "block";
                    fill.style.width = "100%";

                    if (clipboardTimer) clearTimeout(clipboardTimer);
                    if (clipboardProgressInterval) clearInterval(clipboardProgressInterval);

                    let secondsLeft = 20;
                    clipboardProgressInterval = setInterval(() => {
                        secondsLeft -= 0.1;
                        if (secondsLeft <= 0) {
                            clearInterval(clipboardProgressInterval);
                            container.style.display = "none";
                        } else {
                            fill.style.width = (secondsLeft / 20) * 100 + "%";
                        }
                    }, 100);

                    clipboardTimer = setTimeout(() => {
                        window.api_clear_clipboard_if_matching(clipboardPass)
                            .then(data => {
                                if (data.success) {
                                    showAlert("Clipboard cleared for security.", true);
                                }
                            });
                        clipboardPass = "";
                    }, 20000);
                })
                .catch(() => showAlert("Failed to copy password."));
        }

        function startTotpPoller() {
            if (totpTimer) clearInterval(totpTimer);
            totpTimer = setInterval(() => {
                const dashboard = document.getElementById("dashboard-view");
                if (dashboard && !dashboard.classList.contains("hidden")) {
                    window.api_get_credentials()
                        .then(data => {
                            credentials = data.credentials || [];
                            const detailsModal = document.getElementById("details-modal");
                            if (detailsModal && !detailsModal.classList.contains("hidden")) {
                                const openSite = document.getElementById("details-site").value;
                                const openUser = document.getElementById("details-user").value;
                                const cred = credentials.find(c => c.website === openSite && c.username === openUser);
                                if (cred && cred.totp) {
                                    document.getElementById("details-totp-code").value = cred.totp;
                                    document.getElementById("totp-countdown-val").textContent = cred.totp_remaining;
                                    const percentage = (cred.totp_remaining / 30) * 100;
                                    const progressFill = document.getElementById("totp-progress-bar-fill");
                                    if (progressFill) {
                                        progressFill.style.width = percentage + "%";
                                        if (cred.totp_remaining <= 5) {
                                            progressFill.style.backgroundColor = "var(--accent-red)";
                                        } else {
                                            progressFill.style.backgroundColor = "var(--success-green)";
                                        }
                                    }
                                }
                            }
                        })
                        .catch(() => {});
                }
            }, 1000);
        }

        function startSystemIdlePoller() {
            if (systemIdleInterval) clearInterval(systemIdleInterval);
            systemIdleInterval = setInterval(() => {
                const dashboard = document.getElementById("dashboard-view");
                if (dashboard && !dashboard.classList.contains("hidden")) {
                    window.api_get_system_idle_time()
                        .then(data => {
                            if (data.idleMs && data.idleMs >= 2 * 60 * 1000) { // 2 minutes system idle
                                handleLock();
                                showAlert("Vault locked automatically due to system inactivity.");
                            }
                        })
                        .catch(() => {});
                }
            }, 5000);
        }

        // On Page Load
        window.addEventListener("DOMContentLoaded", () => {
            checkVaultStatus();
            startTotpPoller();
            startSystemIdlePoller();
        });

        // 🔹 Alert Banner Helpers
        function showAlert(msg, isSuccess = false) {
            const banner = document.getElementById("alert-banner");
            banner.textContent = msg;
            banner.className = isSuccess ? "banner banner-success" : "banner banner-error";
            
            if (bannerTimer) clearTimeout(bannerTimer);
            bannerTimer = setTimeout(() => {
                banner.className = "banner";
            }, 5000);
        }

        // 🔹 Check status of database
        function checkVaultStatus() {
            window.api_status()
                .then(data => {
                    // Hide all
                    document.getElementById("setup-view").classList.add("hidden");
                    document.getElementById("login-view").classList.add("hidden");
                    document.getElementById("dashboard-view").classList.add("hidden");

                    if (!data.exists) {
                        document.getElementById("setup-view").classList.remove("hidden");
                    } else {
                        document.getElementById("login-view").classList.remove("hidden");
                        if (data.lockoutRemaining > 0) {
                            startLockoutCountdown(data.lockoutRemaining);
                        } else {
                            stopLockoutCountdown();
                        }
                    }
                })
                .catch(err => {
                    console.error("API call failed", err);
                    showAlert("Could not communicate with application backend.");
                });
        }

        // 🔹 Handle Lockout Countdown
        function startLockoutCountdown(seconds) {
            const timerBox = document.getElementById("lockout-timer-box");
            const timerVal = document.getElementById("lockout-timer");
            const formInputs = document.querySelectorAll("#login-form input, #login-form button");

            timerBox.classList.remove("hidden");
            formInputs.forEach(el => el.disabled = true);

            let remaining = seconds;
            timerVal.textContent = remaining;

            if (lockoutInterval) clearInterval(lockoutInterval);
            lockoutInterval = setInterval(() => {
                remaining--;
                if (remaining <= 0) {
                    clearInterval(lockoutInterval);
                    stopLockoutCountdown();
                    checkVaultStatus();
                } else {
                    timerVal.textContent = remaining;
                }
            }, 1000);
        }

        // 🔹 Stop Lockout Countdown
        function stopLockoutCountdown() {
            if (lockoutInterval) clearInterval(lockoutInterval);
            document.getElementById("lockout-timer-box").classList.add("hidden");
            document.querySelectorAll("#login-form input, #login-form button").forEach(el => el.disabled = false);
        }

        // 🔹 Handle Setup Submit
        function handleSetup(e) {
            e.preventDefault();
            const pass = document.getElementById("setup-pass").value;
            const confirm = document.getElementById("setup-confirm").value;

            if (pass !== confirm) {
                showAlert("Passwords do not match.");
                return;
            }

            if (pass.length < 12) {
                showAlert("Master password must be at least 12 characters long.");
                return;
            }

            if (!/[A-Z]/.test(pass) || !/[a-z]/.test(pass) || !/[0-9]/.test(pass) || !/[^A-Za-z0-9]/.test(pass)) {
                showAlert("Master password must contain uppercase, lowercase, numbers, and symbols.");
                return;
            }

            window.api_setup(pass)
            .then(data => {
                // Secure wipe inputs
                document.getElementById("setup-pass").value = "";
                document.getElementById("setup-confirm").value = "";
                document.getElementById("strength-meter-container").style.display = "none";

                if (data.success) {
                    showAlert("Vault initialized successfully! Please login.", true);
                    checkVaultStatus();
                } else {
                    showAlert(data.error || "Failed to initialize vault.");
                }
            })
            .catch(() => showAlert("API request failed."));
        }

        // 🔹 Handle Login Submit
        function handleLogin(e) {
            e.preventDefault();
            const pass = document.getElementById("login-pass").value;

            window.api_login(pass)
            .then(data => {
                // Secure wipe input
                document.getElementById("login-pass").value = "";

                if (data.success) {
                    showAlert("Vault unlocked successfully.", true);
                    loadDashboard();
                } else {
                    if (data.destructed) {
                        showAlert(data.error || "Vault has been permanently self-destructed due to 10 consecutive failed attempts!");
                    } else {
                        showAlert(data.error || "Invalid password.");
                    }
                    checkVaultStatus(); // Updates lockout status or shows setup view if self-destructed
                }
            })
            .catch(() => showAlert("API request failed."));
        }

        // 🔹 Load Dashboard
        function loadDashboard() {
            document.getElementById("login-view").classList.add("hidden");
            document.getElementById("dashboard-view").classList.remove("hidden");
            
            fetchCredentials();
            resetInactivityTimer();
            handleGeneratePassword(); // Generate a random password on load
        }

        // 🔹 Fetch Credentials
        function fetchCredentials() {
            window.api_get_credentials()
                .then(data => {
                    credentials = data.credentials || [];
                    renderTable();
                })
                .catch(() => showAlert("Failed to retrieve credentials."));
        }

        // 🔹 Render Table
        function renderTable() {
            const tbody = document.getElementById("table-body");
            tbody.innerHTML = "";

            const filterText = document.getElementById("search-filter").value.toLowerCase();

            credentials.forEach((c, idx) => {
                if (filterText) {
                    const siteMatch = c.website.toLowerCase().includes(filterText);
                    const userMatch = c.username.toLowerCase().includes(filterText);
                    if (!siteMatch && !userMatch) return; // Skip non-matching rows
                }

                const row = document.createElement("tr");
                row.onclick = (e) => {
                    if (tdActions.contains(e.target)) return;
                    openDetailsModal(c);
                };

                // Cell: Website
                const tdSite = document.createElement("td");
                tdSite.textContent = c.website;
                row.appendChild(tdSite);

                // Cell: Username
                const tdUser = document.createElement("td");
                tdUser.textContent = c.username;
                row.appendChild(tdUser);

                // Cell: Password (with Show/Hide toggling)
                const tdPass = document.createElement("td");
                const key = c.website + "||" + c.username;
                const isVisible = visiblePasswords.has(key);
                
                if (isVisible) {
                    tdPass.textContent = c.password;
                    tdPass.style.fontFamily = "inherit";
                    tdPass.style.letterSpacing = "normal";
                } else {
                    tdPass.textContent = "••••••••";
                    tdPass.className = "cell-mono";
                }
                row.appendChild(tdPass);

                // Cell: Actions
                const tdActions = document.createElement("td");
                tdActions.className = "cell-actions";

                // Copy Button
                const btnCopy = document.createElement("button");
                btnCopy.className = "table-btn";
                btnCopy.textContent = "Copy";
                btnCopy.onclick = (e) => {
                    e.stopPropagation();
                    copyPassword(c.password);
                };
                tdActions.appendChild(btnCopy);

                // History Button
                if (c.history && c.history.length > 0) {
                    const btnHistory = document.createElement("button");
                    btnHistory.className = "table-btn";
                    btnHistory.innerHTML = "🕒";
                    btnHistory.title = "View Password History";
                    btnHistory.onclick = (e) => {
                        e.stopPropagation();
                        openHistoryModal(c.website, c.username, c.history);
                    };
                    tdActions.appendChild(btnHistory);
                }

                // Show/Hide Toggle
                const btnToggle = document.createElement("button");
                btnToggle.className = "table-btn";
                btnToggle.textContent = isVisible ? "Hide" : "Show";
                btnToggle.onclick = (e) => {
                    e.stopPropagation();
                    if (isVisible) {
                        visiblePasswords.delete(key);
                        renderTable();
                    } else {
                        openVerifyModal("show", key);
                    }
                };
                tdActions.appendChild(btnToggle);

                // Delete Button
                const btnDelete = document.createElement("button");
                btnDelete.className = "table-btn table-btn-danger";
                btnDelete.textContent = "Delete";
                btnDelete.onclick = (e) => {
                    e.stopPropagation();
                    openVerifyModal("delete", { site: c.website, user: c.username });
                };
                tdActions.appendChild(btnDelete);

                row.appendChild(tdActions);
                tbody.appendChild(row);
            });
        }

        // 🔹 File Attachment Management for Add Form
        function attachFile() {
            window.api_attach_file()
                .then(data => {
                    if (data.success) {
                        currentAttachedFile.name = data.fileName;
                        currentAttachedFile.data = data.fileData;
                        
                        const info = document.getElementById("attached-file-info");
                        info.textContent = data.fileName;
                        info.style.color = "var(--text-primary)";
                        document.getElementById("btn-clear-attachment").classList.remove("hidden");
                        showAlert("File attached successfully!", true);
                    } else if (data.error && data.error !== "Cancelled") {
                        showAlert(`Failed to attach file: ${data.error}`);
                    }
                })
                .catch(() => showAlert("Attach file request failed."));
        }

        function clearAttachment() {
            currentAttachedFile.name = "";
            currentAttachedFile.data = "";
            const info = document.getElementById("attached-file-info");
            info.textContent = "No file attached";
            info.style.color = "var(--text-secondary)";
            document.getElementById("btn-clear-attachment").classList.add("hidden");
        }

        // 🔹 Add new credential
        function handleAddCredential(e) {
            e.preventDefault();
            const site = document.getElementById("add-site").value;
            const user = document.getElementById("add-user").value;
            const pass = document.getElementById("add-pass").value;
            const totp = document.getElementById("add-totp").value || "";
            const notes = document.getElementById("add-notes").value || "";
            const attName = currentAttachedFile.name || "";
            const attData = currentAttachedFile.data || "";

            window.api_add_credential(site, user, pass, totp, notes, attName, attData)
            .then(data => {
                // Secure wipe inputs
                document.getElementById("add-site").value = "";
                document.getElementById("add-user").value = "";
                document.getElementById("add-pass").value = "";
                document.getElementById("add-totp").value = "";
                document.getElementById("add-notes").value = "";
                clearAttachment();

                if (data.success) {
                    showAlert("Credential added and vault saved!", true);
                    fetchCredentials();
                } else {
                    showAlert(data.error || "Failed to add credential.");
                }
            })
            .catch(() => showAlert("API request failed."));
        }

        // 🔹 Delete credential
        function handleDeleteCredential(site, user) {
            window.api_delete_credential(site, user)
            .then(data => {
                if (data.success) {
                    showAlert("Credential deleted & vault saved.", true);
                    
                    // Clear visibility tracking for deleted entry
                    const key = site + "||" + user;
                    visiblePasswords.delete(key);

                    fetchCredentials();
                } else {
                    showAlert(data.error || "Failed to delete credential.");
                }
            })
            .catch(() => showAlert("API request failed."));
        }

        // 🔹 Save changes explicitly
        function saveVaultChanges() {
            window.api_save()
                .then(data => {
                    if (data.success) {
                        showAlert("Vault saved successfully.", true);
                    } else {
                        showAlert(data.error || "Failed to save vault.");
                    }
                })
                .catch(() => showAlert("API request failed."));
        }

        // 🔹 Lock Vault
        function handleLock() {
            stopInactivityTimer();
            if (clipboardTimer) {
                clearTimeout(clipboardTimer);
                navigator.clipboard.writeText("");
            }
            window.api_lock()
                .then(data => {
                    if (data.success) {
                        credentials = [];
                        visiblePasswords.clear();
                        showAlert("Vault locked successfully.", true);
                        checkVaultStatus();
                    }
                })
                .catch(() => showAlert("API request failed."));
        }

        // 🔹 Credential Details Inspector Modal Handlers
        function openDetailsModal(c) {
            document.getElementById("details-site").value = c.website;
            document.getElementById("details-user").value = c.username;
            document.getElementById("details-pass").value = c.password;
            
            // Password show/hide toggle state reset
            const passInput = document.getElementById("details-pass");
            passInput.type = "password";
            document.getElementById("btn-details-show-pass").textContent = "Show";

            // TOTP Section
            const totpContainer = document.getElementById("details-totp-container");
            if (c.totp_secret && c.totp_secret.trim() !== "") {
                totpContainer.classList.remove("hidden");
                document.getElementById("details-totp-code").value = c.totp || "";
                document.getElementById("totp-countdown-val").textContent = c.totp_remaining || "30";
                
                const percentage = ((c.totp_remaining || 30) / 30) * 100;
                const progressFill = document.getElementById("totp-progress-bar-fill");
                if (progressFill) {
                    progressFill.style.width = percentage + "%";
                    if (c.totp_remaining <= 5) {
                        progressFill.style.backgroundColor = "var(--accent-red)";
                    } else {
                        progressFill.style.backgroundColor = "var(--success-green)";
                    }
                }
            } else {
                totpContainer.classList.add("hidden");
            }

            // Notes Section
            document.getElementById("details-notes").value = c.notes || "";

            // File Attachment Section
            const attachmentContainer = document.getElementById("details-attachment-container");
            if (c.attachmentName && c.attachmentName.trim() !== "") {
                attachmentContainer.classList.remove("hidden");
                document.getElementById("details-attachment-name").textContent = c.attachmentName;
                document.getElementById("details-attachment-name").title = c.attachmentName;
            } else {
                attachmentContainer.classList.add("hidden");
            }

            // Password History Section (within Details modal)
            const historyContainer = document.getElementById("details-history-container");
            const historyBody = document.getElementById("details-history-body");
            historyBody.innerHTML = "";
            if (c.history && c.history.length > 0) {
                historyContainer.classList.remove("hidden");
                c.history.forEach((oldPass, idx) => {
                    const hRow = document.createElement("tr");

                    const tdHPass = document.createElement("td");
                    tdHPass.textContent = "••••••••";
                    tdHPass.className = "cell-mono";
                    
                    const tdHAction = document.createElement("td");
                    tdHAction.style.textAlign = "right";

                    const btnHCopy = document.createElement("button");
                    btnHCopy.className = "table-btn";
                    btnHCopy.textContent = "Copy";
                    btnHCopy.onclick = (e) => {
                        e.stopPropagation();
                        copyPassword(oldPass);
                    };
                    tdHAction.appendChild(btnHCopy);

                    const btnHShow = document.createElement("button");
                    btnHShow.className = "table-btn";
                    btnHShow.textContent = "Show";
                    btnHShow.style.marginLeft = "6px";
                    btnHShow.onclick = (e) => {
                        e.stopPropagation();
                        if (btnHShow.textContent === "Show") {
                            tdHPass.textContent = oldPass;
                            tdHPass.style.fontFamily = "inherit";
                            tdHPass.style.letterSpacing = "normal";
                            btnHShow.textContent = "Hide";
                        } else {
                            tdHPass.textContent = "••••••••";
                            tdHPass.className = "cell-mono";
                            tdHPass.style.fontFamily = "'Courier New', Courier, monospace";
                            tdHPass.style.letterSpacing = "0.08em";
                            btnHShow.textContent = "Show";
                        }
                    };
                    tdHAction.appendChild(btnHShow);

                    hRow.appendChild(tdHPass);
                    hRow.appendChild(tdHAction);
                    historyBody.appendChild(hRow);
                });
            } else {
                historyContainer.classList.add("hidden");
            }

            document.getElementById("details-modal").classList.remove("hidden");
        }

        function closeDetailsModal() {
            document.getElementById("details-modal").classList.add("hidden");
        }

        function toggleDetailsPassword() {
            const passInput = document.getElementById("details-pass");
            const btn = document.getElementById("btn-details-show-pass");
            if (passInput.type === "password") {
                passInput.type = "text";
                btn.textContent = "Hide";
            } else {
                passInput.type = "password";
                btn.textContent = "Show";
            }
        }

        function copyDetailsPassword() {
            const val = document.getElementById("details-pass").value;
            if (val) copyPassword(val);
        }

        function copyDetailsTotp() {
            const val = document.getElementById("details-totp-code").value;
            if (val) {
                navigator.clipboard.writeText(val)
                    .then(() => showAlert("TOTP code copied to clipboard!", true))
                    .catch(() => showAlert("Failed to copy TOTP code."));
            }
        }

        function downloadAttachment() {
            const site = document.getElementById("details-site").value;
            const user = document.getElementById("details-user").value;
            const cred = credentials.find(c => c.website === site && c.username === user);
            if (cred && cred.attachmentName && cred.attachmentData) {
                window.api_download_file(cred.attachmentName, cred.attachmentData)
                    .then(data => {
                        if (data.success) {
                            showAlert(`File downloaded successfully to ${data.path}`, true);
                        } else if (data.error && data.error !== "Cancelled") {
                            showAlert(`Download failed: ${data.error}`);
                        }
                    })
                    .catch(() => showAlert("Download request failed."));
            }
        }

        // 🔹 Verification Modal handlers
        let pendingVerifyAction = null;
        let pendingVerifyData = null;

        function openVerifyModal(action, data) {
            pendingVerifyAction = action;
            pendingVerifyData = data;
            document.getElementById("verify-pass").value = "";
            
            const prompt = document.getElementById("verify-prompt");
            if (action === "delete") {
                prompt.textContent = `Please enter your master password to confirm deleting credentials for "${data.site}".`;
            } else if (action === "export") {
                prompt.textContent = "Please enter your master password to authorize exporting the database to CSV.";
            } else {
                prompt.textContent = "Please enter your master password to reveal this credential.";
            }

            document.getElementById("verify-modal").classList.remove("hidden");
            document.getElementById("verify-pass").focus();
        }

        function closeVerifyModal() {
            pendingVerifyAction = null;
            pendingVerifyData = null;
            document.getElementById("verify-pass").value = "";
            document.getElementById("verify-modal").classList.add("hidden");
        }

        function submitVerifyModal() {
            const pass = document.getElementById("verify-pass").value;
            if (!pass) return;

            window.api_verify_password(pass)
                .then(data => {
                    document.getElementById("verify-pass").value = "";
                    if (data.success) {
                        if (pendingVerifyAction === "show") {
                            if (pendingVerifyData) {
                                visiblePasswords.add(pendingVerifyData);
                                renderTable();
                            }
                        } else if (pendingVerifyAction === "delete") {
                            if (pendingVerifyData) {
                                handleDeleteCredential(pendingVerifyData.site, pendingVerifyData.user);
                            }
                        } else if (pendingVerifyAction === "export") {
                            closeVerifyModal();
                            exportCSV(pass);
                            return;
                        }
                        closeVerifyModal();
                    } else {
                        window.api_status().then(status => {
                            if (status.lockoutRemaining > 0) {
                                handleLock();
                            } else {
                                showAlert("Incorrect master password.");
                            }
                        }).catch(() => showAlert("Incorrect master password."));
                    }
                })
                .catch(() => showAlert("Verification failed."));
        }

        // Handle Escape key to close modals
        window.addEventListener("keydown", (e) => {
            if (e.key === "Escape") {
                const modal = document.getElementById("verify-modal");
                if (modal && !modal.classList.contains("hidden")) {
                    closeVerifyModal();
                }
                const histModal = document.getElementById("history-modal");
                if (histModal && !histModal.classList.contains("hidden")) {
                    closeHistoryModal();
                }
                const detModal = document.getElementById("details-modal");
                if (detModal && !detModal.classList.contains("hidden")) {
                    closeDetailsModal();
                }
            }
        });

        // 🔹 Exit/Shutdown App
        function handleShutdown() {
            if (confirm("Are you sure you want to lock the vault and close the application?")) {
                stopInactivityTimer();
                if (clipboardTimer) {
                    clearTimeout(clipboardTimer);
                    navigator.clipboard.writeText("");
                }
                window.api_shutdown()
                    .then(() => {
                        if (lockoutInterval) clearInterval(lockoutInterval);
                    })
                    .catch(() => showAlert("Failed to close application."));
            }
        }

        // 🔹 Switch between tabs (Vault and Security Audit)
        function switchTab(tab) {
            const vaultTab = document.getElementById("tab-vault");
            const auditTab = document.getElementById("tab-audit");
            const vaultBtn = document.getElementById("nav-vault-btn");
            const auditBtn = document.getElementById("nav-audit-btn");

            if (tab === "vault") {
                vaultTab.classList.remove("hidden");
                auditTab.classList.add("hidden");
                
                // Highlight active button
                vaultBtn.style.background = "rgba(59, 130, 246, 0.2)";
                vaultBtn.style.borderColor = "var(--accent-blue)";
                auditBtn.style.background = "rgba(255, 255, 255, 0.06)";
                auditBtn.style.borderColor = "var(--card-border)";
            } else if (tab === "audit") {
                vaultTab.classList.add("hidden");
                auditTab.classList.remove("hidden");

                auditBtn.style.background = "rgba(59, 130, 246, 0.2)";
                auditBtn.style.borderColor = "var(--accent-blue)";
                vaultBtn.style.background = "rgba(255, 255, 255, 0.06)";
                vaultBtn.style.borderColor = "var(--card-border)";

                runSecurityAudit();
            }
        }

        // 🔹 Run client-side completely offline security audit
        function runSecurityAudit() {
            const weakList = document.getElementById("audit-weak-list");
            const reusedList = document.getElementById("audit-reused-list");
            weakList.innerHTML = "";
            reusedList.innerHTML = "";

            let weakCount = 0;
            let reusedMap = new Map();

            credentials.forEach(c => {
                // Weak if length < 12
                if (c.password.length < 12) {
                    weakCount++;
                    const row = document.createElement("tr");
                    const tdSite = document.createElement("td");
                    tdSite.textContent = c.website;
                    const tdUser = document.createElement("td");
                    tdUser.textContent = c.username;
                    const tdLen = document.createElement("td");
                    tdLen.textContent = `${c.password.length} chars`;
                    tdLen.style.color = "var(--accent-red)";
                    row.appendChild(tdSite);
                    row.appendChild(tdUser);
                    row.appendChild(tdLen);
                    weakList.appendChild(row);
                }

                if (!reusedMap.has(c.password)) {
                    reusedMap.set(c.password, []);
                }
                reusedMap.get(c.password).push(c);
            });

            let reusedCount = 0;
            reusedMap.forEach((users, pass) => {
                if (users.length > 1) {
                    reusedCount += users.length;
                    const row = document.createElement("tr");
                    const tdPass = document.createElement("td");
                    tdPass.textContent = pass.substring(0, 3) + "••••";
                    tdPass.className = "cell-mono";

                    const tdDetails = document.createElement("td");
                    const detailsStr = users.map(u => `"${u.website}" (${u.username})`).join(", ");
                    tdDetails.textContent = detailsStr;
                    tdDetails.style.fontSize = "0.9em";
                    tdDetails.style.color = "var(--text-secondary)";

                    row.appendChild(tdPass);
                    row.appendChild(tdDetails);
                    reusedList.appendChild(row);
                }
            });

            document.getElementById("audit-total-count").textContent = credentials.length;
            document.getElementById("audit-weak-count").textContent = weakCount;
            document.getElementById("audit-reused-count").textContent = reusedCount;
        }

        // 🔹 CSV Backup functions
        function openExportModal() {
            openVerifyModal("export", null);
        }

        function exportCSV(pass) {
            window.api_export_csv(pass)
                .then(data => {
                    if (data.success) {
                        showAlert(`Vault successfully exported to ${data.path}`, true);
                    } else {
                        showAlert(data.error || "Export failed.");
                    }
                })
                .catch(() => showAlert("Export failed."));
        }

        function importCSV() {
            window.api_import_csv()
                .then(data => {
                    if (data.success) {
                        showAlert(`Successfully imported ${data.imported} credentials!`, true);
                        fetchCredentials();
                    } else {
                        showAlert(data.error || "Import failed.");
                    }
                })
                .catch(() => showAlert("Import failed."));
        }

        // 🔹 Password History modal functions
        function openHistoryModal(site, user, historyList) {
            document.getElementById("history-prompt").textContent = `Historical passwords for "${site}" (${user})`;
            const tbody = document.getElementById("history-table-body");
            tbody.innerHTML = "";

            historyList.forEach((oldPass, idx) => {
                const row = document.createElement("tr");

                const tdPass = document.createElement("td");
                tdPass.textContent = "••••••••";
                tdPass.className = "cell-mono";
                
                const tdAction = document.createElement("td");
                tdAction.style.textAlign = "right";

                const btnCopy = document.createElement("button");
                btnCopy.className = "table-btn";
                btnCopy.textContent = "Copy";
                btnCopy.onclick = () => {
                    copyPassword(oldPass);
                };
                tdAction.appendChild(btnCopy);

                const btnShow = document.createElement("button");
                btnShow.className = "table-btn";
                btnShow.textContent = "Show";
                btnShow.style.marginLeft = "6px";
                btnShow.onclick = () => {
                    if (btnShow.textContent === "Show") {
                        tdPass.textContent = oldPass;
                        tdPass.style.fontFamily = "inherit";
                        tdPass.style.letterSpacing = "normal";
                        btnShow.textContent = "Hide";
                    } else {
                        tdPass.textContent = "••••••••";
                        tdPass.className = "cell-mono";
                        tdPass.style.fontFamily = "'Courier New', Courier, monospace";
                        tdPass.style.letterSpacing = "0.08em";
                        btnShow.textContent = "Show";
                    }
                };
                tdAction.appendChild(btnShow);

                row.appendChild(tdPass);
                row.appendChild(tdAction);
                tbody.appendChild(row);
            });

            document.getElementById("history-modal").classList.remove("hidden");
        }

        function closeHistoryModal() {
            document.getElementById("history-modal").classList.add("hidden");
        }
    </script>

        <!-- 🕒 Password History Modal -->
        <div id="history-modal" class="modal-overlay hidden">
            <div class="glass-card modal-content" style="max-width: 480px !important;">
                <h3>Password History</h3>
                <p id="history-prompt" class="subtitle" style="margin-bottom: 16px;">Historical passwords for website...</p>
                <div class="table-wrapper" style="max-height: 250px; margin-bottom: 16px;">
                    <table style="width: 100%;">
                        <thead>
                            <tr>
                                <th>Old Password</th>
                                <th style="text-align: right; width: 140px;">Action</th>
                            </tr>
                        </thead>
                        <tbody id="history-table-body">
                            <!-- Dynamic rows -->
                        </tbody>
                    </table>
                </div>
                <button onclick="closeHistoryModal()" class="btn btn-secondary" style="width: 100%;">Close</button>
            </div>
        </div>

        <!-- 🔐 Master Password Verification Modal -->
        <div id="verify-modal" class="modal-overlay hidden">
            <div class="glass-card modal-content">
                <h3>Confirm Identity</h3>
                <p id="verify-prompt" class="subtitle" style="margin-bottom: 16px;">Please enter your master password to reveal this credential.</p>
                <div class="form-group">
                    <input type="password" id="verify-pass" placeholder="Enter master password..." onkeydown="if(event.key === 'Enter') submitVerifyModal()">
                </div>
                <div style="display: flex; gap: 12px; margin-top: 10px;">
                    <button id="verify-cancel-btn" onclick="closeVerifyModal()" class="btn btn-secondary" style="flex: 1;">Cancel</button>
                    <button id="verify-confirm-btn" onclick="submitVerifyModal()" class="btn btn-primary" style="flex: 1;">Confirm</button>
                </div>
            </div>
        </div>

        <!-- 🔎 Credential Details Inspector Modal -->
        <div id="details-modal" class="modal-overlay hidden">
            <div class="glass-card modal-content" style="max-width: 550px !important; text-align: left;">
                <h3 id="details-title" style="margin-bottom: 4px;">Account Details</h3>
                <p id="details-subtitle" class="subtitle" style="margin-bottom: 20px;">Detailed inspector for credential record.</p>
                
                <div style="display: flex; flex-direction: column; gap: 16px;">
                    <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 12px;">
                        <div>
                            <label style="font-weight: 600; font-size: 0.85em;">Website</label>
                            <input type="text" id="details-site" readonly style="background: rgba(255,255,255,0.02); cursor: default; margin-top: 4px;">
                        </div>
                        <div>
                            <label style="font-weight: 600; font-size: 0.85em;">Username / Email</label>
                            <input type="text" id="details-user" readonly style="background: rgba(255,255,255,0.02); cursor: default; margin-top: 4px;">
                        </div>
                    </div>

                    <div>
                        <label style="font-weight: 600; font-size: 0.85em;">Password</label>
                        <div style="display: flex; gap: 8px; align-items: center; margin-top: 4px;">
                            <input type="password" id="details-pass" readonly style="background: rgba(255,255,255,0.02); cursor: default; flex-grow: 1;">
                            <button id="btn-details-show-pass" onclick="toggleDetailsPassword()" class="table-btn" style="height: 44px; min-width: 60px;">Show</button>
                            <button onclick="copyDetailsPassword()" class="table-btn" style="height: 44px; min-width: 60px;">Copy</button>
                        </div>
                    </div>

                    <div id="details-totp-container" class="hidden">
                        <label style="font-weight: 600; font-size: 0.85em; color: var(--success-green);">Two-Factor Authentication (2FA)</label>
                        <div style="display: flex; gap: 8px; align-items: center; margin-top: 4px;">
                            <input type="text" id="details-totp-code" readonly style="font-family: monospace; font-size: 1.4em; letter-spacing: 0.15em; text-align: center; background: rgba(34, 197, 94, 0.05); border-color: rgba(34, 197, 94, 0.2); color: var(--success-green); cursor: default; flex-grow: 1; height: 48px;">
                            <button onclick="copyDetailsTotp()" class="table-btn" style="height: 48px; min-width: 60px;">Copy</button>
                        </div>
                        <div class="totp-progress-container">
                            <span style="font-size: 0.75em; color: var(--text-secondary);">Code refreshes in <span id="totp-countdown-val">30</span>s</span>
                            <div class="totp-progress-bar">
                                <div id="totp-progress-bar-fill" class="totp-progress-fill"></div>
                            </div>
                        </div>
                    </div>

                    <div>
                        <label style="font-weight: 600; font-size: 0.85em;">Secure Notes</label>
                        <textarea id="details-notes" readonly style="background: rgba(255,255,255,0.02); cursor: default; min-height: 100px; margin-top: 4px; font-size: 0.95em;"></textarea>
                    </div>

                    <div id="details-attachment-container" class="hidden">
                        <label style="font-weight: 600; font-size: 0.85em;">Attached File</label>
                        <div style="display: flex; justify-content: space-between; align-items: center; margin-top: 4px; background: rgba(59, 130, 246, 0.05); border: 1px solid rgba(59, 130, 246, 0.15); border-radius: 8px; padding: 10px 14px;">
                            <span id="details-attachment-name" style="font-size: 0.95em; color: #93c5fd; font-weight: 500; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; max-width: 320px;">filename.pdf</span>
                            <button onclick="downloadAttachment()" class="btn btn-secondary" style="font-size: 0.85em; padding: 6px 12px; margin: 0; width: auto;">Download</button>
                        </div>
                    </div>

                    <div id="details-history-container" class="hidden">
                        <label style="font-weight: 600; font-size: 0.85em; color: #c084fc;">Password History</label>
                        <div class="table-wrapper" style="max-height: 120px; margin-top: 4px;">
                            <table style="width: 100%;">
                                <tbody id="details-history-body">
                                </tbody>
                            </table>
                        </div>
                    </div>
                </div>

                <div style="display: flex; gap: 12px; margin-top: 24px;">
                    <button onclick="closeDetailsModal()" class="btn btn-secondary" style="flex: 1; margin: 0;">Close</button>
                </div>
            </div>
        </div>

    </body>
</html>
)rawhtml";

#endif
