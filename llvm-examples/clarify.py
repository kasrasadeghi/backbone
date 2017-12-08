import sys
import re


def clarify(file):
    lines = [l for l in file]
    lines = list(filter(lambda l: not l.startswith('; Function Attrs'), lines))
    lines = list(filter(lambda l: not l.startswith('attributes #'), lines))
    lines = list(filter(lambda l: not l.startswith('!'), lines))
    lines = [re.sub(' #\\d', '', l) for l in lines]
    lines = [re.sub(', !tbaa !\\d+', '', l) for l in lines]
    lines = [re.sub(' local_unnamed_addr', '', l) for l in lines]

    return lines


def main():
    filename = sys.argv[1]
    with open(filename, 'r') as f:
        lines = clarify(f)

    with open(filename, 'w') as f:
        f.truncate(0)
        f.writelines(lines)


if __name__ == '__main__':
    main()