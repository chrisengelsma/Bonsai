#ifndef BONSAI_KEYEVENT_H_
#define BONSAI_KEYEVENT_H_

#include <sstream>

#include "Export.h"
#include "Event.h"

namespace Bonsai
{
    class BONSAI_EXPORT KeyEvent : public Event
    {
    public:
        inline int GetKeyCode() const { return _keyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        explicit KeyEvent(int keyCode) : _keyCode(keyCode) { }

        int _keyCode;
    };

    class BONSAI_EXPORT KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), _repeatCount(repeatCount) { }

        inline int GetRepeatCount() const { return _repeatCount; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int _repeatCount;
    };

    class BONSAI_EXPORT KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) { }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << _keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

}

#endif
