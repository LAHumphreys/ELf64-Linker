Utilities Library
===========

This defines several objects that have general use. 

If an object is a utility it (ideally) should be useful beyond this project and should not perform any non-generic logic.

<table>
<tr><td>*File Basename*</td><td>*Object*</td><td>*Description*</td><tr>
<tr><td>binaryData</td><td>Data</td><td>Store binary data (holds section data). Provide read / write access to the data, and can convert to LINK format (pretty print the data)</td><tr>
<tr><td>binaryDescribe</td><td>BinaryDescribe</td><td>Provides a static function to read a chunk of data from a reader object and return a string hexdump</td><tr>
<tr><td>dataLump</td><td>DataLump</td><td>Full (read + write) array implementation of the IO interface 

( Defines its own array as a member, templated on size )</td><tr>
<tr><td>dataReader</td><td>DataReader</td><td>(Read only) array implementation of the IO interface - initialised with the location of the already allocated array</td><tr>
<tr><td>dataVector</td><td>DataVector</td><td>Full (read + write) std::vector implementation of the IO interface.

DataVector inherits from std::vector and can be used as such - but provides the additional accessors required by the IO interface.</td><tr>

<tr><td>dataWriter</td><td>DataWriter</td><td>(Write only) array implementation of the IO interface - initialised with the location of the already allocated array</td><tr>

<tr>
    <td> 
        env
    </td>
    <td>
        ENV
    </td>
    <td>
        Provides utilities for access environmental variables.
    </td>
</tr>


<tr><td>flags</td><td>Flags</td><td>Bitmap flag implementation. 

By defining a map, provides translation methods to and from LINK ASCII flags</td><tr>

<tr>
    <td> 
        logger
    </td>
    <td>
- Logger, 
- LogDevice,
- LogFactory,
- LogDevice_BinaryWriter,
- GenericFormater
    </td>
    <td>
        Logging utilities. 
        Called through supplied Macros, which check if logging is enabled before executing any code:
+ PRINT:     Equivalent to a print statement
+ LOG        Log at the specified level
+ LOG_FROM   Log at the specified level, specifying a context

Supplied Objects:

+ Logger -  Singleton responsible for dispatching Log messages. 
+ LogDevice -  Interface which must be inplmented by objects wishing to register with Logger
+ LogFactory - provides access to the cout / cerr /clog LogDevices
+ LogDevice_BinaryWriter is an implementation that can be used by any binary writer. All output is passed through Generic Formater
+ Gneric Formater can handle a log message and return a formated string
        
    </td>
</tr>

<tr><td>stdReader</td><td> 
StdReader,
IFStreamReader
</td><td>

- StdWriter
  + Provides a reader implementation of the IO interface, based on a reference to a std::istream
- IFStreamReader
  + Extends the ifstream object to implement the reader IO interface
</td><tr>

<tr><td>stdWriter</td><td> 
StdWriter,
OFStreamWriter
</td><td>

- StdReader
  + Provides a wrtier implementation of the IO interface, based on a reference to a std::ostream
- OFStreamWriter
  + Extends the ofstream reader object to implement the writer IO interface
</td></tr>

<tr>
    <td> 
        util_time
    </td>
    <td>
        Time
    </td>
    <td>
        Provides basic time functions, e.g Timestamp()
    </td>
</tr>

<table> 
