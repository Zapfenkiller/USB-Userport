// ----------------------------------------------------------------------------
// a worker thread
// taken from wxWidgets samples/thread.cpp
// ----------------------------------------------------------------------------

#ifdef WX_PRECOMP
   #include "wx_pch.h"
#else
   #include <wx/wx.h>
#endif
#include "MyWorkerThread.h"

MyWorkerThread::MyWorkerThread(HWwxw305Frame *frame)
        : wxThread()
{
    m_frame = frame;
    m_count = 0;
}

void MyWorkerThread::OnExit()
{
}

wxThread::ExitCode MyWorkerThread::Entry()
{
    for (m_count = 0; !m_frame->Cancelled() && (m_count < 100); m_count++)
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;

        // create any type of command event here
        wxThreadEvent event(wxEVT_THREAD, HWwxw305Frame::WORKER_EVENT);
        event.SetInt( m_count+1 );

        // send in a thread-safe way
        wxQueueEvent( m_frame, event.Clone() );

        wxMilliSleep(100);
    }

    wxThreadEvent event(wxEVT_THREAD, HWwxw305Frame::WORKER_EVENT);
    event.SetInt(-1); // that's all
    wxQueueEvent( m_frame, event.Clone() );

    return NULL;
}

MyWorkerThread2::MyWorkerThread2(HWwxw305Frame *frame)
        : wxThread()
{
    m_frame = frame;
    m_count = 0;
}

void MyWorkerThread2::OnExit()
{
}

wxThread::ExitCode MyWorkerThread2::Entry()
{
    for (m_count = 0; m_count < 50; m_count++)
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;

        // create any type of command event here
        wxThreadEvent event(wxEVT_THREAD, HWwxw305Frame::WORKER_EVENT2);
        event.SetInt( m_count+1 );

        // send in a thread-safe way
        wxQueueEvent( m_frame, event.Clone() );

        wxMilliSleep(200);
    }

    wxMilliSleep(200);

    wxThreadEvent event(wxEVT_THREAD, HWwxw305Frame::WORKER_EVENT2);
    event.SetInt(-1); // that's all
    wxQueueEvent( m_frame, event.Clone() );

    return NULL;
}
