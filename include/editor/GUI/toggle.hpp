#pragma once
#include "button.hpp"
#include "rounded_rectangle.hpp"
#include "../transition.hpp"
#include "container.hpp"


namespace GUI
{

struct Toggle : public Button
{
    bool state = false;
    trn::Transition<sf::Vector2f> toggle_position;
    trn::Transition<sf::Vector3f> toggle_color;

    float padding = 3.0f;

    Toggle()
        : Button("", nullptr)
    {
        click_callback = [this](){
            setState(!state);
        };

        setWidth(40.0f);
        setHeight(24.0f);

        toggle_position.setSpeed(5.0f);
        toggle_color.setSpeed(5.0f);

        updateColor(true);
    }

    void setState(bool s)
    {
        if (s == state) {
            return;
        }

        state = s;
        updateTogglePosition();
        updateColor();
        this->notifyChanged();
    }

    void onPositionChange() override
    {
        updateTogglePosition(true);
    }

    void updateTogglePosition(bool instant = false)
    {
        const float radius  = 0.5f * size.y - padding;
        const sf::Vector2f target_position = position + (state ? sf::Vector2f{size.x - radius - padding, size.y * 0.5f} : sf::Vector2f{radius + padding, size.y * 0.5f});
        if (instant) {
            toggle_position.setValueInstant(target_position);
        } else {
            toggle_position = target_position;
        }
    }

    void updateColor(bool instant = false)
    {
        const sf::Vector3f color_on{180.0f, 240.0f, 180.0f};
        const sf::Vector3f color_off{180.0f, 180.0f, 180.0f};
        if (instant) {
            toggle_color.setValueInstant(state ? color_on : color_off);
        } else {
            toggle_color = state ? color_on : color_off;
        }
    }

    void render(sf::RenderTarget& target) override
    {
        RoundedRectangle back(size, position, size.y);
        back.setFillColor(sf::Color(200, 200, 200));
        GUI::Item::draw(target, back);

        const float radius  = 0.5f * size.y - padding;
        sf::CircleShape toggle(radius);
        toggle.setOrigin(radius, radius);
        toggle.setPosition(toggle_position);
        toggle.setFillColor(vec3ToColor(toggle_color.as()));
        GUI::Item::draw(target, toggle);
    }
};

struct NamedToggle : public Container
{
    SPtr<Toggle> toggle;

    explicit
    NamedToggle(const std::string& name)
        : Container(Container::Orientation::Vertical)
    {
        padding = 0.0f;
        size_type.y = Size::FitContent;
        Container::addItem(create<TextLabel>(name, 12));
        toggle = create<Toggle>();
        Container::addItem(toggle);

        watch(toggle, [this](){
            notifyChanged();
        });
    }

    [[nodiscard]]
    bool getState() const
    {
        return toggle->state;
    }

    void setState(bool state) const
    {
        toggle->setState(state);
    }
};

}
