import subprocess
import json
import datetime
from PyQt5.QtWidgets import (QMainWindow, QWidget, QVBoxLayout, 
                             QHBoxLayout, QPushButton, QLabel, QFileDialog, 
                             QMessageBox, QFrame, QSplitter,
                             QScrollArea, QStatusBar, QSpinBox, QSlider)
from PyQt5.QtCore import Qt, QSize, QTimer, QRect, pyqtSignal, QThread
from PyQt5.QtGui import QFont, QColor, QPainter, QPen, QBrush

MEM_SIZE = 6144
MEMORY_COLUMNS = 100
DEFAULT_HISTORY_INTERVAL = 200
MAX_HISTORY_STATES = 1000

class CorewarRunner(QThread):
    """Thread to run the Corewar program and read JSON output"""
    
    state_received = pyqtSignal(dict)
    finished_signal = pyqtSignal(dict)
    error_signal = pyqtSignal(str)
    
    def __init__(self, champion_files, corewar_executable="./corewar"):
        super().__init__()
        self.champion_files = champion_files
        self.corewar_executable = corewar_executable
        self.process = None
        self.running = False
        self.all_states = []
        
    def run(self):
        cmd = [self.corewar_executable, "--json"] + self.champion_files
        
        try:
            self.process = subprocess.Popen(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1
            )
            
            self.running = True
            
            buffer = ""
            brace_count = 0
            line_count = 0
            
            for line in iter(self.process.stdout.readline, ''):
                if not self.running:
                    break
                    
                line_count += 1
                if line_count <= 5:
                    print(f"Line {line_count}: {repr(line)}")
                
                buffer += line
                
                for char in line:
                    if char == '{':
                        brace_count += 1
                    elif char == '}':
                        brace_count -= 1
                        
                        if brace_count == 0 and buffer.strip():
                            try:
                                json_str = buffer.strip()
                                if json_str:
                                    state = json.loads(json_str)
                                    
                                    state_copy = json.loads(json_str)
                                    self.all_states.append(state_copy)
                                    
                                    self.state_received.emit(state)
                                    self.last_state = state
                                buffer = ""
                            except json.JSONDecodeError as e:
                                print(f"JSON error: {e}")
                                print(f"Buffer content: {repr(buffer)}")
                                buffer = ""
                            except Exception as e:
                                print(f"Unexpected error: {e}")
                                buffer = ""
            
            stderr_output = self.process.stderr.read()
            if stderr_output:
                print(f"Stderr: {stderr_output}")
                self.error_signal.emit(stderr_output)
            
            self.process.wait()
            if hasattr(self, 'last_state'):
                if hasattr(self, 'all_states'):
                    self.last_state['_all_states'] = self.all_states
                self.finished_signal.emit(self.last_state)
            else:
                self.error_signal.emit("No state received from the Corewar program")
                
        except Exception as e:
            error_msg = f"Error while running Corewar: {e}"
            print(error_msg)
            self.error_signal.emit(error_msg)
    
    def stop(self):
        self.running = False
        if self.process:
            self.process.terminate()
            self.process.wait()

