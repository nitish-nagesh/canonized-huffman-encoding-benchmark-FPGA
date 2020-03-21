#!/usr/bin/env python3
import itertools
import os, sys
import glob
import re
import numpy as np
import xml.etree.ElementTree as ET

# ***************************************************************************
# Knobs
# ***********
copy0=[1,4,16]
copy1=[1,4,16]
processsymbols=[1,2,4]
createtree=[1,2,4]
createtree_partition=[1,2,4]
assigncodeword=[1,2,4]
assigncodeword_partition=[1,2,4]

allCombinations = list(itertools.product(
    copy0,
    copy1,
    processsymbols,
    createtree,
    createtree_partition,
    assigncodeword,
    assigncodeword_partition))
# ***************************************************************************


def parse_resources(resources_node):
    tags = ['BRAM_18K','DSP48E','FF','LUT']
    resources = [ resources_node.find(t).text for t in tags ]
    return list(map(int, resources))


def parse_xml(filename1,filename2):
    tree = ET.parse(filename1)
    root = tree.getroot()

    #resources_node       = root.find('AreaEstimates/Resources')
    #avail_resources_node = root.find('AreaEstimates/AvailableResources')
    est_clk_period = root.find('TimingReport/AchievedClockPeriod').text
    slices=root.find('AreaReport/Resources/SLICE').text
    tree=ET.parse(filename2)
    root=tree.getroot()
    slices=int(slices)
    best_latency = root.find('PerformanceEstimates/SummaryOfOverallLatency/Interval-min').text
    worst_latency = root.find('PerformanceEstimates/SummaryOfOverallLatency/Interval-max').text
    avg_latency = (int(best_latency) + int(worst_latency)) / 2 
    print(best_latency + " " + worst_latency + " " + str(avg_latency) + "\n" )
    #resources       = parse_resources(resources_node)
    #avail_resources = parse_resources(avail_resources_node)
    # throughput="{0:.3f}".format(((avg_latency*float(est_clk_period))/1000000000))

    # Throughput in KHz
    throughput="{0:.3f}".format(1000000 / (float(est_clk_period) * avg_latency))
    #resources_util = np.divide(resources, avail_resources)*100
    #for i in range(4):
        #resources_util[i]="{0:.2f}".format(resources_util[i])
    return slices,throughput



def main():

    finalCombinations = allCombinations
    file1=open('final_result_impl_huffman.csv','w')
    file1.write("copy0"+","+"copy1"+","+"processsymbols"+","+"createtree"+","+"createtree_partition"+","+"assigncodeword"+","+"assigncodeword_partition"+","+","+"obj1"+","+"obj2\n")
    for d in sorted(glob.glob('script/impl_reports/huffman_encoding_export*.xml')):
        m = re.search('huffman_encoding_export(\d+)', d)
        num = m.group(1)
        synth_path=os.path.join('script/syn_reports/csynth'+num+'.xml')
        slices,lat=parse_xml(d,synth_path)
        file1.write(num+",")
        for j in range(8):
            file1.write(str(finalCombinations[int(num)][j])+",")
        file1.write(str(lat)+","+str(slices)+"\n")


if __name__ == "__main__":
    main()
        
