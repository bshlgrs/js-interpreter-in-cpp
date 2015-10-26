//
// Created by Shlegeris, Buck on 10/23/15.
//

#ifndef CPP_LANGUAGE_THING_JSVALUE_H
#define CPP_LANGUAGE_THING_JSVALUE_H

#include <vector>

enum JSType { JSNumberType, JSUndefinedType, JSBoolType, JSStringType, JSObjectType, JSFunctionType, JSArrayType };

class JSValue {
public:
    JSType jsType;
    virtual std::string toString() = 0;
    virtual JSValue * operate(std::string op, JSValue *other) = 0;
    virtual JSValue * get(std::string key) = 0;
    virtual JSValue * set(std::string key, JSValue *value) {
        exit(123);
    }
    virtual bool isTruthy() = 0;
    virtual JSValue * call(std::vector<JSValue *> args) {
        exit(532);
    }
};

class JSNumber;
class JSUndefined;
class JSExternalFunction;
class JSObject;

extern JSValue *jsUndefined;

JSValue * number(double value);
JSValue * jsString(std::string value);
JSValue * object(std::map<std::string, JSValue*> fields);

class JSBool;
extern JSValue *makeJSBool(bool value);

typedef JSValue *(*JSFunction)(std::vector<JSValue *> args);

extern JSValue * logFunction;

#endif //CPP_LANGUAGE_THING_JSVALUE_H
