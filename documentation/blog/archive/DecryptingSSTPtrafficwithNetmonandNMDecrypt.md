<div id="page">

# Decrypting SSTP traffic with Netmon and NMDecrypt

[Sebastian
Canevari](https://social.msdn.microsoft.com/profile/Sebastian%20Canevari)
5/23/2011 1:11:00 PM

-----

<div id="content">

**<span style="text-decoration: underline;">Intro</span>**

I have recently received some inquiries about
[\[MS-SSTP\]](http://msdn.microsoft.com/en-us/library/cc247338\(v=prot.10\).aspx).

I must admit that I had to review most of the information about this
“fairly” newly introduced VPN protocol (it was first shipped with
Windows Vista) since I hadn’t been working with it for a while.

For those that are just learning about the existence of this protocol, a
brief description of SSTP would be:

*“SSTP (Secure Socket Tunneling Protocol) allows encapsulating PPP
(Point to Point Protocol) packets into a secure HTTPS connection.”*

There are many articles that help understanding how to set up this
environment so I won’t be jumping into details about this task. Instead
I’ll be just pointing to the most relevant resources:

\-          SSTP Remote Access Step-by-Step Guide: Deployment:
<http://technet.microsoft.com/en-us/library/cc731352(WS.10).aspx>

\-          How an IIS Web server and a Secure Socket Tunneling Protocol
(SSTP)-based Routing and Remote Access server can co-exist on a Windows
Server 2008-based server: <http://support.microsoft.com/kb/947026/en-us>

\-          How to troubleshoot Secure Socket Tunneling Protocol
(SSTP)-based connection failures in Windows Server 2008:
<http://support.microsoft.com/kb/947031>

**<span style="text-decoration: underline;">What’s needed?</span>**

In order to be able to decrypt SSTP traffic with Network Monitor, the
following are needed:

<span style="text-decoration: underline;">Network Monitor:</span>  Its
latest version can be downloaded from
<http://support.microsoft.com/?kbid=933741>. That article points to the
download site (for both x86 and x64) and to related information.

<span style="text-decoration: underline;">NMDecrypt (Network Monitor
Decryption Expert): </span>This expert can be downloaded from
<http://nmdecrypt.codeplex.com/>. Once downloaded, it needs to be
installed. Note that there are x86 and x64 versions as well.

<span style="text-decoration: underline;">The server certificate /
private key in one of the following formats:</span>

  - Binary [PKCS12](http://en.wikipedia.org/wiki/PKCS) (PFX) IIS
    certificate (and the password used when exported)  
    The steps to do this from the Windows MMC can be found on
    NMDecrypt’s help
  - PEM (text-based) private certificate
  - .key file (contains the private key of the server in text format)

<span style="text-decoration: underline;">A network capture file (.cap)
containing the SSTP traffic :</span> It must include the initial SSL
challenge response. In some cases, if the capture was taken with a later
version of Netmon than the one where NMDecrypt is installed, the
analysis may fail, so it is sometimes a time saver for the capture to be
taken with the same version of Netmon that is being used with
NMDecrypt.  
Also, if at all possible, the capture should contain just the TCP
conversation related to the SSTP traffic. If the capture contains
multiple conversations, it is a time saver to select the appropriate TCP
conversation once opened in Netmon and then running the expert.  
This is an example of what you should
see:

[![](images/sstpdecrypt1/8371.conversation.jpg)

**<span style="text-decoration: underline;">How to make magic
happen?</span>**

Once Netmon is open, NMDecrypt is run by following these steps:

1.  Click on the Experts
    menu:  
      
    [![](images/sstpdecrypt1/0028.step1.jpg)  
      
2.  Select the correct expert (NMDecrypt) and then select “Run
    Expert”:  
      
    [![](images/sstpdecrypt1/1512.step2.jpg)  
      
3.  Complete the requested information:
    1.  The path to the encrypted capture
    2.  The path to the server certificate
    3.  The password for the .pfx file (if this is the format for the
        certificate)
    4.  The path to the resultant decrypted file
    5.  The path to the log
        file  
          
        [![](images/sstpdecrypt1/4667.step3.jpg)  
          
    6.  Once the fields are completed, press Start.  
          
    7.  Magic will happen during a couple minutes (execution time
        depends on the size of the capture) and a new instance of Netmon
        will pop up with the decrypted capture. Things could go wrong
        while decrypting in which case the descriptive errors will be
        logged in the resultant log file.  
          
    8.  For comparison purposes, here are screenshots of how some frames
        look like before and after
            decryption:  
          
        1.  Before:  
              
            [![](images/sstpdecrypt1/1172.step6before.jpg)  
              
        2.  After:  
              
            [![](images/sstpdecrypt1/5488.step6after.jpg)  
              
            As you may already have noticed, the SSL Application Data is
            mostly broken down. However, most SSTP messages may not be
            available at first sight.  
              
        3.  If the SSTP packets of interest are not viewable at first
            sight, some HTTP payload needs to be decoded as SSTP. In
            order to do that:  
              
            1.  Select an HTTP
                frame:  
                  
                [![](images/sstpdecrypt1/3386step7a.jpg)
            2.  On the Frame Details section select the HTTPPayloadLine
                property:  
                  
                  
                [![](images/sstpdecrypt1/8055.step7b.jpg)
            3.  On the Hex Details section, the raw data corresponding
                to that property will be
                highlighted:  
                  
                [![](images/sstpdecrypt1/7043.step7c.jpg)  
                  
            4.  Click on the Decode As button (right above the raw
                data):  
                  
                [![](images/sstpdecrypt1/1754.step7d.jpg)
            5.  The Decode As window will pop up, select SSTP under the
                Protocols
                branch:  
                  
                [![](images/sstpdecrypt1/6433.step7e.jpg)
            6.  Click OK and a new Decoded Frame window with the decoded
                frame will pop
                up:  
                  
                [![](images/sstpdecrypt1/1323.step7f.jpg)

I hope that you find this information useful.

If you want to expand on NMDecrypt and Netmon, you can take a look at
[this](http://blogs.technet.com/b/netmon/) blog.

Until next time\!  
  
  

</div>

</div>
