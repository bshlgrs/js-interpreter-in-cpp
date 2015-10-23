#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <math.h>
#include "JS_things.h"

using namespace std;

class JSValue {
public:
    virtual std::string toString() = 0;
    virtual JSValue * operate(std::string op, JSValue *other) = 0;
    virtual JSValue * rOperateWithNumber(std::string op, JSNumber *left) = 0;
    virtual JSValue * rOperateWithUndefined(std::string op) = 0;
    virtual JSValue * get(std::string key) {
        return new JSUndefined;
    };
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
};

JSUndefined *jsUndefined = new JSUndefined;

class JSObject : public JSValue {
    std::map<string, JSValue*> fields;

public:
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
            fields[field]
        }
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


};

JSValue *log(std::vector<JSValue *> args) {
    for (JSValue * value : args) {
        cout << value->toString() << "; ";
    }
    cout << endl;
    return jsUndefined;
}

class Scope {
    Scope *parent = NULL;
    std::map<string, JSValue*> values;

public:
    string toString() {
        string outputString = "Scope(";

        for (auto const& value : values) {
            outputString += value.first + " -> " + value.second->toString() + ", ";
        }

        if (parent != NULL) {
            outputString += " parent scope -> " + parent->toString();
        }

        return outputString  + ")";
    }

    JSValue* getValue(string name) {
        if (values.count(name) == 1)
            return values[name];
        else if (parent != NULL)
            return parent->getValue(name);
        else
            return NULL;
    }

    void setValue(string name, JSValue *value) {
        values[name] = value;
    }
};

class JSExpression {
public:
    virtual JSValue *evaluate(Scope *scope) = 0;

    virtual string toString() = 0;
};

class JSValueExpression : public JSExpression {
    JSValue *value;
public:
    JSValueExpression(JSValue *value): value(value) { }

    JSValue *evaluate(Scope *scope) {
        return value;
    }

    string toString() {
        return value->toString();
    }
};

class JSVariableExpression : public JSExpression {
    string variableName;

public:
    JSVariableExpression(string name): variableName(name) {}

    JSValue *evaluate(Scope *scope) {
        JSValue *result = scope->getValue(variableName);
        if (result != NULL) {
            return result;
        } else {
            cout << "variable not defined: " << variableName << endl;
            exit(145);
        }
    }

    string toString() {
        return variableName;
    }
};

class JSBinaryOperator : public JSExpression {
    string op;
    JSExpression *lhs;
    JSExpression *rhs;

public:
    JSBinaryOperator(string op, JSExpression *lhs, JSExpression *rhs): op(op), lhs(lhs), rhs(rhs) {}

    JSValue *evaluate(Scope *scope) {
        JSValue *lhsValue = lhs->evaluate(scope);
        JSValue *rhsValue = rhs->evaluate(scope);
        return lhsValue->operate(op, rhsValue);
    }

    string toString() {
        return "(" + lhs->toString() + " " + op + " " + rhs->toString() + ")";
    }
};

class JSFunctionCall : public JSExpression {
    JSExpression *lhs;
    vector<JSExpression *>rhs;

public:
    JSFunctionCall(JSExpression *lhs, vector<JSExpression *>rhs): lhs(lhs), rhs(rhs) {}

    JSValue *evaluate(Scope *scope) {
        JSValue *lhsValue = lhs->evaluate(scope);
        vector<JSValue *> args;
        for (JSExpression *x: rhs) {
            args.push_back(x->evaluate(scope));
        }
        return lhsValue->call(args);
    }

    string toString() {
        string output = lhs->toString() + "(";

        for (JSExpression *x: rhs) {
            output += x->toString() + ", ";
        }

        return output + ")";
    }
};

class JSAssignmentExpression : public JSExpression {
    string lhs;
    JSExpression * rhs;

public:
    JSAssignmentExpression(string lhs, JSExpression *rhs): lhs(lhs), rhs(rhs) { }

    JSValue *evaluate(Scope *scope) {
        JSValue *result = rhs->evaluate(scope);
        scope->setValue(lhs, result);
        return result;
    }

    string toString() {
        return lhs + " = " + rhs->toString();
    }
};

class JSStatement {
    virtual void run(Scope *scope) = 0;

public:
    void runStatement(Scope *scope) {
        run(scope);
    }

    virtual string toString() = 0;
};

class JSExpressionStatement : public JSStatement {
    JSExpression * expression;

    void run(Scope *scope) {
        expression->evaluate(scope);
    }

public:
    JSExpressionStatement(JSExpression *expression): expression(expression) { }

    string toString() {
        return expression->toString();
    }
};

class JSWhileStatement : public JSStatement {
    JSExpression *condition;
    vector<JSStatement *> body;

    void run(Scope *scope) {
        while (condition->evaluate(scope)->isTruthy()) {
            for (JSStatement *x: body) {
                x->runStatement(scope);
            }
        }
    }

public:
    JSWhileStatement(JSExpression *condition, vector<JSStatement *> body): condition(condition), body(body) { }

    string toString() {
        string output = "while (" + condition->toString() + ") {\n";
        for (JSStatement *x: body) {
            output += x->toString() + "\n";
        }

        return output + "}";
    }
};

class JSFunctionBody {
    vector<JSStatement *> code;

public:
    JSFunctionBody(vector<JSStatement *> code): code(code) {}

    void run(Scope *scope) {
        for (JSStatement *statement : code) {
            statement->runStatement(scope);
        }
    }

    string toString() {
        string output = "{\n";
        for (JSStatement *x: code) {
            output += x->toString() + "\n";
        }

        return output + "}\n";
    }
};

int main() {
    Scope *scope = new Scope();
    scope->setValue("log", new JSExternalFunction(&log));

    JSStatement * firstLine = (JSStatement *) new JSExpressionStatement(new JSAssignmentExpression("x",
         (JSExpression *) new JSValueExpression(new JSNumber(2))));

    JSStatement * increment = (JSStatement *) new JSExpressionStatement(
            new JSAssignmentExpression(
                    "x",
                    new JSBinaryOperator("+", new JSValueExpression(new JSNumber(3)), new JSVariableExpression("x"))));


    JSExpression * condition = new JSBinaryOperator("<", new JSVariableExpression("x"), new JSValueExpression(new JSNumber(10)));

    vector<JSExpression *> logArgs;
    logArgs.push_back(new JSVariableExpression("x"));

    JSStatement * lastLine = (JSStatement *) new JSExpressionStatement(
            new JSFunctionCall(
                    new JSVariableExpression("log"),
                    logArgs)
    );

    vector<JSStatement *> body;
    body.push_back(increment);
    body.push_back(lastLine);

    JSWhileStatement * whileStatement = new JSWhileStatement(condition, body);

    vector<JSStatement *> programBody;
    programBody.push_back(firstLine);
    programBody.push_back((JSStatement *) whileStatement);

    JSFunctionBody * program = new JSFunctionBody(programBody);

    cout << program->toString();

    program->run(scope);

    cout << scope->toString() << endl;
    return 0;
}
