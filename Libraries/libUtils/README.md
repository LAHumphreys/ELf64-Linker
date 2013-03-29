Utilities Library
===========

This defines several objects that have general use. If an object is a utility it should be useful beyond this project (it may only depend the IO library and other std libraries).

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

<tr><td>flags</td><td>Flags</td><td>Bitmap flag implementation. 

By defining a map, provides translation methods to and from LINK ASCII flags</td><tr>

<tr><td>stdReader</td><td> 
StdReader
IFStreamReader
</td><td>

- StdReader
  + Provides a reader implementation of the IO interface, based on a reference to a std::ifstream
- IFStreamReader
  + Extends the ifstream object to implement the reader IO interface
</td><tr>

<tr><td>stdWriter</td><td> 
StdWriter
OFStreamWriter
</td><td>

- StdReader
  + Provides a wrtier implementation of the IO interface, based on a reference to a std::ofstream
- OFStreamWriter
  + Extends the ofstream reader object to implement the writer IO interface
</td><tr>

<table> 
