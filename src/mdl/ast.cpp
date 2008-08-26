#include "pch.h"
#include "ast.h"

//////////////////////////////////////////////////////////////////////////////
//
// Closure
//
//////////////////////////////////////////////////////////////////////////////

class Closure : public Value {
public:
    virtual TRef<Delay> Evaluate(Delay* pdelay) = 0;

    static TRef<Type> StaticGetType()
    {
        return
            new FunctionType(
                new PolymorphicType(),
                new PolymorphicType()
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Closure
//
//////////////////////////////////////////////////////////////////////////////

class ExpressionClosure : public Closure {
private:
    ZString           m_str;
    TRef<Environment> m_penv;
    TRef<Expression>  m_pexpr;

public:
    ExpressionClosure(Environment* penv, const ZString& str, Expression* pexpr) :
        m_str(str),
        m_penv(penv),
        m_pexpr(pexpr)
    {
    }

    TRef<Delay> Evaluate(Delay* pdelay)
    {
        return 
            CreateDelay(
                new Environment(m_penv, m_str, pdelay),
                m_pexpr
            );
    }

    ZString GetString()
    {
        return "(function arg -> expr)";
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Expression
//
//////////////////////////////////////////////////////////////////////////////

bool Expression::IsValue()
{
    return false;
}

Value* Expression::GetValue()
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//
// ValueExpression
//
//////////////////////////////////////////////////////////////////////////////

class ValueExpression  : public Expression {
private:
    TRef<Value> m_pvalue;

public:
    ValueExpression(Value* pvalue) :
        m_pvalue(pvalue)
    {
    }

    bool IsValue()
    {
        return true;
    }

    Value* GetValue()
    {
        return m_pvalue;
    }

    TRef<Type> GetType(Environment* penv)
    {
        return m_pvalue->GetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        return CreateDelay(NULL, this);
    }
};

TRef<Expression> CreateValueExpression(Value* pvalue)
{
    return new ValueExpression(pvalue);
}

//////////////////////////////////////////////////////////////////////////////
//
// SymbolExpression
//
//////////////////////////////////////////////////////////////////////////////

class SymbolExpression : public Expression {
private:
    ZString m_str;

public:
    SymbolExpression(const ZString& str) :
        m_str(str)
    {
    }

    TRef<Type> GetType(Environment* penv)
    {
        TRef<Delay> pdelay = penv->Find(m_str);

        if (pdelay == NULL) {
            ZError("Undefined symbol: " + m_str);
        } 

        return pdelay->GetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        return penv->Find(m_str);
    }
};

TRef<Expression> CreateSymbolExpression(const ZString& str)
{
    return new SymbolExpression(str);
}

//////////////////////////////////////////////////////////////////////////////
//
// ApplyExpression
//
//////////////////////////////////////////////////////////////////////////////

class ApplyExpression : public Expression {
private:
    TRef<Expression> m_pexprFunc;
    TRef<Expression> m_pexprArg;

public:
    ApplyExpression(Expression* pexprFunc, Expression* pexprArg) :
        m_pexprFunc(pexprFunc),
        m_pexprArg(pexprArg)
    {
    }

    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type>         ptypeFunc    = m_pexprFunc->GetType(penv);

        ZVerify(ptypeFunc->IsMatch(Closure::StaticGetType()));

        TRef<FunctionType> pfuncType    = ptypeFunc->GetFunctionType();
        TRef<Type>         ptypeFuncArg = pfuncType->GetArgType();
        TRef<Type>         ptypeArg     = m_pexprArg->GetType(penv);

        if (ptypeFuncArg->IsMatch(ptypeArg)) {
            return pfuncType->GetResultType();
        } else {
            int id = 'a';
            ZError(
                  "Type mismatch: "
                + ptypeFuncArg->GetString(id)
                + " != "
                + ptypeArg->GetString(id)
            );

            return NULL;
        }
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<Delay>   pdelayFunc = m_pexprFunc->Evaluate(penv); 
        TRef<Value>   pvalue     = pdelayFunc->Evaluate();
        TRef<Closure> pclosure;    CastTo(pclosure, pvalue);
        TRef<Delay>   pdelayArg  = m_pexprArg->Evaluate(penv);

        return pclosure->Evaluate(pdelayArg);
    }
};

TRef<Expression> CreateApplyExpression(Expression* pexprFunc, Expression* pexprArg)
{
    return new ApplyExpression(pexprFunc, pexprArg);
}

//////////////////////////////////////////////////////////////////////////////
//
// FunctionExpression
//
//////////////////////////////////////////////////////////////////////////////

class FunctionExpression : public Expression {
private:
    ZString          m_str;
    TRef<Expression> m_pexpr;

public:
    FunctionExpression(const ZString& str, Expression* pexpr) :
        m_str(str),
        m_pexpr(pexpr)
    {
    }

    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type> ptypeArg = new PolymorphicType();

        penv =
            new Environment(
                penv,
                m_str,
                CreateTypeDelay(ptypeArg)
            );

        TRef<Type> ptypeResult = m_pexpr->GetType(penv);

        return new FunctionType(ptypeArg, ptypeResult);
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<Value>      pvalue = new ExpressionClosure(penv, m_str, m_pexpr);
        TRef<Expression> pexpr  = new ValueExpression(pvalue);

        return CreateDelay(NULL, pexpr);
    }
};

TRef<Expression> CreateFunctionExpression(const ZString& str, Expression* pexpr)
{
    return new FunctionExpression(str, pexpr);
}

//////////////////////////////////////////////////////////////////////////////
//
// LetExpression
//
//////////////////////////////////////////////////////////////////////////////

class LetExpression : public Expression {
private:
    TRef<Environment> m_penv;
    TRef<Expression>  m_pexprIn;

public:
    LetExpression(Environment* penv, Expression* pexprIn) :
        m_penv(penv),
        m_pexprIn(pexprIn)
    {
    }

    TRef<Type> GetType(Environment* penv)
    {
        ZUnimplemented();
        return NULL;
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        Environment*      penvLet = m_penv;
        TRef<Environment> penvTop = new Environment(NULL, "let", NULL);

        while (penvLet != NULL) {
            TRef<Delay> pdelay = penvLet->GetDelay();
            TRef<Expression> pexpr;

            if (pdelay != NULL) {
                pexpr = pdelay->GetExpression();
            }

            penv = 
                new Environment(
                    penv,
                    penvLet->GetString(),
                    CreateDelay(
                        penvTop,
                        pexpr
                    )
                );

            penvLet = penvLet->GetNext();
        }

        penvTop->SetNext(penv);

        return CreateDelay(penvTop, m_pexprIn);
    }
};

TRef<Expression> CreateLetExpression(Environment* penv, Expression* pexprIn)
{
    return new LetExpression(penv, pexprIn);
}

//////////////////////////////////////////////////////////////////////////////
//
// TypeDelay
//
//////////////////////////////////////////////////////////////////////////////

class TypeDelay : public Delay {
private:
    TRef<Type> m_ptype;

public:
    TypeDelay(Type* ptype) :
        m_ptype(ptype)
    {
    }

    Expression* GetExpression()
    {
        return NULL;
    }

    TRef<Value> Evaluate()
    {
        return NULL;
    }

    TRef<Type> GetType()
    {
        return m_ptype;
    }
};

TRef<Delay> CreateTypeDelay(Type* ptype)
{
    return new TypeDelay(ptype);
}

//////////////////////////////////////////////////////////////////////////////
//
// Delay
//
//////////////////////////////////////////////////////////////////////////////

class DelayImpl : public Delay {
private:
    TRef<Environment> m_penv;
    TRef<Expression>  m_pexpr;
    TRef<Type>        m_ptype;
    TRef<Value>       m_pvalue;

public:
    DelayImpl(Environment* penv, Expression* pexpr) :
        m_penv(penv),
        m_pexpr(pexpr)
    {
    }

    Expression* GetExpression()
    {
        return m_pexpr;
    }

    TRef<Value> Evaluate()
    {
        if (m_pvalue == NULL) {
            if (m_pexpr->IsValue()) {
                m_pvalue = m_pexpr->GetValue();
            } else {
                m_pvalue = m_pexpr->Evaluate(m_penv)->Evaluate();
            }
        }

        return m_pvalue;
    }

    TRef<Type> GetType()
    {
        if (m_ptype) {
            return m_ptype;
        } else {
            m_ptype = new PolymorphicType();
            TRef<Type> ptype = m_pexpr->GetType(m_penv);

            ZVerify(m_ptype->IsMatch(ptype));
            m_ptype = NULL;

            return ptype;
        }
    }
};

TRef<Delay> CreateDelay(Environment* penv, Expression* pexpr)
{
    return new DelayImpl(penv, pexpr);
}

//////////////////////////////////////////////////////////////////////////////
//
// Environment
//
//////////////////////////////////////////////////////////////////////////////

Environment::Environment(Environment* penv, const ZString& str, Delay* pdelay) :
    m_penv(penv),
    m_str(str),
    m_pdelay(pdelay)
{
}

Delay* Environment::Find(const ZString& str)
{
    if (m_str == str) {
        return m_pdelay;
    } else if (m_penv != NULL) {
        return m_penv->Find(str);
    }

    return NULL;
}

Delay* Environment::GetArgument(int index)
{
    if (index == 1) {
        return m_pdelay;
    } else {
        return m_penv->GetArgument(index - 1);
    }
}
