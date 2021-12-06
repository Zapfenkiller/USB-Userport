/***************************************************************
 * Name:      DemoApp.h
 * Purpose:   Defines the Application Class
 * Author:    R. Trapp
 * Created:   2021-12-03
 * Copyright: R. Trapp
 * License:   GPLv3 - see License.txt
 **************************************************************/

#ifndef DEMOAPP_H
#define DEMOAPP_H


#include <wx/app.h>


class MyThread;
WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);


class DemoApp : public wxApp
{
   public:
      virtual bool OnInit();

      // critical section protects access to all of the fields below
      wxCriticalSection m_critsect;

      // all the threads currently alive - as soon as the thread terminates, it's
      // removed from the array
      wxArrayThread m_threads;

      // semaphore used to wait for the threads to exit, see MyFrame::OnQuit()
      wxSemaphore m_semAllDone;

      // indicates that we're shutting down and all threads should exit
      bool m_shuttingDown;
};


#endif // DEMOAPP_H
