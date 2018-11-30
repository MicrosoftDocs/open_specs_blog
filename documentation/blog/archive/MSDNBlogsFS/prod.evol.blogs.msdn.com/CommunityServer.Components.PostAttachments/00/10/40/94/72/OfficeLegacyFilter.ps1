#################################################################################
#
# This script is not officially supported by Microsoft, use it at your own risk.
# Microsoft has no liability, obligations, warranty, or responsibility regarding
# any result produced by use of this file.
#
# -------------------------------------------------------------------------------
#
# Name   : OfficeLegacyFilter.ps1
# Version: 1.0.0
# Usage  : OfficeLegacyFilter <directory or file name>
#          <directory or file name> - If the parameter is a file name, the file
#              is checked whether it complies to the syntax of the Office Legacy
#              file format, if yes, then the full file name and a few items
#              printed. Each item starts with an offset where this item is in
#              the file. For the properties their hex value printed, too.
#              The extension from the file name is not used.
#              If the parameter is a directory, then all the files in this
#              directory and in the subdirectories recursively are checked.
#
#################################################################################

Set-StrictMode -Version "Latest"
Clear-Host

<# Office Legacy files; the even indices contain the file name,
the odd ones contain a hash table, the Key (Name) is the property name
                                   the Value is its value
#>
[System.Object[]]$global:OfficeLegacyFiles = @() # Create empty array
$global:properties = @{} # Create empty hash table

