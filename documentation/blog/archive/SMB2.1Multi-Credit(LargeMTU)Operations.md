<div id="page">

# SMB 2.1 Multi-Credit (Large MTU) Operations

[Bill Wesse](https://social.msdn.microsoft.com/profile/Bill%20Wesse)
6/22/2009 11:40:00 AM

-----

<div id="content">

One design goal for the SMB 2.1 protocol implementation on Windows 2008
R2 and Windows 7 was to achieve better performance for 10 Gigabit
Ethernet (very high speed / low latency networks). This has been
achieved with a new feature called ‘Large MTU’, or ‘multi-credit’
operations. The maximum transmission unit (MTU), of course, is the size
(in bytes) of the largest protocol data unit that a communication
protocol can pass across the network.

This blog entry is informative, and should not be considered normative
in any way, as it is preliminary to the release of Windows 2008 R2 and
Windows 7.

Portions of this entry have been adapted from the “\[MS-SMB 2\]: Server
Message Block (SMB) Version 2 Protocol Specification”. A future update
of that document will provide normative documentation concerning the
material introduced here.

To access the current document release, search the Microsoft MSDN
website (<http://msdn.microsoft.com/>) for “Server Message Block (SMB)
Version 2 Protocol”. If you wish to receive information about protocol
documentation updates, you can subscribe to the Microsoft ‘Protocols
Perspective e-Newsletter’ at Microsoft Open Protocols
(<http://www.microsoft.com/protocols/>).

There are five files attached to the ‘SMB2-Multi-Credit.zip’ attachment
to this
entry.:

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">1.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>BothEnabled.cap – this is a Network Monitor 3.3
capture showing two Windows 7 systems using multi-credit operations on a
file
read.

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">2.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>ClientDisabled.cap – this is a Network Monitor 3.3
capture showing the same file read operation with multi-credit
operations
disabled.

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">3.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>3.3.1625.0.smb2.npl – this is a modified Network
Monitor 3.3 parser for SMB 2, with updates for fields involved with
multi-credit
operations.

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">4.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>3.4.1761.1.smb2.npl – this is a modified Network
Monitor 3.3 parser for SMB 2, with updates for fields involved with
multi-credit
operations.

<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin"><span style="mso-list: Ignore">5.<span style="FONT: 7pt &#39;Times New Roman&#39;">      
</span></span></span>SMB2-Multi-Credit.pdf – the full content for this
blog
entry.

 

[SMB2-Multi-Credit.zip](images/SMB2-Multi-Credit.zip)

</div>

</div>
