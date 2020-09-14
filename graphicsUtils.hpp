#include <SFML/Graphics.hpp>
#include <cstring>
#include <functional>
#include <list>
#include <optional>

class MouseInteractive {
    friend class MouseInteractiveEH;

   protected:
    std::list<MouseInteractive*>* containingList;
    std::list<MouseInteractive*>::iterator descriptor;
    void detach();

   public:
    bool isHovered;
    bool isPressed;
    virtual void onMousePress() = 0;
    virtual void onMouseRelease() = 0;
    virtual void onHoverEnter() = 0;
    virtual void onHoverExit() = 0;
    virtual bool isHoveringOver(sf::Vector2f pos) = 0;

    virtual ~MouseInteractive() {
        detach();
    }
};

void MouseInteractive::detach() {
    if (containingList) {
        containingList->erase(descriptor);
    }
}

class MouseInteractiveEH {
   private:
    std::list<MouseInteractive*> trackedObjects;

   public:
    // MouseInteractiveEH();                                            // Default constructor
    // MouseInteractiveEH(const MouseInteractiveEH& other);             // Copy constructor
    // MouseInteractiveEH& operator=(const MouseInteractiveEH& other);  // Copy assignment
    // MouseInteractiveEH(MouseInteractiveEH&& other);                  // Move constructor
    // MouseInteractiveEH& operator=(MouseInteractive&& other);         // Move assignment

    void handleEvent(const sf::Event& event);  // Handle event
    void attach(MouseInteractive& obj);        // Add object for handling
};

