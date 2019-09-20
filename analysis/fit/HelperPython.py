
import numpy
import math
import inspect
from array import array
# this allows you to call console (bash) commands into your script
from subprocess import call
# help us see if a file exists
import os.path
# import operator needed to sort dictionaries in a list
import operator
# to catch the stdout from ROOT in a text file
import os,sys
# for checking the itme stamps on the files
import datetime
# time measurement
from time import time
# deep copy for dictionaries
from copy import deepcopy

# start functions

def percentageDifference(x,y,debug=False):
  if debug:
    print "x",x,"y",y
  if -0.0001<y<0.0001:
    result=0.0
  else:
    result=100*(x-y)/y
  return result
# done function

def listVariables(name,prefix):
  return prefix+name.replace(',',','+prefix)
# done function

def updateListVariables(prefix,name,suffix):
  return prefix+name.replace(',',suffix+','+prefix)+suffix
# done function

def getValues(event,listVariables,debug=False):
  if debug:
    print "listVariables",listVariables
  return numpy.array([getattr(event,variableName) for variableName in listVariables.split(',')])
# done function

def getFileNameStem(fileName,debug=False):
  if debug:
    print "fileName",fileName
  fileNameSplitBySlash=fileName.split("/")
  # now pick the last element, which means with path removed
  # find out how many elements are in the list
  nrElements=len(fileNameSplitBySlash)
  fileNameWithoutPath=fileNameSplitBySlash[nrElements-1]
  if debug:
    print "fileNameWithoutPath",fileNameWithoutPath
  fileNameStem=fileNameWithoutPath.split(".")[0]
  if debug:
    print "fileNameStem",fileNameStem
  # a bit hard coded, but we know the name of the root files start with
  # train_tree_ and we want to remove that, it has 11 characters
  fileNameStem=fileNameStem[12:]
  if debug:
    print "fileNameStem",fileNameStem
  return fileNameStem
# done function

def getProcessName(fileNameStem,debug=False):
  if debug:
    print "fileNameStem",fileNameStem
  fileNameStemSplitByUnderscore=fileNameStem.split("_")
  # now pick the last before last element, which means the process
  # as the last means _merged or _0
  processName=fileNameStemSplitByUnderscore[-2]
  if debug:
    print "processStem",processName
  return processName
# done function

# if based on the sample name and the index of the event (even or odd) we should skip the event, then continue
# for the train sample we use events of index 0, 2, 4, etc (even)
# for the test sample we use events of index 1, 3, 5, etc (odd)
# for the all sample we use all events
def keepEntry(sample,i,debug):
  if debug:
    print "i",i,"sample",sample
  result=False
  if sample=="train":
    # keep only even-number entries (events or jets), as they were used in the training of the NN
    if i%2==0:
      result=True
  elif sample=="test":
    # keep only odd-number entries (events or jets), as they were used in the testinging of the NN
    if i%2==1:
      result=True
  elif sample=="all":
    # keep all the entries (events or jets)
    result=True
  else:
    print "sample",sample,"not known, will abort. Need to choose between train, test and all"
    assert(False)
  # done if on sample
  return result
# done function

def combinations(iterable, r):
    # combinations('ABCD', 2) --> AB AC AD BC BD CD
    # combinations(range(4), 3) --> 012 013 023 123
    pool = tuple(iterable)
    n = len(pool)
    if r > n:
        return
    indices = range(r)
    yield tuple(pool[i] for i in indices)
    while True:
        for i in reversed(range(r)):
            if indices[i] != i + n - r:
                break
        else:
            return
        indices[i] += 1
        for j in range(i+1, r):
            indices[j] = indices[j-1] + 1
        yield tuple(pool[i] for i in indices)
# done function

def doesFileExist(filename, debug):

    if os.path.isfile(filename):
        if debug:
            print "DEBUG: File: %s Exisits" % (filename)

        return True
    else:
        return False
# done function

def OpenFile(filename, debug):
    
    try:
        file = open(filename,"r")
        if debug:
            print "DEBUG: Opening File: %s" % (filename)
        return file
    except IOError, (erno, sterror):
        print "IOError error(%s): %s" % (errno, strerror)
    except:
        print "Unexpected error:", system.exc_info()[0]
        raise
# done function

