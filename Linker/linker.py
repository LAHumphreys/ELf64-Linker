#!/usr/bin/python 

class EOF(Exception): pass

def int2hex(i):
    return Hex(hex(int(i)))
class Hex(object):
    def __init__(self,token):
        self.value = int(token,16)
    def nextBoundrary(self,pageSize):
        offset = pageSize.value - ( self.value % pageSize.value )
        return Hex(hex(offset)) + self
    def __add__(self,other):
        return Hex(hex(self.value + other.value))
    def __int__(self):
        return int(self.value)
    def __repr__(self):
        return hex(self.value)
    def __str__(self):
        return self.__unicode__()
    def __unicode__(self):
        s =  hex(self.value)
        return s[2:]

class Byte(object):
    def __init__(self,ltoken,rtoken):
        if type(ltoken) != type(Hex('0')):
            ltoken = Hex(ltoken)
        if type(rtoken) != type(Hex('0')):
            rtoken = Hex(rtoken)
        self.left  = ltoken
        self.right = rtoken
        self.value = Hex("%s%s" %(ltoken, rtoken))
    def __int__(self):
        return int(self.value)
    def __repr__(self):
        return Hex(self.value).__repr__()
    def __str__(self):
        return self.__unicode__()
    def __unicode__(self):
        return str(self.value).zfill(2)


class Flags(object):
    def __init__(self,token,aliases={},ic=True):
        self.ignoreCase = ic
        if self.ignoreCase:
           aliases = { key.upper() : value.upper() for key, value in aliases.items() }
        self.flags = [f for f in token]
        self.icflags = [ f.upper() for f in token]
               
        self.map = aliases
        self.known = self.FromSet(sorted(self.map.values()))

    def FromSet(self,flags):
        foundFlags = ""
        for f in flags:
            if self[f]:
               foundFlags += f
        return foundFlags
    def Add(self,flag,alias=""):
        if not self[f]:
           self.flags.append(flag)
           self.icflags.append(flag.upper())
    def __getitem__(self,key):
        if self.ignoreCase:
           key = key.upper()
           flags = self.icflags
        else:
           flags = self.flags
        if key in self.map:
            key = self.map[key]
        return key in flags

    def __repr__(self):
        return 'Flags object'
    def __str__(self):
        return self.__unicode__()
    def __unicode__(self):
        if self.ignoreCase:
           flags = self.icflags
        else:
           flags = self.flags
        return ''.join(sorted(flags))


class Base(object):
    def ProcessLine(self,line):
        tokens = line.split()
        if len(tokens) < self.mandatoryTokens or len(tokens) > self.maxTokens:
              err = "Badly fomrated %s line: %s\n" % (self.title, line)
              err += "Expected format: %s" % self.expectedFormat
              raise Exception(err)
        self.Map = {}
        for i in range(min(len(tokens),len(self.fields))):
            try:
               field = self.fields[i]
               constructor = self.constructors[i]
               self.Map[field] =  constructor(tokens[i])
            except Exception, e:
               err = "Invalid token for field %s in %s\n" % (field, self.title)
               err += "From line: %s\n" % line
               err += "Error was: %s" % e
               raise Exception(err)
        if len(tokens) > len(self.fields):
           self.extra = tokens[len(self.fields):]
        else:
           self.extra = []
        
    def __getitem__(self,key): 
        return self.Map[key]
    def __setitem__(self,key,value): 
        self.Map[key] = value
    def __str__(self):
        return self.__unicode__()
    def __unicode__(self):
        tokens = [ "%s " % str(self[field]) for field in self.fields ]
        tokens += [ "%s " % t for t in self.extra ]
        # Join and remove the trailing space
        return ''.join(tokens)[:-1]


class Header(Base):
    def __init__(self,line):
        self.title = "Header block"
        self.expectedFormat = "#segs #syms #relocs"
        self.mandatoryTokens = 3
        self.maxTokens = 3
        self.fields = ["nsegs", "nsyms", "nrels"]
        self.constructors = [int, int, int]
        self.ProcessLine(line)

