#!/usr/bin/env python3

import sys
import os
import csv
import matplotlib.pyplot as plt
import math

# Create bar graph based on rows of CSV file
def bar(first,second,folder):
    fig,ax = plt.subplots()

    # Convert "second" to a list of floats
    x_vals = [float(x) for x in second[1:]]

    # Get title of graph
    name = second[0]

    # Get colors to use
    colors = 'rgbkymc'

    x_pos = range(len(first))

    plt.ylim([0,int(math.ceil(float(max(second[1:]))))])

    # Graph data
    plt.bar(x_pos, x_vals, align='center', color=colors)
    plt.margins(0,0)

    plt.xticks(x_pos, first, rotation=45)

    # Annotate graph
    plt.title(name)
    plt.ylabel('Time to Create (seconds)')
    plt.xlabel('Algorithms')

    plt.savefig(os.path.join(folder,'{0}.png'.format(name)), bbox_inches='tight', format='png')
    plt.close('all')

def usage():
    print('usage: python graph.py results.csv out-folder')
    sys.exit(1)

def _main():
    if len(sys.argv) != 3:
        usage()

    fn = sys.argv[1]
    outFolder = sys.argv[2]

    if not os.path.exists(outFolder):
        os.mkdir(outFolder)

    # Read contents of CSV file
    with open(fn,'r') as fr:
        # Get pairs of rows
        content = csv.reader(fr, delimiter=',')
        first = None
        second = None
        for e,row in enumerate(content):
            if (e+1) % 2 == 0:
                second = row
                bar(first,second,outFolder)
            else:
                first = row

if __name__ == '__main__':
    _main()