void MouseInteractiveEH::handleEvent(const sf::Event& event) {
    for (MouseInteractive* el : trackedObjects) {
        switch (event.type) {
            case sf::Event::MouseMoved:
                if (el->isHoveringOver(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)) && !el->isHovered) {
                    el->onHoverEnter();
                    el->isHovered = true;
                } else if (!el->isHoveringOver(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)) && el->isHovered && !el->isPressed) {
                    el->onHoverExit();
                    el->isHovered = false;
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (el->isHoveringOver(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    el->isPressed = true;
                    el->onMousePress();
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (el->isPressed) {
                    if (el->isHoveringOver(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        el->onMouseRelease();
                        el->isPressed = false;
                    } else {
                        el->onHoverExit();
                        el->isHovered = false;
                        el->isPressed = false;
                    }
                    el->isPressed = false;
                }
                break;

            default:
                break;
        }
    }
}

void MouseInteractiveEH::attach(MouseInteractive& obj) {
    trackedObjects.push_back(&obj);
    obj.containingList = &trackedObjects;
    obj.descriptor = --trackedObjects.end();
}

class Button : public sf::Drawable, public MouseInteractive {
   private:
    sf::RectangleShape bkgRect;
    sf::Text label;
    sf::Color defaultBkgColor;
    sf::Color hoverBkgColor;
    sf::Color clickBkgColor;

    sf::Color defaultTextColor;
    sf::Color clickTextColor;

    std::function<void(void)> callback;

    void moveLabelOriginToCenter();

   public:
    Button();                  // Default constructor
    Button(const char* text);  // Text-based constructor

    // Button(const Button& other);             // Copy constructor
    // Button& operator=(const Button& other);  // Copy assignment

    // Button(Button&& other);             // Move cosntructor
    // Button& operator=(Button&& other);  // Move assignment

    virtual ~Button();  // Destructor

    void setString(const char* text);                        // Function for replacing string
    void setString(const sf::String& text);                  // Overload for sf::String, purely for the sake of copy convenience
    void setFont(const sf::Font& font);                      // Set font for button label
    void setSize(const sf::Vector2f& size);                  // Set button size
    void setPosition(float x, float y);                      // Set position of button
    void setPosition(const sf::Vector2f& pos);               // setPosition overload for SFML 2D Vector type for the sake of convenience
    void setFontColor(const sf::Color& color);               // Set font color for the button
    void setClickFontColor(const sf::Color& color);          // Set font color for the button when it is clicked
    void setBkgColor(const sf::Color& color);                // Set background color for the button
    void setHoverBkgColor(const sf::Color& color);           // Set background color for the button when it is hovered over
    void setClickBkgColor(const sf::Color& color);           // Set background color for the button when it is clicked
    void setCharacterSize(unsigned int size);                // Set font size for the button
    void setOutlineColor(const sf::Color& color);            // Set outline color of the button
    void setOutlineThickness(unsigned int thickness);        // Set outline thickness of the button
    void setCallback(const std::function<void(void)>& cbk);  // Set callback function

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;  // Virtual function for drawing, derived from sf::Drawable

    virtual void onMousePress();
    virtual void onMouseRelease();
    virtual void onHoverEnter();
    virtual void onHoverExit();
    virtual bool isHoveringOver(sf::Vector2f);
};

Button::Button() = default;

Button::Button(const char* text) {
    setString(text);
}

// Button::Button(const Button& other) {
//     setString(other.label.getString());
// }

// Button& Button::operator=(const Button& other) {
//     setString(other.label.getString());

//     return *this;
// }

// Button::Button(Button&& other) {
//     using std::swap;

//     swap(label, other.label);
//     swap(bkgRect, other.bkgRect);
// }

// Button& Button::operator=(Button&& other) {
//     using std::swap;

//     swap(label, other.label);
//     swap(bkgRect, other.bkgRect);

//     return *this;
// }

Button::~Button() {
}

void Button::setString(const sf::String& text) {
    label.setString(text);
    moveLabelOriginToCenter();
}

void Button::setString(const char* text) {
    label.setString(text);
    moveLabelOriginToCenter();
}

void Button::setFont(const sf::Font& font) {
    label.setFont(font);
}

void Button::setSize(const sf::Vector2f& size) {
    bkgRect.setSize(size);
    auto pos = bkgRect.getPosition();
    label.setPosition(pos + size / 2.0f);
}

void Button::setPosition(float x, float y) {
    bkgRect.setPosition(x, y);
    auto size = bkgRect.getSize();
    label.setPosition(x + size.x / 2.0f, y + size.y / 2.0f);
}

void Button::setPosition(const sf::Vector2f& pos) {
    bkgRect.setPosition(pos);
    label.setPosition(pos + bkgRect.getSize() / 2.0f);
}

void Button::setFontColor(const sf::Color& color) {
    defaultTextColor = color;

    if (!isHovered && !isPressed) {
        label.setFillColor(defaultTextColor);
    }
}

void Button::setBkgColor(const sf::Color& color) {
    defaultBkgColor = color;

    if (!isHovered && !isPressed) {
        bkgRect.setFillColor(defaultBkgColor);
    }
}

void Button::setHoverBkgColor(const sf::Color& color) {
    hoverBkgColor = color;

    if (isHovered) {
        bkgRect.setFillColor(hoverBkgColor);
    }
}

void Button::setClickBkgColor(const sf::Color& color) {
    clickBkgColor = color;

    if (isPressed) {
        bkgRect.setFillColor(color);
    }
}

void Button::setClickFontColor(const sf::Color& color) {
    clickTextColor = color;

    if (isPressed) {
        label.setFillColor(color);
    }
}

void Button::setCharacterSize(unsigned int size) {
    label.setCharacterSize(size);
}

void Button::setOutlineColor(const sf::Color& color) {
    bkgRect.setOutlineColor(color);
}

void Button::setOutlineThickness(unsigned int thickness) {
    bkgRect.setOutlineThickness(thickness);
}

void Button::moveLabelOriginToCenter() {
    sf::FloatRect labelRect = label.getLocalBounds();
    label.setOrigin(labelRect.left + labelRect.width / 2.0f, labelRect.top + labelRect.height / 2.0f);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(bkgRect);
    target.draw(label);
}

void Button::onMousePress() {
    bkgRect.setFillColor(clickBkgColor);
    label.setFillColor(clickTextColor);
}

void Button::onMouseRelease() {
    onHoverEnter();

    if (callback) {
        callback();
    }
}

void Button::onHoverEnter() {
    label.setFillColor(defaultTextColor);
    bkgRect.setFillColor(hoverBkgColor);
}

void Button::onHoverExit() {
    label.setFillColor(defaultTextColor);
    bkgRect.setFillColor(defaultBkgColor);
}

bool Button::isHoveringOver(sf::Vector2f pos) {
    return bkgRect.getGlobalBounds().contains(pos);
}

void Button::setCallback(const std::function<void(void)>& cbk) {
    callback = cbk;
}