//////////////////////////////////////////////////////////////////////////////
//
// ast.h
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _ast_h_
#define _ast_h_

class Value;
class Delay;
class Environment;
class Expression;

//////////////////////////////////////////////////////////////////////////////
//
// Type
//
//////////////////////////////////////////////////////////////////////////////

class PolymorphicType;
class BaseType;
class FunctionType;
class ListType;
class PairType;

class Type : public IObject {
public:
    virtual ZString GetString(int& id)             = 0;
    virtual bool IsMatchNotPolymorphic(Type* ptype) = 0;

    virtual bool IsMatch(Type* ptype)
    {
        return ptype->IsMatchNotPolymorphic(this);
    }

    virtual BaseType*        GetBaseType()        { return NULL; };
    virtual FunctionType*    GetFunctionType()    { return NULL; };
    virtual ListType*        GetListType()        { return NULL; };
    virtual PairType*        GetPairType()        { return NULL; };
    virtual PolymorphicType* GetPolymorphicType() { return NULL; };
};

class PolymorphicType : public Type {
private:
    TRef<Type> m_ptype;
    ZString    m_str;

public:
    PolymorphicType()
    {
    }

    ZString GetString(int& id)
    {
        if (m_ptype) {
            return m_ptype->GetString(id);
        } else {
            if (m_str.IsEmpty()) {
                char ch = id;
                m_str = "'" + ZString(&ch, 1);
            }

            return m_str;
        }
    }

    bool IsMatchNotPolymorphic(Type* ptype)
    {
        return IsMatch(ptype);
    }

    bool IsMatch(Type* ptypeArg)
    {
        if (m_ptype) {
            return m_ptype->IsMatch(ptypeArg);
        } else {
            Type* ptype = ptypeArg;

            while (ptype != NULL) {
                TRef<PolymorphicType> ppolymorphicType = ptype->GetPolymorphicType();

                if (ppolymorphicType != NULL) {
                    if (ppolymorphicType == this) {
                        return true;
                    }
                } else {
                    break;
                }

                ptype = ppolymorphicType->m_ptype;
            }

            m_ptype = ptypeArg;
            return true;
        }
    }

    BaseType* GetBaseType()        
    { 
        if (m_ptype) {
            return m_ptype->GetBaseType();
        } else {
            return NULL; 
        }
    };

    FunctionType* GetFunctionType()    
    { 
        if (m_ptype) {
            return m_ptype->GetFunctionType();
        } else {
            return NULL; 
        }
    };

    ListType* GetListType()        
    { 
        if (m_ptype) {
            return m_ptype->GetListType();
        } else {
            return NULL; 
        }
    };

    PairType* GetPairType()        
    { 
        if (m_ptype) {
            return m_ptype->GetPairType();
        } else {
            return NULL; 
        }
    };

    PolymorphicType* GetPolymorphicType()
    {
        return this;
    }
};

class BaseType : public Type {
private:
    ZString m_str;

public:
    BaseType(const ZString& str) :
        m_str(str)
    {
    }

    ZString GetString(int& id)
    {
        return m_str;
    }

    BaseType* GetBaseType()
    {
        return this;
    }

    bool IsMatchNotPolymorphic(Type* ptypeArg)
    {
        TRef<BaseType> ptype = ptypeArg->GetBaseType();

        if (ptype != NULL) {
            return m_str == ptype->m_str;
        }

        return false;
    }
};

class FunctionType : public Type {
private:
    TRef<Type> m_ptypeArg;
    TRef<Type> m_ptypeResult;

public:
    FunctionType(Type* ptypeArg, Type* ptypeResult) :
        m_ptypeArg(ptypeArg),
        m_ptypeResult(ptypeResult)
    {
    }

    Type* GetArgType()
    {
        return m_ptypeArg;
    }

    Type* GetResultType()
    {
        return m_ptypeResult;
    }

    ZString GetString(int& id)
    {
        return m_ptypeArg->GetString(id) + " -> " + m_ptypeResult->GetString(id);
    }

    FunctionType* GetFunctionType()
    {
        return this;
    }

