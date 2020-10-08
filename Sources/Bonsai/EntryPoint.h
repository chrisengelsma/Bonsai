#ifndef BONSAI_ENTRYPOINT_H_
#define BONSAI_ENTRYPOINT_H_

extern Bonsai::Application* Bonsai::CreateApplication();

int main(int argc, char** argv)
{
    Bonsai::Log::Init();
    BONSAI_CORE_WARN("Welcome to Bonsai!");
    int a = 5;
    BONSAI_CORE_TRACE(a);


    auto app = Bonsai::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif
