#include <SPIFFS.h>
#include <Battery/BatteryService.h>
#include <TypeDef.h>
#include <modules/LLCC68/LLCC68.h>
#include <Settings.h>
#include "JigHWTest.h"
#include "SPIFFSChecksum.hpp"
#include <Pins.hpp>
#include <Chatter.h>
#include <soc/efuse_reg.h>
#include <esp_efuse.h>

#define LED_R 15
#define LED_G 26
#define LED_B 27

JigHWTest *JigHWTest::test = nullptr;

JigHWTest::JigHWTest(Display* display, bool manual) : display(display), canvas(display->getBaseSprite()){
	test = this;

	tests.push_back({JigHWTest::LoRaTest, "LoRa", Pixel::Blue});
	tests.push_back({JigHWTest::SPIFFSTest, "SPIFFS", Pixel::Magenta});
	// tests.push_back({JigHWTest::BatteryCalib, "Bat calib", Pixel::Yellow});
	tests.push_back({JigHWTest::BatteryCheck, "Bat check", Pixel::Red});

	if(manual){
		Chatter.fadeIn();
	}else{
		SPI.end();

		this->display = nullptr;
		this->canvas = nullptr;

		for(auto pin : { LED_R, LED_G, LED_B }){
			pinMode(pin, OUTPUT);
			digitalWrite(pin, HIGH);
		}
	}
}

void JigHWTest::start(){
	Serial.println();
	Serial.printf("TEST:begin:%llx\n", ESP.getEfuseMac());

	if(display){
		canvas->clear(TFT_BLACK);
		canvas->setTextColor(TFT_GOLD);
		canvas->setTextWrap(false, false);
		canvas->setTextDatum(textdatum_t::middle_center);

		canvas->setTextFont(0);
		canvas->setTextSize(1);
		canvas->setCursor(0, 6);

		canvas->printCenter("Chatter Hardware Test");
		canvas->setCursor(canvas->width() / 2, 16);
		canvas->println();
		display->commit();
	}

	bool pass = true;
	for(const Test &test : tests){
		currentTest = test.name;

		if(display){
			canvas->setTextColor(TFT_WHITE);
			canvas->printf("%s: ", test.name);
			display->commit();
		}

		Serial.printf("TEST:startTest:%s\n", currentTest);

		bool result = test.test();

		if(display){
			canvas->setTextColor(result ? TFT_GREEN : TFT_RED);
			canvas->printf("%s\n", result ? "PASSED" : "FAILED");
			display->commit();
		}

		Serial.printf("TEST:endTest:%s\n", result ? "pass" : "fail");

		if(!(pass &= result)){
			if(!display){
				auto color = test.color;
				if(color.r == 255) digitalWrite(LED_R, LOW);
				if(color.g == 255) digitalWrite(LED_G, LOW);
				if(color.b == 255) digitalWrite(LED_B, LOW);
			}

			break;
		}
	}

	if(!pass){
		Serial.printf("TEST:fail:%s\n", currentTest);
		for(;;);
	}

	Serial.println("TEST:passall");

	if(display){
		canvas->print("\n\n");
		canvas->setTextColor(TFT_GREEN);
		canvas->printCenter("All OK!");
		display->commit();
	}else{
		digitalWrite(LED_G, LOW);
	}
}

void JigHWTest::log(const char* property, const char* value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, float value){
	Serial.printf("%s:%s:%f\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, double value){
	Serial.printf("%s:%s:%lf\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, bool value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value ? "TRUE" : "FALSE");
}

void JigHWTest::log(const char* property, uint32_t value){
	Serial.printf("%s:%s:%lu\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, int32_t value){
	Serial.printf("%s:%s:%ld\n", currentTest, property, value);
}

void JigHWTest::log(const char* property, const String& value){
	Serial.printf("%s:%s:%s\n", currentTest, property, value.c_str());
}

bool JigHWTest::LoRaTest(){
	LLCC68 radio(new Module(RADIO_CS, RADIO_DIO1, RADIO_RST, RADIO_BUSY, Chatter.getSPILoRa()));
	int state = radio.begin(868, 500, 9, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 22, 8, 0, false);

	if(state != RADIOLIB_ERR_NONE){
		test->log("begin", false);
		return false;
	}
	return true;
}

bool JigHWTest::BatteryCalib(){
	return true;

	if(Battery.getVoltOffset() != 0){
		test->log("calibrated", Battery.getVoltOffset());
		if(test->display) test->display->getBaseSprite()->print("fused. ");
		return true;
	}

	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 50;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += analogRead(BATTERY_PIN);
		delay(readDelay);
	}
	reading /= numReadings;


	int32_t mapped = Battery.mapReading(reading);

	int16_t offset = referenceVoltage - mapped;

	test->log("reading", reading);
	test->log("mapped", mapped);
	test->log("offset", offset);

	if(abs(offset) >= 1000){
		test->log("offset too big, read voltage: ", (uint32_t)reading);
		return false;
	}

	uint16_t offsetLow = offset & 0b01111111;
	uint16_t offsetHigh = offset >> 7;

	REG_SET_FIELD(EFUSE_BLK3_WDATA3_REG, EFUSE_ADC1_TP_LOW, offsetLow);
	REG_SET_FIELD(EFUSE_BLK3_WDATA3_REG, EFUSE_ADC1_TP_HIGH, offsetHigh);
	esp_efuse_burn_new_values();
	esp_efuse_reset();

	return true;
}

bool JigHWTest::BatteryCheck(){
	constexpr uint16_t numReadings = 50;
	constexpr uint16_t readDelay = 10;
	uint32_t reading = 0;

	for(int i = 0; i < numReadings; i++){
		reading += analogRead(BATTERY_PIN);
		delay(readDelay);
	}
	reading /= numReadings;

	uint32_t voltage = Battery.mapReading(reading) + Battery.getVoltOffset();
	if(voltage < referenceVoltage - 100 || voltage > referenceVoltage + 100){
		test->log("raw", reading);
		test->log("mapped", Battery.mapReading(reading));
		test->log("offset", Battery.getVoltOffset());
		test->log("mapped+offset", voltage);
		return false;
	}

	return true;
}

bool JigHWTest::SPIFFSTest(){
	for(const auto& f : SPIFFSChecksums){
		if(!SPIFFS.exists(f.name)){
			test->log("missing", f.name);
			return false;
		}

		fs::File file = SPIFFS.open(f.name, "r");
		uint32_t sum = calcChecksum(file);
		file.close();

		if(sum != f.sum){
			test->log("file", f.name);
			test->log("expected", (uint32_t) f.sum);
			test->log("got", (uint32_t) sum);

			return false;
		}
	}

	return true;
}

uint32_t JigHWTest::calcChecksum(File& file){
	if(!file) return 0;

#define READ_SIZE 512

	uint32_t sum = 0;
	uint8_t b[READ_SIZE];
	size_t read = 0;
	while(read = file.read(b, READ_SIZE)){
		for(int i = 0; i < read; i++){
			sum += b[i];
		}
	}

	return sum;
}


