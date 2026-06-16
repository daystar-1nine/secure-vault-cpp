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
        :root, [data-theme="zinc"] {
            --app-scale: 1.0;
            --bg-dark: #09090b;
            --card-bg: rgba(24, 24, 27, 0.65);
            --card-border: rgba(255, 255, 255, 0.08);
            --text-primary: #fafafa;
            --text-secondary: #a1a1aa;
            --accent-primary: #3b82f6;
            --accent-secondary: #6d28d9;
            --accent-glow: rgba(59, 130, 246, 0.15);
            --gradient-primary: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            --success-green: #22c55e;
            --accent-red: #ef4444;
        }

        [data-theme="midnight"] {
            --app-scale: 1.0;
            --bg-dark: #020617;
            --card-bg: rgba(15, 23, 42, 0.7);
            --card-border: rgba(255, 255, 255, 0.06);
            --text-primary: #f8fafc;
            --text-secondary: #94a3b8;
            --accent-primary: #6366f1;
            --accent-secondary: #ec4899;
            --accent-glow: rgba(99, 102, 241, 0.2);
            --gradient-primary: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            --success-green: #10b981;
            --accent-red: #f43f5e;
        }

        [data-theme="cyberpunk"] {
            --app-scale: 1.0;
            --bg-dark: #0f051d;
            --card-bg: rgba(26, 9, 50, 0.75);
            --card-border: rgba(0, 255, 255, 0.25);
            --text-primary: #fdf2ff;
            --text-secondary: #b586f8;
            --accent-primary: #ff007f;
            --accent-secondary: #00ffff;
            --accent-glow: rgba(255, 0, 127, 0.25);
            --gradient-primary: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            --success-green: #39ff14; /* Neon green */
            --accent-red: #ff3131; /* Neon red */
        }

        [data-theme="emerald"] {
            --app-scale: 1.0;
            --bg-dark: #022c22;
            --card-bg: rgba(6, 78, 59, 0.65);
            --card-border: rgba(255, 255, 255, 0.08);
            --text-primary: #f0fdf4;
            --text-secondary: #a7f3d0;
            --accent-primary: #10b981;
            --accent-secondary: #06b6d4;
            --accent-glow: rgba(16, 185, 129, 0.25);
            --gradient-primary: linear-gradient(135deg, var(--accent-primary), var(--accent-secondary));
            --success-green: #34d399;
            --accent-red: #f87171;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            transition: background-color 0.2s, border-color 0.2s, transform 0.1s;
        }

        body {
            background: radial-gradient(circle at 15% 20%, var(--accent-glow), transparent 45%),
                        radial-gradient(circle at 85% 80%, var(--accent-glow), transparent 45%),
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

        /* Scrollbars */
        ::-webkit-scrollbar {
            width: 8px;
            height: 8px;
        }
        ::-webkit-scrollbar-track {
            background: rgba(0,0,0,0.2);
        }
        ::-webkit-scrollbar-thumb {
            background: rgba(255,255,255,0.1);
            border-radius: 4px;
        }
        ::-webkit-scrollbar-thumb:hover {
            background: var(--accent-primary);
        }

        /* Containers */
        .app-container {
            width: 100%;
            max-width: 1250px;
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
            background: var(--gradient-primary);
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

        input, select, textarea {
            background: rgba(15, 15, 20, 0.6);
            border: 1px solid var(--card-border);
            border-radius: 8px;
            padding: 12px 16px;
            color: var(--text-primary);
            font-family: 'Inter', sans-serif;
            font-size: 1em;
            width: 100%;
            outline: none;
        }

        input:focus, select:focus, textarea:focus {
            border-color: var(--accent-primary);
            box-shadow: 0 0 0 4px var(--accent-glow);
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
            background: var(--gradient-primary);
            color: white;
            box-shadow: 0 4px 20px var(--accent-glow);
        }

        .btn-primary:hover {
            transform: translateY(-2px);
            box-shadow: 0 6px 24px var(--accent-glow);
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

        /* Navigation buttons */
        .nav-btn {
            background: rgba(255, 255, 255, 0.06);
            color: var(--text-primary);
            border: 1px solid var(--card-border);
            font-size: 0.9em;
            padding: 8px 16px;
            border-radius: 8px;
            cursor: pointer;
            transition: all 0.2s;
        }

        .nav-btn:hover {
            background: rgba(255, 255, 255, 0.12);
        }

        .nav-btn.active {
            background: var(--accent-glow);
            border-color: var(--accent-primary);
            border: 1px solid var(--accent-primary);
            color: var(--text-primary);
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
            font-size: 0.85em;
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
            background: var(--accent-primary);
            cursor: pointer;
            box-shadow: 0 0 8px var(--accent-glow);
        }

        .scale-label {
            font-size: 0.9em;
            color: var(--text-secondary);
            min-width: 45px;
            text-align: right;
        }

        /* ==================== DASHBOARD VIEW (THREE PANE) ==================== */
        .dashboard-container {
            width: 100%;
            max-width: 1250px;
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
            grid-template-columns: 220px 1.2fr 1.3fr;
            gap: 20px;
            align-items: start;
        }

        @media (max-width: 1024px) {
            .dashboard-layout {
                grid-template-columns: 200px 1fr;
            }
            .right-pane {
                grid-column: 1 / -1;
            }
        }

        @media (max-width: 768px) {
            .dashboard-layout {
                grid-template-columns: 1fr;
            }
            .left-pane, .right-pane, .sidebar-pane {
                grid-column: 1 / -1;
            }
        }

        /* Pane structures */
        .sidebar-pane {
            background: rgba(18, 18, 24, 0.55);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 1px solid var(--card-border);
            border-radius: 12px;
            padding: 20px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);
            display: flex;
            flex-direction: column;
            gap: 16px;
        }

        .sidebar-menu {
            list-style: none;
            display: flex;
            flex-direction: column;
            gap: 6px;
        }

        .sidebar-item {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 10px 14px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 0.9em;
            color: var(--text-secondary);
            font-weight: 500;
            background: transparent;
            transition: all 0.2s;
        }

        .sidebar-item:hover, .sidebar-item.active {
            background: rgba(255, 255, 255, 0.05);
            color: var(--text-primary);
        }

        .sidebar-item.active {
            border-left: 3px solid var(--accent-primary);
            background: rgba(255, 255, 255, 0.07);
        }

        .sidebar-badge {
            background: rgba(255, 255, 255, 0.08);
            padding: 2px 6px;
            border-radius: 10px;
            font-size: 0.75em;
            color: var(--text-secondary);
        }

        .sidebar-section-title {
            font-size: 0.75em;
            text-transform: uppercase;
            letter-spacing: 0.08em;
            color: var(--text-secondary);
            margin-top: 10px;
            margin-bottom: 4px;
            font-weight: 600;
            text-align: left;
        }

        .left-pane, .right-pane {
            background: rgba(18, 18, 24, 0.55);
            backdrop-filter: blur(16px);
            -webkit-backdrop-filter: blur(16px);
            border: 1px solid var(--card-border);
            border-radius: 12px;
            padding: 24px;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);
            text-align: left;
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
            background: var(--gradient-primary);
            transition: width 0.1s linear;
        }

        /* 🔎 Credential row hover cursor */
        #credentials-table tbody tr {
            cursor: pointer;
        }

        #credentials-table tbody tr td:last-child {
            cursor: default;
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

        /* 📎 Attachment file tag */
        .file-tag {
            background: var(--accent-glow);
            border: 1px solid var(--card-border);
            color: var(--text-primary);
            border-radius: 6px;
            padding: 4px 10px;
            font-size: 0.85em;
            display: inline-flex;
            align-items: center;
            gap: 6px;
            margin-top: 6px;
        }

        /* 🥞 Stackable Toast Notification System */
        .toast-stack-container {
            position: fixed;
            bottom: 24px;
            right: 24px;
            display: flex;
            flex-direction: column;
            gap: 10px;
            z-index: 2000;
            pointer-events: none;
        }

        .toast-item {
            background: var(--card-bg);
            backdrop-filter: blur(12px);
            -webkit-backdrop-filter: blur(12px);
            border: 1px solid var(--card-border);
            border-left: 4px solid var(--accent-primary);
            border-radius: 8px;
            padding: 14px 20px;
            min-width: 280px;
            max-width: 380px;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.3);
            display: flex;
            flex-direction: column;
            gap: 6px;
            pointer-events: auto;
            position: relative;
            overflow: hidden;
            animation: slideInLeft 0.3s cubic-bezier(0.16, 1, 0.3, 1);
        }

        @keyframes slideInLeft {
            from { transform: translateX(100%); opacity: 0; }
            to { transform: translateX(0); opacity: 1; }
        }

        .toast-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .toast-title {
            font-weight: 600;
            font-size: 0.9em;
            font-family: 'Outfit', sans-serif;
            text-transform: uppercase;
            letter-spacing: 0.05em;
        }

        .toast-content {
            font-size: 0.85em;
            color: var(--text-secondary);
            text-align: left;
        }

        .toast-close {
            background: transparent;
            border: none;
            color: var(--text-secondary);
            cursor: pointer;
            font-size: 1.15em;
            outline: none;
        }

        .toast-close:hover {
            color: var(--text-primary);
        }

        .toast-progress {
            position: absolute;
            bottom: 0;
            left: 0;
            height: 3px;
            background: var(--accent-primary);
            width: 100%;
        }

        .toast-success {
            border-left-color: var(--success-green);
        }
        .toast-success .toast-progress {
            background: var(--success-green);
        }

        .toast-error {
            border-left-color: var(--accent-red);
        }
        .toast-error .toast-progress {
            background: var(--accent-red);
        }

        .toast-info {
            border-left-color: var(--accent-primary);
        }
        .toast-info .toast-progress {
            background: var(--accent-primary);
        }

        /* ⌨️ Command Palette Styling */
        .command-palette-overlay {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.6);
            backdrop-filter: blur(8px);
            z-index: 1000;
            display: flex;
            justify-content: center;
            padding-top: 120px;
            animation: fadeIn 0.2s ease-out;
        }

        .command-palette-box {
            background: var(--card-bg);
            backdrop-filter: blur(24px);
            border: 1px solid var(--card-border);
            border-radius: 12px;
            box-shadow: 0 20px 60px rgba(0, 0, 0, 0.6);
            width: 100%;
            max-width: 600px;
            max-height: 400px;
            overflow: hidden;
            display: flex;
            flex-direction: column;
            text-align: left;
        }

        .command-palette-input-container {
            display: flex;
            align-items: center;
            padding: 16px 20px;
            border-bottom: 1px solid var(--card-border);
        }

        .command-palette-input {
            background: transparent;
            border: none;
            color: var(--text-primary);
            font-family: inherit;
            font-size: 1.1em;
            width: 100%;
            outline: none;
            padding: 0;
        }

        .command-palette-results {
            overflow-y: auto;
            max-height: 320px;
            padding: 8px;
        }

        .command-palette-item {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 10px 16px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 0.95em;
            color: var(--text-secondary);
            transition: all 0.15s;
        }

        .command-palette-item.selected {
            background: rgba(255, 255, 255, 0.08);
            color: var(--text-primary);
        }

        .command-palette-item-shortcut {
            font-size: 0.75em;
            background: rgba(255, 255, 255, 0.08);
            padding: 2px 6px;
            border-radius: 4px;
            color: var(--text-secondary);
            font-family: monospace;
        }

        .clickable-audit-card {
            transition: transform 0.2s, border-color 0.2s;
        }
        .clickable-audit-card:hover {
            transform: translateY(-2px);
            border-color: var(--accent-primary) !important;
        }
    </style>
