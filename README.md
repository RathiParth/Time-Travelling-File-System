<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Time-Travelling File System Simulator</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <!-- Chosen Palette: Serene Sage -->
    <!-- Application Structure Plan: The SPA is designed as a three-panel interactive simulator. 1) Left Control Panel: for command input and logging, mimicking the user's direct interaction. 2) Center Visualization Panel: for displaying the core data structure—the version history tree of the selected file. 3) Right State Panel: for showing system-wide status (all files, analytics heaps, active content). This structure was chosen because it provides a direct cause-and-effect learning experience. Users can execute a command and immediately see its consequences across the entire system, making abstract concepts like branching trees and heap-based analytics tangible and easy to understand. This is far more effective for educational purposes than a static report. -->
    <!-- Visualization & Content Choices: 
        - All Files List: Goal: Organize/Navigate. Method: Interactive HTML list. Interaction: Click to select the file to visualize. Justification: Provides a simple, clear way for users to switch the main context of the application.
        - Heaps (Recent Files/Biggest Trees): Goal: Compare/Analyze. Method: Chart.js horizontal bar charts. Interaction: Charts dynamically update after commands like CREATE, INSERT, UPDATE. Justification: Bar charts are the standard and most effective way to visualize ranked data, clearly showing the "top N" files as required by the report.
        - Version History Tree: Goal: Show Relationships/Hierarchy. Method: Rendered with HTML `div`s and CSS pseudo-elements for connecting lines. Interaction: Nodes are highlighted to show the 'active' version and the `HISTORY` path to the root. Justification: This custom HTML/CSS approach offers superior control over layout, styling, and interactivity for a branching tree structure compared to the limitations of standard charting libraries.
        - Active File Content: Goal: Inform. Method: HTML `<pre>` tag. Interaction: Content updates in real-time with file operations. Justification: Clearly displays the text content of any version, preserving basic formatting. -->
    <!-- CONFIRMATION: NO SVG graphics used. NO Mermaid JS used. -->
    <style>
        :root {
            --bg-main: #F5F5F4; /* Stone 100 */
            --bg-panel: #FFFFFF;
            --text-primary: #334155; /* Slate 700 */
            --text-secondary: #64748B; /* Slate 500 */
            --border-color: #E7E5E4; /* Stone 200 */
            --accent-primary: #4A90E2; /* A calm blue */
            --accent-primary-hover: #357ABD;
            --node-active: #4A90E2;
            --node-snapshot: #10B981; /* Emerald 500 */
            --node-history: #F59E0B; /* Amber 500 */
            --font-sans: 'Inter', sans-serif;
        }
        body {
            font-family: var(--font-sans);
            background-color: var(--bg-main);
            color: var(--text-primary);
        }
        .panel {
            background-color: var(--bg-panel);
            border-radius: 0.75rem;
            border: 1px solid var(--border-color);
            box-shadow: 0 4px 6px -1px rgb(0 0 0 / 0.1), 0 2px 4px -2px rgb(0 0 0 / 0.1);
            padding: 1.5rem;
        }
        .tree-container ul {
            position: relative;
            padding-left: 20px;
            list-style: none;
        }
        .tree-container li {
            position: relative;
        }
        .tree-container li::before, .tree-container li::after {
            content: '';
            position: absolute;
            left: -12px;
        }
        .tree-container li::before {
            border-top: 1px solid var(--border-color);
            top: 18px;
            width: 12px;
            height: 0;
        }
        .tree-container li:last-child::after {
            height: 18px;
        }
        .tree-container ul > li::after {
            border-left: 1px solid var(--border-color);
            height: 100%;
            width: 0px;
            top: -14px;
        }
        .chart-container {
            position: relative;
            width: 100%;
            height: 250px;
            max-height: 250px;
        }
    </style>
     <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap" rel="stylesheet">
