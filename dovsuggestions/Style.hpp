#pragma once

class Style {
public:
	static Style define(uint8_t id, uint32_t w, uint32_t h, uint32_t bgColor, uint32_t fgColor,
											const char* fontFamily, uint32_t fontSize, FontMod);

	// write binary html header containing style definitions
	//TODO: remove and put in separate file? This way each file could share styles
	static void generate(ofstream& f);

};
