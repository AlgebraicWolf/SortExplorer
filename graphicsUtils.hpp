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
    MouseInteractive() {
        isHovered = false;
        isPressed = false;
    }

    void printOutDesctiptor() {
        printf("I am %p and my descriptor points to %p\n", this, *descriptor);
    }

    virtual ~MouseInteractive() {
        detach();
    }
};

void MouseInteractive::detach() {
    if (containingList) {
        printf("Detaching interactive element %p =?= %p from element list with address %p\n", *descriptor, this, containingList);

        containingList->erase(descriptor);
        containingList = nullptr;
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

    ~MouseInteractiveEH() {
        // Make all elements seem detached
        for (auto it = trackedObjects.begin(); it != trackedObjects.end();) {
            (*(it++))->detach();
        }

        printf("MouseInteractive Event Handler with address %p is dead\n", this);
    }

    bool handleEvent(const sf::Event& event);  // Handle event
    void attach(MouseInteractive& obj);        // Add object for handling
};

bool MouseInteractiveEH::handleEvent(const sf::Event& event) {
    bool updated = false;
    for (MouseInteractive* el : trackedObjects) {
        switch (event.type) {
            case sf::Event::MouseMoved:
                if (el->isHoveringOver(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)) && !el->isHovered) {
                    el->onHoverEnter();
                    el->isHovered = true;
                    updated = true;
                } else if (!el->isHoveringOver(sf::Vector2f(event.mouseMove.x, event.mouseMove.y)) && el->isHovered && !el->isPressed) {
                    el->onHoverExit();
                    el->isHovered = false;
                    updated = true;
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (el->isHoveringOver(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                    el->isPressed = true;
                    el->onMousePress();
                    updated = true;
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (el->isPressed) {
                    if (el->isHoveringOver(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        el->onMouseRelease();
                    } else {
                        el->onHoverExit();
                        el->isHovered = false;
                    }
                    el->isPressed = false;
                    updated = true;
                }
                break;

            default:
                break;
        }
    }

    return updated;
}

void MouseInteractiveEH::attach(MouseInteractive& obj) {
    trackedObjects.push_back(&obj);
    obj.containingList = &trackedObjects;
    obj.descriptor = --trackedObjects.end();

    printf("Attaching interactive element with address %p =?= %p\n", &obj, *obj.descriptor);
    obj.printOutDesctiptor();
}

class Plot : public sf::Drawable {
   private:
    struct limits_t {
        float minX;
        float maxX;
        float minY;
        float maxY;
    };

    limits_t currentLimits;

    std::vector<sf::VertexArray> vertexArrays;

    std::vector<sf::Color> colors;
    std::vector<std::vector<sf::Vector2f>> dataPointsVector;

    sf::RectangleShape bkgRect;
    sf::Text xLabel;
    sf::Text yLabel;

    void recomputeVertexArrays();


   public:
    Plot(); // Default constructor

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;  // Drawing function
    void resetPlot();                                                                     // Clear plot
    void plot(std::vector<sf::Vector2f> dataPoints, const sf::Color& lineColor);          // Add the graph onto the plot

    void setSize(const sf::Vector2f& size);     // Set plot size
    void setPosition(float x, float y);         // Set plot position
    void setPosition(const sf::Vector2f& pos);  // setPosition overload for SFML 2D Vector type for the sake of convenience
    void setXString(const char* text);          // Set label for X axis
    void setXString(const sf::String& text);    // Overload for sf::String, purely for the sake of copy convenience
    void setYString(const char* text);          // Set label for X axis
    void setYString(const sf::String& text);    // Overload for sf::String, purely for the sake of copy convenience
};

void Plot::setSize(const sf::Vector2f& size) {
    bkgRect.setSize(size);
}

void Plot::setPosition(float x, float y) {

}

void Plot::setPosition(const sf::Vector2f& pos) {

}

void Plot::setXString(const char* text) {
    xLabel.setString(text);
}

void Plot::setXString(const sf::String& text) {
    xLabel.setString(text);
}

void Plot::resetPlot() {
    colors.clear();
    dataPointsVector.clear();
}

void Plot::plot(std::vector<sf::Vector2f> dataPoints, const sf::Color& lineColor) {
    dataPointsVector.emplace_back(dataPoints);
    colors.emplace_back(lineColor);
}

void Plot::recomputeVertexArrays() {
    if (dataPointsVector.size()) {
        currentLimits.minX = dataPointsVector[0][0].x;
        currentLimits.maxX = dataPointsVector[0][0].x;
        currentLimits.minY = dataPointsVector[0][0].y;
        currentLimits.maxY = dataPointsVector[0][0].y;

        for (auto& pointSeries : dataPointsVector) {
            for (auto& point : pointSeries) {
                if (point.x < currentLimits.minX) {
                    currentLimits.minX = point.x;
                } else if (point.x > currentLimits.maxX) {
                    currentLimits.maxX = point.x;
                }

                if (point.y < currentLimits.minY) {
                    currentLimits.minY = point.y;
                } else if (point.y > currentLimits.maxY) {
                    currentLimits.maxY = point.y;
                }
            }
        }

        vertexArrays.clear();

        for (auto& pointSeries : dataPointsVector) {
            vertexArrays.emplace_back(sf::Lines, pointSeries.size());
        }
    }
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

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;  // Virtual function for drawing, derived from sf::Drawable

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