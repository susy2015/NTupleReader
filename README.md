# Instructions


## Setup CMSSW

First setup a CMSSW release. We recommend CMSSW_10_2_9, which has support for uproot. You should do this from a working directory in your ~/nobackup area, which has more storage space on cmslpc 

```
mkdir ~/nobackup/NTupleReader
cd ~/nobackup/NTupleReader
cmsrel CMSSW_10_2_9
cd CMSSW_10_2_9
cmsenv
```

## NTupleReader Repo

Checkout and compile the NTupleReader repository. 

```
cd $CMSSW_BASE/src
git clone git@github.com:susy2015/NTupleReader.git
cd NTupleReader/test/
autoconf
./configure
make clean
make -j 4
```

Run test

```
cd $CMSSW_BASE/src/NTupleReader/test/
./tupleReadTest
```

