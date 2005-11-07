#
# these are constants for use in accessing the NT Registry database.
#

#
# standard access types
#

$READ_CONTROL              =  0x00020000;
$SYNCHRONIZE               =  0x00100000;
$STANDARD_RIGHTS_REQUIRED  =  0x000F0000;
$STANDARD_RIGHTS_READ      =  $READ_CONTROL;
$STANDARD_RIGHTS_WRITE     =  $READ_CONTROL;
$STANDARD_RIGHTS_EXECUTE   =  $READ_CONTROL;
$STANDARD_RIGHTS_ALL       =  0x001F0000;
$SPECIFIC_RIGHTS_ALL       =  0x0000FFFF;


#
# pre-defined registry keys
#

$HKEY_CLASSES_ROOT     = 0x80000000;
$HKEY_CURRENT_USER     = 0x80000001;
$HKEY_LOCAL_MACHINE    = 0x80000002;
$HKEY_USERS            = 0x80000003;
$HKEY_PERFORMANCE_DATA = 0x80000004;

#
# Open/Create Options
#

$REG_OPTION_RESERVED     = 0x00000000;  # Parameter is reserved
$REG_OPTION_NON_VOLATILE = 0x00000000;  # Key is preserved
                                         # when system is rebooted
$REG_OPTION_VOLATILE     = 0x00000001;  # Key is not preserved
                                         # when system is rebooted
$REG_OPTION_CREATE_LINK  = 0x00000002;  # Created key is a
                                         # symbolic link

#
# Key creation/open disposition
#

$REG_CREATED_NEW_KEY      =  0x00000001;   # New Registry Key created
$REG_OPENED_EXISTING_KEY  =  0x00000002;   # Existing Key opened

#
# Key restore flags
#

$REG_WHOLE_HIVE_VOLATILE   =  0x00000001;   # Restore whole hive volatile
$REG_REFRESH_HIVE          =  0x00000002;   # Unwind changes to last flush


#
# Notify filter values
#

$REG_NOTIFY_CHANGE_NAME        =  0x00000001; # Create or delete (child)
$REG_NOTIFY_CHANGE_ATTRIBUTES  =  0x00000002;
$REG_NOTIFY_CHANGE_LAST_SET    =  0x00000004; # time stamp
$REG_NOTIFY_CHANGE_SECURITY    =  0x00000008;

$REG_LEGAL_CHANGE_FILTER = $REG_NOTIFY_CHANGE_NAME |
    $REG_NOTIFY_CHANGE_ATTRIBUTES |
    $REG_NOTIFY_CHANGE_LAST_SET |
    $REG_NOTIFY_CHANGE_SECURITY;

#
#
# Predefined Value Types.
#

$REG_NONE                    = 0; # No value type
$REG_SZ                      = 1; # Unicode nul terminated string
$REG_EXPAND_SZ               = 2; # Unicode nul terminated string
                                  # (with environment variable references)
$REG_BINARY                  = 3; # Free form binary
$REG_DWORD                   = 4; # 32-bit number
$REG_DWORD_LITTLE_ENDIAN     = 4; # 32-bit number (same as REG_DWORD)
$REG_DWORD_BIG_ENDIAN        = 5; # 32-bit number
$REG_LINK                    = 6; # Symbolic Link (unicode)
$REG_MULTI_SZ                = 7; # Multiple Unicode strings
$REG_RESOURCE_LIST           = 8; # Resource list in the resource map
$REG_FULL_RESOURCE_DESCRIPTOR = 9;# Resource list in the hardware description

    
#
# Registry Specific Access Rights.
#

$KEY_QUERY_VALUE         = 0x0001;
$KEY_SET_VALUE           = 0x0002;
$KEY_CREATE_SUB_KEY      = 0x0004;
$KEY_ENUMERATE_SUB_KEYS  = 0x0008;
$KEY_NOTIFY              = 0x0010;
$KEY_CREATE_LINK         = 0x0020;

$KEY_READ                = ($STANDARD_RIGHTS_READ   |
                            $KEY_QUERY_VALUE        |
                            $KEY_ENUMERATE_SUB_KEYS |
                            $KEY_NOTIFY)                 
                             &                           
                           ~$SYNCHRONIZE;


$KEY_WRITE               = ($STANDARD_RIGHTS_WRITE   |
                            $KEY_SET_VALUE           |
                            $KEY_CREATE_SUB_KEY)
                             &
                            ~$SYNCHRONIZE;

$KEY_EXECUTE             = $KEY_READ & (~$SYNCHRONIZE);

$KEY_ALL_ACCESS          = ($STANDARD_RIGHTS_ALL    |
                            $KEY_QUERY_VALUE        |
                            $KEY_SET_VALUE          |
                            $KEY_CREATE_SUB_KEY     |
                            $KEY_ENUMERATE_SUB_KEYS |
                            $KEY_NOTIFY             |
                            $KEY_CREATE_LINK)
                             &
                           (~$SYNCHRONIZE);
1;