class Segment(Base):
    class SegmentFlags(Flags):
        def __init__(self,token):
            aliases = {"Readable": "R", "Writeable": "w", "Present": "P", "Allocate": "A"}
            ic = True
            super(type(self),self).__init__(token,aliases,ic)
        
    def __init__(self,line):
        self.title = "Segment definition"
        self.expectedFormat = ".name address size flags"
        self.mandatoryTokens = 5
        self.maxTokens = 5
        self.fields = ["name", "address", "size" , "flags", "alignment"]
        self.constructors = [str, Hex, Hex, self.SegmentFlags, int2hex]
        self.ProcessLine(line)
        self.data = ""
        self.offset = Hex("0")
        self.fillToken = Byte("0","0")
        self.CalcNextNeighbour()

    def CalcNextNeighbour(self):
        self.nextNeighbour = (self.offset + \
                              self['size']+Hex("-1")).nextBoundrary(self['alignment'])
    def Paded(self):
        outbytes = self.GetData()
        tok = str(self.fillToken)
        for i in range(int(self['size']),int(self.nextNeighbour)):
            outbytes += tok
        return outbytes
            
    def hasData(self): 
        return self['flags']['present'] and int(self['size']) > 0
    def SetData(self,line):
        try:
          tokens = [Hex(t) for t in line]
        except Exception, e:
           err = "Invalid code segment"
           err += "From line: %s\n" % line
           err += "Error was: %s" % e
           raise Exception(err)

        count = int(self['size'])
        if len(tokens) != count*2:
           err = "Invalid code segment\n"
           err += "Segment is defined to have size: %d " % self['size']
           err += "but actually has size %d\n" % (len(tokens)/2)
           err += "%s" % self
           err += "\n %s" %line
           raise Exception(err)

        self.bytes = [Byte(tokens[i],tokens[i+1]) for i in range(0,len(tokens),2)]

    def GetData(self):
        if self.hasData():
           return ''.join([str(b) for b in self.bytes])
        else:
           return ''
    def CompatibleWith(self,other):
        return self['name']       == other['name'] and \
               str(self['flags'].known) == str(other['flags'].known)
    def __add__(self,other):
        if not self.CompatibleWith(other):
           err =  "Trying to add two non compatible segments \n"
           err += "self: %s \n other: %s" % (self, other)
           raise Exception(err)
        name    = self['name']
        address = self['address']
        size    = self.nextNeighbour + other['size']
        flags   = self['flags']
        align   = self['alignment']
        oalign  = other['alignment']
        if align != oalign:
           err =   "Trying to add two non compatible segments \n"
           err +=  "( alignments do not match )\n"
           err += "self: %s \n other: %s" % (self, other)
        seg = "%s %s %s %s %s" %(name, address, size, flags, align)
        new =  Segment(seg)
        # We could simply append the two byte lists, but for small files may as
        # well take the performance hit for the extra validation
        data = ""
        sdata = self.hasData()
        odata = other.hasData()
        # If we're going to add neighbouring data, first need to pad
        if sdata and odata:  data += self.Paded()
        if sdata and not odata: data += self.GetData()
        if other.hasData(): data += other.GetData()
        if len(data) > 0 : new.SetData(data)
        return new



