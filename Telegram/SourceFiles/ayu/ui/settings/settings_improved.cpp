// This is created by Anissimov12

#include "ayu/ui/settings/settings_improved.h"

#include "lang_auto.h"
#include "ayu/ayu_settings.h"
#include "ayu/ayu_ui_settings.h"
#include "ayu/ui/settings/ayu_builder.h"
#include "ayu/ui/settings/settings_main.h"
#include "settings/settings_builder.h"
#include "settings/settings_common.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"
#include "ui/boxes/single_choice_box.h"
#include "ui/wrap/vertical_layout.h"
#include "window/window_session_controller.h"

namespace Settings {

using namespace Builder;
using namespace AyuBuilder;

namespace {

void BuildCursorSection(SectionBuilder &builder, AyuSectionBuilder &ayu) {
	auto *settings = &AyuSettings::getInstance();

	builder.addSubsectionTitle(tr::ayu_ImprovedCursorHeader());

	const auto options = std::vector<QString>{
		tr::ayu_CursorShapeDefault(tr::now),
		tr::ayu_CursorShapeLine(tr::now),
		tr::ayu_CursorShapeBlock(tr::now),
		tr::ayu_CursorShapeUnderline(tr::now),
	};

	const auto getIndex = [](CursorShape val) {
		return static_cast<int>(val);
	};

	auto currentShapeVal = settings->cursorShapeValue()
		| rpl::map([=](CursorShape val) {
			return options[getIndex(val)];
		});

	const auto controller = builder.controller();
	builder.addButton({
		.id = u"ayu/cursorShape"_q,
		.title = tr::ayu_CursorShape(),
		.st = &st::settingsButtonNoIcon,
		.label = std::move(currentShapeVal),
		.onClick = [=] {
			controller->show(Box(
				[=](not_null<Ui::GenericBox*> box) {
					SingleChoiceBox(box, {
						.title = tr::ayu_CursorShape(),
						.options = options,
						.initialSelection = getIndex(settings->cursorShape()),
						.callback = [=](int index) {
							AyuSettings::getInstance().setCursorShape(
								static_cast<CursorShape>(index));
						},
					});
				}));
		},
	});

	constexpr auto kBlinkMin = 100;
	constexpr auto kBlinkMax = 2000;
	constexpr auto kBlinkStep = 50;
	constexpr auto kBlinkSteps = (kBlinkMax - kBlinkMin) / kBlinkStep + 1;

	const auto indexToMs = [](int index) -> int {
		return kBlinkMin + index * kBlinkStep;
	};

	ayu.addSlider({
		.id = u"ayu/cursorBlinkDelay"_q,
		.title = tr::ayu_CursorBlinkDelay(),
		.steps = kBlinkSteps,
		.current = settings->cursorBlinkDelay(),
		.indexToValue = indexToMs,
		.onChanged = [=](int ms) {
			AyuSettings::getInstance().setCursorBlinkDelay(ms);
		},
		.onFinalChanged = [=](int ms) {
			AyuSettings::getInstance().setCursorBlinkDelay(ms);
		},
		.formatLabel = [=](int ms) {
			return QString::number(ms) + u" ms"_q;
		},
	});

	ayu.addSectionDivider();
}

void BuildAnimationSection(SectionBuilder &builder, AyuSectionBuilder &ayu) {
	auto *settings = &AyuSettings::getInstance();

	builder.addSubsectionTitle(tr::ayu_ImprovedAnimationHeader());

	ayu.addSettingToggle({
		.id = u"ayu/cursorAnimation"_q,
		.title = tr::ayu_CursorAnimation(),
		.getter = &AyuSettings::cursorAnimationEnabled,
		.setter = &AyuSettings::setCursorAnimationEnabled,
	});

	constexpr auto kSpeedMin = 5;
	constexpr auto kSpeedMax = 95;
	constexpr auto kSpeedStep = 5;
	constexpr auto kSpeedSteps = (kSpeedMax - kSpeedMin) / kSpeedStep + 1;

	const auto indexToSpeed = [](int index) -> int {
		return kSpeedMin + index * kSpeedStep;
	};

	ayu.addSlider({
		.id = u"ayu/cursorAnimationSpeed"_q,
		.title = tr::ayu_CursorAnimationSpeed(),
		.steps = kSpeedSteps,
		.current = settings->cursorAnimationSpeed(),
		.indexToValue = indexToSpeed,
		.onChanged = [=](int val) {
			AyuSettings::getInstance().setCursorAnimationSpeed(val);
		},
		.onFinalChanged = [=](int val) {
			AyuSettings::getInstance().setCursorAnimationSpeed(val);
		},
		.formatLabel = [=](int val) {
			return QString::number(val) + u"%"_q;
		},
	});

	builder.addSkip();
	builder.addDividerText(tr::ayu_CursorAnimationDescription());
	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = AyuImproved::Id(),
	.parentId = AyuMain::Id(),
	.title = &tr::ayu_CategoryImproved,
	.icon = &st::menuIconPremium,
}, [](SectionBuilder &builder) {
	auto ayu = AyuSectionBuilder(builder);

	builder.addSkip();
	BuildCursorSection(builder, ayu);
	BuildAnimationSection(builder, ayu);
});

} // namespace

rpl::producer<QString> AyuImproved::title() {
	return tr::ayu_CategoryImproved();
}

AyuImproved::AyuImproved(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	setupContent();
}

void AyuImproved::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

Type AyuImprovedId() {
	return AyuImproved::Id();
}

}
