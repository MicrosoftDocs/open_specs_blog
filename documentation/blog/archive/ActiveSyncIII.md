<div id="page">

# ActiveSync III

[doms](https://social.msdn.microsoft.com/profile/doms) 4/24/2010 3:40:00
PM

-----

<div id="content">

# ActiveSync III

<span class="MsoSubtleEmphasis">*Dominic Salemno*</span>

## Introduction

In the last segment I went over some basic details regarding the
ActiveSync protocol. In this latest in a series, I want to touch on the
ability to Sync a folder.

## Initialization

Before the client grabs a list of folders from the server and syncs a
particular folder, the client must issue an ***Autodiscover*** command
followed by a ***Provision*** (*I will be explaining these in future*).

## Synchronization

After a client determines configuration and security policy settings,
the session should be in a state to allow the synchronization of data.
By issuing a ***FolderSync*** command with a ***SyncKey*** of ***0***,
we are telling the server to send us back a list of folders and their
associated synchronization key.

 

*Contained blow is a sample request to the server*:

 

**4.13.1.1<span style="mso-spacerun: yes">   </span>Request**

 

POST
/Microsoft-Server-

ActiveSync?Cmd=FolderSync\&User=deviceuser\&DeviceId=v140Device\&DeviceType=PPC
HTTP/1.1

Content-Type: application/vnd.ms-sync.wbxml

MS-ASProtocolVersion: 14.0

User-Agent: ASOM

Host: contoso.com

Content-Length: 13

Cache-Control: no-cache

Authorization: Basic YXJocnB6LWRvbVxzLmJvbGxlczpKJHAxdGVy

<span style="mso-spacerun: yes"> </span>

\<?xml version="1.0" encoding="utf-8"?\>

\<FolderSync<span style="mso-spacerun: yes"> 
</span>xmlns="FolderHierarchy:"\>

<span style="mso-spacerun: yes">  </span>\<SyncKey\>0\</SyncKey\>

\</FolderSync\>

 

*You’ll notice in our response we have each folder and associated ID
(inside of \<ServerId\>) in an Add element*:

****

 

**4.13.1.2<span style="mso-spacerun: yes">   </span>Response**

 

HTTP/1.1 200 OK

Content-Type: application/vnd.ms-sync.wbxml

X-MS-RP: 2.0,2.1,2.5,12.0,12.1,14.0

MS-ASProtocolVersions:
2.0,2.1,2.5,12.0,12.1,14.0

MS-ASProtocolCommands:

Sync,SendMail,SmartForward,SmartReply,GetAttachment,GetHierarchy,CreateCollection,DeleteColle

ction,MoveCollection,FolderSync,FolderCreate,FolderDelete,FolderUpdate,MoveItems,GetItemEstim

ate,MeetingResponse,Search,Settings,Ping,ItemOperations,Provision,ResolveRecipients,ValidateC

ert

Date: Wed, 01 Apr 2009 06:33:13 GMT

Content-Length: 346

<span style="mso-spacerun: yes"> </span>

\<?xml version="1.0" encoding="utf-8"?\>\<FolderSync
xmlns="FolderHierarchy:"\>

<span style="mso-spacerun: yes">  </span>\<Status\>1\</Status\>

<span style="mso-spacerun: yes">  </span>\<SyncKey\>1\</SyncKey\>

<span style="mso-spacerun: yes">  </span>\<Changes\>

<span style="mso-spacerun: yes">    </span>\<Count\>11\</Count\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>1\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Calendar\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>8\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>2\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Contacts\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>9\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>3\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">      </span>\<DisplayName\>Deleted
Items\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>4\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>4\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Drafts\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>3\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>5\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Inbox\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>2\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>6\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Journal\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>11\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>7\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">      </span>\<DisplayName\>Junk
E-Mail\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>12\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>8\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Notes\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>10\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>9\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Outbox\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>6\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>10\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">      </span>\<DisplayName\>Sent
Items\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>5\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">    </span>\<Add\>

<span style="mso-spacerun: yes">      </span>\<ServerId\>11\</ServerId\>

<span style="mso-spacerun: yes">      </span>\<ParentId\>0\</ParentId\>

<span style="mso-spacerun: yes">     
</span>\<DisplayName\>Tasks\</DisplayName\>

<span style="mso-spacerun: yes">      </span>\<Type\>7\</Type\>

<span style="mso-spacerun: yes">    </span>\</Add\>

<span style="mso-spacerun: yes">  </span>\</Changes\>

\</FolderSync\>

****

 

Now that we have our list, we can send an initial ***Sync*** to the
server with a ***SyncKey*** of *0* and the associated ***ServerId*** in
the ***\<CollectionId\>*** element. In our example below, we are
informing the server we only support the *JobTitle* and *Department*
properties (this is referred to as ***ghosting properties***). (Note: A
***Type*** value of *2* indicates we are synchronizing against
***Contacts***; please see *Page 42* of
[MS-ASCMD](http://msdn.microsoft.com/en-us/library/dd299441\(v=EXCHG.80\).aspx).)

****

 

**4.13.2.1<span style="mso-spacerun: yes">   </span>Request**

 

POST
/Microsoft-Server-ActiveSync?Cmd=Sync\&User=deviceuser\&DeviceId=v140Device\&DeviceType=PPC

HTTP/1.1

Content-Type: application/vnd.ms-sync.wbxml

MS-ASProtocolVersion: 14.0

User-Agent: ASOM

Host: exh-b-252

Content-Length: 20

Cache-Control: no-cache

Authorization: Basic YXJocnB6LWRvbVxzLmJvbGxlczpKJHAxdGVy

<span style="mso-spacerun: yes"> </span>

\<?xml version="1.0" encoding="utf-8"?\>

\<Sync xmlns="AirSync:" xmlns:A1="POOMCONTACTS:"\>

<span style="mso-spacerun: yes">  </span>\<Collections\>

<span style="mso-spacerun: yes">    </span>\<Collection\>

<span style="mso-spacerun: yes">      </span>\<SyncKey\>0\</SyncKey\>

<span style="mso-spacerun: yes">     
</span>\<CollectionId\>2\</CollectionId\>

<span style="mso-spacerun: yes">      </span>\<Supported\>

<span style="mso-spacerun: yes">        </span>\<A1:JobTitle/\>\>

<span style="mso-spacerun: yes">        </span>\<A1:Department/\>\>

<span style="mso-spacerun: yes">      </span>\</Supported\>

<span style="mso-spacerun: yes">    </span>\</Collection\>

<span style="mso-spacerun: yes">  </span>\</Collections\>

\</Sync\>

 

**4.13.2.2<span style="mso-spacerun: yes">   </span>Response**

 

HTTP/1.1 200 OK

Content-Type: application/vnd.ms-sync.wbxml

Date: Wed, 01 Apr 2009 06:35:02 GMT

Content-Length: 33

<span style="mso-spacerun: yes"> </span>

\<?xml version="1.0" encoding="utf-8"?\>\<Sync xmlns="AirSync:"\>

<span style="mso-spacerun: yes">  </span>\<Collections\>

<span style="mso-spacerun: yes">    </span>\<Collection\>

<span style="mso-spacerun: yes">     
</span>\<SyncKey\>878266863\</SyncKey\>

<span style="mso-spacerun: yes">     
</span>\<CollectionId\>2\</CollectionId\>

<span style="mso-spacerun: yes">      </span>\<Status\>1\</Status\>

<span style="mso-spacerun: yes">    </span>\</Collection\>

<span style="mso-spacerun: yes">  </span>\</Collections\>

\</Sync\>

 

In our response you will notice the ***SyncKey*** is different and we
have a ***Status*** of ***1***. Following
[MS-ASCMD](http://msdn.microsoft.com/en-us/library/dd299441\(v=EXCHG.80\).aspx)
(*ActiveSync Command Reference Protocol Specification*) on *Page 206*,
we can see that a ***Status*** value of ***1*** indicates that the
*server successfully completed the command*. The new value contained in
SyncKey is our initial value that we must use to get our initial set of
objects from the server. The initial ***SyncKey*** of ***0*** was merely
used to establish a relationship (think of building a session via
Syn/Ack). Now that this relationship has been established, we can use
our newly obtained ***SyncKey*** to synchronize against ***Contacts***:

 

**4.13.3.1<span style="mso-spacerun: yes">   </span>Request**

 

POST
/Microsoft-Server-ActiveSync?Cmd=Sync\&User=deviceuser\&DeviceId=v140Device\&DeviceType=PPC

HTTP/1.1

Content-Type: application/vnd.ms-sync.wbxmlMS-ASProtocolVersion: 14.0

User-Agent: ASOM

Host: exh-b-252

Content-Length: 20

Cache-Control: no-cache

Authorization: Basic YXJocnB6LWRvbVxzLmJvbGxlczpKJHAxdGV

<span style="mso-spacerun: yes"> </span>

\<?xml version="1.0" encoding="utf-8"?\>

\<Sync xmlns="AirSync:" xmlns:A1="POOMCONTACTS:"\>

<span style="mso-spacerun: yes">  </span>\<Collections\>

<span style="mso-spacerun: yes">    </span>\<Collection\>

<span style="mso-spacerun: yes">     
</span>\<SyncKey\>878266863\</SyncKey\>

<span style="mso-spacerun: yes">     
</span>\<CollectionId\>2\</CollectionId\>

<span style="mso-spacerun: yes">      </span>\<DeletesAsMoves/\>\>

<span style="mso-spacerun: yes">      </span>\<GetChanges/\>\>

<span style="mso-spacerun: yes">    </span>\</Collection\>

<span style="mso-spacerun: yes">  </span>\</Collections\>

\</Sync\>

 

After issuing our ***Sync*** with our ***SyncKey*** for this session,
the server will respond with the set of objects in question.

 

**4.13.3.2<span style="mso-spacerun: yes">   </span>Response**

 

HTTP/1.1 200 OK

Content-Type: application/vnd.ms-sync.wbxml

Date: Wed, 01 Apr 2009 06:38:34 GMT

Content-Length: 448

<span style="mso-spacerun: yes"> </span>

\<?xml version="1.0" encoding="utf-8"?\>\<Sync xmlns:A1="POOMCONTACTS:"

xmlns:A12="POOMCONTACTS2:" xmlns:A17="AirSyncBase:" xmlns="AirSync:"\>

<span style="mso-spacerun: yes">  </span>\<Collections\>

<span style="mso-spacerun: yes">    </span>\<Collection\>

<span style="mso-spacerun: yes">     
</span>\<SyncKey\>619052475\</SyncKey\>

<span style="mso-spacerun: yes">     
</span>\<CollectionId\>2\</CollectionId\>

<span style="mso-spacerun: yes">      </span>\<Status\>1\</Status\>

<span style="mso-spacerun: yes">      </span>\<Commands\>

<span style="mso-spacerun: yes">        </span>\<Add\>

<span style="mso-spacerun: yes">         
</span>\<ServerId\>2:1\</ServerId\>

<span style="mso-spacerun: yes">          </span>\<ApplicationData\>

<span style="mso-spacerun: yes">            </span>\<A17:Body\>

<span style="mso-spacerun: yes">             
</span>\<A17:Type\>1\</A17:Type\>

<span style="mso-spacerun: yes">             
</span>\<A17:EstimatedDataSize\>0\</A17:EstimatedDataSize\>

<span style="mso-spacerun: yes">             
</span>\<A17:Truncated\>1\</A17:Truncated\>

<span style="mso-spacerun: yes">            </span>\</A17:Body\>

<span style="mso-spacerun: yes">           
</span>\<A1:WebPage\>http://contoso.com\</A1:WebPage\>

<span style="mso-spacerun: yes">           
</span>\<A1:BusinessCountry\>USA\</A1:BusinessCountry\>

<span style="mso-spacerun: yes">           
</span>\<A1:Department\>Executive\</A1:Department\>

<span style="mso-spacerun: yes">           
</span>\<A1:Email1Address\>"president@contoso.com"

\&lt;president@contoso.com\&gt;\</A1:Email1Address\>

<span style="mso-spacerun: yes">            </span>\<A1:FileAs\>Hassall,
Mark\</A1:FileAs\>

<span style="mso-spacerun: yes">           
</span>\<A1:FirstName\>Mark\</A1:FirstName\>

<span style="mso-spacerun: yes">           
</span>\<A1:HomeCity\>Seattle\</A1:HomeCity\>

<span style="mso-spacerun: yes">           
</span>\<A1:HomeCountry\>USA\</A1:HomeCountry\>

<span style="mso-spacerun: yes">           
</span>\<A1:HomePhoneNumber\>(206) 555-0100\</A1:HomePhoneNumber\>

<span style="mso-spacerun: yes">           
</span>\<A1:HomePostalCode\>98000\</A1:HomePostalCode\>

<span style="mso-spacerun: yes">           
</span>\<A1:HomeState\>WA\</A1:HomeState\>

<span style="mso-spacerun: yes"> </span><span style="mso-spacerun: yes">          
</span>\<A1:HomeStreet\>234 Main Street\</A1:HomeStreet\>

<span style="mso-spacerun: yes">           
</span>\<A1:BusinessCity\>Seattle\</A1:BusinessCity\>

<span style="mso-spacerun: yes">           
</span>\<A1:MiddleName\>I\</A1:MiddleName\>

<span style="mso-spacerun: yes">           
</span>\<A1:MobilePhoneNumber\>(206) 555-0101\</A1:MobilePhoneNumber\>

<span style="mso-spacerun: yes">           
</span>\<A1:CompanyName\>Contoso Inc.\</A1:CompanyName\>

<span style="mso-spacerun: yes">           
</span>\<A1:BusinessPostalCode\>98000\</A1:BusinessPostalCode\>

<span style="mso-spacerun: yes">           
</span>\<A1:AssistantName\>Andy Jacobs\</A1:AssistantName\>

<span style="mso-spacerun: yes">           
</span>\<A1:AssistantTelephoneNumber\>(206)
555-0102\</A1:AssistantTelephoneNumber\>

<span style="mso-spacerun: yes">    
</span><span style="mso-spacerun: yes">       </span>\<A1:LastName\>Hassall\</A1:LastName\>

<span style="mso-spacerun: yes">           
</span>\<A1:BusinessState\>WA\</A1:BusinessState\>

<span style="mso-spacerun: yes">           
</span>\<A1:BusinessStreet\>123 Main Street\</A1:BusinessStreet\>

<span style="mso-spacerun: yes">           
</span>\<A1:BusinessPhoneNumber\>(206)
555-0103\</A1:BusinessPhoneNumber\>

<span style="mso-spacerun: yes">           
</span>\<A1:JobTitle\>President\</A1:JobTitle\>

<span style="mso-spacerun: yes">           
</span>\<A1:OfficeLocation\>TopFloor\</A1:OfficeLocation\>

<span style="mso-spacerun: yes">           
</span>\<A12:ManagerName\>Roya Asbari\</A12:ManagerName\>

<span style="mso-spacerun: yes">           
</span>\<A17:NativeBodyType\>1\</A17:NativeBodyType\>

<span style="mso-spacerun: yes">          </span>\</ApplicationData\>

<span style="mso-spacerun: yes">        </span>\</Add\>

<span style="mso-spacerun: yes">      </span>\</Commands\>

<span style="mso-spacerun: yes">    </span>\</Collection\>

<span style="mso-spacerun: yes">  </span>\</Collections\>

\</Sync\>

 

From this point on the client must always include the ***SyncKey***
received from the latest response from the server. For example, from
this response we will have to use a ***SyncKey*** of **619052475** as
this was the latest value returned to us from the server.

 

## Summary

Stay tuned for the next article which will explain the
***Autodiscover*** and ***Provision*** commands.

</div>

</div>
