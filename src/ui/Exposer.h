#pragma once

#include "ui_sol.h"

class Exposer {
protected:
    std::shared_ptr<void> m_pobject;

public:
    Exposer(std::shared_ptr<void>& pobject) : m_pobject(pobject) {
    }

    template <typename T>
    Exposer(T object) {
        m_pobject = std::make_shared<T>(object);
    }

    virtual sol::object ExposeSolObject(lua_State* L) = 0;

    template <typename T>
    operator const T&() const {
        return *std::static_pointer_cast<T>(m_pobject);
    }
};

template <typename T>
class TypeExposer : public Exposer {
    //T m_object;

public:
    TypeExposer(const T& obj) : Exposer(obj) {
    }

    static std::shared_ptr<TypeExposer<T>> Create(const T& obj) {
        return std::shared_ptr<TypeExposer<T>>(new TypeExposer(obj));
    }

    sol::object ExposeSolObject(lua_State* L) {
        return sol::make_object<T>(L, *std::static_pointer_cast<T>(m_pobject));
    }

    operator T() const {
        return m_object;
    }
};

template <typename T>
class TStaticValueExposer : public TypeExposer<TRef<TStaticValue<T>>> {
    typedef TRef<TStaticValue<T>> ExposedType;

public:
    TStaticValueExposer(const ExposedType& obj) : TypeExposer(obj) {
    }

    static std::shared_ptr<TStaticValueExposer<T>> CreateStatic(const T& value) {
        return std::shared_ptr<TStaticValueExposer<T>>(new TStaticValueExposer<T>(new TStaticValue<T>(value)));
    }

    //static std::shared_ptr<TStaticValueExposer<T>> CreateModifiable(const T& value) {
    //    return std::shared_ptr<T>(new TypeExposer(TRef(new TModifiableValue<T>(value))));
    //}
};

typedef TStaticValueExposer<bool> BooleanExposer;
typedef TStaticValueExposer<float> NumberExposer;
typedef TStaticValueExposer<ZString> StringExposer;

class SolEventSinkInitializer;
typedef TypeExposer<std::shared_ptr<SolEventSinkInitializer>> EventSinkInitializerExposer;

class Exposed {
    sol::object m_object;
    sol::state m_lua;

public:
    template <typename T>
    Exposed(T arg) {
        m_object = sol::make_object<T>(m_lua, arg);
    }

    sol::object GetSolObject(lua_State* L) {
        return sol::make_object<sol::object>(L, m_object);
    }

};