</head>
<body class="p-4 lg:p-6">
    <main class="max-w-screen-2xl mx-auto">
        <header class="text-center mb-6">
            <h1 class="text-3xl lg:text-4xl font-bold tracking-tight">Time-Travelling File System</h1>
            <p class="text-lg text-text-secondary mt-1">An interactive simulator for the version control system.</p>
        </header>

        <div class="grid grid-cols-1 lg:grid-cols-4 gap-6">
            
            <aside class="lg:col-span-1 flex flex-col gap-6">
                <div class="panel">
                    <h2 class="text-xl font-semibold mb-3">Controls</h2>
                    <p class="text-sm text-text-secondary mb-4">Enter a command below and press 'Run' to see its effect on the file system state. Use the command list for reference.</p>
                    <div class="flex gap-2">
                         <input type="text" id="command-input" class="flex-grow p-2 border border-border-color rounded-md focus:ring-2 focus:ring-accent-primary focus:outline-none" placeholder="e.g., CREATE file.txt">
                         <button id="run-button" class="bg-accent-primary text-white font-semibold px-4 py-2 rounded-md hover:bg-accent-primary-hover transition-colors">Run</button>
                    </div>
                    <div class="mt-4">
                        <h3 class="font-semibold text-md mb-2">Command Log</h3>
                        <div id="command-log" class="text-sm bg-stone-50 h-48 p-2 border border-border-color rounded-md overflow-y-auto font-mono">
                            Welcome! System ready.
                        </div>
                    </div>
                </div>

                 <div class="panel">
                    <h2 class="text-xl font-semibold mb-3">Command Reference</h2>
                    <ul class="space-y-1 text-sm text-text-secondary">
                        <li><strong class="text-text-primary">CREATE</strong> &lt;filename&gt;</li>
                        <li><strong class="text-text-primary">INSERT</strong> &lt;filename&gt; &lt;content&gt;</li>
                        <li><strong class="text-text-primary">SNAPSHOT</strong> &lt;filename&gt; &lt;msg&gt;</li>
                        <li><strong class="text-text-primary">ROLLBACK</strong> &lt;filename&gt; [versionID]</li>
                        <li><strong class="text-text-primary">HISTORY</strong> &lt;filename&gt;</li>
                        <li><strong class="text-text-primary">READ</strong> &lt;filename&gt;</li>
                        <li><strong class="text-text-primary">RECENT_FILES</strong> [num]</li>
                        <li><strong class="text-text-primary">BIGGEST_TREES</strong> [num]</li>
                    </ul>
                </div>
            </aside>
            
            <section class="lg:col-span-2 panel min-h-[500px]">
                <h2 class="text-xl font-semibold mb-3">Version History Tree</h2>
                <p id="tree-intro" class="text-sm text-text-secondary mb-4">This panel visualizes the version history of the selected file as a tree. Each node represents a version. Create a file to begin.</p>
                <div id="tree-container" class="tree-container mt-4 overflow-x-auto">
                    <p class="text-text-secondary">No file selected.</p>
                </div>
            </section>
            
            <aside class="lg:col-span-1 flex flex-col gap-6">
                <div class="panel">
                    <h2 class="text-xl font-semibold mb-3">File System State</h2>
                    <p class="text-sm text-text-secondary mb-4">This section shows a live overview of all files currently managed by the system.</p>
                    <h3 class="font-semibold text-md mb-2">All Files</h3>
                    <ul id="file-list" class="space-y-1 text-accent-primary cursor-pointer">
                       <li class="text-text-secondary cursor-default">No files created yet.</li>
                    </ul>
                </div>
                <div class="panel">
                    <h2 class="text-xl font-semibold mb-3">Active Content</h2>
                     <p class="text-sm text-text-secondary mb-4">Displays the content of the active version for the selected file.</p>
                    <pre id="file-content" class="bg-stone-50 h-32 p-2 border border-border-color rounded-md overflow-y-auto text-sm">No file selected.</pre>
                </div>
                 <div class="panel">
                    <h2 class="text-xl font-semibold mb-3">Analytics</h2>
                    <p class="text-sm text-text-secondary mb-4">These charts represent the system's analytical heaps, showing top files by recent activity and total versions.</p>
                    <div>
                        <h3 class="font-semibold text-md mb-2">Recently Modified Files</h3>
                        <div class="chart-container">
                            <canvas id="recent-files-chart"></canvas>
                        </div>
                    </div>
                    <div class="mt-6">
                        <h3 class="font-semibold text-md mb-2">Files with Most Versions</h3>
                        <div class="chart-container">
                            <canvas id="biggest-trees-chart"></canvas>
                        </div>
                    </div>
                </div>
            </aside>
        </div>
    </main>

    <script>
        document.addEventListener('DOMContentLoaded', () => {
            
            let fileSystem = {
                files: {},
            };
            let activeFile = null;
            let charts = {};

            const commandInput = document.getElementById('command-input');
            const runButton = document.getElementById('run-button');
            const commandLog = document.getElementById('command-log');
            const fileList = document.getElementById('file-list');
            const treeContainer = document.getElementById('tree-container');
            const fileContent = document.getElementById('file-content');
            const treeIntro = document.getElementById('tree-intro');


            function logMessage(message, type = 'info') {
                const p = document.createElement('p');
                if (type === 'command') {
                    p.innerHTML = `<span class="text-blue-500">&gt;</span> ${message}`;
                } else if (type === 'error') {
                    p.innerHTML = `<span class="text-red-500">Error:</span> ${message}`;
                } else {
                    p.textContent = message;
                }
                commandLog.appendChild(p);
                commandLog.scrollTop = commandLog.scrollHeight;
            }

            function getArgument(parts) {
                let content = parts.join(' ').trim();
                if (content.startsWith('"') && content.endsWith('"')) {
                    content = content.substring(1, content.length - 1);
                }
                return content;
            }


            const commands = {
                CREATE: (filename) => {
                    if (!filename) {
                        logMessage("Filename must be provided.", "error");
                        return;
                    }
                    if (fileSystem.files[filename]) {
                        logMessage(`File '${filename}' already exists.`, "error");
                        return;
                    }
                    const now = new Date().getTime();
                    const rootNode = {
                        versionId: 0,
                        content: "",
                        message: "Initial version",
                        timestamp: now,
                        isSnapshot: true,
                        parent: null,
                        children: []
                    };
                    fileSystem.files[filename] = {
                        root: rootNode,
                        activeVersion: rootNode,
                        versions: { 0: rootNode },
                        nextVersionId: 1,
                        lastChangeTime: now,
                    };
                    if (!activeFile) {
                       activeFile = filename;
                    }
                    logMessage(`File '${filename}' created with snapshot version 0.`);
                },
                INSERT: (filename, ...contentParts) => {
                    const file = fileSystem.files[filename];
                    if (!file) {
                        logMessage(`File '${filename}' not found.`, "error");
                        return;
                    }
                    const content = getArgument(contentParts);
                    const now = new Date().getTime();
                    if (file.activeVersion.isSnapshot) {
                        const newVersion = {
                            versionId: file.nextVersionId++,
                            content: file.activeVersion.content + content,
                            message: "",
                            timestamp: now,
                            isSnapshot: false,
                            parent: file.activeVersion,
                            children: []
                        };
                        file.activeVersion.children.push(newVersion);
                        file.versions[newVersion.versionId] = newVersion;
                        file.activeVersion = newVersion;
                        logMessage(`New version ${newVersion.versionId} created for '${filename}'.`);

                    } else {
                        file.activeVersion.content += content;
                         logMessage(`Content inserted into active version of '${filename}'.`);
                    }
                    file.lastChangeTime = now;
                },
                SNAPSHOT: (filename, ...messageParts) => {
                    const file = fileSystem.files[filename];
                    if (!file) {
                        logMessage(`File '${filename}' not found.`, "error");
                        return;
                    }
                    if (file.activeVersion.isSnapshot) {
                        logMessage(`Active version is already a snapshot.`, "error");
                        return;
                    }
                    const message = getArgument(messageParts);
                    file.activeVersion.isSnapshot = true;
                    file.activeVersion.message = message;
                    file.lastChangeTime = new Date().getTime();
                    logMessage(`Snapshot created for active version ${file.activeVersion.versionId} of '${filename}'.`);
                },
                ROLLBACK: (filename, versionIdStr) => {
                    const file = fileSystem.files[filename];
                    if (!file) {
                        logMessage(`File '${filename}' not found.`, "error");
                        return;
                    }
                    let targetVersion;
                    if (versionIdStr === undefined) {
                        if(file.activeVersion.parent){
                            targetVersion = file.activeVersion.parent;
                        } else {
                            logMessage(`Cannot ROLLBACK from root version.`, "error");
                            return;
                        }
                    } else {
                        const versionId = parseInt(versionIdStr, 10);
                        targetVersion = file.versions[versionId];
                        if (!targetVersion) {
                            logMessage(`Version ID ${versionId} not found for file '${filename}'.`, "error");
                            return;
                        }
                    }
                    file.activeVersion = targetVersion;
                    logMessage(`Active version for '${filename}' set to ${targetVersion.versionId}.`);
                },
                HISTORY: (filename) => {
                    const file = fileSystem.files[filename];
                    if (!file) {
                        logMessage(`File '${filename}' not found.`, "error");
                        return;
                    }
                    logMessage(`History for ${filename}:`);
                    let current = file.activeVersion;
                    let historyPath = new Set();
                    while (current) {
                        historyPath.add(current.versionId);
                        if (current.isSnapshot) {
                            const timeStr = new Date(current.timestamp).toLocaleString('en-IN');
                            logMessage(`Version ${current.versionId}: ${timeStr} - "${current.message}"`);
                        }
                        current = current.parent;
                    }
                    if (activeFile === filename) {
                        renderVersionTree(filename, historyPath);
                        setTimeout(() => renderVersionTree(filename), 3000); 
                    }
                },
                READ: (filename) => {
                    const file = fileSystem.files[filename];
                    if (!file) {
                        logMessage(`File '${filename}' not found.`, "error");
                        return;
                    }
                    logMessage(`Content of '${filename}' (v${file.activeVersion.versionId}):\n${file.activeVersion.content || '(empty)'}`);
                },
                RECENT_FILES: (numStr) => {
                    const num = numStr ? parseInt(numStr, 10) : Object.keys(fileSystem.files).length;
                    const sortedFiles = Object.entries(fileSystem.files)
                        .sort(([, a], [, b]) => b.lastChangeTime - a.lastChangeTime)
                        .slice(0, num);

                    logMessage(`Most Recently Modified Files:`);
                    sortedFiles.forEach(([name, file]) => {
                        logMessage(`- ${name} (Last modified: ${new Date(file.lastChangeTime).toLocaleTimeString('en-IN')})`);
                    });
                },
                 BIGGEST_TREES: (numStr) => {
                    const num = numStr ? parseInt(numStr, 10) : Object.keys(fileSystem.files).length;
                    const sortedFiles = Object.entries(fileSystem.files)
                        .sort(([, a], [, b]) => b.nextVersionId - a.nextVersionId)
                        .slice(0, num);
                    
                    logMessage(`Files with Most Versions:`);
                    sortedFiles.forEach(([name, file]) => {
                        logMessage(`- ${name} (${file.nextVersionId} versions)`);
                    });
                },
            };

            function runCommand() {
                const commandStr = commandInput.value.trim();
                if (!commandStr) return;

                logMessage(commandStr, 'command');
                commandInput.value = '';

                const parts = commandStr.split(/\s+/);
                const command = parts[0].toUpperCase();
                const args = parts.slice(1);

                if (commands[command]) {
                    commands[command](...args);
                    updateAllViews();
                } else {
                    logMessage(`Unknown command '${command}'.`, 'error');
                }
            }

            function updateAllViews() {
                renderFileList();
                if (activeFile) {
                    renderVersionTree(activeFile);
                    renderFileContent(activeFile);
                } else {
                     treeContainer.innerHTML = '<p class="text-text-secondary">No file selected.</p>';
                     fileContent.textContent = 'No file selected.';
                     treeIntro.textContent = 'This panel visualizes the version history of the selected file as a tree. Create a file to begin.';
                }
                renderAnalyticsCharts();
            }

            function renderFileList() {
                fileList.innerHTML = '';
                const filenames = Object.keys(fileSystem.files);
                if (filenames.length === 0) {
                    fileList.innerHTML = '<li class="text-text-secondary cursor-default">No files created yet.</li>';
                    return;
                }
                filenames.forEach(name => {
                    const li = document.createElement('li');
                    li.textContent = name;
                    li.dataset.filename = name;
                    if (name === activeFile) {
                        li.className = 'font-bold text-accent-primary';
                    }
                    fileList.appendChild(li);
                });
            }

            function renderVersionTree(filename, historyPath = null) {
                const file = fileSystem.files[filename];
                if (!file) return;

                treeContainer.innerHTML = '';
                const ul = document.createElement('ul');
                treeContainer.appendChild(ul);
                
                const buildTreeRecursive = (node, parentElement) => {
                    const li = document.createElement('li');
                    
                    let nodeClasses = 'inline-block px-2 py-1 text-sm rounded-md border ';
                    let nodeContent = `v${node.versionId}`;
                    if(node.isSnapshot) {
                        nodeClasses += 'bg-emerald-50 text-emerald-700 border-emerald-200 ';
                        nodeContent += ` "${node.message}"`;
                    } else {
                        nodeClasses += 'bg-stone-50 text-stone-700 border-stone-200 ';
                    }

                    if(node === file.activeVersion) {
                        nodeClasses += ' ring-2 ring-offset-2 ring-node-active';
                    }

                    if(historyPath && historyPath.has(node.versionId)) {
                        nodeClasses += ' bg-amber-100 border-amber-300';
                    }

                    li.innerHTML = `<div class="${nodeClasses}">${nodeContent}</div>`;
                    parentElement.appendChild(li);

                    if (node.children.length > 0) {
                        const childUl = document.createElement('ul');
                        li.appendChild(childUl);
                        node.children.forEach(child => buildTreeRecursive(child, childUl));
                    }
                };

                buildTreeRecursive(file.root, ul);
                 treeIntro.textContent = `Showing version history for '${filename}'. The highlighted node is the currently active version.`;
            }

            function renderFileContent(filename) {
                const file = fileSystem.files[filename];
                fileContent.textContent = file ? (file.activeVersion.content || '(empty)') : 'No file selected.';
            }

            function createChart(ctx, type, options = {}) {
                return new Chart(ctx, {
                    type: type,
                    data: {
                        labels: [],
                        datasets: [{
                            label: '',
                            data: [],
                            backgroundColor: 'rgba(74, 144, 226, 0.5)',
                            borderColor: 'rgba(74, 144, 226, 1)',
                            borderWidth: 1
                        }]
                    },
                    options: {
                        indexAxis: 'y',
                        responsive: true,
                        maintainAspectRatio: false,
                        plugins: {
                            legend: {
                                display: false
                            }
                        },
                        scales: {
                            x: {
                                beginAtZero: true
                            }
                        },
                        ...options
                    }
                });
            }

            function renderAnalyticsCharts() {
                const allFiles = Object.entries(fileSystem.files);

                const recentData = allFiles
                    .sort(([, a], [, b]) => b.lastChangeTime - a.lastChangeTime)
                    .slice(0, 5);

                const biggestData = allFiles
                    .sort(([, a], [, b]) => b.nextVersionId - a.nextVersionId)
                    .slice(0, 5);

                updateChart(charts.recent, 'Last Modified Time', recentData.map(([name]) => name), recentData.map(([, file]) => file.lastChangeTime));
                updateChart(charts.biggest, '# of Versions', biggestData.map(([name]) => name), biggestData.map(([, file]) => file.nextVersionId));
            }
            
            function updateChart(chart, label, labels, data) {
                if (!chart) return;
                
                if (label === 'Last Modified Time') {
                    const now = Date.now();
                     chart.data.datasets[0].data = data.map(d => now-d); // Show relative time
                     chart.options.plugins.tooltip.callbacks = {
                        label: function(context) {
                            return 'Modified: ' + new Date(data[context.dataIndex]).toLocaleString('en-IN');
                        }
                    }
                } else {
                    chart.data.datasets[0].data = data;
                }
                
                chart.data.labels = labels;
                chart.data.datasets[0].label = label;
                chart.update();
            }

            fileList.addEventListener('click', (e) => {
                if (e.target.tagName === 'LI' && e.target.dataset.filename) {
                    activeFile = e.target.dataset.filename;
                    updateAllViews();
                }
            });

            runButton.addEventListener('click', runCommand);
            commandInput.addEventListener('keypress', (e) => {
                if (e.key === 'Enter') {
                    runCommand();
                }
            });
            
            charts.recent = createChart(document.getElementById('recent-files-chart').getContext('2d'), 'bar');
            charts.biggest = createChart(document.getElementById('biggest-trees-chart').getContext('2d'), 'bar');
            updateAllViews();
        });
    </script>
</body>
</html>
