#include <iostream>
#include "property.h"

using namespace std;

int main(int argc, char **argv)
{    
    try {
        Property p;

        p.add("misc", "");
        p.get("misc").add("foo", "hello");
        p.get("misc").add("bar", "world");

        cout << p.get("misc.foo").value() << " " << p.get("misc.bar").value() << endl;
    }
    catch(exception err) {
        cout << "property get test failed: " << err.what();
        return -1;
    }
    bool bOk = false;
    try {
        Property p;
        p.add("foo", "foo");
        cout << p.get("bar").value() << endl; 
    }
    catch(exception err) {
        bOk = true;;
    }
    
    try {
        Property p;
        load_property_from_xml(p, "test.xml");
        cout << p << endl;
        if(p.get("hello.world").value() != "true")
            bOk = false;
        
        cout << "properties that match \"hello.list.it*\": ";
        list<Property *> sel = p.Select("hello.list.it*");
        for(list<Property*>::iterator iter=sel.begin(); iter!=sel.end(); ++iter)
            cout << (*iter)->value() << " ";
        cout << endl;
        if(sel.size() != 2) bOk = false;        
    }
    catch(exception err) {
        bOk = false;
    }
    
    if(!bOk) {
        cout << "throw of not available key failed" << endl;
        return -1;
    }
    cout << "all tests succesful\n";
    return 0;
}

