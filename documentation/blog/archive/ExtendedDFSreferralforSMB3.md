<div id="page">

# Extended DFS referral for SMB 3

[Edgar A
Olougouna](https://social.msdn.microsoft.com/profile/Edgar%20A%20Olougouna)
2/21/2014 9:40:00 AM

-----

<div id="content">

This blog talks about site-aware DFS referral introduced in Windows
Server 2012. Extended DFS referrals provide remote client computers with
optimal DFS referrals when the computers connect to the corporate
network by using DirectAccess. This blog also describes how to configure
a Window 8 client to issue extended DFS referral request for testing a
SMB 3 server implementation.

## Feature summary

Windows Server 2012 added extended DFS referral which enables the
support of site awareness for Direct Access clients on Windows 8.

When a user on Windows 8 connects to Windows Server 2012 based DFS
namespaces over Direct Access, the client receives referrals to the
namespace servers and folder targets that are closest to their location.

In Windows 7, a remote computer (e.g. connected over Direct Access)
whose IP address is outside of the sites specified in Active Directory
would receive DFS referrals in a random order (from Windows Server 2008
R2 namespace server), including servers in distant sites. This may cause
network latency and increased bandwidth usage.

In Windows 8, Direct Access was enhanced with site awareness. When
accessing a DFS namespace over a Direct Access connection, the client
provides a site name in the DFS referral request to Windows Server 2012
namespace server. The server uses the site name to order the referrals
so that the client can get to the closest site available.

The extended DFS referral is sent using the new control code
FSCTL\_DFS\_GET\_REFERRALS\_EX \[MS-SMB2\].  It is important to note
that if an SMB 3.x server is DFS capable, it must support extended DFS
referral. All Windows-based SMB 3.x servers (2012 and 2012 R2) support
this new FSCTL.

## Windows 8 implementation-specific triggers

A Windows-based DFS client decides to use FSCTL\_DFS\_GET\_REFERRALS\_EX
if the computer is connected over Direct Access and the negotiated SMB
dialect is 3.0 or above.

The extended referral provisioning is done on Windows 8/8.1 by Direct
Access configuration.  
The DFS client detects that the client is connected over Direct Access
if UseDcLocatorSiteName \!= 0. Direct Access also provisions the
SiteName configuration. Recall that the value of the SiteName entry
takes precedence over the value of DynamicSiteName (which is dynamically
set by Netlogon service; the site that the computer belongs to can be
determined with DsGetSiteName API call). 

The following instructions may be practical for testing an SMB 3 server
implementation with extended DFS referral. The key is to get the client
issue the new FSCTL by setting the configuration that Direct Access
would have provisioned.  
When a Windows client requests for site-aware referrals, the server
should not fail with the request, even with STATUS\_NOT\_SUPPORTED,
otherwise the client will not be able to access the SMB 3 share, because
there is no fallback mechanism from FSCTL\_DFS\_GET\_REFERRALS\_EX to
FSCTL\_DFS\_GET\_REFERRALS.

## Windows-based extended DFS referrals testing

In my testing environment, I have a configuration similar to the
following.

DC01 is running Windows Server 2012 in this site MS-SMB\_Internal.  
DFS Management \\ Namespaces  
SiteName MS-SMB\_Internal  
Namespace
[\\\\contoso.com\\ShareVolume1](file://\\\\contoso.com\\ShareVolume1),
Add folder Share1 with target
[\\\\dc01\\FileShare2](file://\\\\dc01\\FileShare2)

On the Windows 8
client:  
HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Dfsc\\Parameters  
UseDcLocatorSiteName = 1  
This indicates to the DFS client that the client is connected over
“Direct
Access”.  
HKEY\_LOCAL\_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Netlogon\\Parameters  
SiteName MS-SMB\_Internal  
(SiteName is a string)  
NOTE: If the SiteName is not configured, the client will issue
FSCTL\_DFS\_GET\_REFERRALS.  
Restart Workstation service, and accept to restart Netlogon and Dfs
Namespace services.

With the proper configuration, the client will send site-aware referral
requests when asking for DFS referrals over SMB 3 connections.

\--- Site-aware DFS referral request example ---

\- SMB2: C   IOCTL (0xb), FID=0xFFFFFFFFFFFFFFFF
FSCTL\_DFS\_GET\_REFERRALS\_EX,  
  - CIoCtl:  
     StructureSize: 57 (0x39)  
     Reserved: 0 (0x0)  
     CtlCode: FSCTL\_DFS\_GET\_REFERRALS\_EX  
   + FileId: Persistent: 0xFFFFFFFFFFFFFFFF, Volatile:
0xFFFFFFFFFFFFFFFF  
     InputOffset: 120 (0x78)  
     InputCount: 97 (0x61)  
     MaxInputResponse: 0 (0x0)  
     OutputOffset: 120 (0x78)  
     OutputCount: 0 (0x0)  
     MaxOutputResponse: 4096 (0x1000)  
     Flags: (00000000000000000000000000000001) FSCTL request  
     Reserved2: 0 (0x0)  
   - ReqGetDFSReferralEx:  
      MaxReferralLevel: 4 (0x4)  
    - RequestFlags: 1 (0x1)  
       SiteName: (...............1) - SiteName present: The SiteName bit
MUST be set to 1 if the packet contains the site name of the client.  
       Unused:   (000000000000000.) - Unused  
      RequestDataLength: 88 (0x58)  
    - RequestData:  
       RequestFileNameLength: 52 (0x34)  
       RequestFileName: \\contoso.com\\ShareVolume1  
       SiteNameLength: 32 (0x20)  
       SiteName: MS-SMB\_Internal  
    padding: Binary Large Object (1 Bytes)

\--- Response example ---

\- SMB2: R   IOCTL (0xb) FSCTL\_DFS\_GET\_REFERRALS\_EX,  
  - RIoCtl:  
     StructureSize: 49 (0x31)  
     Reserved: 0 (0x0)  
     CtlCode: FSCTL\_DFS\_GET\_REFERRALS\_EX  
   + FileId: Persistent: 0xFFFFFFFFFFFFFFFF, Volatile:
0xFFFFFFFFFFFFFFFF  
     InputOffset: 112 (0x70)  
     InputCount: 0 (0x0)  
     OutputOffset: 112 (0x70)  
     OutputCount: 184 (0xB8)  
     Flags: 0 (0x0)  
     Reserved2: 0 (0x0)  
   - RespGetDFSReferral:  
      PathConsumed: 50 bytes  
      NumberOfReferrals: 1 (0x1)  
    + ReferralHeaderFlags: 3 (0x3)  
    - ReferralEntries: Version:4  
     - ReferralV4: Index:1 TTL:300 Seconds  
        VersionNumber: 0x4, MUST be set to 0x4  
        Size: 34 (0x22)  
        ServerType: Root targets returned  
      + ReferralEntryFlags: 4 (0x4)  
        TimeToLive: 300 Seconds  
        DfsPathOffset: 34 (0x22) Offset:0xD4  
        DfsAlternatePathOffset: 86 (0x56) Offset:0x108  
        NetworkAddressOffset: 138 (0x8A) Offset:0x13C  
        ServiceSiteGuid: {00000000-0000-0000-0000-000000000000}   
       DfsPath: Index:1 \\contoso.com\\ShareVolume1  
       DfsAlternatePath: Index:1 \\contoso.com\\ShareVolume1  
       TargetPath: Index:1 \\DC01\\ShareVolume1 

### References

What's New in DFS Namespaces and DFS Replication in Windows Server
2012  
<http://technet.microsoft.com/en-us/library/dn270370.aspx>

How DFS Works  
<http://technet.microsoft.com/en-us/library/cc782417(v=WS.10).aspx>

\[MS-DFSC\]: Distributed File System (DFS): Referral Protocol  
<http://msdn.microsoft.com/en-us/library/cc226982.aspx>  
\[MS-CIFS\]: Common Internet File System (CIFS) Protocol  
<http://msdn.microsoft.com/en-us/library/ee442092.aspx>  
\[MS-SMB2\]: Server Message Block (SMB) Protocol Versions 2 and 3  
<http://msdn.microsoft.com/en-us/library/cc246482.aspx>  
DirectAccess  
<http://technet.microsoft.com/en-us/windows/directaccess.aspx>  
SiteName  
<http://technet.microsoft.com/en-us/library/cc937923.aspx>  
DynamicSiteName  
<http://technet.microsoft.com/en-us/library/cc960209.aspx>

 

</div>

</div>
