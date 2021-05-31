/**
 * @ 文件名: main_wrapper.c
 * @ 功能: 封装, 并提供与Python的交互接口
 * @ 作者: AyajiLin & YesunHuang
 * @ 邮箱: 493987054@qq.com & 944812203@qq.com
 * @ github: https://github.com/leonardodalinky & https://github.com/yesunhuang
 * @ 日期: 2020年2月9日
 *
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>

#include "deb.h"
#include "derive_data.h"

#include <cmath>

using namespace ayaji;

#define RAISE_PY_ERROR(err, strerr) do{if (!PyErr_Occurred()){PyErr_SetString(err, strerr);}}while(0)

void destructDataCapsule(PyObject * capsule) {
	DeriveData* p = (DeriveData*)PyCapsule_GetPointer(capsule, "ClusterData");
	if (p == NULL)
		return;
	delete p;
	return;
}

int SetCurrentValueOfDData(DeriveData& data, const std::vector<Complex>& vecs) {
	for (int i = 0; i < std::min(std::size_t(data.size), vecs.size()); ++i) {
		data.curValues[i] = vecs[i];
	}
	DeriveData::_UpdateDDTTreeValue(data);
	return 0;
}

int SetHOCoefOfDData(DeriveData& data, const std::vector<Complex>& vecs) {
	Complex temp = { 0,1 };
	for (int i = 0; i < std::min(std::size_t(data.hoSize), vecs.size()); ++i) {
		data.hoCoefs[i] = vecs[i] * temp;
	}
	return 0;
}

int SetCOCoefOfDData(DeriveData& data, const std::vector<Complex>& vecs) {
	Complex temp = { (double)0.5, 0 };
	for (int i = 0; i < std::min(std::size_t(data.coSize), vecs.size()); ++i) {
		data.coCoefs[i] = vecs[i] * temp;
	}
	return 0;
}

/**
 * @ 函数: int CheckArgsDData(PyObject* dataobjct, pDeriveData* outputdd)
 *
 * @ 功能: 将被包装的DeriveData数据解包, 并传回c指针
 *
 * @ param{dataobject}: 被包装的pDeriveData数据
 *
 * @ param{outputdd}: 解包的pDeriveData指针输出
 *
 * @ 返回值: 返回DeriveData指针。
 */
DeriveData* CheckArgsDData(PyObject* dataobject) {
	if (!PyCapsule_CheckExact(dataobject)) {
		/* 检查是否为capsule */
		RAISE_PY_ERROR(PyExc_TypeError, "The passed arg isn't a cluster data object.");
	}
	if (strcmp(PyCapsule_GetName(dataobject), "ClusterData") != 0) {
		/* 检查是否为cluster的数据 */
		RAISE_PY_ERROR(PyExc_TypeError, "The passed arg isn't a cluster data object.");
	}
	return (DeriveData*)PyCapsule_GetPointer(dataobject, "ClusterData");
}

/// <summary>
/// 若PyList中为Long类型，转换成LabelSeq
/// </summary>
/// <param name="pylist"></param>
/// <returns></returns>
LabelSeq parse_PyList_Long_To_LabelSeq(PyObject* pylist) {
	if (!PyList_CheckExact(pylist)) {
		RAISE_PY_ERROR(PyExc_TypeError, "parse_PyList_Long_To_LabelSeq error.");
	}
	ssize_t listsize = PyList_Size(pylist);
	LabelSeq ans;
	ans.reserve(listsize);
	for (int i = 0; i < listsize; ++i) {
		PyObject* tempobj = PyList_GetItem(pylist, i);
		if (!PyLong_Check(tempobj)) {
			RAISE_PY_ERROR(PyExc_TypeError, "parse_PyList_Long_To_LabelSeq error.");
		}
		ans.push_back(PyLong_AsLong(tempobj));
	}
	return std::move(ans);
}