class MemoryDisplayWidget(QWidget):
    """Widget to display memory as a grid of colored squares"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        self.setMinimumSize(100, 62)
        
        self.champion_colors = {}
        self.memory = [0] * MEM_SIZE
        self.memory_owners = [None] * MEM_SIZE
        self.process_positions = []
        self.process_colors = []
        
    def sizeHint(self):
        """Return the suggested size for the widget"""
        return QSize(MEMORY_COLUMNS * 12, (MEM_SIZE // MEMORY_COLUMNS) * 12 + 20)
        
    def set_champion_colors(self, champion_colors):
        """Set the colors associated with each champion"""
        self.champion_colors = champion_colors
        self.update()
        
    def update_memory(self, memory_data, champions_data):
        """Update the memory state from JSON data"""
        self.memory = [0] * MEM_SIZE
        self.memory_owners = [None] * MEM_SIZE
        
        for addr_str, value in memory_data.items():
            addr = int(addr_str)
            if 0 <= addr < MEM_SIZE:
                self.memory[addr] = value
                closest_champion = None
                min_distance = MEM_SIZE
                
                for champion in champions_data:
                    load_addr = champion.get('load_address', 0)
                    distance = min(abs(addr - load_addr), MEM_SIZE - abs(addr - load_addr))
                    if distance < min_distance:
                        min_distance = distance
                        closest_champion = champion['number']
                
                self.memory_owners[addr] = closest_champion
        
        self.update()
        
    def update_processes(self, processes_data):
        """Update process positions"""
        self.process_positions = []
        self.process_colors = []
        
        for process in processes_data:
            if process.get('alive', False):
                self.process_positions.append(process['pc'])
                champion_num = process['champion_number']
                color = self.champion_colors.get(champion_num, QColor(255, 255, 255))
                enhanced_color = QColor(
                    min(255, color.red() + 100),
                    min(255, color.green() + 100),
                    min(255, color.blue() + 100)
                )
                self.process_colors.append(enhanced_color)
        
        self.update()
        
    def get_cell_rect(self, index):
        """Get the rectangle coordinates for a memory cell"""
        row = index // MEMORY_COLUMNS
        col = index % MEMORY_COLUMNS
        
        cell_size = 10
        margin = 2
        
        x = col * (cell_size + margin)
        y = row * (cell_size + margin)
        
        return QRect(x, y, cell_size, cell_size)
        
    def paintEvent(self, event):
        """Draw the memory grid"""
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        
        for i in range(MEM_SIZE):
            rect = self.get_cell_rect(i)
            
            if self.memory_owners[i] is not None:
                base_color = self.champion_colors.get(self.memory_owners[i], QColor(200, 200, 200))
                
                intensity = 0.3 + (self.memory[i] / 255.0) * 0.7
                
                r, g, b = base_color.red(), base_color.green(), base_color.blue()
                adjusted_color = QColor(
                    min(255, int(r * intensity)),
                    min(255, int(g * intensity)),
                    min(255, int(b * intensity))
                )
                
                brush = QBrush(adjusted_color)
            else:
                brush = QBrush(QColor(240, 240, 240))
            
            painter.setBrush(brush)
            painter.setPen(QPen(Qt.lightGray, 1))
            painter.drawRect(rect)
            
        for i, pos in enumerate(self.process_positions):
            if 0 <= pos < MEM_SIZE:
                rect = self.get_cell_rect(pos)
                
                if i < len(self.process_colors):
                    color = self.process_colors[i]
                else:
                    color = QColor(255, 0, 0)
                
                painter.setPen(QPen(color, 3))
                painter.setBrush(Qt.NoBrush)
                
                painter.drawEllipse(rect.center(), 3, 3)

class ChampionStatusWidget(QWidget):
    """Widget to display champion status"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        self.layout = QVBoxLayout(self)
        self.layout.setSpacing(10)
        
        title_label = QLabel("Champion status")
        title_label.setFont(QFont("Arial", 14, QFont.Bold))
        title_label.setAlignment(Qt.AlignCenter)
        self.layout.addWidget(title_label)
        
        self.status_frame = QFrame()
        self.status_frame.setFrameShape(QFrame.StyledPanel)
        self.status_frame.setFrameShadow(QFrame.Raised)
        self.status_layout = QVBoxLayout(self.status_frame)
        self.layout.addWidget(self.status_frame)
        
        self.layout.addStretch()
        
        self.champion_labels = {}
        
    def update_champions(self, champions_data, processes_data):
        """Update the champion display from JSON data"""
        process_count = {}
        for process in processes_data:
            if process.get('alive', False):
                champ_num = process['champion_number']
                process_count[champ_num] = process_count.get(champ_num, 0) + 1
        
        for champion in champions_data:
            champ_id = champion['number']
            
            if champ_id not in self.champion_labels:
                champ_frame = QFrame()
                champ_frame.setFrameShape(QFrame.StyledPanel)
                champ_layout = QVBoxLayout(champ_frame)
                
                name_label = QLabel(f"Champion #{champ_id}: {champion['name']}")
                name_label.setFont(QFont("Arial", 11, QFont.Bold))
                champ_layout.addWidget(name_label)
                
                status_label = QLabel()
                champ_layout.addWidget(status_label)
                
                live_label = QLabel()
                champ_layout.addWidget(live_label)
                
                process_label = QLabel()
                champ_layout.addWidget(process_label)
                
                self.champion_labels[champ_id] = {
                    'status': status_label,
                    'live': live_label,
                    'process': process_label,
                    'frame': champ_frame
                }
                
                self.status_layout.addWidget(champ_frame)
            
            labels = self.champion_labels[champ_id]
            
            is_alive = champion.get('alive', False)
            status_text = "Status: Alive" if is_alive else "Status: Dead"
            labels['status'].setText(status_text)
            
            last_live = champion.get('last_live', -1)
            live_text = f"Last live: cycle {last_live}" if last_live >= 0 else "Last live: -"
            labels['live'].setText(live_text)
            
            proc_count = process_count.get(champ_id, 0)
            labels['process'].setText(f"Processes: {proc_count}")
            
            if is_alive:
                labels['frame'].setStyleSheet("background-color: rgba(200, 255, 200, 100);")
            else:
                labels['frame'].setStyleSheet("background-color: rgba(255, 200, 200, 100);")

