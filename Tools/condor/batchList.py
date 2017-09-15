import os
import optparse
from glob import glob

eosurl = "root://cmseos.fnal.gov"

def eoscp(filename, path):
    stripedName = filename.split("/")[-1]
    command = "xrdcp %s %s/%s"%(filename, eosurl, "/".join([path, filename]))
    print command
    stdout = os.popen(command)
    for l in stdout:
        print l.strip("\n")

def eosls(path, option = ""):
    return os.popen("eos %s ls %s %s"%(eosurl, option, path))

def recSearch(path, fout = None):
    for l in eosls(path, "-l"):
        if l.startswith("d"):
            endpath = l.split()[-1]
            recSearch("/".join([path, endpath]), fout)
        else:
            filename = "/".join([path, l.split()[-1]])
            if filename.endswith(".root") and not "failed" in filename:
                if fout == None:
                    print "".join(["%s/"%eosurl, filename])
                else:
                    fout.write("".join(["%s/"%eosurl, filename, "\n"]))


if __name__ == "__main__":

    parser = optparse.OptionParser("usage: %prog [options]\n")
    
    parser.add_option ('-d', dest='directory', type='string', default = "", help="file path to begin")
    parser.add_option ('-f', dest='file',      type='string', default = "", help="File name to transfer")
    parser.add_option ('-l', dest='list', action="store_true", default = False, help="Create file lists")
    parser.add_option ('-c', dest='copy', action="store_true", default = False, help="Copy a file to eos")
    
    options, args = parser.parse_args()
    
    startPath = options.directory
    
    if options.list:
        for l in eosls(startPath, "-l"):
            if l.startswith("d"):
                endpath = l.split()[-1]
                print "".join([endpath, ".txt"])
                fileName = "".join([endpath, ".txt"])
                f = open(fileName, "w")
                recSearch("/".join([startPath, endpath]), f)
                f.close()
        
                if options.copy:
                    eoscp(fileName, startPath)

    if options.copy:
        files = glob(options.file)
        if not(len(files)):
            print "No files to transfer with glob options.file"
        else:
            for f in files:
                eoscp(f, startPath)
            
