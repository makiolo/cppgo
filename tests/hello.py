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
# not possible, fd is only for Derived objects
# inheritance.fd(b)
inheritance.fd(d)
x = inheritance.factory()
inheritance.fb(x)

import factory
print (dir(factory))
base = factory.Base("hola", 33)
a = factory.A("hi", 2)
b = factory.B("hi", 4)
print (base)
print (a)
print (b)

f1 = factory.create("A", "hola", "adios", 1234)
f2 = factory.create("B", "hola222", "adios333", 1234567)
print (f1)
print (f2)
