#!/usr/bin/env python

import foo_python

class PythonDerived1(foo_python.Base):
    def __init__(self, a, b):
        super(PythonDerived1, self).__init__(a, b)

    def name(self):
        return "override from python: PythonDerived1"

