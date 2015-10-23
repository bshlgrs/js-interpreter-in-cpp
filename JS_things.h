//
// Created by Shlegeris, Buck on 10/18/15.
//

#ifndef CPP_LANGUAGE_THING_JS_THINGS_H
#define CPP_LANGUAGE_THING_JS_THINGS_H

class JSNumber;

class JSValue;
class JSUndefined;
class JSBool;
//JSBool::JSBool(bool value);
JSValue *makeJSBool(bool value);

typedef JSValue *(*JSFunction)(std::vector<JSValue *> args);

#endif //CPP_LANGUAGE_THING_JS_THINGS_H
