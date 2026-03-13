import sys
import os
import json
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                             QHBoxLayout, QPushButton, QLabel, QFileDialog, 
                             QListWidget, QMessageBox, QFrame, QAction)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QFont, QColor, QPalette

class CorewarWelcomeScreen(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.setWindowTitle("Corewar Visualisation")
        self.setMinimumSize(800, 600)
        
        self.champion_files = []
        self.saved_state = None
        
        self.escape_fullscreen_action = QAction("Exit fullscreen", self)
        self.escape_fullscreen_action.setShortcut("F")
        self.escape_fullscreen_action.triggered.connect(self.toggle_fullscreen)
        self.addAction(self.escape_fullscreen_action)
        
        self.init_ui()
        
    def init_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        main_layout = QVBoxLayout(central_widget)
        
        title_label = QLabel("COREWAR VISUALISATION")
        title_label.setAlignment(Qt.AlignCenter)
        title_font = QFont("Arial", 28, QFont.Bold)
        title_label.setFont(title_font)
        title_label.setStyleSheet("color: #D32F2F; margin: 15px;")
        main_layout.addWidget(title_label)
        
        subtitle_label = QLabel("Select your champions to start the battle")
        subtitle_label.setAlignment(Qt.AlignCenter)
        subtitle_font = QFont("Arial", 14)
        subtitle_label.setFont(subtitle_font)
        main_layout.addWidget(subtitle_label)
        
        separator = QFrame()
        separator.setFrameShape(QFrame.HLine)
        separator.setFrameShadow(QFrame.Sunken)
        main_layout.addWidget(separator)
        
        selection_layout = QHBoxLayout()
        
        buttons_layout = QVBoxLayout()
        
        select_files_btn = QPushButton("Select champions")
        select_files_btn.setMinimumHeight(40)
        select_files_btn.setStyleSheet("background-color: #3F51B5; color: white; border-radius: 4px;")
        select_files_btn.clicked.connect(self.select_champion_files)
        buttons_layout.addWidget(select_files_btn)
        
        select_folder_btn = QPushButton("Select a folder")
        select_folder_btn.setMinimumHeight(40)
        select_folder_btn.setStyleSheet("background-color: #3F51B5; color: white; border-radius: 4px;")
        select_folder_btn.clicked.connect(self.select_champion_folder)
        buttons_layout.addWidget(select_folder_btn)
        
        load_game_btn = QPushButton("Load a saved game")
        load_game_btn.setMinimumHeight(40)
        load_game_btn.setStyleSheet("background-color: #FF9800; color: white; border-radius: 4px;")
        load_game_btn.clicked.connect(self.load_saved_game)
        buttons_layout.addWidget(load_game_btn)
        
        clear_selection_btn = QPushButton("Clear selection")
        clear_selection_btn.setMinimumHeight(40)
        clear_selection_btn.setStyleSheet("background-color: #F44336; color: white; border-radius: 4px;")
        clear_selection_btn.clicked.connect(self.clear_selection)
        buttons_layout.addWidget(clear_selection_btn)
        
        buttons_layout.addStretch()
        
        selection_layout.addLayout(buttons_layout, 1)
        
        self.champions_list = QListWidget()
        self.champions_list.setMinimumWidth(400)
        selection_layout.addWidget(self.champions_list, 2)
        
        main_layout.addLayout(selection_layout)
        
        self.mode_label = QLabel("")
        self.mode_label.setAlignment(Qt.AlignCenter)
        self.mode_label.setFont(QFont("Arial", 12, QFont.Bold))
        self.mode_label.setStyleSheet("color: #FF9800;")
        main_layout.addWidget(self.mode_label)
        
        start_btn = QPushButton("START THE REAL BATTLE")
        start_btn.setMinimumHeight(50)
        start_font = QFont("Arial", 14, QFont.Bold)
        start_btn.setFont(start_font)
        start_btn.setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px;")
        start_btn.clicked.connect(self.start_real_battle)
        main_layout.addWidget(start_btn)
        
    def toggle_fullscreen(self):
        """Toggle between fullscreen and windowed mode"""
        if self.isFullScreen():
            self.showNormal()
        else:
            self.showFullScreen()
    
    def select_champion_files(self):
        """Select individual champion files"""
        files, _ = QFileDialog.getOpenFileNames(
            self, "Select champions", "", "Champion files (*.cor)"
        )
        
        if files:
            for file_path in files:
                if file_path not in self.champion_files:
                    self.champion_files.append(file_path)
                    self.champions_list.addItem(os.path.basename(file_path))
            
            if self.saved_state:
                self.saved_state = None
                self.mode_label.setText("")
    
    def select_champion_folder(self):
        """Select a folder containing champion files"""
        folder = QFileDialog.getExistingDirectory(
            self, "Select a folder containing champions"
        )
        
        if folder:
            for file_name in os.listdir(folder):
                if file_name.endswith(".cor"):
                    file_path = os.path.join(folder, file_name)
                    if file_path not in self.champion_files:
                        self.champion_files.append(file_path)
                        self.champions_list.addItem(os.path.basename(file_path))
            
            if self.saved_state:
                self.saved_state = None
                self.mode_label.setText("")
    
    def load_saved_game(self):
        """Load a saved game state"""
        file_path, _ = QFileDialog.getOpenFileName(
            self, "Load a saved game", "", "JSON files (*.json)"
        )
        
        if not file_path:
            return
        
        try:
            with open(file_path, 'r') as f:
                loaded_data = json.load(f)
            
            cycle = 0
            if "final_state" in loaded_data:
                self.saved_state = loaded_data
                cycle = loaded_data["final_state"].get('cycle', 0)
            elif "history" in loaded_data and isinstance(loaded_data["history"], list) and loaded_data["history"]:
                self.saved_state = loaded_data
                cycle = loaded_data["history"][-1].get('cycle', 0)
            else:
                self.saved_state = loaded_data
                cycle = loaded_data.get('cycle', 0)
            
            save_metadata = self.saved_state.get('_save_metadata', {})
            champion_files = save_metadata.get('champion_files', [])
            
            if not champion_files:
                QMessageBox.warning(
                    self, "Warning",
                    "Champion files were not found in the save.\n"
                    "Please manually select the champions used during the game."
                )
            else:
                missing_files = []
                for file_path in champion_files:
                    if not os.path.exists(file_path):
                        missing_files.append(file_path)
                
                if missing_files:
                    missing_list = "\n".join(missing_files)
                    QMessageBox.warning(
                        self, "Missing files",
                        f"Some champion files were not found:\n{missing_list}\n"
                        "Visualization will work, but you won't be able to start a new game."
                    )
                
                self.champion_files = champion_files
                self.champions_list.clear()
                for file_path in self.champion_files:
                    self.champions_list.addItem(os.path.basename(file_path))
                
                history_count = len(loaded_data.get("history", []))
                if history_count > 1:
                    self.mode_label.setText(f"ADVANCED VISUALIZATION MODE - Saved state loaded (cycle {cycle}, {history_count} states)")
                    QMessageBox.information(
                        self, "Game loaded",
                        f"Saved game loaded successfully (cycle {cycle}, {history_count} states).\n"
                        "Click 'START THE REAL BATTLE' to visualize the game progression."
                    )
                else:
                    self.mode_label.setText(f"VISUALIZATION MODE - Saved state loaded (cycle {cycle})")
                    QMessageBox.information(
                        self, "Game loaded",
                        f"Saved game loaded successfully (cycle {cycle}).\n"
                        "Click 'START THE REAL BATTLE' to visualize the state."
                    )
            
        except json.JSONDecodeError:
            QMessageBox.critical(
                self, "Error",
                "The selected file is not a valid JSON file."
            )
        except Exception as e:
            QMessageBox.critical(
                self, "Error",
                f"Unable to load saved game:\n{str(e)}"
            )
    
    def clear_selection(self):
        """Clear the selected champion list and the saved state"""
        self.champion_files.clear()
        self.champions_list.clear()
        self.saved_state = None
        self.mode_label.setText("")
    
    def start_real_battle(self):
        """Start the real battle with the Corewar program"""
        if not self.saved_state and len(self.champion_files) < 2:
            QMessageBox.warning(
                self, "Warning", "Please select at least 2 champions to start."
            )
            return

        if not self.saved_state:
            corewar_path = "./corewar"
            if not os.path.exists(corewar_path):
                QMessageBox.critical(
                    self, "Error",
                    "The corewar executable was not found.\n"
                    "Make sure the C program is compiled in the same folder."
                )
                return

        try:
            from visual import CorewarMainWindow
            self.main_window = CorewarMainWindow(self.champion_files, self.saved_state)
            self.main_window.showFullScreen()
            self.hide()
        except ImportError as e:
            QMessageBox.critical(
                self, "Error",
                f"The real visualization module is not available.\nError: {str(e)}"
            )

if __name__ == "__main__":
    app = QApplication(sys.argv)
    
    app.setStyle("Fusion")
    
    light_palette = QPalette()
    light_palette.setColor(QPalette.Window, QColor(240, 240, 245))
    light_palette.setColor(QPalette.WindowText, QColor(30, 30, 30))
    light_palette.setColor(QPalette.Base, QColor(255, 255, 255))
    light_palette.setColor(QPalette.AlternateBase, QColor(233, 233, 238))
    light_palette.setColor(QPalette.ToolTipBase, QColor(255, 255, 220))
    light_palette.setColor(QPalette.ToolTipText, QColor(0, 0, 0))
    light_palette.setColor(QPalette.Text, QColor(30, 30, 30))
    light_palette.setColor(QPalette.Button, QColor(220, 220, 225))
    light_palette.setColor(QPalette.ButtonText, QColor(30, 30, 30))
    light_palette.setColor(QPalette.BrightText, QColor(220, 0, 0))
    light_palette.setColor(QPalette.Link, QColor(0, 100, 200))
    light_palette.setColor(QPalette.Highlight, QColor(70, 130, 210))
    light_palette.setColor(QPalette.HighlightedText, QColor(255, 255, 255))
    app.setPalette(light_palette)
    
    window = CorewarWelcomeScreen()
    window.showFullScreen()
    
    sys.exit(app.exec_())