/// <summary>
/// 若PyList中为复数、Long或浮点类型，转换成Vector含Complex。
/// </summary>
/// <param name="pylist"></param>
/// <returns></returns>
std::vector<Complex> parse_PyList_XXX_To_Vector_Complex(PyObject* pylist) {
	if (!PyList_CheckExact(pylist)) {
		RAISE_PY_ERROR(PyExc_TypeError, "parse_PyList_XXX_To_Vector_Complex error.");
	}
	ssize_t listsize = PyList_Size(pylist);
	std::vector<Complex> ans;
	ans.reserve(listsize);
	for (int i = 0; i < listsize; ++i) {
		PyObject* tempc = PyList_GetItem(pylist, i);
		if (PyComplex_CheckExact(tempc)) {
			ans.push_back(Complex(PyComplex_RealAsDouble(tempc), PyComplex_ImagAsDouble(tempc)));
		}
		else if (PyFloat_Check(tempc)) {
			ans.push_back(Complex(PyFloat_AsDouble(tempc), 0));
		}
		else if (PyLong_Check(tempc)) {
			ans.push_back(Complex(PyLong_AsDouble(tempc), 0));
		}
		else {
			RAISE_PY_ERROR(PyExc_TypeError, "parse_PyList_XXX_To_Vector_Complex error.");
		}
	}
	return std::move(ans);
}


/**
 * @ 函数: static PyObject* core_DeriveAssign(PyObject* self, PyObject* args)
 *
 * @ 功能: 根据传入的参数, 进行DeriveAndAssign过程
 *
 * @ 七个参数说明: [Initial状态], [Hamilton算符...], [Hamilton算符系数], [Collapse算符...], [Collapse算符系数], [Tracking算符...], 最长算符长度
 *
 * @ 返回值: 若存在删除并成功时,返回值为1; 否则,返回值为0.
 */
static PyObject*
core_DeriveAssign(PyObject* self, PyObject* args) {
	if (PyTuple_Size(args) != 7) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many(few) args are passed.");
	}
	/* 读入数据并转换类型 */
	std::vector<LabelSeq> inputSeq_HO;
	std::vector<Complex> inputSeqCoef_HO;
	std::vector<LabelSeq> inputSeq_CO; 
	std::vector<Complex> inputSeqCoef_CO;
	LabelSeq inputSeq_Init; 
	std::vector<LabelSeq> inputSeq_Track;
	int maxOPLen;
	//pOPArray* inputArr_HO; int* inputArrLens_HO; Complex* inputArrCoef_HO; int inputArrLen_HO;
	//pOPArray* inputArr_CO; int* inputArrLens_CO; Complex* inputArrCoef_CO; int inputArrLen_CO;
	//int* inputArr_Init; int inputArrLen_Init;
	//pOPArray* inputArr_Track; int* inputArrLens_Track; int inputArrLen_Track;
	//int maxOPLen;
	PyObject* objects[6];
	// 解析表格
	if (!PyArg_ParseTuple(args, "OOOOOOI", objects, objects + 1, objects + 2, objects + 3, objects + 4, objects + 5, &maxOPLen)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The passed args aren't valid.");
	}
	// [Initial状态]
	if (!PyList_CheckExact(objects[0])) {
		RAISE_PY_ERROR(PyExc_TypeError, "The 1st arg isn't a list.");
	}
	inputSeq_Init = parse_PyList_Long_To_LabelSeq(objects[0]);
#ifdef __CRTDEBUG__
	printf("object[0]\n");
#endif // __TREEDEBUG__
	// [Hamiton算符...],
	if (!PyList_CheckExact(objects[1])) {
		RAISE_PY_ERROR(PyExc_TypeError, "The 2nd arg isn't a list.");
	}
	ssize_t inputArrLen_HO = PyList_Size(objects[1]);
	for (int i = 0; i < inputArrLen_HO; ++i) {
		PyObject* tempobj = PyList_GetItem(objects[1], i);
		if (!PyList_CheckExact(tempobj)) {
			RAISE_PY_ERROR(PyExc_TypeError, "The arg in hamilton list isn't a list.");
		}
		inputSeq_HO.push_back(parse_PyList_Long_To_LabelSeq(tempobj));
	}
	// [Hamilton算符系数...]
	if (!PyList_CheckExact(objects[2])) {
		RAISE_PY_ERROR(PyExc_TypeError, "The 3rd arg isn't a list.");
	}
	inputSeqCoef_HO = parse_PyList_XXX_To_Vector_Complex(objects[2]);
#ifdef __CRTDEBUG__
	printf("object[1,2]\n");