class Sigmoid:
    def __call__( self, x, par ):
        return 1/(1+par[0]*math.exp(par[1]*(x[0])))
    # done function
# done class

class Linear:
    def __call__( self, x, par ):
        return par[0]+x[0]*par[1]
    # done function
# done class

class Parabolic:
    def __call__( self, x, par ):
      return par[0]+par[1]*x[0]+par[2]*math.pow(x[0],2)
    # done function
# done class

class Polynomial3:
    def __call__( self, x, par ):
      result=0.0
      for i in xrange(4):
        result+=par[i]*math.pow(x[0],i)
      return result
    # done function
# done class

class Polynomial4:
    def __call__( self, x, par ):
      result=0.0
      for i in xrange(5):
        result+=par[i]*math.pow(x[0],i)
      return result
    # done function
# done class

class Polynomial5:
    def __call__( self, x, par ):
      result=0.0
      for i in xrange(6):
        result+=par[i]*math.pow(x[0],i)
      return result
    # done function
# done class

class Polynomial6:
    def __call__( self, x, par ):
      result=0.0
      for i in xrange(7):
        result+=par[i]*math.pow(x[0],i)
      return result
    # done function
# done class

class Gauss:
    def __call__( self, x, par ):
        if par[2]!=0:
          result=par[0]*math.exp(-0.5*math.pow((x[0]-par[1])/par[2],2))
        else:
            result=0.0
        return result
    # done function
# done class

class Bukin:
    def __call__( self, x, par ):

      debug=True

      if debug:
        print "******"
      # inputs
      xx =x[0]
      norm = par[0] # overall normalization
      x0 = par[1] # position of the peak
      sigma = par[2] # width of the core
      xi = par[3] # asymmetry
      rhoL = par[4] # size of the lower tail
      rhoR = par[5] # size of the higher tail
      if debug:
        print "xx",xx
        print "norm",norm
        print "x0",x0
        print "sigma",sigma
        print "xi",xi
        print "rhoL",rhoL
        print "rhoR",rhoR
  
      # initializations
      r1=0.0
      r2=0.0
      r3=0.0
      r4=0.0
      r5=0.0
      hp=0.0
      
      x1=0.0
      x2=0.0
      fit_result=0.0
  
      # set them other values
      consts=2*math.sqrt(2*math.log(2.0))
      hp=sigma*consts
      r3=math.log(2.0)
      r4=math.sqrt(math.pow(xi,2)+1.0)
      r1=xi/r4
      if debug:
        print "consts",consts
        print "hp",hp
        print "r3",r3
        print "r4",r4
        print "r1",r1
        print "x1",x1
        print "x2",x2
        print "x0",x0
        print "xx",xx
        print "xi",xi
        print "math.exp(-6.)",math.exp(-6.)

      if abs(xi)>math.exp(-6.):
        r5=xi/math.log(r4+xi)
      else:
        r5=1.0
      if debug:
        print "r5",r5

      x1=x0+(hp/2)*(r1-1)
      x2=x0+(hp/2)*(r1+1)
      if debug:
        print "x1",x1
        print "x2",x2
        print "x0",x0
        print "xx",xx

      if xx<x1:
        # Left Side
        r2=rhoL*math.pow((xx-x1)/(x0-x1),2)-r3+4*r3*(xx-x1)/hp*r5*r4/math.pow((r4-xi),2)
      elif xx < x2:
        # Centre
        if abs(xi)>math.exp(-6.):
          r2=math.log(1+4*xi*r4*(xx-x0)/hp)/math.log(1+2*xi*(xi-r4))
          r2=-r3*math.pow(r2,2)
        else:
          r2=-4*r3*math.pow(((xx-x0)/hp),2)
        # ended if
      else:
        # Right Side
        r2=rhoR*math.pow((xx-x2)/(x0-x2),2)-r3-4*r3*(xx-x2)/hp*r5*r4/math.pow((r4+xi),2)
      # ended if on what side
        
      if abs(r2)>100:
        fit_result=0
      else:
        # Normalize the result
        fit_result=math.exp(r2)
      # compute result
      result=norm*fit_result
      # return result
      return result
    # done function
# done class

def doesFileExist(filename, debug):

    if os.path.isfile(filename):
        if debug:
            print "DEBUG: File: %s Exisits" % (filename)

        return True
    else:
        return False
