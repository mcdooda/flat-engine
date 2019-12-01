#ifndef FLAT_SHARP_UI_NUMBERINPUTWIDGET_H
#define FLAT_SHARP_UI_NUMBERINPUTWIDGET_H

#include "sharp/ui/textinputwidget.h"

#include "misc/vector.h"

#pragma warning(disable: 4250)

namespace flat
{
namespace sharp
{
namespace ui
{

class NumberInputWidget : public TextInputWidget
{
	using Widget = ui::Widget;
	public:
		NumberInputWidget() = delete;
		NumberInputWidget(const NumberInputWidget&) = delete;
		NumberInputWidget(NumberInputWidget&&) = delete;
		NumberInputWidget(Flat& flat, const std::shared_ptr<const video::font::Font>& font);
		~NumberInputWidget() override;
		NumberInputWidget& operator=(const NumberInputWidget&) = delete;
		float getValue() const;
		void setValue(float value);
		inline void setPrecision(size_t precision) { m_precision = precision; }
		inline void setMin(float min) { m_min = min; }
		inline void setMax(float max) { m_max = max; }

		bool leftFocus(Widget* widget) override;

	private:
		static std::string floatToString(float f);
		static std::string floatToString(float f, size_t precision);
		static size_t getPrecision(float f);
		static size_t getPrecision(const std::string& s);
		static float computeSmallestStep(const std::string& text);
		float constraintValue(float f) const;
		void submitFixedValue();
		bool keyJustPressed(input::Key key) override;
		bool textEdited(const std::string& text) override;
		bool wheelJustMoved(Widget* widget, bool& handled, const Vector2& offset);
		bool mouseEntered(Widget*);
		bool mouseLeft(Widget*);
		void stepUp();
		void stepDown();

	private:
		float m_step;
		float m_min;
		float m_max;
		float m_oldValue;
		size_t m_precision;
};

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_NUMBERINPUTWIDGET_H


