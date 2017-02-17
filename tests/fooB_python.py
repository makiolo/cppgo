#!/usr/bin/env python

import foo_python

class PythonDerived2(foo_python.Base):
    def __init__(self, a, b):
        super(PythonDerived2, self).__init__(a, b)

    def name(self):
        return "override from python: PythonDerived2"

