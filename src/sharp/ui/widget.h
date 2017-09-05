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
		
		using Size = Vector2;
		
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
		
		using Position = Vector2;
		
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
		
		using Padding = Margin;

		using Rotation = Vector3;

		enum BackgroundRepeat : unsigned char
		{
			SCALED,
			REPEAT
		};

		using BackgroundPosition = Vector2;

		using ScrollPosition = Vector2;

		static constexpr float SCROLL_SPEED = 50000.f;

	protected:
		struct ScissorRectangle
		{
			GLint x;
			GLint y;
			GLsizei width;
			GLsizei height;
		};
		
	public:
		Widget();
		Widget(const Widget&) = delete;
		Widget(Widget&&) = delete;
		virtual ~Widget();
		Widget& operator=(const Widget&) = delete;

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
		void setSize(const Size& size);
		inline const Size& getSize() const { return m_size; }
		
		void setPositionPolicy(PositionPolicy positionPolicy);
		inline PositionPolicy getPositionPolicy() const { return m_positionPolicy; }
		void setPosition(const Position& position);
		inline const Position& getPosition() const { return m_position; }

		void setAbsolutePosition(const Vector2& absolutePosition);
		Vector2 getAbsolutePosition() const;

		inline void setRotation(const Rotation& rotation) { m_rotation = rotation; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; }
		inline const Rotation& getRotation() const { return m_rotation; }
		
		inline void setBackground(const std::shared_ptr<const video::Texture>& background) { m_background = background; setBackgroundColor(flat::video::Color::WHITE); }

		inline void setBackgroundRepeat(BackgroundRepeat backgroundRepeat) { m_backgroundRepeat = backgroundRepeat; }
		inline const video::Texture* getBackground() const { return m_background.get(); }

		inline void setBackgroundPosition(const BackgroundPosition& backgroundPosition) { m_backgroundPosition = backgroundPosition; }
		inline const BackgroundPosition& getBackgroundPosition() const { return m_backgroundPosition; }

		inline void setBackgroundColor(const flat::video::Color& backgroundColor) { m_backgroundColor = backgroundColor; }
		inline const flat::video::Color& getBackgroundColor() const { return m_backgroundColor; }
		
		inline void setVisible(bool visible) { m_visible = visible; }
		inline bool getVisible() const { return m_visible; }
		inline void hide() { setVisible(false); }
		inline void show() { setVisible(true); }

		inline void setAllowScrollX(bool allowScrollX) { m_allowScrollX = allowScrollX; }
		inline bool getAllowScrollX() const { return m_allowScrollX; }
		inline void setAllowScrollY(bool allowScrollY) { m_allowScrollY = allowScrollY; }
		inline bool getAllowScrollY() const { return m_allowScrollY; }

		inline void setRestrictScrollX(bool restrictScrollX) { m_restrictScrollX = restrictScrollX; }
		inline bool getRestrictScrollX() const { return m_restrictScrollX; }
		inline void setRestrictScrollY(bool restrictScrollY) { m_restrictScrollY = restrictScrollY; }
		inline bool getRestrictScrollY() const { return m_restrictScrollY; }

		void scrollX(float scrollValueX, float dt);
		void scrollY(float scrollValueY, float dt);

		inline const ScrollPosition& getScrollPosition() const { return m_scrollPosition; }

		void drag();
		void drop();
		
		void addChild(const std::shared_ptr<Widget>& widget);
		void removeChild(const std::shared_ptr<Widget>& widget);
		void removeFromParent();
		void removeAllChildren();

		inline std::weak_ptr<Widget> getParent() { return m_parent; }

		virtual void preLayout() = 0;
		virtual void layout(bool computePosition) = 0;
		virtual void postLayout() = 0;
		virtual void fullLayout() = 0;
		virtual void layoutDone();

		virtual void draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const;

		template <class CandidateLayoutType>
		bool hasLayout() const;

		bool isInside(const Vector2& point) const;
		Vector2 getRelativePosition(const Vector2& absolutePosition) const;
		Vector2 getRelativePosition(const Widget& other) const;

		virtual void setDirty();
		virtual void clearDirty();

		virtual bool canBeFocused() const;

	public:
		Slot<Widget*, bool&> click;
		Slot<Widget*, bool&> mouseDown;
		Slot<Widget*, bool&> mouseUp;
		Slot<Widget*, bool&> mouseMove;
		Slot<Widget*> mouseEnter;
		Slot<Widget*> mouseLeave;
		Slot<Widget*> layoutFinished;
		
	protected:
		static bool intersect(const ScissorRectangle& a, const ScissorRectangle& b);
		static void computeParentScissorIntersection(ScissorRectangle& scissor, const ScissorRectangle& parentScissor);
		void getScissor(ScissorRectangle& scissor) const;
		bool computeAndApplyScissor(ScissorRectangle& scissor, const ScissorRectangle& parentScissor) const;
		void drawChildren(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const;
		
		float getInnerWidth() const { return m_computedSize.x - m_padding.left - m_padding.right; }
		void setInnerWidth(float innerWidth) { m_computedSize.x = innerWidth + m_padding.left + m_padding.right; }
		
		float getInnerHeight() const { return m_computedSize.y - m_padding.top - m_padding.bottom; }
		void setInnerHeight(float innerHeight) { m_computedSize.y = innerHeight + m_padding.top + m_padding.bottom; }

		float getOuterWidth() const { return m_computedSize.x + m_margin.left + m_margin.right; }
		float getOuterHeight() const { return m_computedSize.y + m_margin.top + m_margin.bottom; }

		Vector2 getMaxScrollPosition() const;

		Widget* getMouseOverWidget(const Vector2& mousePosition);

		virtual bool isRoot() const { return false; }
		RootWidget* getRootIfAncestor();
		Widget* getFixedLayoutAncestor();
		void setAncestorDirty();
		bool hasFixedSize() const;
		
		// Widget settings
		Margin m_margin;
		Padding m_padding;
		Size m_size;
		Position m_position;
		Rotation m_rotation;

		std::shared_ptr<const video::Texture> m_background;
		BackgroundPosition m_backgroundPosition;
		BackgroundRepeat m_backgroundRepeat;
		flat::video::Color m_backgroundColor;
		
		SizePolicy m_sizePolicy;
		PositionPolicy m_positionPolicy;

		bool m_visible : 1;

		// computed
		bool m_mouseOver : 1;
		bool m_hasFocus : 1;
		bool m_allowScrollX : 1;
		bool m_allowScrollY : 1;
		bool m_restrictScrollX : 1;
		bool m_restrictScrollY : 1;

		Matrix4 m_transform;
		Size m_computedSize;
		ScrollPosition m_scrollPosition;
		ScrollPosition m_minScrollPosition;
		
		std::weak_ptr<Widget> m_self;
		std::weak_ptr<Widget> m_parent;
		std::vector<std::shared_ptr<Widget>> m_children;
};

template <class LayoutType>
class WidgetImpl : public virtual Widget
{
	using Super = Widget;
public:
	WidgetImpl() : Super() {}

	void preLayout() override final
	{
		LayoutType::preLayout(*this);
	}

	void layout(bool computePosition) override final
	{
		LayoutType::layout(*this, computePosition);
	}

	void postLayout() override final
	{
		LayoutType::postLayout(*this);
		layoutDone();
	}

	void fullLayout() override final
	{
		LayoutType::preLayout(*this);
		LayoutType::layout(*this, true);
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



