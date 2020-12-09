install:
	python ./setup.py install

require:
	pip install -r requirements.txt

dist:
	python setup.py sdist
	python setup.py bdist_wheel
	
testpypi:
	python -m twine upload --repository testpypi dist/*
	
pypi:
	python -m twine upload --repository pypi dist/*