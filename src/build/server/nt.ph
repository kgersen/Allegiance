# (c) 1995 Microsoft Corporation. All rights reserved
#          Developed by hip communications inc., http://info.hip.com/info

# WYT 1995-05-18 Wei-Yuen Tan (wyt@hip.com)
# reg.ph - macro definitions for NT Registry Extensions
#          auto-extracted from windows.h, winreg.h, winnt.h
#          EDIT AT YOUR OWN RISK!

eval '

# miscellaneous

sub NULL { (0);}
sub WIN31_CLASS { &NULL;}

# eventlog read flags

sub EVENTLOG_SEQUENTIAL_READ {0x0001;}
sub EVENTLOG_SEEK_READ {0x0002;}
sub EVENTLOG_FORWARDS_READ {0x0004;}
sub EVENTLOG_BACKWARDS_READ {0x0008;}

# event types

sub EVENTLOG_SUCCESS {0x0000;}
sub EVENTLOG_ERROR_TYPE {0x0001;}
sub EVENTLOG_WARNING_TYPE {0x0002;}
sub EVENTLOG_INFORMATION_TYPE {0x0004;}
sub EVENTLOG_AUDIT_SUCCESS {0x0008;}
sub EVENTLOG_AUDIT_FAILURE {0x0010;}

# write flags for auditing for paired events.

sub EVENTLOG_START_PAIRED_EVENT {0x0001;}
sub EVENTLOG_END_PAIRED_EVENT {0x0002;}
sub EVENTLOG_END_ALL_PAIRED_EVENTS {0x0004;}
sub EVENTLOG_PAIRED_EVENT_ACTIVE {0x0008;}
sub EVENTLOG_PAIRED_EVENT_INACTIVE {0x0010;}

# predefined registry handles.

sub HKEY_CLASSES_ROOT {( 0x80000000 );}
sub HKEY_CURRENT_USER {( 0x80000001 );}
sub HKEY_LOCAL_MACHINE {( 0x80000002 );}
sub HKEY_USERS {( 0x80000003 );}
sub HKEY_PERFORMANCE_DATA {( 0x80000004 );}
sub HKEY_PERFORMANCE_TEXT {( 0x80000050 );}
sub HKEY_PERFORMANCE_NLSTEXT {( 0x80000060 );}

# registry key access modes.

sub DELETE {(0x00010000);}
sub READ_CONTROL {(0x00020000);}
sub WRITE_DAC {(0x00040000);}
sub WRITE_OWNER {(0x00080000);}
sub SYNCHRONIZE {(0x00100000);}
sub STANDARD_RIGHTS_REQUIRED {(0x000F0000);}
sub STANDARD_RIGHTS_READ {( &READ_CONTROL);}
sub STANDARD_RIGHTS_WRITE {( &READ_CONTROL);}
sub STANDARD_RIGHTS_EXECUTE {( &READ_CONTROL);}
sub STANDARD_RIGHTS_ALL {(0x001F0000);}
sub SPECIFIC_RIGHTS_ALL {(0x0000FFFF);}
sub ACCESS_SYSTEM_SECURITY {(0x01000000);}
sub MAXIMUM_ALLOWED {(0x02000000);}
sub GENERIC_READ {(0x80000000);}
sub GENERIC_WRITE {(0x40000000);}
sub GENERIC_EXECUTE {(0x20000000);}
sub GENERIC_ALL {(0x10000000);}
sub KEY_QUERY_VALUE {(0x0001);}
sub KEY_SET_VALUE {(0x0002);}
sub KEY_CREATE_SUB_KEY {(0x0004);}
sub KEY_ENUMERATE_SUB_KEYS {(0x0008);}
sub KEY_NOTIFY {(0x0010);}
sub KEY_CREATE_LINK {(0x0020);}
sub KEY_READ {(( &STANDARD_RIGHTS_READ |  &KEY_QUERY_VALUE |  &KEY_ENUMERATE_SUB_KEYS |  &KEY_NOTIFY) & (~ &SYNCHRONIZE));}
sub KEY_WRITE {(( &STANDARD_RIGHTS_WRITE |  &KEY_SET_VALUE |  &KEY_CREATE_SUB_KEY) & (~ &SYNCHRONIZE));}
sub KEY_EXECUTE {(( &KEY_READ) & (~ &SYNCHRONIZE));}
sub KEY_ALL_ACCESS {(( &STANDARD_RIGHTS_ALL |  &KEY_QUERY_VALUE |  &KEY_SET_VALUE |  &KEY_CREATE_SUB_KEY |  &KEY_ENUMERATE_SUB_KEYS |  &KEY_NOTIFY |  &KEY_CREATE_LINK) & (~ &SYNCHRONIZE));}