# done function

def OpenFile(filename, debug):
    
    try:
        file = open(filename,"r")
        if debug:
            print "DEBUG: Opening File: %s" % (filename)
        return file
    except IOError, (erno, sterror):
        print "IOError error(%s): %s" % (errno, strerror)
    except:
        print "Unexpected error:", system.exc_info()[0]
        raise
# done function

def dictMaker(dictObjects, debug):

    dict = {}
    for name in dictObjects.split(","):
        title = name.rstrip()
        key = ""
        value = ""
        dict[name.split(":")[0]] = name.split(":")[1]

    if debug :
        print "DEBUG: Dict: %s" % (dict)
    return dict
#done function

def tupleMaker(filename, debug):
  
  arg_list = OpenFile(filename, debug)
  
  tuple = []
  for histo_info in arg_list:
    if not histo_info.startswith("#"):
      parsed_arg = [x.strip() for x in histo_info.split(',')]
      tuple.append(parsed_arg)
      
      if debug:
        print "\nDEBUG: TUPLE OF FILE: %s\n %s\n" % (filename, tuple)
        
  return tuple
#done function

# used to run over files in step, for example a root file has 310000 events
# and we want to run on 7 files, each having 50000 events and the last one
# having 10000
# we run on all events with a step of 50000
def getListSteps(nrEntries,event_step,debug):
  if debug:
    print type(nrEntries)
    print type(event_step)
    print "event_step",event_step
  result=[]
  remainder=nrEntries%event_step
  nrSteps=(nrEntries-remainder)/event_step
  if debug:
    print "nrEntries",nrEntries,"event_step",event_step,"nrSteps",nrSteps,"plus remainder",remainder
  for i in xrange(nrSteps):
    result.append((1+i*event_step,(i+1)*event_step))
  result.append((1+nrSteps*event_step,nrEntries))
  if debug:
    print "getListSteps",result
  return result
#done function

def getModTime(file,debug):
  if debug:
    print "file name",file
   
  if os.path.exists(file):
    t = os.path.getmtime(file)
    result=datetime.datetime.fromtimestamp(t)
  else:
    result=0
  # done if
  if debug:
    print "result=",result
  return result
# done function

def find_if_we_run_given_timestamps_input_output_files(fileInputName,fileOutputName,debug):
  TimeFileInput = getModTime(fileInputName,debug)
  TimeFileOutput = getModTime(fileOutputName,debug)
  file_input_exists = TimeFileInput != 0
  file_output_exists = TimeFileOutput != 0
  file_input_newer_than_file_output = False
  if file_input_exists and file_output_exists:
    file_input_newer_than_file_output = TimeFileInput > TimeFileOutput
  # run only if the input file exists and either the output file does not exist or it is older than the input file
  shall_we_run=file_input_exists and (not file_output_exists or file_input_newer_than_file_output)
  if debug:
    print "file_input_exists", file_input_exists
    print "file_output_exists", file_output_exists
    print "file_input_newer_than_file_output", file_input_newer_than_file_output
    print "shall_we_run",shall_we_run
  # ready to return
  return shall_we_run
# done function

def testTrainRankCalc(t_train, t_test):
    
    t_rank = []
    for i in xrange(len(t_train)):
        rank = abs((t_train[i][1] - t_test[i][1])/t_train[i][1])
        l_rank = [t_train[i][0], rank]
        t_rank.append(l_rank)
    
    d_rank = {}
    for i in xrange(len(t_rank)):
        d_rank[t_rank[i][0]] = t_rank[i][1]
    
    return d_rank
# done function

# start functions used for Adrian in runAdi.py to create
# and plot histograms from the flat tree
# ex: list_values ['20', '30', '40']
# ex: returns [(-inf, 20.0), (20.0, 30.0), (30.0, 40.0), (40.0, inf)]

def get_list_intervals(string_values,debug):
    if debug:
        print "string_values",string_values
    result=[]
    if string_values=="":
      result.append((float("-inf"),float("inf")))
    else:
      list_values=string_values.split(",")
      if debug:
        print "list_values",list_values
      # add underflow (from minus infinity to the first element)
      result.append((float("-inf"),float(list_values[0])))
      # add all regular values
      for i in range(len(list_values)-1):
        result.append((float(list_values[i]),float(list_values[i+1])))
      # add overflow (from the last element to infinity)
      result.append((float(list_values[-1]),float("inf")))
    # end if
    # add the bin representing all possible values
    # the superset of the sets above
    # for inclusive studies
    # result.append((float("-inf"),float("inf")))
    # check result
    if debug:
        print "get_list_intervals",result
    # return result
    return result   
