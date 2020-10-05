/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.
(c) 2020 Artem Lutov
 */

#include "ESPAsyncWebServer.h"
#include "NTriplesParser.h"
#include "NTriplesSerializer.h"
#include "Hydra.h"

using namespace smallhydra;

String  localhostApi = String("http://localhost/api", true);

Hydra::Hydra()
	: raw(nullptr),
	  api()
{
}

Hydra::~Hydra()
{
	if(raw) {
		delete raw;
		raw = nullptr;
	}
}

void Hydra::begin(const String& filename, const String& path, const String& hostname)
{
	this->path = String(path);  // Acquire the ownership
	readApiDocument(filename);
}

String Hydra::absoluteUrl(const AsyncWebServerRequest& request)
{
	String url = String("http://");

	if (request.host().length() != 0)
		url += String(request.host());
	else url += String(WiFi.localIP().toString());

	url += String(request.url());
	if(!url.allocated())
		url.acquire();

	return url;
}

void Hydra::handleRequest(const AsyncWebServerRequest& request,
                          AsyncWebServerResponse& response)
{
	response.addHeader(
	    "Link",
	    "<" + AString(absoluteUrl(request) += path)
	    + ">; rel=\"http://www.w3.org/ns/hydra/core#apiDocumentation\"");
}

void Hydra::handleApiRequest(AsyncWebServerRequest& request)
{
	if (!api) {
		String hostname = request.host();
		if (hostname.length() == 0)
			hostname = WiFi.localIP().toString();
		api = patchApiDocument(hostname);
	}

	NTriplesSerializer  ser;
	request.send(200, "application/n-triples", ser.serialize(api).c_str());
}

void Hydra::readApiDocument(const String& path)
{
	File file = SPIFFS.open(AString(path), "r");
	if (!file)
		return;
	NTriplesParser  prs;
	prs.parse(String(file.readString(), true));
	if(raw)
		delete raw;
	raw = prs.release();
	for(Document::Quads::Iter* piq = document.quads.begin(); piq != document.quads.end(); piq = piq->next())
		raw.quads.add(**piq);
}

Dataset& Hydra::patchApiDocument(const String& hostname)
{
	String  hnameUrl = String("http://");
	hnameUrl += hostname.length() != 0 ? hostname : String(WiFi.localIP().toString());
	hnameUrl += String("/");
	hnameUrl += path;
	const Term* apiTerm = document.namedNode(hnameUrl);

	if (!raw)
		return document;
	Dataset& patched = document;
	for(Document::Quads::Iter* piq = document.quads.begin(); piq != document.quads.end(); piq = piq->next()) {
		const Quad& triple = **piq;
		const Term* subject = triple.subject;
		const Term* object = triple.object;

		if (*ubject->value == localhostApi)
			subject = apiTerm;

		if (*object->value == localhostApi)
			object = apiTerm;

		patched.quads.add(document.quad(*subject, *triple.predicate, *object));
	}

	return patched;
}
