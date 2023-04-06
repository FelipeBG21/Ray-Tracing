#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::vector<double> stringToVector(std::string str) {
    std::vector<double> vec;
    std::stringstream ss(str);

    while (ss.good()) {
        std::string substr;
        std::getline(ss, substr, ',');
        double d = std::stod(substr);
        vec.push_back(d);
    }

    return vec;
}

class Screen {
    
public:
    sf::RenderWindow window_;
    Screen(int width, int height, std::string title){
        window_.create(sf::VideoMode(width, height), title);
    };

};

//BUTTON --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

class Button {

public:
    // Default constructor
    sf::Font font;
    sf::RectangleShape rectangle; //Create vector of rectangles
    sf::Text text;
    bool rectangleClicked=false;
    sf::Color colorText;

    Button(sf::Vector2f position, sf::Vector2f size, std::string textString, sf::Color color, sf::Color colorText) : colorText(colorText) {

        rectangle.setPosition(position);
        rectangle.setSize(size);
        rectangle.setFillColor(color);

        //Create text
        font.loadFromFile("Fonts/Metropolian-Display.ttf");
        text.setFont(font);
        text.setString(textString);
        text.setCharacterSize(30);
        text.setFillColor(colorText);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
        text.setPosition(sf::Vector2f(position.x + size.x/5, position.y + 5)); // Set the position of the text inside the rectangle
    };

    bool click();
    void handleEvents(sf::Event event, const sf::Vector2f& mousePos); //the operator & is used to pass the address of the object but i can't modify it
    void render(sf::RenderWindow& window);
    void setRectClic(bool rectangleClicked_);

};

void Button::setRectClic(bool rectangleClicked_){
    rectangleClicked = rectangleClicked_;
}

void Button::handleEvents(sf::Event event, const sf::Vector2f& mousePos) 
{   
    if (rectangleClicked==true)
    {
        rectangleClicked = true;
        text.setFillColor(sf::Color::Blue);
    }
    else
    {
        text.setFillColor(colorText);
    }
    //Clicking on rectangle
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (rectangle.getGlobalBounds().contains(mousePos)) //Check if mouse position is inside rectangle 
        {
            rectangleClicked = true; 
        }
        else
        {
            text.setFillColor(colorText);
        }

    }

        
};

bool Button::click(){
    return rectangleClicked;
}

void Button::render(sf::RenderWindow& window)
{
    window.draw(rectangle);
    window.draw(text);
};


class DropDownList {
public:

    // Default constructor
    DropDownList(std::vector<std::string> items, float width, float height, sf::Vector2f position){
        m_background.setSize(sf::Vector2f(width, height));
        m_background.setFillColor(sf::Color::White);
        m_background.setOutlineColor(sf::Color::Black);
        m_background.setOutlineThickness(2.f);
        m_background.setPosition(position);

        font.loadFromFile("Fonts/Metropolian-Display.ttf");
        m_text.setFont(font);
        m_text.setCharacterSize(20);
        m_text.setPosition(position.x + 5, position.y + 5);
        m_text.setFillColor(sf::Color::Black);
        m_text.setString("Select an option");

        for (int i = 0; i < items.size(); ++i) {
            sf::Text itemText(items[i], font, 20);
            itemText.setPosition(position.x + 5, position.y + height + (i + 1) * height);
            itemText.setFillColor(sf::Color::Red);
            m_itemsTexts.push_back(itemText);
        }
    };

    std::vector<std::string> m_items;
    sf::Vector2f m_position;
    sf::Font font;
    bool m_isOpen=false;
    int m_selectedIndex=-1;
    sf::RectangleShape m_background;
    sf::Text m_text;
    std::vector<sf::Text> m_itemsTexts;

    void handleEvents(sf::Event event, const sf::Vector2f mousePos);
    void render(sf::RenderWindow& window);

};

class Image {

public:

    Image(const std::string& filename, float x, float y) {
        
        texture.loadFromFile(filename);
        sprite.setTexture(texture);
        sprite.setPosition(x, y);
    }

    sf::Texture texture;
    sf::Sprite sprite;

    void render(sf::RenderWindow& window);
};