#endif // __CRTDEBUG__
	// [Collapse算符...]
	if (!PyList_CheckExact(objects[3])) {
		RAISE_PY_ERROR(PyExc_TypeError, "The 4th arg isn't a list.");
	}
	ssize_t inputArrLen_CO = PyList_Size(objects[3]);
	for (int i = 0; i < inputArrLen_CO; ++i) {
		PyObject* tempobj = PyList_GetItem(objects[3], i);
		if (!PyList_CheckExact(tempobj)) {
			RAISE_PY_ERROR(PyExc_TypeError, "The arg in collapse list isn't a list.");
		}
		inputSeq_CO.push_back(parse_PyList_Long_To_LabelSeq(tempobj));
	}
	// [Collapse算符系数...]
	if (!PyList_CheckExact(objects[4])) {
		RAISE_PY_ERROR(PyExc_TypeError, "The 5th arg isn't a list.");
	}
	inputSeqCoef_CO = parse_PyList_XXX_To_Vector_Complex(objects[4]);
#ifdef __CRTDEBUG__
	printf("object[3,4]\n");
#endif // __CRTDEBUG__
	// [Tracking算符...]
	if (!PyList_CheckExact(objects[5])) {
		RAISE_PY_ERROR(PyExc_TypeError, "The 6th arg isn't a list.");
	}
	ssize_t inputArrLen_Track = PyList_Size(objects[5]);
	for (int i = 0; i < inputArrLen_Track; ++i) {
		PyObject* tempobj = PyList_GetItem(objects[5], i);
		if (!PyList_CheckExact(tempobj)) {
			RAISE_PY_ERROR(PyExc_TypeError, "The arg in tracking list isn't a list.");
		}
		inputSeq_Track.push_back(parse_PyList_Long_To_LabelSeq(tempobj));
	}
#ifdef __CRTDEBUG__
	printf("object[5]\n");
#endif // __CRTDEBUG__
	/* 生成data */
	DeriveData* data = DeriveData::deriveAssign_raw(
		inputSeq_HO, inputSeqCoef_HO,
		inputSeq_CO, inputSeqCoef_CO,
		inputSeq_Init,
		inputSeq_Track,
		maxOPLen);

	/* capsule包装 */
	PyObject* cap = PyCapsule_New(data, "ClusterData", destructDataCapsule);

	return cap;
}

static PyObject*
core_CalEvolution(PyObject* self, PyObject* args) {
	PyObject* pyData = NULL;
	if (PyTuple_Size(args) != 1) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many args are passed.");
	}
	if (!PyArg_ParseTuple(args, "O", &pyData)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The passed arg isn't a object.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;

	std::vector<Complex> output = data.calEvolution();
	if (output.empty()) {
		RAISE_PY_ERROR(PyExc_RuntimeError, "Fatal Error.");
	};
	int size = data.size;
	PyObject* comList = PyList_New(size);
	for (int i = 0; i < size; ++i) {
		PyObject* c = PyComplex_FromDoubles(output[i].getReal(), output[i].getImage());
		PyList_SetItem(comList, i, c);
	}

	return comList;
}

static PyObject*
core_SetCurrentValue(PyObject* self, PyObject* args) {
	if (PyTuple_Size(args) != 2) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many(few) args are passed.");
	}
	PyObject* pyData, * pyList;
	if (!PyArg_ParseTuple(args, "OO", &pyData, &pyList))
	{
		RAISE_PY_ERROR(PyExc_TypeError, "The passed args aren't objects.");
	}
	if (!PyList_CheckExact(pyList)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The second arg isn't a strict list.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;
	auto ret = parse_PyList_XXX_To_Vector_Complex(pyList);

	SetCurrentValueOfDData(data, ret);

	Py_RETURN_NONE;
}

static PyObject*
core_SetHamiltonCoef(PyObject* self, PyObject* args) {
	if (PyTuple_Size(args) != 2) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many(few) args are passed.");
	}
	PyObject* pyData, * pyList;
	if (!PyArg_ParseTuple(args, "OO", &pyData, &pyList))
	{
		RAISE_PY_ERROR(PyExc_TypeError, "The passed args aren't objects.");
	}
	if (!PyList_CheckExact(pyList)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The second arg isn't a strict list.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;
	ssize_t listSize = PyList_Size(pyList);
	auto ret = parse_PyList_XXX_To_Vector_Complex(pyList);

	SetHOCoefOfDData(data, ret);

	Py_RETURN_NONE;
}

