//////////////////////////////////////////////////////////////////////////////
//
// Test file
//
//////////////////////////////////////////////////////////////////////////////

class Foo {
private:
    int m_value;

public:
    Foo(int value) :
        m_value(value)
    {
    }

    int Twice()
    {
        int y = m_value * m_value;

        return y;
    }
};

int main()
{
    return Foo(3).Twice();
}
