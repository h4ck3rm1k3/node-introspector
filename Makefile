build2:
	npm install --nodedir=~/experiments/node-stuff/node

clean: build2
	npm clean --nodedir=~/experiments/node-stuff/node

test : build2
	node introspector.js
