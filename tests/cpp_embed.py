import foo_python

def info(msg):
    print('[INFO] {}'.format(msg))

foo_python.init()

info('foo_python.create("A, ....")')
f1 = foo_python.create("A", "create c++ class from python", 1)

info('foo_python.create("B, ....")')
f2 = foo_python.create("B", "create c++ class from python", 2)

info('foo_python.create("PythonDerived1, ....")')
f3 = foo_python.create("Python", "PythonDerived1", 3)

info('foo_python.create("PythonDerived2, ....")')
f4 = foo_python.create("Python", "PythonDerived2", 4)

info("1.name() = %s" % f1.name())
info("2.name() = %s" % f2.name())
info("3.name() = %s" % f3.name())
info("4.name() = %s" % f4.name())
