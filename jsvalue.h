//
// Created by Shlegeris, Buck on 10/23/15.
//

#ifndef CPP_LANGUAGE_THING_JSVALUE_H
#define CPP_LANGUAGE_THING_JSVALUE_H

#include <vector>

class JSValue;

class JSNumber;
class JSUndefined;

class JSBool;
extern JSValue *makeJSBool(bool value);

typedef JSValue *(*JSFunction)(std::vector<JSValue *> args);


#endif //CPP_LANGUAGE_THING_JSVALUE_H
