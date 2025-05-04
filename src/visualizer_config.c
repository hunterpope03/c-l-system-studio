#include "visualizer_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

/**
 * @brief Initialize the Python environment once, at the very start of program.
 * 
 * This function must be called once, at the very start of program, to setup the Python
 * environment. It imports the required modules, sets up the Python environment, and
 * adds the current directory to the Python path.
 * 
 * The function is implemented such that it will not crash if the Python environment
 * has already been setup. It is still recommended to call this function only once,
 * at the very start of program.
 */
void initialize_python() { // setup Python environment once, at the very start of program
    Py_Initialize();
    PyObject *sys_module = PyImport_ImportModule("sys");
    PyObject *sys_path = PyObject_GetAttrString(sys_module, "path"); // get sys.path
    PyList_Append(sys_path, PyUnicode_FromString("./py"));
    Py_DECREF(sys_module);
    Py_DECREF(sys_path); // free memory after adding the path

    PyRun_SimpleString( // setup the Python environment
        "import sys\n"
        "import PyQt5.QtWidgets\n"
        "import threading\n"
        "import time\n"
        "from visualizer import LSystemVisualizer\n"
        "\n"
        "class QuitableTimer(threading.Thread):\n"
        "    def __init__(self, app):\n"
        "        threading.Thread.__init__(self)\n"
        "        self.app = app\n"
        "        self.daemon = True\n"
        "\n"
        "    def run(self):\n"
        "        time.sleep(0.5)\n"
        "        self.app.quit()\n"
    );
}

/**
 * @brief Finalize the Python environment once, at the very end of program.
 * 
 * This function must be called once, at the very end of program, to finalize the
 * Python environment. It should not be called more than once, as it will
 * crash the program if called more than once.
 * 
 * The function is implemented such that it will not crash if the Python environment
 * has not been setup. It is still recommended to call this function only once,
 * at the very end of program.
 */
void finalize_python() { // finalize Python environment once, at the very end of program
    Py_Finalize(); 
}

/**
 * @brief Visualize an L-System.
 * 
 * This function visualizes an L-System, given its parsed string, turn angle, and
 * starting direction. The function imports the `visualizer` module, creates
 * an instance of the `LSystemVisualizer` class, and calls its `visualize` method
 * to visualize the L-System. The function also creates a `QApplication` instance
 * and starts the event loop to run the visualization.
 * 
 * @param parsed The parsed string of the L-System.
 * @param turn_angle The angle at which to turn left or right.
 * @param start_direction The starting direction of the visualization.
 */
void visualize(const char* parsed, double turn_angle, double start_direction) { // visualize an L-System
    PyObject *pModule = PyImport_ImportModule("visualizer"); // find visualizer.py
    if (!pModule) {
        PyErr_Print();
        return;
    }
    
    PyObject *pClass = PyObject_GetAttrString(pModule, "LSystemVisualizer"); // find LSystemvisualizer object
    if (!pClass) {
        PyErr_Print();
        Py_DECREF(pModule); // if any failure occurs: print the error, free the memory, and end the function
        return;
    }

    PyObject *pArgs = PyTuple_Pack(3, // pack argumetnts to pass to LSystemVisualizer
        PyUnicode_FromString(parsed),
        PyFloat_FromDouble(turn_angle),
        PyFloat_FromDouble(start_direction)
    );

    PyObject *pInstance = PyObject_CallObject(pClass, pArgs); // call the object with the arguments
    if (!pInstance) {
        PyErr_Print();
        Py_DECREF(pModule);
        Py_DECREF(pClass);
        Py_DECREF(pArgs);
        return;
    }

    PyObject *pMethod = PyObject_GetAttrString(pInstance, "visualize"); // get the visualize method of the LSystemVisualizer object
    if (pMethod) {
        PyObject_CallObject(pMethod, NULL);
        Py_DECREF(pMethod);
        
        PyRun_SimpleString( // execute Python code to create a QApplication instance and start the event loop
            "app = PyQt5.QtWidgets.QApplication.instance()\n"
            "if app:\n"
            "    timer = QuitableTimer(app)\n"
            "    timer.start()\n"
            "    app.exec_()\n"
        );
    } else {
        PyErr_Print();
    }
    
    Py_DECREF(pModule);
    Py_DECREF(pClass);
    Py_DECREF(pArgs);
    Py_DECREF(pInstance); // clean up memory after each visualization
}