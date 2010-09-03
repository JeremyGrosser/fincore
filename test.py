import fincore
import sys

def main():
    if len(sys.argv) < 2:
        print 'Usage: %s <filename>' % sys.argv[0]
        return

    fd = file(sys.argv[1], 'r')

    cached = 0

    vec = fincore.fincore(fd.fileno())
    fd.close()

    for page in vec:
        if ord(page) & 0x01:
            cached += 1

    total = len(vec)
    print '%i of %i pages cached (%.00f%%)' % (cached, total, (float(cached) / float(total)) * 100.0)

if __name__ == '__main__':
    main()
