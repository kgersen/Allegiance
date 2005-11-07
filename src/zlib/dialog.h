#ifndef _dialog_h_
#define _dialog_h_

//////////////////////////////////////////////////////////////////////////////
//
// Dialog
//
//////////////////////////////////////////////////////////////////////////////

class Dialog : public Window 
{
public:
    Dialog(Window* pwndParent, int nResourceID);
    ~Dialog();

    virtual BOOL Create();
    virtual void Destroy();

protected:
    static BOOL CALLBACK Win32DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    virtual DWORD WndProc(UINT message, WPARAM wParam, LPARAM lParam);

    // overridable message handles
    virtual BOOL OnInitDialog();
    virtual BOOL OnNotify(HWND hwndFrom, UINT idFrom, UINT code, LPARAM lParam, LRESULT* pResult);
    virtual void OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpdis);


    // Dialog Hook functions
    static LRESULT CALLBACK DialogCreationHook(int code, WPARAM wParam, LPARAM lParam);
    void HookDialogCreate();
    BOOL UnhookDialogCreate();

protected:
    int    m_nResourceID;

};


#endif
