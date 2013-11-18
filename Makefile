build2:
	npm install --nodedir=~/experiments/node-stuff/node

clean:
	npm clean --nodedir=~/experiments/node-stuff/node

test :
	node introspector.js
