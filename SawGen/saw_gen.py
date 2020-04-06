import sys, os, shutil, getopt

A_MAX = 500
INC = 40
NUM_PULSE = 5

def form_pulse(pulse_num):
    with open(str(pulse_num) + '.txt', 'w') as f:
        a = 0
        print(a)
        f.write(str(a) + '\n')
        while a <= (A_MAX - INC):
            a += INC
            print(a)
            f.write(str(a) + '\n')

def main():
    for i in range(NUM_PULSE):
        form_pulse(i)


if __name__ == '__main__':
    main()