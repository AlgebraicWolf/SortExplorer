#include <SFML/Graphics.hpp>
#include <cstring>

class Button : public sf::Drawable {
   private:
    sf::RectangleShape bkgRect;
    sf::Text label;

   public:
    Button();                  // Default constructor
    Button(const char* text);  // Text-based constructor

    Button(const Button& other);             // Copy constructor
    Button& operator=(const Button& other);  // Copy assignment

    Button(Button&& other);             // Move cosntructor
    Button& operator=(Button&& other);  // Move assignment

    ~Button();  // Destructor

    void setString(const char* text);        // Function for replacing string
    void setString(const sf::String& text);  // Overload for sf::String, purely for the sake of copy convenience

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;  // Virtual function for drawing, derived from sf::Drawable
};

Button::Button() = default;

Button::Button(const char* text) {
    setString(text);
}

Button::Button(const Button& other) {
    setString(other.label.getString());
}

Button& Button::operator=(const Button& other) {
    setString(other.label.getString());

    return *this;
}

Button::Button(Button&& other) {
    using std::swap;

    swap(label, other.label);
    swap(bkgRect, other.bkgRect);
}

Button& Button::operator=(Button&& other) {
    using std::swap;

    swap(label, other.label);
    swap(bkgRect, other.bkgRect);

    return *this;
}

Button::~Button() = default;

void Button::setString(const sf::String& text) {
    label.setString(text);
}

void Button::setString(const char* text) {
    label.setString(text);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
}