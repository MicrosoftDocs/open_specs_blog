<div id="page">

# An Exploration of CFB using Python 6

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
5/18/2011 7:40:00 PM

-----

<div id="content">

**Directory Entries**

The general concept of a CFB File is the containment of a

hierarchical structure of storages and streams. These are analogous to

directories and files in the traditional FAT file system used in the DOS
and

Windows Operating Systems. A storage can be considered a folder or a
directory

while a stream is considered to be like that of a file. Both storages
and

streams are represented by Directory Sectors which are a type of file
sector. The

number of directory entries per sector depends on the version of the
file.

Version 3 equates to 4 directory entries per sector while version 4 has
32

directory entries per sector.

These entries take on a form similar to that of the CFB

Header. Thus to read a Directory Sector would take on a logic similar to
the

following:

*if majver == 3:  
*                for

i in range ( 0, 3 ):  
                                read\_dir\_entry

( )

*if majver == 4:  
*                for

j in range ( 0, 31 ):  
                                read\_dir\_entry

( )

This logic would continue based upon the Directory Sector

Markers found in the FAT. The Directory Entry itself takes on a rather
simple

form:

*def read\_dir\_entry:  
*                dir\_entry\_name

                              = f.read (

64 )  
                dir\_entry\_name\_len

                     = f.read ( 2 )  
                object\_type

                                       =

f.read ( 1 )  
                color\_flag

                                          =

f.read ( 1 )  
                left\_sibling\_id

                                  = f.read

( 4 )  
                right\_sibling\_id                                =

f.read ( 4 )  
                child\_id

                                               =

f.read ( 4 )  
                clsid

                                                      =

f.read ( 16 )  
                state\_bits

                                           =

f.read ( 4 )  
                creation\_time

                                   =

f.read ( 8 )  
                modified\_time

                                 = f.read

( 8 )                starting\_sector\_location

             = f.read ( 4 )  
                stream\_size

                                       =

f.read ( 8 )

Thus, every time a directory sector is located inside of the FAT, this

logic could be used in the subsequent storing of each individual
directory

entry.

</div>

</div>
