<div id="page">

# Exchange ActiveSync Provisioning

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
6/9/2011 10:28:23 PM

-----

<div id="content">

Exchange ActiveSync

Provisioning

EAS Provisioning is a means

to download and apply devices on an activesync device.  

The provisioning process on a client will generally occur when the
server

requests  

that the provisioning is necessary. If a client specifically requests

provisioning, this  

will take extra bandwidth that is not needed, not only because
provisioning may

not  

be necessary. The provisioning process is a 4 phase process and is not
merely a

  

request/response 2-step process as are other activesync commands.

Let's see how this process

occurs:

A client wants to

synchornize the entire folder hierarchy (grab every folder in the
mailbox).  

The client will issue a FolderSync command with a SyncKey of 0 (sync
folder

hierarchy).  

Once this happens the server will kick back a status code of 449
indicating to

the client that  

the server does not accept the client's command because it feels as it
does not

have the  

latest policy. Therefore, the client will then issue a Provision
command. In

response, the  

server will kick back a response with a temporary SyncKey. The corporate
policy

is also provided along with this synckey. Once the client applies the
policies

requested by the  

server, the client will send back another Provision command with the
temporary

SyncKey  

in the request. This is a receipt that informs the server that we
accepted the

policies and have applied them. In response, the server sends the client
a

"final key" that will be used  

for all subsequent commincations. Thus, the client can reissue the
foldersync

command with  

the final sync key and all should work well.

Here is an overview of the

process:

1\. Client attempts command /

server responds with status code 449.  

2\. Client sends provision / server responds with temp key  

3\. Client sends temp key / server responds with final key  

4\. Client reissues failed command / server responds successfully  

  

It's also important to note that the synckey is not a guid of any kind,
it is a

unique random integer presented to the client as a string. It is up to
the

client to store this synckey after  

receiving the final key from the server. It \*will\* be necessary to
re-provision

again sometime  

in the near future, so the client has to be constantly looking out for
the

status code 449.

 

 

</div>

</div>
