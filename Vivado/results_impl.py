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
copy0=[1,2,4,8]
copy1=[1,2,4,8]
computehistogram=[1,2,4]
computehistogram_partition=[1,2,4]
resort=[1,2]
processsymbols=[1,2]
movenodes=[1,2]
assigncodeword=[1,2]

allCombinations = list(itertools.product(
    copy0,
    copy1,
    computehistogram,
    computehistogram_partition,
    resort,
    processsymbols,
    movenodes,
    assigncodeword))
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
    best_latency = root.find('PerformanceEstimates/SummaryOfOverallLatency/Best-caseLatency').text
    worst_latency = root.find('PerformanceEstimates/SummaryOfOverallLatency/Worst-caseLatency').text
    avg_latency = (int(best_latency) + int(worst_latency)) / 2 
    #resources       = parse_resources(resources_node)
    #avail_resources = parse_resources(avail_resources_node)
    throughput="{0:.3f}".format(((int(avg_latency)*float(est_clk_period))/1000000000))
    #resources_util = np.divide(resources, avail_resources)*100
    #for i in range(4):
        #resources_util[i]="{0:.2f}".format(resources_util[i])
    return slices,throughput



def removeCombinations(combs):

    finalList = []

    for c in combs:
        copyit = True
        if c[5]>c[0]: 
            copyit =False

        if copyit:
            finalList.append(c)

    return finalList


def main():

    finalCombinations = removeCombinations(allCombinations)
    file1=open('final_result_impl_huffman.csv','w')
    file1.write("copy0"+","+"copy1"+","+"computehistogram"+","+"computehistogram_partition"+","+"resort"+","+"processsymbols"+","+"movenodes"+","+"assigncodeword"+","+"obj1"+","+"obj2\n")
    for d in sorted(glob.glob('script/impl_reports/huffman_encoding_export*.xml')):
        m = re.search('huffman_encoding_export(\d+)', d)
        num = m.group(1)
        synth_path=os.path.join('script/syn_reports/csynth'+num+'.xml')
        slices,lat=parse_xml(d,synth_path)
        file1.write(num+",")
        for j in range(8):
            file1.write(str(finalCombinations[int(num)][j])+",")
        file1.write(str(lat)+","+str(slices)+"\n")
        print("processed design" + num)

if __name__ == "__main__":
    main()
        
