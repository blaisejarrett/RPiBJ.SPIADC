#include <Python.h>
#include <stdint.h>
#include "spi_adc.h"

/*
 * Exceptions
 */
static PyObject * SetupException;
static PyObject * NotSetupException;
static PyObject * InvalidChannelException;
static PyObject * InvalidDeviceException;


static PyObject * py_read_adc(PyObject * self, PyObject * args)
{
	int channel;
	int16_t read_value;

	if (!PyArg_ParseTuple(args, "i", &channel))
    	return NULL;

    read_value = read_adc((uint8_t)channel);
    switch (read_value)
    {
    	case ERROR_INVALID_CH:
    		PyErr_SetString(InvalidChannelException, "Channel not supported. Use channels 0-7 inclusive.");
    		return NULL;
    	case ERROR_DEVICE_NOT_OPEN:
    	case ERROR_READ:
    		PyErr_SetString(NotSetupException, "Read Failed. Did you call setup?");
    		return NULL;

    	default:
    		break;
    }

	return Py_BuildValue("i", read_value);
}

static PyObject * py_setup_adc(PyObject * self, PyObject * args, PyObject * kwargs)
{
	static char * kwlist[] = {"device", "speed", NULL};
	int device;
	long speed;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "il", kwlist, &device, &speed))
        return NULL;
	
    
    switch (setup_adc((uint8_t)device, (uint32_t)speed))
    {
    	case ERROR_INVALID_DEVICE:
    		PyErr_SetString(InvalidDeviceException, "Setup failed. Valid devices are 0 or 1.");
      		return NULL;
    	case ERROR_FAILED_TO_OPEN:
    	case ERROR_FAILED_TO_SET_MODE:
    	case ERROR_FAILED_TO_SET_BITS:
    	case ERROR_FAILED_TO_SET_SPEED:
    		PyErr_SetString(SetupException, "Setup failed. Are you running as root?");
      		return NULL;

    	default:
    		break;
    }

 
    Py_RETURN_NONE;
}

static PyMethodDef rpi_spiadc_methods[] =
{
    {"setup", (PyCFunction)py_setup_adc, METH_VARARGS | METH_KEYWORDS, "Setup ADC SPI module.\nArg1: device, 0 or 1 on RPI.\nArg2: speed, speed in hz of spi."},
    {"read", py_read_adc, METH_VARARGS, "Read the value for the select channel"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initSPIADC(void)
{
	PyObject * module = NULL;

	if ((module = Py_InitModule("RPiBJ.SPIADC", rpi_spiadc_methods)) == NULL)
		return;

	SetupException = PyErr_NewException("RPiBJ.SPIADC.SetupException", NULL, NULL);
	PyModule_AddObject(module, "SetupException", SetupException);

	NotSetupException = PyErr_NewException("RPiBJ.SPIADC.NotSetupException", NULL, NULL);
	PyModule_AddObject(module, "NotSetupException", NotSetupException);

	InvalidChannelException = PyErr_NewException("RPiBJ.SPIADC.InvalidChannelException", NULL, NULL);
	PyModule_AddObject(module, "InvalidChannelException", InvalidChannelException);

	InvalidDeviceException = PyErr_NewException("RPiBJ.SPIADC.InvalidDeviceException", NULL, NULL);
	PyModule_AddObject(module, "InvalidDeviceException", InvalidDeviceException);

	if (Py_AtExit(close_adc) != 0)
   	{
    	close_adc();
    	return;
    }
}
