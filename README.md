<h1> Experimento de paralelización de conteo de palabras de distintos libros implementado en lenguaje de programación C++ y el estándar MPI </h1>

<h2> Instituto Tecnológico Autónomo de México </h2>

<h2> Cómputo Paralelo y en la Nube </h2>



<h3> Profesor </h3>

<h5> Dr. José Octavio Gutiérrez García </h5>


<h3> Integrantes </h3>

<h5> Diego Hernández Delgado - 176262 </h5>

<h5> Isaías Jesús García Moreno - 179474 </h5>



<h2> Introducción </h2>
 
El presente proyecto consiste en la comparación entre la eficiencia de la implementación serial y la implementación paralela de un algoritmo de conteo de palabras conocido como Bolsa de palabras. 


<h2> Objetivos </h2>

- Implementar el algoritmo Bolsa de palabras de forma serial en lenguaje de programación C++, medir su tiempo de ejecución y almacenar los resultados.

- Implementar el algoritmo Bolsa de palabras de forma paralela en lenguaje de programación C++ y el estándar MPI, medir su tiempo de ejecución y almacenar los resultados.

- Cada prueba debe ser ejecutada diez veces y se debe calcular el promedio de los tiempos de ejecución.

- Comparar la eficiencia de la implementación serial y la implementación paralela del algoritmo de Bolsa de palabras, mediante la obtención de la gráfica de speedups.

- Realizar un análisis de resultados y determinar las conclusiones correspondientes.

<h2> Estructura del proyecto </h2>

- .vscode/
    * c_cpp_properties.json
    * settings.json
    * tasks.json

- Books/
    * dickens_a_christmas_carol.txt
    * dickens_a_tale_of_two_cities.txt
    * dickens_oliver_twist.txt
    * shakespeare_hamlet.txt
    * shakespeare_romeo_juliet.txt
    * shakespeare_the_merchant_of_venice.txt
- computo_paralelo_bolsa_de_palabras.ipynb
- parallel_output.csv
- Parallel_Words_Bag.cpp
- Parallel_Words_Bag.exe
- README.md
- run_parallel.bat
- run_serial.bat
- serial_output.csv
- Serial_Words_Bag.cpp
- Serial_Words_Bag.exe
- vocabulario.csv


<h2> Ejecución del experimento </h2>

Para la implementación del algoritmo de Bolsa de palabras en lenguaje de programación C++ se utilizó la biblioteca de MPI para la paralelización del algoritmo.

Para ejecutar el proceso completo del experimento, se debe ejcutar el archivo **run_serial.bat** y, posteriormente, se debe ejecutar el archivo **run_parallel.bat**. 


Argumentos:

- El número de procesos corresponde con el número de libros a ejecutar para que cada proceso cuente las palabras de un libro en particular. En este caso el número de procesos es 6.

- El número de puntos se itera entre los siguientes valores: 100, 100000, 200000, 300000, 400000, 600000, 800000, 1000000


<h2> Explicación detallada de la implementación </h2>

--


<h2> Estrategia de paralelización </h2>

--


<h2> Instrucciones de ejecución </h2>

--

<h2> Interpretación y análisis de resultados </h2>

<p> El speedup observado en la implementación paralela debería ser, en principio, directamente proporcional al número de cores utilizados. 
</p>

<h3> Gráficas del Speed up </h3>

![Speed up 100](./Images/speed_up_100.png "Title")

![Speed up 100000](./Images/speed_up_100000.png "Title")


<h2> Conclusiones </h2>

<p> El speed-up es evidente porque la división el trabajo de forma paralela reduce el tiempo de ejecución. </p>




<h2> Descripción del equipo donde se ejecutaron las pruebas</h2>

<h3> Hardware </h3>

- Computadora: Laptop Huawei Matebook 14
- Procesador: AMD Ryzen 5 3600 6-Core Processor
- Memoria RAM: 16 GB
- Disco duro: SSD 1 TB WD Blue SN550 NVMe M.2 2280
- Cores físicos: 6
- Cores lógicos: 12
- Velocidad del procesador: 3.6 GHz
- Velocidad de la memoria RAM: 3200 MHz

<h3> Software </h3>

- Sistema Operativo: Windows 11
- IDE: Visual Studio Code
- Lenguajes de programación: C++, Bash, Python
- Biblioteca de paralelización: MPI
- Compilador: g++ 
- Versión de C++: C++17
- Versión de Python: 3.8.5


<h2> Referencias </h2>

- [MPI](https://www.mpi-forum.org/docs/)

- [C++](https://docs.python.org/3/howto/regex.html)

- [G++](https://www.geeksforgeeks.org/compiling-with-g-plus-plus/)

- [Python](https://www.python.org/)

- [Bash](https://www.gnu.org/software/bash/)

- [Google Colab](https://colab.research.google.com/notebooks/intro.ipynb?authuser=1)

- Robey, Robert, and Yuliana Zamora. Parallel and High Performance Computing. Simon and
Schuster, 2021.

- Tuomanen, Brian. Hands-On GPU Programming with Python and CUDA: Explore high-
performance parallel computing with CUDA. Packt Publishing Ltd, 2018.

- Wilt, Nicholas. The CUDA handbook: A comprehensive guide to gpu programming. Pearson
Education, 2013.