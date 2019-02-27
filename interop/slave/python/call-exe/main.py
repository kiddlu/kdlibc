import commands
import os

main = "./test"
if os.path.exists(main):
    ret, out = commands.getstatusoutput(main)
    print 'ret = %d, \nout = %s' % (ret, out)

print '*'*10
print '*'*10

f = os.popen(main)
data = f.readlines()
f.close()
print data

print '*'*10
print '*'*10
os.system(main)