// PROGRESS BAR --------------------------------------------------------------------------------------------------66

class ProgressBar
{
public:
    ProgressBar(sf::Vector2f position, sf::Vector2f size, float minValue, float maxValue, double currentValue);

    void handleEvent(sf::Event event);
    void render(sf::RenderWindow& window);

    double getValue() const;

private:
    sf::RectangleShape m_bar;
    sf::RectangleShape m_fill;
    float m_minValue;
    float m_maxValue;
    double m_currentValue;
    bool m_isDragging;
};

ProgressBar::ProgressBar(sf::Vector2f position, sf::Vector2f size, float minValue, float maxValue, double currentValue)
{
    m_bar.setPosition(position);
    m_bar.setSize(size);
    m_bar.setFillColor(sf::Color::Transparent);
    m_bar.setOutlineThickness(2.0f);
    m_bar.setOutlineColor(sf::Color::White);

    m_fill.setPosition(position);
    m_fill.setSize(sf::Vector2f(0.0f, size.y));
    m_fill.setFillColor(sf::Color::Green);

    m_minValue = minValue;
    m_maxValue = maxValue;
    m_currentValue = currentValue;

    m_isDragging = false;
}

void ProgressBar::handleEvent(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (m_bar.getGlobalBounds().contains(mousePos))
        {
            m_isDragging = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        m_isDragging = false;
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        if (m_isDragging)
        {
            sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
            float posX = std::min(std::max(mousePos.x, m_bar.getPosition().x), m_bar.getPosition().x + m_bar.getSize().x);
            float percentage = (posX - m_bar.getPosition().x) / m_bar.getSize().x;
            m_currentValue = m_minValue + percentage * (m_maxValue - m_minValue);
            m_fill.setSize(sf::Vector2f(posX - m_bar.getPosition().x, m_bar.getSize().y));
        }
    }
}

void ProgressBar::render(sf::RenderWindow& window)
{
    window.draw(m_bar);
    window.draw(m_fill);
}

double ProgressBar::getValue() const
{
    return m_currentValue;
}

//DROP DOWN LIST MATERIAL ----------------------------------------------------------------------------------------------------------------------------

class DropDownListMat {
public:

    // Default constructor
    DropDownListMat(std::vector<std::string> items, float width, float height, sf::Vector2f position){
        m_background.setSize(sf::Vector2f(width, height));
        m_background.setFillColor(sf::Color::White);
        m_background.setOutlineColor(sf::Color::Black);
        m_background.setOutlineThickness(2.f);
        m_background.setPosition(position);

        

        font.loadFromFile("Fonts/Metropolian-Display.ttf");
        m_text.setFont(font);
        m_text.setCharacterSize(20);
        m_text.setPosition(position.x + 5, position.y + 5);
        m_text.setFillColor(sf::Color::Black);
        m_text.setString("Select an option");

        for (int i = 0; i < items.size(); ++i) {
            sf::Text itemText(items[i], font, 20);
            itemText.setPosition(position.x + (i + 2.2)*140 , 130);
            itemText.setFillColor(sf::Color::Red);
            m_itemsTexts.push_back(itemText);
        }
    };

    std::vector<std::string> m_items;
    sf::Vector2f m_position;
    sf::Font font;
    bool m_isOpen=false;
    int m_selectedIndex=-1;
    sf::RectangleShape m_background;
    sf::Text m_text;
    std::vector<sf::Text> m_itemsTexts;

    void handleEvents(sf::Event event, const sf::Vector2f mousePos, float progress);
    void render(sf::RenderWindow& window);
    bool getOpen();
    int getSelectedIndex();
    void setSelectIndex(int m_selectedIndex_);

};

