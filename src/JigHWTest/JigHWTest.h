#ifndef CHATTER_FIRMWARE_JIGHWTEST_H
#define CHATTER_FIRMWARE_JIGHWTEST_H

#include <Display/Display.h>
#include <Util/Vector.h>
#include <Input/Input.h>
#include <FS.h>
#include <Util/Task.h>

struct Test {
	bool (*test)();
	const char* name;
	void (*onFail)();
};

class JigHWTest
{
public:
	JigHWTest(Display* display);
	void start();

private:
	Sprite *canvas;
	Display *display;

	static JigHWTest* test;
	Vector<Test> tests;
	const char* currentTest;

	void log(const char* property, const char* value);
	void log(const char* property, float value);
	void log(const char* property, double value);
	void log(const char* property, bool value);
	void log(const char *property, uint32_t value);
	void log(const char *property, int32_t value);
	void log(const char *property, const String& value);

	static bool LoRaTest();
	static bool BatteryCalib();
	static bool BatteryCheck();
	static bool SPIFFSTest();

	static uint32_t calcChecksum(fs::File& file);

	static const int16_t referenceVoltage = 3597;
};

#endif //CHATTER_FIRMWARE_JIGHWTEST_H
