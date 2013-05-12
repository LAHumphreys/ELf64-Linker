#! /usr/bin/env python2
"""Transform gprof(1) output into useful HTML."""

import re, os, sys, cgi, webbrowser

header = """\
<html>
<head>
 <p> <i> Luke Humphreys 2013  </i> </p>
  <p> I've take this script from the python trunk <a href="http://svn.python.org/projects/python/trunk/Tools/scripts/gprof2html.py"> here. </a>
 Sadly the regex no longer matches my current version of gprof, so I have updated the expressions, but all other logic is courtesy of the original author</p>
 <hrule />

  <title>gprof output (%s)</title>
</head>
<body>
<pre>
"""

trailer = """\
</pre>
</body>
</html>
"""

flatPattern = re.compile("( *[0-9.]+ +[0-9.]+ +[0-9.]+ +[0-9.]+ +[0-9.]+ +[0-9.]+ +)(.*)")
callPattern = re.compile("( *[\[\]0-9]* *[0-9.]* *[0-9.]+ +[0-9.]+ +[0-9/]* *)(.*) *(\[[0-9]+\])")

def add_escapes(input):
    for line in input:
        yield cgi.escape(line)

def main():
    filename = "gprof.out"
    if sys.argv[1:]:
        filename = sys.argv[1]
    outputfilename = filename + ".html"
    input = add_escapes(file(filename))
    output = file(outputfilename, "w")
    output.write(header % filename)
    for line in input:
        output.write(line)
        if line.startswith(" time"):
            break
    labels = {}
    for line in input:
        m = flatPattern.match(line)
        if not m:
            output.write(line)
            if "the percentage of the" in line:
                break
        else:
            fname = m.group(2)
            stuff = m.group(1)
            url =  cgi.escape(fname.strip())
            labels[fname] = fname
            output.write('%s<a name="flat:%s" href="#call:%s">%s</a>\n' %
                         (stuff, url, url, fname))
    for line in input:
        output.write(line)
        if line.startswith("index % time"):
            break
    for line in input:
        m = callPattern.match(line)
        if not m:
            output.write(line)
            if line.startswith("Index by function name"):
                break
            continue
        prefix, fname, suffix = m.group(1, 2, 3)
        url = cgi.escape(fname.strip())
        if line.strip().startswith("["):
            output.write('%s<a name="call:%s" href="#flat:%s">%s</a>%s\n' %
                         (prefix, url, url, fname, suffix))
        else:
            output.write('%s<a href="#call:%s">%s</a>%s\n' %
                         (prefix, url, fname, suffix))
    for line in input:
        for part in re.findall(r"(\w+(?:\.c)?|\W+)", line):
            if part in labels:
                part = '<a href="#call:%s">%s</a>' % (part, part)
            output.write(part)
    output.write(trailer)
    output.close()

if __name__ == '__main__':
    main()
