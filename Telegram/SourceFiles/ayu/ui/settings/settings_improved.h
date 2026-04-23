// This is created by Anissimov12

#pragma once

#include "settings/settings_common.h"
#include "settings/settings_common_session.h"

namespace Window {
class SessionController;
}

namespace Settings {

class AyuImproved : public Section<AyuImproved> {
public:
	AyuImproved(QWidget *parent, not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;

private:
	void setupContent();
};

[[nodiscard]] Type AyuImprovedId();

}
