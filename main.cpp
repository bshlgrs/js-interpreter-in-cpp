#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <math.h>

using namespace std;


enum JSType { JSNumberType, JSUndefinedType, JSBoolType, JSStringType, JSObjectType, JSExternalFunctionType, JSFunctionType, JSArrayType };

class JSValue {
public:
    static int instance_counter;
    JSValue() {
        cout << "I'M MAKING A THING" << endl;
        instance_counter ++;
    }
    virtual JSType jsType() = 0;
    virtual std::string toString() = 0;
    virtual JSValue * operate(std::string op, JSValue *other) = 0;
    virtual JSValue * get(std::string key) = 0;
    virtual JSValue * set(std::string key, JSValue *value) {
        exit(123);
    }
    virtual bool isTruthy() = 0;
};

int JSValue::instance_counter = 0;

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

typedef JSValue *(*JSExternalFunctionImplementation)(std::vector<JSValue *> args);

extern JSValue * logFunction;



class JSNumber : public JSValue {
    double value;

public:
    JSType jsType() {
        return JSNumberType;
    }

    JSNumber(double value) : value(value) {}

    string toString() {
        return to_string(value);
    }

    JSValue *operate(std::string op, JSValue *other) {
        switch (other->jsType())
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
    JSType jsType() {
        return JSUndefinedType;
    }

    JSValue *operate(std::string op, JSValue *other) {
        switch (other->jsType())
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
    JSType jsType() {
        return JSObjectType;
    }

    JSObject(std::map<string, JSValue*> fields): fields(fields) { }

    JSValue *operate(std::string op, JSValue *other) {
        switch (other->jsType())
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
    JSType jsType() {
        return JSBoolType;
    }

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
    JSExternalFunctionImplementation implementation;
public:
    JSType jsType() {
        return JSExternalFunctionType;
    }

    JSExternalFunctionImplementation getImplementation() {
        return implementation;
    };

    JSExternalFunction(JSExternalFunctionImplementation implementation): implementation(implementation) { };

    string toString() {
        return "function<something>";
    }

    JSValue *operate(std::string op, JSValue *other) {
        cout << "note: unimplemented operate: " << toString() << op << other->toString() << endl;
        return jsUndefined;
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
    JSType jsType() {
        return JSStringType;
    }

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


class Scope;

class JSStatement;
JSValue *interpretBlock(vector<JSStatement*> body, Scope *scope);

class JSFunction : public JSValue {
    std::vector<string> argNames;
    std::vector<JSStatement *> statements;
public:
    JSType jsType() {
        return JSFunctionType;
    }

    JSFunction(const vector<string> &argNames, std::vector<JSStatement *> statements) : argNames(argNames), statements(statements) { }

    std::vector<string> getArgNames() {
        return argNames;
    }

    std::vector<JSStatement *> getStatements() {
        return statements;
    }

    JSValue *operate(std::string op, JSValue *other) {
        cout << "note: unimplemented operate: " << toString() << op << other->toString() << endl;
        return jsUndefined;
    }

    string toString() {
        return "<function...>";
    }

    bool isTruthy() {
        return true;
    }

    JSValue *get(std::string key) {
        return jsUndefined;
    }
};


////// STANDARD LIBRARY


JSValue *log(std::vector<JSValue *> args) {
    for (JSValue * value : args) {
        cout << value->toString() << " ";
    }
    cout << endl;
    return jsUndefined;
}

JSExternalFunction logFunctionObject(&log);
JSValue * logFunction = &logFunctionObject;










class Scope {
    Scope *parent = NULL;
    std::map<string, JSValue*> values;

public:
    Scope(Scope *parent): parent(parent) {}

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
        if (parent == NULL || parent->getValue(name) == NULL)
            values[name] = value;
        else
            parent->setValue(name, value);
    }

    void declareValue(string name, JSValue *value) {
        // shadow if necessary
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

class JSBinOpExpr : public JSExpression {
    string op;
    JSExpression *lhs;
    JSExpression *rhs;

public:
    JSBinOpExpr(string op, JSExpression *lhs, JSExpression *rhs): op(op), lhs(lhs), rhs(rhs) {}

    JSValue *evaluate(Scope *scope) {
        if (op == "&&" || op == "||") {
            JSValue *lhsValue = lhs->evaluate(scope);
            JSValue *rhsValue = rhs->evaluate(scope);
            return lhsValue->operate(op, rhsValue);
        } else if (op == "||") {

        } else {
            JSValue *lhsValue = lhs->evaluate(scope);
            JSValue *rhsValue = rhs->evaluate(scope);
            return lhsValue->operate(op, rhsValue);
        }
    }

    string toString() {
        return "(" + lhs->toString() + " " + op + " " + rhs->toString() + ")";
    }
};

class JSFunctionCallExpression : public JSExpression {
    JSExpression *lhs;
    vector<JSExpression *>rhs;

public:
    JSFunctionCallExpression(JSExpression *lhs, vector<JSExpression *>rhs): lhs(lhs), rhs(rhs) {}

    JSValue *evaluate(Scope *scope) {
        JSValue *lhsValue = lhs->evaluate(scope);

        if (lhsValue->jsType() == JSExternalFunctionType) {
            vector<JSValue *> args;
            for (JSExpression *x: rhs) {
                args.push_back(x->evaluate(scope));
            }

            JSExternalFunctionImplementation implementation = ((JSExternalFunction *) lhsValue)->getImplementation();

            return implementation(args);
        }
        else if (lhsValue->jsType() == JSFunctionType) {
            JSFunction *functionValue = (JSFunction *) lhsValue;

            Scope *newScope = new Scope(scope);

            for (int i = 0; i < functionValue->getArgNames().size(); i++) {
                newScope->declareValue(functionValue->getArgNames()[i], rhs[i]->evaluate(scope));
            }

            JSValue *result = interpretBlock(functionValue->getStatements(), newScope);
            return (result == nullptr) ? jsUndefined : result;
        }
        else {
            cout << "trying to call " << lhsValue->toString() << ", which can't be called :(" << endl;
            exit(123);
        }
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

class JSFieldAccessExpression : public JSExpression {
    JSExpression * lhs;
    JSExpression * rhs;

public:
    JSFieldAccessExpression(JSExpression *lhs, JSExpression *rhs): lhs(lhs), rhs(rhs) { }

    JSValue *evaluate(Scope *scope) {
        JSValue *lhsResult = lhs->evaluate(scope);
        JSValue *rhsResult = rhs->evaluate(scope);
        return lhsResult->get(rhsResult->toString());
    }

    string toString() {
        return lhs->toString() + "[" + rhs->toString() + "]";
    }
};



class JSStatement {
    virtual JSValue * run(Scope *scope) = 0;

public:
    JSValue *runStatement(Scope *scope) {
        return run(scope);
    }

    virtual string toString() = 0;
};

class JSExpressionStatement : public JSStatement {
    JSExpression * expression;

    JSValue * run(Scope *scope) {
        expression->evaluate(scope);
        return NULL;
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

    JSValue * run(Scope *scope) {
        while (condition->evaluate(scope)->isTruthy()) {
            JSValue *result = interpretBlock(body, scope);
            if (result != NULL) {
                return result;
            }
        }
        return NULL;
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

class JSReturnStatement : public JSStatement {
    JSExpression *value;

    JSValue * run(Scope *scope) {
        return value->evaluate(scope);
    }

public:
    JSReturnStatement(JSExpression *value): value(value) { }

    string toString() {
        return "return " + value->toString() + ";\n";
    }
};


class JSProgram {
    vector<JSStatement *> code;

public:
    JSProgram(vector<JSStatement *> code): code(code) {}

    void run(Scope *scope) {
        interpretBlock(code, scope);
    }

    string toString() {
        string output = "{\n";
        for (JSStatement *x: code) {
            output += x->toString() + "\n";
        }

        return output + "}\n";
    }
};

JSValue *interpretBlock(vector<JSStatement *>body, Scope *scope) {
    for (JSStatement *statement : body) {
        JSValue * result = statement->runStatement(scope);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}

JSStatement *mkSetStmtFromValue(string name, JSValue *value) {
    return new JSExpressionStatement(new JSAssignmentExpression(name, new JSValueExpression(value)));
}

JSStatement *mkSetStmtFromExpr(string name, JSExpression *value) {
    return new JSExpressionStatement(new JSAssignmentExpression(name, value));
}

int runExampleProgram() {
    Scope *scope = new Scope(NULL);

    std::map<string, JSValue*> consoleMethods;
    consoleMethods["log"] = logFunction;
    JSValue *console = object(consoleMethods);

    scope->setValue("console", console);

    JSStatement * firstLine = mkSetStmtFromValue("x", number(1));

    JSStatement * increment = mkSetStmtFromExpr("x", new JSBinOpExpr("+", new JSValueExpression(number(3)), new JSVariableExpression("x")));

    JSExpression * condition = new JSBinOpExpr("<", new JSVariableExpression("x"), new JSVariableExpression("y"));

    vector<JSExpression *> logArgs;
    logArgs.push_back(new JSBinOpExpr("+", new JSValueExpression(jsString("the value is ")), new JSVariableExpression("x")));

    JSStatement * lastLine = (JSStatement *) new JSExpressionStatement(
            new JSFunctionCallExpression(
                    new JSFieldAccessExpression(new JSVariableExpression("console"), new JSValueExpression(jsString("log"))),
                    logArgs)
    );

    vector<JSStatement *> body { increment, lastLine };

    JSWhileStatement * whileStatement = new JSWhileStatement(condition, body);

    const vector<JSStatement *> programBody { firstLine, whileStatement };

    const vector<string> argNames = { "y" };

    JSFunction * mainFunction = new JSFunction(argNames, programBody);

    const vector<JSExpression *> args = { new JSValueExpression(number(10000000)) };

    JSStatement * callMain = new JSExpressionStatement(
                    new JSFunctionCallExpression(
                            new JSVariableExpression("f"), args
                            ));

    vector<JSStatement *> programCode = { mkSetStmtFromValue("f", mainFunction), callMain };

    JSProgram * program = new JSProgram(programCode);

    cout << program->toString();

    program->run(scope);

    cout << JSValue::instance_counter << endl;

    cout << scope->toString() << endl;
    return 0;
}

int main () {
    runExampleProgram();
    return 0;
}
