<div id="page">

# How to manually decode an ActiveSync WBXML stream

[JCurry](https://social.msdn.microsoft.com/profile/JCurry) 2/4/2013
10:14:00 AM

-----

<div id="content">

<span style="font-size: large;">Overview</span>  
ActiveSync requests and responses are sent as HTTP messages. In order to
reduce the size of the messages, the body is encoded in a format known
as WAP Binary XML. The information about Microsoft's implementation of
this protocol that is used by ActiveSync is detailed in the
[MS-ASWBXML](http://msdn.microsoft.com/en-us/library/dd299442\(EXCHG.80\).aspx) document
which is based in the [WAP Binary XML Content Format
version 1.2](http://www.w3.org/1999/06/NOTE-wbxml-19990624/).  
  
<span style="font-size: large;">  
Resources:</span>  
   - MS-ASWBXML  
        
<http://msdn.microsoft.com/en-us/library/dd299442(EXCHG.80).aspx>

   - WAP Binary XML Content Format version 1.2  
         <http://www.w3.org/1999/06/NOTE-wbxml-19990624/>

   - IANA Character Sets  
        
<http://www.iana.org/assignments/character-sets/character-sets.xml>  
  
  
<span style="font-size: large;">Why do you need to know how to do
this?</span>  
If you are troubleshooting an application that uses ActiveSync you will
probably collect some type of network trace that contains request and/or
response messages. However, when you try to look at the message contents
it will just appear as a stream of bytes. Unless you have a utility that
can interpret these bytes for you, you will need to know how to decode
them manually. Once you know how to do this you might decide to write a
utility that can do the work for you next time.  
  
  
<span style="font-size: large;">How does it work?</span>  
WBXML is NOT an encryption method, it is an encoding method. The XML
elements in a message are replaced with single byte tokens. MS-ASWBXML
contains a list of these tokens and maps them to an XML element. Simple
enough right?  
  
  
<span style="font-size: large;">Lets get started\!</span>  
The following streams of bytes are a request and response from an
ActiveSync mobile device to an Exchange Server.  
  
<span style="text-decoration: underline;">**Request:**</span>  
<span style="font-family: courier new,courier;">03 01 6A 00 45 5C 4F 4B
03 30 00 01 52 03 32 00  
01 57 00 11 45 46 03 31 00 01 47 03 33 32 37 36  
38 00 01 01 01 01 01 01</span>  
  
<span style="text-decoration: underline;">**Response:**</span>  
<span style="font-family: courier new,courier;">03 01 6A 00 45 5C 4F 4B
03 31 39 39 37 34 33 33  
30 37 34 00 01 52 03 32 00 01 4E 03 31 00 01 01  
01 01</span>  
  
Each message is divided into 3 parts:  
   - Header  
   - String Table  
   - Token List  
  
  
<span style="text-decoration: underline;">**Header**</span>  
The Header consists of the first 3 bytes of the stream and contains the
Version Number, the Document Public Identifier, and the Character Set.
Lets take a look at these.  
  
<span style="font-family: courier new,courier;">03 01 6A</span>  
  
The first byte represents the Version Number and tells us that the
message is encoded using the WBXML 1.3 format. You may have noticed that
MS-ASWBXML references the WBXML 1.2 format. However, we will not
encounter any issues because the message is encoded with the 1.3 format.
The second byte represents the Document Public Identifier. In this case
it means that the public identifier is unknown or missing. The last byte
specifies the Character Set that is being used. The possible values that
you might find here are defined in the [IANA Character
Sets](http://www.iana.org/assignments/character-sets/character-sets.xml)
specification. In this case, 6A (106) is UTF-8. The first line of the
request might look something like this:  
  
<span style="font-family: courier new,courier;">\<?xml version="1.0"
encoding="utf-8"?\></span>  
  
You can find more information about the Version Number, Document Public
Identifier, and the Character Set in the [WAP Binary XML Format
version 1.2](http://www.w3.org/1999/06/NOTE-wbxml-19990624/) and [IANA
Character
Sets](http://www.iana.org/assignments/character-sets/character-sets.xml)
specifications.  
  
  
<span style="text-decoration: underline;">**String Table**</span>  
The 4th byte of the message specifies the length of the String Table. In
this case, the String Table is 0 bytes in length and no other work is
necessary.  
  
  
<span style="text-decoration: underline;">**Token List**</span>  
Following the String Table is the Token List. This is where the real
work/fun starts\! If you haven't already downloaded the latest version
of the
[MS-ASWBXML](http://msdn.microsoft.com/en-us/library/dd299442\(EXCHG.80\).aspx) document,
now would be a good time. To get the most out of this blog I suggest you
follow along in the document. You'll be glad you did if you ever have to
do this again on your own.  
  
<span style="font-family: courier new,courier;">45 5C 4F 4B 03 30 00 01
52 03 32 00 01 57 00 11  
45 46 03 31 00 01 47 03 33 32 37 36 38 00 01 01  
01 01 01 01</span>  
  
For the remainder of the stream it's a simple matter of looking up the
token value in MS-ASWBXML and constructing the XML message. Lets take a
look at the first token, 0x45. Go look it up in MS-ASWBXML and then come
back here to see if you are correct.  
  
What do you mean you can't find it? Here's a hint, it's in section
[2.1.2.1.1](http://msdn.microsoft.com/en-us/library/ee218904\(EXCHG.80\).aspx).
Go look again.  
  
You still can't find it? OK, sorry, that was a trick question. The truth
is that it really is in section
[2.1.2.1.1](http://msdn.microsoft.com/en-us/library/ee218904\(EXCHG.80\).aspx),
but you have to take a closer look at the individual bits in the token
first. This is the only real hurdle in understanding WBXML encoding.
Once you understand the Tag Format the rest is easy.  
  
The Tag Format looks like this.

<div>

**  ** ****
<span style="text-decoration: underline;">**Bits**</span>**  ** ****
****

</div>

</div>

</div>

**  ** ****
<span style="text-decoration: underline;">**Description**</span>

**  ** **** <span style="text-decoration: underline;">**Example**</span>

   8

   If set, the element contains attributes  

   \<element attr="1"\>\</element\>  

   7

   If set, the element contains content

   \<element\>test\</element\>

   6-1

   The tag identity

 

***Note: If you are using Windows 7 or newer, I have found that using
the built in Calculator in Programmer mode makes this much easier. You
can simply click on the individual bits in the display to toggle them
on/off.***  
  
The tag 0x45 in binary is 01000101. We can clearly see that the 7th bit
is set which means that the element will contain content of some sort.
This could be a string or it could be other elements, we don't know yet.
If we ignore the 7th bit and just examine the first 6 bits (see my note
above about using the Windows Calculator for this) we can see that the
actual value is 5. Now, go look at
[MS-ASWBXML](http://msdn.microsoft.com/en-us/library/dd299442\(EXCHG.80\).aspx) section
[2.1.2.1.1](http://msdn.microsoft.com/en-us/library/ee218904\(EXCHG.80\).aspx) and
find the element with a token value of 0x05. See, I told you it was
there, it's the
<span style="font-family: courier new,courier;">\<Sync\></span>
element\!  
  
More information about the Tag Format can be found in the [WAP Binary
XML Content Format
version 1.2](http://www.w3.org/1999/06/NOTE-wbxml-19990624/) in the
section titled Tag Code Space.  
  
Follow the same process to decode the next 2 tokens 5C and 4F. Each one
of these also has the 7th bit set. The actual token values are 1C and
0F. The next 2 elements are
<span style="font-family: courier new,courier;">\<Collections\></span>
and
<span style="font-family: courier new,courier;">\<Collection\></span>.
So far, our completed XML looks like this.  
  
<span style="font-family: courier new,courier;">\<?xml version="1.0"
encoding="utf-8"?\></span>  
<span style="font-family: courier new,courier;">\<Sync
xmlns="AirSync"\></span>  
<span style="font-family: courier new,courier;">  
\<Collections\></span>  
<span style="font-family: courier new,courier;">     
\<Collection\></span>  
  
You might be wondering where the
<span style="font-family: courier new,courier;">xmlns="AirSync"</span>
part came from. Every ActiveSync element is defined in a code page, or
namespace. In order for the server to be able to interpret the XML, it
needs to know what code page the elements belong to. AirSync is the
default code page (or namespace) for ActiveSync requests and responses.
Unless you see an explicit code page switch (see SWITCH\_PAGE below)
after the String Table, this namespace definition is implied even though
its not encoded into the byte stream. It's not necessary to provide an
alias for the namespace. Any elements that are not prefixed with an
alias will be assumed to be part of the AirSync namespace.  
  
Decode the next token in the same fashion. 0x4B is 0x0B when you flip
the 7th bit. This is the
<span style="font-family: courier new,courier;">\<SyncKey\></span>
element.  
  
<span style="font-family: courier new,courier;">4B</span>  
<span style="font-family: courier new,courier;">\<SyncKey\></span>  
  
The next token is 0x03, which you won't find in any of the code pages.
This is because 0x03 is a global token. Global tokens are listed in
[MS-ASWBXML](http://msdn.microsoft.com/en-us/library/dd299442\(EXCHG.80\).aspx) section
[1.6](http://msdn.microsoft.com/en-us/library/ee159495\(EXCHG.80\).aspx) and
described in more detail in the [WAP Binary XML Content Format
version 1.2](http://www.w3.org/1999/06/NOTE-wbxml-19990624/)
specification. The token 0x03 represents an inline string with a
terminator (0x00). In this case the string is very short and only
consists of a single value, 0x30, followed by the terminator 0x00. The
string is encoded using the Character Set specified in the header, which
is UTF-8. UTF-8 is very similar to the ANSI character set and in this
case you can look up the character value in either. 0x30 is the
character '0'.  
  
The following token, 0x01, is another global token, END. This token is
generic in that it represents a closing tag for whatever the currently
'open' tag is, which is the
<span style="font-family: courier new,courier;">\<SyncKey\></span>
element. So, this 0x01 token represents
<span style="font-family: courier new,courier;">\</SyncKey\></span>. The
following is the entire sequence of bytes for the
<span style="font-family: courier new,courier;">\<SyncKey\></span>
element. Once you get used to looking at WBXML you'll start to notice
small patterns like this.  
  
<span style="font-family: courier new,courier;">4B 03 30 00
01</span>  
<span style="font-family: courier new,courier;">\<SyncKey\>0\</SyncKey\></span>  
  
Do you see the next pattern? Bytes 9-13 look like this: 52 03 32 00 01.
If you've been following along you might assume that the next chunk of
XML is going to look something like this
<span style="font-family: courier new,courier;">\<element\>string\</element\></span>,
and you would be right. 0x52 is actually 0x12 when you flip the 7th bit,
which is the
<span style="font-family: courier new,courier;">\<CollectionId\></span>
element. We know that 0x03 means the next bytes represent an inline
string followed by a terminator. Again, it’s only a single character,
0x32, which is the value '2' followed by the terminator 0x00, and the
END token, 0x01. These bytes represent the following XML segment.  
  
<span style="font-family: courier new,courier;">52 03 32 00
01</span>  
<span style="font-family: courier new,courier;">\<CollectionId\>2\</CollectionId\></span>  
  
You now have almost everything you need to decode the rest of the stream
with 1 exception so let's keep going. The next token is 0x57, which is
0x17 without the 7th bit set, which is the
<span style="font-family: courier new,courier;">\<Options\></span>
element. The token right after this is 0x00. What does that mean? It's
another global token, SWITCH\_PAGE. You might have been wondering why
all of the tokens we have looked up so far have been in section
[2.1.2.1.1](http://msdn.microsoft.com/en-us/library/ee218904\(EXCHG.80\).aspx) "Code
Page 0: AirSync". This is the default code page. When we encounter the
token 0x00 when not in the context of an inline string this tells us
that we are switching to a different code page and that any elements
that follow this switch should reference that code page. The byte that
follows the switch is 0x11. The list of Code Pages can be found in
section
[2.1.2.1](http://msdn.microsoft.com/en-us/library/ee219143\(EXCHG.80\).aspx).
However, remember that the token is a hexidecimal value and the code
pages are base 10 integers. This is important because the switch is
telling us to look at code page 17, not code page 11.  
  
<span style="font-family: courier new,courier;">00 11</span>  
<span style="font-family: courier new,courier;">SWITCH\_PAGE to Code
Page 17</span>  
  
The next token is 0x45, but remember that because of the code page
switch that we need to look at code page 17, AirSyncBase, which can be
found in section
[2.1.2.1.18](http://msdn.microsoft.com/en-us/library/ee202595\(v=exchg.80\).aspx).
Flipping the 7th bit gives us 0x05 which is
<span style="font-family: courier new,courier;">\<BodyPreference\></span>.  
  
<span style="font-family: courier new,courier;">45</span>  
<span style="font-family: courier new,courier;">\<airsyncbase:BodyPreference
xmlns:airsyncbase="AirSyncBase"\></span>  
  
There it is again\! I snuck in something that isn't actually in the byte
stream. Here again, defining the alias for the xml namespace is implied,
but not actually present in the byte stream. It actually doesn't matter
if we put it here or after the AirSync namespace in the Sync element,
however it MUST exist in one of these two places. Without the
<span style="font-family: courier new,courier;">xmlns:airsyncbase="AirSyncBase"</span>
definition we cannot use the
<span style="font-family: courier new,courier;">airsyncbase:</span>
prefix. We MUST prefix any elements contained in the AirSyncBase
namespace with the alias that we provided in the definition. If you
don’t, the server will look in the default namespace, which in this
case is AirSync and it will think the 0x45 token is a
<span style="font-family: courier new,courier;">\<Sync\></span> element
and you will get an error.  
  
The next token 0x46 would be 0x06,
<span style="font-family: courier new,courier;">\<Type\></span>, which
is then followed by an inline string as indicated by the 0x03 token. The
string consists of a single character 0x31, '1' followed by the
terminator and 0x01 token indicating the closing tag for the
<span style="font-family: courier new,courier;">\<Type\></span>
element.  
  
<span style="font-family: courier new,courier;">46 03 31 00
01</span>  
<span style="font-family: courier new,courier;">\<airsyncbase:Type\>1\</airsyncbase:Type\></span>  
  
The next pattern of bytes is very similar. 0x47, which is 0x07, or
<span style="font-family: courier new,courier;">\<TruncationSize\></span>,
followed by 0x03 indicating  an inline string, "32768", the string
terminator, and the closing element. 

<span style="font-family: courier new,courier;">47 03 33 32 37 36 38 00
01</span>  
<span style="font-family: courier new,courier;">\<airsyncbase:TruncationSize\>32768\</airsyncbase:TruncationSize\></span>  
  
Here we are at the home stretch and all we see is a few 0x01 tokens. If
we examine the complete XML that we have built so far those last 0x01
tokens will make sense.  
  
<span style="font-family: courier new,courier;">\<?xml version="1.0"
encoding="utf-8"?\></span>  
<span style="font-family: courier new,courier;">\<Sync
xmlns:"AirSync"\><span style="font-family: courier new,courier;">   </span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span></span>\<Collections\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"> </span><span style="font-family: courier new,courier;">   </span> 
</span>\<Collection\>  
</span><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>  
</span>\<SyncKey\>0\</SyncKey\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>  
</span>\<CollectionID\>2\</CollectionID\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>  
</span>\<Options\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>   </span>  
</span>\<airsyncbase:BodyPreference
xmlns:airsyncbase="AirSyncBase"\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>   </span>   </span>  
</span>\<airsyncbase:Type\>1\</airsyncbase:Type\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>   </span>   </span>  
</span>\<airsyncbase:TruncationSize\>32768\</airsyncbase:TruncationSize\>  
  
</span>All that is missing is a series of closing elements for
BodyPreference, Options, Collection, Collections, and Sync. Notice that
there are exactly (5) 0x01 tokens remaining in the stream. That is
exactly what those are, closing tags in that order. And you're done\!
The complete XML request should look like this. I've tried to highlight
some key areas so you can associate those with the bytes from the
stream.  
  
<span style="text-decoration: underline;">**Decoded
Request**</span>  
<span style="font-family: courier new,courier;"><span style="background-color: #ffff00;">03
01 6A</span> 00 45 5C 4F <span style="background-color: #00ff00;">4B 03
30 00 01</span> <span style="background-color: #00ccff;">52 03 32
00</span>  
<span>01</span> 57 00 11 45 <span style="background-color: #ffcc00;">46
03 31 00 01</span> <span style="background-color: #ff99cc;">47 03 33 32
37 36</span>  
<span style="background-color: #ff99cc;">38 00 01</span>
<span style="background-color: #ff0000;">01 01 01 01
01</span></span>  
  
<span style="font-family: courier new,courier; background-color: #ffff00;">\<?xml
version="1.0" encoding="utf-8"?\></span>  
<span style="font-family: courier new,courier;">\<Sync
xmlns:"AirSync"\></span>  
<span style="font-family: courier new,courier;">  
\<Collections\></span>  
<span style="font-family: courier new,courier;">     
\<Collection\></span>  
<span style="font-family: courier new,courier;">       
 <span><span style="background-color: #00ff00;">\<SyncKey\>0\</SyncKey\></span></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>  
<span style="background-color: #00ccff;">\<CollectionID\>2\</CollectionID\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>  
</span>\<Options\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>   </span>  
</span>\<airsyncbase:BodyPreference
xmlns:airsyncbase="AirSyncBase"\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>   </span>   </span>  
</span><span style="background-color: #ffcc00;">\<airsyncbase:Type\>1\</airsyncbase:Type\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>   </span>   </span>  
</span><span style="background-color: #ff99cc;">\<airsyncbase:TruncationSize\>32768\</airsyncbase:TruncationSize\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>   </span>  
</span><span style="background-color: #ff0000;">\</airsyncbase:BodyPreference\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>   </span>  
</span><span style="background-color: #ff0000;">\</Options\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">   </span>  
</span><span style="background-color: #ff0000;">\</Collection\></span></span>  
<span style="font-family: courier new,courier;">  
</span><span style="font-family: courier new,courier; background-color: #ff0000;">\</Collections\></span>  
<span style="font-family: courier new,courier; background-color: #ff0000;">\</Sync\></span>  
  
You should now have enough information to be able to decode the response
on your own. Try to decode it on your own. Check what you get against
the answer below.  
  
<span style="text-decoration: underline;">**Decoded
Response**</span>  
<span style="font-family: courier new,courier;"><span style="background-color: #ffff00;">03
01 6A</span> 00 45 5C 4F <span style="background-color: #00ff00;">4B 03
31 39 39 37 34 33 33</span>  
<span style="background-color: #00ff00;">30 37 34 00 01</span>
<span style="background-color: #00ccff;">52 03 32 00 01</span>
<span style="background-color: #ffcc00;">4E 03 31 00
</span><span><span style="background-color: #ffcc00;">01</span>
<span style="background-color: #ff0000;">01</span></span>  
<span style="background-color: #ff0000;">01
01</span></span>  
  
<span style="font-family: courier new,courier; background-color: #ffff00;">\<?xml
version="1.0" encoding="utf-8"?\></span>  
<span style="font-family: courier new,courier;">\<Sync
xmlns:"AirSync"\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span>\<Collections\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span>  
</span>\<Collection\></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span>   </span>  
</span><span style="background-color: #00ff00;">\<SyncKey\>1997433074\</SyncKey\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span>   </span>  
</span><span style="background-color: #00ccff;">\<CollectionID\>2\</CollectionID\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span>   </span>  
</span><span style="background-color: #ffcc00;">\<Status\>1\</Status\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span>  
</span><span style="background-color: #ff0000;">\</Collection\></span></span>  
<span style="font-family: courier new,courier;"><span style="font-family: courier new,courier;">  
</span><span style="background-color: #ff0000;">\</Collections\></span></span>  
<span style="font-family: courier new,courier; background-color: #ff0000;">\</Sync\></span>
