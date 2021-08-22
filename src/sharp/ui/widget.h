#ifndef FLAT_SHARP_UI_WIDGET_H
#define FLAT_SHARP_UI_WIDGET_H

#include <memory>
#include <vector>
#include <unordered_map>

#include "sharp/ui/cursor.h"

#include "misc/slot.h"
#include "misc/matrix4.h"
#include "misc/vector.h"
#include "util/convertible.h"
#include "video/attribute.h"
#include "video/color.h"
#include "input/gamepadbuttons.h"

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

		using BackgroundSize = Vector2;

		using ScrollPosition = Vector2;

		static constexpr float SCROLL_SPEED = 80.f;

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
		inline const Size& getComputedSize() const { return m_computedSize; }

		void setPositionPolicy(PositionPolicy positionPolicy);
		inline PositionPolicy getPositionPolicy() const { return m_positionPolicy; }
		void setPosition(const Position& position);
		inline const Position& getPosition() const { return m_position; }

		void setAbsolutePosition(const Vector2& absolutePosition);
		Vector2 getAbsolutePosition() const;

		inline void setRotation(const Rotation& rotation) { m_rotation = rotation; }
		inline void setRotationZ(float rotationZ) { m_rotation.z = rotationZ; }
		inline const Rotation& getRotation() const { return m_rotation; }

		void setBackground(const std::shared_ptr<const video::Texture>& background);

		inline void setBackgroundRepeat(BackgroundRepeat backgroundRepeat) { m_backgroundRepeat = backgroundRepeat; }
		inline const video::Texture* getBackground() const { return m_background.get(); }

		inline void setBackgroundPosition(const BackgroundPosition& backgroundPosition) { m_backgroundPosition = backgroundPosition; }
		inline const BackgroundPosition& getBackgroundPosition() const { return m_backgroundPosition; }

		inline void setBackgroundSize(const BackgroundSize& backgroundSize) { m_backgroundSize = backgroundSize; }
		inline const BackgroundSize& getBackgroundSize(const BackgroundSize& backgroundSize) const { return m_backgroundSize; }

		inline void setBackgroundColor(const flat::video::Color& backgroundColor) { m_backgroundColor = backgroundColor; }
		inline const flat::video::Color& getBackgroundColor() const { return m_backgroundColor; }

		inline void setVisible(bool visible) { m_visible = visible; }
		inline bool getVisible() const { return m_visible; }
		inline void hide() { setVisible(false); }
		inline void show() { setVisible(true); }

		inline void setFocusable(bool focusable) { m_focusable = focusable;  }
		inline bool isFocusable() const { return m_focusable; }
		inline bool hasFocus() const { return m_hasFocus; }

		void setAllowScrollX(bool allowScrollX);
		inline bool getAllowScrollX() const { return m_allowScrollX; }
		void setAllowScrollY(bool allowScrollY);
		inline bool getAllowScrollY() const { return m_allowScrollY; }

		inline void setAllowDragScrolling(bool allowDragScrolling) { m_allowDragScrolling = allowDragScrolling; }
		inline bool getAllowDragScrolling() { return m_allowDragScrolling; }
		CursorType getDragScrollingCursorType() const;

		inline void setRestrictScrollX(bool restrictScrollX) { m_restrictScrollX = restrictScrollX; }
		inline bool getRestrictScrollX() const { return m_restrictScrollX; }
		inline void setRestrictScrollY(bool restrictScrollY) { m_restrictScrollY = restrictScrollY; }
		inline bool getRestrictScrollY() const { return m_restrictScrollY; }

		bool mouseWheelMoved(Widget* widget, bool& handled, const Vector2& offset);
		void scrollX(float scrollValueX);
		void scrollY(float scrollValueY);

		void setScrollX(float scrollX);
		void setScrollY(float scrollY);
		void setScrollPosition(const ScrollPosition& scrollPosition);
		inline const ScrollPosition& getScrollPosition() const { return m_scrollPosition; }

		void drag();
		void drop();

		void addChild(const std::shared_ptr<Widget>& widget);
		void removeChild(const std::shared_ptr<Widget>& widget);
		void removeChildAtIndex(int index);
		void removeFromParent();
		void removeAllChildren();

		inline size_t getChildrenCount() const { return m_children.size(); }
		inline const std::shared_ptr<Widget>& getChildAtIndex(int index) const { return m_children[index]; }
		inline const std::vector<std::shared_ptr<Widget>>& getChildren() const { return m_children; }
		inline const std::weak_ptr<Widget>& getParent() const { return m_parent; }
		inline bool hasParent() const { return !m_parent.expired(); }

		virtual void preLayout() = 0;
		virtual void layout(bool computePosition) = 0;
		virtual void postLayout() = 0;
		virtual void fullLayout() = 0;
		virtual void layoutDone();

		virtual void draw(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const;

		virtual CursorType getCursorType() const;

		template <class CandidateLayoutType>
		bool hasLayout() const;

		bool isInside(const Vector2& point) const;
		Vector2 getRelativePosition(const Vector2& absolutePosition) const;
		Vector2 getRelativePosition(const Widget& other) const;

		virtual void setDirty();
		virtual void clearDirty();

		bool isAncestor(Widget* ancestorWidget) const;
		static Widget* getCommonAncestor(Widget* a, Widget* b);

	public:
		Slot<Widget*, bool&> leftClick;
		Slot<Widget*, bool&> rightClick;
		Slot<Widget*, bool&> mouseDown;
		Slot<Widget*, bool&> mouseUp;
		Slot<Widget*, bool&> mouseMove;
		Slot<Widget*, bool&, const Vector2&> mouseWheelMove;
		Slot<Widget*> scroll;
		Slot<Widget*, Vector2&> beforeDrag;
		Slot<Widget*> dragged;
		Slot<Widget*> mouseEnter;
		Slot<Widget*> mouseLeave;
		Slot<Widget*> enterFocus;
		Slot<Widget*> leaveFocus;

		Slot<Widget*, flat::input::GamepadIndex, flat::input::GamepadButton> gamepadButtonDown;
		Slot<Widget*, flat::input::GamepadIndex, flat::input::GamepadButton> gamepadButtonUp;

		Slot<Widget*> layoutFinished;

		Slot<Widget*, std::string&> copy;
		Slot<Widget*, const std::string&> paste;
		Slot<Widget*> undo;
		Slot<Widget*> redo;


	protected:
		static bool intersect(const ScissorRectangle& a, const ScissorRectangle& b);
		static void computeParentScissorIntersection(ScissorRectangle& scissor, const ScissorRectangle& parentScissor);
		void getScissor(ScissorRectangle& scissor) const;
		bool computeAndApplyScissor(ScissorRectangle& scissor, const ScissorRectangle& parentScissor) const;
		void drawChildren(const render::RenderSettings& renderSettings, const ScissorRectangle& parentScissor) const;
		void drawScrollbars(const render::RenderSettings& renderSettings, const ScissorRectangle& scissor) const;

		float getInnerWidth() const { return m_computedSize.x - m_padding.left - m_padding.right; }
		void setInnerWidth(float innerWidth) { m_computedSize.x = innerWidth + m_padding.left + m_padding.right; }

		float getInnerHeight() const { return m_computedSize.y - m_padding.top - m_padding.bottom; }
		void setInnerHeight(float innerHeight) { m_computedSize.y = innerHeight + m_padding.top + m_padding.bottom; }

		float getOuterWidth() const { return m_computedSize.x + m_margin.left + m_margin.right; }
		float getOuterHeight() const { return m_computedSize.y + m_margin.top + m_margin.bottom; }

		Widget* getMouseOverWidget(const Vector2& mousePosition);

		virtual bool isRoot() const { return false; }
		RootWidget* getRootIfAncestor();
		Widget* getFixedLayoutAncestor();
		void setAncestorDirty();
		bool hasFixedSize() const;

		void resetScrollPosition();

	protected:
		// Widget settings
		Margin m_margin;
		Padding m_padding;
		Size m_size;
		Position m_position;
		Rotation m_rotation;

		std::shared_ptr<const video::Texture> m_background;
		BackgroundPosition m_backgroundPosition;
		BackgroundSize m_backgroundSize;
		BackgroundRepeat m_backgroundRepeat;
		flat::video::Color m_backgroundColor;

		SizePolicy m_sizePolicy;
		PositionPolicy m_positionPolicy;

		bool m_visible : 1;
		bool m_focusable : 1;
		bool m_allowScrollX : 1;
		bool m_allowScrollY : 1;
		bool m_allowDragScrolling : 1;
		bool m_restrictScrollX : 1;
		bool m_restrictScrollY : 1;

		// computed
		bool m_hasFocus : 1;
		bool m_scrolled : 1;
		bool m_dragged : 1;

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
		if (m_scrolled)
		{
			scroll(this);
			m_scrolled = false;
		}
		if (m_dragged)
		{
			dragged(this);
			m_dragged = false;
		}
	}

	void fullLayout() override final
	{
		preLayout();
		layout(true);
		postLayout();
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



