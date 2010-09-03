from distutils.core import setup, Extension

setup(name='python-fincore',
      version='0.1',
      ext_modules=[
          Extension('fincore', ['fincore.c']),
      ])