    bool IsMatchNotPolymorphic(Type* ptypeArg)
    {
        TRef<FunctionType> ptype = ptypeArg->GetFunctionType();

        if (ptype != NULL) {
            return m_ptypeArg->IsMatch(ptype->m_ptypeArg) && m_ptypeResult->IsMatch(ptype->m_ptypeResult);
        }

        return false;
    }
};

class ListType : public Type {
private:
    TRef<Type> m_ptype;

public:
    ListType(Type* ptype) :
        m_ptype(ptype)
    {
    }

    Type* GetType()
    {
        return m_ptype;
    }

    ZString GetString(int& id)
    {
        return m_ptype->GetString(id) + " list";
    }

    ListType* GetListType()
    {
        return this;
    }

    bool IsMatchNotPolymorphic(Type* ptypeArg)
    {
        TRef<ListType> ptype = ptypeArg->GetListType();

        if (ptype != NULL) {
            return m_ptype->IsMatch(ptype->m_ptype);
        }

        return false;
    }
};

class PairType : public Type {
private:
    TRef<Type> m_ptypeFirst;
    TRef<Type> m_ptypeSecond;

public:
    PairType(Type* ptypeFirst, Type* ptypeSecond) :
        m_ptypeFirst(ptypeFirst),
        m_ptypeSecond(ptypeSecond)
    {
    }

    Type* GetFirstType()
    {
        return m_ptypeFirst;
    }

    Type* GetSecondType()
    {
        return m_ptypeSecond;
    }

    ZString GetString(int& id)
    {
        return 
              m_ptypeFirst->GetString(id) 
            + ZString(", " )
            + m_ptypeSecond->GetString(id);
    }

    PairType* GetPairType()
    {
        return this;
    }

    bool IsMatchNotPolymorphic(Type* ptypeArg)
    {
        TRef<PairType> ptype = ptypeArg->GetPairType();

        if (ptype != NULL) {
            return m_ptypeFirst->IsMatch(ptype->m_ptypeFirst) && m_ptypeSecond->IsMatch(ptype->m_ptypeSecond);
        }

        return false;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Value
//
//////////////////////////////////////////////////////////////////////////////

class Value : public IObject {
public:
    virtual TRef<Type> GetType()   { ZUnimplemented(); return NULL; }
    virtual ZString    GetString() = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// Expression
//
//////////////////////////////////////////////////////////////////////////////

class Expression : public IObject {
public:
    virtual bool        IsValue();
    virtual Value*      GetValue();
    virtual TRef<Type>  GetType(Environment* penv)  = 0;
    virtual TRef<Delay> Evaluate(Environment* penv) = 0;
};

TRef<Expression> CreateSymbolExpression(const ZString& str);
TRef<Expression> CreateApplyExpression(Expression* pexprFunc, Expression* pexprArg);
TRef<Expression> CreateFunctionExpression(const ZString& str, Expression* pexpr);
TRef<Expression> CreateLetExpression(Environment* penv, Expression* pexprIn);
TRef<Expression> CreateValueExpression(Value* pvalue);

//////////////////////////////////////////////////////////////////////////////
//
// Delay
//
//////////////////////////////////////////////////////////////////////////////

class Delay : public IObject {
public:
    virtual Expression* GetExpression() = 0;
    virtual TRef<Value> Evaluate()      = 0;
    virtual TRef<Type>  GetType()       = 0;
};

TRef<Delay> CreateDelay(Environment* penv, Expression* pexpr);
TRef<Delay> CreateTypeDelay(Type* ptype);

//////////////////////////////////////////////////////////////////////////////
//
// Environment
//
//////////////////////////////////////////////////////////////////////////////

class Environment : public IObject {
private:
    ZString           m_str;
    TRef<Delay>       m_pdelay;
    TRef<Environment> m_penv;

public:
    Environment(Environment* penv, const ZString& str, Delay* pdelay);

    const ZString& GetString()
    {
        return m_str;
    }

    Environment* GetNext()
    {
        return m_penv;
    }

    void SetNext(Environment* penv)
    {
        m_penv = penv;
    }

    Delay* GetDelay()
    {
        return m_pdelay;
    }

    Delay* Find(const ZString& str);
    Delay* GetArgument(int index);
};

#endif
