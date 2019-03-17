#include <cctype>
#include <cmath>
#include <limits>
#include <sstream>

#include "sharp/ui/numberinputwidget.h"

#include "flat.h"

namespace flat
{
namespace sharp
{
namespace ui
{

NumberInputWidget::NumberInputWidget(Flat& flat, const std::shared_ptr<const video::font::Font>& font) : 
	TextInputWidget(flat, font),
	m_step(1.f),
	m_min(0.f),
	m_max(std::numeric_limits<float>::max()),
	m_oldValue(m_min),
	m_precision(1)
{
	Widget::setAllowScrollY(true);
	mouseEnter.on(this, &NumberInputWidget::mouseEntered);
	mouseLeave.on(this, &NumberInputWidget::mouseLeft);
}

NumberInputWidget::~NumberInputWidget()
{
	mouseEnter.off(this);
	mouseLeave.off(this);
}

bool NumberInputWidget::mouseEntered(ui::Widget*)
{
	mouseWheelMove.on(this, &NumberInputWidget::wheelJustMoved);
	return true;
}

bool NumberInputWidget::mouseLeft(ui::Widget*)
{
	mouseWheelMove.off(this);
	return true;
}

bool NumberInputWidget::wheelJustMoved(ui::Widget* widget, bool& handled, const Vector2& offset)
{
	handled = true;
	float value = getValue();
	value += offset.y * m_step;
	setValue(value);
	submit(this);
	return true;
}

std::string NumberInputWidget::floatToString(float f)
{
	std::stringstream ss;
	ss << f;
	return ss.str();
}

std::string NumberInputWidget::floatToString(float f, size_t precision)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << f;
	return ss.str();
}

size_t NumberInputWidget::getPrecision(float f)
{
	std::stringstream ss;
	ss << f;
	return getPrecision(ss.str());
}

size_t NumberInputWidget::getPrecision(const std::string& text)
{
	size_t precision = 0;
	try
	{
		size_t i;
		std::stof(text, &i);
		if (i == text.size())
		{
			size_t lastOf = text.find_last_of(".");
			if (lastOf != std::string::npos && text.size() > 0)
			{
				precision = text.size() - 1 - lastOf;
			}
		}
	}
	catch (...)
	{
	}
	return precision;
}

float NumberInputWidget::computeSmallestStep(const std::string& text)
{
	float smallestStep = 1.f;
	if (getPrecision(text) > 0)
	{
		std::string decimals = text.substr(text.find_last_of("."));
		size_t precisionWithoutTrailingZeros = decimals.find_last_not_of('0');
		smallestStep = 1.f / static_cast<float>(std::pow(10.f, precisionWithoutTrailingZeros));
	}
	return smallestStep;
}

void NumberInputWidget::submitFixedValue()
{
	float value;
	if (getText().empty())
	{
		value = m_oldValue;
	}
	else
	{
		value = getValue();
	}
	setValue(value);
	submit(this);
}

bool NumberInputWidget::keyJustPressed(input::Key key)
{
	std::string text = getText();
	if (key == K(RETURN) || key == K(KP_ENTER))
	{
		submitFixedValue();
	}
	else if (key == K(UP))
	{
		stepUp();
	}
	else if (key == K(DOWN))
	{
		stepDown();
	}
	else
	{
		return TextInputWidget::keyJustPressed(key);
	}
	return true;
}

bool NumberInputWidget::textEdited(const std::string& text)
{
	std::string currentText = getText();
	std::string newText = replaceSelectedText(text);
	try
	{
		size_t i = 0;
		float value = std::stof(newText, &i);
		if (i == newText.size() && getPrecision(value) <= m_precision)
		{
			TextInputWidget::textEdited(text);
			float newStep = computeSmallestStep(getText());
			if (newStep < m_step)
			{
				m_step = newStep;
			}
		}
	}
	catch (...)
	{
	}
	return true;
}

float NumberInputWidget::getValue() const
{
	float value = m_min;
	try
	{
		size_t i = 0;
		value = std::stof(getText(), &i);
		if (i != getText().size())
		{
			value = m_min;
		}
	}
	catch(...)
	{
	}
	return value;
}

float NumberInputWidget::constraintValue(float f)
{
	float fixed = f;
	if (fixed < m_min)
	{
		fixed = m_min;
	}
	else if (fixed > m_max)
	{
		fixed = m_max;
	}
	return fixed;
}

void NumberInputWidget::setValue(float value)
{
	if (!getText().empty())
	{
		m_oldValue = getValue();
	}
	value = constraintValue(value);
	std::string text = floatToString(value, m_precision);
	if (getText() != text)
	{
		replaceText(text);
		valueChanged(this);
	}
}

bool NumberInputWidget::leftFocus(Widget* widget)
{
	TextInputWidget::leftFocus(widget);
	submitFixedValue();
	return true;
}

void NumberInputWidget::stepUp()
{
	float value = getValue();
	value += m_step;
	setValue(value);
	submit(this);
}

void NumberInputWidget::stepDown()
{
	float value = getValue();
	value -= m_step;
	setValue(value);
	submit(this);
}
} // ui
} // sharp
} // flat


