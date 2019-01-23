<div id="page">

# Incremental Change Synchronization

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
6/20/2011 11:29:57 AM

-----

<div id="content">

# Incremental Change Synchronization

ICS provides a means for the client to replicate changes in a folder or
hierarchy between two different points in time. This protocol is
actively used by Outlook to synchronize changes between the Exchange
server and the Outlook client. These changes include creating,
modifying, and deleting folders and messages. The way these changes are
synchronized is a game of hot potato between the client and the server.
A state is kept, commonly referred to as the ICS state.

## ICS State

The ICS state is a collection of 4 properties inside of an IDSET object.
 The four properties contained inside the object are as follows:

1.  PidTagIdsetGiven
2.  PidTagCnsetSeen
3.  PidTagCnsetSeenFAI
4.  PidTagCnsetRead

This object of 4 properties, or ICS state, is transferred back and forth
from client to server using a method that represents the IDSET data in a
serialized stream of binary data. The specification document
\[MS-OXCFXICS\] details how the IDSET object gets serialized into a
binary representation.

The state itself is maintained by the client. In this case, the server
does not retain the state. It relies on the client to upload its current
state, and then the synchronization occurs from that point forward.

## Obtain changes from the server

The client has to perform an ICS download to obtain the changes that are
currently on the server.

The process begins as follows:

1.  The client creates a download context.  
    (***RopSynchronizationConfigure***)
2.  The client uploads its state to the synchronization download
    context.  
    (***RopSynchornizationUploadStateStreamBegin****  
    **RopSynchronizationUploadStateStreamContinue**  
    **RopSynchronizationUploadStateStreamEnd***)
3.  The client gets the faster transfer stream (gets synchronization
    changes).  
    (***RopFastTransferSourceGetBuffer***)  
      
    Note: *This is a serialized representation of the changes in a
    folder or hierarchy as explained above.  
      
    *
4.  Release the synchronization object after downloading the entire
    stream.  
    (***RopRelease***)

You can also include all of this information in one RPC Call using
EcDoRpcExt2 (defined in ***\[MS-OXCRPC\]***).

## Push changes to the server

Once the client has changes that must be represented on the server, it
then uploads its current ICS state, and continues to perform any of the
relevant Import Rops against the server.

1.  Create synchronization upload context  
    (***RopSynchronizationOpenCollector***)
2.  Upload ICS state to upload context  
    (***RopSynchornizationUploadStateStreamBegin****  
    **RopSynchronizationUploadStateStreamContinue**  
    **RopSynchronizationUploadStateStreamEnd***)
3.  Client will then issue any one of the following operations to the
    upload context  
    (***RopSynchronizationImportMessageChange  
    RopSynchronizationImportMessageMove  
    RopSynchronizationImportDeletes  
    RopSynchronizationImportReadStateChanges  
    RopSynchronizationImportHierarchyChange  
    RopSynchronizationImportDeletes***)

## Bootstrap State (Empty State)

The bootstrap state is considered the initial state for a client that
has never synchronized with the server. Since the client has never
talked to the server, it does not have an ICS state. Therefore, it has
nothing to pass to the server to indicate the enumeration progress of
changes from the server. Thus, the client will upload something called
an empty state. This empty state is zero-length byte arrays for the
properties in question. This causes the server to send the current state
to the client. This first synchronization is obviously the longest
synchronization operation as it usually contains the most changes as the
client does not have any data yet.

*This is an overview of the ICS protocol; please stay tuned for further
blogs regarding the Rop specific details involved, and for a posting
about conflict detection.*

* *

</div>

</div>
