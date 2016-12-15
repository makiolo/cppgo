#!/usr/bin/env python

import os
import sys
sys.path.append(os.getcwd())
print (sys.path)

import logging
import imp

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

factory.register("hello.PythonDerived")

pd = PythonDerived("from python", 1234)
print (pd)
print (pd.name())

commands = {}
def load_simple_tasks(plugin_folder, main_module = '__init__'):
    def load_plugin(plugin):
        return imp.load_module(main_module, *plugin["info"])
    if os.path.isdir(plugin_folder):
        # create plugin list
        possibleplugins = os.listdir(plugin_folder)
        plugins = []
        for i in possibleplugins:
            location = os.path.join(plugin_folder, i)
            if not os.path.isdir(location) or not main_module + ".py" in os.listdir(location):
                continue
            info = imp.find_module(main_module, [location])
            plugins.append({"name": i, "info": info})
        # load each plugin
        for p in plugins:
            name = p["name"]
            logging.debug("Loading plugin " + name)
            plugin = load_plugin(p)
            if name not in commands:
                try:
                    plugin_class = getattr(plugin, name)
                    if issubclass(plugin_class, TaskSimple):
                        commands[name] = plugin_class
                    else:
                        logging.info('Error in plugin %s. Your class "%s" don\'t comply with "TaskSimple" interface.' % (name, name))
                except AttributeError:
                    logging.info('Error in plugin %s. Don\'t found a class with name "%s".' % (name, name))
            else:
                logging.error('A command with name %s is already used. Error registering plugin.' % name)
    else:
        logging.debug('No plugins folder found in %s.' % os.path.abspath(plugin_folder))

def func1(n1, n2, n3, f1, str1):
    print ("exec in python {} {} {} {} {}".format(n1, n2, n3, f1, str1))

def func2(str1):
    print ("{}".format(str1))

if __name__ == '__main__':
    print ("loading embed python.")

