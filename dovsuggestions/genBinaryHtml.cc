#include <iostream>
#include <fstream>

#include "CompressedHtml.hpp"

int main() {
	ofstream f("test1.htb", ios::binary);

	const uint8_t DEFAULT =  Style::define(0, 0, 0, 0x000000, 0xFF0000, Style::ARIAL, 12, Style::PLAIN); //default style 0
	const uint8_t BIG_TEXT = Style::define(1, 800, 400, 0x000000, 0xFFFFFF, Style::ARIAL, 24, Style::BOLD); // register style 1
	const uint8_t TEXT = Style::define(2, 0, 0, 0x000000, 0xFFFFFF, Style::ARIAL, 24, Style::BOLD); // register style 2, has now width/height
	Style::generate(f);
	
	f <<
		uint8_t(HTML) << DEFAULT << // HTML tag with default class ID
		uint8_t(HEAD) << DEFAULT << // HTML tag with default class ID
		uint8_t(BODY) << DEFAULT << // HTML tag with default class ID
		uint8_t(DIV) << BIG_TEXT << // HTML tag with class ID=1
