from setuptools import setup, Extension

qcmulator_module = Extension(
    'qcmulator',
    sources=['python/qcmulator.c'],
    libraries=['quantum'],  # Link against libquantum.so
    library_dirs=['.'],  # Path to libquantum.so
    include_dirs=['src/headers']
)

setup(
    name='qcmulator',
    version='1.0',
    description='Python bindings for the quantum statevector simulator.',
    ext_modules=[qcmulator_module],
)
