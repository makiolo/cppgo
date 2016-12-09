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

