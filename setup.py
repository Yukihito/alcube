from setuptools import setup

requires = ['pyyaml']
extras = {'test': ['flake8']}

setup(
    name='app',
    install_requires=requires,
    test_requires=extras['test'],
    extras_require=extras,
)