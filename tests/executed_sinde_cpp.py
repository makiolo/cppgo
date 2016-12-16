#!/usr/bin/env python

import factory

class PythonDerived1(factory.Base):
    def name(self):
        return "PythonDerived1 -> base: {} and derived: {}".format(super(PythonDerived1, self).name(), "PythonDerived1")

class PythonDerived2(factory.Base):
    def name(self):
        return "PythonDerived2 -> base: {} and derived: {}".format(super(PythonDerived2, self).name(), "PythonDerived2")

'''
factory.register("PythonDerived1")
factory.register("PythonDerived2")

print ("creating A since python")
f1 = factory.create("A", "hi", 1234)
print ("creating B since python")
f2 = factory.create("B", "hizzz", 1234567)
print ("show A name")
print (f1.name())
print ("show B name")
print (f2.name())
'''
