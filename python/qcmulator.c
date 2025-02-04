#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../src/headers/statevec.h"

// Wrapper for init_statevec
static PyObject *py_init_statevec(PyObject *self, PyObject *args) {
    unsigned char nqubits;
    if (!PyArg_ParseTuple(args, "b", &nqubits)) {
        return NULL;
    }

    struct Statevec *sv = init_statevec(nqubits);
    if (!sv) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to initialize statevector.");
        return NULL;
    }

    return PyCapsule_New(sv, "Statevec", NULL);
}

// Wrapper for print_statevec
static PyObject *py_print_statevec(PyObject *self, PyObject *args) {
    PyObject *capsule;
    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }

    struct Statevec *sv = PyCapsule_GetPointer(capsule, "Statevec");
    if (!sv) {
        PyErr_SetString(PyExc_ValueError, "Invalid Statevec capsule.");
        return NULL;
    }

    print_statevec(sv);
    Py_RETURN_NONE;
}

// Method definitions
static PyMethodDef QCMulatorMethods[] = {
    {"init_statevec", py_init_statevec, METH_VARARGS, "Initialize a statevector."},
    {"print_statevec", py_print_statevec, METH_VARARGS, "Print a statevector."},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef qcmulatormodule = {
    PyModuleDef_HEAD_INIT,
    "qcmulator",
    NULL,
    -1,
    QCMulatorMethods
};

// Module initialization
PyMODINIT_FUNC PyInit_qcmulator(void) {
    return PyModule_Create(&qcmulatormodule);
}
