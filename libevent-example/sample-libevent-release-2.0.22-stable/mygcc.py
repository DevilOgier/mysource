#!/usr/bin/env python
# coding=utf-8

import argparse
import sys
import os



USAGE = """%s [options]
example: mygcc run/build test.c  -c "-std=gnu++11 -I /home/zhong/ceph-9.2.0/src/" -l "-lpthread" -g "g++"

"""
CFLAGS = "-O0 -Wall -g"
LIBFLAGS ="-levent_core"
lib_filename = ""
        

          

def main():
    parser = argparse.ArgumentParser(usage=USAGE)
    parser.add_argument('-g', '--g', 
            default="gcc", help='gcc or g++.')
    parser.add_argument('-c', '--cflags', 
            default="", help='cflags.')
    parser.add_argument('-l', '--lib',
                        default="",help='lib flags.')
    if len(sys.argv)<3:
        print USAGE
        return -1
    args = sys.argv[3:]
    opts = parser.parse_args(args)
    
    filename = sys.argv[2]
    
    outname = filename[:filename.index(".")]
    
    if sys.argv[1] == "build":
        cmd="%s %s %s -o %s %s %s %s %s"%(opts.g,filename,lib_filename,outname,CFLAGS,LIBFLAGS,opts.cflags,opts.lib)
        print cmd
        os.system(cmd)
    elif sys.argv[1] == "run":
         cmd="%s %s  %s -o %s %s %s %s %s"%(opts.g,filename,lib_filename,outname,CFLAGS,LIBFLAGS,opts.cflags,opts.lib)
         print cmd
         ret = os.system(cmd)
         if ret != 0:
             print "build error"
             return
         cmd = "./%s" % outname
         os.system(cmd)
         print ""
         os.system("rm -rf %s"% outname)
    else:
        print USAGE
        return -1
    
    
if __name__ == "__main__":
    sys.exit(main())  