# done function

# list_bin: [(-inf, 20.0), (20.0, 30.0), (30.0, 40.0), (40.0, inf)]
# value_bin: 40.0840078125
# The value_bin 40.0840078125 is in the interval (40.0, inf)
def find_bin_in_list(list_bin,value_bin,debug):
  for bin in list_bin:
    if bin[0]<=value_bin<bin[-1]:
      if debug:
        print "The value_bin",value_bin,"is in the interval",bin
      return bin
# done function

def get_array_values(string_values,debug):
    if debug:
        print "string_values",string_values
    result=[]
    list_values=string_values.split(",")
    if debug:
        print "list_values",list_values
    for i in range(len(list_values)):
        result.append((float(list_values[i])))
    # check result
    if debug:
        print "array_values",result
    # return result
    return result   
# done function

# (0.0,90,0) -> "0_90"
# (-inf, 90) -> "inf_90"
# we want to not have dots and - in the name of a histogram
# so that we can do in TBrowswer histo_name->Draw(), etc
def get_bin_string(bin,debug):
    if debug:
        print "bin",bin
    result=""
    for i,value in enumerate(bin):
        if value==float("-inf") or value==float("inf"):
            value_string="inf"
        else:
            value_string="%-.0f" % value
        if debug:
            print "i",i,"value_string",value_string
        if i!=0:
            result+="_"
        result+=value_string
    # done for loop over the two elements in the bin
    if debug:
        print "bin_string",result
    return result
# done function

def get_string_value(axis_name,variable_name,variable_value):
    result="%-2s  %-30s  %-.2f" % (axis_name,variable_name,variable_value)
    return result
# done function

def get_string_scale_resolution(fit_name,scale_value,resolution_value):
    result="%-7s %-.2f %-.2f" % (fit_name,scale_value,resolution_value)
    return result
# done function

def ratio(s,b,debug=False):
  if debug:
    print "s",s,"b",b
  if -0.0001<b<0.0001:
    result=0
  else:
    result=s/b
  return result
# done function

def sensitivity(s,b,debug=False):
  if debug:
    print "s",s,"b",b
  if b<0.0001:
    result=0
  else:
    result=s/math.sqrt(b)
  return result
# done function

def significance(s,b,debug=False):
  if debug:
    print "s",s,"b",b
  if b<0.001:
    result=0
  else:
    # for very low numbers, the sensititivity is a very good approximation
    # of the significance, but the code runs out of digits and approximates
    # the log(1+s/b) wity zero, which makes it have negative values 
    # under the square root and then it crashes
    if s/b<0.000001:
      result=sensitivity(s,b,debug) # sensitivity
    else:
      result=math.sqrt(2*((s+b)*math.log(1+s/b)-s))
  return result
# done function

def average(a,b):
  return 0.5*(a+b)

def average(a,b,c):
  return (1.0/3)*(a+b+c)

def sum_in_quadrature(a,b):
  return math.sqrt(a*a+b*b)

def sum_in_quadrature(a,b,c):
  return math.sqrt(a*a+b*b+c*c)

def print_figures_of_merit(s,b):
    print "s",s,"b",b,"sensitivity",sensitivity(s,b),"significance",significance(s,b)
# done function

def get_duration_of_run(time_start,time_previous,option,debug):
  if option=="start":
    time_first=time_start
  elif option=="current":
    time_first=time_previous
  else:
    print "Option",option,"not known in get_duration_of_run(...). Choose start or current. Will ABORT!!!"
    assert(False)
  # done if
  time_current=time()
  if debug:
    print "current",time_current,"first",time_first,"previous",time_previous
  seconds=time_current-time_first
  minutes=seconds/60.0
  hours=minutes/60.0
  result="%-.0f s. %-.1f min. %-.3f h." % (seconds,minutes,hours)
  time_previous=deepcopy(time_current)
  if debug:
    print "result",result
  return time_previous,result
# done function

