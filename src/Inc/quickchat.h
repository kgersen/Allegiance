class SonicChatVoice : public IMDLObject {
public:
    TRef< IObject >& GetSound() { return *(TRef< IObject >*)(GetDataPointer() + 0); }
    ZString& GetString() { return *(ZString*)(GetDataPointer() + 4); }
};

class SonicChat : public IMDLObject {
public:
    TRef<IObjectList>& GetVoices() { return *(TRef<IObjectList>*)(GetDataPointer() + 0); }
};

class QuickChatCommand : public QuickChatNode {
public:
    TRef< SonicChat >& GetSonicChat() { return *(TRef< SonicChat >*)(GetDataPointer() + 0); }
    float& GetCommandID() { return *(float*)(GetDataPointer() + 4); }
    float& GetTargetType() { return *(float*)(GetDataPointer() + 8); }
    float& GetAbilityMask() { return *(float*)(GetDataPointer() + 12); }
};

class QuickChatMenuItem : public IMDLObject {
public:
    float& GetChar() { return *(float*)(GetDataPointer() + 0); }
    ZString& GetString() { return *(ZString*)(GetDataPointer() + 4); }
    TRef< QuickChatNode >& GetNode() { return *(TRef< QuickChatNode >*)(GetDataPointer() + 8); }
};

class QuickChatMenu : public QuickChatNode {
public:
    TRef<IObjectList>& GetItems() { return *(TRef<IObjectList>*)(GetDataPointer() + 0); }
};
