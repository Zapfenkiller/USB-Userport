// ----------------------------------------------------------------------------
// a worker thread
// taken from wxWidgets samples/thread.cpp
// ----------------------------------------------------------------------------

#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif
#include "HWwxw305Main.h"

class MyWorkerThread : public wxThread
{
public:
    MyWorkerThread(HWwxw305Frame *frame);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
    HWwxw305Frame *m_frame;
    unsigned m_count;
};


class MyWorkerThread2 : public wxThread
{
public:
    MyWorkerThread2(HWwxw305Frame *frame);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
    HWwxw305Frame *m_frame;
    unsigned m_count;
};