void DropDownListMat::handleEvents(sf::Event event, const sf::Vector2f mousePos, float progres)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (m_background.getGlobalBounds().contains(mousePos)) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                m_isOpen = true;
            }
        }

        if (m_isOpen) {
            for (int i = 0; i < m_itemsTexts.size(); ++i) {
                if (m_itemsTexts[i].getGlobalBounds().contains(mousePos)) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        m_selectedIndex = i;
                            if (m_selectedIndex==0){
                                if (progres > 0.0)
                                {
                                    std::cout << "Selected index: " << m_selectedIndex << std::endl;
                                    m_text.setString(m_itemsTexts[m_selectedIndex].getString());
                                    //m_text.setString(m_selectedIndex);
                                    m_isOpen = false;
                                }
                                else
                                {
                                    m_selectedIndex = 0;
                                }
                            }
                            else
                            {
                                std::cout << "Selected index: " << m_selectedIndex << std::endl;
                                m_text.setString(m_itemsTexts[m_selectedIndex].getString());
                                //m_text.setString(m_selectedIndex);
                                m_isOpen = false;
                            }                  
                                              
                    }
                }
            }
        }
    }

};

int DropDownListMat::getSelectedIndex()
{
    return m_selectedIndex;
};

bool DropDownListMat::getOpen()
{
    return m_isOpen;
};

void DropDownListMat::render(sf::RenderWindow& m_window)
{
    m_window.draw(m_background);
    m_window.draw(m_text);
    if (m_isOpen) {
        for (const auto& itemText : m_itemsTexts) {
            m_window.draw(itemText);
        }
    }
};

//INCREASE / DECREASE --------------------------------------------------------------------------------------------------
class CounterButton {

public:
    // Default constructor
    sf::Font font;
    sf::RectangleShape rectangle;
    sf::Text text;
    float count=1.0; // contador inicializado en 0

    CounterButton(sf::Vector2f position, sf::Vector2f size, std::string textString, sf::Color color, sf::Color colorText) {

        rectangle.setPosition(position);
        rectangle.setSize(size);
        rectangle.setFillColor(color);

        //Crear texto
        font.loadFromFile("Fonts/Metropolian-Display.ttf");
        text.setFont(font);
        text.setString(textString + std::to_string(count)); // Añade el valor actual del contador al texto
        text.setCharacterSize(30);
        text.setFillColor(colorText);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(1);
        text.setPosition(sf::Vector2f(position.x + size.x/5, position.y + 5)); //Establece la posición del texto dentro del rectángulo
    };

    void handleEvent(sf::Event event, const sf::Vector2f& mousePos);
    void render(sf::RenderWindow& window);

};

void CounterButton::handleEvent(sf::Event event, const sf::Vector2f& mousePos)
{
    //Click en el rectángulo
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (rectangle.getGlobalBounds().contains(mousePos) && event.mouseButton.button == sf::Mouse::Left) //Comprueba si la posición del ratón está dentro del rectángulo
        {
            count=count+0.1; //Aumenta el contador si se hace clic en el botón
            text.setString("Count: " + std::to_string(count)); // Actualiza el texto del botón con el valor actual del contador
        }
        else if (rectangle.getGlobalBounds().contains(mousePos) && event.mouseButton.button == sf::Mouse::Right) //Comprueba si se hace clic derecho en el botón
        {
            count=count-0.1; //Disminuye el contador si se hace clic derecho en el botón
            text.setString("Count: " + std::to_string(count)); // Actualiza el texto del botón con el valor actual del contador
        }

    }
};

void CounterButton::render(sf::RenderWindow& window)
{
    window.draw(rectangle);
    window.draw(text);
};

// COLOR PAQ --------------------------------------------------------------------------------------------------
class ColorPaq {
public:
    ColorPaq(int num_columns, int num_rows, int rectangle_size) :
        NUM_COLUMNS(num_columns), NUM_ROWS(num_rows), RECTANGLE_SIZE(rectangle_size)
    {
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLUMNS; j++) {
                sf::RectangleShape rect(sf::Vector2f(RECTANGLE_SIZE, RECTANGLE_SIZE));
                rect.setPosition(270 + j * RECTANGLE_SIZE, 300 + i * RECTANGLE_SIZE);
                rect.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256)); // Set RGB color
                rectangles_.push_back(rect);
            }
        }
    }

    const int NUM_COLUMNS;
    const int NUM_ROWS;
    const int RECTANGLE_SIZE;
    std::vector<sf::RectangleShape> rectangles_;
    sf::Color colors_selected = sf::Color::Transparent;
    bool col_sel = false;

    std::vector<int> getRGB();
    bool getColSel();
    std::string getRGBString(sf::Color color);
    void handleEvent(sf::Event event,sf::Vector2i mousePos);
    void render(sf::RenderWindow& window);
};

