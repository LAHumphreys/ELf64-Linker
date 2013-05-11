" Luke Humphreys 2013
"
" This files maintains a file which contains a list of breakpoints,
" gdb should be configured to read this file in when it starts
"
" A python dictionary is maintained so that adding / deleting 
" marks is in-memory, and the only file system actions are to write
" the new file
"
" Note: a subtlety is that vim requires the full path (buffer name)
"       to annotate the symbol, while we only want the filename in the
"       breakpoints file.
 
function! BreakPointsInit()
"
" First Setup the python environment
"
python << endpython
import vim
#regular expression library
import re
# Compile the breakpoint search:
# For simplicity lets just not handle the corner case where someone was
# crazy enough to put a ':' in a filename...

fileNamePattern = "[^:]*"
lineNumPattern = "[0-9]*"
breakRegex = re.compile("^ *break *(%s):(%s) *$" %(fileNamePattern, lineNumPattern))

breakpoints = {}

#
# Replace the breakpoints file by dumping the dictionary
# in the form:
#   break  <fname>:<line number>
#   break  <fname 2>:<line number 2>
#   ...
def WriteNewBreakFile ():
    global breakpoints
    f = open(vim.eval("g:breakpoints_file"), "w")
    for fname in breakpoints:
        for lnum in breakpoints[fname]:
            f.write("break %s:%s\n" %(fname, lnum))
    f.close()

#
# Called to add the breakpoint symbol to an open vim window
# Vim requires the id to be numeric, but only unique within a file,
# so we use the linenumber
#
def AddBreakSymbol(fname, lnum):
    global breakpoints
    bid = lnum
    cmd = "sign place %s line=%s name=breakpoint file=%s" %(bid, lnum,fname)
    vim.command(cmd)

#
# Remvoe a breakpoint symbol from an open vim window
#
def RemoveBreakSymbol(fname, lnum):
    global breakpoints
    bid = lnum
    cmd = "sign unplace %s file=%s" %(bid, fname)
    vim.command(cmd)

#
# Read the breakpoints file into a dictionary of the form:
# breakpoints[<filename>] = [<list of line numbers>]
#
def LoadBreakPoints ():
    global breakpoints
    breakfile = open(vim.eval("g:breakpoints_file"), "r")
    for line in breakfile.readlines():
        match = breakRegex.match(line)
        if match:
            file = match.group(1)
            lnum = match.group(2)
            if file not in breakpoints:
                breakpoints[file] = [int(lnum)]
            else:
                breakpoints[file] += [int(lnum)]
    breakfile.close()

#
# Annotate the current file, by looping through 
# the breakpoints loaded by LoadBreakPoints
#
def AnnotateBreakPoints():
    global breakpoints
    fname=vim.eval("expand('%:t')")
    full_path=vim.eval("expand('%')")
    if fname in breakpoints:
        for lnum in breakpoints[fname]:
            AddBreakSymbol(full_path,lnum)

#
# Called by the user too add a breakpoint at their
# cursor position.
#
# We first do a check if it is the dictionary to avoid 
# file system work if we can avoid it.
#
# If it is new, add it to the dictionary, add the symbol to the
# vim window then replace the breakpoints file.
#
def AddBreakPoint():
    global breakpoints
    new = True
# First get required info
    fname=vim.eval("expand('%:t')")
    full_path=vim.eval("expand('%')")
    (lnum,col) = vim.current.window.cursor

# Add it to the global dictionary
    if fname in breakpoints:
        if lnum in breakpoints[fname]  :
            # already there, don't care
            new=False
        else:
            breakpoints[fname] += [lnum]
    else:
        breakpoints[fname] = [lnum]

    if new:
        # Add the sign
        AddBreakSymbol(full_path,lnum)
        # Update the breakpoints file
        WriteNewBreakFile()

#
# Called by the user too remove a breakpoint at their
# cursor position.
#
# We first do a check if it is the dictionary to avoid 
# file system work if we can avoid it.
#
# If it does exist, remove it from the dictinoary, remove
# it from the vim window, then finally replace the
# breakpoints file
#
def RemoveBreakpoint():
    global breakpoints
    new = True
# First get required info
    fname=vim.eval("expand('%:t')")
    full_path=vim.eval("expand('%')")
    (lnum,col) = vim.current.window.cursor

# Add it to the global dictionary
    if fname in breakpoints:
        if lnum in breakpoints[fname]  :
            breakpoints[fname].remove(lnum)
            new=False

    if not new:
        # Remove the sign
        RemoveBreakSymbol(full_path,lnum)
        # Update the breakpoints file
        WriteNewBreakFile()


endpython

"
" Now configure vim
"

" Define the breakpoint symbol
highlight SignColumn ctermbg=darkgrey
sign define breakpoint text=B+ texthl=SignColumn 

" Load the dictionary, and annotate any open window
call LoadBreakPoints ()
call AnnotateBreakPoints()

" Define user commands to add / remove breakpoints
command! BreakAdd call AddBreakPoint()
command! BreakDel call RemoveBreakPoint()
command! BAdd call AddBreakPoint()
command! BDel call RemoveBreakPoint()
command! BreakFile call OpenBreakFile()

" Annotate new files, when they are opened
au BufNewFile,BufRead * call AnnotateBreakPoints()

endfunction

"
" Now expose the python function to vim
"
function! LoadBreakPoints () 
python << endpython
LoadBreakPoints()
endpython
endfunction

function! AnnotateBreakPoints ()
python << endpython
AnnotateBreakPoints()
endpython
endfunction

function! AddBreakPoint ( ) 
python << endpython
AddBreakPoint()
endpython
endfunction

function! RemoveBreakPoint ( ) 
python << endpython
RemoveBreakpoint()
endpython
endfunction

function! OpenBreakFile () 
    let cmd = "sp " . g:breakpoints_file
    exec cmd
endfunction

"
" Trigger initialisation when this script is sourced
"
call BreakPointsInit()
