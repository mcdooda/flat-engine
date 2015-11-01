#ifndef FLAT_SHARP_UI_WIDGET_H
#define FLAT_SHARP_UI_WIDGET_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../geometry/vector2.h"
#include "../../geometry/matrix4.h"
#include "../../video/attribute.h"
#include "../../misc/slot.h"

namespace flat
{
namespace video
{
class FileTexture;
}

namespace util
{
class RenderSettings;
}

namespace sharp
{
namespace ui
{

class Layout;
class RootWidget;

class Widget
{
	friend class Layout;
	friend class RootWidget;

	public:
		enum SizePolicy : unsigned char
		{
			COMPRESS_X = (1 << 0),
			EXPAND_X   = (1 << 1),
			FIXED_X    = (1 << 2),
			
			COMPRESS_Y = (1 << 3),
			EXPAND_Y   = (1 << 4),
			FIXED_Y    = (1 << 5),
			
			COMPRESS = COMPRESS_X | COMPRESS_Y,
			EXPAND   = EXPAND_X | EXPAND_Y,
			FIXED    = FIXED_X | FIXED_Y
		};
		
		typedef geometry::Vector2 Size;
		
		enum PositionPolicy : unsigned char
		{
		LEFT     = (1 << 0),
			CENTER_X = (1 << 1),
			RIGHT    = (1 << 2),
			FLOW_X   = (1 << 3),
			
			TOP      = (1 << 4),
			CENTER_Y = (1 << 5),
			BOTTOM   = (1 << 6),
			FLOW_Y   = (1 << 7),
			
			TOP_LEFT    = LEFT | TOP,
			CENTER      = CENTER_X | CENTER_Y,
			COLUMN_FLOW = CENTER_X | FLOW_Y,
			LINE_FLOW   = FLOW_X | CENTER_Y
		};
		
		typedef geometry::Vector2 Position;
		
		struct Margin
		{
			Margin(float margin) :
				top(margin),
				right(margin),
				bottom(margin),
				left(margin)
			{
				
			}
			
			float top;
			float right;
			float bottom;
			float left;
		};
		
		typedef Margin Padding;

		typedef geometry::Vector3 Rotation;

		enum class BackgroundRepeat
		{
			SCALED,
			REPEAT
		};
		
	public:
		Widget();
		virtual ~Widget();
		
		void setMargin(const Margin& margin) { m_margin = margin; }
		
		void setSizePolicy(SizePolicy sizePolicy);
		void setSizePolicyX(SizePolicy sizePolicyX);
		void setSizePolicyY(SizePolicy sizePolicyY);
		inline void setSize(const Size& size) { m_size = size; }
		inline const Size& getSize() const { return m_size; }
		
		void setPositionPolicy(PositionPolicy positionPolicy);
		inline void setPosition(const Position& position) { m_position = position; }
		inline const Position& getPosition() const { return m_position; }

		inline void setRotation(const Rotation& rotation) { m_rotation = rotation; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; }
		inline const Rotation& getRotation() const { return m_rotation; }
		
		inline void setBackground(std::shared_ptr<const video::FileTexture> background) { m_background = background; }
		inline void setBackgroundRepeat(BackgroundRepeat backgroundRepeat) { m_backgroundRepeat = backgroundRepeat; }
		inline const video::FileTexture* getBackground() const { return m_background.get(); }
		
		void addChild(Widget* widget);

		Widget* getParent() { return m_parent; }
		const Widget* getParent() const { return m_parent; }

		virtual void preLayout() = 0;
		virtual void layout() = 0;
		virtual void postLayout() = 0;
		virtual void fullLayout() = 0;

		virtual void draw(const util::RenderSettings& renderSettings) const;

		template <class CandidateLayoutType>
		bool hasLayout();

		bool isInside(const geometry::Vector2& point) const;

		Slot<Widget*, bool> click;
		Slot<Widget*> mouseEnter;
		Slot<Widget*> mouseLeave;
		
	protected:
		void drawChildren(const util::RenderSettings& renderSettings) const;
		
		float getInnerWidth() const { return m_size.x - m_padding.left - m_padding.right; }
		void setInnerWidth(float innerWidth) { m_size.x = innerWidth + m_padding.left + m_padding.right; }
		
		float getInnerHeight() const { return m_size.y - m_padding.top - m_padding.bottom; }
		void setInnerHeight(float innerHeight) { m_size.y = innerHeight + m_padding.top + m_padding.bottom; }

		float getOuterWidth() const { return m_size.x + m_margin.left + m_margin.right; }
		float getOuterHeight() const { return m_size.y + m_margin.top + m_margin.bottom; }

		Widget* getMouseOverWidget(const geometry::Vector2& mousePosition);
		
		Margin m_margin;
		Padding m_padding;
		Size m_size;
		Position m_position;
		Rotation m_rotation;

		std::shared_ptr<const video::FileTexture> m_background;
		BackgroundRepeat m_backgroundRepeat;
		
		SizePolicy m_sizePolicy;
		PositionPolicy m_positionPolicy;

		bool m_mouseOver;

		geometry::Matrix4 m_transform;
		
		Widget* m_parent;
		std::vector<Widget*> m_children;
};

template <class LayoutType>
class WidgetImpl : public Widget
{
	typedef Widget Super;
public:
	WidgetImpl() : Super() {}

	void preLayout() override final
	{
		LayoutType::preLayout(*this);
	}

	void layout() override final
	{
		LayoutType::layout(*this);
	}

	void postLayout() override final
	{
		LayoutType::postLayout(*this);
	}

	void fullLayout() override final
	{
		LayoutType::preLayout(*this);
		LayoutType::layout(*this);
		LayoutType::postLayout(*this);
	}
};

template <class CandidateLayoutType>
bool Widget::hasLayout()
{
	return dynamic_cast<WidgetImpl<CandidateLayoutType>*>(this) != nullptr;
}

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_WIDGET_H



