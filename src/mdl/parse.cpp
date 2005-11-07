#include "pch.h"
#include "ast.h"
#include "main.h"

//////////////////////////////////////////////////////////////////////////////
//
// UnitValue
//
//////////////////////////////////////////////////////////////////////////////

class UnitValue : public Value {
public:
    UnitValue()
    {
    }

    ZString GetString()
    {
        return "value";
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// NumberValue
//
//////////////////////////////////////////////////////////////////////////////

class NumberValue : public Value {
private:
    float m_value;

public:
    NumberValue(float value) :
        m_value(value)
    {
    }

    float GetValue()
    {
        return m_value;
    }

    ZString GetString()
    {
        return m_value;
    }

    TRef<Type> GetType()
    {
        return new BaseType("number");
    }

    static TRef<Type> StaticGetType()
    {
        return new BaseType("number");
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// BooleanValue
//
//////////////////////////////////////////////////////////////////////////////

class BooleanValue : public Value {
private:
    bool m_value;

public:
    BooleanValue(bool value) :
        m_value(value)
    {
    }

    bool GetValue()
    {
        return m_value;
    }

    ZString GetString()
    {
        return m_value;
    }

    TRef<Type> GetType()
    {
        return new BaseType("boolean");
    }

    static TRef<Type> StaticGetType()
    {
        return new BaseType("boolean");
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// StringValue
//
//////////////////////////////////////////////////////////////////////////////

class StringValue : public Value {
private:
    ZString m_value;

public:
    StringValue(const ZString& value) :
        m_value(value)
    {
    }

    const ZString& GetValue()
    {
        return m_value;
    }

    ZString GetString()
    {
        return "\"" + m_value + "\"";
    }

    TRef<Type> GetType()
    {
        return new BaseType("string");
    }

    static TRef<Type> StaticGetType()
    {
        return new BaseType("string");
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// PairValue
//
//////////////////////////////////////////////////////////////////////////////

class PairValue : public Value {
private:
    TRef<Delay> m_pdelayFirst;
    TRef<Delay> m_pdelaySecond;

public:
    PairValue(Delay* pdelayFirst, Delay* pdelaySecond) :
        m_pdelayFirst(pdelayFirst),
        m_pdelaySecond(pdelaySecond)
    {
    }

    Delay* GetFirst()
    {
        return m_pdelayFirst;
    }

    Delay* GetSecond()
    {
        return m_pdelaySecond;
    }

    ZString GetString()
    {
        return
              m_pdelayFirst->Evaluate()->GetString()
            + ", "
            + m_pdelaySecond->Evaluate()->GetString();
    }

    TRef<Type> GetType()
    {
        return
            new PairType(
                m_pdelayFirst->GetType(),
                m_pdelaySecond->GetType()
            );
    }

    static TRef<Type> StaticGetType()
    {
        return
            new PairType(
                new PolymorphicType(),
                new PolymorphicType()
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// ConsValue
//
//////////////////////////////////////////////////////////////////////////////

class ConsValue : public Value {
private:
    TRef<Delay> m_pdelayHead;
    TRef<Delay> m_pdelayTail;

public:
    ConsValue(Delay* pdelayHead, Delay* pdelayTail) :
        m_pdelayHead(pdelayHead),
        m_pdelayTail(pdelayTail)
    {
    }

    Delay* GetHead()
    {
        return m_pdelayHead;
    }

    Delay* GetTail()
    {
        return m_pdelayTail;
    }

    ZString GetString()
    {
        if (m_pdelayHead == NULL) {
            return "[]";
        } else {
            ZString str = "[";
            TRef<ConsValue> pcons = this;

            while (true) {
                str += pcons->m_pdelayHead->Evaluate()->GetString();
                CastTo(pcons, pcons->m_pdelayTail->Evaluate());
                if (pcons->m_pdelayHead == NULL) {
                    return str + "]";
                } else {
                    str += ", ";
                }
            }
        }
    }

    TRef<Type> GetType()
    {
        if (m_pdelayHead == NULL) {
            return ConsValue::StaticGetType();
        } else {
            return new ListType(m_pdelayHead->GetType());
        }
    }

    static TRef<Type> StaticGetType()
    {
        return new ListType(new PolymorphicType());
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// BooleanExpression
//
//////////////////////////////////////////////////////////////////////////////

class BooleanExpression : public Expression {
public:
    TRef<Delay> Evaluate(Environment* penv, bool b)
    {
        return CreateDelay(penv, CreateValueExpression(new BooleanValue(b)));
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x -> NotExpression;
//
//////////////////////////////////////////////////////////////////////////////

class NotExpression : public BooleanExpression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(BooleanValue::StaticGetType()));

        return BooleanValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<BooleanValue> pvalue1; CastTo(pvalue1, penv->GetArgument(1)->Evaluate());

        return BooleanExpression::Evaluate(penv, !pvalue1->GetValue());
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> EqualExpression;
//
//////////////////////////////////////////////////////////////////////////////

class EqualNumberExpression : public BooleanExpression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(2)->GetType()->IsMatch(NumberValue::StaticGetType()));
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(NumberValue::StaticGetType()));

        return BooleanValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<NumberValue> pvalue1; CastTo(pvalue1, penv->GetArgument(2)->Evaluate());
        TRef<NumberValue> pvalue2; CastTo(pvalue2, penv->GetArgument(1)->Evaluate());

        return BooleanExpression::Evaluate(penv, pvalue1->GetValue() == pvalue2->GetValue());
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> MultiplyExpression;
//
//////////////////////////////////////////////////////////////////////////////

class MultiplyExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(NumberValue::StaticGetType()));
        ZVerify(penv->GetArgument(2)->GetType()->IsMatch(NumberValue::StaticGetType()));

        return NumberValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<NumberValue> pvalue1; CastTo(pvalue1, penv->GetArgument(2)->Evaluate());
        TRef<NumberValue> pvalue2; CastTo(pvalue2, penv->GetArgument(1)->Evaluate());

        return
            CreateDelay(
                NULL,
                CreateValueExpression(
                    new NumberValue(pvalue1->GetValue() * pvalue2->GetValue())
                )
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> DivideExpression;
//
//////////////////////////////////////////////////////////////////////////////

class DivideExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(NumberValue::StaticGetType()));
        ZVerify(penv->GetArgument(2)->GetType()->IsMatch(NumberValue::StaticGetType()));

        return NumberValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<NumberValue> pvalue1; CastTo(pvalue1, penv->GetArgument(2)->Evaluate());
        TRef<NumberValue> pvalue2; CastTo(pvalue2, penv->GetArgument(1)->Evaluate());

        return
            CreateDelay(
                NULL,
                CreateValueExpression(
                    new NumberValue(pvalue1->GetValue() / pvalue2->GetValue())
                )
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> ModExpression;
//
//////////////////////////////////////////////////////////////////////////////

class ModExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(NumberValue::StaticGetType()));
        ZVerify(penv->GetArgument(2)->GetType()->IsMatch(NumberValue::StaticGetType()));

        return NumberValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<NumberValue> pvalue1; CastTo(pvalue1, penv->GetArgument(2)->Evaluate());
        TRef<NumberValue> pvalue2; CastTo(pvalue2, penv->GetArgument(1)->Evaluate());

        return
            CreateDelay(
                NULL,
                CreateValueExpression(
                    new NumberValue(mod(pvalue1->GetValue(), pvalue2->GetValue()))
                )
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> AddExpression;
//
//////////////////////////////////////////////////////////////////////////////

class AddExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(NumberValue::StaticGetType()));
        ZVerify(penv->GetArgument(2)->GetType()->IsMatch(NumberValue::StaticGetType()));

        return NumberValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<NumberValue> pvalue1; CastTo(pvalue1, penv->GetArgument(2)->Evaluate());
        TRef<NumberValue> pvalue2; CastTo(pvalue2, penv->GetArgument(1)->Evaluate());

        return
            CreateDelay(
                NULL,
                CreateValueExpression(
                    new NumberValue(pvalue1->GetValue() + pvalue2->GetValue())
                )
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> SubtractExpression;
//
//////////////////////////////////////////////////////////////////////////////

class SubtractExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(NumberValue::StaticGetType()));
        ZVerify(penv->GetArgument(2)->GetType()->IsMatch(NumberValue::StaticGetType()));

        return NumberValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<NumberValue> pvalue1; CastTo(pvalue1, penv->GetArgument(2)->Evaluate());
        TRef<NumberValue> pvalue2; CastTo(pvalue2, penv->GetArgument(1)->Evaluate());

        return
            CreateDelay(
                NULL,
                CreateValueExpression(
                    new NumberValue(pvalue1->GetValue() - pvalue2->GetValue())
                )
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function condition then else -> IfExpression;
//
//////////////////////////////////////////////////////////////////////////////

class IfExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        ZVerify(penv->GetArgument(1)->GetType()->IsMatch(penv->GetArgument(2)->GetType()));
        ZVerify(penv->GetArgument(3)->GetType()->IsMatch(BooleanValue::StaticGetType()));

        return penv->GetArgument(1)->GetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<BooleanValue> pbool; 
        CastTo(pbool, penv->GetArgument(3)->Evaluate());

        if (pbool->GetValue()) {
            return penv->GetArgument(2);
        } else {
            return penv->GetArgument(1);
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x -> FirstExpression;
//
//////////////////////////////////////////////////////////////////////////////

class FirstExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type> ptype = penv->GetArgument(1)->GetType();
        ZVerify(ptype->IsMatch(PairValue::StaticGetType()));
        return ptype->GetPairType()->GetFirstType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<PairValue> ppair; CastTo(ppair, penv->GetArgument(1)->Evaluate());

        return ppair->GetFirst();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x -> SecondExpression;
//
//////////////////////////////////////////////////////////////////////////////

class SecondExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type> ptype = penv->GetArgument(1)->GetType();
        ZVerify(ptype->IsMatch(PairValue::StaticGetType()));
        return ptype->GetPairType()->GetSecondType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<PairValue> ppair; CastTo(ppair, penv->GetArgument(1)->Evaluate());

        return ppair->GetSecond();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> PairExpression;
//
//////////////////////////////////////////////////////////////////////////////

class PairExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        return
            new PairType(
                penv->GetArgument(2)->GetType(),
                penv->GetArgument(1)->GetType()
            );
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        return
            CreateDelay(
                NULL,
                CreateValueExpression(
                    new PairValue(
                        penv->GetArgument(2),
                        penv->GetArgument(1)
                    )
                )
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function cons -> EmptyExpression;
//
//////////////////////////////////////////////////////////////////////////////

class EmptyExpression : public BooleanExpression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type> ptype = penv->GetArgument(1)->GetType();
        ZVerify(ptype->IsMatch(ConsValue::StaticGetType()));

        return BooleanValue::StaticGetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<ConsValue> pcons; CastTo(pcons, penv->GetArgument(1)->Evaluate());
        return BooleanExpression::Evaluate(penv, pcons->GetHead() == NULL);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x -> HeadExpression;
//
//////////////////////////////////////////////////////////////////////////////

class HeadExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type> ptype = penv->GetArgument(1)->GetType();
        ZVerify(ptype->IsMatch(ConsValue::StaticGetType()));
        return ptype->GetListType()->GetType();
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<ConsValue> pcons; CastTo(pcons, penv->GetArgument(1)->Evaluate());

        return pcons->GetHead();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x -> TailExpression;
//
//////////////////////////////////////////////////////////////////////////////

class TailExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type> ptype = penv->GetArgument(1)->GetType();
        ZVerify(ptype->IsMatch(ConsValue::StaticGetType()));
        return ptype;
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        TRef<ConsValue> pcons; CastTo(pcons, penv->GetArgument(1)->Evaluate());

        return pcons->GetTail();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// function x y -> ConsExpression;
//
//////////////////////////////////////////////////////////////////////////////

class ConsExpression : public Expression {
public:
    TRef<Type> GetType(Environment* penv)
    {
        TRef<Type> ptypeHead = penv->GetArgument(2)->GetType();
        TRef<Type> ptypeTail = penv->GetArgument(1)->GetType();

        ZVerify(ptypeTail->IsMatch(ConsValue::StaticGetType()));
        ZVerify(ptypeHead->IsMatch(ptypeTail->GetListType()->GetType()));

        return ptypeTail;
    }

    TRef<Delay> Evaluate(Environment* penv)
    {
        return
            CreateDelay(
                NULL,
                CreateValueExpression(
                    new ConsValue(
                        penv->GetArgument(2),
                        penv->GetArgument(1)
                    )
                )
            );
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// MDLParser
//
//////////////////////////////////////////////////////////////////////////////

class MDLParser : public TextTokenImpl {
public:
    int Comma        ;
    int SemiColon    ;
    int LeftParen    ;
    int RightParen   ;
    int LeftBracket  ;
    int RightBracket ;
    int Arrow        ;
    int Equals       ;
    int NotEquals    ;
    int Less         ;
    int LessEquals   ;
    int Greater      ;
    int GreaterEquals;
    int Plus         ;
    int Minus        ;
    int Multiply     ;
    int Divide       ;
    int Cons         ;
    int Period       ;

    int Let          ;
    int In           ;
    int Function     ;
    int If           ;
    int Then         ;
    int Else         ;

    TRef<Expression> m_pexprEqual;
    TRef<Expression> m_pexprMultiply;
    TRef<Expression> m_pexprDivide;
    TRef<Expression> m_pexprAdd;
    TRef<Expression> m_pexprSubtract;
    TRef<Expression> m_pexprCons;
    TRef<Expression> m_pexprPair;
    TRef<Expression> m_pexprEmptyList;
    TRef<Expression> m_pexprIf;

    MDLParser(PCC pcc, int length) :
        TextTokenImpl(pcc, length)
    {
        Comma        = AddToken("','");
        SemiColon    = AddToken("';'");
        LeftParen    = AddToken("'('");
        RightParen   = AddToken("')'");
        LeftBracket  = AddToken("'['");
        RightBracket = AddToken("']'");
        Arrow        = AddToken("'->'");
        Equals       = AddToken("'='");
        NotEquals    = AddToken("'!='");
        Less         = AddToken("'<'");
        LessEquals   = AddToken("'<='");
        Greater      = AddToken("'>'");
        GreaterEquals= AddToken("'>='");
        Plus         = AddToken("'+'");
        Minus        = AddToken("'-'");
        Multiply     = AddToken("'*'");
        Divide       = AddToken("'/'");
        Cons         = AddToken("'::'");
        Period       = AddToken("'.'");

        Let          = AddSymbol("let");
        In           = AddSymbol("in");
        Function     = AddSymbol("function");
        If           = AddSymbol("if");
        Then         = AddSymbol("then");
        Else         = AddSymbol("else");

        Next();

        m_pexprEqual     = CreateSymbolExpression("equal");
        m_pexprMultiply  = CreateSymbolExpression("mul");
        m_pexprDivide    = CreateSymbolExpression("div");
        m_pexprAdd       = CreateSymbolExpression("add");
        m_pexprSubtract  = CreateSymbolExpression("sub");
        m_pexprCons      = CreateSymbolExpression("cons");
        m_pexprPair      = CreateSymbolExpression("pair");
        m_pexprEmptyList = CreateSymbolExpression("emptyList");
        m_pexprIf        = CreateSymbolExpression("if");
    }

    int ParseToken(PCC& pcc)
    {
        switch (pcc[0]) {
            case '[': pcc++; return LeftBracket;
            case ']': pcc++; return RightBracket;
            case '(': pcc++; return LeftParen;
            case ')': pcc++; return RightParen;
            case ',': pcc++; return Comma;
            case ';': pcc++; return SemiColon;
            case '=': pcc++; return Equals;
            case '+': pcc++; return Plus;
            case '*': pcc++; return Multiply;
            case '/': pcc++; return Divide;
            case '^': pcc++; return Period;

            case ':':
                if (pcc[1] == ':') {
                    pcc += 2;
                    return Cons;
                }

            case '!':
                if (pcc[1] == '=') {
                    pcc += 2;
                    return NotEquals;
                }
                return Null;

            case '-':
                if (pcc[1] == '>') {
                    pcc += 2;
                    return Arrow;
                } else {
                    pcc++;
                    return Minus;
                }

            case '<':
                if (pcc[0] == '=') {
                    pcc += 2;
                    return LessEquals;
                } else {
                    pcc++;
                    return Less;
                }

            case '>':
                if (pcc[0] == '=') {
                    pcc += 2;
                    return GreaterEquals;
                } else {
                    pcc++;
                    return Greater;
                }
        }

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Parser
    //
    //////////////////////////////////////////////////////////////////////////////

    bool ReadFunction(TRef<Expression>& pexprOut, int tokenID)
    {
        ZString str;
        TRef<Expression> pexpr;

        if (IsSymbol(str, true)) {
            if (Is(tokenID, false)) {
                if (!ReadExpression(pexpr, true, true)) {
                    return false;
                }
            } else if (!ReadFunction(pexpr, tokenID)) {
                return false;
            }

            pexprOut = CreateFunctionExpression(str, pexpr);
            return true;
        }

        return false;
    }

    bool ReadLet(TRef<Expression>& pexpr)
    {
        TRef<Environment> penv;

        if (ReadDefinitions(penv, NULL, true)) {
            if (Is(In, true)) {
                TRef<Expression> pexprIn;

                if (ReadExpression(pexprIn, true, true)) {
                    pexpr = CreateLetExpression(penv, pexprIn);
                    return true;
                }
            }
        }

        return false;
    }

    bool ReadIf(TRef<Expression>& pexpr)
    {
        TRef<Expression> pexprCondition;

        if (ReadExpression(pexprCondition, true, true)) {
            if (Is(Then, true)) {
                TRef<Expression> pexprThen;
                if (ReadExpression(pexprThen, true, true)) {
                    if (Is(Else, true)) {
                        TRef<Expression> pexprElse;
                        if (ReadExpression(pexprElse, true, true)) {
                            pexpr =
                                CreateApplyExpression(
                                    CreateApplyExpression(
                                        CreateApplyExpression(
                                            m_pexprIf,
                                            pexprCondition
                                        ),
                                        pexprThen
                                    ),
                                    pexprElse
                                 );
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    bool ReadList(TRef<Expression>& pexpr)
    {
        if (Is(RightBracket, false)) {
            pexpr = m_pexprEmptyList;
            return true;
        } else if (ReadExpression(pexpr, false, true)) {
            if (Is(Comma, false)) {
                TRef<Expression> pexpr2;

                if (ReadList(pexpr2)) {
                    pexpr =
                        CreateApplyExpression(
                            CreateApplyExpression(
                                m_pexprCons,
                                pexpr
                            ),
                            pexpr2
                        );
                    return true;
                }
            } else if (Is(RightBracket, true)) {
                pexpr =
                    CreateApplyExpression(
                        CreateApplyExpression(
                            m_pexprCons,
                            pexpr
                        ),
                        m_pexprEmptyList
                    );
                return true;
            }
        }

        return false;
    }

    bool ReadTerminal(TRef<Expression>& pexpr, bool bError)
    {
        ZString str;
        float value;

        if (IsSymbol(str, false)) {
            pexpr = CreateSymbolExpression(str);
            return true;
        } else if (IsNumber(value, false)) {
            pexpr = CreateValueExpression(new NumberValue(value));
            return true;
        } else if (IsString(str, false)) {
            pexpr = CreateValueExpression(new StringValue(str));
            return true;
        } else if (Is(LeftParen, false)) {
            if (ReadExpression(pexpr, true, false)) {
                if (Is(RightParen, true)) {
                    return true;
                }
            } else if (Is(RightParen, true)) {
                pexpr = CreateValueExpression(new UnitValue());
                return true;
            }
        } else if (Is(Let, false)) {
            return ReadLet(pexpr);
        } else if (Is(If, false)) {
            return ReadIf(pexpr);
        } else if (Is(Function, false)) {
            return ReadFunction(pexpr, Arrow);
        } else if (Is(LeftBracket, false)) {
            return ReadList(pexpr);
        }

        if (bError) {
            SetError("Expected Terminal");
        }

        return bError;
    }

    // a.b.c ==> c (b a)

    bool ReadMember(TRef<Expression>& pexpr, bool bError)
    {
        if (ReadTerminal(pexpr, bError)) {
            while (true) {
                if (Is(Period, false)) {
                    TRef<Expression> pexpr2;

                    if (ReadTerminal(pexpr2, true)) {
                        pexpr = CreateApplyExpression(pexpr2, pexpr);
                        continue;
                    }
                }

                return true;
            }
        }

        return false;
    }

    bool ReadMultiply(TRef<Expression>& pexpr, bool bError)
    {
        if (ReadMember(pexpr, bError)) {
            while (true) {
                if (Is(Multiply, false)) {
                    pexpr =
                        CreateApplyExpression(
                            m_pexprMultiply,
                            pexpr
                        );

                    TRef<Expression> pexpr2;

                    if (ReadMember(pexpr2, false)) {
                        pexpr = CreateApplyExpression(pexpr, pexpr2);
                        continue;
                    }
                } else if (Is(Divide, false)) {
                    pexpr =
                        CreateApplyExpression(
                            m_pexprDivide,
                            pexpr
                        );

                    TRef<Expression> pexpr2;

                    if (ReadMember(pexpr2, false)) {
                        pexpr = CreateApplyExpression(pexpr, pexpr2);
                        continue;
                    }
                }

                return true;
            }
        }

        return false;
    }

    bool ReadAddition(TRef<Expression>& pexpr, bool bError)
    {
        if (ReadMultiply(pexpr, bError)) {
            while (true) {
                if (Is(Plus, false)) {
                    pexpr =
                        CreateApplyExpression(
                            m_pexprAdd,
                            pexpr
                        );

                    TRef<Expression> pexpr2;

                    if (ReadMultiply(pexpr2, false)) {
                        pexpr = CreateApplyExpression(pexpr, pexpr2);
                        continue;
                    }
                } else if (Is(Minus, false)) {
                    pexpr =
                        CreateApplyExpression(
                            m_pexprSubtract,
                            pexpr
                        );

                    TRef<Expression> pexpr2;

                    if (ReadMultiply(pexpr2, false)) {
                        pexpr = CreateApplyExpression(pexpr, pexpr2);
                        continue;
                    }
                }

                return true;
            }
        }

        return false;
    }

    bool ReadApply(TRef<Expression>& pexpr, bool bError)
    {
        if (ReadAddition(pexpr, bError)) {
            while (true) {
                TRef<Expression> pexpr2;

                if (ReadAddition(pexpr2, false)) {
                    pexpr = CreateApplyExpression(pexpr, pexpr2);
                    continue;
                }

                return true;
            }
        }

        return false;
    }

    bool ReadCons(TRef<Expression>& pexpr, bool bError)
    {
        if (ReadApply(pexpr, bError)) {
            if (Is(Cons, false)) {
                pexpr =
                    CreateApplyExpression(
                        m_pexprCons,
                        pexpr
                    );

                TRef<Expression> pexpr2;

                if (ReadCons(pexpr2, true)) {
                    pexpr = CreateApplyExpression(pexpr, pexpr2);
                    return true;
                }

                return false;
            }

            return true;
        }

        return false;
    }

    bool ReadPair(TRef<Expression>& pexpr, bool bAllowPairs, bool bError)
    {
        if (bAllowPairs) {
            if (ReadCons(pexpr, bError)) {
                if (Is(Comma, false)) {
                    pexpr =
                        CreateApplyExpression(
                            m_pexprPair,
                            pexpr
                        );

                    TRef<Expression> pexpr2;

                    if (ReadPair(pexpr2, true, true)) {
                        pexpr = CreateApplyExpression(pexpr, pexpr2);
                        return true;
                    }

                    return false;
                }

                return true;
            }

            return false;
        } else {
            return ReadCons(pexpr, bError);
        }
    }

    bool ReadExpression(TRef<Expression>& pexpr, bool bAllowPairs, bool bError)
    {
        return ReadPair(pexpr, bAllowPairs, bError);
    }
    
    bool ReadDefinitions(TRef<Environment>& penvOut, Environment* penvIn, bool bError)
    {
        TRef<Environment> penvTop = new Environment(NULL, "file", NULL);

        penvOut = penvIn;

        while (true) {
            ZString str;

            if (IsSymbol(str, false)) {
                TRef<Expression> pexpr;

                if (Is(Equals, false)) {
                    if (ReadExpression(pexpr, true, true)) {
                        if (Is(SemiColon, true)) {
                            penvOut =
                                new Environment(
                                    penvOut,
                                    str,
                                    CreateDelay(
                                        penvTop,
                                        pexpr
                                    )
                                );

                            continue;
                        }
                    }
                } else if (ReadFunction(pexpr, Equals)) {
                    if (Is(SemiColon, true)) {
                        penvOut =
                            new Environment(
                                penvOut,
                                str,
                                CreateDelay(
                                    penvTop,
                                    pexpr
                                )
                            );

                        continue;
                    }
                }

                return false;
            }

            if (penvOut != penvIn) {
                penvTop->SetNext(penvOut);
                penvOut = penvTop;

                return true;
            }

            return false;
        }
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Application
//
//////////////////////////////////////////////////////////////////////////////

class MDLApp : public Win32App {
private:
    TRef<Environment> m_penv;

public:
    MDLApp()
    {
        AddValue("emptyList", new ConsValue(NULL, NULL));
        AddValue("true",      new BooleanValue(true));
        AddValue("false",     new BooleanValue(false));

        AddFunction("not",    new NotExpression(),         "x");

        AddFunction("equal",  new EqualNumberExpression(), "x",    "y");
        AddFunction("mul",    new MultiplyExpression(),    "x",    "y");
        AddFunction("div",    new DivideExpression(),      "x",    "y");
        AddFunction("mod",    new ModExpression(),         "x",    "y");
        AddFunction("add",    new AddExpression(),         "x",    "y");
        AddFunction("sub",    new SubtractExpression(),    "x",    "y");

        AddFunction("cons",   new ConsExpression(), "head", "tail");
        AddFunction("head",   new HeadExpression(), "cons");
        AddFunction("tail",   new TailExpression(), "cons");
        AddFunction("empty",  new EmptyExpression(), "cons");

        AddFunction("pair",   new PairExpression(),   "first", "second");
        AddFunction("first",  new FirstExpression(),  "first");
        AddFunction("second", new SecondExpression(), "second");

        AddFunction("if",     new IfExpression(), "condition", "then", "else");
    }

    void AddValue(const ZString& strName, Value* pvalue)
    {
        m_penv =
            new Environment(
                m_penv,
                strName,
                CreateDelay(
                    NULL, 
                    CreateValueExpression(
                        pvalue
                    )
                )
            );
    }
    
    void AddFunction(const ZString& strName, Expression* pexpr, const ZString& str1)
    {
        m_penv = 
            new Environment(
                m_penv,
                strName,
                CreateDelay(
                    NULL,
                    CreateFunctionExpression(
                        str1,
                        pexpr
                    )
                )
            );
    }

    void AddFunction(const ZString& strName, Expression* pexpr, const ZString& str1, const ZString& str2)
    {
        m_penv = 
            new Environment(
                m_penv,
                strName,
                CreateDelay(
                    NULL,
                    CreateFunctionExpression(
                        str1,
                        CreateFunctionExpression(
                            str2,
                            pexpr
                        )
                    )
                )
            );
    }

    void AddFunction(const ZString& strName, Expression* pexpr, const ZString& str1, const ZString& str2, const ZString& str3)
    {
        m_penv = 
            new Environment(
                m_penv,
                strName,
                CreateDelay(
                    NULL,
                    CreateFunctionExpression(
                        str1,
                        CreateFunctionExpression(
                            str2,
                            CreateFunctionExpression(
                                str3,
                                pexpr
                            )
                        )
                    )
                )
            );
    }

    void Output(const ZString& str)
    {
        OutputDebugStringA((PCC)str);
    }

    void Open(const ZString& str)
    {
        TRef<ZFile> pfile = new ZFile(str);

        if (pfile) {
            MDLParser parser((PCC)pfile->GetPointer(), pfile->GetLength());

            TRef<Environment> penv;
            if (parser.ReadDefinitions(penv, m_penv, false)) {
                TRef<Delay> pdelay = penv->Find("result");
                TRef<Type>  ptype  = pdelay->GetType();
                TRef<Value> pvalue = pdelay->Evaluate();

                int id = 'a';
                Output(ptype->GetString(id) + "\n");
                Output(pvalue->GetString() + "\n");
            } else if (parser.Error()) {
                // !!! there was an error in the file
            }
        }
    }

    HRESULT Initialize(const ZString& strCommandLine)
    {
        PCC pcc = strCommandLine;
        CommandLineToken token(pcc, strCommandLine.GetLength());

        while (token.MoreTokens()) {
            ZString strInput;
            ZString strOutput;

            if (token.Is(token.Minus, false)) {
                ZString str;

                if (token.IsSymbol(str, true)) {
                }
            } else if (token.IsPath(strInput, true)) {
                Open(strInput);
                return S_FALSE;
            }
            break;
        }

        printf("Usage: MDL filename\n");

        return S_FALSE;
    }
} g_app;
