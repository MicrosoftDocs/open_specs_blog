<div id="page">

# Free/Busy Data in Exchange

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
6/30/2011 12:15:00 PM

-----

<div id="content">

<span style="font-size: 12px; font-weight: normal;">In today’s
fast-paced market, availability of an individual is increasingly
important. Thus presents the need to be able to schedule an
appointment/meeting with a person even if they are not available.
Exchange provides such a service by storing what is referred to as
free/busy data. This information will indicate what requested time range
is free, busy, and tentative for a particular user. It will also provide
any out of office (oof) status as well.  
</span>

### <span style="color: #666699;">Web Services Method</span>

Free/Busy data can be obtained using the Availability service provided
by the Exchange Server. This is simply a web service which obtains the
free/busy data directly from a user’s calendar. Specifically, the
GetUserAvailability operation will provide the free/busy state at
specific level of detail. ****** The response to this operation will
vary. This will depend on the permissions granted to the user requesting
the availability. If the user in question does have access to perform
the search you will get a ResponseCode of “*NoError*” indicating
success. When a user requests data through web services this data is
actually obtained from the specified user’s calendar in question.

Due to the nature of the data in question being held in each
individual’s calendar, modifying free/busy data is simply done by
creating calendar items using the CreateItem operation or modifying the
items using UpdateItem operation. Thus, any subsequent calls to
GetUserAvailability will show the updated information back to the
requesting party.

### <span style="color: #666699;">Public Folders-Based Method</span>

In Exchange Server versions pre-2010, Exchange stored free/busy data in
the public folders store. Inside of the **NON\_IPM\_SUBTREE\\SCHEDULE+
FREE BUSY** folder in the public folders store there will be a message
object for each user. The free/busy data in question is represented by a
set of properties that are set on that user’s message object in this
folder. The message object is referred to as the “free/busy message”.
There MUST be only one free/busy message per user. The subject of the
message (PidTagNormalizedSubject property) is ultimately what is used in
uniquely identifying a user’s free/busy message. The
PidTagNormalizedSubject property will be in the form of
“USER-/CN=RECIPIENTS/CN=CRAZYIVAN”.

Finding free/busy information for a specific user in the public
folders-based method requires a series of remote operations (rops) to
traverse the public folders store, and ultimately using the RopFindRow
request looking for the data in question in the PidTagNormalizedSubject
property. This procedure is shown in more detail in Section 4.2 of
\[MS-OXOPFFB\].

All users have access to the public folders store. Thus, a client can
read and write free/busy data to another user’s free/busy message other
than the user who has logged in. The client will issue a series of rops
with the client ultimately issuing a RopSetProperties to transmit the
availability data back to the server.

**  
<span style="color: #ff0000;">Please note the
following</span>**<span style="color: #ff0000;">: The public
folders-based protocol is no longer a feature in Exchange Server 2010.
This has been removed and the user must use Exchange Web
Services.</span>

***Availability Web Service Protocol Specification References***:  
Information for the availability service is provided via our open
specifications document: \[MS-OXWAVLS\].  
Information for the security model used in the availability service,
please reference the following document: \[MS-OXCPERM\].

***Public Folder-Based Free/Busy Protocol Specification References***:  
Information for the public folder-based free/busy Protocol is provided
by the specification document: \[MS-OXOPFFB\].

 

</div>

</div>