class Symbol(Base):
    class SymbolFlags(Flags):
        def __init__(self,token):
            aliases = {"Defined": "D", "Undefined": "U", \
                       "Global" : "G", "Local"   :  "L", \
                       "File"  :  "F", "Section" :  "S", \
                       "Object":  "O", "Func"    :  "P" }
            ic = True
            super(type(self),self).__init__(token,aliases,ic)
               
    def __init__(self,line,parentFile):
        self.title = "Symbol definition"
        self.expectedFormat = "name value segmentIndex type scope"
        self.mandatoryTokens = 5
        self.maxTokens = 5
        self.fields = ["name", "value", "seg" , "type", "scope"]
        self.constructors = [str, Hex, int, self.SymbolFlags, self.SymbolFlags]
        self.ProcessLine(line)
        self.sourceFile=parentFile

        if self['type']['Undefined'] and int(self['value']) != 0:
            self.commonBlock = True
        else: self.commonBlock = False

        if not self['type']['Undefined']: self.resolvesTo=self

    def ResolveSymbol(self,sym):
        if sym['name'] != self['name']:
           err = "Can not resolve symbols with different names\n"
           err += "%s\n" %sym
           err += "%s" %self
           raise Exception(err)
        else: 
           if self.commonBlock:
               err = "Error: attempted  to resolve a common block"
               err += "%s\n" %sym
               err += "%s" %self
	       raise Exception(err)
		    
	   # This is a bit disgusting - but, by setting a couple of pointers
	   # we've "resolved" the symbol
           self.resolvesTo = sym
	   self.Map = sym.Map
	   # When we come to want an address we're gong to want the one defined
	   # symbol has been mapped to
	   self.sourceFile = sym.sourceFile
    def __repr__(self):
         return "Symbol"
            


class Relocation(Base):
    def __init__(self,line):
        self.title = "relocation definition"
        self.expectedFormat = "location segment reference type"
        self.mandatoryTokens = 4
        self.maxTokens = 99
        self.fields = ["loc", "seg", "ref" , "type"]
        self.constructors = [Hex, int, int, str]
        self.ProcessLine(line)


class Iter(object):
    def __init__(self,items):
        self.items = items
        self.index = 0
        self.length = len(items)
    def __iter__(self):
        return self
    def next(self):
        if self.index >= self.length:
            raise StopIteration
        else:
             i = self.items[self.index]
             self.index += 1
             return i


class Reader:
    def __init__(self,fname):
        self.fname = fname
        self.lines = open(fname,"r").readlines()
        self.seeker = 0
    
    def GetNextLine(self):
        linkValid = False
        while not linkValid:
            if self.seeker >= len(self.lines) :
                raise EOF
            code = self.CleanLine(self.lines[self.seeker])
            linkValid = self.ValidateLine(code)
            self.seeker+=1
        return code

    def Read(self,count):
        for i in range(count):
            yield self.GetNextLine()
    def ReadLines(self):
        while 1 :
            try:
                yield self.GetNextLine()
            except EOF, e:
                break

    def CleanLine (self, line):
        return line.strip()
    def ValidateLine(self,line):
        if len(line) > 0:
            return line[0] != "#"
        else:
            return 0
# Homogenous data container
class Container(object):
    # Customise the itterator to deal with our indexes going 1-> end +1
    class Iter(Iter):
        def __init__(self,items):
            super(type(self),self).__init__(items)
            self.index = 1
            self.length +=1
    def __init__(self, items=[],nameField='name',dataType=None):
         if len(items) > 0: self.itemType = type(items[0])
         else:              self.itemType = dataType
         self.items = []
         self.itemMap = {}
         self.nameField = nameField
         for i in items:
            self._addItem(i)

    def _addItem(self, item):
        iname = item[self.nameField]
        self.items.append(item)
        ind = len(self.items)-1
        # table count starts at 1
        self.itemMap[iname] = ind+1
    def __len__(self):
       return len(self.items)
    def __iter__(self):
       return self.Iter(self)
    def evalKey(self,key):
       if type(key) == type(0):
          return key-1
       elif type(key) == self.itemType:
          return self.evalKey(key[self.nameField])
       else:
          return self.evalKey(self.itemMap[key])
    def __getitem__(self,key):
       key = self.evalKey(key)
       return self.items[key]
    def __setitem__(self,key,item):
       try:
           key = self.evalKey(key)
           self.items[key] = item
       except KeyError, e:
           self.append(item)
    def __contains__(self,item):
       return item[self.nameField] in self.itemMap
    def append(self,item):
        iname = item[self.nameField]
        if iname in self.itemMap:
            err =  "Can not add item to the container: "
            err += "key name already exists \n"
            err += "Tried to add: %s \n" %item
            err += "to : %s \n" %self
            raise Exception(err)
        self.items.append(item)
        self.itemMap[iname] = len(self.items)

    def __repr__(self):
        return 'Homongenous container'
    def __str__(self):
        return self.__unicode__()
    def __unicode__(self):
        lines = [ "%s\n" % str(s) for s in self.items]
        # Join and remove the trailing newline
        if len (lines) > 0:
           return ''.join(lines)[:-1]
        else:
           return ""