std::string ColorPaq::getRGBString(sf::Color color) {
        return "(" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ")";
    }

bool ColorPaq::getColSel(){
    return col_sel;
}

std::vector<int> ColorPaq::getRGB() {
    std::vector<int> rgb;
    rgb.push_back(colors_selected.r);
    rgb.push_back(colors_selected.g);
    rgb.push_back(colors_selected.b);
    return rgb;
}

void ColorPaq::handleEvent(sf::Event event,sf::Vector2i mousePos){
    for (int i = 0; i < rectangles_.size(); i++) {
        if (event.type == sf::Event::MouseButtonPressed){
            if (rectangles_[i].getGlobalBounds().contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                sf::Color color = rectangles_[i].getFillColor();
                colors_selected=color;
                col_sel = true;
                break;
            }
        }
    }
}

void ColorPaq::render(sf::RenderWindow& window) {
    for (const auto& rect : rectangles_) {
        window.draw(rect);
    }
}

//INPUT TEXT --------------------------------------------------------------------------------------------------

class InputText {
public:
    InputText(int size, sf::Color color, sf::Vector2f position) {
        m_rect.setSize(sf::Vector2f(150, 30));
        m_rect.setFillColor(sf::Color::White);
        m_rect.setPosition(position);

        font.loadFromFile("Fonts/Metropolian-Display.ttf");
        m_text.setFont(font);
        m_text.setCharacterSize(size);
        m_text.setFillColor(color);
        m_text.setPosition(position);
    }

    std::string getInputString();
    std::vector<double> getInputStringToInt();
    void handleEvent(sf::Event event);
    void render(sf::RenderWindow& window);

    std::string sel_string = " ";
    std::string m_inputString;
    sf::Text m_text;
    sf::RectangleShape m_rect;
    sf::Font font;
};

std::string InputText::getInputString() {
    return sel_string;
;
}

std::vector<double> InputText::getInputStringToInt() {
    std::vector<double> vec = stringToVector(sel_string);
    return vec;
}
void InputText::handleEvent(sf::Event event) {
        if (event.type == sf::Event::TextEntered)
        {
            // Handle backspace
            if (event.text.unicode == 8 && m_inputString.size() > 0)
            {
                m_inputString.pop_back();
                m_text.setString(m_inputString);
            }
            // Handle other characters
            else if (event.text.unicode < 128 && event.text.unicode != 13) // ASCII characters excluding enter key
            {
                m_inputString += static_cast<char>(event.text.unicode);
                m_text.setString(m_inputString);
            }
            // Handle enter key
            else if (event.text.unicode == 13)
            {
                sel_string = m_inputString;
                std::cout << "Input text: " << m_inputString << std::endl; // Display input text in console
            }
        }
    }

void InputText::render(sf::RenderWindow& window) {
        window.draw(m_rect);
        window.draw(m_text);
}

//DROP DOWN LIST ----------------------------------------------------------------------------------------------------------------------------


void DropDownList::handleEvents(sf::Event event, const sf::Vector2f mousePos)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (m_background.getGlobalBounds().contains(mousePos)) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                m_isOpen = true;
            }
        }

        if (m_isOpen) {
            for (int i = 0; i < m_itemsTexts.size(); ++i) {
                if (m_itemsTexts[i].getGlobalBounds().contains(mousePos)) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        m_selectedIndex = i;
                        std::cout << "Selected index: " << m_selectedIndex << std::endl;
                        m_text.setString(m_itemsTexts[m_selectedIndex].getString());
                        //m_text.setString(m_selectedIndex);
                        m_isOpen = false;
                    }
                }
            }
        }
    }

};

void DropDownList::render(sf::RenderWindow& m_window)
{
    m_window.draw(m_background);
    m_window.draw(m_text);
    if (m_isOpen) {
        for (const auto& itemText : m_itemsTexts) {
            m_window.draw(itemText);
        }
    }
};

//IMAGE ----------------------------------------------------------------------------------------------------------------------------
void Image::render(sf::RenderWindow& window)
{
    window.draw(sprite);
};