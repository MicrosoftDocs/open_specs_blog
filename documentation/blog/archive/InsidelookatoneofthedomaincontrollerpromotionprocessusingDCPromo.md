<div id="page">

# Inside look at one of the domain controller promotion process using DCPromo

[Hongwei
Sun-MSFT](https://social.msdn.microsoft.com/profile/Hongwei%20Sun-MSFT)
2/1/2010 12:15:00 PM

-----

<div id="content">

In order to add an additional domain controller to an existing domain,
DCPromo must be used to complete the task.  The process will install the
AD DS on the Windows Server, join the machine to the domain and
replicate all partitions from the existing domain controller.  The steps
for running DCPromo to promote a server to DC have been well
documented.  They can be done either through the Windows Interface for
user attended installation, or via an answer file for unattended
installation. <sup>\[1\]\[2\]</sup>   This blog will focus on what
really happens after the user input is taken and the real installation
starts.  Hopefully this can help readers have a better understanding of
domain controller installation process, so that they can have effective
debugging if any problem arises.

**<span class="underline">Preparing  for AD DS Installation</span>**

DCPromo starts by checking if the binaries for Active Directory Domain
Services have been installed. This should be done when the AD DS role is
added through Server Manager.

The process validates the supplied paths for NTDS, including path for
NTDS DIT path and System Volume Root Path (SYSVOL). The paths must be on
an NTFS volume and a fixed hard drive.

Time synchronization with the existing domain controller is forced.

Stops and configures the Netlogon Service on the server to be promoted.

Creates SYSVOL and copies the initial AD database to the installation
path.

Validates the user input parameter from the interface or answer file.

Verifies the existing DS environment to ensure:

  - The server name doesn't already exist in the existing site as a
    valid RDN
  - The site specified by user parameter or automatically discovered
    exists

Sets the credentials for the replication.

Configures the local machine (Windows server) for AD DS installation:

  - Sets DS configuration parameters in registry. The location is
    *System\\CurrentControlSet\\Services\\NTDS\\Parameters.* Some of the
    parameters including Root Domain, Configuration NC, etc. will be
    used in the next phase of installation.
  - Configures the event logging categories.

**<span class="underline">Starting the AD DS installation</span>**

  - Read DS configuration from the registry.
  - Create the NTDS setting object for the new domain controller in the
    existing remote DC. The object can be added using the DRSR RPC
    interface.
  - Start replicating Schema NC from the remote DC. After the
    replication is successfully done, the schema is reloaded.
  - Start replicating Configuration NC from remote DC.
  - Start replicating Domain NC from remote DC.
  - Convert the machine account to a domain controller machine account
    so it can be replicated in the next step
  - Do a domain NC Critical Only replication which replicates the
    system-critical objects only, as documented in 7.1.1.3
    MS-ADTS<sup>\[3\]</sup>
  - After replication of critical domain objects is completed, check to
    make sure there is no duplicated machine name in the updated DIT
    that has the same SAM account name. If so, it has to be renamed.
  - Update Replication Epoch on the NTDS setting object.
  - Set the computer's root domain name to the name of domain to join.
  - Delete intermediate registry keys used during installation.

Once the installation is complete, the machine will be required to
reboot in order to complete the entire installation.  Again the process
above is just for adding an additional domain controller to an existing
domain, which is important in some interoperability scenarios when
partners try to join a Windows domain controller into a non-Windows
domain.  We can leave the other domain controller promotion processes ,
such as creating the first domain controller in the forest, creating a
child domain or creating a Read-only DC to the future blogs.

**References**

\[1\]   "Steps for Installing AD DS",
<http://technet.microsoft.com/en-us/library/cc754438(WS.10).aspx>

\[2\]  "How to use unattended mode to install and remove Active
Directory Domain Services on Windows Server 2008-based domain
controllers", <http://support.microsoft.com/kb/947034>

\[3\]  "\[MS-ADTS\]: Active Directory Technical Specification",
<http://msdn.microsoft.com/en-us/library/cc223122(PROT.10).aspx>

</div>

</div>