<#
Check whether the file is Office Legacy based on MS-CFB and MS-OLEPS
$n - FullName
#>
function isOfficeLegacy {
  Param ([System.String]$n)
  [System.Boolean]$ret = $true # Assume the file is in the form Office Legacy form

  try {
    $fs = New-Object System.IO.FileStream($n, [System.IO.FileMode]::Open)
  }
  catch {
    Write-Host($_.ToString() -replace "^.*: ",'');
    return $false
  }

  $rdr = New-Object System.IO.BinaryReader($fs)
  :checkScope do { # Make scope for checking
  <# [MS-CFB] — v20130118 / 2.2 Compound File Header
  Header Signature (8 bytes): Identification signature for the compound file structure,
  and MUST be set to the value 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1.
  #>
    [System.Byte[]]$HeaderSignature = @(0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1);
    [System.Byte[]]$b8 = @(1..8);
    [void]$fs.Seek(0,"Begin") # To Header Signature
    [void]$rdr.Read($b8, 0, 8);
    if (!(Compare-Object $HeaderSignature $b8)) { # Returns FALSE if they are the same!
      # Write-Host "TRUE - Header Signature MATCH"
    } else {
      $ret = $false
      break;
    }
    $global:properties.Add("000000 Header Signature", 'D0 CF 11 E0 A1 B1 1A E1');
  # 0x18: Minor Version (2 bytes) - must be 0x003E
    [void]$fs.Seek(0x18,"Begin") # To Minor Version
    [System.UInt16]$MinorVersion = $rdr.ReadUInt16();
    if ($MinorVersion -ne 0x003E) {
      $ret = $false
      break;
    }
    $global:properties.Add("000018 MinorVersion", $MinorVersion);
  # 0x1A: Major Version (2 bytes) - must be 0x0003 or 0x0004
    [void]$fs.Seek(0x1A,"Begin") # To Major Version
    [System.UInt16]$MajorVersion = $rdr.ReadUInt16();
    if ($MajorVersion -ne 0x0003 -and $MajorVersion -ne 0x0004) {
      $ret = $false
      break;
    }
    $global:properties.Add("00001A MajorVersion", $MajorVersion);
  # 0x1C: Byte Order (2 bytes) - must be 0xFFFE - little-endian
    [void]$fs.Seek(0x1C,"Begin") # To Byte Order
    [System.UInt16]$ByteOrder = $rdr.ReadUInt16();
    if ($ByteOrder -ne 0xFFFE) {
      $ret = $false
      break;
    }
  <# 0x1E: Sector Shift (2 bytes): This field MUST be set to 0x0009, or 0x000c,
  depending on the Major Version field.
  This field specifies the sector size of the compound file as a power of 2.
  If Major Version is 3, then the Sector Shift MUST be 0x0009, specifying a sector size of 512 bytes.
  If Major Version is 4, then the Sector Shift MUST be 0x000C, specifying a sector size of 4096 bytes.
  #>
    [System.UInt32]$SectorSize = 0;
    [void]$fs.Seek(0x1E,"Begin") # To Sector Shift
    [System.UInt16]$SectorShift = $rdr.ReadUInt16();
    if ($MajorVersion -eq 0x0003) {
      if ($SectorShift -ne 0x0009) {
        $ret = $false
        break;
      }
      $SectorSize = 512;
    } else {
      if ($SectorShift -ne 0x000C) {
        $ret = $false
        break;
      }
      $SectorSize = 4096;
    }
  <# 0x20: Mini Sector Shift (2 bytes): This field MUST be set to 0x0006.
  This field specifies the sector size of the Mini Stream as a power of 2.
  The sector size of the Mini Stream MUST be 64 bytes.
  #>
    [System.UInt32]$MiniSectorSize = 64; # Assuming the check is fine
    [void]$fs.Seek(0x20,"Begin") # To Mini Sector Shift
    [System.UInt16]$MiniSectorShift = $rdr.ReadUInt16();
    if ($MiniSectorShift -ne 0x0006) {
      $ret = $false
      break;
    }
  <# 0x28: Number of Directory Sectors (4 bytes): This integer field contains the
  count of the number of directory sectors in the compound file.
  If Major Version is 3, then the Number of Directory Sectors MUST be zero.
  This field is not supported for version 3 compound files.
  #>
    [void]$fs.Seek(0x28,"Begin") # To Number of Directory Sectors
    [System.UInt32]$NumberOfDirectorySectors = $rdr.ReadUInt32();
    if ($MajorVersion -eq 0x0003) {
      if ($NumberOfDirectorySectors -ne 0x00000000) {
        $ret = $false
        break;
      }
    }
  <# 0x2C: Number of FAT Sectors (4 bytes): This integer field contains the count
  of the number of FAT sectors in the compound file.
  #>
    [void]$fs.Seek(0x2C,"Begin") # To Number of FAT Sectors
    [System.UInt32]$NumberOfFATSectors = $rdr.ReadUInt32();
    # No check for it
  <# 0x30: First Directory Sector Location (4 bytes): This integer field contains
  the starting sector number for the directory stream.
  directory stream: An array of directory entries grouped into sectors.
  sector: The smallest addressable unit of a disk.
  #>
    [void]$fs.Seek(0x30,"Begin") # To First Directory Sector Location
    [System.UInt32]$FirstDirectorySectorLocation = $rdr.ReadUInt32();
    # No check for it
    # Skip to the First Directory Sector
    [System.UInt32]$StartOffset = ($FirstDirectorySectorLocation + 1) * $SectorSize;
    [System.Byte[]]$b128 = @(1..128);
    [System.Byte[]]$SummInfo = @(
        0x05,0x00,0x53,0x00,0x75,0x00,0x6D,0x00,0x6D,0x00,0x61,0x00,0x72,0x00,0x79,0x00
        0x49,0x00,0x6E,0x00,0x66,0x00,0x6F,0x00,0x72,0x00,0x6D,0x00,0x61,0x00,0x74,0x00
        0x69,0x00,0x6F,0x00,0x6E,0x00,0x00,0x00);
    # Step through the Compound File Directory Entry
    [long]$offs = $StartOffset;
  <# Find the Directory Entry, [MS-CFB] v20130118 / 2.6.1 Compound File Directory
  Entry, which has: 0x0005+"Summary Information"
  #>
    do {
      [void]$fs.Seek($offs,"Begin")
      [void]$rdr.Read($b128, 0, 128);
      # Returns FALSE if they are the same!
      if (!(Compare-Object $SummInfo $b128[0..($SummInfo.Length - 1)])) {
        # Write-Host "TRUE - Summary Information found"
        break;
      }
      $offs += 128; # 0x80
      if ( $offs+128 -gt $fs.Length ) {
        $ret = $false
        break checkScope;
      }
    } while ($true); # breaks out

    [void]$fs.Seek($offs+0x74,"Begin") # To Starting Sector Location (4 bytes)
    [System.UInt32]$StartingSectorLocation = $rdr.ReadUInt32();
    [System.UInt64]$StreamSize = $rdr.ReadUInt64();

    # Skip to the Summary Information Sector
    [System.UInt32]$StartSummaryInformationSector = ($StartingSectorLocation + 1) * $SectorSize;
    # Skip to [MS-OLEPS] — v20130118 / 2.21 PropertySetStream

    [void]$fs.Seek($StartSummaryInformationSector+0x2C,"Begin"); # To Offset0
    [System.UInt32]$Offset0 = $rdr.ReadUInt32();
    # Skip to [MS-OLEPS] — v20130118 / 2.20 PropertySet
    [System.UInt64]$PropertySet_Offset = $StartSummaryInformationSector + $Offset0;
    [void]$fs.Seek($PropertySet_Offset+0x04,"Begin"); # To PropertySet.NumProperties
    [System.UInt32]$NumProperties = $rdr.ReadUInt32();

    # List the properties
    [System.UInt32]$Offset = 0;
    for ($i=0; $i -lt $NumProperties; $i++) {
      [void]$fs.Seek($PropertySet_Offset+0x08+$i*0x8,"Begin");
      [System.UInt32]$Property = $rdr.ReadUInt32();
      [System.UInt32]$Offset = $rdr.ReadUInt32();

      # http://msdn.microsoft.com/en-us/library/dd942311.aspx  PDSI_...
      switch ( $Property ) {
      0x00000001 { # Property CODEPAGE_PROPERTY_IDENTIFIER
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 4 byte Code Page
        [System.UInt32]$CodePageOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($CodePageOffs,"Begin");
        [System.UInt32]$CodePage = $rdr.ReadUInt32();
        $global:properties.Add($CodePageOffs.ToString("X6")+" 01 CODEPAGE_PROPERTY_IDENTIFIER", $CodePage);
        } # 0x00000001
      0x00000002 { # Property PIDSI_TITLE
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the length, name and display the name
        [System.UInt32]$TitleOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($TitleOffs,"Begin");
        [System.UInt32]$TitleLength = $rdr.ReadUInt32();
        [System.Char[]]$Title = New-Object System.Char[] $TitleLength;
        [void]$rdr.Read($Title, 0, $TitleLength);
        $global:properties.Add($TitleOffs.ToString("X6")+" 02 PIDSI_TITLE", (-join $Title));
        } # 0x00000002
      0x00000003 { # Property PIDSI_SUBJECT
        # Skip the type (2 bytes) + 2 bytes padding
        # VT_LPSTR, Read the length, name and display the name
        [System.UInt32]$SubjectOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($SubjectOffs,"Begin");
        [System.UInt32]$SubjectLength = $rdr.ReadUInt32();
        [System.Char[]]$Subject = New-Object System.Char[] $SubjectLength;
        [void]$rdr.Read($Subject, 0, $SubjectLength);
        $global:properties.Add($SubjectOffs.ToString("X6")+" 03 PIDSI_SUBJECT", (-join $Subject));
        } # 0x00000003
      0x00000004 { # Property PIDSI_AUTHOR
        # Skip the type (2 bytes) + 2 bytes padding
        # VT_LPSTR, Read the length, name and display the name
        [System.UInt32]$AuthorNameOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($AuthorNameOffs,"Begin");
        [System.UInt32]$AuthorNameLength = $rdr.ReadUInt32();
        [System.Char[]]$AuthorName = New-Object System.Char[] $AuthorNameLength;
        [void]$rdr.Read($AuthorName, 0, $AuthorNameLength);
        $global:properties.Add($AuthorNameOffs.ToString("X6")+" 04 PIDSI_AUTHOR", (-join $AuthorName));
        } # 0x00000004
      0x00000005 { # Property PIDSI_KEYWORDS
        # Skip the type (2 bytes) + 2 bytes padding
        # VT_LPSTR, Read the length, name and display the words
        [System.UInt32]$KeywordsOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($KeywordsOffs,"Begin");
        [System.UInt32]$KeywordsLength = $rdr.ReadUInt32();
        [System.Char[]]$Keywords = New-Object System.Char[] $KeywordsLength;
        [void]$rdr.Read($Keywords, 0, $KeywordsLength);
        $global:properties.Add($KeywordsOffs.ToString("X6")+" 05 PIDSI_KEYWORDS", (-join $Keywords));
        } # 0x00000004
      0x00000007 { # Property PIDSI_TEMPLATE
        # Skip the type (x1E, 2 bytes) + 2 bytes padding
        # Read the length, name and display the name
        [System.UInt32]$TemplateNameOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($TemplateNameOffs,"Begin");
        [System.UInt32]$TemplateNameLength = $rdr.ReadUInt32();
        [System.Char[]]$TemplateName = New-Object System.Char[] $TemplateNameLength;
        [void]$rdr.Read($TemplateName, 0, $TemplateNameLength);
        $global:properties.Add($TemplateNameOffs.ToString("X6")+" 07 PIDSI_TEMPLATE",
                               (-join $TemplateName));
        } # 0x00000007
      0x00000008 { # Property PIDSI_LASTAUTHOR
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the length, name and display the name
        [System.UInt32]$LastAuthorNameOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($LastAuthorNameOffs,"Begin");
        [System.UInt32]$LastAuthorNameLength = $rdr.ReadUInt32();
        [System.Char[]]$LastAuthorName = New-Object System.Char[] $LastAuthorNameLength;
        [void]$rdr.Read($LastAuthorName, 0, $LastAuthorNameLength);
        $global:properties.Add($LastAuthorNameOffs.ToString("X6")+" 08 PIDSI_LASTAUTHOR",
                               (-join $LastAuthorName));
        } # 0x00000008
      0x00000009 { # Property PIDSI_REVNUMBER
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the length, name and display the name
        [System.UInt32]$RevNumberOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($RevNumberOffs,"Begin");
        [System.UInt32]$RevNumberLength = $rdr.ReadUInt32(); # Two zeros are at the end
        [System.Char[]]$RevNumber = New-Object System.Char[] $RevNumberLength;
        [void]$rdr.Read($RevNumber, 0, $RevNumberLength);
        $global:properties.Add($RevNumberOffs.ToString("X6")+" 09 PIDSI_REVNUMBER", (-join $RevNumber));
        } # 0x00000009
      0x0000000A { # Property PIDSI_EDITTIME
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 8 byte FILETIME
        [System.UInt64]$EditTimeOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($EditTimeOffs,"Begin");
        [System.Int64]$EditTime = $rdr.ReadInt64();
        $a = [System.TimeSpan]::FromTicks($EditTime);
        $global:properties.Add($AuthorNameOffs.ToString("X6")+" 0A PIDSI_EDITTIME", $a.ToString());
        } # 0x0000000A
      0x0000000C { # Property PIDSI_CREATE_DTM
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 8 byte FILETIME
        [System.UInt64]$CreateDTMOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($CreateDTMOffs,"Begin");
        [System.UInt64]$CreateDTM = $rdr.ReadUInt64();
        $a = [System.DateTime]::FromFileTime($CreateDTM);
        $global:properties.Add($CreateDTMOffs.ToString("X6")+" 0C PIDSI_CREATE_DTM",
                               [System.String]($a.DayOfWeek)+', '+$a.ToString());
        } # 0x0000000C
      0x0000000D { # Property PIDSI_LASTSAVE_DTM
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 8 byte FILETIME
        [System.UInt64]$LastSaveDTMOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($LastSaveDTMOffs,"Begin");
        [System.Int64]$LastSaveDTM = $rdr.ReadInt64();
        $a = [System.DateTime]::FromFileTime($LastSaveDTM);
        $global:properties.Add($LastSaveDTMOffs.ToString("X6")+" 0D PIDSI_LASTSAVE_DTM",
                               [System.String]($a.DayOfWeek)+', '+$a.ToString());
        } # 0x0000000D
      0x0000000E { # Property PIDSI_PAGECOUNT
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 4 byte Signed Integer
        [System.UInt32]$PageCountOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($PageCountOffs,"Begin");
        [System.Int32]$PageCount = $rdr.ReadInt32();
        $global:properties.Add($PageCountOffs.ToString("X6")+" 0E PIDSI_PAGECOUNT", $PageCount);
        } # 0x0000000E
      0x0000000F { # Property PIDSI_WORDCOUNT
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 4 byte Signed Integer
        [System.UInt32]$WordCountOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($WordCountOffs,"Begin");
        [System.Int32]$WordCount = $rdr.ReadInt32();
        $global:properties.Add($WordCountOffs.ToString("X6")+" 0F PIDSI_WORDCOUNT", $WordCount.ToString());
        } # 0x0000000F
      0x00000010 { # Property PIDSI_CHARCOUNT
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 4 byte Signed Integer
        [System.UInt32]$CharCountOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($CharCountOffs,"Begin");
        [System.Int32]$CharCount = $rdr.ReadInt32();
        $global:properties.Add($CharCountOffs.ToString("X6")+" 10 PIDSI_CHARCOUNT", $CharCount.ToString());
        } # 0x00000010
      0x00000011 { # Property PIDSI_THUMBNAIL
        # Type (2 bytes) VT_CF 0x0047 + 2 bytes padding
        # http://msdn.microsoft.com/en-us/library/windows/desktop/aa380376(v=vs.85).aspx
        # [MS-OLEPS] 2.15 / Must be a ClopboardData
        # 2.11 ClipboardData: 4 byte Size | 4 byte Format | Data (multiple of 4)
        # Read the 4 byte Unsigned Integer, Size
        [System.UInt32]$ThumbnailSizeOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($ThumbnailSizeOffs,"Begin");
        [System.UInt32]$ThumbnailSize = $rdr.ReadUInt32();
        [System.UInt32]$ThumbnailFormat = $rdr.ReadUInt32();
        $global:properties.Add($ThumbnailSizeOffs.ToString("X6")+" 11 PIDSI_THUMBNAIL size format",
                                 ($ThumbnailSize.ToString()+' '+$ThumbnailFormat.ToString("X8")));
        } # 0x00000011
      0x00000012 { # Property PIDSI_APPNAME
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the length, name and display the name
        [System.UInt32]$AppNameOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($AppNameOffs,"Begin");
        [System.UInt32]$AppNameLength = $rdr.ReadUInt32();
        [System.Char[]]$AppName = New-Object System.Char[] $AppNameLength;
        [void]$rdr.Read($AppName, 0, $AppNameLength);
        [System.String]$str = -join [System.Char[]]$AppName;
        $global:properties.Add($AppNameOffs.ToString("X6")+" 12 PIDSI_APPNAME", $str.Clone());
        } # 0x00000012
      0x00000013 { # Property PIDSI_DOC_SECURITY
        # Skip the type (2 bytes) + 2 bytes padding
        # Read the 4 byte FILETIME
        [System.UInt32]$SecurityOffs = $StartSummaryInformationSector + $Offset + $Offset0 + 0x4;
        [void]$fs.Seek($SecurityOffs,"Begin");
        [System.UInt32]$Security = $rdr.ReadUInt32();
        $global:properties.Add($SecurityOffs.ToString("X6")+" 13 PIDSI_DOC_SECURITY", $Security);
        } # 0x00000013
      default {
        Write-Host ("Unknown Property "+$i+". 0x"+$Property.ToString("X8"));
        Write-Host ("Offset 0x"+$Offset.ToString("X8"));
        [System.UInt32]$UnknownOffs = $StartSummaryInformationSector + $Offset + $Offset0;
        [void]$fs.Seek($UnknownOffs,"Begin");
        [System.Byte[]]$bx30 = New-Object System.Byte[] 0x30;
        [void]$rdr.Read($bx30, 0, 0x30);
        Write-Host ("STOP  HERE");
        } # default
      } # switch
    } # for ($i=0; $i -lt $NumProperties; $i++)
  } while ($false);

  $rdr.Close();
  return $ret

} # isOfficeLegacy

