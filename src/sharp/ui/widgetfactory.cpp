#include "canvaswidget.h"
#include "rootwidget.h"
#include "textinputwidget.h"
#include "textwidget.h"
#include "widgetfactory.h"
#include "../../flat/game.h"

#include "layouts/fixedlayout.h"
#include "layouts/lineflowlayout.h"
#include "layouts/columnflowlayout.h"

namespace flat
{
namespace sharp
{
namespace ui
{

WidgetFactory::WidgetFactory(Flat& flat) :
	m_flat(flat)
{
	
}

std::shared_ptr<RootWidget> WidgetFactory::makeRoot() const
{
	std::shared_ptr<RootWidget> rootWidget = std::make_shared<RootWidget>(m_flat);
	rootWidget->setWeakPtr(rootWidget);
	return rootWidget;
}

std::shared_ptr<Widget> WidgetFactory::makeImage(const std::string& fileName) const
{
	std::shared_ptr<const video::FileTexture> texture = m_flat.video->getTexture(fileName);
	std::shared_ptr<Widget> widget = makeFixedSize(texture->getSize());
	widget->setBackground(texture);
	return widget;
}

std::shared_ptr<Widget> WidgetFactory::makeFixedSize(const Vector2& size) const
{
	std::shared_ptr<Widget> widget = std::make_shared<WidgetImpl<FixedLayout>>();
	widget->setWeakPtr(widget);
	widget->setSizePolicy(Widget::SizePolicy::FIXED);
	widget->setSize(size);
	return widget;
}

std::shared_ptr<Widget> WidgetFactory::makeExpand() const
{
	std::shared_ptr<Widget> widget = std::make_shared<WidgetImpl<FixedLayout>>();
	widget->setWeakPtr(widget);
	widget->setSizePolicy(Widget::SizePolicy::EXPAND);
	return widget;
}

std::shared_ptr<Widget> WidgetFactory::makeLineFlow() const
{
	std::shared_ptr<Widget> widget = std::make_shared<WidgetImpl<LineFlowLayout>>();
	widget->setWeakPtr(widget);
	widget->setSizePolicy(Widget::SizePolicy::COMPRESS);
	return widget;
}

std::shared_ptr<Widget> WidgetFactory::makeColumnFlow() const
{
	std::shared_ptr<Widget> widget = std::make_shared<WidgetImpl<ColumnFlowLayout>>();
	widget->setWeakPtr(widget);
	widget->setSizePolicy(Widget::SizePolicy::COMPRESS);
	return widget;
}

std::shared_ptr<TextWidget> WidgetFactory::makeText(const std::string& text, const std::string& fileName, int fontSize) const
{
	std::shared_ptr<const video::font::Font> font = m_flat.video->getFont(fileName, fontSize);
	std::shared_ptr<TextWidget> widget = std::make_shared<TextWidget>(font);
	widget->setWeakPtr(widget);
	widget->setText(text);
	return widget;
}

std::shared_ptr<TextInputWidget> WidgetFactory::makeTextInput(const std::string& fileName, int fontSize) const
{
	std::shared_ptr<const video::font::Font> font = m_flat.video->getFont(fileName, fontSize);
	std::shared_ptr<TextInputWidget> widget = std::make_shared<TextInputWidget>(m_flat, font);
	widget->setWeakPtr(widget);
	return widget;
}

std::shared_ptr<CanvasWidget> WidgetFactory::makeCanvas(const Vector2& size) const
{
	std::shared_ptr<CanvasWidget> widget = std::make_shared<CanvasWidget>(getCanvasRender(), size);
	widget->setWeakPtr(widget);
	return widget;
}

std::shared_ptr<const render::ProgramSettings> WidgetFactory::getCanvasRender() const
{
	if (m_canvasRender != nullptr)
	{
		return m_canvasRender;
	}

	render::ProgramSettings* canvasRender = new render::ProgramSettings();
	canvasRender->program.load("data/shaders/ui/canvas.frag", "data/shaders/ui/canvas.vert");

	canvasRender->settings.colorUniform = canvasRender->program.getUniform<flat::video::Color>("color");
	canvasRender->settings.viewProjectionMatrixUniform = canvasRender->program.getUniform<flat::Matrix4>("viewMatrix");

	canvasRender->settings.positionAttribute = canvasRender->program.getAttribute("position");

	m_canvasRender.reset(canvasRender);
	return m_canvasRender;
}

} // ui
} // sharp
} // flat
