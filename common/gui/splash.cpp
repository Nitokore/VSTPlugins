// (c) 2019-2020 Takamitsu Endo
//
// This file is part of Uhhyou Plugins.
//
// Uhhyou Plugins is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Uhhyou Plugins is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Uhhyou Plugins.  If not, see <https://www.gnu.org/licenses/>.

#include "splash.hpp"

namespace Steinberg {
namespace Vst {

using namespace VSTGUI;

CMouseEventResult CreditView::onMouseDown(CPoint &where, const CButtonState &buttons)
{
  if (buttons.isLeftButton()) {
    setVisible(false);
    return kMouseDownEventHandledButDontNeedMovedOrUpEvents;
  }
  return kMouseEventNotHandled;
}

CMouseEventResult CreditView::onMouseEntered(CPoint &where, const CButtonState &buttons)
{
  isMouseEntered = true;
  invalid();
  return kMouseEventHandled;
}

CMouseEventResult CreditView::onMouseExited(CPoint &where, const CButtonState &buttons)
{
  isMouseEntered = false;
  invalid();
  return kMouseEventHandled;
}

void SplashLabel::draw(CDrawContext *pContext)
{
  pContext->setDrawMode(CDrawMode(CDrawModeFlags::kAntiAliasing));
  CDrawContext::Transform t(
    *pContext, CGraphicsTransform().translate(getViewSize().getTopLeft()));

  const auto width = getWidth();
  const auto height = getHeight();

  const double borderWidth = isMouseEntered ? highlightFrameWidth : frameWidth;
  const double halfBorderWidth = int(borderWidth / 2.0);
  pContext->setFillColor(pal.boxBackground());
  pContext->setFrameColor(isMouseEntered ? pal.highlightButton() : pal.border());
  pContext->setLineWidth(borderWidth);
  pContext->drawRect(
    CRect(
      halfBorderWidth, halfBorderWidth, width - halfBorderWidth,
      height - halfBorderWidth),
    kDrawFilledAndStroked);

  pContext->setFont(fontId);
  pContext->setFontColor(pal.foreground());
  pContext->drawString(label.c_str(), CRect(0.0, 0.0, width, height), kCenterText, true);

  setDirty(false);
}

CMouseEventResult SplashLabel::onMouseDown(CPoint &where, const CButtonState &buttons)
{
  splashView->setVisible(true);
  return kMouseEventHandled;
}

CMouseEventResult SplashLabel::onMouseEntered(CPoint &where, const CButtonState &buttons)
{
  isMouseEntered = true;
  invalid();
  return kMouseEventHandled;
}

CMouseEventResult SplashLabel::onMouseExited(CPoint &where, const CButtonState &buttons)
{
  isMouseEntered = false;
  invalid();
  return kMouseEventHandled;
}

CMouseEventResult SplashLabel::onMouseCancel()
{
  isMouseEntered = false;
  return kMouseEventHandled;
}

void SplashLabel::setDefaultFrameWidth(CCoord width) { frameWidth = width; }
void SplashLabel::setHighlightWidth(CCoord width) { highlightFrameWidth = width; }

} // namespace Vst
} // namespace Steinberg
