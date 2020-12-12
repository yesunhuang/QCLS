try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension

module1 = Extension("QCLSolver.core",
                    sources=["./QCLSolver/core/cluster_expansion.cpp", "./QCLSolver/core/common.cpp",
                             "./QCLSolver/core/complex.cpp", "./QCLSolver/core/deb.cpp",
                             "./QCLSolver/core/derive_data.cpp", "./QCLSolver/core/node.cpp",
                             "./QCLSolver/core/operator_tree.cpp", "./QCLSolver/core/operator_tree_iterator.cpp",
                             "./QCLSolver/core/py_wrapper.cpp", "./QCLSolver/core/static_dtree.cpp"],
                    include_dirs=['./QCLSolver/core/'],
                    #define_macros=[('DEB', 1)]
                    )

# long description
with open('README.md', "r", encoding='utf-8') as fp:
    long_description = fp.read()

# install requirements automatically
requirements = []
with open('requirements.txt', 'r', encoding='utf-8') as fp:
    for s in fp.readlines():
        requirements.append(s.strip())

setup(name="QCLSolver",
      version="1.0.4",
      description="A tool for cluster expansion solver.",
      long_description=long_description,
      long_description_content_type="text/markdown",
      author="Yuexun Huang & Ke Lin",
      author_email="yesunhuang@mail.ustc.edu.cn",
      packages=['QCLSolver'],
      ext_modules=[module1],
      classifiers=[
          'Development Status :: 5 - Production/Stable',
          'Intended Audience :: Science/Research',
          'Topic :: Scientific/Engineering :: Physics',
          'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
          'Natural Language :: English',
          'Operating System :: OS Independent',
          'Programming Language :: C++',
      ],
      keywords='physics quantum optics',
      project_urls={
          'Documentation': 'https://github.com/yesunhuang/QCLS',
          'Source': 'https://github.com/yesunhuang/QCLS',
          'Tracker': 'https://github.com/yesunhuang/QCLS/issues',
      },
      install_requires=requirements,
      python_requires='>=3.7')
