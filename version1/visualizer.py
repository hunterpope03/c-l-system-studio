import math # import math for trigonometry and angles.
import sys # import sys to create an QApplication class.
from PyQt5.QtWidgets import QApplication, QGraphicsView, QGraphicsScene, QMainWindow # import PyQt graphic libraries for creating the GUI.
from PyQt5.QtGui import QPen, QColor, QBrush, QPainter # import PyQt drawing libraries for drawing the system.
from PyQt5.QtCore import Qt, QTimer # import PyQt core libraries for running the animation.

class LSystemVisualizer(QMainWindow): 
    def __init__(self, parsed_system, turn_angle, starting_direction) -> None:
        """
        Initializes a new LSystemVisualizer object.
        Creates application instance, scene, view, and drawing tools.

        Parameters:
        drawing_color (str): The color of the drawing.
        background_color (str): The color of the background of the visualization.
        parsed_system (str): The fully parsed L-System.
        turn_angle (float): The angle at which to turn left or right.
        starting_direction (float): The starting direction of the visualization's drawing.
        """
        if not QApplication.instance(): # create a new QApplication instance if there is not already one.
            self.app = QApplication(sys.argv)
            self.owns_app = True
        else: # if an application instance already exists, use it.
            self.app = QApplication.instance()
            self.owns_app = False

        super().__init__() # call the parent class constructor.

        # clean up the previous instance.
        if hasattr(self, 'timer') and self.timer is not None:
            self.timer.stop()
        if hasattr(self, 'scene') and self.scene is not None:
            self.scene.clear()
        if hasattr(self, 'view') and self.view is not None:
            self.view.setScene(None)

        self.plot_color = QColor('red')
        self.plot_background = QColor('black')
        self.parsed = parsed_system
        self.turn_angle = turn_angle
        self.starting_direction = starting_direction
        self.path_items = [] # declare initial variables.

        self.boundaries = self.set_boundaries()
        self.min_x = self.boundaries[0]
        self.max_x = self.boundaries[1]
        self.min_y = self.boundaries[2]
        self.max_y = self.boundaries[3] # get the boundaries of the L-System drawing.

        self.scene = QGraphicsScene() # create a new scene object.
        self.view = QGraphicsView(self.scene) # create a new view object of the scene.
        self.view.setRenderHint(QPainter.Antialiasing) # set the rendering hint for the view to antialiasing for smooth lines.
        self.setCentralWidget(self.view) # create a central widget for the main window to hold the main content of the application.

        self.scene.setBackgroundBrush(QBrush(self.plot_background)) # create a new brush object for the background of the scene.
        self.pen = QPen(self.plot_color) # create a new pen object for the drawing of the scene.
        self.pen.setWidthF(0.5) # set pen width.

        self.set_frame() # set the frame of the visualization scene based on the boundaries.
        self.set_starting_point() # set the starting point for the visualization.

        self.timer = QTimer() # create a new timer object.
        self.timer.timeout.connect(self.update_frame) # connect the timeout signal to the update_frame method.
        
        self.setWindowTitle("L-System Visualization") # set window title.
        self.resize(800, 800)  # set window size.
            
    def set_boundaries(self) -> list: 
        """
        Calculates the minimum and maxiumum x and y coordinates in the parsed L-System string; used for setting the boundaries of the visualization.

        The boundaries are calculated by "walking" through the parsed L-System string and keeping track of the maximum and minimum x and y coordinates.

        Returns:
        list: A list of the minimum and maximum x and y coordinates.
        """
        x, y = 0, 0
        min_x, max_x = 0, 0
        min_y, max_y = 0, 0
        direction = self.starting_direction
        stack = [] # initialize visualization variables.
        
        for char in self.parsed: # mimic the behavior of the visualization by "walking" through the parsed L-System to find extreme coordinates. 
            if char.isalpha() and char.isupper(): 
                x += math.cos(math.radians(direction))
                y -= math.sin(math.radians(direction)) 
                min_x = min(min_x, x)
                max_x = max(max_x, x)
                min_y = min(min_y, y)
                max_y = max(max_y, y)
            elif char.isalpha():
                x += math.cos(math.radians(direction))
                y -= math.sin(math.radians(direction))
            elif char == '+':
                direction += self.turn_angle
            elif char == '-':
                direction -= self.turn_angle
            elif char == '[':
                stack.append((x, y, direction))
            elif char == ']':
                if stack:
                    x, y, direction = stack.pop()
                    min_x = min(min_x, x)
                    max_x = max(max_x, x)
                    min_y = min(min_y, y)
                    max_y = max(max_y, y)
        
        return [min_x, max_x, min_y, max_y]
    
    def set_frame(self) -> None:
        """
        Sets the boundaries of the visualization scene.

        The boundaries are set by determining the extent of the L-System drawing and adding a 10% margin around it.
        This is to ensure that the entire drawing is visible when the visualization is displayed.
        """
        width = self.max_x - self.min_x
        height = self.max_y - self.min_y
        margin_x = width * 0.1 
        margin_y = height * 0.1 # calculate a 10% margin around the drawing.
        self.scene.setSceneRect(
            self.min_x - margin_x,
            self.min_y - margin_y,
            width + 2 * margin_x,
            height + 2 * margin_y
        ) # set the scene rectangle to the calculated boundaries with margins.

    def set_starting_point(self) -> None: 
        """
        Initializes the starting point for the visualization.

        The starting point is set to the origin (0, 0) and the stack, points, and current index are reset.
        """
        self.x = 0  # start at origin.
        self.y = 0
        self.stack = []
        self.points = [(self.x, self.y)]
        self.current_index = 0

    def update_frame(self) -> None: 
        """
        Updates the visualization frame.

        This function is called repeatedly by the QTimer to incrementally build the visualization.
        It processes the next batch of characters in the parsed L-System string and updates the visualization accordingly.
        If the end of the string is reached, the QTimer is stopped.
        """
        if self.current_index >= len(self.parsed): # stop the timer when the string has been fully parsed.
            self.timer.stop()
            return
        
        batch_size = 1000  # handle 1000 characters at a time.
        
        for _ in range(batch_size):
            if self.current_index >= len(self.parsed):
                self.timer.stop()
                return
                
            old_x, old_y = self.x, self.y
            char = self.parsed[self.current_index]
            
            if char.isalpha() and char.isupper(): # for uppercase letters, move while drawing.
                self.x += math.cos(math.radians(self.starting_direction)) # move in the x direction at the starting angle. 
                self.y -= math.sin(math.radians(self.starting_direction)) # move in the y direction at the starting angle. 
                self.points.append((self.x, self.y)) # add the points.
                line = self.scene.addLine(old_x, old_y, self.x, self.y, self.pen) # create a line between the old and new points.
                self.path_items.append(line) # add the line to the path items.
            elif char.isalpha(): # for lowercase letters, move without drawing.
                self.x += math.cos(math.radians(self.starting_direction))
                self.y -= math.sin(math.radians(self.starting_direction)) # simply add to the x and y coordinates. 
            elif char == '+': # turn left at turn angle
                self.starting_direction += self.turn_angle
            elif char == '-': # turn right at turn angle
                self.starting_direction -= self.turn_angle
            elif char == '[': # create a new stack frame
                self.stack.append((self.x, self.y, self.starting_direction))
            elif char == ']':
                if self.stack: # remove the top stack frame
                    self.x, self.y, self.starting_direction = self.stack.pop()
                    self.points.append((self.x, self.y))
                    
            self.current_index += 1 # continue to the next character.

    def visualize(self) -> None: 
        """
        Shows the QGraphicsView and starts the QTimer to animate the visualization at 1000 frames per second.
        """
        self.show()
        self.view.fitInView(self.scene.sceneRect(), Qt.KeepAspectRatio)
        self.timer.start(1)

        if hasattr(self, 'app') and not self.app.startingUp():
            return self.app.exec_()