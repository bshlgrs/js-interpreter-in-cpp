#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <math.h>
#include "jsvalue.h"

using namespace std;



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

class JSProgram {
    vector<JSStatement *> code;

public:
    JSProgram(vector<JSStatement *> code): code(code) {}

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

    std::map<string, JSValue*> consoleMethods;
    consoleMethods["log"] = logFunction;
    JSValue *console = object(consoleMethods);

    scope->setValue("console", console);

    JSStatement * firstLine = (JSStatement *) new JSExpressionStatement(new JSAssignmentExpression("x",
         (JSExpression *) new JSValueExpression(number(2))));

    JSStatement * increment = (JSStatement *) new JSExpressionStatement(
            new JSAssignmentExpression(
                    "x",
                    new JSBinaryOperator("+", new JSValueExpression(number(3)), new JSVariableExpression("x"))));


    JSExpression * condition = new JSBinaryOperator("<", new JSVariableExpression("x"), new JSValueExpression(number(10)));

    vector<JSExpression *> logArgs;
    logArgs.push_back(new JSVariableExpression("x"));

    JSStatement * lastLine = (JSStatement *) new JSExpressionStatement(
            new JSFunctionCall(
                    new JSFieldAccessExpression(new JSVariableExpression("console"), new JSValueExpression(jsString("log"))),
                    logArgs)
    );

    vector<JSStatement *> body;
    body.push_back(increment);
    body.push_back(lastLine);

    JSWhileStatement * whileStatement = new JSWhileStatement(condition, body);

    vector<JSStatement *> programBody;
    programBody.push_back(firstLine);
    programBody.push_back((JSStatement *) whileStatement);

    JSProgram * program = new JSProgram(programBody);

    cout << program->toString();

    program->run(scope);

    cout << scope->toString() << endl;
    return 0;
}
