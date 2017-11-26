#pragma once

#include "ui.h"

/*template <class PropertiesContainerType>
class UiState {

private:
    std::string m_name;
    PropertiesContainerType m_properties;

public:
    UiState(std::string name, PropertiesContainerType properties);

    std::string GetName() const;

    PropertiesContainerType& GetProperties() const;

};

typedef TStaticValue<UiState<UiObjectContainer>> UiStateValue;

// Maybe remove when we really don't care about mdl logging anymore
class ModifiableUiStateName {
public:
    static ZString GetName() { return "ModifiableState"; }
};

void Write(IMDLBinaryFile* pmdlFile, const UiState& value);
ZString GetString(int indent, const UiState& value);
ZString GetFunctionName(const UiState& value);
typedef TModifiableValue<UiState<UiObjectContainer>, ModifiableUiStateName> UiStateModifiableValue;*/

// Maybe remove when we really don't care about mdl logging anymore
class ModifiableUiStateName {
public:
    static ZString GetName() { return "ModifiableState"; }
};

class UiState : public UiObjectContainer {

private:
    std::string m_name;
public:
    using UiObjectContainer::UiObjectContainer;
    UiState(std::string name, std::map<std::string, boost::any> map = {}) :
        m_name(name),
        UiObjectContainer(map)
    {}

    std::string GetName() const {
        return m_name;
    }

    template <class T>
    T* as() const {
        //const T* result = dynamic_cast<const T*>(this);
        //if (!result) {
        //    throw std::runtime_error("Not in the correct state");
        //}
        return (T*)this;
    }
};

class SimpleUiState : public UiState {
public:
    SimpleUiState(std::string name, std::map<std::string, boost::any> map = {}) :
        UiState(name, map)
    {
    }
};

typedef TStaticValue<UiState> UiStateValue;

// Maybe remove when we really don't care about mdl logging anymore
void Write(IMDLBinaryFile* pmdlFile, const UiState& value);
ZString GetString(int indent, const UiState& value);
ZString GetFunctionName(const UiState& value);
typedef TModifiableValue<UiState, ModifiableUiStateName> UiStateModifiableValue;