</head>
<body>

    <div class="app-container">

        <!-- Banner Alert (kept for fallback) -->
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
                <div style="text-align: center; margin-top: 10px;">
                    <button type="button" onclick="openResetModal()" style="background: none; border: none; color: var(--accent-primary); cursor: pointer; font-size: 0.85em; text-decoration: underline;">Forgot password? Reset via Recovery Key</button>
                </div>
            </form>
        </div>

        <!-- 🔑 Recovery Key Display Modal (shown once after setup) -->
        <div id="recovery-key-modal" class="modal-overlay hidden" style="z-index: 9999;">
            <div class="modal-card" style="max-width: 520px; text-align: center;">
                <div style="font-size: 2.5em; margin-bottom: 12px;">🔑</div>
                <h3 style="margin-bottom: 8px; color: var(--accent-primary);">Save Your Recovery Key</h3>
                <p style="color: var(--text-secondary); margin-bottom: 16px; font-size: 0.9em;">This key can reset your master password. <strong style="color: var(--accent-red);">Store it safely — it will not be shown again.</strong></p>
                <div id="recovery-key-display" style="background: rgba(10,10,15,0.8); border: 1px solid var(--accent-primary); border-radius: 8px; padding: 16px; font-family: monospace; font-size: 1.1em; letter-spacing: 0.1em; color: var(--accent-primary); word-break: break-all; margin-bottom: 16px;"></div>
                <div style="display: flex; gap: 10px; justify-content: center;">
                    <button onclick="copyRecoveryKey()" class="btn btn-secondary" style="flex: 1;">📋 Copy Key</button>
                    <button onclick="closeRecoveryKeyModal()" class="btn btn-primary" style="flex: 1;">✅ I've Saved It</button>
                </div>
            </div>
        </div>

        <!-- 🔄 Reset Password Modal -->
        <div id="reset-password-modal" class="modal-overlay hidden" style="z-index: 9998;">
            <div class="modal-card" style="max-width: 440px;">
                <h3 style="margin-bottom: 16px; color: var(--accent-primary);">🔄 Reset Master Password</h3>
                <div class="form-group">
                    <label for="reset-recovery-key">Recovery Key</label>
                    <input type="text" id="reset-recovery-key" placeholder="Enter your 32-character recovery key" style="font-family: monospace;">
                </div>
                <div class="form-group">
                    <label for="reset-new-pass">New Master Password</label>
                    <input type="password" id="reset-new-pass" placeholder="Min 12 chars, mixed case + symbol">
                </div>
                <div class="form-group">
                    <label for="reset-confirm-pass">Confirm New Password</label>
                    <input type="password" id="reset-confirm-pass" placeholder="Confirm new password">
                </div>
                <div style="display: flex; gap: 10px; margin-top: 12px;">
                    <button onclick="closeResetModal()" class="btn btn-secondary" style="flex: 1;">Cancel</button>
                    <button onclick="handleResetPassword()" class="btn btn-primary" style="flex: 1;">Reset Password</button>
                </div>
            </div>
        </div>

        <!-- ✏️ Edit Credential Modal -->
        <div id="edit-credential-modal" class="modal-overlay hidden" style="z-index: 9996; overflow-y: auto;">
            <div class="modal-card" style="max-width: 540px; width: 100%; max-height: 90vh; overflow-y: auto;">
                <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 16px;">
                    <h3 style="color: var(--accent-primary); margin: 0;">✏️ Edit Credential</h3>
                    <button onclick="closeEditModal()" style="background:none;border:none;color:var(--text-secondary);cursor:pointer;font-size:1.4em;line-height:1;">✕</button>
                </div>

                <div class="form-group">
                    <label for="edit-category">Category</label>
                    <select id="edit-category" style="width:100%;">
                        <option value="Login">Login</option>
                        <option value="Secure Note">Secure Note</option>
                        <option value="Credit Card">Credit Card</option>
                        <option value="Identity">Identity</option>
                    </select>
                </div>

                <div class="form-group">
                    <label for="edit-site">Website / Title</label>
                    <input type="text" id="edit-site" placeholder="e.g. google.com" required>
                </div>

                <div class="form-group">
                    <label for="edit-user">Username / Email</label>
                    <input type="text" id="edit-user" placeholder="e.g. user@email.com" required>
                </div>

                <div class="form-group">
                    <label for="edit-pass">Password</label>
                    <div style="display:flex;gap:8px;align-items:center;">
                        <input type="password" id="edit-pass" placeholder="Password" required style="flex:1;" oninput="updateEditStrengthMeter(this.value)">
                        <button type="button" onclick="(()=>{const p=document.getElementById('edit-pass');p.type=p.type==='password'?'text':'password';})()" class="table-btn" style="height:44px;min-width:60px;">Show</button>
                    </div>
                    <!-- Live strength meter -->
                    <div style="margin-top:6px;">
                        <div style="background:rgba(255,255,255,0.07);border-radius:4px;height:5px;overflow:hidden;">
                            <div id="edit-strength-bar" style="height:5px;width:0%;border-radius:4px;transition:width 0.35s ease,background 0.35s ease;"></div>
                        </div>
                        <span id="edit-strength-label" style="font-size:0.73em;margin-top:4px;display:block;color:var(--text-secondary);"></span>
                    </div>
                </div>

                <div class="form-group">
                    <label for="edit-totp">TOTP Secret (2FA Key) — Optional</label>
                    <input type="text" id="edit-totp" placeholder="e.g. JBSWY3DPEHPK3PXP">
                </div>

                <div class="form-group">
                    <label for="edit-notes">Secure Notes — Optional</label>
                    <textarea id="edit-notes" placeholder="Recovery keys, pin codes..."></textarea>
                </div>

                <div class="form-group">
                    <label for="edit-tags">Custom Tags — Optional (comma-separated)</label>
                    <input type="text" id="edit-tags" placeholder="e.g. Work, Finance">
                </div>

                <div class="form-group">
                    <label for="edit-expiry">Password Expiry Date — Optional</label>
                    <input type="date" id="edit-expiry" style="color-scheme:dark;">
                </div>

                <div style="display:flex;gap:10px;margin-top:16px;">
                    <button onclick="closeEditModal()" class="btn btn-secondary" style="flex:1;">Cancel</button>
                    <button onclick="handleEditCredential()" class="btn btn-primary" style="flex:1;">💾 Save Changes</button>
                </div>
            </div>
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

            <!-- 🗂️ Three-Pane Dashboard Tab -->
            <div id="tab-vault" class="dashboard-layout">
                
                <!-- 1. Left Sidebar Pane: Category filters, custom tags, theme selector -->
                <aside class="sidebar-pane">
                    <div class="sidebar-section-title">Categories</div>
                    <ul class="sidebar-menu">
                        <li id="sidebar-cat-all" class="sidebar-item active" onclick="switchCategory('all')">
                            <span>📂 All Items</span>
                            <span id="badge-all" class="sidebar-badge">0</span>
                        </li>
                        <li id="sidebar-cat-Login" class="sidebar-item" onclick="switchCategory('Login')">
                            <span>🔑 Logins</span>
                            <span id="badge-login" class="sidebar-badge">0</span>
                        </li>
                        <li id="sidebar-cat-Secure-Note" class="sidebar-item" onclick="switchCategory('Secure Note')">
                            <span>📝 Secure Notes</span>
                            <span id="badge-note" class="sidebar-badge">0</span>
                        </li>
                        <li id="sidebar-cat-Credit-Card" class="sidebar-item" onclick="switchCategory('Credit Card')">
                            <span>💳 Credit Cards</span>
                            <span id="badge-card" class="sidebar-badge">0</span>
                        </li>
                        <li id="sidebar-cat-Identity" class="sidebar-item" onclick="switchCategory('Identity')">
                            <span>👤 Identities</span>
                            <span id="badge-identity" class="sidebar-badge">0</span>
                        </li>
                    </ul>

                    <div class="sidebar-section-title">Filters</div>
                    <ul class="sidebar-menu">
                        <li id="sidebar-cat-favorites" class="sidebar-item" onclick="switchCategory('favorites')">
                            <span>⭐ Favorites</span>
                            <span id="badge-favorites" class="sidebar-badge">0</span>
                        </li>
                    </ul>

                    <div class="sidebar-section-title">Tags</div>
                    <ul class="sidebar-menu" id="sidebar-tags-list">
                        <!-- Custom tags populated dynamically -->
                    </ul>

                    <div class="sidebar-section-title">Aesthetic Theme</div>
                    <select id="theme-selector" onchange="setTheme(this.value, true)" style="margin-top: 4px; padding: 8px; background: rgba(15,15,20,0.6); border: 1px solid var(--card-border); border-radius: 8px; color: var(--text-primary); outline: none; width: 100%;">
                        <option value="zinc">Default Zinc</option>
                        <option value="midnight">Midnight Blue</option>
                        <option value="cyberpunk">Cyberpunk</option>
                        <option value="emerald">Emerald Forest</option>
                    </select>
                </aside>

                <!-- 2. Middle Pane: Credentials table -->
                <main class="left-pane">
                    <div class="controls-row">
                        <div class="search-container">
                            <label>Filter:</label>
                            <input type="text" id="search-filter" oninput="renderTable()" placeholder="Filter by site or username...">
                        </div>
                        <div id="audit-filter-badge" class="hidden" style="background: rgba(59, 130, 246, 0.15); border: 1px solid var(--accent-primary); border-radius: 6px; padding: 4px 8px; font-size: 0.8em; display: flex; align-items: center; gap: 6px; white-space: nowrap;">
                            <span id="audit-filter-text">Weak</span>
                            <button onclick="clearAuditFilter()" style="background: transparent; border: none; color: var(--text-primary); cursor: pointer; font-weight: bold; font-size: 1.1em;">&times;</button>
                        </div>
                        <button onclick="showAddPane()" class="btn btn-primary" style="width: auto; padding: 8px 14px; font-size: 0.9em;">+ Add</button>
                        <button onclick="saveVaultChanges()" class="btn btn-secondary">Save</button>
                    </div>

                    <div class="table-wrapper">
                        <table id="credentials-table">
                            <thead>
                                <tr>
                                    <th>Website</th>
                                    <th>Username</th>
                                    <th>Password</th>
                                    <th style="text-align: right; width: 230px;">Actions</th>
                                </tr>
                            </thead>
                            <tbody id="table-body">
                                <!-- Dynamic Rows -->
                            </tbody>
                        </table>
                    </div>
                </main>

                <!-- 3. Right Pane: Switchable Add / Inspector pane -->
                <aside class="right-pane" style="display: flex; flex-direction: column; gap: 20px;">
                    
                    <!-- ADD CREDENTIAL PANE -->
                    <div id="add-pane">
                        <h3>ADD CREDENTIAL</h3>
                        <p class="subtitle" style="margin-bottom: 16px;">Save a new password record to the local database.</p>

                        <form id="add-form" onsubmit="handleAddCredential(event)">
                            <div class="form-group">
                                <label for="add-category">Category</label>
                                <select id="add-category" style="width: 100%;">
                                    <option value="Login">Login</option>
                                    <option value="Secure Note">Secure Note</option>
                                    <option value="Credit Card">Credit Card</option>
                                    <option value="Identity">Identity</option>
                                </select>
                            </div>
                            <div class="form-group">
                                <label for="add-site">Website / Title</label>
                                <input type="text" id="add-site" required placeholder="e.g. google.com">
                            </div>
                            <div class="form-group">
                                <label for="add-user">Username / Email</label>
                                <input type="text" id="add-user" required placeholder="e.g. user@email.com">
                            </div>
                            <div class="form-group">
                                <label for="add-pass">Password</label>
                                <input type="password" id="add-pass" required placeholder="Type password..." oninput="updateStrengthMeter(this.value)">
                                <!-- 🔴🟠🟡🟢 Live Password Strength Meter -->
                                <div id="pass-strength-wrap" style="margin-top:6px;">
                                    <div style="background:rgba(255,255,255,0.07);border-radius:4px;height:5px;overflow:hidden;">
                                        <div id="pass-strength-bar" style="height:5px;width:0%;border-radius:4px;transition:width 0.35s ease,background 0.35s ease;"></div>
                                    </div>
                                    <span id="pass-strength-label" style="font-size:0.73em;margin-top:4px;display:block;color:var(--text-secondary);"></span>
                                </div>
                            </div>
                            <div class="form-group">
                                <label for="add-totp">TOTP Secret (2FA Key) - Optional</label>
                                <input type="text" id="add-totp" placeholder="e.g. JBSWY3DPEHPK3PXP">
                            </div>
                            <div class="form-group">
                                <label for="add-notes">Secure Notes - Optional</label>
                                <textarea id="add-notes" placeholder="e.g. Recovery keys, pin codes, security questions..."></textarea>
                            </div>
                            
                            <!-- Custom Tags Input -->
                            <div class="form-group">
                                <label for="add-tags">Custom Tags - Optional (comma-separated)</label>
                                <input type="text" id="add-tags" placeholder="e.g. Work, Finance, Personal">
                            </div>

                            <!-- Password Expiry Date -->
                            <div class="form-group">
                                <label for="add-expiry">Password Expiry Date - Optional</label>
                                <input type="date" id="add-expiry" style="color-scheme:dark;">
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
                    </div>

                    <!-- CREDENTIAL DETAILS INSPECTOR PANE -->
                    <div id="details-pane" class="hidden" style="display: flex; flex-direction: column; gap: 16px;">
                        <div style="display: flex; justify-content: space-between; align-items: center;">
                            <h3>RECORD DETAILS</h3>
                            <button onclick="showAddPane()" class="btn btn-secondary" style="font-size: 0.85em; padding: 6px 12px; width: auto;">+ New</button>
                        </div>
                        <p class="subtitle" style="margin-bottom: 8px;">Detailed inspector for credential record.</p>

                        <div style="display: flex; flex-direction: column; gap: 12px;">
                            <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 10px;">
                                <div>
                                    <label>Category</label>
                                    <input type="text" id="details-category" readonly style="background: rgba(255,255,255,0.02); cursor: default;">
                                </div>
                                <div>
                                    <label>Website</label>
                                    <input type="text" id="details-site" readonly style="background: rgba(255,255,255,0.02); cursor: default;">
                                </div>
                            </div>
                            <div>
                                <label>Username / Email</label>
                                <input type="text" id="details-user" readonly style="background: rgba(255,255,255,0.02); cursor: default;">
                            </div>
                            <div>
                                <label>Password</label>
                                <div style="display: flex; gap: 8px; align-items: center; margin-top: 4px;">
                                    <input type="password" id="details-pass" readonly style="background: rgba(255,255,255,0.02); cursor: default; flex-grow: 1;">
                                    <button id="btn-details-show-pass" onclick="toggleDetailsPassword()" class="table-btn" style="height: 44px; min-width: 60px;">Show</button>
                                    <button onclick="copyDetailsPassword()" class="table-btn" style="height: 44px; min-width: 60px;">Copy</button>
                                </div>
                            </div>

                            <!-- Favorites Toggle -->
                            <div style="display: flex; gap: 8px; align-items: center;">
                                <button id="btn-details-favorite" onclick="toggleDetailsFavorite()" class="btn btn-secondary" style="font-size: 0.9em; padding: 8px 16px; flex-grow: 1;">⭐ Add to Favorites</button>
                            </div>

                            <!-- Details Tags -->
                            <div>
                                <label>Custom Tags</label>
                                <div style="display: flex; gap: 8px; align-items: center; margin-top: 4px;">
                                    <input type="text" id="details-tags" placeholder="No tags" style="flex-grow: 1;">
                                    <button onclick="saveDetailsTags()" class="table-btn" style="height: 44px; min-width: 60px;">Save</button>
                                </div>
                            </div>

                            <div id="details-totp-container" class="hidden">
                                <label style="color: var(--success-green);">Two-Factor Authentication (2FA)</label>
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
                                <label>Secure Notes</label>
                                <textarea id="details-notes" readonly style="background: rgba(255,255,255,0.02); cursor: default; min-height: 80px;"></textarea>
                            </div>

                            <div id="details-attachment-container" class="hidden">
                                <label>Attached File</label>
                                <div style="display: flex; justify-content: space-between; align-items: center; margin-top: 4px; background: rgba(59, 130, 246, 0.05); border: 1px solid rgba(59, 130, 246, 0.15); border-radius: 8px; padding: 10px 14px;">
                                    <span id="details-attachment-name" style="font-size: 0.9em; color: #93c5fd; font-weight: 500; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; max-width: 200px;">filename.pdf</span>
                                    <button onclick="downloadAttachment()" class="btn btn-secondary" style="font-size: 0.8em; padding: 6px 12px; margin: 0; width: auto;">Download</button>
                                </div>
                            </div>

                            <div id="details-history-container" class="hidden">
                                <label style="color: #c084fc;">Password History</label>
                                <div class="table-wrapper" style="max-height: 100px; margin-top: 4px;">
                                    <table style="width: 100%;">
                                        <tbody id="details-history-body">
                                        </tbody>
                                    </table>
                                </div>
                            </div>
                        </div>
                    </div>

                    <!-- Secure Password Generator Section -->
                    <div class="settings-section" style="margin-top: 10px;">
                        <div class="settings-title">Secure Generator</div>

                        <!-- Generator Mode Tabs -->
                        <div style="display:flex;gap:6px;margin-bottom:12px;">
                            <button id="gen-tab-pass" onclick="switchGenTab('password')" class="table-btn" style="flex:1;font-size:0.8em;background:rgba(59,130,246,0.2);border-color:var(--accent-blue);">🔑 Password</button>
                            <button id="gen-tab-phrase" onclick="switchGenTab('phrase')" class="table-btn" style="flex:1;font-size:0.8em;">💬 Passphrase</button>
                        </div>

                        <!-- Password Options -->
                        <div id="gen-panel-password">
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
                        </div>

                        <!-- Passphrase Options -->
                        <div id="gen-panel-phrase" style="display:none;">
                            <div class="form-group" style="margin-bottom:10px;">
                                <label for="gen-words">Words: <span id="gen-words-val">4</span></label>
                                <input type="range" id="gen-words" min="3" max="8" value="4" class="scale-slider" oninput="document.getElementById('gen-words-val').textContent=this.value; handleGeneratePassphrase();">
                            </div>
                            <div class="form-group" style="margin-bottom:12px;">
                                <label for="gen-sep">Separator</label>
                                <select id="gen-sep" onchange="handleGeneratePassphrase()" style="padding:6px 10px;border-radius:8px;background:rgba(10,10,15,0.7);color:var(--text-primary);border:1px solid var(--card-border);font-size:0.85em;">
                                    <option value="-">Hyphen (-)</option>
                                    <option value=".">Dot (.)</option>
                                    <option value="_">Underscore (_)</option>
                                    <option value=" ">Space</option>
                                </select>
                            </div>
                        </div>

                        <!-- Shared Result Row -->
                        <div style="display: flex; gap: 8px; align-items: center; margin-bottom: 10px;">
                            <input type="text" id="gen-result" readonly placeholder="Click Gen..." style="font-family: monospace; font-size: 0.9em; padding: 8px 12px; background: rgba(10, 10, 15, 0.7); cursor: default; border: 1px solid var(--card-border); border-radius: 8px; color: var(--text-primary); flex-grow: 1;">
                            <button onclick="copyGeneratedPassword()" class="table-btn" style="min-width: 50px; padding: 8px 12px;">Copy</button>
                        </div>
                    </div>

                    <div class="settings-section">
                        <div class="settings-title">Backup & Portability</div>
                        <div style="display: flex; gap: 10px; margin-top: 8px; flex-wrap: wrap;">
                            <button onclick="importCSV()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">Import CSV</button>
                            <button onclick="openExportModal()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">Export CSV</button>
                        </div>
                        <div style="display: flex; gap: 10px; margin-top: 8px; flex-wrap: wrap;">
                            <button onclick="importJSON()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">Import JSON (Bitwarden)</button>
                            <button onclick="openExportVaultModal()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">Export Encrypted Vault</button>
                        </div>
                    </div>

                    <div class="settings-section">
                        <div class="settings-title">Security</div>
                        <div style="display: flex; gap: 10px; margin-top: 8px; flex-wrap: wrap;">
                            <button onclick="openChangePasswordModal()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">🔑 Change Master Password</button>
                        </div>
                        <div style="display: flex; gap: 10px; margin-top: 8px; flex-wrap: wrap;">
                            <button onclick="showStoredRecoveryKey()" class="btn btn-secondary" style="flex: 1; font-size: 0.85em; padding: 8px 12px;">🔐 View Recovery Key</button>
                        </div>
                    </div>

                    <!-- Change Password Modal (inline) -->
                    <div id="change-password-modal" class="modal-overlay hidden" style="z-index: 9997;">
                        <div class="modal-card" style="max-width: 420px;">
                            <h3 style="margin-bottom: 16px; color: var(--accent-primary);">🔑 Change Master Password</h3>
                            <div class="form-group">
                                <label for="cp-current">Current Password</label>
                                <input type="password" id="cp-current" placeholder="Current master password">
                            </div>
                            <div class="form-group">
                                <label for="cp-new">New Password</label>
                                <input type="password" id="cp-new" placeholder="Min 12 chars, mixed + symbol">
                            </div>
                            <div class="form-group">
                                <label for="cp-confirm">Confirm New Password</label>
                                <input type="password" id="cp-confirm" placeholder="Confirm new password">
                            </div>
                            <div style="display: flex; gap: 10px; margin-top: 12px;">
                                <button onclick="closeChangePasswordModal()" class="btn btn-secondary" style="flex: 1;">Cancel</button>
                                <button onclick="handleChangePassword()" class="btn btn-primary" style="flex: 1;">Update Password</button>
                            </div>
                        </div>
                    </div>

                    <!-- Export Vault Modal (inline) -->
                    <div id="export-vault-modal" class="modal-overlay hidden" style="z-index: 9997;">
                        <div class="modal-card" style="max-width: 400px; text-align: center;">
                            <h3 style="margin-bottom: 12px; color: var(--accent-primary);">🔒 Export Encrypted Vault</h3>
                            <p style="color: var(--text-secondary); font-size: 0.9em; margin-bottom: 16px;">Your vault will be encrypted with your master password and saved as a <code>.vault</code> file.</p>
                            <div class="form-group">
                                <label for="export-vault-pass">Confirm Master Password</label>
                                <input type="password" id="export-vault-pass" placeholder="Enter master password">
                            </div>
                            <div style="display: flex; gap: 10px; margin-top: 12px;">
                                <button onclick="closeExportVaultModal()" class="btn btn-secondary" style="flex: 1;">Cancel</button>
                                <button onclick="doExportVault()" class="btn btn-primary" style="flex: 1;">Export</button>
                            </div>
                        </div>
                    </div>

                </aside>

            </div>

            <!-- 📊 Security Health Analytics & Dashboard -->
            <div id="tab-audit" class="dashboard-layout hidden">
                <main class="left-pane" style="grid-column: 1 / -1;">
                    <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 20px;">
                        <h3>Security Audit Dashboard</h3>
                        <p class="subtitle" style="margin: 0;">Audit and fix weak or reused credentials completely offline.</p>
                    </div>

                    <!-- SVG Score Ring and Stats Grid -->
                    <div style="display: flex; justify-content: center; align-items: center; gap: 40px; margin-bottom: 24px; background: rgba(18, 18, 24, 0.4); border: 1px solid var(--card-border); border-radius: 12px; padding: 24px; flex-wrap: wrap;">
                        <div style="position: relative; width: 150px; height: 150px; flex-shrink: 0;">
                            <svg width="150" height="150" viewBox="0 0 150 150" style="transform: rotate(-90deg);">
                                <circle cx="75" cy="75" r="65" stroke="rgba(255,255,255,0.05)" stroke-width="12" fill="transparent" />
                                <circle id="audit-score-ring" cx="75" cy="75" r="65" stroke="var(--accent-primary)" stroke-width="12" fill="transparent" stroke-dasharray="408.4" stroke-dashoffset="408.4" style="transition: stroke-dashoffset 0.8s ease-in-out, stroke 0.5s;" />
                            </svg>
                            <div style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; display: flex; flex-direction: column; align-items: center; justify-content: center;">
                                <span id="audit-score-percent" style="font-size: 2em; font-weight: 700; font-family: 'Outfit'; color: var(--text-primary);">100%</span>
                                <span style="font-size: 0.7em; text-transform: uppercase; color: var(--text-secondary); letter-spacing: 0.05em; margin-top: 2px;">Security Score</span>
                            </div>
                        </div>
                        
                        <div style="display: grid; grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); gap: 16px; flex-grow: 1; width: 100%; max-width: 700px;">
                            <div class="glass-card clickable-audit-card" onclick="filterVaultFromAudit('all')" style="padding: 16px; text-align: left; max-width: none; cursor: pointer;">
                                <div style="font-size: 0.8em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Total Records</div>
                                <div id="audit-total-count" style="font-size: 1.8em; font-weight: 700; margin-top: 4px; color: var(--text-primary);">0</div>
                            </div>
                            <div class="glass-card clickable-audit-card" onclick="filterVaultFromAudit('weak')" style="padding: 16px; text-align: left; max-width: none; border-color: rgba(239, 68, 68, 0.25); cursor: pointer;">
                                <div style="font-size: 0.8em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Weak Passwords</div>
                                <div id="audit-weak-count" style="font-size: 1.8em; font-weight: 700; margin-top: 4px; color: #fca5a5;">0</div>
                            </div>
                            <div class="glass-card clickable-audit-card" onclick="filterVaultFromAudit('reused')" style="padding: 16px; text-align: left; max-width: none; border-color: rgba(245, 158, 11, 0.25); cursor: pointer;">
                                <div style="font-size: 0.8em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Reused Passwords</div>
                                <div id="audit-reused-count" style="font-size: 1.8em; font-weight: 700; margin-top: 4px; color: #fde047;">0</div>
                            </div>
                            <div class="glass-card clickable-audit-card" onclick="filterVaultFromAudit('totp')" style="padding: 16px; text-align: left; max-width: none; border-color: rgba(16, 185, 129, 0.25); cursor: pointer;">
                                <div style="font-size: 0.8em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">2FA Protected</div>
                                <div id="audit-totp-count" style="font-size: 1.8em; font-weight: 700; margin-top: 4px; color: #86efac;">0</div>
                            </div>
                            <div class="glass-card clickable-audit-card" onclick="filterVaultFromAudit('duplicate')" style="padding: 16px; text-align: left; max-width: none; border-color: rgba(167, 139, 250, 0.25); cursor: pointer;">
                                <div style="font-size: 0.8em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Duplicate Passwords</div>
                                <div id="audit-dup-count" style="font-size: 1.8em; font-weight: 700; margin-top: 4px; color: #c4b5fd;">0</div>
                            </div>
                            <div class="glass-card clickable-audit-card" onclick="filterVaultFromAudit('expired')" style="padding: 16px; text-align: left; max-width: none; border-color: rgba(239, 68, 68, 0.2); cursor: pointer;">
                                <div style="font-size: 0.8em; text-transform: uppercase; color: var(--text-secondary); font-weight: 600;">Expired / Expiring</div>
                                <div id="audit-exp-count" style="font-size: 1.8em; font-weight: 700; margin-top: 4px; color: #fb923c;">0</div>
                            </div>
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

    <!-- 🥞 Stackable Toast Notification Container -->
    <div id="toast-container" class="toast-stack-container"></div>

    <!-- ⌨️ Command Palette Modal -->
    <div id="command-palette" class="command-palette-overlay hidden">
        <div class="command-palette-box">
            <div class="command-palette-input-container">
                <input type="text" id="command-palette-input" class="command-palette-input" placeholder="Search commands (e.g. > Lock) or credentials..." autocomplete="off">
            </div>
            <div id="command-palette-results" class="command-palette-results">
                <!-- Dynamic Results -->
            </div>
        </div>
    </div>

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

    <script>
        // JS Error/Log interceptors
        window.onerror = function(message, source, lineno, colno, error) {
            if (window.api_console_log) {
                window.api_console_log("ERROR: " + message + " at " + source + ":" + lineno + ":" + colno + " (Error object: " + error + ")");
            }
            return false;
        };
        window.addEventListener('unhandledrejection', function(event) {
            if (window.api_console_log) {
                window.api_console_log("UNHANDLED REJECTION: " + event.reason);
            }
        });

        // 🔹 Virtual Storage Polyfill to resolve WebView2 about:blank SecurityError
        let virtualStorage = {};

        function persistVirtualStorage() {
            if (window.api_save_settings) {
                const settingsToSave = { ...virtualStorage };
                delete settingsToSave.theme; // Theme is handled separately by api_set_theme
                window.api_save_settings(JSON.stringify(settingsToSave))
                    .catch(err => {
                        if (window.api_console_log) window.api_console_log("Failed to save settings: " + err);
                    });
            }
        }

        const localStorage = {
            getItem: function(key) {
                if (key === 'theme') {
                    return virtualStorage['theme'] || 'zinc';
                }
                return virtualStorage.hasOwnProperty(key) ? virtualStorage[key] : null;
            },
            setItem: function(key, value) {
                virtualStorage[key] = String(value);
                if (key === 'theme') {
                    if (window.api_set_theme) {
                        window.api_set_theme(String(value))
                            .catch(err => {
                                if (window.api_console_log) window.api_console_log("Failed to set theme: " + err);
                            });
                    }
                } else {
                    persistVirtualStorage();
                }
            },
            removeItem: function(key) {
                delete virtualStorage[key];
                if (key === 'theme') {
                    if (window.api_set_theme) {
                        window.api_set_theme('zinc')
                            .catch(err => {
                                if (window.api_console_log) window.api_console_log("Failed to remove theme: " + err);
                            });
                    }
                } else {
                    persistVirtualStorage();
                }
            },
            clear: function() {
                virtualStorage = {};
                if (window.api_set_theme) {
                    window.api_set_theme('zinc')
                        .catch(err => {
                            if (window.api_console_log) window.api_console_log("Failed to clear theme: " + err);
                        });
                }
                persistVirtualStorage();
            }
        };

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

        // Visual Suite variables
        let activeCategory = 'all';
        let activeAuditFilter = null;
        let commandPaletteSelectedIndex = 0;
        let commandPaletteItems = [];
        let selectedCredential = null;

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
                .catch(() => showToast("Failed to generate password.", "error"));
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
                    showToast("Vault locked automatically due to inactivity.", "info");
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
                    showToast("Password copied to clipboard! (Auto-clears in 20s)", "success", 4000);
                    
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
                                    showToast("Clipboard cleared for security.", "info", 2000);
                                }
                            });
                        clipboardPass = "";
                    }, 20000);
                })
                .catch(() => showToast("Failed to copy password.", "error"));
        }

        function startTotpPoller() {
            if (totpTimer) clearInterval(totpTimer);
            totpTimer = setInterval(() => {
                const dashboard = document.getElementById("dashboard-view");
                if (dashboard && !dashboard.classList.contains("hidden")) {
                    window.api_get_credentials()
                        .then(data => {
                            credentials = data.credentials || [];
                            
                            // update details pane totp if visible
                            const detailsPane = document.getElementById("details-pane");
                            if (detailsPane && !detailsPane.classList.contains("hidden") && selectedCredential) {
                                const cred = credentials.find(c => c.website === selectedCredential.website && c.username === selectedCredential.username);
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
                                showToast("Vault locked automatically due to system inactivity.", "info");
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

            // Set persisted theme
            if (window.api_get_theme) {
                window.api_get_theme()
                    .then(theme => {
                        virtualStorage['theme'] = theme || 'zinc';
                        setTheme(theme || 'zinc', false);
                    })
                    .catch(() => {
                        setTheme('zinc', false);
                    });
            } else {
                setTheme('zinc', false);
            }
            
            // Setup Command Palette search listener
            document.getElementById("command-palette-input").addEventListener("input", renderCommandPaletteResults);
        });

        // 🔹 Stackable Toast Notification System
        function showToast(message, type = 'info', duration = 4000) {
            const container = document.getElementById("toast-container");
            if (!container) return;
            
            const toast = document.createElement("div");
            toast.className = `toast-item toast-${type}`;
            toast.innerHTML = `
                <div class="toast-header">
                    <span class="toast-title" style="color: ${type === 'success' ? 'var(--success-green)' : type === 'error' ? 'var(--accent-red)' : 'var(--accent-primary)'};">${type}</span>
                    <button class="toast-close">&times;</button>
                </div>
                <div class="toast-content">${message}</div>
                <div class="toast-progress"></div>
            `;
            
            container.appendChild(toast);
            
            // Animate progress bar
            const progress = toast.querySelector(".toast-progress");
            progress.style.transition = `width ${duration}ms linear`;
            // Force reflow
            progress.offsetWidth;
            progress.style.width = "0%";
            
            const closeBtn = toast.querySelector(".toast-close");
            closeBtn.onclick = () => {
                toast.style.opacity = '0';
                toast.style.transform = 'translateX(100%)';
                toast.style.transition = 'all 0.2s ease-out';
                setTimeout(() => toast.remove(), 200);
            };
            
            setTimeout(() => {
                if (toast.parentNode) {
                    toast.style.opacity = '0';
                    toast.style.transform = 'translateX(100%)';
                    toast.style.transition = 'all 0.2s ease-out';
                    setTimeout(() => toast.remove(), 200);
                }
            }, duration);
        }

        // Legacy Alert Banner compatibility
        function showAlert(msg, isSuccess = false) {
            showToast(msg, isSuccess ? "success" : "error");
        }

        // 🔹 Theme Manager
        function setTheme(name, showToastAlert = false) {
            document.documentElement.setAttribute('data-theme', name);
            localStorage.setItem('theme', name);
            
            const selector = document.getElementById("theme-selector");
            if (selector) selector.value = name;
            
            if (showToastAlert) {
                const themeNames = {
                    zinc: "Default Zinc",
                    midnight: "Midnight Blue",
                    cyberpunk: "Cyberpunk",
                    emerald: "Emerald Forest"
                };
                showToast(`Applied ${themeNames[name]} theme!`, "success", 2000);
            }
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
                    showToast("Could not communicate with application backend.", "error");
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
                showToast("Passwords do not match.", "error");
                return;
            }

            if (pass.length < 12) {
                showToast("Master password must be at least 12 characters long.", "error");
                return;
            }

            if (!/[A-Z]/.test(pass) || !/[a-z]/.test(pass) || !/[0-9]/.test(pass) || !/[^A-Za-z0-9]/.test(pass)) {
                showToast("Master password must contain uppercase, lowercase, numbers, and symbols.", "error");
                return;
            }

            window.api_setup(pass)
            .then(data => {
                // Secure wipe inputs
                document.getElementById("setup-pass").value = "";
                document.getElementById("setup-confirm").value = "";
                document.getElementById("strength-meter-container").style.display = "none";

                if (data.success) {
                    // Show recovery key modal if returned
                    if (data.recoveryKey) {
                        document.getElementById("recovery-key-display").textContent = data.recoveryKey;
                        document.getElementById("recovery-key-modal").classList.remove("hidden");
                    } else {
                        showToast("Vault initialized successfully! Please login.", "success");
                    }
                    checkVaultStatus();
                } else {
                    showToast(data.error || "Failed to initialize vault.", "error");
                }
            })
            .catch(() => showToast("API request failed.", "error"));
        }

        // 🔹 Recovery Key Modal
        function copyRecoveryKey() {
            const key = document.getElementById("recovery-key-display").textContent;
            navigator.clipboard.writeText(key).catch(() => {});
            showToast("Recovery key copied to clipboard!", "success");
        }
        function closeRecoveryKeyModal() {
            document.getElementById("recovery-key-modal").classList.add("hidden");
            showToast("Vault ready. Please login.", "success");
        }

        // 🔹 Reset Password Modal
        function openResetModal() {
            document.getElementById("reset-recovery-key").value = "";
            document.getElementById("reset-new-pass").value = "";
            document.getElementById("reset-confirm-pass").value = "";
            document.getElementById("reset-password-modal").classList.remove("hidden");
        }
        function closeResetModal() {
            document.getElementById("reset-password-modal").classList.add("hidden");
        }
        function handleResetPassword() {
            const key = document.getElementById("reset-recovery-key").value.trim();
            const newPass = document.getElementById("reset-new-pass").value;
            const confirmPass = document.getElementById("reset-confirm-pass").value;
            if (!key) { showToast("Recovery key is required.", "error"); return; }
            if (newPass !== confirmPass) { showToast("Passwords do not match.", "error"); return; }
            if (newPass.length < 12) { showToast("Password must be at least 12 characters.", "error"); return; }
            window.api_reset_password(key, newPass)
            .then(data => {
                document.getElementById("reset-new-pass").value = "";
                document.getElementById("reset-confirm-pass").value = "";
                if (data.success) {
                    closeResetModal();
                    showToast("Password reset successful! Please login with your new password.", "success");
                } else {
                    showToast(data.error || "Reset failed.", "error");
                }
            })
            .catch(() => showToast("Reset failed.", "error"));
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
                    showToast("Vault unlocked successfully.", "success");
                    
                    if (window.api_get_settings) {
                        window.api_get_settings()
                            .then(data => {
                                try {
                                    const parsed = (typeof data === 'string') ? JSON.parse(data || "{}") : (data || {});
                                    const theme = virtualStorage['theme'] || 'zinc';
                                    virtualStorage = { ...parsed, theme };
                                } catch (e) {
                                    if (window.api_console_log) window.api_console_log("Failed to parse settings: " + e);
                                }
                                loadDashboard();
                            })
                            .catch(err => {
                                if (window.api_console_log) window.api_console_log("Failed to get settings: " + err);
                                loadDashboard();
                            });
                    } else {
                        loadDashboard();
                    }
                } else {
                    if (data.destructed) {
                        showToast(data.error || "Vault has been permanently self-destructed due to 10 consecutive failed attempts!", "error");
                    } else {
                        showToast(data.error || "Invalid password.", "error");
                    }
                    checkVaultStatus(); // Updates lockout status or shows setup view if self-destructed
                }
            })
            .catch(() => showToast("API request failed.", "error"));
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
                    updateSidebarCounts();
                    updateTagsSidebar();
                    
                    // Keep details panel updated with fresh data if matching
                    if (selectedCredential) {
                        const fresh = credentials.find(c => c.website === selectedCredential.website && c.username === selectedCredential.username);
                        if (fresh) {
                            openCredentialDetails(fresh);
                        } else {
                            showAddPane();
                        }
                    }
                })
                .catch(() => showToast("Failed to retrieve credentials.", "error"));
        }

        // 🔹 Category Switcher & Filter
        function switchCategory(cat) {
            activeCategory = cat;
            activeAuditFilter = null;
            const badge = document.getElementById("audit-filter-badge");
            if (badge) badge.classList.add("hidden");
            
            // Update active state in sidebar
            const sidebarItems = document.querySelectorAll(".sidebar-pane .sidebar-item");
            sidebarItems.forEach(item => item.classList.remove("active"));
            
            // Highlight current category
            const targetId = `sidebar-cat-${cat.replace(/\s+/g, '-')}`;
            const targetEl = document.getElementById(targetId);
            if (targetEl) {
                targetEl.classList.add("active");
            } else {
                // If it's a tag, search tag list items
                const tagItems = document.querySelectorAll("#sidebar-tags-list .sidebar-item");
                tagItems.forEach(item => {
                    if (item.getAttribute("data-tag") === cat) {
                        item.classList.add("active");
                    }
                });
            }
            
            renderTable();
        }

        // 🔹 Update Sidebar Count Badges
        function updateSidebarCounts() {
            let counts = {
                all: 0,
                login: 0,
                note: 0,
                card: 0,
                identity: 0,
                favorites: 0
            };
            
            credentials.forEach(c => {
                counts.all++;
                const cat = c.category || "Login";
                if (cat === "Login") counts.login++;
                else if (cat === "Secure Note") counts.note++;
                else if (cat === "Credit Card") counts.card++;
                else if (cat === "Identity") counts.identity++;
                
                const isFav = localStorage.getItem("fav||" + c.website + "||" + c.username) === "true";
                if (isFav) counts.favorites++;
            });
            
            const badgeAll = document.getElementById("badge-all");
            const badgeLogin = document.getElementById("badge-login");
            const badgeNote = document.getElementById("badge-note");
            const badgeCard = document.getElementById("badge-card");
            const badgeIdentity = document.getElementById("badge-identity");
            const badgeFavorites = document.getElementById("badge-favorites");
            
            if (badgeAll) badgeAll.textContent = counts.all;
            if (badgeLogin) badgeLogin.textContent = counts.login;
            if (badgeNote) badgeNote.textContent = counts.note;
            if (badgeCard) badgeCard.textContent = counts.card;
            if (badgeIdentity) badgeIdentity.textContent = counts.identity;
            if (badgeFavorites) badgeFavorites.textContent = counts.favorites;
        }

        // 🔹 Tags list updates
        function updateTagsSidebar() {
            let allTags = new Set();
            credentials.forEach(c => {
                let tagsStr = localStorage.getItem("tags||" + c.website + "||" + c.username) || "";
                if (tagsStr) {
                    tagsStr.split(",").forEach(t => {
                        let tag = t.trim();
                        if (tag) allTags.add(tag);
                    });
                }
            });

            const tagsMenu = document.getElementById("sidebar-tags-list");
            if (!tagsMenu) return;
            tagsMenu.innerHTML = "";
            
            if (allTags.size === 0) {
                tagsMenu.innerHTML = `<li style="font-size: 0.8em; color: var(--text-secondary); padding: 4px 14px; list-style: none;">No tags yet</li>`;
                return;
            }

            allTags.forEach(tag => {
                const li = document.createElement("li");
                li.className = "sidebar-item";
                li.setAttribute("data-tag", tag);
                if (activeCategory === tag) li.classList.add("active");
                li.onclick = () => switchCategory(tag);
                li.innerHTML = `<span>🏷️ ${tag}</span>`;
                tagsMenu.appendChild(li);
            });
        }

        function saveDetailsTags() {
            if (!selectedCredential) return;
            const tags = document.getElementById("details-tags").value.trim();
            localStorage.setItem("tags||" + selectedCredential.website + "||" + selectedCredential.username, tags);
            showToast("Tags updated successfully!", "success", 2000);
            updateTagsSidebar();
            renderTable();
        }

        // 🔹 Render Table
        function renderTable() {
            const tbody = document.getElementById("table-body");
            tbody.innerHTML = "";

            const filterText = document.getElementById("search-filter").value.toLowerCase();

            // Precalculate reused map for reused filtering
            let reusedMap = new Map();
            credentials.forEach(c => {
                if (!reusedMap.has(c.password)) reusedMap.set(c.password, 0);
                reusedMap.set(c.password, reusedMap.get(c.password) + 1);
            });

            credentials.forEach((c, idx) => {
                // Category filter
                if (activeCategory !== 'all') {
                    if (activeCategory === 'favorites') {
                        const isFav = localStorage.getItem("fav||" + c.website + "||" + c.username) === "true";
                        if (!isFav) return;
                    } else if (activeCategory === 'Login' || activeCategory === 'Secure Note' || activeCategory === 'Credit Card' || activeCategory === 'Identity') {
                        if (c.category !== activeCategory) return;
                    } else {
                        // Tag filter
                        let tagsStr = localStorage.getItem("tags||" + c.website + "||" + c.username) || "";
                        let hasTag = tagsStr.split(",").map(t => t.trim()).includes(activeCategory);
                        if (!hasTag) return;
                    }
                }

                // Security Audit Card filters
                if (activeAuditFilter) {
                    if (activeAuditFilter === 'weak' && c.password.length >= 12) return;
                    if (activeAuditFilter === 'totp' && (!c.totp_secret || c.totp_secret.trim() === "")) return;
                    if (activeAuditFilter === 'reused' && reusedMap.get(c.password) <= 1) return;
                    if (activeAuditFilter === 'duplicate') {
                        const cnt = credentials.filter(x => x.password === c.password).length;
                        if (cnt <= 1) return;
                    }
                    if (activeAuditFilter === 'expired' && c.expiryDate) {
                        const exp = new Date(c.expiryDate);
                        const tday = new Date(); tday.setHours(0,0,0,0);
                        const sn = new Date(tday); sn.setDate(sn.getDate()+30);
                        if (exp > sn) return;
                    } else if (activeAuditFilter === 'expired' && !c.expiryDate) {
                        return;
                    }
                }

                // Text filter
                if (filterText) {
                    const siteMatch = c.website.toLowerCase().includes(filterText);
                    const userMatch = c.username.toLowerCase().includes(filterText);
                    if (!siteMatch && !userMatch) return; // Skip non-matching rows
                }

                const row = document.createElement("tr");
                row.onclick = (e) => {
                    if (tdActions.contains(e.target)) return;
                    openCredentialDetails(c);
                };

                // Cell: Website (with optional expiry badge)
                const tdSite = document.createElement("td");
                const isFav = localStorage.getItem("fav||" + c.website + "||" + c.username) === "true";
                let expiryBadge = "";
                if (c.expiryDate) {
                    const exp = new Date(c.expiryDate);
                    const tday = new Date(); tday.setHours(0,0,0,0);
                    const sn = new Date(tday); sn.setDate(sn.getDate()+30);
                    if (exp < tday) {
                        expiryBadge = ` <span style="font-size:0.68em;background:rgba(239,68,68,0.18);color:#f87171;border:1px solid rgba(239,68,68,0.3);border-radius:4px;padding:1px 5px;vertical-align:middle;">EXPIRED</span>`;
                    } else if (exp <= sn) {
                        expiryBadge = ` <span style="font-size:0.68em;background:rgba(249,115,22,0.15);color:#fb923c;border:1px solid rgba(249,115,22,0.3);border-radius:4px;padding:1px 5px;vertical-align:middle;">EXPIRING</span>`;
                    }
                }
                tdSite.innerHTML = (isFav ? "⭐ " : "") + c.website + expiryBadge;
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

                // Edit Button
                const btnEdit = document.createElement("button");
                btnEdit.className = "table-btn";
                btnEdit.textContent = "✏️ Edit";
                btnEdit.style.background = "rgba(99,102,241,0.15)";
                btnEdit.style.borderColor = "rgba(99,102,241,0.4)";
                btnEdit.onclick = (e) => {
                    e.stopPropagation();
                    openEditModal(c);
                };
                tdActions.appendChild(btnEdit);

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
                        currentAttachedFile.data = "";
                        
                        const info = document.getElementById("attached-file-info");
                        info.textContent = data.fileName;
                        info.style.color = "var(--text-primary)";
                        document.getElementById("btn-clear-attachment").classList.remove("hidden");
                        showToast("File attached successfully!", "success", 2000);
                    } else if (data.error && data.error !== "Cancelled") {
                        showToast(`Failed to attach file: ${data.error}`, "error");
                    }
                })
                .catch(() => showToast("Attach file request failed.", "error"));
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
            const category = document.getElementById("add-category").value;
            const site = document.getElementById("add-site").value;
            const user = document.getElementById("add-user").value;
            const pass = document.getElementById("add-pass").value;
            const totp = document.getElementById("add-totp").value || "";
            const notes = document.getElementById("add-notes").value || "";
            const tags = document.getElementById("add-tags").value.trim();
            const expiry = document.getElementById("add-expiry").value || "";
            const attName = currentAttachedFile.name || "";

            window.api_add_credential(site, user, pass, totp, notes, attName, "", category, expiry)
            .then(data => {
                if (data.success) {
                    // Save tags in localStorage
                    if (tags) {
                        localStorage.setItem("tags||" + site + "||" + user, tags);
                    } else {
                        localStorage.removeItem("tags||" + site + "||" + user);
                    }

                    // Secure wipe inputs
                    document.getElementById("add-site").value = "";
                    document.getElementById("add-user").value = "";
                    document.getElementById("add-pass").value = "";
                    document.getElementById("add-totp").value = "";
                    document.getElementById("add-notes").value = "";
                    document.getElementById("add-tags").value = "";
                    document.getElementById("add-expiry").value = "";
                    updateStrengthMeter("");
                    clearAttachment();

                    showToast("Credential added and vault saved!", "success");
                    fetchCredentials();
                } else {
                    showToast(data.error || "Failed to add credential.", "error");
                }
            })
            .catch(() => showToast("API request failed.", "error"));
        }

        // 🔹 Delete credential
        function handleDeleteCredential(site, user) {
            window.api_delete_credential(site, user)
            .then(data => {
                if (data.success) {
                    showToast("Credential deleted & vault saved.", "success");
                    
                    // Clear tag and favorite data
                    localStorage.removeItem("tags||" + site + "||" + user);
                    localStorage.removeItem("fav||" + site + "||" + user);

                    // Clear visibility tracking for deleted entry
                    const key = site + "||" + user;
                    visiblePasswords.delete(key);

                    fetchCredentials();
                } else {
                    showToast(data.error || "Failed to delete credential.", "error");
                }
            })
            .catch(() => showToast("API request failed.", "error"));
        }

        // 🔹 Edit Credential Modal
        let editOriginalSite = "";
        let editOriginalUser = "";

        function openEditModal(c) {
            editOriginalSite = c.website;
            editOriginalUser = c.username;

            // Populate all fields
            document.getElementById("edit-category").value = c.category || "Login";
            document.getElementById("edit-site").value = c.website || "";
            document.getElementById("edit-user").value = c.username || "";
            document.getElementById("edit-pass").value = c.password || "";
            document.getElementById("edit-totp").value = c.totp_secret || "";
            document.getElementById("edit-notes").value = c.notes || "";
            document.getElementById("edit-expiry").value = c.expiryDate || "";
            // Tags from localStorage
            const storedTags = localStorage.getItem("tags||" + c.website + "||" + c.username) || "";
            document.getElementById("edit-tags").value = storedTags;
            // Reset pass strength
            updateEditStrengthMeter(c.password || "");
            // Show modal
            document.getElementById("edit-credential-modal").classList.remove("hidden");
        }

        function closeEditModal() {
            document.getElementById("edit-credential-modal").classList.add("hidden");
            document.getElementById("edit-pass").value = "";
        }

        function updateEditStrengthMeter(pass) {
            const bar = document.getElementById("edit-strength-bar");
            const label = document.getElementById("edit-strength-label");
            if (!bar || !label) return;
            let score = 0;
            if (pass.length >= 8)  score += 20;
            if (pass.length >= 12) score += 20;
            if (pass.length >= 16) score += 10;
            if (/[A-Z]/.test(pass)) score += 15;
            if (/[a-z]/.test(pass)) score += 10;
            if (/[0-9]/.test(pass)) score += 10;
            if (/[^A-Za-z0-9]/.test(pass)) score += 15;
            bar.style.width = score + "%";
            if (score >= 80) { bar.style.background = "#22c55e"; label.textContent = "Strong"; label.style.color = "#22c55e"; }
            else if (score >= 55) { bar.style.background = "#f59e0b"; label.textContent = "Moderate"; label.style.color = "#f59e0b"; }
            else if (score >= 30) { bar.style.background = "#f97316"; label.textContent = "Weak"; label.style.color = "#f97316"; }
            else { bar.style.background = "#ef4444"; label.textContent = "Very Weak"; label.style.color = "#ef4444"; }
        }

        function handleEditCredential() {
            const category = document.getElementById("edit-category").value;
            const site     = document.getElementById("edit-site").value.trim();
            const user     = document.getElementById("edit-user").value.trim();
            const pass     = document.getElementById("edit-pass").value;
            const totp     = document.getElementById("edit-totp").value || "";
            const notes    = document.getElementById("edit-notes").value || "";
            const tags     = document.getElementById("edit-tags").value.trim();
            const expiry   = document.getElementById("edit-expiry").value || "";

            if (!site || !user || !pass) {
                showToast("Website, username, and password are required.", "error");
                return;
            }

            // If site or user changed, delete the old entry first
            const siteChanged = (site !== editOriginalSite);
            const userChanged = (user !== editOriginalUser);

            const doSave = () => {
                window.api_add_credential(site, user, pass, totp, notes, "", "", category, expiry)
                .then(data => {
                    if (data.success) {
                        // Save tags
                        if (tags) {
                            localStorage.setItem("tags||" + site + "||" + user, tags);
                        } else {
                            localStorage.removeItem("tags||" + site + "||" + user);
                        }
                        // Clean up old key tags if renamed
                        if (siteChanged || userChanged) {
                            localStorage.removeItem("tags||" + editOriginalSite + "||" + editOriginalUser);
                            localStorage.removeItem("fav||" + editOriginalSite + "||" + editOriginalUser);
                        }
                        closeEditModal();
                        showToast("Credential updated and vault saved!", "success");
                        fetchCredentials();
                    } else {
                        showToast(data.error || "Failed to update credential.", "error");
                    }
                })
                .catch(() => showToast("API request failed.", "error"));
            };

            // Delete old entry if key fields changed
            if (siteChanged || userChanged) {
                window.api_delete_credential(editOriginalSite, editOriginalUser)
                .then(() => doSave())
                .catch(() => doSave()); // still try save even if delete fails
            } else {
                doSave();
            }
        }

        // 🔹 Save changes explicitly
        function saveVaultChanges() {
            window.api_save()
                .then(data => {
                    if (data.success) {
                        showToast("Vault saved successfully.", "success", 2000);
                    } else {
                        showToast(data.error || "Failed to save vault.", "error");
                    }
                })
                .catch(() => showToast("API request failed.", "error"));
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
                        selectedCredential = null;
                        visiblePasswords.clear();
                        
                        // Clear virtual settings on lock (except theme)
                        const theme = virtualStorage['theme'] || 'zinc';
                        virtualStorage = { theme };
                        
                        showToast("Vault locked successfully.", "success");
                        checkVaultStatus();
                    }
                })
                .catch(() => showToast("API request failed.", "error"));
        }

        // 🔹 Credential Details Inspector Pane Helpers
        function openCredentialDetails(c) {
            selectedCredential = c;
            
            document.getElementById("add-pane").classList.add("hidden");
            document.getElementById("details-pane").classList.remove("hidden");
            
            document.getElementById("details-category").value = c.category || "Login";
            document.getElementById("details-site").value = c.website;
            document.getElementById("details-user").value = c.username;
            document.getElementById("details-pass").value = c.password;
            
            // Password show/hide toggle state reset
            const passInput = document.getElementById("details-pass");
            passInput.type = "password";
            document.getElementById("btn-details-show-pass").textContent = "Show";

            // Favorite button label update
            const isFav = localStorage.getItem("fav||" + c.website + "||" + c.username) === "true";
            document.getElementById("btn-details-favorite").textContent = isFav ? "⭐ Remove Favorite" : "⭐ Add to Favorites";

            // Tags input value
            document.getElementById("details-tags").value = localStorage.getItem("tags||" + c.website + "||" + c.username) || "";

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

            // Password History Section (within Details pane)
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
        }

        function showAddPane() {
            selectedCredential = null;
            document.getElementById("add-pane").classList.remove("hidden");
            document.getElementById("details-pane").classList.add("hidden");
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

        function toggleDetailsFavorite() {
            if (!selectedCredential) return;
            const key = "fav||" + selectedCredential.website + "||" + selectedCredential.username;
            const isFav = localStorage.getItem(key) === "true";
            
            if (isFav) {
                localStorage.removeItem(key);
                document.getElementById("btn-details-favorite").textContent = "⭐ Add to Favorites";
                showToast("Removed from Favorites", "info", 2000);
            } else {
                localStorage.setItem(key, "true");
                document.getElementById("btn-details-favorite").textContent = "⭐ Remove Favorite";
                showToast("Added to Favorites!", "success", 2000);
            }
            
            updateSidebarCounts();
            renderTable();
        }

        function copyDetailsTotp() {
            const val = document.getElementById("details-totp-code").value;
            if (val) {
                navigator.clipboard.writeText(val)
                    .then(() => showToast("TOTP code copied to clipboard!", "success", 2000))
                    .catch(() => showToast("Failed to copy TOTP code.", "error"));
            }
        }

        function downloadAttachment() {
            const site = document.getElementById("details-site").value;
            const user = document.getElementById("details-user").value;
            const cred = credentials.find(c => c.website === site && c.username === user);
            if (cred && cred.attachmentName) {
                window.api_download_file(site, user)
                    .then(data => {
                        if (data.success) {
                            showToast(`File downloaded successfully to ${data.path}`, "success");
                        } else if (data.error && data.error !== "Cancelled") {
                            showToast(`Download failed: ${data.error}`, "error");
                        }
                    })
                    .catch(() => showToast("Download request failed.", "error"));
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
                                showToast("Incorrect master password.", "error");
                            }
                        }).catch(() => showToast("Incorrect master password.", "error"));
                    }
                })
                .catch(() => showToast("Verification failed.", "error"));
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
                    .catch(() => showToast("Failed to close application.", "error"));
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

        // ⚡ Feature 1: Live Password Strength Meter
        function updateStrengthMeter(pass) {
            const bar = document.getElementById("pass-strength-bar");
            const label = document.getElementById("pass-strength-label");
            if (!bar || !label) return;
            if (!pass) { bar.style.width = "0%"; label.textContent = ""; return; }

            let score = 0;
            if (pass.length >= 8)  score += 15;
            if (pass.length >= 12) score += 15;
            if (pass.length >= 16) score += 10;
            if (pass.length >= 20) score += 10;
            if (/[A-Z]/.test(pass)) score += 10;
            if (/[a-z]/.test(pass)) score += 10;
            if (/[0-9]/.test(pass)) score += 10;
            if (/[^A-Za-z0-9]/.test(pass)) score += 15;
            // Penalty for repeating chars
            const unique = new Set(pass).size;
            if (unique < pass.length * 0.5) score -= 10;
            score = Math.max(0, Math.min(100, score));

            const levels = [
                { min: 0,  max: 25,  label: "Weak",        color: "#ef4444" },
                { min: 26, max: 49,  label: "Fair",        color: "#f97316" },
                { min: 50, max: 69,  label: "Good",        color: "#eab308" },
                { min: 70, max: 89,  label: "Strong",      color: "#22c55e" },
                { min: 90, max: 100, label: "Very Strong", color: "#10b981" }
            ];
            const lvl = levels.find(l => score >= l.min && score <= l.max) || levels[0];
            bar.style.width = score + "%";
            bar.style.background = lvl.color;
            label.textContent = lvl.label;
            label.style.color = lvl.color;
        }

        // ⚡ Feature 2: Generator Tab Switcher
        function switchGenTab(mode) {
            const passPanel = document.getElementById("gen-panel-password");
            const phrasePanel = document.getElementById("gen-panel-phrase");
            const passBtn = document.getElementById("gen-tab-pass");
            const phraseBtn = document.getElementById("gen-tab-phrase");
            if (mode === 'password') {
                passPanel.style.display = "";
                phrasePanel.style.display = "none";
                passBtn.style.background = "rgba(59,130,246,0.2)";
                passBtn.style.borderColor = "var(--accent-blue)";
                phraseBtn.style.background = "";
                phraseBtn.style.borderColor = "";
                handleGeneratePassword();
            } else {
                passPanel.style.display = "none";
                phrasePanel.style.display = "";
                phraseBtn.style.background = "rgba(59,130,246,0.2)";
                phraseBtn.style.borderColor = "var(--accent-blue)";
                passBtn.style.background = "";
                passBtn.style.borderColor = "";
                handleGeneratePassphrase();
            }
        }

        // ⚡ Feature 2: Passphrase Generator
        function handleGeneratePassphrase() {
            const words = parseInt(document.getElementById("gen-words").value) || 4;
            const sep = document.getElementById("gen-sep").value;
            window.api_generate_passphrase(words.toString(), sep)
                .then(data => {
                    if (data && data.passphrase) {
                        document.getElementById("gen-result").value = data.passphrase;
                    }
                })
                .catch(() => showToast("Failed to generate passphrase.", "error"));
        }

        // 🔹 Security Auditor Filters
        function filterVaultFromAudit(type) {
            activeAuditFilter = type;
            
            const badge = document.getElementById("audit-filter-badge");
            const badgeText = document.getElementById("audit-filter-text");
            
            if (type && type !== 'all') {
                badge.classList.remove("hidden");
                const filterNames = {
                    weak: "Weak Passwords",
                    reused: "Reused Passwords",
                    totp: "2FA Protected",
                    duplicate: "Duplicate Passwords",
                    expired: "Expired / Expiring Soon"
                };
                badgeText.textContent = filterNames[type] || type;
            } else {
                activeAuditFilter = null;
                badge.classList.add("hidden");
            }
            
            switchTab('vault');
        }

        function clearAuditFilter() {
            filterVaultFromAudit(null);
        }

        // 🔹 Run client-side completely offline security audit
        function runSecurityAudit() {
            const weakList = document.getElementById("audit-weak-list");
            const reusedList = document.getElementById("audit-reused-list");
            weakList.innerHTML = "";
            reusedList.innerHTML = "";

            let weakCount = 0;
            let reusedMap = new Map();
            let totpCount = 0;
            let dupCount = 0;
            let expiredCount = 0;
            const today = new Date();
            today.setHours(0,0,0,0);
            const soon = new Date(today);
            soon.setDate(soon.getDate() + 30);

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

                if (c.totp_secret && c.totp_secret.trim() !== "") {
                    totpCount++;
                }

                // Expiry tracking
                if (c.expiryDate) {
                    const exp = new Date(c.expiryDate);
                    if (exp <= soon) expiredCount++;
                }
            });

            let reusedCount = 0;
            reusedMap.forEach((users, pass) => {
                if (users.length > 1) {
                    reusedCount += users.length;
                    dupCount += users.length;
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

            // Update DOM element values
            document.getElementById("audit-total-count").textContent = credentials.length;
            document.getElementById("audit-weak-count").textContent = weakCount;
            document.getElementById("audit-reused-count").textContent = reusedCount;
            document.getElementById("audit-totp-count").textContent = totpCount;
            document.getElementById("audit-dup-count").textContent = dupCount;
            document.getElementById("audit-exp-count").textContent = expiredCount;

            // Calculate Overall Security Score
            let totalScore = 100;
            if (credentials.length > 0) {
                let sum = 0;
                credentials.forEach(c => {
                    let s = 0;
                    const len = c.password.length;
                    if (len >= 12) {
                        const hasUpper = /[A-Z]/.test(c.password);
                        const hasLower = /[a-z]/.test(c.password);
                        const hasDigit = /[0-9]/.test(c.password);
                        const hasSpecial = /[^A-Za-z0-9]/.test(c.password);
                        if (hasUpper && hasLower && hasDigit && hasSpecial) s += 80;
                        else s += 60;
                    } else if (len >= 8) {
                        s += 40;
                    } else {
                        s += 10;
                    }

                    // Reuse penalty
                    if (reusedMap.get(c.password).length > 1) {
                        s -= 30;
                    }

                    // 2FA bonus
                    if (c.totp_secret && c.totp_secret.trim() !== "") {
                        s += 20;
                    }

                    // Expiry penalty
                    if (c.expiryDate) {
                        const exp = new Date(c.expiryDate);
                        if (exp < today)      s -= 10;  // Expired
                        else if (exp <= soon) s -= 5;   // Expiring soon
                    }

                    // Clamp
                    s = Math.max(0, Math.min(100, s));
                    sum += s;
                });
                totalScore = Math.round(sum / credentials.length);
            }

            // Update DOM element values
            document.getElementById("audit-total-count").textContent = credentials.length;
            document.getElementById("audit-weak-count").textContent = weakCount;
            document.getElementById("audit-reused-count").textContent = reusedCount;
            document.getElementById("audit-totp-count").textContent = totpCount;

            // SVG circular progress ring animation
            const ring = document.getElementById("audit-score-ring");
            const percentText = document.getElementById("audit-score-percent");
            if (ring && percentText) {
                percentText.textContent = `${totalScore}%`;
                
                // Circumference = 2 * PI * r = 2 * 3.14159 * 65 = 408.4
                const circumference = 408.4;
                const offset = circumference - (totalScore / 100) * circumference;
                ring.style.strokeDashoffset = offset;
                
                // Stroke color based on score
                if (totalScore >= 85) {
                    ring.style.stroke = "var(--success-green)";
                } else if (totalScore >= 60) {
                    ring.style.stroke = "#f59e0b"; // Amber/Orange
                } else {
                    ring.style.stroke = "var(--accent-red)";
                }
            }
        }

        // 🔹 CSV Backup functions
        function openExportModal() {
            openVerifyModal("export", null);
        }

        function exportCSV(pass) {
            window.api_export_csv(pass)
                .then(data => {
                    if (data.success) {
                        showToast(`Vault successfully exported to ${data.path}`, "success");
                    } else {
                        showToast(data.error || "Export failed.", "error");
                    }
                })
                .catch(() => showToast("Export failed.", "error"));
        }

        function importCSV() {
            window.api_import_csv()
                .then(data => {
                    if (data.success) {
                        showToast(`Successfully imported ${data.imported} credentials!`, "success");
                        fetchCredentials();
                    } else {
                        showToast(data.error || "Import failed.", "error");
                    }
                })
                .catch(() => showToast("Import failed.", "error"));
        }

        // 🔹 JSON Import (Bitwarden)
        function importJSON() {
            window.api_import_json()
                .then(data => {
                    if (data.success) {
                        showToast(`Successfully imported ${data.imported} credentials from JSON!`, "success");
                        fetchCredentials();
                    } else {
                        showToast(data.error || "JSON import failed.", "error");
                    }
                })
                .catch(() => showToast("JSON import failed.", "error"));
        }

        // 🔹 Encrypted Vault Export
        function openExportVaultModal() {
            document.getElementById("export-vault-pass").value = "";
            document.getElementById("export-vault-modal").classList.remove("hidden");
        }
        function closeExportVaultModal() {
            document.getElementById("export-vault-modal").classList.add("hidden");
        }
        function doExportVault() {
            const pass = document.getElementById("export-vault-pass").value;
            if (!pass) { showToast("Please enter master password.", "error"); return; }
            window.api_export_vault(pass)
            .then(data => {
                document.getElementById("export-vault-pass").value = "";
                if (data.success) {
                    closeExportVaultModal();
                    showToast(`Vault exported to: ${data.path}`, "success");
                } else {
                    showToast(data.error || "Export failed.", "error");
                }
            })
            .catch(() => showToast("Export failed.", "error"));
        }

        // 🔹 Change Master Password
        function openChangePasswordModal() {
            document.getElementById("cp-current").value = "";
            document.getElementById("cp-new").value = "";
            document.getElementById("cp-confirm").value = "";
            document.getElementById("change-password-modal").classList.remove("hidden");
        }
        function closeChangePasswordModal() {
            document.getElementById("change-password-modal").classList.add("hidden");
        }
        function handleChangePassword() {
            const curr = document.getElementById("cp-current").value;
            const newp = document.getElementById("cp-new").value;
            const conf = document.getElementById("cp-confirm").value;
            if (!curr) { showToast("Please enter current password.", "error"); return; }
            if (newp !== conf) { showToast("New passwords do not match.", "error"); return; }
            if (newp.length < 12) { showToast("New password must be at least 12 characters.", "error"); return; }
            window.api_change_password(curr, newp)
            .then(data => {
                document.getElementById("cp-current").value = "";
                document.getElementById("cp-new").value = "";
                document.getElementById("cp-confirm").value = "";
                if (data.success) {
                    closeChangePasswordModal();
                    showToast("Master password changed successfully!", "success");
                } else {
                    showToast(data.error || "Failed to change password.", "error");
                }
            })
            .catch(() => showToast("Failed to change password.", "error"));
        }

        // 🔹 View stored recovery key
        function showStoredRecoveryKey() {
            window.api_get_recovery_key()
            .then(data => {
                if (data.success && data.recoveryKey) {
                    document.getElementById("recovery-key-display").textContent = data.recoveryKey;
                    document.getElementById("recovery-key-modal").classList.remove("hidden");
                } else {
                    showToast(data.error || "No recovery key found.", "error");
                }
            })
            .catch(() => showToast("Failed to retrieve recovery key.", "error"));
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

        // 🔹 Command Palette Management
        const staticCommands = [
            { name: "Lock Vault", action: () => handleLock(), shortcut: "Ctrl+L" },
            { name: "Security Audit Tab", action: () => switchTab('audit'), shortcut: "Alt+A" },
            { name: "Vault Tab", action: () => switchTab('vault'), shortcut: "Alt+V" },
            { name: "Export to CSV", action: () => openExportModal(), shortcut: "" },
            { name: "Import from CSV", action: () => importCSV(), shortcut: "" },
            { name: "Theme: Default Zinc", action: () => setTheme('zinc', true), shortcut: "" },
            { name: "Theme: Midnight Blue", action: () => setTheme('midnight', true), shortcut: "" },
            { name: "Theme: Cyberpunk", action: () => setTheme('cyberpunk', true), shortcut: "" },
            { name: "Theme: Emerald Forest", action: () => setTheme('emerald', true), shortcut: "" },
            { name: "Generate & Copy Password", action: () => { handleGeneratePassword(); setTimeout(() => copyGeneratedPassword(), 100); }, shortcut: "" }
        ];

        function openCommandPalette() {
            const palette = document.getElementById("command-palette");
            palette.classList.remove("hidden");
            const input = document.getElementById("command-palette-input");
            input.value = "";
            input.focus();
            renderCommandPaletteResults();
        }

        function closeCommandPalette() {
            const palette = document.getElementById("command-palette");
            palette.classList.add("hidden");
        }

        // Bind global key listener for Command Palette shortcut
        window.addEventListener("keydown", (e) => {
            if ((e.ctrlKey || e.metaKey) && e.key.toLowerCase() === 'k') {
                e.preventDefault();
                const palette = document.getElementById("command-palette");
                if (palette.classList.contains("hidden")) {
                    openCommandPalette();
                } else {
                    closeCommandPalette();
                }
            }
            
            const palette = document.getElementById("command-palette");
            if (!palette.classList.contains("hidden")) {
                if (e.key === "ArrowDown") {
                    e.preventDefault();
                    navigateCommandPalette(1);
                } else if (e.key === "ArrowUp") {
                    e.preventDefault();
                    navigateCommandPalette(-1);
                } else if (e.key === "Enter") {
                    e.preventDefault();
                    executeCommandPaletteSelected();
                } else if (e.key === "Escape") {
                    e.preventDefault();
                    closeCommandPalette();
                }
            }
        });

        // Close command palette when clicking outside
        document.getElementById("command-palette").onclick = (e) => {
            if (e.target === document.getElementById("command-palette")) {
                closeCommandPalette();
            }
        };

        function navigateCommandPalette(direction) {
            const items = document.querySelectorAll(".command-palette-item");
            if (items.length === 0) return;
            
            items[commandPaletteSelectedIndex].classList.remove("selected");
            commandPaletteSelectedIndex = (commandPaletteSelectedIndex + direction + items.length) % items.length;
            items[commandPaletteSelectedIndex].classList.add("selected");
            items[commandPaletteSelectedIndex].scrollIntoView({ block: "nearest" });
        }

        function executeCommandPaletteSelected() {
            if (commandPaletteItems.length > 0 && commandPaletteSelectedIndex < commandPaletteItems.length) {
                const item = commandPaletteItems[commandPaletteSelectedIndex];
                item.action();
                closeCommandPalette();
            }
        }

        function renderCommandPaletteResults() {
            const query = document.getElementById("command-palette-input").value.toLowerCase();
            const resultsDiv = document.getElementById("command-palette-results");
            resultsDiv.innerHTML = "";
            
            commandPaletteItems = [];
            
            // 1. Match static commands
            staticCommands.forEach(cmd => {
                if (cmd.name.toLowerCase().includes(query) || (query.startsWith(">") && cmd.name.toLowerCase().includes(query.slice(1).trim()))) {
                    commandPaletteItems.push({
                        type: "command",
                        label: `⚙️ ${cmd.name}`,
                        shortcut: cmd.shortcut,
                        action: cmd.action
                    });
                }
            });
            
            // 2. Match credentials
            credentials.forEach(c => {
                if (c.website.toLowerCase().includes(query) || c.username.toLowerCase().includes(query)) {
                    commandPaletteItems.push({
                        type: "credential",
                        label: `🔑 ${c.website} — ${c.username}`,
                        shortcut: c.category,
                        action: () => {
                            switchTab('vault');
                            openCredentialDetails(c);
                        }
                    });
                }
            });
            
            commandPaletteSelectedIndex = 0;
            
            commandPaletteItems.forEach((item, index) => {
                const div = document.createElement("div");
                div.className = "command-palette-item";
                if (index === 0) div.classList.add("selected");
                
                div.onclick = () => {
                    item.action();
                    closeCommandPalette();
                };
                
                const labelSpan = document.createElement("span");
                labelSpan.textContent = item.label;
                div.appendChild(labelSpan);
                
                if (item.shortcut) {
                    const shortcutSpan = document.createElement("span");
                    shortcutSpan.className = "command-palette-item-shortcut";
                    shortcutSpan.textContent = item.shortcut;
                    div.appendChild(shortcutSpan);
                }
                
                resultsDiv.appendChild(div);
            });
            
            if (commandPaletteItems.length === 0) {
                resultsDiv.innerHTML = `<div style="padding: 12px; color: var(--text-secondary); text-align: center;">No results found</div>`;
            }
        }

        function closeHistoryModal() {
            document.getElementById("history-modal").classList.add("hidden");
        }
    </script>

    </body>
)rawhtml";

#endif
