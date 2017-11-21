
#include "UiState.h"

UiState::UiState(std::string& name, UiObjectContainer& properties) :
    m_name(name),
    m_properties(properties)
{
}

std::string& UiState::GetName()
{
    return m_name;
}

UiObjectContainer& UiState::GetProperties()
{
    return m_properties;
}