static PyObject*
core_SetCollapseCoef(PyObject* self, PyObject* args) {
	if (PyTuple_Size(args) != 2) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many(few) args are passed.");
	}
	PyObject* pyData, * pyList;
	if (!PyArg_ParseTuple(args, "OO", &pyData, &pyList))
	{
		RAISE_PY_ERROR(PyExc_TypeError, "The passed args aren't objects.");
	}
	if (!PyList_CheckExact(pyList)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The second arg isn't a strict list.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;
	ssize_t listSize = PyList_Size(pyList);
	auto ret = parse_PyList_XXX_To_Vector_Complex(pyList);

	SetCOCoefOfDData(data, ret);

	Py_RETURN_NONE;
}

static PyObject*
core_GetCurrentValue(PyObject* self, PyObject* args) {
	PyObject* pyData = NULL;
	if (PyTuple_Size(args) != 1) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many args are passed.");
	}
	if (!PyArg_ParseTuple(args, "O", &pyData)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The passed arg isn't a object.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;

	PyObject* ansList = PyList_New(data.size);
	for (int i = 0; i < data.size; ++i) {
		PyList_SetItem(ansList, i, PyComplex_FromDoubles(data.curValues[i].getReal(), data.curValues[i].getImage()));
	}

	return ansList;
}

static PyObject*
core_GetHamiltonCoef(PyObject* self, PyObject* args) {
	PyObject* pyData = NULL;
	if (PyTuple_Size(args) != 1) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many args are passed.");
	}
	if (!PyArg_ParseTuple(args, "O", &pyData)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The passed arg isn't a object.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;

	if (data.size == 0) {
		RAISE_PY_ERROR(PyExc_RuntimeError, "The data is empty.");
	}
	PyObject* ansList = PyList_New(data.hoSize);
	for (int i = 0; i < data.hoSize; ++i) {
		PyList_SetItem(ansList, i, PyComplex_FromDoubles(data.hoCoefs[i].getReal(), data.hoCoefs[i].getImage()));
	}

	return ansList;
}

static PyObject*
core_GetCollapseCoef(PyObject* self, PyObject* args) {
	PyObject* pyData = NULL;
	if (PyTuple_Size(args) != 1) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many args are passed.");
	}
	if (!PyArg_ParseTuple(args, "O", &pyData)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The passed arg isn't a object.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;

	if (data.size == 0) {
		RAISE_PY_ERROR(PyExc_RuntimeError, "The data is empty.");
	}
	PyObject* ansList = PyList_New(data.coSize);
	for (int i = 0; i < data.coSize; ++i) {
		PyList_SetItem(ansList, i, PyComplex_FromDoubles(data.coCoefs[i].getReal() * 2, data.coCoefs[i].getImage() * 2));
	}

	return ansList;
}

static PyObject*
core_UpdateInitialState(PyObject* self, PyObject* args) {
	if (PyTuple_Size(args) != 2) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many(few) args are passed.");
	}
	PyObject* pyData, * pyList;
	if (!PyArg_ParseTuple(args, "OO", &pyData, &pyList))
	{
		RAISE_PY_ERROR(PyExc_TypeError, "The passed args aren't objects.");
	}
	if (!PyList_CheckExact(pyList)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The second arg isn't a strict list.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;
	auto tmp1 = parse_PyList_XXX_To_Vector_Complex(pyList);
	LabelSeq init_seq;
	for (auto it = tmp1.cbegin(); it != tmp1.cend(); ++it) {
		init_seq.push_back(static_cast<Label>((*it).getReal()));
	}
	std::vector<Complex> ret;
	for (int i = 0; i < data.size; ++i) {
		pNode nowTrack = data.trackNodes[i];
		LabelSeq s = nowTrack->seqFromRoot();
		auto init_tmp = DeriveData::initialValue(s, init_seq);
		ret.push_back(init_tmp);
	}
	data.curValues = ret;
	DeriveData::_UpdateDDTTreeValue(data);

	PyObject* ansList = PyList_New(data.size);
	for (int i = 0; i < data.size; ++i) {
		PyList_SetItem(ansList, i, PyComplex_FromDoubles(ret[i].getReal(), ret[i].getImage()));
	}

	return ansList;
}

