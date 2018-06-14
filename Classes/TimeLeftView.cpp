#include "TimeLeftView.h"
#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
constexpr float kPi = 3.14159265359f;
constexpr float kBorderRadius = 6.0f;
constexpr float kBorderWidth = 4.0f;
const Vec2 kMargins = { 8.0f, 6.0f };
const Color4F kFillColor = { 1.0f, 0.86f, 0.39f, 1.0f };
const Color4F kBorderColor = { 1.0f, 1.0f, 1.0f, 1.0f };

Rect deflateRect(const Rect &value, const Vec2 &delta)
{
	return {
		value.origin + Vec2{ delta.x, delta.y },
		value.size - Size{ 2 * delta.x, 2 * delta.y }
	};
}

float clamp(float value, float minValue, float maxValue)
{
	return (std::min)(maxValue, (std::max)(minValue, value));
}

std::vector<Vec2> polygonizeRoundRect(const Rect &bounds, float borderRadius)
{
	// All angles in this function measured in radians.
	const float angleStep = 1.0f / borderRadius;
	std::vector<Vec2> points;
	const auto addSector = [&](const Vec2 &position, float angleFrom, float angleTo) {
		for (float angle = angleFrom; angle < angleTo + 0.5f * angleStep; angle += angleStep)
		{
			const Vec2 offset = { borderRadius * std::sin(angle), borderRadius * std::cos(angle) };
			points.push_back(position + offset);
		}
	};

	addSector(Vec2{ bounds.getMaxX() - borderRadius, bounds.getMaxY() - borderRadius },
		0.0f * kPi, 0.5f * kPi);
	addSector(Vec2{ bounds.getMaxX() - borderRadius, bounds.getMinY() + borderRadius },
		0.5f * kPi, 1.0f * kPi);
	addSector(Vec2{ bounds.getMinX() + borderRadius, bounds.getMinY() + borderRadius },
		1.0f * kPi, 1.5f * kPi);
	addSector(Vec2{ bounds.getMinX() + borderRadius, bounds.getMaxY() - borderRadius },
		1.5f * kPi, 2.0f * kPi);

	return points;
}

// Crops path to given bounds by moving all points that are out of bounds to boundary.
std::vector<Vec2> cropPath(std::vector<Vec2> points, const Rect &bounds)
{
	for (Vec2 &point : points)
	{
		point.x = clamp(point.x, bounds.getMinX(), bounds.getMaxX());
		point.y = clamp(point.y, bounds.getMinY(), bounds.getMaxY());
	}
	return points;
}
} // namespace

TimeLeftView *TimeLeftView::create(const Size &contentSize)
{
	RefPtr<TimeLeftView> self = new (std::nothrow) TimeLeftView;
	if (self && self->initWithSize(contentSize))
	{
		return self;
	}
	return nullptr;
}

void TimeLeftView::setEstimatedTime(float seconds)
{
	if (m_estimatedTime != seconds)
	{
		m_estimatedTime = seconds;
		redraw();
	}
}

void TimeLeftView::setSecondsLeft(float value)
{
	if (m_secondsLeft != value)
	{
		m_secondsLeft = value;
		redraw();
	}
}

bool TimeLeftView::initWithSize(const Size &contentSize)
{
	if (!DrawNode::init())
	{
		return false;
	}
	try
	{
		setContentSize(contentSize);
	}
	catch (const std::exception &ex)
	{
		CCLOG("init failed: %s", ex.what());
		return false;
	}
	return true;
}

void TimeLeftView::redraw()
{
	const float borderRadius = (std::min)(kBorderRadius, 0.5f * getContentSize().height);
	const Size contentSize = getContentSize();
	const Rect drawRect = deflateRect(Rect{ Vec2{ 0, 0 }, contentSize }, kMargins);
	Rect fillRect = drawRect;
	fillRect.size.width *= m_secondsLeft / m_estimatedTime;

	const std::vector<Vec2> strokePath = polygonizeRoundRect(drawRect, borderRadius);
	const std::vector<Vec2> fillPath = cropPath(strokePath, fillRect);

	clear();
	setLineWidth(kBorderWidth);
	drawSolidPoly(fillPath.data(), static_cast<unsigned>(fillPath.size()), kFillColor);
	drawPoly(strokePath.data(), static_cast<unsigned>(strokePath.size()), true, kBorderColor);
}
