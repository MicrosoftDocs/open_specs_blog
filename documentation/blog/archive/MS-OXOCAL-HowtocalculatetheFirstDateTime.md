<div id="page">

# MS-OXOCAL - How to calculate the FirstDateTime for monthly and yearly recurring appointments for the Hebrew calendar.

[JCurry](https://social.msdn.microsoft.com/profile/JCurry) 7/28/2011
1:33:00
PM

-----

<div id="content">

<span style="color: #3366ff; text-decoration: underline;">**<span style="font-size: large;">Alternate
Calendars</span>**</span>

<span style="font-size: small;">As you may or may not be aware, Outlook
supports multiple calendars. Not only does it support multiple
calendars, it also supports alternate calendar types. Some languages
that enable alternate calendars are Arabic, English, Hebrew, Hindi,
Chinese, Japanese, Korean, and Thai.</span>

<span style="font-size: small;">In Outlook 2010 you can display an
alternate calendar by performing the following steps.</span>

1.  <span style="font-size: small;">Select File -\> Options to open the
    options dialog.</span>
2.  <span style="font-size: small;">Select the Calendar option on the
    left.</span>
3.  <span style="font-size: small;">Under Calendar Options, check the
    box 'Enable an alternate calendar' to enable the alternate calendar
    drop down list boxes.</span>
4.  <span style="font-size: small;">Select the language and calendar
    type you want to use.</span>
5.  <span style="font-size: small;">Click OK.</span>

<span style="font-size: small;">When you view your calendar now you will
see the date values displayed in the language you selected next to the
date as it appears normally. So far, nothing has really changed other
than how your calendar looks.</span>

<span style="font-size: small;">What happens if you want to create a
monthly or yearly recurring appointment and you work with alternate
calendars? You might ask yourself, which calendar should the appointment
follow? How do you tell it which one to use? If you're using Outlook
2010, go ahead and try it. When you get to the Appointment Recurrence
dialog everything looks normal until you select either the Monthly or
Yearly recurrence option. When you select one of those options, some
strange things happen that you might not have expected. You now have a
choice of which </span><span style="font-size: small;">calendar to use
and when you switch between these options you will also see the values
in a few of the drop down change to that language as well.</span>

<span style="font-size: small;">Did you know that not every calendar
type has the same number of days in a month, or months in a year? As a
test, create a yearly recurring appointment that starts on the same day
for 2 different calendar types. I created one that starts on July 26th
2011 for both the Gregorian and Hebrew Lunar calendars. When I go look
at July 26th 2012 I only see one of the yearly recurring appointments,
the one for the Gregorian calendar. The one that I created for the
Hebrew Lunar calendar actually appears on July 14th
2012.</span>

 

<span style="text-decoration: underline;">**<span style="color: #3366ff; font-size: large; text-decoration: underline;">How
does this affect me as a developer?</span>**</span>

<span style="font-size: small;">Why is any of this important? If you are
a developer implementing a calendaring system that synchronizes with
Exchange Server you MUST be aware of this and take the calendar type
into account when creating monthly or yearly recurring
appointments.</span>

<span style="font-size: small;">Most of the properties of the
[RecurrrencePattern](http://msdn.microsoft.com/en-us/library/ee203303\(EXCHG.80\).aspx) structure
are fairly straight forward. However, there is one in particular that
will cause you some grief if you are creating monthly or yearly
recurring appointments, FirstDateTime.</span>

<span style="font-size: small;">The documentation provides the following
description for calculating this value:</span>

1.  <span style="font-size: small;">Find the first day of the month of
    StartDate.</span>
2.  <span style="font-size: small;">Determine MinimumDate. For
    Gregorian calendars, this is midnight, January 1, 1601.For
    non-Gregorian calendars, this is the first day of the calendar's
    year that falls in the Gregorian year of 1601. For example, if the
    CalendarType is CAL\_HEBREW, the first day of that calendar's year
    that falls in the Gregorian year of 1601 is 1/1/5362, which is the
    Gregorian date of 9/27/1601.</span>
3.  <span style="font-size: small;">Calculate the number of
    calendar months between midnight of the days calculated in step 1
    and step 2.</span>
4.  <span style="font-size: small;">Take that value modulo
    Period.</span>
5.  <span style="font-size: small;">Add that number of months to
    the MinimumDate, as determined in step 2.</span>
6.  <span style="font-size: small;">Calculate the number of
    minutes between midnight that day and midnight, January 1,
    1601.</span>

 

<span style="font-size: large; text-decoration: underline;">**<span style="color: #3366ff; text-decoration: underline;">Example</span>**</span>

<span style="font-size: small;">Let's walk through these steps and try
to calculate the FirstDateTime for the July 26th appointment that we
created for the Hebrew calendar. Along the way, we'll discover why the
appointment shows up on July 14th 2012.</span>

<span style="font-size: small;">Find the first day of the month
of StartDate.</span>

1.  <span style="font-size: small;">This is simple enough. Our start
    date is 7/26/2011 so the fist day of the month would be
    <span style="background-color: #ffff00;">**7/1/2011**</span></span>

<span style="font-size: small;"><span style="background-color: #ffff00;"></span></span><span style="font-size: small;">Determine
MinimumDate. For Gregorian calendars, this is midnight, January 1, 1601.
For non-Gregorian calendars, this is the first day of the calendar's
year that falls in the Gregorian year of 1601. For example, if the
CalendarType is CAL\_HEBREW, the first day of that calendar's year that
falls in the Gregorian year of 1601 is 1/1/5362, which is the Gregorian
date of 9/27/1601.</span>

1.  <span style="font-size: small;">This is the first step that will
    usually cause confusion. Luckily, the documentation gives us the
    value we need for this one. Why do you think I choose to work
    with the Hebrew calendar to start with?</span>
2.  <span style="font-size: small;"> Minimum Date =
    <span style="background-color: #ffff00;">**9/27/1601**</span>.</span>

<span style="font-size: small;">Calculate the number of calendar months
between midnight of the days calculated in step 1 and step 2.</span>

1.  <span style="font-size: small;">There are a few different ways that
    you could calculate this.  Here's the code I used to calculate
    it. FirstDay is the value that we got in step 1 and MinDate is the
    value we got in step 2.</span>
2.  <span style="font-size: small;">Months = ((FirstDay.Year -
    MinDate.Year) \* 12) + (FirstDay.Month - MinDate.Month) =
    <span style="background-color: #ffff00;">**4918**</span></span>

<span style="font-size: small;">Take that value modulo Period.</span>

1.  <span style="font-size: small;">Just in case you forgot your high
    school math, Modulo, or Modulus, means the remainder from a division
    operation. In our scenario the Period value is 12.</span>
2.  <span style="font-size: small;">4918 MOD 12 =
    <span style="background-color: #ffff00;">**10**</span>.</span>

<span style="font-size: small;">Add that number of months to
the MinimumDate, as determined in step 2.</span>

1.  <span style="font-size: small;">This is the second step that will
    cause confusion and unfortunately the documentation doesn’t give us
    the answer this time. You can't just add the value from step 4 to
    the month value from step 2 and add a year if the new date goes past
    December. If you know anything about the Hebrew calendar you
    might know that a year can have 12 or 13 months, that a month can
    have 29 or 30 days, and that the lunar year has approximately 11
    days less than the solar year. So, what do we do? Because the Hebrew
    calendar is based on the lunar cycle we will use lunar months
    instead of calendar months. A lunar month is 29.53 days. Take our
    result from step 4, which is 10, and multiply that by the number of
    days in a lunar month and you get 295.3 days. Drop the decimal and
    add that many days to the Minimum Date.</span>
2.  <span style="font-size: small;">9/27/1601 + 295 days =
    **<span style="background-color: #ffff00;">7/19/1602</span>**.</span>

<span style="font-size: small;">Calculate the number of minutes between
midnight that day and midnight, January 1, 1601.</span>

1.  <span style="font-size: small;">First we need to figure out how many
    days will pass between those dates. 7/19/1602 - 1/1/1601 = 564
    days.</span>
2.  <span style="font-size: small;">Then you can multiply by 1440
    minutes in a day to get the final answer. 564 \* 1440 =
    <span style="background-color: #ffff00;">**812160**</span>.</span>

<span style="font-size: small;">Using a tool like
[MFCMAPI](http://mfcmapi.codeplex.com/), you can verify that this is
correct. So, if you were a developer building a calendaring system that
supported the Hebrew calendar and you need to sync with Exchange Server,
that is how you would calculate the FirstDateTime field when you
populate the
[RecurrencePattern](http://msdn.microsoft.com/en-us/library/ee203303\(EXCHG.80\).aspx) structure.</span>

 

<span style="text-decoration: underline;">**<span style="color: #3366ff; font-size: large; text-decoration: underline;">Conclusion</span>**</span>

<span style="font-size: small;">So, can you figure out why the
appointment we created on July 26th 2011 shows up next year on July 14th
2012? Go back to step 5 where we talk about the importance of the lunar
month to the Hebrew calendar. The next occurrence of the appointment
will be 1 year from July 26th 2011. But the lunar year has approximately
11 fewer days than the solar year. July 26 - 11 days  = July 15. Well,
that's not right. What's going on?  Don't forget that 2012 is a leap
year, so there's an extra day in February. The correct way to do it is
to use the same method as in step 5, multiply 12 months by 29.53 lunar
days and drop the decimal. You will see that the lunar year contains 354
days. Add 354 days to July 26th 2011, remembering that 2012 is a leap
year and February has 29 days, not 28, and you will get July 14th
2012.</span>

</div>

</div>
