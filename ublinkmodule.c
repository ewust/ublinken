
#include <Python.h>

static PyObject *
ublink_show(PyObject *self, PyObject *args)
{
    const char buf[400];
    int buf_len = sizeof(buf);

    if (!PyArg_ParseTuple(args, "s#", buf, &buf_len)) {
        return NULL;
    }

    printf("Woo got a %d byte string\n", buf_len);

    return Py_BuildValue("i", 0);
}

static PyMethodDef ublink_methods[] = {
    {"show", ublink_show, METH_VARARGS},
    {NULL, NULL}
};

/*
 * Python calls this to let us initialize our module
 */
void initublink()
{
    (void) Py_InitModule("ublink", ublink_methods);
}
