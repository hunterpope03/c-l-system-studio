#include "visualizer_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <Python.h>

void initialize_python() {
    Py_Initialize();
    PyObject *sys_module = PyImport_ImportModule("sys");
    PyObject *sys_path = PyObject_GetAttrString(sys_module, "path");
    PyList_Append(sys_path, PyUnicode_FromString("./py"));
    Py_DECREF(sys_module);
    Py_DECREF(sys_path);

    PyRun_SimpleString(
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

void finalize_python() {
    Py_Finalize();
}

void visualize(const char* parsed, double turn_angle, double start_direction) {
    PyObject *pModule = PyImport_ImportModule("visualizer");
    if (!pModule) {
        PyErr_Print();
        return;
    }
    
    PyObject *pClass = PyObject_GetAttrString(pModule, "LSystemVisualizer");
    if (!pClass) {
        PyErr_Print();
        Py_DECREF(pModule);
        return;
    }

    PyObject *pArgs = PyTuple_Pack(3,
        PyUnicode_FromString(parsed),
        PyFloat_FromDouble(turn_angle),
        PyFloat_FromDouble(start_direction)
    );

    PyObject *pInstance = PyObject_CallObject(pClass, pArgs);
    if (!pInstance) {
        PyErr_Print();
        Py_DECREF(pModule);
        Py_DECREF(pClass);
        Py_DECREF(pArgs);
        return;
    }

    PyObject *pMethod = PyObject_GetAttrString(pInstance, "visualize");
    if (pMethod) {
        PyObject_CallObject(pMethod, NULL);
        Py_DECREF(pMethod);
        
        PyRun_SimpleString(
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
    Py_DECREF(pInstance);
}