class CorewarMainWindow(QMainWindow):
    """Main window for Corewar visualization"""
    
    def __init__(self, champion_files, saved_state=None):
        super().__init__()
        
        self.setWindowTitle("Corewar visualisation - Real battle")
        self.setMinimumSize(1200, 800)
        
        self.champion_files = champion_files
        self.current_state = None
        self.saved_state = saved_state
        self.corewar_runner = None
        self.is_paused = False
        self.update_speed = 50
        self.replay_mode = saved_state is not None
        self.animation_active = False
        self.game_finished = False
        self.winner_announced = False
        
        self.state_history = []
        self.current_history_index = 0
        self.last_saved_cycle = -1
        self.history_interval = DEFAULT_HISTORY_INTERVAL
        
        if self.replay_mode and saved_state:
            if "cycle" in saved_state and "champions" in saved_state:
                self.current_state = saved_state
                self.state_history = [saved_state]
            elif "history" in saved_state and isinstance(saved_state["history"], list):
                self.state_history = saved_state["history"]
                if len(self.state_history) == 0 and "final_state" in saved_state:
                    self.state_history = [saved_state["final_state"]]
                if len(self.state_history) > 0:
                    self.current_state = self.state_history[0]
                    metadata = saved_state.get("_save_metadata", {})
                    self.history_interval = metadata.get("history_interval", DEFAULT_HISTORY_INTERVAL)
            elif "final_state" in saved_state:
                self.state_history = [saved_state["final_state"]]
                self.current_state = saved_state["final_state"]
                
            if "complete_history" in saved_state and isinstance(saved_state["complete_history"], list) and len(saved_state["complete_history"]) > 0:
                print(f"Loading complete history with {len(saved_state['complete_history'])} states")
                self.state_history = saved_state["complete_history"]
                self.current_state = self.state_history[0]
        
        self.champion_colors = self.generate_champion_colors(len(champion_files))
        
        self.init_ui()
        
        self.display_timer = QTimer(self)
        self.display_timer.timeout.connect(self.update_display)
        
        if saved_state is not None:
            self.update_display()
            
            if self.current_state:
                cycle = self.current_state.get('cycle', 0)
                total_cycles = len(self.state_history)
                self.status_bar.showMessage(f"Viewing saved state (cycle {cycle}, {total_cycles} states total)")
            
            self.start_button.setText("Start visualization")
            self.start_button.setEnabled(True)
            
            self.export_button.setEnabled(True)
        
    def init_ui(self):
        """Initialize the user interface"""
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        
        main_layout = QVBoxLayout(central_widget)

        main_splitter = QSplitter(Qt.Horizontal)
        
        memory_scroll = QScrollArea()
        memory_scroll.setWidgetResizable(True)
        memory_scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        memory_scroll.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        
        self.memory_display = MemoryDisplayWidget()
        self.memory_display.set_champion_colors({i+1: QColor(*color) for i, color in enumerate([(255, 0, 0), (0, 0, 255), (0, 255, 0), (255, 255, 0)])})
        
        memory_scroll.setWidget(self.memory_display)
        main_splitter.addWidget(memory_scroll)
        
        right_panel = QWidget()
        right_layout = QVBoxLayout(right_panel)
        
        control_box = QFrame()
        control_box.setFrameShape(QFrame.StyledPanel)
        control_box.setFrameShadow(QFrame.Raised)
        control_box_layout = QVBoxLayout(control_box)
        
        control_title = QLabel("Controls")
        control_title.setFont(QFont("Arial", 14, QFont.Bold))
        control_title.setAlignment(Qt.AlignCenter)
        control_box_layout.addWidget(control_title)
        
        speed_layout = QHBoxLayout()
        speed_label = QLabel("Speed:")
        speed_layout.addWidget(speed_label)
        
        self.speed_slider = QSlider(Qt.Horizontal)
        self.speed_slider.setRange(1, 100)
        self.speed_slider.setValue(20)
        self.speed_slider.setTickPosition(QSlider.TicksBelow)
        self.speed_slider.setTickInterval(10)
        self.speed_slider.valueChanged.connect(self.update_timer_speed)
        speed_layout.addWidget(self.speed_slider)
        
        self.speed_spinbox = QSpinBox()
        self.speed_spinbox.setRange(1, 100)
        self.speed_spinbox.setValue(20)
        self.speed_spinbox.valueChanged.connect(self.speed_slider.setValue)
        self.speed_slider.valueChanged.connect(self.speed_spinbox.setValue)
        speed_layout.addWidget(self.speed_spinbox)
        
        fps_label = QLabel("FPS")
        speed_layout.addWidget(fps_label)
        
        control_box_layout.addLayout(speed_layout)
        
        self.pause_button = QPushButton("Pause")
        self.pause_button.setCheckable(True)
        self.pause_button.setMinimumHeight(40)
        self.pause_button.clicked.connect(self.toggle_pause)
        self.pause_button.setEnabled(False)
        control_box_layout.addWidget(self.pause_button)
        
        if self.replay_mode:
            replay_controls = QHBoxLayout()
            
            self.rewind_button = QPushButton("⏮ Start")
            self.rewind_button.clicked.connect(self.rewind_to_start)
            replay_controls.addWidget(self.rewind_button)

            self.prev_button = QPushButton("⏪ Previous")
            self.prev_button.clicked.connect(self.previous_state)
            replay_controls.addWidget(self.prev_button)

            self.next_button = QPushButton("Next ⏩")
            self.next_button.clicked.connect(self.next_state)
            replay_controls.addWidget(self.next_button)

            self.forward_button = QPushButton("End ⏭")
            self.forward_button.clicked.connect(self.forward_to_end)
            replay_controls.addWidget(self.forward_button)
            
            control_box_layout.addLayout(replay_controls)
            
            history_layout = QHBoxLayout()
            history_label = QLabel("Position:")
            history_layout.addWidget(history_label)
            
            self.history_slider = QSlider(Qt.Horizontal)
            self.history_slider.setRange(0, max(0, len(self.state_history) - 1))
            self.history_slider.setValue(0)
            self.history_slider.valueChanged.connect(self.set_history_position)
            history_layout.addWidget(self.history_slider)
            
            self.position_label = QLabel("0/0")
            history_layout.addWidget(self.position_label)
            
            control_box_layout.addLayout(history_layout)
        
        right_layout.addWidget(control_box)
        
        self.champion_status_widget = ChampionStatusWidget()
        right_layout.addWidget(self.champion_status_widget)
        
        self.winner_label = QLabel("")
        self.winner_label.setAlignment(Qt.AlignCenter)
        self.winner_label.setFont(QFont("Arial", 14, QFont.Bold))
        self.winner_label.setStyleSheet("color: #009933; background-color: #EEFFEE; padding: 10px; border-radius: 5px; border: 1px solid #66CC66;")
        self.winner_label.setVisible(False)
        right_layout.addWidget(self.winner_label)
        
        self.export_button = QPushButton("Export game")
        self.export_button.setMinimumHeight(40)
        self.export_button.setStyleSheet("background-color: #FF9800; color: white; border-radius: 4px;")
        self.export_button.clicked.connect(self.export_game_state)
        self.export_button.setEnabled(False)
        right_layout.addWidget(self.export_button)
        
        main_splitter.addWidget(right_panel)
        
        main_splitter.setSizes([700, 300])
        
        main_layout.addWidget(main_splitter)
        
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)
        self.status_bar.showMessage("Ready to start")
        
        control_frame = QFrame()
        control_layout = QHBoxLayout(control_frame)
        
        self.start_button = QPushButton("Start battle")
        self.start_button.clicked.connect(self.start_corewar)
        control_layout.addWidget(self.start_button)
        
        self.stop_button = QPushButton("Stop")
        self.stop_button.clicked.connect(self.stop_corewar)
        self.stop_button.setEnabled(False)
        control_layout.addWidget(self.stop_button)
        
        control_layout.addStretch()
        
        main_layout.addWidget(control_frame)
        
        if self.replay_mode and hasattr(self, 'history_slider'):
            max_val = max(0, len(self.state_history) - 1)
            self.history_slider.setRange(0, max_val)
            self.position_label.setText(f"0/{max_val}")
    
    def rewind_to_start(self):
        """Go back to the beginning of the history"""
        if len(self.state_history) > 0:
            self.current_history_index = 0
            self.current_state = self.state_history[0]
            if hasattr(self, 'history_slider'):
                self.history_slider.setValue(0)
            self.update_display(force=True)
            
            self.winner_label.setVisible(False)
            self.winner_announced = False
    
    def previous_state(self):
        """Go to the previous state in the history"""
        if self.current_history_index > 0:
            self.current_history_index -= 1
            self.current_state = self.state_history[self.current_history_index]
            if hasattr(self, 'history_slider'):
                self.history_slider.setValue(self.current_history_index)
            self.update_display(force=True)
            
            if self.current_history_index < len(self.state_history) - 1:
                self.winner_label.setVisible(False)
                self.winner_announced = False
    
    def next_state(self):
        """Go to the next state in the history"""
        if self.current_history_index < len(self.state_history) - 1:
            self.current_history_index += 1
            self.current_state = self.state_history[self.current_history_index]
            if hasattr(self, 'history_slider'):
                self.history_slider.setValue(self.current_history_index)
            self.update_display(force=True)
            
            if self.current_history_index == len(self.state_history) - 1:
                self.check_winner()
    
    def forward_to_end(self):
        """Advance to the end of the history"""
        if len(self.state_history) > 0:
            self.current_history_index = len(self.state_history) - 1
            self.current_state = self.state_history[self.current_history_index]
            if hasattr(self, 'history_slider'):
                self.history_slider.setValue(self.current_history_index)
            self.update_display(force=True)
            
            self.check_winner()
    
    def check_winner(self):
        """Display the winner when reaching the end"""
        if not self.winner_announced and self.current_history_index == len(self.state_history) - 1:
            last_state = self.state_history[-1]
            winner_num = last_state.get('last_alive_champion', 0)
            
            if winner_num > 0:
                winner_name = "Unknown"
                for champion in last_state.get('champions', []):
                    if champion.get('number') == winner_num:
                        winner_name = champion.get('name', 'Unknown')
                        break
                
                self.winner_label.setText(f"🏆 Champion #{winner_num} ({winner_name}) won! 🏆")
                self.winner_label.setVisible(True)
                self.winner_announced = True
                
                self.toggle_pause(True)
                self.pause_button.setChecked(True)
    
    def set_history_position(self, position):
        """Set the position in the history from the slider"""
        if 0 <= position < len(self.state_history):
            self.current_history_index = position
            self.current_state = self.state_history[position]
            self.update_display(force=True)
            
            self.position_label.setText(f"{position}/{max(0, len(self.state_history) - 1)}")
            
            if position == len(self.state_history) - 1:
                self.check_winner()
            else:
                self.winner_label.setVisible(False)
                self.winner_announced = False
    
    def export_game_state(self):
        """Export the current game state to a JSON file"""
        if not self.state_history and not self.current_state:
            QMessageBox.warning(self, "Warning", "No game state to export.")
            return
        
        timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        default_filename = f"corewar_save_{timestamp}.json"
        
        file_path, _ = QFileDialog.getSaveFileName(
            self, "Export game state",
            default_filename,
            "JSON files (*.json)"
        )
        
        if not file_path:
            return
        
        try:
            all_states = []
            complete_history = []
            
            if hasattr(self, 'corewar_runner') and self.corewar_runner and hasattr(self.corewar_runner, 'all_states'):
                all_states = self.corewar_runner.all_states
                print(f"Using all_states from runner: {len(all_states)} states")
                
                if len(all_states) > 500:
                    retained_end = min(100, len(all_states))
                    sample_interval = 5
                    
                    for i in range(0, len(all_states) - retained_end, sample_interval):
                        complete_history.append(all_states[i])
                    
                    for i in range(len(all_states) - retained_end, len(all_states)):
                        complete_history.append(all_states[i])
                else:
                    complete_history = all_states
                    
                print(f"Kept {len(complete_history)} states for export")
            
            elif self.current_state and '_all_states' in self.current_state:
                all_states = self.current_state['_all_states']
                current_state_copy = self.current_state.copy()
                del current_state_copy['_all_states']
                
                if len(all_states) > 500:
                    retained_end = min(100, len(all_states))
                    sample_interval = 5
                    
                    for i in range(0, len(all_states) - retained_end, sample_interval):
                        complete_history.append(all_states[i])
                    
                    for i in range(len(all_states) - retained_end, len(all_states)):
                        complete_history.append(all_states[i])
                else:
                    complete_history = all_states
            
            save_data = {
                "final_state": self.current_state if self.current_state else (self.state_history[-1] if self.state_history else {}),
                "history": self.state_history,
                "complete_history": complete_history if complete_history else self.state_history,
                "_save_metadata": {
                    "timestamp": timestamp,
                    "champion_files": self.champion_files,
                    "is_complete_save": True,
                    "history_interval": self.history_interval,
                    "replay_version": "1.1",
                    "complete_game": len(complete_history) > 0
                }
            }
            
            with open(file_path, 'w') as f:
                json.dump(save_data, f, indent=2)
            
            self.status_bar.showMessage(f"Game state saved to {file_path}")
            QMessageBox.information(
                self, "Save successful",
                f"Game state saved to:\n{file_path}"
            )
        except Exception as e:
            QMessageBox.critical(
                self, "Save error",
                f"Unable to save game state:\n{str(e)}"
            )
    
    def update_timer_speed(self, value):
        """Update the timer speed"""
        if value <= 0:
            value = 1
        self.update_speed = 1000 // value
        
        if self.display_timer.isActive() and not self.is_paused:
            self.display_timer.stop()
            self.display_timer.start(self.update_speed)
    
    def toggle_pause(self, checked):
        """Toggle between pause and play"""
        self.is_paused = checked

        if checked:
            self.pause_button.setText("Resume")
            if self.display_timer.isActive():
                self.display_timer.stop()
        else:
            self.pause_button.setText("Pause")
            if not self.display_timer.isActive() and self.animation_active:
                self.display_timer.start(self.update_speed)
    
    def generate_champion_colors(self, num_champions):
        """Generate distinct colors for each champion"""
        colors = [
            (255, 0, 0),    # Red
            (0, 0, 255),    # Blue
            (0, 255, 0),    # Green
            (255, 255, 0),  # Yellow
            (255, 0, 255),  # Magenta
            (0, 255, 255),  # Cyan
            (255, 128, 0),  # Orange
            (128, 0, 255)   # Purple
        ]
        return colors[:num_champions]
    
    def start_corewar(self):
        """Start the Corewar execution or visualization"""
        if self.replay_mode:
            self.animation_active = True
            self.pause_button.setEnabled(True)
            self.pause_button.setChecked(False)
            self.is_paused = False
            self.stop_button.setEnabled(True)
            self.start_button.setEnabled(False)
            self.display_timer.start(self.update_speed)
            
            if self.current_state:
                cycle = self.current_state.get('cycle', 0)
                self.status_bar.showMessage(f"Visualization in progress (cycle {cycle})...")
        else:
            self.corewar_runner = CorewarRunner(self.champion_files)
            self.corewar_runner.state_received.connect(self.handle_state_update)
            self.corewar_runner.finished_signal.connect(self.handle_corewar_finished)
            self.corewar_runner.error_signal.connect(self.handle_corewar_error)
            
            self.corewar_runner.start()
            
            self.animation_active = True
            self.start_button.setEnabled(False)
            self.stop_button.setEnabled(True)
            self.pause_button.setEnabled(True)
            self.pause_button.setChecked(False)
            self.is_paused = False
            self.export_button.setEnabled(True)
            self.status_bar.showMessage("Battle in progress...")
            
            self.display_timer.start(self.update_speed)
    
    def stop_corewar(self):
        """Stop the Corewar execution or visualization"""
        if self.corewar_runner:
            self.corewar_runner.stop()
            
        self.display_timer.stop()
        self.animation_active = False
        
        if self.replay_mode:
            self.start_button.setText("Start visualization")
        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)
        self.pause_button.setEnabled(False)

        if self.replay_mode:
            self.status_bar.showMessage("Visualization stopped")
        else:
            self.status_bar.showMessage("Battle stopped")
    
    def handle_state_update(self, state):
        """Handle a Corewar state update"""
        self.current_state = state
        
        if not self.export_button.isEnabled():
            self.export_button.setEnabled(True)
            
        current_cycle = state.get('cycle', 0)
        
        early_dense_sampling = current_cycle < 2000
        
        cycle_to_die = state.get('cycle_to_die', 1536)
        close_to_cycle_to_die = (current_cycle % cycle_to_die) > (cycle_to_die * 0.8)
        
        save_this_state = early_dense_sampling or close_to_cycle_to_die
        
        if (self.last_saved_cycle == -1 or 
            current_cycle >= self.last_saved_cycle + self.history_interval or
            save_this_state):
            
            state_copy = json.loads(json.dumps(state))
            self.state_history.append(state_copy)
            self.last_saved_cycle = current_cycle
            
            if len(self.state_history) > MAX_HISTORY_STATES and not save_this_state:
                self.history_interval *= 1.5
                
                new_history = [self.state_history[0]]
                
                retain_count = max(100, int(len(self.state_history) * 0.2))
                retain_from = max(1, len(self.state_history) - retain_count)
                
                middle_states = self.state_history[1:retain_from]
                if len(middle_states) > 100:
                    sample_interval = max(2, len(middle_states) // 100)
                    for i in range(0, len(middle_states), sample_interval):
                        new_history.append(middle_states[i])
                else:
                    new_history.extend(middle_states)
                    
                new_history.extend(self.state_history[retain_from:])
                
                self.state_history = new_history
                print(f"Reduced history from {len(self.state_history)} to {len(new_history)} states, new interval: {self.history_interval}")
    
    def handle_corewar_error(self, error_message):
        """Handle Corewar errors"""
        self.display_timer.stop()
        self.animation_active = False

        QMessageBox.critical(
            self, "Corewar Error",
            f"Error during execution:\n{error_message}"
        )

        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)
        self.pause_button.setEnabled(False)
        self.status_bar.showMessage(f"Error: {error_message}")
    
    def handle_corewar_finished(self, final_state):
        """Handle the end of Corewar execution"""
        self.display_timer.stop()
        self.animation_active = False
        self.game_finished = True
        
        winner = final_state.get('last_alive_champion', 0)
        winner_name = "Unknown"

        for champion in final_state.get('champions', []):
            if champion.get('number') == winner:
                winner_name = champion.get('name', "Unknown")
                break
        
        if winner > 0:
            self.winner_label.setText(f"🏆 Champion #{winner} ({winner_name}) won! 🏆")
            self.winner_label.setVisible(True)

            QMessageBox.information(
                self, "Battle finished",
                f"Champion #{winner} ({winner_name}) won the battle!"
            )
        else:
            QMessageBox.information(
                self, "Battle finished",
                "The battle is over."
            )
        
        self.current_state = final_state
        
        if (not self.state_history or 
            self.state_history[-1].get('cycle', -1) != final_state.get('cycle', -2)):
            self.state_history.append(final_state)
        
        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)
        self.pause_button.setEnabled(False)
        self.status_bar.showMessage("Battle finished")

        response = QMessageBox.question(
            self, "Export game?",
            "Would you like to export this game to replay it later?",
            QMessageBox.Yes | QMessageBox.No,
            QMessageBox.Yes
        )
        
        if response == QMessageBox.Yes:
            self.export_game_state()
    
    def update_display(self, force=False):
        """Update the display with the current state"""
        if (not self.current_state or self.is_paused) and not force:
            return
        
        if self.replay_mode and not self.is_paused and not force:
            if self.current_history_index < len(self.state_history) - 1:
                self.current_history_index += 1
                self.current_state = self.state_history[self.current_history_index]
                
                if hasattr(self, 'history_slider'):
                    self.history_slider.setValue(self.current_history_index)
                    self.position_label.setText(f"{self.current_history_index}/{max(0, len(self.state_history) - 1)}")
                
                if self.current_history_index == len(self.state_history) - 1:
                    self.check_winner()
            else:
                if not self.winner_announced:
                    self.check_winner()
                
                self.toggle_pause(True)
                self.pause_button.setChecked(True)
        
        memory_data = self.current_state.get('memory', {})
        champions_data = self.current_state.get('champions', [])
        processes_data = self.current_state.get('processes', [])
        
        champion_colors = {}
        for i, champion in enumerate(champions_data):
            if i < len(self.champion_colors):
                champion_colors[champion['number']] = QColor(*self.champion_colors[i])
        
        self.memory_display.set_champion_colors(champion_colors)
        self.memory_display.update_memory(memory_data, champions_data)
        self.memory_display.update_processes(processes_data)
        
        self.champion_status_widget.update_champions(champions_data, processes_data)
        
        cycle = self.current_state.get('cycle', 0)
        cycle_to_die = self.current_state.get('cycle_to_die', 0)
        alive_champions = sum(1 for c in champions_data if c.get('alive', False))
        total_processes = len([p for p in processes_data if p.get('alive', False)])
        
        if self.replay_mode:
            history_info = f"State {self.current_history_index + 1}/{len(self.state_history)}" if self.state_history else ""
            self.status_bar.showMessage(
                f"VISUALIZATION - Cycle: {cycle} | Cycle to die: {cycle_to_die} | "
                f"Alive champions: {alive_champions}/{len(champions_data)} | "
                f"Processes: {total_processes} | {history_info}"
            )
        else:
            self.status_bar.showMessage(
                f"Cycle: {cycle} | Cycle to die: {cycle_to_die} | "
                f"Alive champions: {alive_champions}/{len(champions_data)} | "
                f"Processes: {total_processes}"
            )
