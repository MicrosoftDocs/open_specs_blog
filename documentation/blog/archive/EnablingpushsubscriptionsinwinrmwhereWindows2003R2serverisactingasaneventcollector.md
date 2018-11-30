<div id="page">

# Enabling push subscriptions in winrm where Windows 2003 R2 server is acting as an event collector

[rguthrie-msft](https://social.msdn.microsoft.com/profile/rguthrie-msft)
4/17/2009 4:52:00 PM

-----

<div id="content">

Windows remote management is a technology that can be used to manage
windows servers. By installing the download
[ws-management](http://www.microsoft.com/downloads/details.aspx?FamilyID=845289ca-16cc-4c73-8934-dd46b5ed1d33&displaylang=en)
onto a windows 2003 r2 server, this server can act as an event
collector. This document describes the steps taken to enable windows
2003 r2 as an event collector. Two utilities will be utilized to enable
Windows 2003 r2 as a collector. These utilities are:

  - Winrm
  - Wecutil

It is important to note that some functionality of wecutil is not
available on Windows 2003 R2, most notably the qc command. This command
is not required though. Here are the steps taken to enable Windows 2003
R2 to act as an event collector. Further reading can be found in this
[blog
post](http://blogs.technet.com/otto/archive/2008/07/08/quick-and-dirty-enterprise-eventing-for-windows.aspx)
by Otto Helweg.

**Environment Configuration for sample**

**Source Computer**

· Windows 2003 R2 with latest service pack and patches

· WS-Management 1.1

**Collector Computer**

· Windows 2003 R2 with latest service pack and patches

· WS-Management 1.1

Installation and Configuration Steps.

1\. Install Windows 2003 R2 on collector computer.

2\. Install ws-management download onto both source and colletor
computers.

3\. Ensure Windows Firewall/Internet Connection Sharing (ICS) is started
(You can have the firewall turned off but the service needs to be
enabled for **winrm quickconfig** to
succeed.

[![pic1](images/pic1_thumb.png
"pic1")](images/pic1_2.png)

4\. Open a command prompt and run the command ***winrm quickconfig –q***
on both the source and collector machine. NOTE: This step is not needed
on the source machine if you were creating a collector initiated PULL
subscription.

5\. From the command prompt run the command ***gpedit.msc***, this will
allow you to edit the local security policy settings that affect the way
winrm functions. This should be done on both source and collector
machines.

[![pic2](images/pic2_thumb.png
"pic2")](images/pic2_2.png)

6\. Most important is to configure your authentication scheme and
trusted hosts settings under WinRM Client and your selected
authentication settings. Below is a table to help with determining an
authentication scheme.

7\.

**Type of Request**

</div>

</div>

**Authentication Scheme(s) Supported**

**Notes**

Source-To-Collector

Kerberos

If certificates are not configured on the source it will always use
Kerberos for event deliveries to the event collector. This requires that
the Source have a valid Kerberos ticket.

Source-To-Collector

Certificates

The Source may also use client certificates for source-initiated
subscriptions

Collector-To-Source

Basic

Digest

Negotiate (Kerberos first then NTLM)

Kerberos

Kerberos is the default authentication scheme in this request type but
alternatives can be configured in the event subscription for collector
initiated.

(Events sent from the source are always encrypted with either a Kerberos
Ticket or client
certificate)

[![pic3](images/pic3_thumb.png
"pic3")](images/pic3_2.png)

**Trusted host settings for winrm client configuration (The explain tab
has more detailed help on the configuration setting. NOTE: This is a
potential security risk if enabling all hosts to communicate with the
collector. This is only need on the collector machine if Kerberos
Authentication is not used to communicate with the source machine.)**

8\. Once you have configured winrm settings, you need to execute
**gpupdate /force** to cause the policy settings to be refreshed.

9\. Next, start winrm and wecsvc from the command line using the
following commands:

· ***net start winrm*** (on source\[Not needed for source initiated
subscription\] and collector)******

· ***net start wecsvc*** (on collector only)******

This will ensure both services are started and operational.

10\. Use the following command to validate the configuration winrm is
using:

· ***winrm get
winrm/config***

[![pic4](images/pic4_thumb.png
"pic4")](images/pic4_2.png)

The above configuration shows both client and server configuration. This
output should match what is stored in the policy database shown in
**step 5**.

11\. You can also validate that there is a valid listener on the
source/collector by executing the command ***winrm e
winrm/config/listener***. You should see output similar to the following
letting you know that the machine has a valid and operational listener
(Note the ListeningOn settings if running in on a multi-homed server):

**C:\\etw\>winrm e winrm/config/listener**

Listener \[Source="GPO"\]

Address = \*

Transport = HTTP

Port = 80

Hostname

Enabled = true

URLPrefix = wsman

CertificateThumbprint

**ListeningOn = 127.0.0.1, 172.29.14.8**

**12. Now that we have a valid configuration we need to create a
subscription file. This file is an xml file that contains the
subscription xml. Below is an example of a subscription with comments.**

\<Subscription
xmlns="[http://schemas.microsoft.com/2006/03/windows/events/subscription"](http://schemas.microsoft.com/2006/03/windows/events/subscription%22)\>  
    \<\!--Unique Id of subscription --\>  
    \<SubscriptionId\>Collection1\</SubscriptionId\>  
    \<\!--SourceInitiated or CollectorInitiated are  --\>  
    \<SubscriptionType\>CollectorInitiated\</SubscriptionType\>  
    \<\!-- Friendly description of what this subscription does--\>  
    \<Description\>Collector Initiated Subscription Test
1\</Description\>  
    \<\!-- Sets whether subscription is currently enabled/disabled--\>  
    \<Enabled\>true\</Enabled\>  
   
\<Uri\><http://schemas.microsoft.com/wbem/wsman/1/windows/EventLog>\</Uri\>  
    \<\!-- Use Normal (default), Custom, MinLatency, MinBandwidth --\>  
    \<ConfigurationMode\>Custom\</ConfigurationMode\>  
    \<\!-- Delivery mode can be either Push or Pull --\>  
    \<Delivery Mode="Push"\>  
        \<Batching\>  
            \<\!-- MaxItems and MaxLatencyTime determine how source will
batch items.   
            In this example it will wait for 20 events or 20 seconds
--\>  
            \<MaxItems\>20\</MaxItems\>  
            \<MaxLatencyTime\>20000\</MaxLatencyTime\>  
        \</Batching\>  
        \<PushSettings\>  
            \<\!-- Determines how often collector will check if source
is still online--\>  
            \<Heartbeat Interval="20000"/\>  
        \</PushSettings\>  
    \</Delivery\>  
    \<\!-- UTC time when subscription will expire--\>  
    \<Expires\>2018-01-01T00:00:00.000Z\</Expires\>  
    \<Query\>  
        \<\!\[CDATA\[  
            \<QueryList\>  
                \<Query Path="Application"\>  
                    \<Select\>\*\</Select\>  
                \</Query\>  
            \</QueryList\>  
        \]\]\>  
    \</Query\>  
    \<\!-- Determines if subscription will read existing events from
event source--\>  
    \<ReadExistingEvents\>false\</ReadExistingEvents\>  
    \<\!--Can be either http or https --\>  
    \<TransportName\>http\</TransportName\>  
    \<\!--Can be either RenderedText or Raw --\>  
    \<ContentFormat\>RenderedText\</ContentFormat\>  
    \<\!--Locale to be used --\>  
    \<Locale Language="en-US"/\>  
    \<\!--Windows event log to forward event to --\>  
    \<LogFile\>ForwardedEvents\</LogFile\>  
    \<\!--Valid values are Default, Basic, Negotiate, Digest --\>  
    \<CredentialsType\>Basic\</CredentialsType\>  
    \<\!--1..n event sources, determines source list for collector for
this subscription--\>  
    \<EventSources\>  
        \<EventSource Enabled="true"\>  
            \<Address\>LC1-12C28.DC162220.lab\</Address\>  
            \<\!-- This setting is overridded by wecutil if /cun
parameter is used.--\>  
            \<UserName\>administrator\</UserName\>  
        \</EventSource\>  
    \</EventSources\>  
\</Subscription\>

 

**13. Once you have a valid configuration file, you can setup the
subscription by executing the command:**

**wecutil cs ci.xml /cun:\<username\> /cup:\<password for username\>**

**14. If using a network capture utility you will see a request similar
to the following (Assumes allow unencrypted is set and using basic
authentication with http)**

**<span class="underline">REQUEST</span>**

POST /wsman HTTP/1.1  
Content-Type: application/soap+xml;charset=UTF-8  
User-Agent: Microsoft WinRM Client  
Host: LC1-12C28.DC162220.lab  
Content-Length: 2878  
Connection: Keep-Alive  
Authorization: Basic YWRtaW5pc3RyYXRvcjpyZ3V0aHJpZQ==

 

\<s:Envelope
xmlns:s="[http://www.w3.org/2003/05/soap-envelope"](http://www.w3.org/2003/05/soap-envelope%22)
xmlns:a="[http://schemas.xmlsoap.org/ws/2004/08/addressing"](http://schemas.xmlsoap.org/ws/2004/08/addressing%22)
xmlns:e="[http://schemas.xmlsoap.org/ws/2004/08/eventing"](http://schemas.xmlsoap.org/ws/2004/08/eventing%22)
xmlns:n="[http://schemas.xmlsoap.org/ws/2004/09/enumeration"](http://schemas.xmlsoap.org/ws/2004/09/enumeration%22)
xmlns:w="[http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd"](http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd%22)\>  
  \<s:Header\>  
   
\<a:To\>[http://LC1-12C28.DC162220.lab:80/wsman](http://lc1-12c28.dc162220.lab/wsman)\</a:To\>  
    \<w:ResourceURI
s:mustUnderstand="true"\><http://schemas.microsoft.com/wbem/wsman/1/windows/EventLog>\</w:ResourceURI\>  
    \<a:ReplyTo\>  
      \<a:Address
s:mustUnderstand="true"\><http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous>\</a:Address\>  
    \</a:ReplyTo\>  
    \<a:Action
s:mustUnderstand="true"\><http://schemas.xmlsoap.org/ws/2004/08/eventing/Subscribe>\</a:Action\>  
    \<w:MaxEnvelopeSize
s:mustUnderstand="true"\>153600\</w:MaxEnvelopeSize\>  
   
\<a:MessageID\>uuid:2711428A-D19A-42A4-98DF-079CFCD7ABD5\</a:MessageID\>  
    \<w:Locale xml:lang="en-US" s:mustUnderstand="false" /\>  
    \<w:OptionSet
xmlns:xsi="[http://www.w3.org/2001/XMLSchema-instance"](http://www.w3.org/2001/XMLSchema-instance%22)\>  
      \<w:Option Name="SubscriptionName"\>collection3\</w:Option\>  
      \<w:Option Name="Compression"\>SLDC\</w:Option\>  
      \<w:Option Name="CDATA" xsi:nil="true"/\>  
      \<w:Option Name="ContentFormat"\>RenderedText\</w:Option\>  
      \<w:Option Name="IgnoreChannelError" xsi:nil="true"/\>  
    \</w:OptionSet\>  
    \<w:OperationTimeout\>PT60.000S\</w:OperationTimeout\>  
  \</s:Header\>  
  \<s:Body\>  
    \<e:Subscribe\>  
      \<e:EndTo\>  
       
\<a:Address\>[HTTP://lc1-12c29.DC162220.lab:80/wsman/subscriptions/F494AF9C-3AE0-41BB-B502-1FA4DC4D7816](http://lc1-12c29.dc162220.lab/wsman/subscriptions/F494AF9C-3AE0-41BB-B502-1FA4DC4D7816)\</a:Address\>  
        \<a:ReferenceProperties\>  
         
\<e:Identifier\>882AF072-DB9F-49DC-AC3A-D851A537886C\</e:Identifier\>  
        \</a:ReferenceProperties\>  
      \</e:EndTo\>  
      \<e:Delivery
Mode="[http://schemas.dmtf.org/wbem/wsman/1/wsman/Events"](http://schemas.dmtf.org/wbem/wsman/1/wsman/Events%22)\>  
        \<w:Heartbeats\>PT20.000S\</w:Heartbeats\>  
        \<e:NotifyTo\>  
         
\<a:Address\>[HTTP://lc1-12c29.DC162220.lab:80/wsman/subscriptions/F494AF9C-3AE0-41BB-B502-1FA4DC4D7816](http://lc1-12c29.dc162220.lab/wsman/subscriptions/F494AF9C-3AE0-41BB-B502-1FA4DC4D7816)\</a:Address\>  
          \<a:ReferenceProperties\>  
           
\<e:Identifier\>882AF072-DB9F-49DC-AC3A-D851A537886C\</e:Identifier\>  
          \</a:ReferenceProperties\>  
          \<c:Policy
xmlns:c="[http://schemas.xmlsoap.org/ws/2002/12/policy"](http://schemas.xmlsoap.org/ws/2002/12/policy%22)
xmlns:auth="[http://schemas.microsoft.com/wbem/wsman/1/authentication"](http://schemas.microsoft.com/wbem/wsman/1/authentication%22)\>  
            \<c:ExactlyOne\>  
              \<c:All\>  
                \<auth:Authentication
Profile="[http://schemas.dmtf.org/wbem/wsman/1/wsman/secprofile/http/spnego-kerberos"](http://schemas.dmtf.org/wbem/wsman/1/wsman/secprofile/http/spnego-kerberos%22)\>\</auth:Authentication\>  
              \</c:All\>  
            \</c:ExactlyOne\>  
          \</c:Policy\>  
        \</e:NotifyTo\>  
        \<w:ConnectionRetry Total="5"\>PT60.0S\</w:ConnectionRetry\>  
        \<w:MaxElements\>20\</w:MaxElements\>  
        \<w:MaxTime\>PT20.000S\</w:MaxTime\>  
        \<w:MaxEnvelopeSize
Policy="Notify"\>153600\</w:MaxEnvelopeSize\>  
        \<w:Locale xml:lang="en-US"/\>  
        \<w:ContentEncoding\>UTF-8\</w:ContentEncoding\>  
      \</e:Delivery\>  
      \<e:Expires\>PT275480553.0S\</e:Expires\>  
      \<w:Filter
Dialect="[http://schemas.microsoft.com/win/2004/08/events/eventquery"](http://schemas.microsoft.com/win/2004/08/events/eventquery%22)\>  
        \&lt;QueryList\&gt;  
        \&lt;Query Path=\&quot;Application\&quot;\&gt;  
        \&lt;Select\&gt;\*\&lt;/Select\&gt;  
        \&lt;/Query\&gt;  
        \&lt;/QueryList\&gt;  
      \</w:Filter\>  
      \<w:SendBookmarks/\>  
    \</e:Subscribe\>  
  \</s:Body\>  
\</s:Envelope\>

 

**<span class="underline">RESPONSE</span>**

****

Content-Type: application/soap+xml;charset=UTF-8  
Server: Microsoft-HTTPAPI/1.0  
Date: Thu, 09 Apr 2009 13:37:26 GMT  
Content-Length: 958

 

\<s:Envelope xml:lang="en-US"
xmlns:s="[http://www.w3.org/2003/05/soap-envelope"](http://www.w3.org/2003/05/soap-envelope%22)
xmlns:a="[http://schemas.xmlsoap.org/ws/2004/08/addressing"](http://schemas.xmlsoap.org/ws/2004/08/addressing%22)
xmlns:e="[http://schemas.xmlsoap.org/ws/2004/08/eventing"](http://schemas.xmlsoap.org/ws/2004/08/eventing%22)
xmlns:n="[http://schemas.xmlsoap.org/ws/2004/09/enumeration"](http://schemas.xmlsoap.org/ws/2004/09/enumeration%22)
xmlns:w="[http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd"](http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd%22)\>  
  \<s:Header\>  
    \<a:Action 
\><http://schemas.xmlsoap.org/ws/2004/08/eventing/SubscribeResponse>\</a:Action\>  
    \<a:MessageID 
\>uuid:629302C2-3441-4459-A5DA-994ACFB40F32\</a:MessageID\>  
   
\<a:To\><http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous>\</a:To\>  
    \<a:RelatesTo 
\>uuid:2711428A-D19A-42A4-98DF-079CFCD7ABD5\</a:RelatesTo\>  
  \</s:Header\>  
  \<s:Body\>  
    \<e:SubscribeResponse\>  
      \<e:SubscriptionManager\>  
       
\<a:Address\>[http://LC1-12C28.DC162220.lab:80/wsman](http://lc1-12c28.dc162220.lab/wsman)\</a:Address\>  
        \<a:ReferenceProperties\>  
         
\<e:Identifier\>143DDA8B-85A6-4BCD-8427-6E409C178389\</e:Identifier\>  
        \</a:ReferenceProperties\>  
      \</e:SubscriptionManager\>  
      \<e:Expires\>PT0.0S\</e:Expires\>  
    \</e:SubscribeResponse\>  
  \</s:Body\>  
\</s:Envelope\>

 

 

 

**15. At this point the subscription should be enabled and events will
be processed. You can test this by going to the source computer and
using the eventcreate command as follows:**

***eventcreate /id 999 /t error /l application /d "Test event."***

**To remove the subscription you can use wecutil as follows:**

***wecutil ds \<subscriptionId\>***

**Here is the request/response generated over the network by this
command:**

**<span class="underline">REQUEST</span>**

POST /wsman HTTP/1.1  
Content-Type: application/soap+xml;charset=UTF-8  
User-Agent: Microsoft WinRM Client  
Host: LC1-12C28.DC162220.lab  
Content-Length: 1337  
Connection: Keep-Alive  
Authorization: Basic YWRtaW5pc3RyYXRvcjpyZ3V0aHJpZQ==

 

\<s:Envelope
xmlns:s="[http://www.w3.org/2003/05/soap-envelope"](http://www.w3.org/2003/05/soap-envelope%22)
xmlns:a="[http://schemas.xmlsoap.org/ws/2004/08/addressing"](http://schemas.xmlsoap.org/ws/2004/08/addressing%22)
xmlns:e="[http://schemas.xmlsoap.org/ws/2004/08/eventing"](http://schemas.xmlsoap.org/ws/2004/08/eventing%22)
xmlns:n="[http://schemas.xmlsoap.org/ws/2004/09/enumeration"](http://schemas.xmlsoap.org/ws/2004/09/enumeration%22)
xmlns:w="[http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd"](http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd%22)\>  
  \<s:Header\>  
   
\<a:To\>[http://LC1-12C28.DC162220.lab:80/wsman](http://lc1-12c28.dc162220.lab/wsman)\</a:To\>  
    \<a:ReplyTo\>  
      \<a:Address
s:mustUnderstand="true"\><http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous>\</a:Address\>  
    \</a:ReplyTo\>  
    \<a:Action
s:mustUnderstand="true"\><http://schemas.xmlsoap.org/ws/2004/08/eventing/Unsubscribe>\</a:Action\>  
    \<w:MaxEnvelopeSize
s:mustUnderstand="true"\>153600\</w:MaxEnvelopeSize\>  
   
\<a:MessageID\>uuid:D5A78823-3024-4589-87F0-BEBEE40BD0CC\</a:MessageID\>  
    \<w:Locale xml:lang="en-US" s:mustUnderstand="false" /\>  
    \<w:OptionSet
xmlns:xsi="[http://www.w3.org/2001/XMLSchema-instance"](http://www.w3.org/2001/XMLSchema-instance%22)\>  
      \<w:Option Name="SubscriptionName"\>collection3\</w:Option\>  
      \<w:Option Name="Compression"\>SLDC\</w:Option\>  
      \<w:Option Name="CDATA" xsi:nil="true"/\>  
      \<w:Option Name="ContentFormat"\>RenderedText\</w:Option\>  
      \<w:Option Name="IgnoreChannelError" xsi:nil="true"/\>  
    \</w:OptionSet\>  
    \<w:OperationTimeout\>PT5.000S\</w:OperationTimeout\>  
    \<e:Identifier
xmlns:e="[http://schemas.xmlsoap.org/ws/2004/08/eventing"](http://schemas.xmlsoap.org/ws/2004/08/eventing%22)\>143DDA8B-85A6-4BCD-8427-6E409C178389\</e:Identifier\>  
  \</s:Header\>  
  \<s:Body\>  
    \<e:Unsubscribe/\>  
  \</s:Body\>  
\</s:Envelope\>

 

**<span class="underline">RESPONSE</span>**

HTTP/1.1 200  
Content-Type: application/soap+xml;charset=UTF-8  
Server: Microsoft-HTTPAPI/1.0  
Date: Thu, 09 Apr 2009 13:38:48 GMT  
Content-Length: 668

 

\<s:Envelope xml:lang="en-US"
xmlns:s="[http://www.w3.org/2003/05/soap-envelope"](http://www.w3.org/2003/05/soap-envelope%22)
xmlns:a="[http://schemas.xmlsoap.org/ws/2004/08/addressing"](http://schemas.xmlsoap.org/ws/2004/08/addressing%22)
xmlns:e="[http://schemas.xmlsoap.org/ws/2004/08/eventing"](http://schemas.xmlsoap.org/ws/2004/08/eventing%22)
xmlns:n="[http://schemas.xmlsoap.org/ws/2004/09/enumeration"](http://schemas.xmlsoap.org/ws/2004/09/enumeration%22)
xmlns:w="[http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd"](http://schemas.dmtf.org/wbem/wsman/1/wsman.xsd%22)\>  
  \<s:Header\>  
    \<a:Action 
\><http://schemas.xmlsoap.org/ws/2004/08/eventing/UnsubscribeResponse>\</a:Action\>  
    \<a:MessageID 
\>uuid:61B7D784-99C3-498C-9392-6765B09DDE2E\</a:MessageID\>  
   
\<a:To\><http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous>\</a:To\>  
    \<a:RelatesTo 
\>uuid:D5A78823-3024-4589-87F0-BEBEE40BD0CC\</a:RelatesTo\>  
  \</s:Header\>  
  \<s:Body\>\</s:Body\>  
\</s:Envelope\>

 

**This completes the steps required to enabled/disable a subscription.
Attached is a network trace that shows how the calls would look across
the network. The event data will always be encrypted by either the
Kerberos ticket or certificate depending on how you have configured your
environment.**

#### How to check the events forwarded to collector

Once you have created a subscription, check the even log location you
specified in your subscription file. In the case of this sample, we
specified the forwarded events log. The screenshot below shows an
example of the forwarded
event.

[![pic5](images/pic5_thumb.png
"pic5")](images/pic5_2.png)

#### 

#### 

#### Additional Reading and References

<http://blogs.technet.com/otto/archive/2008/07/08/quick-and-dirty-enterprise-eventing-for-windows.aspx>

<http://technet.microsoft.com/en-us/library/cc782312.aspx>

<http://www.microsoft.com/downloads/details.aspx?FamilyID=845289ca-16cc-4c73-8934-dd46b5ed1d33&displaylang=en>

[attachments.zip](images/46/16/attachments.zip)
