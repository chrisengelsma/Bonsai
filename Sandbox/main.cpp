#include <Bonsai.h>

class Sandbox : public Bonsai::Application
{
public:
    Sandbox()
    {

    }

    ~Sandbox()
    {

    }
};

Bonsai::Application* Bonsai::CreateApplication()
{
    return new Sandbox();
}

