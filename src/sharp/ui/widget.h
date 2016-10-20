#ifndef FLAT_SHARP_UI_WIDGET_H
#define FLAT_SHARP_UI_WIDGET_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "../../misc/vector.h"
#include "../../misc/matrix4.h"
#include "../../video/attribute.h"
#include "../../video/color.h"
#include "../../misc/slot.h"
#include "../../util/convertible.h"

namespace flat
{
namespace video
{
class Texture;
}

namespace render
{
struct RenderSettings;
}

namespace sharp
{
namespace ui
{

class Layout;
class RootWidget;

class Widget : public util::Convertible<Widget>
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
		
		typedef Vector2 Size;
		
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
			
			TOP_LEFT     = LEFT     | TOP,
			TOP_RIGHT    = RIGHT    | TOP,
			BOTTOM_LEFT  = LEFT     | BOTTOM,
			BOTTOM_RIGHT = RIGHT    | BOTTOM,
			CENTER       = CENTER_X | CENTER_Y,
			COLUMN_FLOW  = CENTER_X | FLOW_Y,
			LINE_FLOW    = FLOW_X   | CENTER_Y
		};
		
		typedef Vector2 Position;
		
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

		typedef Vector3 Rotation;

		enum BackgroundRepeat : unsigned char
		{
			SCALED,
			REPEAT
		};
		
	public:
		Widget();
		virtual ~Widget();

		inline void setWeakPtr(const std::weak_ptr<Widget>& self) { m_self = self; }
		inline const std::weak_ptr<Widget>& getWeakPtr() { return m_self; }
		inline std::shared_ptr<Widget> getSharedPtr() const { return m_self.lock(); }
		
		inline void setMargin(const Margin& margin) { m_margin = margin; }
		inline const Margin& getMargin() const { return m_margin; }

		inline void setPadding(const Padding& padding) { m_padding = padding; }
		inline const Padding& getPadding() const { return m_padding; }
		
		void setSizePolicy(SizePolicy sizePolicy);
		void setSizePolicyX(SizePolicy sizePolicyX);
		void setSizePolicyY(SizePolicy sizePolicyY);
		SizePolicy getSizePolicy() const { return m_sizePolicy; }
		inline void setSize(const Size& size) { m_size = size; }
		inline const Size& getSize() const { return m_size; }
		
		void setPositionPolicy(PositionPolicy positionPolicy);
		inline PositionPolicy getPositionPolicy() const { return m_positionPolicy; }
		void setPosition(const Position& position);
		inline const Position& getPosition() const { return m_position; }

		inline void setRotation(const Rotation& rotation) { m_rotation = rotation; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; }
		inline const Rotation& getRotation() const { return m_rotation; }
		
		inline void setBackground(const std::shared_ptr<const video::Texture>& background) { m_background = background; setBackgroundColor(flat::video::Color::WHITE); }
		inline void setBackgroundRepeat(BackgroundRepeat backgroundRepeat) { m_backgroundRepeat = backgroundRepeat; }
		inline const video::Texture* getBackground() const { return m_background.get(); }

		inline void setBackgroundColor(const flat::video::Color& backgroundColor) { m_backgroundColor = backgroundColor; }
		inline const flat::video::Color& getBackgroundColor() const { return m_backgroundColor; }
		
		inline void setVisible(bool visible) { m_visible = visible; }
		inline bool isVisible() const { return m_visible; }
		inline void hide() { setVisible(false); }
		inline void show() { setVisible(true); }
		
		void addChild(const std::shared_ptr<Widget>& widget);
		void removeChild(const std::shared_ptr<Widget>& widget);
		void removeFromParent();

		inline std::weak_ptr<Widget> getParent() { return m_parent; }

		virtual void preLayout() = 0;
		virtual void layout() = 0;
		virtual void postLayout() = 0;
		virtual void fullLayout() = 0;

		virtual void draw(const render::RenderSettings& renderSettings) const;

		template <class CandidateLayoutType>
		bool hasLayout() const;

		bool isInside(const Vector2& point) const;
		Vector2 getRelativePosition(const Vector2& absolutePosition) const;

		Slot<Widget*, bool> click;
		Slot<Widget*, bool> mouseMove;
		Slot<Widget*> mouseEnter;
		Slot<Widget*> mouseLeave;
		
	protected:
		void drawChildren(const render::RenderSettings& renderSettings) const;
		
		float getInnerWidth() const { return m_computedSize.x - m_padding.left - m_padding.right; }
		void setInnerWidth(float innerWidth) { m_computedSize.x = innerWidth + m_padding.left + m_padding.right; }
		
		float getInnerHeight() const { return m_computedSize.y - m_padding.top - m_padding.bottom; }
		void setInnerHeight(float innerHeight) { m_computedSize.y = innerHeight + m_padding.top + m_padding.bottom; }

		float getOuterWidth() const { return m_computedSize.x + m_margin.left + m_margin.right; }
		float getOuterHeight() const { return m_computedSize.y + m_margin.top + m_margin.bottom; }

		Widget* getMouseOverWidget(const Vector2& mousePosition);

		virtual void setDirty();

		virtual bool isRoot() const { return false; }
		RootWidget* getRootIfAncestor();
		Widget* getFixedLayoutAncestor();
		
		Margin m_margin;
		Padding m_padding;
		Size m_size;
		Position m_position;
		Rotation m_rotation;

		std::shared_ptr<const video::Texture> m_background;
		BackgroundRepeat m_backgroundRepeat;
		flat::video::Color m_backgroundColor;
		
		SizePolicy m_sizePolicy;
		PositionPolicy m_positionPolicy;

		bool m_mouseOver : 1;
		bool m_visible : 1;

		Vector2 m_computedSize;
		Matrix4 m_transform;
		
		std::weak_ptr<Widget> m_self;
		std::weak_ptr<Widget> m_parent;
		std::vector<std::shared_ptr<Widget>> m_children;
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
bool Widget::hasLayout() const
{
	return dynamic_cast<const WidgetImpl<CandidateLayoutType>*>(this) != nullptr;
}

} // ui
} // sharp
} // flat

#endif // FLAT_SHARP_UI_WIDGET_H



