/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef JSCSSStyleDeclaration_h
#define JSCSSStyleDeclaration_h

#include "JSDOMBinding.h"
#include <kjs/JSGlobalObject.h>
#include <kjs/ObjectPrototype.h>

namespace WebCore {

class CSSStyleDeclaration;

class JSCSSStyleDeclaration : public DOMObject {
    typedef DOMObject Base;
public:
    JSCSSStyleDeclaration(KJS::JSObject* prototype, CSSStyleDeclaration*);
    virtual ~JSCSSStyleDeclaration();
    virtual bool getOwnPropertySlot(KJS::ExecState*, const KJS::Identifier& propertyName, KJS::PropertySlot&);
    virtual bool getOwnPropertySlot(KJS::ExecState*, unsigned propertyName, KJS::PropertySlot&);
    KJS::JSValue* getValueProperty(KJS::ExecState*, int token) const;
    virtual void put(KJS::ExecState*, const KJS::Identifier& propertyName, KJS::JSValue*);
    void putValueProperty(KJS::ExecState*, int, KJS::JSValue*);
    bool customPut(KJS::ExecState*, const KJS::Identifier&, KJS::JSValue*);
    virtual const KJS::ClassInfo* classInfo() const { return &s_info; }
    static const KJS::ClassInfo s_info;

    virtual void getPropertyNames(KJS::ExecState*, KJS::PropertyNameArray&);
    static KJS::JSValue* getConstructor(KJS::ExecState*);
    enum {
        // Attributes
        CssTextAttrNum, LengthAttrNum, ParentRuleAttrNum, 

        // The Constructor Attribute
        ConstructorAttrNum
    };
    CSSStyleDeclaration* impl() const { return m_impl.get(); }

private:
    RefPtr<CSSStyleDeclaration> m_impl;
    static KJS::JSValue* indexGetter(KJS::ExecState*, const KJS::Identifier&, const KJS::PropertySlot&);
private:
    static bool canGetItemsForName(KJS::ExecState*, CSSStyleDeclaration*, const KJS::Identifier&);
    static KJS::JSValue* nameGetter(KJS::ExecState*, const KJS::Identifier&, const KJS::PropertySlot&);
};

KJS::JSValue* toJS(KJS::ExecState*, CSSStyleDeclaration*);
CSSStyleDeclaration* toCSSStyleDeclaration(KJS::JSValue*);

class JSCSSStyleDeclarationPrototype : public KJS::JSObject {
public:
    static KJS::JSObject* self(KJS::ExecState*);
    virtual const KJS::ClassInfo* classInfo() const { return &s_info; }
    static const KJS::ClassInfo s_info;
    bool getOwnPropertySlot(KJS::ExecState*, const KJS::Identifier&, KJS::PropertySlot&);
    JSCSSStyleDeclarationPrototype(KJS::ExecState* exec)
        : KJS::JSObject(exec->lexicalGlobalObject()->objectPrototype()) { }
};

// Functions

KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionGetPropertyValue(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionGetPropertyCSSValue(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionRemoveProperty(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionGetPropertyPriority(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionSetProperty(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionItem(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionGetPropertyShorthand(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
KJS::JSValue* jsCSSStyleDeclarationPrototypeFunctionIsPropertyImplicit(KJS::ExecState*, KJS::JSObject*, KJS::JSValue*, const KJS::ArgList&);
} // namespace WebCore

#endif