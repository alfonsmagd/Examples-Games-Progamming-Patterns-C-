# Examples-Games-Progamming-Patterns-C-
The Intent provides a snapshot description of the pattern in terms of the problem it intends to solve.
Ejemplos de técnicas avanzadas de programación c++ con templates. 



template_facade_example.cpp
- En diferentes ocasiones, matener la funcionalidad de sw antiguo se hace tedioso y complicado, aplicar nuevas funciones de medida y adaptarlas a un sistema global es una tarea horrible, tediosa y lleva muchas horas. 
- La idea es aplicar el patrón facade(o fachada en español) mediante templates, la idea es teniendo una función Principal, actualizar los valores de diferentes tipos ( sensores, funciones,ect) a una función actual, el usuario unicamente modificaria las subfunciones sin modificar la API.
