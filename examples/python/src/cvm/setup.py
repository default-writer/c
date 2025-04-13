from setuptools import setup, Extension

module = Extension(
    'cvm',
    sources=['cvm.c'],
    include_dirs=['../../../../src'],
    library_dirs=['../../../../lib'],
    libraries=['c-vm', 'c-sys'], 
)

setup(
    name='cvm',
    version='0.0.1',
    description='cvm implementation in C',
    ext_modules=[module],
)
