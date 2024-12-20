import getopt, sys, os

def translate(fname):
    f = open(fname, mode="rb")
    s = f.read()
    f.close()

    b = []
    for c in s:
        if c < 32 or c > 126:
            b.append("\\{:03n}".format(c))
        else:
            if chr(c).isalnum():
                b.append(chr(c))
            else:
                b.append("\\{:03n}".format(c))

    s2 = ''.join(b)

    fname2 = os.path.splitext(fname)[0]+"_result.txt"
    print(fname, fname2)
    f = open(fname2, mode="w", encoding = "ascii")
    f.write(s2)
    f.close()
    

args = sys.argv[1:]

options = "f:"

try:
    arguments, values = getopt.getopt(args, options)

    for c in arguments:
        if c[0] == "-f":
            print(arguments, c)
            translate(c[1])
except getopt.error as err:
    print(str(err))