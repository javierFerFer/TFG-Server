#include "../Headers/JsonObject.h"

JsonObject::JsonObject(string titleParam, vector<string> contentParam) {
	this->title = titleParam;
	this->content = contentParam;
}

JsonObject::JsonObject() {}
