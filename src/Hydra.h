/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.
(c) 2020 Artem Lutov
 */

#ifndef HYDRA_H_
#define HYDRA_H_

#include "ESPAsyncWebServer.h"
#include "RDF.hpp"


namespace smallhydra
{

using namespace smallrdf;
using smallrdf::String;

//class AsyncWebServerRequest;  // Defined in ESPAsyncWebServer

class Hydra {
	Document document;
	Dataset* raw;
	String path;
	Dataset api;

	static String  localhostApi;
public:
	Hydra();
	~Hydra();

	void begin(const String& filename, const String& path, const String& hostname = String());

	String absoluteUrl(const AsyncWebServerRequest& request);
	void handleRequest(const AsyncWebServerRequest& request,
	                   AsyncWebServerResponse& response);
	void handleApiRequest(AsyncWebServerRequest& request);
protected:
	void readApiDocument(const String& filename);
	Dataset& patchApiDocument(const String& hostname);
};

}  // smallhydra

#endif  // HYDRA_H_
