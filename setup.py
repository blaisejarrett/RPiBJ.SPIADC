import distribute_setup
distribute_setup.use_setuptools()
from setuptools import setup, Extension, find_packages

classifiers = ['Development Status :: 1 - Alpha',
               'Operating System :: POSIX :: Linux',
               'Intended Audience :: Developers',
               'Programming Language :: Python :: 2.7',
               'Topic :: Software Development',
               'Topic :: Home Automation',
               'Topic :: System :: Hardware']

setup(name             = 'RPiBJ.SPIADC',
      version          = '0.0.1',
      author           = 'Blaise Jarrett',
      author_email     = 'me@blaisejarrett.com',
      description      = 'A class to control the microchip 10bit ADCs over SPI with the raspberry pi.',
      keywords         = 'Raspberry Pi SPI ADC',
      classifiers      = classifiers,
      packages         = find_packages(),
      ext_modules      = [Extension('RPiBJ.SPIADC', ['src/py_spi_adc.c', 'src/spi_adc.c'])])
