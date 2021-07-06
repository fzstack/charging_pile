from mako.template import Template
import os

test_files = [x for x in os.listdir('./tests') if x.endswith('.cxx')]
tests = {}

for test_file in test_files:
    clazz, comp = test_file.split('.')[:2]
    if clazz not in tests:
        tests[clazz] = []
    tests[clazz] += [comp]

t = Template(filename='./scripts/tests.Kconfig.mako')
with open('./tests/Kconfig', 'w') as f:
    f.write(t.render(tests=tests))
