from setuptools import setup, Extension

module = Extension(
    'clist',
    sources=['clist.c'],
    include_dirs=['../../../../src'],
)

setup(
    name='clist',
    version='0.0.1',
    description='clist implementation in C',
    ext_modules=[module],
)
