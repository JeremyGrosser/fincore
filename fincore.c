#include <Python.h>

#include <unistd.h>
#include <sys/mman.h>

// fincore.fincore(fileno)
static PyObject *fincore_fincore(PyObject *self, PyObject *args) {
	PyObject *ret;
	int fd;
	void *file_mmap;
	unsigned char *mincore_vec;
	struct stat file_stat;
	ssize_t page_size = getpagesize();
	ssize_t vec_size;

	if(!PyArg_ParseTuple(args, "i", &fd)) {
		return NULL;
	}

	if(fstat(fd, &file_stat) < 0) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	file_mmap = mmap((void *)0, file_stat.st_size, PROT_NONE, MAP_SHARED, fd, 0);

	if(file_mmap == MAP_FAILED) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	vec_size = (file_stat.st_size + page_size - 1) / page_size;
	mincore_vec = calloc(1, vec_size);

	if(mincore_vec == NULL) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}

	if(mincore(file_mmap, file_stat.st_size, mincore_vec) != 0) {
		PyErr_SetFromErrno(PyExc_OSError);
		return NULL;
	}

	ret = Py_BuildValue("s#", mincore_vec, vec_size);
	free(mincore_vec);
	munmap(file_mmap, file_stat.st_size);
	return ret;
}

static PyMethodDef FincoreMethods[] = {
	{"fincore", fincore_fincore, METH_VARARGS, "Return the mincore structure for the given file."},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initfincore(void) {
	(void)Py_InitModule("fincore", FincoreMethods);
}
