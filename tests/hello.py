#!/usr/bin/env python

import hello
print (hello.greet())

import classes
t = classes.World()
t.set("bom dia!")
print (t.greet())

import ctor
c1 = ctor.Ctor("Bonjour")
print (c1.greet())
c2 = ctor.Ctor(3.141592654, 1.0)
print (c2.greet())

import member
m1 = member.SomeClass("Pavel")
print ("name = {}".format(m1.name))
m1.name = "Gunther"
print ("name = {}".format(m1.name))
m1.number = 7.3
print ("number = {}".format(m1.number))

import inheritance
b = inheritance.Base()
d = inheritance.Derived()
inheritance.fb(b)
inheritance.fb(d)
inheritance.fd(d)
x = inheritance.factory()
inheritance.fb(x)

import factory
print (dir(factory))

class PythonDerived(factory.Base):
  def name(self):
    return "PythonDerived"

f1 = factory.create("A", "hi", 1234)
f2 = factory.create("B", "hizzz", 1234567)
print (f1)
print (f1.name())
print (f2)
print (f2.name())
pd = PythonDerived("from python", 1234)
print (pd)
print (pd.name())
