/***************************************************************
 * Name:      ftcDemoApp.h
 * Purpose:   Defines Application Class
 * Author:    R. Trapp (rene <dot> trapp [ät] web <dot> de)
 * Created:   2023-01-19
 * Copyright: R. Trapp ()
 * License:   GPLv3 - see License.txt
 **************************************************************/

#ifndef FTCDEMOAPP_H
#define FTCDEMOAPP_H

#include <wx/app.h>


class MyThread;
WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);


class ftcDemoApp : public wxApp
{
   public:
      virtual bool OnInit();

      // critical section protects access to all of the fields below
      wxCriticalSection m_critsect;

      // all the threads currently alive - as soon as the thread terminates,
      // it's removed from the array
      wxArrayThread m_threads;

      // semaphore used to wait for the threads to exit, see
      // ftcDemoFrame::OnQuit()
      wxSemaphore m_semAllDone;

      // indicates that we're shutting down and all threads should exit
      bool m_shuttingDown;
};

#endif // FTCDEMOAPP_H