function Check-and-Store {
  Param ([System.IO.FileInfo]$item)
  if ($item -eq $null) {return}
  # Check whether it is a Office Legacy file
  [System.Boolean]$b = $false;
  # Clean the properties
  $global:properties.Clear();
  $b = isOfficeLegacy($item.FullName);
  if ( $true -eq $b ) {
    $global:OfficeLegacyFiles += $item.FullName;
    $global:OfficeLegacyFiles += $global:properties.Clone();
  } else {
    # Write-Host ("NOT Office Legacy "+$item.FullName)
  }
  return $b
}

# First check all files, second check all sub-directories
function Travel-Directories {
  Param ([System.string]$dir)
  Push-Location $dir
  $private:kids = Get-ChildItem
  if ($kids -eq $null) { return }
  foreach ($item in $kids) {
    if ( $item.PSIsContainer ) {
      # Write-Host ("NOW SKIP DIRECTORY "+$item.FullName)
    } else {
      [void](Check-and-Store $item)
    }
  }
  foreach ($item in $kids) {
    if ( $item.PSIsContainer ) {
      Travel-Directories $item.FullName
    }
  }
  Pop-Location
}

if ( $Args.Count -ne 1 ) {
  Write-Host "Give a directory or file name"
  exit
}
$dir = $Args[0]
$doesExist = Test-Path $dir