static PyObject*
core_ClusterExp(PyObject* self, PyObject* args) {
	PyObject* pyData = NULL;
	if (PyTuple_Size(args) != 2) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many args are passed.");
	}
	PyObject* pyList;
	int maxOPLen;
	if (!PyArg_ParseTuple(args, "OI", &pyList, &maxOPLen))
	{
		RAISE_PY_ERROR(PyExc_TypeError, "The passed args aren't objects.");
	}
	if (!PyList_CheckExact(pyList)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The first arg isn't a strict list.");
	}
	if (maxOPLen < 0) {
		RAISE_PY_ERROR(PyExc_TypeError, "The second arg isn't a non-negative integer.");
	}
	LabelSeq seq = parse_PyList_Long_To_LabelSeq(pyList);
	OPTree tree = ClusterExp::ClusterExpansion(seq);
	// delete and CE
	while (DeriveData::DeleteAndCE(tree, maxOPLen) == 1) {}
	struct {
		std::vector<PyObject*> strs;

		void operator() (Complex c, LabelSeq seq) {
			PyObject* tuple = PyTuple_New(2);
			PyTuple_SetItem(tuple, 0, PyComplex_FromDoubles(c.getReal(), c.getImage()));
			PyObject* arrs = PyTuple_New(seq.size());
			for (size_t i = 0; i < seq.size(); ++i) {
				PyTuple_SetItem(arrs, static_cast<Py_ssize_t>(i), PyLong_FromLong(static_cast<long>(seq[i])));
			}
			PyTuple_SetItem(tuple, 1, arrs);
			strs.push_back(tuple);
		}

		PyObject* toList() {
			PyObject* list = PyList_New(static_cast<Py_ssize_t>(strs.size()));
			for (size_t i = 0; i < strs.size(); ++i) {
				PyList_SetItem(list, static_cast<Py_ssize_t>(i), strs[i]);
			}
			return list;
		}
	} outputer;
	deb_PrintTree(tree, outputer);
	return outputer.toList();
}

static PyObject*
core_PrintData(PyObject* self, PyObject* args) {
	PyObject* pyData = NULL;
	if (PyTuple_Size(args) != 1) {
		RAISE_PY_ERROR(PyExc_TypeError, "Too many args are passed.");
	}
	if (!PyArg_ParseTuple(args, "O", &pyData)) {
		RAISE_PY_ERROR(PyExc_TypeError, "The passed arg isn't a object.");
	}
	DeriveData* pdata = CheckArgsDData(pyData);
	DeriveData& data = *pdata;

	deb_PrintData(data);

	Py_RETURN_NONE;
}


static PyMethodDef coreMethods[] = {
	{"DeriveAssign",  core_DeriveAssign, METH_VARARGS, NULL},
	{"CalEvolution",  core_CalEvolution, METH_VARARGS, NULL},
	{"SetCurrentValue",  core_SetCurrentValue, METH_VARARGS, NULL},
	{"GetCurrentValue",  core_GetCurrentValue, METH_VARARGS, NULL},
	{"SetHamiltonCoef",  core_SetHamiltonCoef, METH_VARARGS, NULL},
	{"SetCollapseCoef",  core_SetCollapseCoef, METH_VARARGS, NULL},
	{"GetHamiltonCoef",  core_GetHamiltonCoef, METH_VARARGS, NULL},
	{"GetCollapseCoef",  core_GetCollapseCoef, METH_VARARGS, NULL},
	{"UpdateInitialState",  core_UpdateInitialState, METH_VARARGS, NULL},
	// Tools
	{"ClusterExp",  core_ClusterExp, METH_VARARGS, NULL},
	{"PrintData",  core_PrintData, METH_VARARGS, NULL},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef coreModule = {
	PyModuleDef_HEAD_INIT,
	"core",		/* name of module */
	"The core for QCLSolver.",			/* module documentation, may be NULL */
	-1,				/* size of per-interpreter state of the module,
						or -1 if the module keeps state in global variables. */
	coreMethods
};

PyMODINIT_FUNC
PyInit_core(void) {
	return PyModule_Create(&coreModule);
}

