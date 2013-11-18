var addon = require('./build/Release/introspector');

console.log('introspector', addon.introspector); 
i = addon.introspector();
//console.log('introspector', i.javascript()); 
js = i.javascript();
console.log('JS', js); 

env = js.env();
console.log('env', env); 

context = env.context();
console.log('ctx', context); 

glbl = context.global();
console.log('glb', glbl); 

console.log('introspector cpp:', i.cpp(addon.introspector)); 

function foo(x, s) {}

console.log('introspector cpp:', i.cpp(addon.introspector.Argument_0)); 

console.log('introspector cpp:', i.cpp(foo)); 

console.log('introspector cpp:', i.cpp(foo.Argument_0)); 
