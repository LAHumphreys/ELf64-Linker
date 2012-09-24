from linker import *
import copy

class MappedFile(Parser):
    def __init__(self, parsedFile):
        # Store a reference back to the file 
        self.source  = parsedFile
        self.segments = copy.deepcopy(parsedFile.segments)
        self.symTable = copy.deepcopy(parsedFile.symTable)
        self.undefinedSymbols = Many21Container(dataType=type(Symbol))
        # Update the file pointer
        for sym in self.symTable: 
            sym.sourceFile = self
            if sym['type']['Undefined']: self.undefinedSymbols.append(sym)

        
# for LINK version
class ObjectFile(object):
     def __init__(self):
         self.categories = ['APR', 'APRW', 'ARW' , 'PR']
         self.segments = Container()
         self.catMap = {}
         self.pageSize = Hex("200000")
         # we don't use the first page since null pointers point here!
	 # Its not clear to my why x86_64 starts in the 3rd and not the second
	 # page though
         self.start = Hex("400000")
         # offset (~e8)because the code also includes the header
         self.headerSize = Hex("e8")
         self.start += self.headerSize
         self.wordSize = Hex(hex(4))
         self.files = []
         self.symbols = Many21Container(dataType=type(Symbol))

     
     def LinkInFile(self,parsedFile):
         fname = parsedFile.reader.fname
         # Add a reference to this parsed file to our stack
         f = MappedFile(parsedFile)
         for segment in f.segments:
             if int(segment['size']) >0 or segment['name']=='.bss': self.AddSegment(segment)
         for symbol in f.symTable:
              if symbol.commonBlock:
                 self.AddCommonBlock(symbol)
              elif not symbol['type']['Undefined']:
                 self.DefineSymbol(symbol)
         self.files.append(f)
                 
     def DefineSymbol(self, symbol):
         # Look for any matching undefined symbols and define them
         if self.SymbolIsUseful(symbol):
            self.symbols.append(symbol)
            for f in self.files:
                for sym in f.undefinedSymbols[symbol]:
                    sym.ResolveSymbol(symbol)
        
     def SymbolIsUseful(self,symbol):
        if not symbol['scope']['Global']:
            return False
        usefulTypes = ["Object", "Func"]
        flags = symbol['type']
        for t in usefulTypes:
            if flags[t]: return True
        return False
     def AddCommonBlock(self, symbol):
        if (not symbol['type']['undefined']) or symbol['value'] == 0:
           e =  "Can not add common block from ths symbol"
           e += "\n%s\n" % (symbol)
           e += "Symbol is not a common block"
           raise Exception(e)
        name = "__common__.%s" % symbol['name']
        addr = 0
        size = symbol['value']
        flags = "ARW" # add to .bss
        seg = "%s %s %s %s" % (name, addr, size, flags)
        self.AddSegment(Segment(seg))
         

     def AddSegment(self,segment):
         name = segment['name']
         fname = name[:name.index(".")]
         if name not in self.catMap:
            self.catMap[name] = self.ClassifySegment(segment)
         else:
            if self.ClassifySegment(segment) != self.catMap[name]:
               e =  "Can not link in segment with "
               e += "name %s and flags %s\n" % (name, self.ClassifySegment(segment))
               e += "A section with this name already "
               e += "exists with flags %s" % catMap[name]
               raise Exception(e)
         if segment not in self.segments:
             # Force a deep copy (the ugly way)
             self.segments[name] = Segment(str(segment))

             segment.offset = Hex("0")
             segment.destination = self.segments[name]
             if segment.hasData():
                 self.segments[name].SetData(segment.GetData())
         else:
             if fname == "__common__":
                currSize = int(self.segments[name]['size'])
                newSize = int(segment['size'])
                if newSize > currSize:
                    self.segments[name]['size'] = Hex(hex(newSize))
             else:
                 # Internally this is a deep copy, data including data
                 segment.offset = self.segments[name].nextNeighbour
                 self.segments[name].append(segment)
                 segment.destination = self.segments[name]

     def ClassifySegment(self,segment):
         flags = str(segment['flags'].FromSet("APRW"))
         if flags not in self.categories:
            e =  "Can not classify segment with "
            e += "name %s and flags %s\n" % (segment['name'], flags)
            e =  "Unknown flag combination: %s (%s)" % (segment['flags'], flags)
            raise Exception(e)
         return flags

     def OrderSegments(self):
         buf = {}
         for cat in self.categories:
             buf[cat] = []
         for s in self.segments:
             name = s['name']
             buf[self.catMap[name]].append(s)
         new = []
         for cat in self.categories:
             new += buf[cat]
         # we have a new list of items - now install new container
         self.segments=Container(new)

         return buf

     def RealignSymbols(self):
         for s in self.symbols:
             sourceSegment = s.GetSegment()
             destinationSegment = sourceSegment.destination
             addr = destinationSegment['address'] + sourceSegment.offset + s['value']
             print s['name'], destinationSegment['address'] , sourceSegment.offset , s['value'], addr
             #update the address
             s['value'] = addr
             s['seg'] = self.segments.GetIdx(destinationSegment['name'])
 
     def AllocateCateogry(self,category,offset):
         first = True
         for segment in category:
             if not segment['flags']['allocate']: continue
             if not first:
                offset = offset.nextBoundrary(segment['alignment'])
             else: first = False
             segment['address'] = offset
             offset = segment['address'] + segment['size'] + Hex("-1")
         return offset
     def Allocate(self):
         # first get everything in the right order
         categories = self.OrderSegments()

         #First write the code section
         offset = self.start
         offset=self.AllocateCateogry(categories['APR'],offset)

	 # Add a full page (this somehow makes it easier to swap 
         # directly with the object file [see QMAGIC] )
         offset = offset + Hex("1")  + self.pageSize

         # DATA
         offset=self.AllocateCateogry(categories['APRW'],offset)

         # BSS etc
         offset = offset.nextBoundrary(self.wordSize)
         offset=self.AllocateCateogry(categories['ARW'],offset)

         # now update the symbol addresses
         self.RealignSymbols()

     def ObjectFile(self):
         file = "LINK\n"
         file += "%s\n" %self.GetHeader()
         file += "%s\n" %self.segments
         file += "%s\n" %self.symbols
         file += "%s\n" %self.GetData()
         return file

     def GetHeader(self):
        nsegs = len(self.segments)
        nrelocs = 0 
        nsyms = 0
        header = "%s %s %s" % (nsegs, nsyms, nrelocs) 
        return Header(header)

     def GetData(self):
         data = ""
         for s in self.segments:
            if s['flags']['present']:
               data += "%s\n" % s.GetData()
         return data

