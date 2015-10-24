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

class JSValue {
public:
    virtual std::string toString() = 0;
    virtual JSValue * operate(std::string op, JSValue *other) = 0;
    virtual JSValue * rOperateWithNumber(std::string op, JSNumber *left) = 0;
    virtual JSValue * rOperateWithUndefined(std::string op) = 0;
    virtual JSValue * get(std::string key) = 0;
    virtual JSValue * set(std::string key, JSValue *value) {
        cout << "you can't set " << toString() << ":(\n";
        exit(123);
    }
    virtual bool isTruthy() = 0;
    virtual JSValue * call(std::vector<JSValue *> args) {
        cout << "you can't call " << toString() << " :( \n";
        exit(532);
    }
};

class JSNumber : public JSValue {
public:
    double value;

    JSNumber(double value) : value(value) {}

    string toString() {
        return to_string(value);
    }

    JSValue *operate(string op, JSValue *right) {
//        cout << typeid(*right) << endl;

        return right->rOperateWithNumber(op, this);
    }

    JSValue *rOperateWithUndefined(string op, JSNumber *right) {
        return new JSNumber(NAN);
    }

    JSValue *rOperateWithNumber(string op, JSNumber *left) {
        if (op == "+")
            return new JSNumber(left->value + value);
        else if (op == "-")
            return new JSNumber(left->value - value);
        else if (op == "*")
            return new JSNumber(left->value * value);
        else if (op == "/")
            return new JSNumber(left->value / value);
        else if (op == "==")
            return new JSNumber(left->value == value);
        else if (op == "<")
            return new JSNumber(left->value < value);
        else
            exit(154);
    }

    JSValue *rOperateWithUndefined(string op) {
        if (op == "+")
            return new JSNumber(NAN);
        else if (op == "-")
            return new JSNumber(NAN);
        else if (op == "*")
            return new JSNumber(NAN);
        else if (op == "/")
            return new JSNumber(NAN);
        else if (op == "==")
            return makeJSBool(false);
        else if (op == "<")
            return makeJSBool(false);
        else
            exit(154);
    }

    bool isTruthy() {
        return value != 0;
    }

    JSValue *get(std::string key) {
        exit(153);
    }
};

class JSUndefined : public JSValue {
public:
    string toString() {
        return "undefined";
    }

    JSValue *operate(string op, JSValue *right) {
        return right->rOperateWithUndefined(op);
    }

    JSValue *rOperateWithNumber(string op, JSNumber *left) {
        return new JSNumber(NAN);
    }

    JSValue *rOperateWithUndefined(string op) {
        exit(354);
    }

    bool isTruthy() {
        return false;
    }

    JSValue *get(std::string key) {
        exit(123);
    }
};


JSUndefined jsUndefined;

class JSObject : public JSValue {
    std::map<string, JSValue*> fields;

public:
    JSObject(std::map<string, JSValue*> fields): fields(fields) { }

    string toString() {
        return "object<...>";
    }

    // these are wrong :/
    JSValue *operate(string op, JSValue *right) {
        return right->rOperateWithUndefined(op);
    }

    JSValue *rOperateWithNumber(string op, JSNumber *left) {
        return new JSNumber(NAN);
    }

    JSValue *rOperateWithUndefined(string op) {
        exit(354);
    }

    bool isTruthy() {
        return !fields.empty();
    }

    JSValue *get(string field) {
        if (fields.count(field)) {
            return fields[field];
        }
        return &jsUndefined;
    }

    JSValue *set(string field, JSValue *value) {
        fields[field] = value;
        return value;
    }
};

class JSBool : public JSValue {
    bool value;
public:
    JSBool(bool value) : value(value) {}

    string toString() {
        return to_string(value);
    }

    JSValue *operate(string op, JSValue *right) {
        return right->rOperateWithNumber(op, new JSNumber(value));
    }

    JSValue *rOperateWithUndefined(string op, JSNumber *right) {
        exit(123);
    }

    JSValue *rOperateWithNumber(string op, JSNumber *left) {
        if (op == "+")
            return new JSNumber(left->value + (int) value);
        else if (op == "-")
            return new JSNumber(left->value - (int) value);
        else if (op == "*")
            return new JSNumber(left->value * (int) value);
        else if (op == "/")
            return new JSNumber(left->value / (int) value);
        else if (op == "==")
            return new JSNumber(left->value == (int) value);
        else if (op == "<")
            return new JSNumber(left->value < (int) value);
        else
            exit(154);
    }

    JSValue *rOperateWithUndefined(string op) {
        if (op == "+")
            return new JSNumber(NAN);
        else if (op == "-")
            return new JSNumber(NAN);
        else if (op == "*")
            return new JSNumber(NAN);
        else if (op == "/")
            return new JSNumber(NAN);
        else if (op == "==")
            return new JSBool(false);
        else if (op == "<")
            return new JSBool(false);
        else
            exit(154);
    }

    bool isTruthy() {
        return value;
    }

    JSValue *get(std::string key) {
        return &jsUndefined;
    }
};

JSValue *makeJSBool(bool value) {
    return new JSBool(value);
}

class JSExternalFunction : public JSValue {
    JSFunction function;
public:
    JSExternalFunction(JSFunction function): function(function) { };

    string toString() {
        return "function<something>";
    }

    JSValue *operate(string op, JSValue *right) {
        exit(123);
    }

    JSValue *rOperateWithUndefined(string op, JSNumber *right) {
        exit(123);
    }

    JSValue *rOperateWithNumber(string op, JSNumber *left) {
        exit(154);
    }

    JSValue *rOperateWithUndefined(string op) {
        exit(154);
    }

    JSValue * call(std::vector<JSValue *> args) {
        return function(args);
    }

    bool isTruthy() {
        return true;
    }

    JSValue *get(std::string key) {
        return &jsUndefined;
    }
};

class JSString : public JSValue {
    string value;

public:
    JSString(string value): value(value) { }

    string toString() {
        return value;
    }

    JSValue *operate(string op, JSValue *right) {
        exit(124);
//        return right->rOperateWithNumber(op, new JSNumber(value));
    }

    JSValue *rOperateWithNumber(string op, JSNumber *left) {
        exit(154);
    }

    JSValue *rOperateWithUndefined(string op) {
        exit(154);
    }

    bool isTruthy() {
        return value.length() > 0;
    }

    JSValue *get(std::string key) {
        return &jsUndefined;
    }
};

