# jaire_opencv

# Compile with
g++ -o search search.cpp `pkg-config opencv --cflags --libs`
g++ -o check_on_off check_on_off.cpp `pkg-config opencv --cflags --libs`

# Examples:

./search origenes/00000001p.jpg patrones/onoff.jpg


./check_on_off origenes/00000001p.jpg
Result: ON

./check_on_off origenes/00000005p.jpg
Result: OFF



