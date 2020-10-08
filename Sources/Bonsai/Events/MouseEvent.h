#ifndef BONSAI_MOUSEEVENT_H_
#define BONSAI_MOUSEEVENT_H_

#include "Export.h"
#include "Event.h"
#include "Bonsai/Utils/MouseCodes.h"

namespace Bonsai
{
    class BONSAI_EXPORT MouseButtonEvent : public Event
    {
    public:
        MouseCode GetMouseButton() const { return _mouseButton; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

    protected:
        explicit MouseButtonEvent(const MouseCode mouseButton) : _mouseButton(mouseButton) { }

        MouseCode _mouseButton;
    };

    class BONSAI_EXPORT MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(const MouseCode mouseButton) : MouseButtonEvent(mouseButton) { }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << _mouseButton;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class BONSAI_EXPORT MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(const MouseCode mouseButton) : MouseButtonEvent(mouseButton) { }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << _mouseButton;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

    class BONSAI_EXPORT MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset) : _xOffset(xOffset), _yOffset(yOffset) { }

        float GetXOffset() const { return _xOffset; }

        float GetYOffset() const { return _yOffset; }

        EVENT_CLASS_TYPE(MouseScrolled)

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float _xOffset, _yOffset;
    };

    class BONSAI_EXPORT MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const float mouseX, const float mouseY) : _mouseX(mouseX), _mouseY(mouseY) { }

        float GetX() const { return _mouseX; }

        float GetY() const { return _mouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << _mouseX << " " << _mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float _mouseX, _mouseY;
    };

}

#endif
