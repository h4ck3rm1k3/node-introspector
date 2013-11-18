var addon = require('./build/Release/introspector');

console.log('introspector', addon.introspector()); 
i = addon.introspector()
console.log('introspector', i.javascript()); 
console.log('introspector', i.cpp()); 