# registry key creation options

sub REG_OPTION_RESERVED {(0x00000000);}
sub REG_OPTION_NON_VOLATILE {(0x00000000);}
sub REG_OPTION_VOLATILE {(0x00000001);}
sub REG_OPTION_CREATE_LINK {(0x00000002);}
sub REG_OPTION_BACKUP_RESTORE {(0x00000004);}
sub REG_LEGAL_OPTION {( &REG_OPTION_RESERVED |  &REG_OPTION_NON_VOLATILE |  &REG_OPTION_VOLATILE |  &REG_OPTION_CREATE_LINK |  &REG_OPTION_BACKUP_RESTORE);}

# miscellaneous registry stuff.

sub REG_CREATED_NEW_KEY {(0x00000001);}
sub REG_OPENED_EXISTING_KEY {(0x00000002);}
sub REG_WHOLE_HIVE_VOLATILE {(0x00000001);}
sub REG_REFRESH_HIVE {(0x00000002);}

# registry data types

sub REG_NONE {( 0 );}
sub REG_SZ {( 1 );}
sub REG_EXPAND_SZ {( 2 );}
sub REG_BINARY {( 3 );}
sub REG_DWORD {( 4 );}
sub REG_DWORD_LITTLE_ENDIAN {( 4 );}
sub REG_DWORD_BIG_ENDIAN {( 5 );}
sub REG_LINK {( 6 );}
sub REG_MULTI_SZ {( 7 );}
sub REG_RESOURCE_LIST {( 8 );}
sub REG_FULL_RESOURCE_DESCRIPTOR {( 9 );}
sub REG_RESOURCE_REQUIREMENTS_LIST {( 10 );}

# OLE Automation data types.

sub VT_EMPTY {0;}
sub VT_NULL {1;}
sub VT_I2 {2;}
sub VT_I4 {3;}
sub VT_R4 {4;}
sub VT_R8 {5;}
sub VT_CY {6;}
sub VT_DATE {7;}
sub VT_BSTR {8;}
sub VT_DISPATCH {9;}
sub VT_ERROR {10;}
sub VT_BOOL {11;}
sub VT_VARIANT {12;}
sub VT_UNKNOWN {13;}
sub VT_I1 {16;}
sub VT_UI1 {17;}
sub VT_UI2 {18;}
sub VT_UI4 {19;}
sub VT_I8 {20;}
sub VT_UI8 {21;}
sub VT_INT {22;}
sub VT_UINT {23;}
sub VT_VOID {24;}
sub VT_HRESULT {25;}
sub VT_PTR {26;}
sub VT_SAFEARRAY {27;}
sub VT_CARRAY {28;}
sub VT_USERDEFINED {29;}
sub VT_LPSTR {30;}
sub VT_LPWSTR {31;}
sub VT_FILETIME {64;}
sub VT_BLOB {65;}
sub VT_STREAM {66;}
sub VT_STORAGE {67;}
sub VT_STREAMED_OBJECT {68;}
sub VT_STORED_OBJECT {69;}
sub VT_BLOB_OBJECT {70;}
sub VT_CF {71;}
sub VT_CLSID {72;}

@supportedTypes = (
    &VT_UI1,
    &VT_I2,
    &VT_I4,
    &VT_R4,
    &VT_R8,
    &VT_DATE,
    &VT_SAFEARRAY,
    &VT_BSTR,
    &VT_CY,
    &VT_EMPTY,
    &VT_ERROR,
    &VT_DISPATCH,
    &VT_VARIANT, 
    &VT_UNKNOWN,
    &VT_USERDEFINED,
    &VT_BOOL,
    );

sub NTOLESupportedType {
    my( $type ) = @_;
    scalar( grep( do { $type == $_ }, @supportedTypes ) );
}

# Typelib

sub TKIND_ENUM {0;}
sub TKIND_RECORD { &TKIND_ENUM + 1;}
sub TKIND_MODULE { &TKIND_RECORD + 1;}
sub TKIND_INTERFACE { &TKIND_MODULE + 1;}
sub TKIND_DISPATCH { &TKIND_INTERFACE + 1;}
sub TKIND_COCLASS { &TKIND_DISPATCH + 1;}
sub TKIND_ALIAS { &TKIND_COCLASS + 1;}
sub TKIND_UNION { &TKIND_ALIAS + 1;}
sub TKIND_MAX { &TKIND_UNION + 1;}

# security information

sub OWNER_SECURITY_INFORMATION {(0x00000001);}
sub GROUP_SECURITY_INFORMATION {(0x00000002);}
sub DACL_SECURITY_INFORMATION {(0x00000004);}
sub SACL_SECURITY_INFORMATION {(0x00000008);}
1;

';
