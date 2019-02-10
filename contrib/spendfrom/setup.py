from distutils.core import setup
setup(name='Altmarketsspendfrom',
      version='1.0',
      description='Command-line utility for Altmarkets "coin control"',
      author='Gavin Andresen',
      author_email='gavin@Altmarketsfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
