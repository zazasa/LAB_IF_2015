with open('rate-hv.dat','r') as f:
    lines = f.readlines()

with open('rate-hv_tab.txt','w') as f:
    for line in lines[:-1]:
        item = [ l.replace(',','.').replace('E','e') for l in line.replace('\n','').split('\t')]
        out = item[0]+" & "+item[1]+" & "+item[3]+" & "+"%.2e" % float(item[5])+" \\\\\n"
        out = out + "\\hline"
        print out