class Many21Container(Container):
    def _addItem(self, item):
        iname = item[self.nameField]
        self.items.append(item)
        ind = len(self.items)-1
        # table count starts at 1
        if iname in self.itemMap: self.itemMap[iname] += [ind+1]
        else:                self.itemMap[iname]  = [ind+1]
    def evalKey(self,key):
       if type(key) == type(0):
          return [key-1]
       elif type(key) == type([0]):
          return [k-1 for k in key] 
       elif type(key) == self.itemType:
          return self.evalKey(key[self.nameField])
       else:
          return self.evalKey(self.itemMap[key])
    def __getitem__(self,key):
       if type(key) == type(0):
           key = self.evalKey(key)[0]
           return self.items[key]
       else:
	   #If we're doing list indexing, returning an empty list is valid
           try:
               key = self.evalKey(key)
               return [ self.items[i] for i in key]
	   except KeyError, e:
	       return []
    def __setitem__(self,key,item):
       raise Exception('setter has no meaning for a many21 container')
    def append(self,item):
       self._addItem(item)
    def __repr__(self):
        return 'Many21 Container'

class Reader:
    def __init__(self,fname):
        self.fname = fname
        self.lines = open(fname,"r").readlines()
        self.seeker = 0
    
    def GetNextLine(self):
        linkValid = False
        while not linkValid:
            if self.seeker >= len(self.lines) :
                raise EOF
            code = self.CleanLine(self.lines[self.seeker])
            linkValid = self.ValidateLine(code)
            self.seeker+=1
        return code

    def Read(self,count):
        for i in range(count):
            yield self.GetNextLine()
    def ReadLines(self):
        while 1 :
            try:
                yield self.GetNextLine()
            except EOF, e:
                break

    def CleanLine (self, line):
        return line.strip()
    def ValidateLine(self,line):
        if len(line) > 0:
            return line[0] != "#"
        else:
            return 0


# for LINK version
class Parser:
     def __init__(self,reader):
         self.reader = reader
         #First read the header block
         self.header = Header(reader.GetNextLine())
         self.ReadSegDefs()
         self.ReadSymTable()
         self.ReadRelocationTable()
         self.ReadData()

     def ReadSegDefs(self):
         expectedDefs = self.header['nsegs']
         segments = [ Segment(l) for l in self.reader.Read(expectedDefs)]
         self.segments = Container(segments)

     def ReadSymTable(self):
         expectedDefs = self.header['nsyms']
         symbols = [ Symbol(l,self) for l in self.reader.Read(expectedDefs)]
         self.symTable = Many21Container(items=symbols)

     def ReadRelocationTable(self):
         expectedDefs = self.header['nrels']
         relocs = [ Relocation(l) for l in self.reader.Read(expectedDefs)]
         self.relocTable = Container(relocs,"loc")

     def ReadData(self):
         for s in self.segments:
            if s['flags']['present'] and int(s['size']) > 0:
               s.SetData(self.reader.GetNextLine())



def GetParser(fname):
    reader = Reader(fname)
    magicMarker = reader.GetNextLine()
    if magicMarker == "LINK":
        return Parser(reader)
    else:
         raise Exception("What the fuck is this? %s" % magicMarker)

