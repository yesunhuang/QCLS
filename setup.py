from distutils.core import setup, Extension

module1 = Extension("QCLSolver.core", 
	sources=["./QCLSolver/core/cluster_expansion.cpp", "./QCLSolver/core/common.cpp", "./QCLSolver/core/complex.cpp", "./QCLSolver/core/deb.cpp", "./QCLSolver/core/derive_data.cpp", "./QCLSolver/core/node.cpp", "./QCLSolver/core/operator_tree.cpp", "./QCLSolver/core/operator_tree_iterator.cpp", "./QCLSolver/core/py_wrapper.cpp", "./QCLSolver/core/static_dtree.cpp"], 
	include_dirs=['./QCLSolver/core/'])

setup(name="QCLSolver",
      version="1.31",
      description="A tool for cluster expansion solver.",
      packages=['QCLSolver'],
      ext_modules=[module1])