if ( $doesExist ) {
  if ( (Get-Item $dir).PSIsContainer ) {
    # Write-Host "$dir is a directory"
    Travel-Directories $dir
  } else {
    # Write-Host "$dir is NOT a directory"
    [void](Check-and-Store (Get-Item $dir))
  }
} else {
  Write-Host "$dir DOES NOT exist"
  exit
}


# Display the finding
for ([System.Int32]$i = 0; $i -lt $global:OfficeLegacyFiles.Length; $i+=2) {
  "`n"+($global:OfficeLegacyFiles[$i]); # Contains the full name of the file
  # $i+1 contains a hash table, write it out
  # Be fancy
  $ml = 0;
  foreach ($pair in ($global:OfficeLegacyFiles[$i+1]).GetEnumerator()) {
    $x = $pair.Key.ToString().Length;
    if ( $ml -lt $x ) { $ml = $x }
  }
  [System.String[]]$a = @()
  foreach ($pair in ($global:OfficeLegacyFiles[$i+1]).GetEnumerator()) {
    $x = $pair.Key.ToString().Length;
    # Write-Host ('    '+$pair.Key+' '*($ml-$x)+': '+$pair.Value)
    [int]$y=[int]$z=0; [int]$w = $ml-$x-1;
    if ($w -ge 0) {$y = $w%3+1; $z=$w/3; if($y -eq 3){--$z}} # "/" doesn't truncate
    [System.String]$l = ('    '+$pair.Key+
                          ' '*$y+'.  '*$z+
                          ': '+$pair.Value)
    $a += $l
  }
  $a | sort
}
