#! /usr/bin/env python
import sys
import argparse

'''
Get function sequence from trace
Author: Yufei Gu
Time-stamp: <2013-09-01 16:43:21 cs3612>
'''

def process_command_line():
    argv = sys.argv[1:]

    # initializing the parser object
    parser = argparse.ArgumentParser(description='Get function sequence from trace .')

    # defining command options
    parser.add_argument('trace', help='input trace file')
    args = parser.parse_args(argv)
    return args.trace


def get_fun_simple( lines):
    '''
    simple version, just keep one line for multiple lines of function executions
    '''
    pre_string = ""
    for line in lines:
        strings = line.split()
        if len(strings) >= 4 and strings[2] != pre_string:
            print strings[2], strings[3]
            pre_string = strings[2]


def format_item(filename, func_name, line_count):
    return "[[./{0}::{1}][{2}]]".format(filename, line_count, func_name)

def source_item(strings):
    source_root_path = "../linux-2.6.32-rc8"
    return "[[{0}::{1}][{2}]]".format(source_root_path + strings[4],strings[3], "s")

def is_call(line):
    strings = line.split()
    if len(strings) >= 7 and strings[2] != "NO": 
        if strings[5] == "call":
            return True
    return False


def is_jmp(line):
    strings = line.split()
    if len(strings) >= 7 and strings[2] != "NO": 
        if strings[5] == "jmp":
            return True
    return False


def print_item(func_stack, strings, filename, line_count, brace):
    print (len(func_stack) - 1)*2* " ", format_item(filename, strings[2], line_count), brace, source_item(strings)


def handle_ret(strings, func_stack, filename, line_count):
    if func_stack[len(func_stack) -1] == strings[2]:
        print_item(func_stack, strings, filename, line_count, "}")
        func_stack.pop()
        return True

    # find the function name from tail of func_stack, get index
    reversed_idx  = 0
    for item in reversed(func_stack):
        reversed_idx = reversed_idx + 1
        if item == strings[2]:
            break
    
    if func_stack[len(func_stack) - reversed_idx] == strings[2]:
        for i in range(reversed_idx - 1):
            func_stack.pop()

        print_item(func_stack, strings, filename, line_count, "}")
        func_stack.pop()
        return True

    
    return False

def get_funs(filename, lines):
    line_count = 0
    pre_string = ""
    pre_line = ""
    func_stack = []
    for line in lines:
        line_count = line_count + 1
        strings = line.split()
        #print a blank line, if there are "NO"s.
        if len(strings) >= 6 and strings[2] == "NO": 
            if pre_string != "NO":
                print ""
            pre_string = "NO"
            
        if len(strings) >= 7 and strings[2] != "NO": 
            pre_string = strings[2]
            if (is_call(pre_line) and (strings[5] in ('push', 'mov', 'xor'))) or (is_jmp(pre_line) and strings[5] == "push" ):
                func_stack.append(strings[2])
                print_item(func_stack, strings, filename, line_count, "{")
                

        if len(strings) >= 7 and strings[2] != "NO" and strings[5] == 'ret': 
            handle_ret(strings, func_stack, filename, line_count) 

        pre_line = line


def main():
    filename = process_command_line()
    f = open(filename)
    lines = f.readlines()
    #header of file, indicate the mode opened in emacs
    print "-*- mode: org; -*-" 
    get_funs(filename, lines)


if __name__ == "__main__":
    status = main()
    sys.exit(status)
