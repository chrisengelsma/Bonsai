#ifndef BONSAI_APPLICATION_H_
#define BONSAI_APPLICATION_H_

#include "Log.h"
#include "Export.h"

namespace Bonsai
{
    class BONSAI_EXPORT Application
    {
    public:
        Application();
        virtual ~Application() = 0;

        void Run();
    };

    // Client it responsible to define this bad boy.
    Application* CreateApplication();
}

#endif
