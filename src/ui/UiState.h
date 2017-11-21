#pragma once

#include "ui.h"

class UiState {

private:
    std::string m_name;
    UiObjectContainer m_properties;

public:
    UiState(std::string& name, UiObjectContainer& properties);

    std::string& GetName();
    UiObjectContainer& GetProperties();

};

typedef TStaticValue<UiState> UiStateValue;

// Maybe remove when we really don't care about mdl logging anymore
class ModifiableUiStateName {
public:
    static ZString GetName() { return "ModifiableState"; }
};

typedef TModifiableValue<UiState, ModifiableUiStateName> UiStateModifiableValue;