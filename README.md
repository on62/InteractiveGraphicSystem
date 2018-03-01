# Trabalho01CG

Karla Aparecida Justen
Evandro Coan

Trabalho de Computação Gráfica - UFSC - 2018.2


Para configurar a IDE Eclipse 3.18.3 ou Neon.3 Release (4.6.3) para incluir GTK:

Para C:
	1º - Clica com o botão direito no projeto, e clica em Propriedades.
	Expande as opções do C/C++ General e vai em "Paths and Symbols".
	clica no GNU C e adiciona: "/usr/include/gtk-3.0" (sem as aspas)

	2º -  Ainda nas propriedades do projeto, expande as opções do C/C++ Build e vai em "Build Variables", clica em "Add".
	Agora, coloca "Variable Name": gtkc (ou qualquer outro nome que queiras, mas vais usar depois), e em "Value" coloca o comando: `pkg-config --cflags --libs gtk+-3.0` (copia e cola o comando aqui, inclusive com os acentos).

	3º - Ainda nas opções do C/C++ Build, vai em "Settings", e em "GCC C Compiler" e em "GCC C Linker" adiciona em "Expert settings - Command line pattern o "${gtkc} por último.

Para C++:
	1º - Clica com o botão direito no projeto, e clica em Propriedades.
	Expande as opções do C/C++ General e vai em "Paths and Symbols".
	clica no GNU C++ e adiciona: "/usr/include/gtk-3.0" (sem as aspas) e adiciona "/usr/include/gtkmm-3.0".

	2º -  Ainda nas propriedades do projeto, expande as opções do C/C++ Build e vai em "Build Variables", clica em "Add".
	Agora, coloca "Variable Name": gtkcmm (ou qualquer outro nome que queiras, mas vais usar depois), e em "Value" coloca o comando:  `pkg-config --cflags --libs gtk+-3.0 gtkmm-3.0` (copia e cola o comando aqui, inclusive com os acentos).

	3º - Ainda nas opções do C/C++ Build, vai em "Settings", e em "GCC C++ Compiler" e em "GCC C++ Linker" adiciona em "Expert settings - Command line pattern"  o "${gtkcmm}" (sem aspas) por último.


Links de Docmentação do GTKmm:
- https://developer.gnome.org/gtkmm/stable/group__gtkmmEnums.html#gga83727a1b6fed51566dfd5c8e58890dbaa7b88e2329efd6367522358da92a3a736
- https://developer.gnome.org/gtkmm/stable/group__Widgets.html

