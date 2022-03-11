#include "usb_reports.h"

local USB_NkroKeyboardReport_t _previousReport;

bool USB_populate_boot_keyboard_report(USB_BootKeyboardReport_t *report) {
	USB_NkroKeyboardReport_t nkroReport = { 0 };
	bool hasUpdated = USB_populate_nkro_keyboard_report(&nkroReport);

	// Modifiers are 0xE0-0xE7, so they take up exactly one page of the NKRO report
	report->modifiers = nkroReport.keys[0xE0 / 8];

	for (u8 keyCodeIndex = 0, page = 0; page < 32 && keyCodeIndex < 6; ++page) {
		// Skip modifier key page
		if (page == 0xE0 / 8) {
			continue;
		}

		if (nkroReport.keys[page]) {
			for (u8 pageIndex = 0; pageIndex < 8 && keyCodeIndex < 6; ++pageIndex) {
				if (nkroReport.keys[page] & _BV(pageIndex)) {
					report->keyCodes[keyCodeIndex++] = (page * 8) + pageIndex;
				}
			}
		}
	}

	return hasUpdated;
}

bool USB_populate_nkro_keyboard_report(USB_NkroKeyboardReport_t *report) {
	keyboard_get_scancodes(report);
	bool hasUpdated = memcmp(&_previousReport, report, sizeof(_previousReport)) != 0;
	memcpy(&_previousReport, report, sizeof(_previousReport));
	return hasUpdated;
}

void USB_process_led_report(USB_LedReport_t report) {
	keyboard_set_status(report);
}
