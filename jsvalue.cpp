//
// Created by Shlegeris, Buck on 10/23/15.
//

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <math.h>

#include "jsvalue.h"

using namespace std;


class JSNumber : public JSValue {
    double value;

public:
    JSType jsType = JSNumberType;

    JSNumber(double value) : value(value) {}

    string toString() {
        return to_string(value);
    }

    JSValue *operate(std::string op, JSValue *other) {
        switch (other->jsType)
        {
            case JSNumberType: {
                double otherValue = ((JSNumber *) other)->value;
                if (op == "+")
                    return new JSNumber(value + otherValue);
                if (op == "-")
                    return new JSNumber(value - otherValue);
                if (op == "*")
                    return new JSNumber(value * otherValue);
                if (op == "/")
                    return new JSNumber(value / otherValue);
                if (op == "<")
                    return makeJSBool(value < otherValue);
                break;
            }
            case JSUndefinedType: {
                return new JSNumber(NAN);
            }
            case JSBoolType: {
                return operate(op, new JSNumber(other->isTruthy() ? 1 : 0));
            }
        }
        cout << "note: unimplemented operate: " << toString() << op << other->toString() << endl;
        return jsUndefined;
    }

    double getValue() {
        return value;
    }


    bool isTruthy() {
        return value != 0;
    }

    JSValue *get(std::string key) {
        exit(153);
    }
};

JSValue * number(double value) {
    return new JSNumber(value);
}

class JSUndefined : public JSValue {
public:
    JSType jsType = JSUndefinedType;

    JSValue *operate(std::string op, JSValue *other) {
        switch (other->jsType)
        {
            case JSNumberType: {
               return new JSNumber(NAN);
            }
            case JSUndefinedType: {
                return new JSNumber(NAN);
            }
            case JSBoolType: {
                return operate(op, new JSNumber(other->isTruthy() ? 1 : 0));
            }
        }
        cout << "note: unimplemented operate: " << toString() << op << other->toString() << endl;
        return jsUndefined;
    }

    string toString() {
        return "undefined";
    }

    bool isTruthy() {
        return false;
    }

    JSValue *get(std::string key) {
        exit(123);
    }
};


JSUndefined _jsUndefined = JSUndefined();
JSValue *jsUndefined = &_jsUndefined;

class JSObject : public JSValue {
    std::map<string, JSValue*> fields;

public:
    JSType jsType = JSObjectType;

    JSObject(std::map<string, JSValue*> fields): fields(fields) { }

    JSValue *operate(std::string op, JSValue *other) {
        switch (other->jsType)
        {
            case JSNumberType: {
                return new JSNumber(NAN);
            }
            case JSUndefinedType: {
                return new JSNumber(NAN);
            }
            case JSBoolType: {
                return operate(op, new JSNumber(other->isTruthy() ? 1 : 0));
            }
        }
        cout << "note: unimplemented operate: " << toString() << op << other->toString() << endl;
        return jsUndefined;
    }

    string toString() {
        return "object<...>";
    }

    bool isTruthy() {
        return !fields.empty();
    }

    JSValue *get(string field) {
        if (fields.count(field)) {
            return fields[field];
        }
        return jsUndefined;
    }

    JSValue *set(string field, JSValue *value) {
        fields[field] = value;
        return value;
    }
};

JSValue * object(std::map<std::string, JSValue*> fields) {
    return new JSObject(fields);
}

class JSBool : public JSValue {
    bool value;
public:
    JSType jsType = JSBoolType;

    JSValue *operate(std::string op, JSValue *other) {
       return (new JSNumber(value ? 1 : 0))->operate(op, other);
    }

    JSBool(bool value) : value(value) {}

    string toString() {
        return to_string(value);
    }

    bool isTruthy() {
        return value;
    }

    JSValue *get(std::string key) {
        return jsUndefined;
    }
};

JSValue *makeJSBool(bool value) {
    return new JSBool(value);
}

class JSExternalFunction : public JSValue {
    JSFunction function;
public:
    JSType jsType = JSFunctionType;

    JSExternalFunction(JSFunction function): function(function) { };

    string toString() {
        return "function<something>";
    }

    JSValue *operate(std::string op, JSValue *other) {
        cout << "note: unimplemented operate: " << toString() << op << other->toString() << endl;
        return jsUndefined;
    }

    JSValue * call(std::vector<JSValue *> args) {
        return function(args);
    }

    bool isTruthy() {
        return true;
    }

    JSValue *get(std::string key) {
        return jsUndefined;
    }
};

class JSString : public JSValue {
    string value;

public:
    JSType jsType = JSStringType;

    JSString(string value): value(value) { }

    JSValue *operate(std::string op, JSValue *other) {
        if (op == "+") {
            return new JSString(value + other->toString());
        }
        cout << "note: unimplemented operate: " << toString() << op << other->toString() << endl;
        return jsUndefined;
    }

    string toString() {
        return value;
    }

    bool isTruthy() {
        return value.length() > 0;
    }

    JSValue *get(std::string key) {
        return jsUndefined;
    }
};

JSValue * jsString(std::string value) {
    return new JSString(value);
}

JSValue *log(std::vector<JSValue *> args) {
    for (JSValue * value : args) {
        cout << value->toString() << " ";
    }
    cout << endl;
    return jsUndefined;
}

JSExternalFunction logFunctionObject(&log);
JSValue * logFunction = &logFunctionObject;
