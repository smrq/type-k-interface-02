#include "usb_reports.h"

bool USB_using_report_protocol = true;

local USB_NkroKeyboardReport_t _nkroReport = { 0 };
local USB_BootKeyboardReport_t _bootReport = { 0 };
local bool _dirty = false;

local USB_LedReport_t _ledReport;

void USB_set_keyboard_report(USB_NkroKeyboardReport_t *report) {
	_dirty = memcmp(&_nkroReport, report, sizeof(_nkroReport)) != 0;
	memcpy(&_nkroReport, report, sizeof(_nkroReport));
}

bool USB_is_keyboard_report_dirty() {
	return _dirty;
}

local void _populate_boot_report() {
	// Modifiers are 0xE0-0xE7, so they take up exactly one page of the NKRO report
	_bootReport.modifiers = _nkroReport.keys[0xE0 / 8];

	for (u8 keyCodeIndex = 0, page = 0; page < 32 && keyCodeIndex < 6; ++page) {
		// Skip modifier key page
		if (page == 0xE0 / 8) {
			continue;
		}

		if (_nkroReport.keys[page]) {
			for (u8 pageIndex = 0; pageIndex < 8 && keyCodeIndex < 6; ++pageIndex) {
				if (_nkroReport.keys[page] & _BV(pageIndex)) {
					_bootReport.keyCodes[keyCodeIndex++] = (page * 8) + pageIndex;
				}
			}
		}
	}
}

void USB_get_keyboard_report(void **report, u16 *length) {
	if (USB_using_report_protocol) {
		*report = &_nkroReport;
		*length = sizeof(_nkroReport);
	} else {
		_populate_boot_report();
		*report = &_bootReport;
		*length = sizeof(_bootReport);
	}
}

void USB_set_led_report(USB_LedReport_t report) {
	_ledReport = report;
}

USB_LedReport_t USB_get_led_report() {
	return _ledReport;
}
