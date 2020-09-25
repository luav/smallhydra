// (c) 2016 Thomas Bergwinkl
// (c) 2020 Artem Lutov

// namespace espdefs {
#ifdef ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#else
#include <Arduino.h>
#include <ESP8266WiFi.h>
// #include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif  // ESP32
#include <FS.h>
#include "SPIFFS.h"
// }  // espdefs

#include "Hydra.h"
#include "RDF.hpp"
#include "NTriplesParser.h"
#include "NTriplesSerializer.h"


using namespace smallhydra;
using namespace smallrdf;
// using namespace espdefs;
using RdfString = smallrdf::String;
using smallrdf::AString;

const char* ssid = "";
const char* password = "";

void initWifi()
{
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
}

RdfString Type("http://www.w3.org/1999/02/22--syntax-ns#type");
RdfString dhLightClass("http://ns.bergnet.org/dark-horse#Light");
RdfString dhOffClass("http://ns.bergnet.org/dark-horse#Off");
RdfString dhOnClass("http://ns.bergnet.org/dark-horse#On");
RdfString dhState("http://ns.bergnet.org/dark-horse#state");
AsyncWebServer server(80);
Hydra hydra;

class Led
{
public:
	Led(int pin): pin(pin) {}

	void begin()
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH);
	}

	bool isOn()
	{
		return digitalRead(pin) == LOW;
	}

	void on()
	{
		digitalWrite(pin, LOW);
	}

	void off()
	{
		digitalWrite(pin, HIGH);
	}

	void get(AsyncWebServerRequest* request)
	{
		Document document;

		const RdfString* iri = document.string(RdfString(hydra.absoluteUrl(*request), true));
		const NamedNode* subject = document.namedNode(*iri);

		const Quad* typeQuad = document.quad(*subject, *document.namedNode(Type), *document.namedNode(dhLightClass));

		RdfString* status = isOn() ? &dhOnClass : &dhOffClass;
		const Quad* statusQuad = document.quad(*subject, *document.namedNode(dhState), *document.namedNode(*status));

		NTriplesSerializer  ser;
		AsyncWebServerResponse* response = request->beginResponse(200, "application/n-triples", ser.serialize(document).c_str());
		hydra.handleRequest(*request, *response);
		request->send(response);
	}

	void put(AsyncWebServerRequest* request, uint8_t* data, size_t len)
	{
		Document document;
		NTriplesParser  prs;
		prs.parse(*document.string(RdfString(data, len)));
		const Quad* state = document.find(Quad(0, document.namedNode(dhState)));

		if (state) {
			if (*state->object->value == dhOnClass) {
				on();
			} else if (*state->object->value == dhOffClass) {
				off();
			}
		}

		get(request);
	}

protected:
	int pin;
};

Led led0(0);

void setup ()
{
	Serial.begin(115200);
	initWifi();
	SPIFFS.begin();
	hydra.begin(RdfString("/api.nt"), RdfString("api"));
	led0.begin();

	server.on("/api", HTTP_GET, [&hydra](AsyncWebServerRequest* request) {
		Serial.println("GET /api");

		hydra.handleApiRequest(*request);
	});

	server.on("/", HTTP_GET, [&led0](AsyncWebServerRequest* request) {
		Serial.println("GET /");

		led0.get(request);
	});

	server.on("/", HTTP_PUT, [](AsyncWebServerRequest* request) {
	}, [](AsyncWebServerRequest* request, AString filename, size_t index, uint8_t* data, size_t len, bool final) {
	}, [&led0](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
		Serial.println("PUT /");

		led0.put(request, data, len);
	});

	server.begin();

	Serial.println("http://" + WiFi.localIP().toString() + "/");
}

void loop ()
{
}
